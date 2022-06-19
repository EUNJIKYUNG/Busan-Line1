#ifndef WEBSOCKETMENUHANDLER_HPP
#define WEBSOCKETMENUHANDLER_HPP

#include "../civetweb/CivetServer.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/prettywriter.h"
#include "../rapidjson/document.h"
#include <cstring>
#include <iostream>
#include <pthread.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "../TablesDefine/TableManage.h"
#include "../OperRelated/OperManage.h"


class CWSMenuHandle
	: public CivetWebSocketHandler
{
public:
	CWSMenuHandle()
	{
		pthread_mutex_init(&mutex_lock, NULL);
		pthread_mutex_init(&mutex_connection, NULL);
	}
	virtual ~CWSMenuHandle()
	{
		pthread_mutex_destroy(&mutex_lock);
		pthread_mutex_destroy(&mutex_connection);
	}
	virtual bool handleConnection(CivetServer *server, const struct mg_connection *conn)
	{
		m_vConn.push_back(conn);
		printf("WS connected\n");
		return true;
	}

	virtual void handleReadyState(CivetServer *server, struct mg_connection *conn)
	{
		printf("WS ready\n");
		setCurrentStatus();
		update();
	}



	virtual bool handleData(CivetServer *server, struct mg_connection *conn, int bits, char *data, size_t data_len)
	{
		/*rapidjson::Document document;
		document.Parse(data, data_len);
		if (document.IsArray())
		{
			for (rapidjson::Value::ConstValueIterator itr = document.Begin(); itr != document.End(); ++itr) {
				const rapidjson::Value& attribute = *itr;
				assert(attribute.IsObject()); // each attribute is an object
				uint32_t uOrder = 0;
				uint32_t uSkip = 0;
				for (rapidjson::Value::ConstMemberIterator itr2 = attribute.MemberBegin(); itr2 != attribute.MemberEnd(); ++itr2) {
					if (!strcmp(itr2->name.GetString(), "order"))
					{
						uOrder = itr2->value.GetUint();
					}
					else if (!strcmp(itr2->name.GetString(), "skip"))
					{
						if (uOrder)
						{
							auto it = find_if(gvSkipInfo.begin(), gvSkipInfo.end(), findSkipData(uOrder));
							if (it != gvSkipInfo.end())
							{
								it->uSkip = itr2->value.GetUint();
								gqSkipInfo.push(*it);
							}
						}
					}
				}
			}
		}
		else
		{
			assert(document.HasMember("name"));
			assert(document.HasMember("value"));
			std::cout<<document["name"].GetString()<<std::endl;
			std::cout<<document["value"].GetUint()<<std::endl;
			//printf("hello = %s\n", document["name"].GetString());
		}
		StringBuffer s;
		PrettyWriter<StringBuffer> writer(s);
		writeChanges(writer);
		mg_websocket_write(conn, MG_WEBSOCKET_OPCODE_TEXT, s.GetString(), strlen(s.GetString()));*/
		return true;
	}

public:

	void pushData(const char *pStrName, const char *pStrValue)
	{
		pthread_mutex_lock(&mutex_lock);
		std::map<std::string, std::string>::iterator fit = m_mNameValue.find(pStrName);
		if (((fit != m_mNameValue.end()) && (fit->second != std::string(pStrValue))) || (fit == m_mNameValue.end()))
		{
			m_mNameValue[std::string(pStrName)] = std::string(pStrValue);
			m_mQueueValue[std::string(pStrName)] = std::string(pStrValue);
		}
		pthread_mutex_unlock(&mutex_lock);
	}

	bool setCurrentStatus()
	{
		bool bRetSize = false;
		pthread_mutex_lock(&mutex_lock);
		std::map<std::string, std::string>::iterator fit;
		for (fit = m_mNameValue.begin(); fit != m_mNameValue.end(); fit++)
		{
			m_mQueueValue[fit->first] = fit->second;
		}
		bRetSize= m_mQueueValue.size() ? true : false;
		pthread_mutex_unlock(&mutex_lock);
		return bRetSize;
	}

	void clearData()
	{
		m_mNameValue.clear();
	}


	bool update()
	{
		rapidjson::StringBuffer s;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);
		fillInfo(writer);
		if (strlen(s.GetString()))
		{
			pthread_mutex_lock(&mutex_connection);
			for (auto it = m_vConn.begin(); it != m_vConn.end(); it++)
			{
				mg_websocket_write((struct mg_connection*)(*it), MG_WEBSOCKET_OPCODE_TEXT, s.GetString(), strlen(s.GetString()));
			}
			pthread_mutex_unlock(&mutex_connection);
			return true;
		}
		return false;
	}




	template <typename Writer>
	void fillInfo(Writer& writer)
	{
		writer.StartArray();  // write "["
		//mutexlock
		pthread_mutex_lock(&mutex_lock);
		for (auto it = m_mQueueValue.begin(); it != m_mQueueValue.end(); it++)
		{
			writer.StartObject();
			writer.String("name");
			writer.String(it->first.c_str());
			writer.String("value");
			writer.String(it->second.c_str());
			writer.EndObject();
		}
		m_mQueueValue.clear();
		pthread_mutex_unlock(&mutex_lock);
		//mutex unlock
		writer.EndArray();   // End Array "]"
	}





	virtual void handleClose(CivetServer *server, const struct mg_connection *conn)
	{
		pthread_mutex_lock(&mutex_connection);
		for (auto it = m_vConn.begin(); it != m_vConn.end();)
		{
			if ((*it) == conn)
				it = m_vConn.erase(it);
			else
				it++;
		}
		pthread_mutex_unlock(&mutex_connection);
	}

	std::map<std::string, std::string> m_mNameValue;
	std::map<std::string, std::string> m_mQueueValue;
	std::vector<const struct mg_connection*> m_vConn;

	pthread_mutex_t mutex_lock;
	pthread_mutex_t mutex_connection;
};


#endif
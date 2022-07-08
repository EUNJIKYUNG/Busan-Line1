#ifndef WEBSOCKETMANUALOPERATIONHANDLER_HPP
#define WEBSOCKETMANUALOPERATIONHANDLER_HPP

#include "../civetweb/CivetServer.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/prettywriter.h"
#include "../rapidjson/document.h"
#include <cstring>
#include <string>
#include <iostream>
#include <pthread.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "../TablesDefine/TableManage.h"
#include "../OperRelated/OperManage.h"

class CWSManualOperationHandle
	: public CivetWebSocketHandler
{
public:
	CWSManualOperationHandle()
	{
		pthread_mutex_init(&mutex_lock, NULL);
		pthread_mutex_init(&mutex_connection, NULL);
	}
	virtual ~CWSManualOperationHandle()
	{
		pthread_mutex_destroy(&mutex_lock);
		pthread_mutex_destroy(&mutex_connection);
	}

	

		void setEventListString(std::string *pString)
		{
			m_pStrPtnEvent = pString;
		}

		void setStopPtnRouteString(std::string *pString)
		{
            m_pStrPtnRoutes = pString;
		}
		std::function<bool(int, std::string&)> m_tSelRoutes;
		std::function<bool(int, std::string&)> m_tRoutes;
		std::function<bool(int, std::string&)> m_tEvent;
		std::function<bool(int)> m_tConfirmEventList;
		std::function<bool(int)> m_tGoToStation;
	private:
		std::string *m_pStrPtnEvent;
		std::string *m_pStrPtnRoutes;
        std::string *m_pStrPtnRoutesSel;

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
			updateList(m_pStrPtnRoutes);
			update();
		}



		virtual bool handleData(CivetServer *server, struct mg_connection *conn, int bits, char *data, size_t data_len)
		{
			rapidjson::Document document;
			if (data_len > 2)
			{
				document.Parse(data, data_len);
				if (document.IsArray())
				{

				}
				else
				{
					if (document.HasMember("name") && document.HasMember("value"))
					{
						char chString[32];
						strcpy(chString, document["name"].GetString());
						if (!strcmp("selectedrouteList", chString))
						{
							int nValue = atoi(document["value"].GetString());
							m_pStrPtnEvent->clear();
							if (m_tEvent(nValue, (*m_pStrPtnEvent)))
							{
								updateList(m_pStrPtnEvent);
							}
							m_tGoToStation(nValue);
						
						}
						else if (!strcmp("selectedeventList", chString))
						{
							int nValue = atoi(document["value"].GetString());
							m_tConfirmEventList(nValue);
						}
					}
				}
				/*StringBuffer s;
				PrettyWriter<StringBuffer> writer(s);
				writeChanges(writer);
				mg_websocket_write(conn, MG_WEBSOCKET_OPCODE_TEXT, s.GetString(), strlen(s.GetString()));*/
				return true;
			}
			return  false;
		}

	public:

		void pushData(const char *pStrName, const char *pStrValue, bool bUpdate = false)
		{
			pthread_mutex_lock(&mutex_lock);
			std::map<std::string, std::string>::iterator fit = m_mNameValue.find(pStrName);
			if (((fit != m_mNameValue.end()) && (fit->second != std::string(pStrValue))) || (fit == m_mNameValue.end()))
			{
				m_mNameValue[std::string(pStrName)] = std::string(pStrValue);
				m_mQueueValue[std::string(pStrName)] = std::string(pStrValue);
			}
			pthread_mutex_unlock(&mutex_lock);
			if (bUpdate)
				update();
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
			bRetSize = m_mQueueValue.size() ? true : false;
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

		bool updateList(std::string *pString)
		{
			if (pString)
				if (pString->length())
				{
					pthread_mutex_lock(&mutex_connection);
					for (auto it = m_vConn.begin(); it != m_vConn.end(); it++)
					{
						mg_websocket_write((struct mg_connection*)(*it), MG_WEBSOCKET_OPCODE_TEXT, pString->c_str(), pString->length());
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

		int m_nSelectedRouteList;
		int m_nSelectedLineType;
	};


#endif
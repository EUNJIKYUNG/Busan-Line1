#ifndef WEBSOCKETDETAILEDDIAGNOSISRHANDLER_HPP
#define WEBSOCKETDETAILEDDIAGNOSISRHANDLER_HPP

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

class CWSDetailedDiagnosisHandle
	: public CivetWebSocketHandler
{
public:
	CWSDetailedDiagnosisHandle()
	{
		pthread_mutex_init(&mutex_lock, NULL);
		pthread_mutex_init(&mutex_lock_array, NULL);
		pthread_mutex_init(&mutex_connection, NULL);
	}
	virtual ~CWSDetailedDiagnosisHandle()
	{
		pthread_mutex_destroy(&mutex_lock);
		pthread_mutex_destroy(&mutex_lock_array);
		pthread_mutex_destroy(&mutex_connection);
	}
	
	std::function<bool(const char *)> m_tConfirmTrainNumber;


	virtual bool handleConnection(CivetServer *server, const struct mg_connection *conn)
	{
		const mg_request_info *pReqInfo = mg_get_request_info(conn);
		std::string s = pReqInfo->query_string;
		std::string delimiter = "=";
		size_t pos = 0;
		std::string token;
		bool bTokenFind = false;
		int nCarOrder = -1;
		int nCnt = 0;
		while ((pos = s.find(delimiter)) != std::string::npos) {
			token = s.substr(0, pos);
			switch (nCnt)
			{
			case 0:
				if (token != "car")
				{
					s.clear();
				}
				else
					bTokenFind = true;
				break;
			}
			s.erase(0, pos + delimiter.length());
		}
		if (bTokenFind)
		{
			nCarOrder = atoi(s.c_str());
			m_mConnection[nCarOrder].push_back(conn);
		}
		m_vConn.push_back(conn);
		printf("WS connected\n");
		return true;
	}

	virtual void handleReadyState(CivetServer *server, struct mg_connection *conn)
	{
		const mg_request_info *pReqInfo = mg_get_request_info(conn);
		std::string s = pReqInfo->query_string;
		std::string delimiter = "=";
		size_t pos = 0;
		std::string token;
		bool bTokenFind = false;
		int nCarOrder = -1;
		int nCnt = 0;
		while ((pos = s.find(delimiter)) != std::string::npos) {
			token = s.substr(0, pos);
			switch (nCnt)
			{
			case 0:
				if (token != "car")
				{
					s.clear();
				}
				else
					bTokenFind = true;
				break;
			}
			s.erase(0, pos + delimiter.length());
		}
		if (bTokenFind)
		{
			nCarOrder = atoi(s.c_str());
			setCurrentStatusArray(nCarOrder);
			updateArray(nCarOrder);
		}
		setCurrentStatus();
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
					if (!strcmp("confirmTrainNumber", chString))
					{
						const char *pTrainNumber = document["value"].GetString();
						m_tConfirmTrainNumber(pTrainNumber);
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

		void pushArrayData(int nOrder, const char *pStrName, const char *pStrValue)
		{

			pthread_mutex_lock(&mutex_lock_array);
			std::map<std::string, std::string>::iterator fit = m_mNameValueArray[nOrder].find(pStrName);
			if (((fit != m_mNameValueArray[nOrder].end()) && (fit->second != std::string(pStrValue))) || (fit == m_mNameValueArray[nOrder].end()))
			{
				m_mNameValueArray[nOrder][std::string(pStrName)] = std::string(pStrValue);
				m_mQueueValueArray[nOrder][std::string(pStrName)] = std::string(pStrValue);
			}
			pthread_mutex_unlock(&mutex_lock_array);
		}


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
			bRetSize = m_mQueueValue.size() ? true : false;
			pthread_mutex_unlock(&mutex_lock);
			return bRetSize;
		}


		bool setCurrentStatusArray(int nOrder)
		{
			bool bRetSize = false;
			pthread_mutex_lock(&mutex_lock_array);
			std::map<std::string, std::string>::iterator fit;
			for (fit = m_mNameValueArray[nOrder].begin(); fit != m_mNameValueArray[nOrder].end(); fit++)
			{
				m_mQueueValueArray[nOrder][fit->first] = fit->second;
			}
			bRetSize = m_mQueueValueArray[nOrder].size() ? true : false;
			pthread_mutex_unlock(&mutex_lock_array);
			return bRetSize;
		}


		void clearData()
		{
			m_mNameValue.clear();
		}

		void clearDataArray(int nOrder)
		{
			m_mNameValueArray[nOrder].clear();
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

		bool updateArray(int nArray)
		{
			rapidjson::StringBuffer s;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(s);
			fillInfoArray(writer, nArray);

			if (strlen(s.GetString()))
			{
				pthread_mutex_lock(&mutex_connection);
				auto it = m_mConnection.find(nArray);
				if (it != m_mConnection.end())
				{
					for (auto xit = it->second.begin(); xit != it->second.end(); xit++)
					{
						mg_websocket_write((struct mg_connection*)(*xit), MG_WEBSOCKET_OPCODE_TEXT, s.GetString(), strlen(s.GetString()));
					}
					pthread_mutex_unlock(&mutex_connection);
					return true;
				}
				pthread_mutex_unlock(&mutex_connection);
			}
			return false;
		}

		template <typename Writer>
		void fillInfo(Writer& writer)
		{
			//mutexlock
			pthread_mutex_lock(&mutex_lock);
			if (m_mQueueValue.size())
			{
				writer.StartArray();  // write "["
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
				writer.EndArray();   // End Array "]"
			}
			pthread_mutex_unlock(&mutex_lock);
			//mutex unlock
		}

		template <typename Writer>
		void fillInfoArray(Writer& writer, int nOrder) 
		{
			pthread_mutex_lock(&mutex_lock_array);
			if (m_mQueueValueArray[nOrder].size())
			{
				writer.StartArray();  // write "["
				for (auto it = m_mQueueValueArray[nOrder].begin(); it != m_mQueueValueArray[nOrder].end(); it++)
				{
					writer.StartObject();
					writer.String("name");
					writer.String(it->first.c_str());
					writer.String("value");
					writer.String(it->second.c_str());
					writer.EndObject();
				}
				m_mQueueValueArray[nOrder].clear();
				writer.EndArray();   // End Array "]"
			}
			pthread_mutex_unlock(&mutex_lock_array);
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

			const mg_request_info *pReqInfo = mg_get_request_info(conn);
			std::string s = pReqInfo->query_string;
			std::string delimiter = "=";
			size_t pos = 0;
			std::string token;
			bool bTokenFind = false;
			int nCarOrder = -1;
			int nCnt = 0;
			while ((pos = s.find(delimiter)) != std::string::npos) {
				token = s.substr(0, pos);
				switch (nCnt)
				{
				case 0:
					if (token != "car")
					{
						s.clear();
					}
					else
						bTokenFind = true;
					break;
				}
				s.erase(0, pos + delimiter.length());
			}
			if (bTokenFind)
			{
				nCarOrder = atoi(s.c_str());

				pthread_mutex_lock(&mutex_connection);
				auto it = m_mConnection.find(nCarOrder);
				if (it != m_mConnection.end())
				{
					for (auto xit = it->second.begin(); xit != it->second.end();)
					{
						if ((*xit) == conn)
							xit = it->second.erase(xit);
						else
							xit++;
					}
				}
				pthread_mutex_unlock(&mutex_connection);
			}
		}
		std::map<std::string, std::string> m_mNameValueArray[15];
		std::map<std::string, std::string> m_mQueueValueArray[15];

		std::map<std::string, std::string> m_mNameValue;
		std::map<std::string, std::string> m_mQueueValue;
		std::vector<const struct mg_connection*> m_vConn;
		pthread_mutex_t mutex_lock;
		pthread_mutex_t mutex_lock_array;
		pthread_mutex_t mutex_connection;
		std::map<int, std::vector<const struct mg_connection*>> m_mConnection;

	};


#endif
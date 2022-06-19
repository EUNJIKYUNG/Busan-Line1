#ifndef WEBSOCKETSIMULATIONHANDLER_HPP
#define WEBSOCKETSIMULATIONHANDLER_HPP

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

class CWSSimulationHandle
	: public CivetWebSocketHandler
{
public:
	CWSSimulationHandle()
	{
		pthread_mutex_init(&mutex_lock, NULL);
		pthread_mutex_init(&mutex_connection, NULL);
	}
	virtual ~CWSSimulationHandle()
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
		std::function<bool(int, std::string&)> m_tRoutes;
		std::function<bool(int, std::string&)> m_tEvent;
		std::function<bool(int)> m_tConfirmEventList;
		std::function<bool(int)> m_tGoToStation;
		std::function<void(bool)> m_tSetSimulationMode;
		std::function<bool()> m_tGetSimulationMode;
		std::function<void(int)> m_tSetDistance;
	private:
		std::string *m_pStrPtnEvent;
		std::string *m_pStrPtnRoutes;

		virtual bool handleConnection(CivetServer *server, const struct mg_connection *conn)
		{
			m_vConn.push_back(conn);
			//printf("[handleConnection %d]\n",__LINE__);
			return true;
		}

		virtual void handleReadyState(CivetServer *server, struct mg_connection *conn)
		{
			//printf("[handleReadyState %d]\n",__LINE__);
			m_tSetSimulationMode(true);
			setCurrentStatus();
			updateList(m_pStrPtnRoutes);
			update();
		}



		virtual bool handleData(CivetServer *server, struct mg_connection *conn, int bits, char *data, size_t data_len)
		{
			printf("[handleData %d]\n",__LINE__);
			rapidjson::Document document;
			if (data_len > 2)
			{
				document.Parse(data, data_len);
				if (document.IsArray())
				{

				}
				else
				{
					printf("[handleData %d]\n",__LINE__);
					if (document.HasMember("name") && document.HasMember("value"))
					{
						char chString[32];
						strcpy(chString, document["name"].GetString());
						if (!strcmp("selectedrouteList", chString))
						{
							printf("[handleData %d]\n",__LINE__);
							int nValue = atoi(document["value"].GetString());
							m_pStrPtnEvent->clear();
							printf("[handleData %d] %d %s \n",__LINE__,nValue,(*m_pStrPtnEvent).c_str());
							if (m_tEvent(nValue, (*m_pStrPtnEvent)))
							{
								updateList(m_pStrPtnEvent);
							}
							if (!m_tGetSimulationMode())
							{
								m_tGoToStation(nValue);
							}
						}
						else if (!strcmp("selectedeventList", chString))
						{
							int nValue = atoi(document["value"].GetString());
							printf("[handleData %d] %d\n",__LINE__,nValue);
							m_tConfirmEventList(nValue);
						}
						else if (!strcmp("stop", chString))
						{
							//m_tSetSimulationMode(false);
							m_tSetDistance(0);
							pushData("val", "mIgnoreInput=0");
							update();
							//int nValue = atoi(document["value"].GetString());
							/*if (m_nSelectedRouteList)
							{
								m_tConfirmStopPtn(m_nSelectedRouteList);
							}*/
						}
						else if (!strcmp("start", chString))
						{
							m_tSetDistance(1);
							//m_tSetSimulationMode(true);
							pushData("val", "mIgnoreInput=1");
							update();
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
			//printf("[pushData %d]\n",__LINE__);
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

		void pushNumberData(const char *pStrName, int nValue,bool bUpdate=false)
		{
			//printf("[pushNumberData %d]\n",__LINE__);
			if (m_tGetSimulationMode())
			{
				pthread_mutex_lock(&mutex_lock);
				std::string strTemp = std::to_string(nValue).c_str();
				if (strTemp.length())
				{
					std::map<std::string, std::string>::iterator fit = m_mNameValue.find(pStrName);
					if (((fit != m_mNameValue.end()) && (fit->second != strTemp)) || (fit == m_mNameValue.end()))
					{
						m_mNameValue[std::string(pStrName)] = strTemp;
						m_mQueueValue[std::string(pStrName)] = strTemp;
					}
					pthread_mutex_unlock(&mutex_lock);
					if (bUpdate)
						update();
					return;
				}
				pthread_mutex_unlock(&mutex_lock);
			}
		}

		bool setCurrentStatus()
		{
			//printf("[currentStatus %d]\n",__LINE__);
			bool bRetSize = false;
			pthread_mutex_lock(&mutex_lock);
			std::map<std::string, std::string>::iterator fit;
			for (fit = m_mNameValue.begin(); fit != m_mNameValue.end(); fit++)
			{
				m_mQueueValue[fit->first] = fit->second;
				printf("[pushNumberData %d] %s %s\n",__LINE__,fit->first.c_str(),fit->second.c_str());
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
			//printf("[update %d]\n",__LINE__);
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
			clearData();
			m_tSetSimulationMode(false);
			pushData("val", "mIgnoreInput=0");

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
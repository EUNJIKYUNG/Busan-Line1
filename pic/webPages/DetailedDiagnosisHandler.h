#pragma once
#include "../civetweb/CivetServer.h"
#include "GetCSSPages.h"


class CDetailedDiagnosisHandle : public CivetHandler
{
public:


	bool handleGet(CivetServer *server, struct mg_connection *conn)
	{
		const mg_request_info *pReqInfo = mg_get_request_info(conn);
		std::string s = pReqInfo->query_string;
		std::string delimiter = "=";


		size_t pos = 0;
		std::string token;
		bool bTokenFind = false;
		int nCarNumber = -1;
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
			nCarNumber = atoi(s.c_str());

		int nPrevPage = 0;
		int nNextPage = 0;
		char chForwardDev[64] = { 0 };
		char chRearDev[64] = { 0 };
		char chUpDev[512] = { 0 };
		char chDnDev[512] = { 0 };
		char chPrevDev[256] = { 0 };
		char chNextDev[256] = { 0 };
		char chCarNumber[128] = { 0 };
		switch (nCarNumber)
		{
		case 0:
			nPrevPage = 7;
			nNextPage = 1;
			strcpy(chForwardDev, "<p class=\"frontDev\" id=\"D_FTNDD\">FTNDD</p>\r\n");
			strcpy(chRearDev, "<p class=\"rearDev\" id=\"D_PII_2\">PII2</p>\r\n");
			strcpy(chUpDev, "                    <div class=\"sideDevice\">  \r\n<p class=\"leftIdd1\" id=\"D_IDD1_R1\">IDD1</p>\r\n<p class=\"leftIdd2\" id=\"D_IDD2_R1\">IDD2</p>\r\n<p class=\"rightIdd1\" id=\"D_IDD1_R2\">IDD1</p>\r\n<p class=\"rightIdd2\" id=\"D_IDD2_R2\">IDD2</p>\r\n                    </div>\r\n");
			strcpy(chDnDev, "                    <div class=\"sideDevice\">  \r\n<p class=\"leftIdd1\" id=\"D_IDD1_L1\">IDD1</p>\r\n<p class=\"leftIdd2\" id=\"D_IDD2_L1\">IDD2</p>\r\n<p class=\"rightIdd1\" id=\"D_IDD1_L2\">IDD1</p>\r\n<p class=\"rightIdd2\" id=\"D_IDD2_L2\">IDD2</p>\r\n                    </div>\r\n");
			sprintf(chPrevDev, "                        <img class=\"arrowImg\" src=\"/app/image/prevCar.png\" alt=\"\xec\x99\xbc\xec\xaa\xbd"" \xed\x99\x94\xec\x82\xb4\xed\x91\x9c""(\xec\x9d\xb4\xec\xa0\x84"" \xec\x97\xb4\xec\xb0\xa8\xeb\xa1\x9c"" \xeb\x84\x98\xea\xb9\x80"")\" onclick=\"location.href='detailedDiagnosis?car=%d'\">\r\n", nPrevPage);
			sprintf(chNextDev, "                        <img class=\"arrowImg\" src=\"/app/image/nextCar.png\" alt=\"\xec\x98\xa4\xeb\xa5\xb8\xec\xaa\xbd"" \xed\x99\x94\xec\x82\xb4\xed\x91\x9c""(\xeb\x8b\xa4\xec\x9d\x8c"" \xec\x97\xb4\xec\xb0\xa8\xeb\xa1\x9c"" \xeb\x84\x98\xea\xb9\x80"")\" onclick=\"location.href='detailedDiagnosis?car=%d'\">\r\n", nNextPage);
			sprintf(chCarNumber,"                        <span id=\"carNum\">%d\xed\x98\xb8\xec\xb0\xa8""</span>\r\n", nCarNumber+1);
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			nPrevPage = nCarNumber - 1;
			nNextPage = nCarNumber + 1;
			strcpy(chForwardDev, "<p class=\"frontDev\" id=\"D_PII_1\">PII1</p>\r\n");
			strcpy(chRearDev, "<p class=\"rearDev\" id=\"D_PII_2\">PII2</p>\r\n");
			strcpy(chUpDev, "                    <div class=\"sideDevice\">  \r\n<p class=\"leftIdd1\" id=\"D_IDD1_R1\">IDD1</p>\r\n<p class=\"leftIdd2\" id=\"D_IDD2_R1\">IDD2</p>\r\n<p class=\"rightIdd1\" id=\"D_IDD1_R2\">IDD1</p>\r\n<p class=\"rightIdd2\" id=\"D_IDD2_R2\">IDD2</p>\r\n                    </div>\r\n");
			strcpy(chDnDev, "                    <div class=\"sideDevice\">  \r\n<p class=\"leftIdd1\" id=\"D_IDD1_L1\">IDD1</p>\r\n<p class=\"leftIdd2\" id=\"D_IDD2_L1\">IDD2</p>\r\n<p class=\"rightIdd1\" id=\"D_IDD1_L2\">IDD1</p>\r\n<p class=\"rightIdd2\" id=\"D_IDD2_L2\">IDD2</p>\r\n                    </div>\r\n");
			sprintf(chPrevDev, "                        <img class=\"arrowImg\" src=\"/app/image/prevCar.png\" alt=\"\xec\x99\xbc\xec\xaa\xbd"" \xed\x99\x94\xec\x82\xb4\xed\x91\x9c""(\xec\x9d\xb4\xec\xa0\x84"" \xec\x97\xb4\xec\xb0\xa8\xeb\xa1\x9c"" \xeb\x84\x98\xea\xb9\x80"")\" onclick=\"location.href='detailedDiagnosis?car=%d'\">\r\n", nPrevPage);
			sprintf(chNextDev, "                        <img class=\"arrowImg\" src=\"/app/image/nextCar.png\" alt=\"\xec\x98\xa4\xeb\xa5\xb8\xec\xaa\xbd"" \xed\x99\x94\xec\x82\xb4\xed\x91\x9c""(\xeb\x8b\xa4\xec\x9d\x8c"" \xec\x97\xb4\xec\xb0\xa8\xeb\xa1\x9c"" \xeb\x84\x98\xea\xb9\x80"")\" onclick=\"location.href='detailedDiagnosis?car=%d'\">\r\n", nNextPage);
			sprintf(chCarNumber, "                        <span id=\"carNum\">%d\xed\x98\xb8\xec\xb0\xa8""</span>\r\n", nCarNumber+1);
			break;
		case 7:
			nPrevPage = 6;
			nNextPage = 0;
			strcpy(chForwardDev, "<p class=\"frontDev\" id=\"D_PII_2\">PII2</p>\r\n");
			strcpy(chRearDev, "<p class=\"rearDev\" id=\"D_FTNDD\">FTNDD</p>\r\n");
			strcpy(chDnDev, "                    <div class=\"sideDevice\">  \r\n<p class=\"leftIdd1\" id=\"D_IDD1_R1\">IDD1</p>\r\n<p class=\"leftIdd2\" id=\"D_IDD2_R1\">IDD2</p>\r\n<p class=\"rightIdd1\" id=\"D_IDD1_R2\">IDD1</p>\r\n<p class=\"rightIdd2\" id=\"D_IDD2_R2\">IDD2</p>\r\n                    </div>\r\n");
			strcpy(chUpDev, "                    <div class=\"sideDevice\">  \r\n <p class=\"leftIdd1\" id=\"D_IDD1_L1\">IDD1</p>\r\n<p class=\"leftIdd2\" id=\"D_IDD2_L1\">IDD2</p>\r\n<p class=\"rightIdd1\" id=\"D_IDD1_L2\">IDD1</p>\r\n<p class=\"rightIdd2\" id=\"D_IDD2_L2\">IDD2</p>\r\n                    </div>\r\n");
			sprintf(chPrevDev, "                        <img class=\"arrowImg\" src=\"/app/image/prevCar.png\" alt=\"\xec\x99\xbc\xec\xaa\xbd"" \xed\x99\x94\xec\x82\xb4\xed\x91\x9c""(\xec\x9d\xb4\xec\xa0\x84"" \xec\x97\xb4\xec\xb0\xa8\xeb\xa1\x9c"" \xeb\x84\x98\xea\xb9\x80"")\" onclick=\"location.href='detailedDiagnosis?car=%d'\">\r\n", nPrevPage);
			sprintf(chNextDev, "                        <img class=\"arrowImg\" src=\"/app/image/nextCar.png\" alt=\"\xec\x98\xa4\xeb\xa5\xb8\xec\xaa\xbd"" \xed\x99\x94\xec\x82\xb4\xed\x91\x9c""(\xeb\x8b\xa4\xec\x9d\x8c"" \xec\x97\xb4\xec\xb0\xa8\xeb\xa1\x9c"" \xeb\x84\x98\xea\xb9\x80"")\" onclick=\"location.href='detailedDiagnosis?car=%d'\">\r\n", nNextPage);
			sprintf(chCarNumber, "                        <span id=\"carNum\">0\xed\x98\xb8\xec\xb0\xa8""</span>\r\n");
			break;
		}

		mg_printf(conn,
			"HTTP/1.1 200 OK\r\nContent-Type: "
			"text/html\r\nConnection: close\r\n\r\n");
		mg_printf(conn,
			"<html lang=\"ko\">\r\n"
			"    <head>\r\n"
			"        <meta charset=\"utf-8\">\r\n"
			"<style>");
		mg_printf(conn, GetCSSPages::pPISC);
		mg_printf(conn, GetCSSPages::pDetailDiag);
		mg_printf(conn, "\r\n</style>");
		mg_printf(conn,
			"        <title>PISC-\xec\x9e\x90\xea\xb8\xb0\xec\xa7\x84\xeb\x8b\xa8""</title>\r\n"
			"    </head>\r\n"
			"\r\n"
			"		<body onload=\"load()\" oncontextmenu='return false' ondragstart='return false' onselectstart='return false'>\r\n"
			"        <div id=\"container\">\r\n"
			"            <header>\r\n"
			"                <img id=\"logo\" src=\"/app/image/Logo.png\" alt=\"\xec\x84\x9c\xec\x9a\xb8\xea\xb5\x90\xed\x86\xb5\xea\xb3\xb5\xec\x82\xac"" \xeb\xa1\x9c\xea\xb3\xa0""\" >\r\n"
			"            <span id=\"nowTime\">\r\n"
			"                <p id=\"txtDate\">2020. 02. 04</p>\r\n"
			"                <p id=\"txtTime\">AM 10 : 25 : 30</p>\r\n"
			"            </span>\r\n"
			"                <span id=\"headerTitle\">\xec\x9e\x90\xea\xb8\xb0\xec\xa7\x84\xeb\x8b\xa8""</span>\r\n"
			"                <hr id=\"headerLine\">\r\n"
			"            </header>\r\n"
			"            <section>\r\n"
			"                <div id=\"stat\">\r\n"
			"                    <p class=\"okColor\">OK</p>\r\n"
			"                    <p class=\"ngColor\">NG</p>\r\n"
			"                </div>\r\n"
			"                <div id=\"detailDiag\">\r\n");
		mg_printf(conn, chUpDev);
		mg_printf(conn, "                    <div class=\"centerDevice\">\r\n");
		mg_printf(conn, chForwardDev);
		mg_printf(conn, chPrevDev);
		mg_printf(conn, chCarNumber);
		mg_printf(conn, chNextDev);
		mg_printf(conn, chRearDev);
		mg_printf(conn, "                    </div> \r\n");
		mg_printf(conn, chDnDev);
		mg_printf(conn,
			"                </div>\r\n"
			"            </section>\r\n"
			"            <footer>\r\n"
			"                <button class=\"roundFootBtn\" id=\"prevBtn\" onclick=\"location.href='/selfDiagnosis'\">\xec\x9d\xb4\xec\xa0\x84""</button>\r\n"
			"            </footer>\r\n"
			"\r\n"
			"        </div>\r\n"
			"        <script>\r\n"
			"            function parseJson(srcData) {\r\n"
			"                    for (var i = 0; i < srcData.length; i++) {\r\n"
			"                        var temp,selTable;\r\n"
			"                        var ctrlType = srcData[i].name.substr(0, 3);\r\n"
			"                        switch(ctrlType){\r\n"
			"                            case \"txt\":\r\n"
			"								 temp = srcData[i].name.substr(3, srcData[i].length); \r\n"
			"                                temp = document.getElementById(temp);\r\n"
			"                                temp.innerText = srcData[i].value;\r\n"
			"                                break;\r\n"
			"                            case \"ind\":\r\n"
			"								 temp = srcData[i].name.substr(3, srcData[i].length); \r\n"
			"                                temp = document.getElementById(temp);\r\n"
			"								 if(temp!=null)\r\n"
			"									temp.style = srcData[i].value;\r\n"
			"                                break;\r\n"
			"                            case \"grp\":\r\n"
			"                                break;\r\n"
			"                            case \"val\":\r\n"
			"                                eval(srcData[i].value);\r\n"
			"                                break;\r\n"
			"                            default:\r\n"
			"                                break;\r\n"
			"                        } \r\n"
			"                    }\r\n"
			"                }\r\n"
			"            function load() {\r\n"
			"                var wsproto = (location.protocol === 'https:') ? 'wss:' : 'ws:';\r\n");
		mg_printf(conn,
			"                connection = new WebSocket(wsproto + '//' + window.location.host + '/wsDD?%s');\r\n", pReqInfo->query_string);
		mg_printf(conn,
			"                connection.onmessage = function (e) {\r\n"
			"                    var parseData = JSON.parse(e.data);\r\n"
			"                    var rV = parseJson(parseData);\r\n"
			"                }\r\n"
			"                connection.onerror = function (error) {\r\n"
			"                    alert('WebSocket error');\r\n"
			"                    connection.close();\r\n"
			"                }\r\n"
			"            }\r\n"
			"        </script>\r\n"
			"    </body>\r\n"
			"\r\n"
			"</html>\r\n");
		return true;
	}
	/*bool handleGet(CivetServer *server, struct mg_connection *conn)
	{
		//UTF-8 Conversion
		//mg_connection *pComm = (mg_connection*)conn;
		//mg_header *pHeader=(mg_header*)&mg_get_request_info(conn)->http_headers[0];

		//redirection!!
		//mg_send_http_redirect(conn, , 301);
		const mg_request_info *pReqInfo=mg_get_request_info(conn);
		std::string s=pReqInfo->query_string;
		std::string delimiter = "=";

		
		size_t pos = 0;
		std::string token;
		bool bTokenFind = false;
		int nCarNumber = -1;
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
			nCarNumber = atoi(s.c_str());

		int nPrevPage = 0;
		int nNextPage = 0;
		char chForwardDev[64] = {0};
		char chRearDev[64] = { 0 };
		char chUpDev[256] = { 0 };
		char chDnDev[256] = { 0 };
		switch (nCarNumber)
		{
		case 1:
			nPrevPage = 8;
			nNextPage = 2;
			strcpy(chForwardDev,"<p id=\"frontDev\" >FTNDD</p>\r\n");
			strcpy(chRearDev,"<p id=\"rearDev\">PII2</p>\r\n");
			strcpy(chUpDev, "<p class=\"leftIdd1\" id=\"D_IDD1_R1\">IDD1</p>\r\n<p class=\"leftIdd2\" id=\"D_IDD2_R1\">IDD2</p>\r\n<p class=\"rightIdd1\" id=\"D_IDD1_R2\">IDD1</p>\r\n<p class=\"rightIdd2\" id=\"D_IDD2_R2\">IDD2</p>\r\n");
			strcpy(chDnDev, "<p class=\"leftIdd1\" id=\"D_IDD1_L1\">IDD1</p>\r\n<p class=\"leftIdd2\" id=\"D_IDD2_L1\">IDD2</p>\r\n<p class=\"rightIdd1\" id=\"D_IDD1_L2\">IDD1</p>\r\n<p class=\"rightIdd2\" id=\"D_IDD2_L2\">IDD2</p>\r\n");





			break;
		case 2:
		case 3:
		case 4:
		case 5:      
		case 6:
		case 7:
			nPrevPage = nCarNumber-1;
			nNextPage = nCarNumber+1;
			strcpy(chForwardDev, "\t\t\t\t\t\t<p id=\"frontDev\"> PII1 </p>\r\n\r\n");
			strcpy(chRearDev, "\t\t\t\t\t\t<p id=\"rearDev\"> PII2 </p>\r\n\r\n");
			strcpy(chUpDev, "<p class=\"leftIdd1\" id=\"D_IDD1_R1\">IDD1</p>\r\n<p class=\"leftIdd2\" id=\"D_IDD2_R1\">IDD2</p>\r\n<p class=\"rightIdd1\" id=\"D_IDD1_R2\">IDD1</p>\r\n<p class=\"rightIdd2\" id=\"D_IDD2_R2\">IDD2</p>\r\n");
			strcpy(chDnDev, "<p class=\"leftIdd1\" id=\"D_IDD1_L1\">IDD1</p>\r\n<p class=\"leftIdd2\" id=\"D_IDD2_L1\">IDD2</p>\r\n<p class=\"rightIdd1\" id=\"D_IDD1_L2\">IDD1</p>\r\n<p class=\"rightIdd2\" id=\"D_IDD2_L2\">IDD2</p>\r\n");

			break;
		case 8:
			nPrevPage = 7;
			nNextPage = 1;
			//strcpy(chForwardDev,"<p id=\"D_PII_1\">PII2</p>\r\n");
			//strcpy(chRearDev,"<p id=\"D_FTNDD\">FTNDD</p>\r\n");

			//strcpy(chForwardDev, "<p class=\"frontDev\" id=\"D_PII_2\">PII2</p>\r\n");
			//strcpy(chRearDev, "<p class=\"rearDev\" id=\"D_FTNDD\">FTNDD</p>\r\n");
			strcpy(chDnDev, "<p class=\"leftIdd1\" id=\"D_IDD1_R1\">IDD1</p>\r\n<p class=\"leftIdd2\" id=\"D_IDD2_R1\">IDD2</p>\r\n<p class=\"rightIdd1\" id=\"D_IDD1_R2\">IDD1</p>\r\n<p class=\"rightIdd2\" id=\"D_IDD2_R2\">IDD2</p>\r\n");
			strcpy(chUpDev, "<p class=\"leftIdd1\" id=\"D_IDD1_L1\">IDD1</p>\r\n<p class=\"leftIdd2\" id=\"D_IDD2_L1\">IDD2</p>\r\n<p class=\"rightIdd1\" id=\"D_IDD1_L2\">IDD1</p>\r\n<p class=\"rightIdd2\" id=\"D_IDD2_L2\">IDD2</p>\r\n");

			break;
		}


		mg_printf(conn,
			"HTTP/1.1 200 OK\r\nContent-Type: "
			"text/html\r\nConnection: close\r\n\r\n");
		mg_printf(conn,
			"<!DOCTYPE html>\r\n"
			"<html lang=\"ko\">\r\n"
			"    <head>\r\n"
			"        <meta charset=\"utf-8\">\r\n"
			"        <style>\r\n");
		mg_printf(conn, GetCSSPages::pPISC);
		mg_printf(conn, GetCSSPages::pDetailDiag);
		mg_printf(conn,
			"		</style>\r\n"
			"        <title>PISC-\xec\x9e\x90\xea\xb8\xb0\xec\xa7\x84\xeb\x8b\xa8""</title>\r\n"
			"    </head>\r\n"
			"    <body>\r\n"
			"        <div id=\"container\">\r\n"
			"\r\n"
			"            <!-- header --------------------------------------------------------------- -->\r\n"
			"            <header>\r\n"
			"                <img id=\"logo\" src=\"/app/image/Logo.png\" alt=\"\xec\x84\x9c\xec\x9a\xb8\xea\xb5\x90\xed\x86\xb5\xea\xb3\xb5\xec\x82\xac"" \xeb\xa1\x9c\xea\xb3\xa0""\" >\r\n"
			"                <span id=\"nowTime\">\r\n"
			"                    <p id=\"txtDate\">2020. 02. 04</p>\r\n"
			"                    <p id=\"txtTime\">AM 10 : 25 : 30</p>\r\n"
			"                </span>\r\n"
			"                <span id=\"headerTitle\">\xec\x9e\x90\xea\xb8\xb0\xec\xa7\x84\xeb\x8b\xa8""</span>\r\n"
			"                <hr id=\"headerLine\">\r\n"
			"            </header>\r\n"
			"\r\n"
			"            <!-- section -------------------------------------------------------------- -->\r\n"
			"            <section>\r\n"
			"                <div id=\"stat\">\r\n"
			"                    <p class=\"okColor\">OK</p> <!-- pisc.css \xed\x8c\x8c\xec\x9d\xbc\xec\x97\x90"" \xec\x83\x89\xec\x83\x81"" \xec\xa0\x95\xec\x9d\x98"" -->\r\n"
			"                    <p class=\"ngColor\">NG</p>\r\n"
			"                </div>\r\n");
		mg_printf(conn,
			"                <div id=\"detailDiag\"\">\r\n"
			"                    <div class=\"sideDevice\">\r\n");
		mg_printf(conn, chUpDev);
		mg_printf(conn,"                    </div>\r\n");
		mg_printf(conn,
			"                    <div id=\"centerDevice\">\r\n");
		mg_printf(conn, chForwardDev);

		mg_printf(conn,
			"                        <img class=\"arrowImg\" src=\"/image/prevCar.png\" alt=\"\xec\x99\xbc\xec\xaa\xbd"" \xed\x99\x94\xec\x82\xb4\xed\x91\x9c""(\xec\x9d\xb4\xec\xa0\x84"" \xec\x97\xb4\xec\xb0\xa8\xeb\xa1\x9c"" \xeb\x84\x98\xea\xb9\x80"")\" onclick=\"location.href='detailedDiagnosis?car=%d'\">\r\n", nPrevPage);
		mg_printf(conn,
			"                        <span id=\"carNum\">%d\xed\x98\xb8\xec\xb0\xa8""</span>\r\n",nCarNumber);
		mg_printf(conn,
			"                        <img class=\"arrowImg\" src=\"/image/nextCar.png\" alt=\"\xec\x98\xa4\xeb\xa5\xb8\xec\xaa\xbd"" \xed\x99\x94\xec\x82\xb4\xed\x91\x9c""(\xeb\x8b\xa4\xec\x9d\x8c"" \xec\x97\xb4\xec\xb0\xa8\xeb\xa1\x9c"" \xeb\x84\x98\xea\xb9\x80"")\" onclick=\"location.href='detailedDiagnosis?car=%d'\">\r\n", nNextPage);
		mg_printf(conn, chRearDev);
		mg_printf(conn, "</div>\r\n");
		
		mg_printf(conn,"                    <div class=\"sideDevice\">\r\n");
		mg_printf(conn, chDnDev);
		mg_printf(conn,"                    </div>\r\n");
		mg_printf(conn,
			"                </div>\r\n"
			"               \r\n"
			"            </section>\r\n"
			"            \r\n"
			"            <!-- footer --------------------------------------------------------------- -->\r\n"
			"            <footer>\r\n"
			"                <button class=\"roundFootBtn\" id=\"prevBtn\" onclick=\"location.href='diagnosis.html'\">\xec\x9d\xb4\xec\xa0\x84""</button>\r\n"
			"            </footer>\r\n"
			"        </div>\r\n"
			"        <script>"
			"		 </script>\r\n"
			"    </body>\r\n"
			"\r\n"
			"</html>\r\n"
		);
		return true;
	}*/

};

#pragma once
#include "../civetweb/CivetServer.h"
#include "GetCSSPages.h"


class CIndexHandle : public CivetHandler
{
public:

	bool handleGet(CivetServer *server, struct mg_connection *conn)
	{
		//UTF-8 Conversion
		//mg_connection *pComm = (mg_connection*)conn;
		//mg_header *pHeader=(mg_header*)&mg_get_request_info(conn)->http_headers[0];

		//redirection!!
		//mg_send_http_redirect(conn, , 301);

		mg_printf(conn,
			"HTTP/1.1 200 OK\r\nContent-Type: "
			"text/html\r\nConnection: close\r\n\r\n");
		mg_printf(conn,
			"<!DOCTYPE html>\r\n"
			"<html lang=\"ko\">\r\n"
			"    <head>\r\n"
			"        <meta charset=\"utf-8\">\r\n"
			"<style>");
		mg_printf(conn, GetCSSPages::pPISC);
		mg_printf(conn, GetCSSPages::pIndex);
		mg_printf(conn,
			"</style>\r\n"
			"        <script>\r\n"
			"                function load() {\r\n"
			"                    var wsproto = (location.protocol === 'https:') ? 'wss:' : 'ws:';\r\n"
			"                    connection = new WebSocket(wsproto + '//' + window.location.host + '/wsIndex');\r\n"
			"                    connection.onmessage = function (e) {\r\n"
			"                        var parseData = JSON.parse(e.data);\r\n"
			"                        var rV = parseJson(parseData);\r\n"
			"                    }\r\n"
			"                    connection.onerror = function (error) {\r\n"
			"                        alert('WebSocket error');\r\n"
			"                        connection.close();\r\n"
			"                    }\r\n"
			"                }\r\n"
			"\r\n"
			"                function parseJson(srcData)\r\n"
			"                {\r\n"
			"                    for (var i = 0; i < srcData.length; i++) {\r\n"
			"                        var temp, selTable;\r\n"
			"                        var ctrlType = srcData[i].name.substr(0, 3);\r\n"
			"                        switch (ctrlType) {\r\n"
			"                            case \"txt\":\r\n"
			"                                temp = document.getElementById(srcData[i].name);\r\n"
			"                                temp.innerText = srcData[i].value;\r\n"
			"                                break;\r\n"
			"                            case \"ind\":\r\n"
			"                                temp = document.getElementById(srcData[i].name);\r\n"
			"                                temp.style = srcData[i].value;\r\n"
			"                                break;\r\n"
			"                            case \"grp\":\r\n"
			"                                break;\r\n"
			"                            case \"val\":\r\n"
			"                                eval(srcData[i].value);\r\n"
			"                                break;\r\n"
			"                            default:\r\n"
			"                                break;\r\n"
			"                        }\r\n"
			"                    }\r\n"
			"                }\r\n"
			"        </script>\r\n"
			"        <title>PISC-MAIN</title>\r\n"
			"\r\n"
			"    </head>\r\n"
			"\r\n"
			"        <body onload=\"load()\" oncontextmenu='return false' ondragstart='return false' onselectstart='return false'>\r\n"
			"        <div id=\"container\">\r\n"
			"            <!-- header --------------------------------------------------------------- -->\r\n"
			"            <header>\r\n"
			"                <img id=\"logo\" src=\"/app/image/Logo.png\" alt=\"\xec\x84\x9c\xec\x9a\xb8\xea\xb5\x90\xed\x86\xb5\xea\xb3\xb5\xec\x82\xac"" \xeb\xa1\x9c\xea\xb3\xa0""\" >\r\n"
			"            <span id=\"nowTime\">\r\n"
			"                <p id=\"txtDate\">2020. 02. 04</p>\r\n"
			"                <p id=\"txtTime\">AM 10 : 25 : 30</p>\r\n"
			"            </span>\r\n"
			"                <span id=\"headerTitle\">\xec\x97\xb4\xec\xb0\xa8\xeb\xb2\x88\xed\x98\xb8"" : xxxx</span>\r\n"
			"                <hr id=\"headerLine\">\r\n"
			"            </header>\r\n"
			"            \r\n"
			"            <!-- aside ---------------------------------------------------------------- -->\r\n"
			"            <aside>\r\n"
			"                <p id=\"version\">Ver 1.0.0</p> <!-- \xed\x8e\x8c\xec\x9b\xa8\xec\x96\xb4"" \xeb\xb2\x84\xec\xa0\x84"" \xed\x91\x9c\xec\x8b\x9c"" -->\r\n"
			"\r\n"
			"                <table id=\"statusTable\">\r\n"
			"                    <tr> <!-- \xec\x9a\xb4\xed\x96\x89\xec\x83\x81\xed\x83\x9c"" -->\r\n"	//Operation Status
			"                        <td>\xec\x9a\xb4\xed\x96\x89\xec\x83\x81\xed\x83\x9c""</td>\r\n"	
			"                        <td>\xec\x88\x98\xeb\x8f\x99""</td>\r\n"
			"                    </tr>\r\n"
			"                    <tr> <!-- \xeb\xac\xb8\xec\x83\x81\xed\x83\x9c"" -->\r\n"		//Door Status
			"                        <td>\xeb\xac\xb8\xec\x83\x81\xed\x83\x9c""</td>\r\n"
			"                        <td>\xec\x97\xb4\xeb\xa6\xbc""</td>\r\n"
			"                    </tr>\r\n"
			"                    <tr> <!-- TCMS -->\r\n"		//TCMS Status
			"                        <td>TCMS</td>\r\n"
			"                        <td>OK</td>\r\n"
			"                    </tr>\r\n"
			"                    <tr> <!-- \xec\x97\xad\xea\xb0\x84\xea\xb1\xb0\xeb\xa6\xac"" -->\r\n"
			"                        <td>\xec\x97\xad\xea\xb0\x84\xea\xb1\xb0\xeb\xa6\xac""</td>\r\n" //Distance Between Station
			"                        <td>0000M</td>\r\n"
			"                    </tr>\r\n"
			"                    <tr> <!-- \xec\x9a\xb4\xed\x96\x89\xea\xb1\xb0\xeb\xa6\xac"" -->\r\n"
			"                        <td>\xec\x9a\xb4\xed\x96\x89\xea\xb1\xb0\xeb\xa6\xac""</td>\r\n"	//Proceeded Distance
			"                        <td>0000M</td>\r\n"
			"                    </tr>\r\n"
			"                </table>\r\n"
			"\r\n"
			"            </aside>\r\n"
			"\r\n"
			"            <!-- section -------------------------------------------------------------- -->\r\n"
			"            <section>\r\n"
			"                <p class=\"sectionTitle\">\xed\x96\x89\xec\x84\xa0""</p>\r\n"
			"                <p class=\"sectionValue\" id=\"txtLineType\">""</p>\r\n"		//LineType
			"\r\n"
			"                <p class=\"sectionTitle\">\xec\xa2\x85\xec\xb0\xa9\xec\x97\xad""</p>\r\n"
			"                <p class=\"sectionValue\" id=\"txtDestination\">""</p>\r\n"				//LastStation
			"\r\n"
			"                <p class=\"sectionTitle\">\xec\x9d\xb4\xeb\xb2\x88\xec\x97\xad""</p>\r\n"
			"                <p class=\"sectionValue\" id=\"txtNextStation\">""</p>\r\n"	//Current Station
			"\r\n"
			"            </section>\r\n"
			"            \r\n"
			"            <!-- footer --------------------------------------------------------------- -->\r\n"
			"            <footer>\r\n"
			"              <button class=\"roundFootBtn\" id=\"menuBtn\" onclick=\"location.href = 'menu'\">\xeb\xa9\x94\xeb\x89\xb4""</a></button>\r\n"
			"            </footer>\r\n"
			"\r\n"
			"        </div>\r\n"
			"    </body>\r\n"
			"\r\n"
			"</html>\r\n"
		);
		return true;
	}
};

/*
"        function load() {\r\n"
"            var wsproto = (location.protocol === 'https:') ? 'wss:' : 'ws:';\r\n"
"            connection = new WebSocket(wsproto + '//' + window.location.host + '/wsIndex');\r\n"
"            connection.onmessage = function (e) {\r\n"
"                console.log(e.data);\r\n"
"                var parseData = JSON.parse(e.data);\r\n"
"                for(var i=0;i<parseData.length;i++)\r\n"
"                {\r\n"
"                    var temp = document.getElementById(parseData[i].name);\r\n"
"                    temp.innerText = parseData[i].value;\r\n"
"                }\r\n"
"            }\r\n"
"            connection.onerror = function (error) {\r\n"
"                alert('WebSocket error');\r\n"
"                connection.close();\r\n"
"            }\r\n"
"        }\r\n"
*/


class CCSSHandle : public CivetHandler
{
public:
	bool handleGet(CivetServer *server, struct mg_connection *conn)
	{
		const struct mg_request_info *request_info = mg_get_request_info(conn);
		std::string strTemp = request_info->local_uri;
	#ifdef _WIN32
		if (strTemp.at(0) == '/')
		{
			strTemp.erase(strTemp.begin());
		}
	#endif
		mg_send_file(conn, strTemp.c_str());
		return false;
	}
};

class CImageHandle : public CivetHandler
{
public:
	bool handleGet(CivetServer *server, struct mg_connection *conn)
	{
		const struct mg_request_info *request_info = mg_get_request_info(conn);
		std::string strTemp = request_info->local_uri;
	#ifdef _WIN32
		if (strTemp.at(0) == '/')
		{
			
			strTemp.erase(strTemp.begin());
		}
	#endif
		mg_send_file(conn, strTemp.c_str());
		return false;
	
		/*http::Civetweb* server = (http::Civetweb*) (request_info ? request_info->user_data : 0);
		if (server == 0) return 0;

		DOUT0("BEGIN_REQ: uri:%s query:%s", request_info->local_uri, request_info->query_string);

		std::string filename;*/
		/*server->f
		if (server->IsFileRequested(request_info->local_uri, filename)) {
			mg_send_file(conn, filename.c_str());
			return 1;
		}*/

		return true;

	}
};
#pragma once
#include "../civetweb/CivetServer.h"
#include "GetCSSPages.h"

class CSelfDiagnosisHandle : public CivetHandler
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
			"<head>\r\n"
			"        <meta charset=\"utf-8\">\r\n"
			"        <style>\r\n");
		mg_printf(conn, GetCSSPages::pPISC);
		mg_printf(conn, GetCSSPages::pDiagnosis);
		mg_printf(conn,	
			"</style>\r\n"
			"\r\n"
			"    <script>\r\n"
			"                function load() {\r\n"
			"                    var wsproto = (location.protocol === 'https:') ? 'wss:' : 'ws:';\r\n"
			"                    connection = new WebSocket(wsproto + '//' + window.location.host + '/wsSelfDiagnosis');\r\n"
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
			"    </script>\r\n"
			"\r\n"
			"        <title>PISC-\xec\x9e\x90\xea\xb8\xb0\xec\xa7\x84\xeb\x8b\xa8""</title>\r\n"
			"    </head>\r\n"
			"\r\n"
			"	<body onload=\"load()\" oncontextmenu='return false' ondragstart='return false' onselectstart='return false'>\r\n"
			"        <div id=\"container\">\r\n"
			"\r\n"
			"            <!-- header --------------------------------------------------------------- -->\r\n"
			"            <header>\r\n"
			"                <img id=\"logo\" src=\"/app/image/Logo.png\" alt=\"\xec\x84\x9c\xec\x9a\xb8\xea\xb5\x90\xed\x86\xb5\xea\xb3\xb5\xec\x82\xac"" \xeb\xa1\x9c\xea\xb3\xa0""\" >\r\n"
			"            <span id=\"nowTime\">\r\n"
			"                <p id=\"txtDate\">2020. 02. 04</p>\r\n"
			"                <p id=\"txtTime\">AM 10 : 25 : 30</p>\r\n"
			"            </span>\r\n"
			"                <span id=\"headerTitle\">\xec\x9e\x90\xea\xb8\xb0\xec\xa7\x84\xeb\x8b\xa8""</span>\r\n"
			"                <hr id=\"headerLine\">\r\n"
			"            </header>\r\n"
			"\r\n"
			"            <!-- section -------------------------------------------------------------- -->\r\n"
			"            <section>\r\n"
			"                <div id=\"stat\">\r\n"
			"                    <p style=\"background-color:red;\">OK</p>\r\n"
			"                    <p style=\"background-color:rgb(0, 211, 28);\">NG</p>\r\n"
			"                </div>\r\n"
			"                <!-- 1~4\xed\x98\xb8\xec\xb0\xa8"" -->\r\n"
			"                <div class=\"carNumber\">\r\n"
			"                    <button id=\"CAR_1\" onclick=\"location.href='detailedDiagnosis?car=0'\">1\xed\x98\xb8\xec\xb0\xa8""</button>\r\n"
			"                    <button id=\"CAR_2\" onclick=\"location.href='detailedDiagnosis?car=1'\">2\xed\x98\xb8\xec\xb0\xa8""</button>\r\n"
			"                    <button id=\"CAR_3\" onclick=\"location.href='detailedDiagnosis?car=2'\">3\xed\x98\xb8\xec\xb0\xa8""</button>\r\n"
			"                    <button id=\"CAR_4\" onclick=\"location.href='detailedDiagnosis?car=3'\">4\xed\x98\xb8\xec\xb0\xa8""</button>\r\n"
			"                </div>\r\n"
			"\r\n"
			"                <!-- 5~0\xed\x98\xb8\xec\xb0\xa8"" -->\r\n"
			"                <div class=\"carNumber\">\r\n"
			"                    <button id=\"CAR_5\" onclick=\"location.href='detailedDiagnosis?car=4'\">5\xed\x98\xb8\xec\xb0\xa8""</button>\r\n"
			"                    <button id=\"CAR_6\" onclick=\"location.href='detailedDiagnosis?car=5'\">6\xed\x98\xb8\xec\xb0\xa8""</button>\r\n"
			"                    <button id=\"CAR_7\" onclick=\"location.href='detailedDiagnosis?car=6'\">7\xed\x98\xb8\xec\xb0\xa8""</button>\r\n"
			"                    <button id=\"CAR_0\" onclick=\"location.href='detailedDiagnosis?car=7'\">0\xed\x98\xb8\xec\xb0\xa8""</button>\r\n"
			"                </div>\r\n"
			"               \r\n"
			"            </section>\r\n"
			"            \r\n"
			"            <!-- footer --------------------------------------------------------------- -->\r\n"
			"            <footer>\r\n"
			"                <button class=\"roundFootBtn\" id=\"prevBtn\" onclick=\"location.href='menu.html'\">\xec\x9d\xb4\xec\xa0\x84""</button>\r\n"
			"            </footer>\r\n"
			"\r\n"
			"        </div>\r\n"
			"\r\n"
			"    </body>\r\n"
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

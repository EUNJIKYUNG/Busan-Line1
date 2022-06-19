#pragma once
#include "../civetweb/CivetServer.h"
#include "GetCSSPages.h"


class CMenuHandle : public CivetHandler
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
		mg_printf(conn, GetCSSPages::pMenu);
		mg_printf(conn,
			"</style>"
			"<script>"
			"                function load() {\r\n"
			"                    var wsproto = (location.protocol === 'https:') ? 'wss:' : 'ws:';\r\n"
			"                    connection = new WebSocket(wsproto + '//' + window.location.host + '/wsMenu');\r\n"
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
			"                        var temp, selTable,tempCtl,disabled;\r\n"
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
			"                            case \"dis\":\r\n"
			"                                temp=srcData[i].name.substr(3,srcData[i].length);\r\n"
			"                                tempCtl = document.getElementById(temp);\r\n"
			"								 disabled=(srcData[i].value == 'true');"
			"                                tempCtl.disabled=disabled;\r\n"
			"                                break;\r\n"
			"                            default:\r\n"
			"                                break;\r\n"
			"                        }\r\n"
			"                    }\r\n"
			"                }\r\n"
			"</script>");
		mg_printf(conn,
			"        <title>PISC-MENU</title>\r\n"
			"    </head>\r\n"
			"\r\n"
			"<body onload=\"load()\" oncontextmenu='return false' ondragstart='return false' onselectstart='return false'>\r\n"
			"        <div id=\"container\">\r\n"
			"\r\n"
			"            <!-- header --------------------------------------------------------------- -->\r\n"
			"            <header>\r\n"
			"                <img id=\"logo\" src=\"/app/image/Logo.png\" alt=\"\xec\x84\x9c\xec\x9a\xb8\xea\xb5\x90\xed\x86\xb5\xea\xb3\xb5\xec\x82\xac"" \xeb\xa1\x9c\xea\xb3\xa0""\" >\r\n"
			"            <span id=\"nowTime\">\r\n"
			"                <p id=\"txtDate\">2020. 02. 04</p>\r\n"
			"                <p id=\"txtTime\">AM 10 : 25 : 30</p>\r\n"
			"            </span>\r\n"
			"                <span id=\"headerTitle\">\xeb\xa9\x94\xeb\x89\xb4""</span>\r\n"
			"                <hr id=\"headerLine\">\r\n"
			"            </header>\r\n"
			"\r\n"
			"            <!-- section -------------------------------------------------------------- -->\r\n"
			"            <section>\r\n"
			"                <!-- \xeb\xa9\x94\xeb\x89\xb4"" 1\xec\x97\xb4"" -->\r\n"
			"                <div class=\"menu\">\r\n"
			"                    <button onclick=\"location.href='/operationModeSelect'\">\xec\x9a\xb4\xed\x96\x89\xeb\xb0\xa9\xec\x8b\x9d\xec\x84\xa4\xec\xa0\x95""</button>\r\n"
			"                    <button id=\"btnTN\" onclick=\"location.href='/trainNumber'\">\xec\x97\xb4\xec\xb0\xa8\xeb\xb2\x88\xed\x98\xb8\xec\x84\xa4\xec\xa0\x95""</button>\r\n"
			"                    <button onclick=\"location.href='/simulation'\">\xeb\xaa\xa8\xec\x9d\x98\xec\xa3\xbc\xed\x96\x89""</button>\r\n"
			//"                    <button onclick=\"location.href='/formNumber'\">\xed\x8e\xb8\xec\x84\xb1\xeb\xb2\x88\xed\x98\xb8""</button>\r\n"
			"                    <button>\xed\x8e\xb8\xec\x84\xb1\xeb\xb2\x88\xed\x98\xb8""</button>\r\n"
			"\r\n"
			"                </div>\r\n"
			"\r\n"
			"                <!-- \xeb\xa9\x94\xeb\x89\xb4"" 2\xec\x97\xb4"" -->\r\n"
			"                <div class=\"menu\">\r\n"
			"                    <button id=\"btnMO\" onclick=\"location.href='/manualOperation'\">\xec\x88\x98\xeb\x8f\x99\xec\x9a\xb4\xed\x96\x89""</button>\r\n"
			"                    <button id=\"btnDS\" onclick=\"location.href='/destination'\">\xeb\x85\xb8\xec\x84\xa0\xec\x84\xa4\xec\xa0\x95""</button>\r\n"
			"                    <button onclick=\"location.href='/selfDiagnosis'\">\xec\x9e\x90\xea\xb8\xb0\xec\xa7\x84\xeb\x8b\xa8""</button>\r\n"
			//"                    <button onclick=\"location.href='/sysInfor'\">\xec\x8b\x9c\xec\x8a\xa4\xed\x85\x9c\xec\xa0\x95\xeb\xb3\xb4""</button>\r\n"
			"                    <button>\xec\x8b\x9c\xec\x8a\xa4\xed\x85\x9c\xec\xa0\x95\xeb\xb3\xb4""</button>\r\n"
			"                </div>\r\n"
			"               \r\n"
			"            </section>\r\n"
			"            \r\n"
			"            <!-- footer --------------------------------------------------------------- -->\r\n"
			"            <footer>\r\n"
			"                <button class=\"roundFootBtn\" id=\"prevBtn\" onclick=\"location.href='/index'\">\xec\x9d\xb4\xec\xa0\x84""</button>\r\n"
			"            </footer>\r\n"
			"\r\n"
			"        </div>\r\n"
			"\r\n"
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

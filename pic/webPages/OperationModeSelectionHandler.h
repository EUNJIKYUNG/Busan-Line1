#pragma once
#include "../civetweb/CivetServer.h"
#include "GetCSSPages.h"


class COperationModeSelectionHandle : public CivetHandler
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
		mg_printf(conn, GetCSSPages::pControlType);
		mg_printf(conn,
			"</style>"
			"<script>"
			"const cCONTROL_AUTO = 0; // \xec\x9a\xb4\xed\x96\x89\xeb\xb0\xa9\xec\x8b\x9d\xec\x9d\xb4"" \xec\x9e\x90\xeb\x8f\x99""\r\n"
			"const cCONTROL_MANUAL = 1; // \xec\x9a\xb4\xed\x96\x89\xeb\xb0\xa9\xec\x8b\x9d\xec\x9d\xb4"" \xec\x88\x98\xeb\x8f\x99""\r\n"
			"\r\n"
			"// ==== variable ==============================================\r\n"
			"var mControlBtnId = document.getElementById(\"txtControl\"); // \xec\x88\x98\xeb\x8f\x99""/\xec\x9e\x90\xeb\x8f\x99"" \xeb\xb2\x84\xed\x8a\xbc""\r\n"
			"var mControlVal = 0; // \xec\x9a\xb4\xed\x96\x89\xeb\xb0\xa9\xec\x8b\x9d\xec\x9d\xb4"" \xec\x9e\x90\xeb\x8f\x99\xec\x9d\xb8\xec\xa7\x80"" \xec\x88\x98\xeb\x8f\x99\xec\x9d\xb8\xec\xa7\x80""?\r\n"
			"var mSaveFlag = false;\r\n"
			"\r\n"
			"displayControlType(); // \xec\x9a\xb4\xed\x96\x89\xeb\xb0\xa9\xec\x8b\x9d\xea\xb0\x92\xec\x97\x90"" \xeb\x94\xb0\xeb\x9d\xbc"" \xeb\xb2\x84\xed\x8a\xbc\xec\x97\x90"" \"\xec\x9e\x90\xeb\x8f\x99""\" \"\xec\x88\x98\xeb\x8f\x99""\" \xed\x91\x9c\xea\xb8\xb0""\r\n"
			"\r\n"
			"\r\n"
			"\r\n"
			"function clickControlType()\r\n"
			"{\r\n"
			"mControlBtnId=document.getElementById(\"txtControl\");\r\n"
			"    // == \xec\x9a\xb4\xed\x96\x89\xeb\xb0\xa9\xec\x8b\x9d"" \xea\xb0\x92"" \xeb\xb3\x80\xea\xb2\xbd"" -----------------------------------------\r\n"
			"    if(mControlVal === 1)     mControlVal = 0; // \xec\x88\x98\xeb\x8f\x99""->\xec\x9e\x90\xeb\x8f\x99""\r\n"
			"    else    mControlVal = 1; // \xec\x9e\x90\xeb\x8f\x99""->\xec\x88\x98\xeb\x8f\x99""\r\n"
			"\r\n"
			"    displayControlType(); // \xec\x9a\xb4\xed\x96\x89\xeb\xb0\xa9\xec\x8b\x9d\xea\xb0\x92\xec\x97\x90"" \xeb\x94\xb0\xeb\x9d\xbc"" \xeb\xb2\x84\xed\x8a\xbc\xec\x97\x90"" \"\xec\x9e\x90\xeb\x8f\x99""\" \"\xec\x88\x98\xeb\x8f\x99""\" \xed\x91\x9c\xea\xb8\xb0""\r\n"
			"\r\n"
			"    // -- save flag \xed\x95\xb4\xec\xa0\x9c"" ------------------------------------------\r\n"
			"    // \xec\x9d\xb4\xec\xa0\x84\xec\x97\x90"" \xed\x99\x95\xec\x9d\xb8"" \xeb\xb2\x84\xed\x8a\xbc\xec\x9d\x84"" \xeb\x88\x8c\xeb\x9f\xac"" \xea\xb0\x92\xec\x9d\xb4"" \xec\xa0\x80\xec\x9e\xa5\xeb\x90\x98\xec\x97\x88\xeb\x8a\x94\xeb\x8d\xb0"" \xeb\x8b\xa4\xec\x8b\x9c"" \xea\xb0\x92\xec\x9d\x84"" \xeb\xb3\x80\xea\xb2\xbd\xed\x95\x9c"" \xea\xb2\xbd\xec\x9a\xb0"" \r\n"
			"    if(mSaveFlag == true){\r\n"
			"        mSaveFlag = false;\r\n"
			"        mControlBtnId.style.background = \"rgb(8, 8, 156)\";\r\n"
			"        mControlBtnId.style.color =\"#ffffff\";\r\n"
			"    }\r\n"
			"\r\n"
			"}\r\n"
			"\r\n"
			"function displayControlType()\r\n"
			"{\r\n"
			"mControlBtnId=document.getElementById(\"txtControl\");\r\n"
            // "    console.log(\"mControlBtnId: \" + mControlBtnId.innerText)\r\n"
			"    if(mControlVal == 1){\r\n"
			"        mControlBtnId.innerText = \"\xec\x88\x98\xeb\x8f\x99""\";\r\n"
			"    }\r\n"
			"    else if(mControlVal == 0){\r\n"
			"        mControlBtnId.innerText = \"\xec\x9e\x90\xeb\x8f\x99""\";\r\n"
			"    }\r\n"
            "    else{}\r\n"
			"}\r\n"
			"function setControlType(val)\r\n"
			"{\r\n"
			"    mControlVal=val;\r\n"
			"    displayControlType();\r\n"
			"}\r\n"
			"\r\n"
			"function saveParam()\r\n"
			"{\r\n"
			"mControlBtnId=document.getElementById(\"txtControl\");\r\n"
			"    mControlBtnId.style.background = \"#ffffff\"\r\n"
			"    mControlBtnId.style.color=\"rgb(8, 8, 156)\";\r\n"
			"    mSaveFlag = true;\r\n"
			"var nValue=mControlVal;\r\n"
			"    sendCommand(\"opMode\",mControlVal);\r\n"
			"}\r\n"
			"\r\n"
			"\r\n"
			" function sendCommand(Id,val)\r\n"
			"{\r\n"
			"	var info = { name: Id, value: val };\r\n"
			"	var strInfo = JSON.stringify(info);\r\n"
			"	connection.send(strInfo);\r\n"
			"}\r\n"
			"                function load() {\r\n"
			"                    mControlBtnId=document.getElementById(\"txtControl\");\r\n"
			"                    var wsproto = (location.protocol === 'https:') ? 'wss:' : 'ws:';\r\n"
			"                    connection = new WebSocket(wsproto + '//' + window.location.host + '/wsOperationModeSelect');\r\n"
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
			"                <span id=\"nowTime\">\r\n"
			"                    <p id=\"txtDate\">2020. 02. 04</p>\r\n"
			"                    <p id=\"txtTime\">AM 10 : 25 : 30</p>\r\n"
			"                </span>\r\n"
			"\r\n"
			"                <span id=\"headerTitle\">\xec\x9a\xb4\xed\x96\x89\xeb\xb0\xa9\xec\x8b\x9d\xec\x84\xa4\xec\xa0\x95""</span>\r\n"
			"                <hr id=\"headerLine\">\r\n"
			"            </header>\r\n"
			"\r\n"
			"            <!-- section -------------------------------------------------------------- -->\r\n"
			"            <section>\r\n"
			"                <button id=\"txtControl\" onclick=\"clickControlType()\"></button> \r\n"
			"\r\n"
			"            </section>\r\n"
			"            \r\n"
			"            <!-- footer --------------------------------------------------------------- -->\r\n"
			"            <footer>\r\n"
			"                <button class=\"rectFootBtn\" id=\"cancBtn\" onclick=\"location.href='menu.html'\">\xec\x9d\xb4\xec\xa0\x84""</button>\r\n"
			"                <button class=\"rectFootBtn\" id=\"okBtn\" onclick=\"saveParam()\">\xed\x99\x95\xec\x9d\xb8""</button>\r\n"
			"                \r\n"
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


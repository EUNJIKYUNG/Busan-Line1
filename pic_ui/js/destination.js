/* -----------------------------------------------------------------------
 *
 * File : destination.js
 * Author : WJIS KEJ
 * PJ Description : 부산1호선 200량 화면설정기(PIC) HTML
 * File Description : destination 화면에 대한 javascript
 * Date : 2022.06.09 ~
 *
 * ----------------------------------------------------------------------- */

// ==== constant ==============================================
const cTABLE_LIST_NUM = 5; // 테이블 리스트의 개수
const cLINE_DOWN = 0; // 하행선
const cLINE_UP = 1; // 상행선

  
// ==== variable ==============================================
var mLineTableId = document.getElementById("lineTable"); // 노선 테이블 id
var mPaTableId = document.getElementById("paTable"); // 방송 테이블 id
var mLineBtnId = document.getElementById("txtLine"); // 상행선/하행선 버튼의 id
var mLineVal = cLINE_DOWN; // 상행선인지 하행선인지에 대한 값(0:하행선, 1:상행선)
var mLineScrollPos = 0; // 노선 리스트 테이블(mLineTableId) 스크롤 위치
var mPaScrollPos = 0; // 방송 리스트 테이블(mPaTableId) 스크롤 위치
var mSelectedLine; // 노선 리스트 테이블에 선택된 노선
var mLineData = {
	"type" : "updateall",
	"count" : 0,
	"list" : [
		{"id" : 001, "name" : "다대포해수욕장→노포"},
        {"id" : 002, "name": "신평→노포"},
		

	]

}

var mPaData = {
    "type" : "none",
    "count" : 1,
    "list" : [
        { "id": 1, "name": "다대포해수욕장→다대포항"},
        { "id": 2, "name": "다대포항→낫개"},
        { "id": 3, "name": "낫개→신장림"},
        { "id": 4, "name": "신장림→장림"},
        { "id": 5, "name": "장림→동매"},
        {"id" : 6, "name" : "동매→신평"},
        {"id" : 7, "name" : "신평→하단"},
        {"id" : 8, "name" : "하단→당리"},
        {"id" : 9, "name" : "당리→사하"},
        {"id" : 10, "name" : "사하→괴정"},
        {"id" : 11, "name" : "괴정→대티"},
        {"id" : 12, "name" : "대티→서대신"},
        {"id" : 13, "name" : "서대신→동대신"},
        {"id" : 14, "name" : "동대신→토성"},
        {"id" : 15, "name" : "토성→자갈치"},
        {"id" : 16, "name" : "자갈치→남포"},
        {"id" : 17, "name" : "남포→중앙"},
        {"id" : 18, "name" : "중앙→부산역"},
        {"id" : 19, "name" : "부산역→초량"},
        {"id" : 20, "name" : "초량→부산진"},
        {"id" : 21, "name" : "부산진→좌천"},
        {"id" : 22, "name" : "좌천→범일"},
        {"id" : 23, "name" : "범일→범내골"},
        {"id" : 24, "name" : "범내골→서면"},
        {"id" : 25, "name" : "서면→부전"},
        {"id" : 26, "name" : "부전→양정"},
        {"id" : 27, "name" : "양정→시청"},
        {"id" : 28, "name" : "시청→연산"},
        {"id" : 29, "name" : "연산→교대"},
        {"id" : 30, "name" : "교대→동래"},
        {"id" : 31, "name" : "동래→명륜"},
        {"id" : 32, "name" : "명륜→온천장"},
        {"id" : 33, "name" : "온천장→부산대"},
        {"id" : 34, "name" : "부산대→장전"},
        {"id" : 35, "name" : "장전→구서"},
        {"id" : 36, "name" : "구서→두실"},
        {"id" : 37, "name" : "두실→남산"},
        {"id" : 38, "name" : "남산→범어사"},
        {"id" : 39, "name" : "범어사→노포"},
    ]
}

/* ============================================================
 * main
 * ============================================================ */
displayLineType(); // 상행선인지 하행선인지 버튼에 표기
uploadLineTable(); // 노선 테이블 Load


/* ============================================================
 * 
 * function : clickLineType()
 * description
 * - 상행선/하행선 버튼 누르면 상행선(1) <-> 하행선(0) 변경
 * - 상행선/하행선에 따라 노선 테이블과 방송 테이블 업데이트
 * param
 * return
 * 
 * ============================================================ */
function clickLineType(){
    if(mLineVal === cLINE_UP)    mLineVal = cLINE_DOWN; // 상행선->하행선
    else    mLineVal = cLINE_UP; // 하행선->상행선

    displayLineType(); // '상행선'/'하행선' 표기
    uploadLineTable(); // 상행선, 하행선에 따른 DB Upload

}

/* ============================================================
 * 
 * function : displayLineType()
 * description
 * - 상행선/하행선을 나타내는 변수(mLineVal)에 따라 버튼에 표기
 * param
 * return
 * 
 * ============================================================ */
function displayLineType(){ 
    if(mLineVal == 1){
        mLineBtnId.innerText = "상행선";
    }
    else{
        mLineBtnId.innerText = "하행선";
    }
}

/* ============================================================
 * 
 * function : clickLineUpDn(btn)
 * description
 * - 노선 리스트 테이블의 스크롤 Up/Down
 *   -----------------------
 *  |                       |
 *  |        section1       |  <-- 현재 보여지는 위치 1
 *  |                       |
 *   -----------------------
 *  |                       |
 *  |        section2       |  <-- 현재 보여지는 위치 2
 *  |                       |
 *  |-----------------------
 *  |                       |
 *  |        section3       |  <-- 현재 보여지는 위치 3
 *  |                       |
 *   ----------------------
 * 
 *  if1) 현재 보여지는 위치가 section1
 *   - 최상단으로 위로 올릴 수 없고 밑으로는 내릴 수 있음
 *   
 *  if2) 현재 보여지는 위치가 section2
 *   - 위아래로 리스트가 있어 위아래로 올리고 내릴 수 있음
 * 
 *  if3) 현재 보여지는 위치가 section3
 *   - 최하단으로 밑으로 내릴 수 없고 위로는 올릴 수 있음
 *   - 테이블 리스트를 추가할 수 있다면 추가함(json데이터 가져옴)
 * 
 * param
 * - btn : Up/Down 버튼
 * return
 * 
 * ============================================================ */
function clickLineUpDn(btn){
    var addNum = 0;
    var tableTrLen = mLineTableId.getElementsByTagName("tr").length; // 테이블의 tr 개수
    

    // -- up 버튼        
    if(btn == 'up'){ 
        // -- 최상단(if1) ---------------------------
        if(mLineScrollPos == 0){

        }
        // -- 최상단이 아니라면(if2,if3) -------------
        else{
            // 위로 5개 이상 있다면
            if(mLineScrollPos >= cTABLE_LIST_NUM){           
                mLineScrollPos -= cTABLE_LIST_NUM; // scroll 위로 tr 5칸        
                     
            }
            // 5개 이하로 있다면
            else{
                mLineScrollPos = 0; // scroll 최상단으로
            }
            moveScroll(mLineTableId, mLineScrollPos); // 스크롤 이동 
        }
    }
                
    // -- down 버튼
    else{ 
        // 밑으로 남아있는(스크롤 이동할 수 있는) 데이터의 개수
        var resDataLen = (tableTrLen-mLineScrollPos-cTABLE_LIST_NUM);
        
        // -- 밑으로 데이터가 5개 이상 남아있는 경우
        if(resDataLen >= cTABLE_LIST_NUM){
            mLineScrollPos += cTABLE_LIST_NUM;  
        }

        // -- 밑으로 데이터가 5개 이하로 있는 경우
        else if((resDataLen < cTABLE_LIST_NUM) && (resDataLen > 0))
        {
            mLineScrollPos += resDataLen;

        }
        // -- 최하단인 경우 ..
        else{
            return;
        }
                    
        
        moveScroll(mLineTableId, mLineScrollPos); // 스크롤 이동
                
    }
                
    console.log("Line Table ScrollPos:"+mLineScrollPos);    
    console.log("Line Table length:"+ mLineTableId.getElementsByTagName("tr").length);
}

/* ============================================================
 * 
 * function : scanLineListNum()
 * description
 * - 업데이트된 노선 테이블의 tr 개수를 체크함
 * - 추가된 tr에도 onclick 이벤트를 주어야하기 때문
 * param
 * return
 * 
 * ============================================================ */
function scanLineListNum(){
    var line = document.querySelectorAll("#lineTable tr");

    for(var i=0; i<line.length; i++){
        line[i].onclick = function(){ // click 이벤트 기다림..
            mSelectedLine.classList.remove("select"); // 이전에 선택한 요소에 className 삭제
            this.className = "select"; // 현재 누른 셀을 class Name 'select'로 줌
            mSelectedLine = this;

            uploadPaTable(); // 방송리스트 테이블 업로드

        }; 
    }
}


/* ============================================================
 * 
 * function : clickPaUpDn(btn)
 * description
 * - PA 테이블의 스크롤 이동
 * param
 * - btn : Up/Down 버튼
 * return
 * 
 * ============================================================ */
function clickPaUpDn(btn)
{
    var addNum = 0;
    var tableTrLen = mPaTableId.getElementsByTagName("tr").length; // 테이블의 tr 개수
    

    // -- up 버튼        
    if(btn == 'up'){ 
        // -- 최상단(if1) ---------------------------
        if(mPaScrollPos == 0){

        }
        // -- 최상단이 아니라면(if2,if3) -------------
        else{
            // 위로 5개 이상 있다면
            if(mPaScrollPos >= cTABLE_LIST_NUM){           
                mPaScrollPos -= cTABLE_LIST_NUM; // scroll 위로 tr 5칸        
                     
            }
            // 5개 이하로 있다면
            else{
                mPaScrollPos = 0; // scroll 최상단으로
            }
            moveScroll(mPaTableId, mPaScrollPos); // 스크롤 이동 
        }
    }
                
    // -- down 버튼
    else{ 
        // 밑으로 남아있는(스크롤 이동할 수 있는) 데이터의 개수
        var resDataLen = (tableTrLen-mPaScrollPos-cTABLE_LIST_NUM);
        
        // -- 밑으로 데이터가 5개 이상 남아있는 경우
        if(resDataLen >= cTABLE_LIST_NUM){
            mPaScrollPos += cTABLE_LIST_NUM;  
        }

        // -- 밑으로 데이터가 5개 이하로 있는 경우
        else if((resDataLen < cTABLE_LIST_NUM) && (resDataLen > 0))
        {
            mPaScrollPos += resDataLen;

        }
        // -- 최하단인 경우 ..
        else{
            return;
        }

        moveScroll(mPaTableId, mPaScrollPos); // 스크롤 이동
                
    }
                
    console.log("PA Table ScrollPos:"+mPaScrollPos);           
    console.log("PA Table length:"+ mPaTableId.getElementsByTagName("tr").length);
            
}

/* ============================================================
 * 
 * function : uploadLineTable()
 * description
 * - 노선 테이블에 리스트 업데이트
 * param
 * return
 * 
 * ============================================================ */ 
function uploadLineTable(){
    var num = mLineTableId.getElementsByTagName("tr").length;

    // -- 테이블 리스트 초기화 ----------------------------------- 
    mLineScrollPos = 0;
    deleteTableAllList(mLineTableId);  

    // -- 테이블에 리스트 추가 -----------------------------------
    addTableList(mLineTableId, mLineData); 

    // -- 스크롤 맨위로 이동 -------------------------------------
    moveScroll(mLineTableId, mLineScrollPos);

    // 첫번째 데이터 자동 선택
    mSelectedLine = mLineTableId.getElementsByTagName("tr")[0];
    mSelectedLine.className = "select"; // 이전에 선택했던 셀

    // -- 추가된 테이블 리스트에 onclick이벤트 --------------------
    scanLineListNum();        

    // -- 노선에 따른 방송리스트 Upload ---------------------------
    uploadPaTable(); 
}


/* ============================================================
 * 
 * function : uploadPaTable()
 * description
 * - 방송 테이블에 리스트 업데이트
 * param
 * return
 * 
 * ============================================================ */
function uploadPaTable()
{
    // -- 테이블 리스트 초기화 ----------------------------------- 
    mPaScrollPos = 0;
    deleteTableAllList(mPaTableId);
                
    // -- 테이블 리스트 추가 ------------------------------------- 
    addTableList(mPaTableId, mPaData); 

    // -- 스크롤 맨위로 이동 -------------------------------------
    moveScroll(mPaTableId, mPaScrollPos);
                
}

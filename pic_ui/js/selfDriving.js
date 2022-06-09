/* -----------------------------------------------------------------------
 *
 * File : selfDriving.js
 * Author : WJIS KEJ
 * PJ Description : 부산1호선 200량 화면설정기(PISC) HTML
 * File Description : simulation 화면에 대한 javascript
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
var mLineScrollPos = 0; // 노선 리스트 테이블(lineTableId) 스크롤 위치
var mPaScrollPos = 0; // 방송 리스트 테이블(paTableId) 스크롤 위치
var mSelectedLine; // 노선 리스트 테이블에 선택된 노선
var mSelectedPa; // 방송 리스트 테이블에 선택된 노선
var mLineData = {
    "type" : "none",
    "count" : 0,
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

var mPaData = {
    "type" : "none",
    "count" : 1,
    "list" : [
        {"id" : 1, "name" : "전체노선도"},
        {"id" : 2, "name" : "혼잡도"},
        {"id" : 3, "name" : "이번역 안내"},
        
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
    
    if(mLineVal == cLINE_UP)    mLineVal = cLINE_DOWN; // 상행선->하행선
    else    mLineVal = cLINE_UP; // 하행선->상행선
                
    displayLineType();  // '상행선','하행선' 표기
    uploadLineTable();  // 상행선, 하행선에 따른 노선 테이블 업데이트
    
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
    // if(mLineVal == cLINE_UP){
    //     mLineBtnId.innerText = "상행선";
    // }
    // else{
    //     mLineBtnId.innerText = "하행선";
    // }

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
function uploadLineTable()
{
    // -- 테이블 리스트 초기화 ----------------------------------- 
    mLineScrollPos = 0;
    deleteTableAllList(mLineTableId);  
  
    // -- 테이블에 리스트 추가 -----------------------------------
    addTableList(mLineTableId, mLineData); 

    // -- 스크롤 맨위로 이동 -------------------------------------
    moveScroll(mLineTableId, mLineScrollPos);
  
    // -- 테이블 첫번째 데이터 자동으로 선택(Cursor 표시) ----------
    mSelectedLine = mLineTableId.getElementsByTagName("tr")[0];
    mSelectedLine.className = "select"; // 이전에 선택했던 셀

    // -- 추가된 테이블 리스트에 onclick이벤트 --------------------
    scanLineListNum();         

    // -- 노선에 따른 방송리스트 Upload ---------------------------
    uploadPaTable(); 

    console.log("uploadLineTable()");
    
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
function scanLineListNum()
{
    var line = document.querySelectorAll("#lineTable tr");
           
    for(var i=0; i<line.length; i++){

        // -- tr에 onclick 이벤트 -------------------------------------
        line[i].onclick = function(){ // click 이벤트 기다림..
                        
            mSelectedLine.classList.remove("select"); // 이전에 선택한 요소에 className 삭제
            this.className = "select"; // 현재 누른 셀을 class Name 'select'로 줌                    
            mSelectedLine = this;

            // -- 노선이 바뀌면 방송리스트 테이블도 업데이트되야함 ---------
            uploadPaTable(); 
  
        }; 
    }
            
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
    
    // -- 테이블 첫번째 데이터 자동으로 선택(Cursor 표시) ----------
    mSelectedPa = mPaTableId.getElementsByTagName("tr")[0];
    mSelectedPa.className = "select"; // 이전에 선택했던 셀

    // -- 추가된 테이블 리스트에 onclick이벤트 --------------------
    scanPaListNum();  // PA 테이블 리스트 개수 스캔
                
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
    
    // -- up 버튼        
    if(btn == 'up'){ 
        // -- 최상단(if1) ---------------------------
        if(mLineScrollPos == 0){
            return;
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
        var tableTrLen = mLineTableId.getElementsByTagName("tr").length; // 테이블의 tr 개수
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
                
    console.log("mLineScrollPos:"+mLineScrollPos);    
    console.log("Line Table length:"+ mLineTableId.getElementsByTagName("tr").length);
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
        var tableTrLen = mPaTableId.getElementsByTagName("tr").length; // 테이블의 tr 개수
    
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
 * function : scanPaListNum()
 * description
 * - 업데이트된 PA 테이블의 tr 개수를 체크함
 * - 추가된 tr에도 onclick 이벤트를 주어야하기 때문
 * param
 * return
 * 
 * ============================================================ */
function scanPaListNum()
{
    var paTableTrId = document.querySelectorAll("#paTable tr");
  
    for(var i=0; i<paTableTrId.length; i++){
        // tr에 onclick 이벤트
        paTableTrId[i].onclick = function(){ // click 이벤트 기다림..
            mSelectedPa.classList.remove("select"); // 이전에 선택한 요소에 className 삭제         
            this.className = "select"; // 현재 누른 셀을 class Name 'select'로 줌       
            mSelectedPa = this;   
        }; 
    }

}
  


/* ============================================================
 * 
 * function addTableList(id,data)
 * description
 * - 테이블의 리스트(tr,td)를 추가함
 * param
 * - id : tr을 추가할 테이블의 아이디
 * - data : 테이블 리스트로 추가할 데이터
 * return
 * - num : 만든 tr 개수
 * 
 * ============================================================ */
function addTableList(id, data){ 
         
    var tdEl = undefined; // td
    var trEl = undefined; // tr
    var num = 0; // 만들 td 개수 
    var strData = undefined; // td에 입력할 텍스트
    var tableLen = id.getElementsByTagName("tr").length; // table의 list 개수
    var dataLen = data.list.length; // json데이터의 list 개수

  
    // --------------------------------------------------
    // 1) td생성하고 td요소에 text node 삽입
    //  ------      ------     
    // |  td  | -- | text |
    //  ------      ------
    // 2) tr 생성하여 td를 붙임
    //  ------      ------      ------ 
    // |  tr  | -- |  td  | -- | text |
    //  ------      ------      ------ 
    // 3) 참조받은 테이블 id에 이것을 붙인다
    //  -------      ------      ------      ------ 
    // | table | -- |  tr  | -- |  td  | -- | text |
    //  -------      ------      ------      ------ 
    // 4) num만큼 반복(num만큼 생성)
    // --------------------------------------------------
    
    // 4) num만큼 생성
    for(var i=0; i<dataLen; i++){
        // 1) td 생성하고 텍스트 노드 삽입
        tdEl = document.createElement("td"); // td생성
        
        strData = data.list[id.getElementsByTagName("tr").length].id 
        + "."
        + data.list[id.getElementsByTagName("tr").length].name;// td에 입력할 데이터
        tdEl.appendChild(document.createTextNode(strData)); // td 요소 안에 text를 넣는다(td<-text)
     
        // 2) tr 생성하고
        trEl = document.createElement("tr"); // tr요소를 만들어서           
        trEl.appendChild(tdEl); // 아까만든 td요소를 이 tr요소의 노드에 붙인다(tr<-(td+text))  
        
        // 3) 참조받은 테이블 id에 이것을 붙인다
        id.appendChild(trEl); // 이 테이블에 만든 tr 요소를 넣는다(table<-(tr+td+text))  

    }
   
    return num; // 만든 tr 개수 리턴
}

/* ============================================================
 * 
 * function : deleteTableAllList(id)
 * description
 * - 테이블 리스트 모두 삭제(tr 삭제)
 * param
 * - id : 리스트를 삭제할 테이블의 아이디
 * return
 * 
 * ============================================================ */
function deleteTableAllList(id)
{
    var num = id.getElementsByTagName("tr").length;

    for(var i=0; i<num;i++){
        id.deleteRow(0);
    }  
            
}


/* ============================================================
 * 
 * function : moveScroll(id, index)
 * description
 * - 테이블 스크롤 이동
 * param
 * - id : 스크롤을 이동할 테이블의 아이디
 * - index : 현재 스크롤 위치
 * return
 * 
 * ============================================================ */
function moveScroll(id, index){        
    var tableTr = id.getElementsByTagName("tr")[index];   
    var scrollTo = tableTr.offsetTop;
                
    id.scrollTop = scrollTo;            

    console.log("scroll"+scrollTo);
            
}


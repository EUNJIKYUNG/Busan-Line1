/* -----------------------------------------------------------------------
 *
 * File : selfDriving.js
 * Author : WJIS KEJ
 * PJ Description : 부산1호선 200량 화면설정기(PIC) HTML
 * File Description : selfDriving 화면에 대한 javascript
 * Date : 2022.06.09 ~
 *
 * ----------------------------------------------------------------------- */

// ==== constant ==============================================
const cTABLE_LIST_NUM = 5; // 테이블 리스트의 개수


// ==== variable ==============================================
var mPaScrollPos = 0; // 테이블 스크롤 위치
var mPaTableId = document.getElementById("paTable");            
var mSelectedPa; // 방송 리스트 테이블에 선택된 노선
var mPaData = {
    "type" : "none",
    "count" : 1,
    "list" : [
        {"id" : 1, "name" : "시발방송:방화"},
        {"id" : 2, "name" : "도착방송:개화산"},
        {"id" : 3, "name" : "도착방송:김포공항"},
        {"id" : 4, "name" : "도착방송:송정"},
        {"id" : 5, "name" : "도착방송:마곡"},
        {"id" : 6, "name" : "시발방송:발산"},
        {"id" : 7, "name" : "도착방송:우장산"},
        {"id" : 8, "name" : "도착방송:화곡"},
        {"id" : 9, "name" : "도착방송:까치산"},
        {"id" : 10, "name" : "도착방송:신정"},
        {"id" : 11, "name" : "도착방송:목동"},
        {"id" : 12, "name" : "도착방송:오목교"},
        {"id" : 13, "name" : "도착방송:양평"},
        {"id" : 14, "name" : "도착방송:영등포구청"}
        
    ]
}

/* ============================================================
 * main
 * ============================================================ */
uploadPaTable(); // 노선 테이블 Load


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
    
    // -- 테이블 첫번째 데이터 자동으로 선택(Cursor 표시) ----------
    mSelectedPa = mPaTableId.getElementsByTagName("tr")[0];
    mSelectedPa.className = "select"; // 이전에 선택했던 셀

    // -- 추가된 테이블 리스트에 onclick이벤트 --------------------
    scanPaListNum();  // PA 테이블 리스트 개수 스캔
                
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
  

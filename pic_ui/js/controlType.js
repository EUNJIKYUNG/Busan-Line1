/* -----------------------------------------------------------------------
 *
 * File : controlType.js
 * Author : WJIS KEJ
 * PJ Description : 부산1호선 200량 화면설정기(PIC) HTML
 * File Description : controlType 화면에 대한 javascript
 * Date : 2022.06.09 ~
 *
 * ----------------------------------------------------------------------- */


// ==== constant ==============================================
const cCONTROL_AUTO = 0; // 운행방식이 자동
const cCONTROL_MANUAL = 1; // 운행방식이 수동

// ==== variable ==============================================
var mControlBtnId = document.querySelector("#txtControl"); // 수동/자동 버튼
var mControlVal = cCONTROL_MANUAL; // 운행방식이 자동인지 수동인지?
var mSaveFlag = false;


/* ============================================================
 * main
 * ============================================================ */
displayControlType(); // 운행방식값에 따라 버튼에 "자동" "수동" 표기



/* ============================================================
 * 
 * function : clickControlType()
 * description
 * - 운행방식 변경. 자동/수동 버튼 누르면 toggle
 * param
 * return
 * 
 * ============================================================ */
function clickControlType()
{
    // == 운행방식 값 변경 -----------------------------------------
    if(mControlVal === cCONTROL_MANUAL)     mControlVal = cCONTROL_AUTO; // 수동->자동
    else    mControlVal = cCONTROL_MANUAL; // 자동->수동

    displayControlType(); // 운행방식값에 따라 버튼에 "자동" "수동" 표기

    // -- save flag 해제 ------------------------------------------
    // 이전에 확인 버튼을 눌러 값이 저장되었는데 다시 값을 변경한 경우 
    if(mSaveFlag == true){
        mSaveFlag = false;
        mControlBtnId.style.background = "rgb(8, 8, 156)";
        mControlBtnId.style.color ="#ffffff";
    }

}

/* ============================================================
 * 
 * function : displayControlType()
 * description
 * - 운행방식값에 따라 버튼에 "자동" "수동" 표기
 * param
 * return
 * 
 * ============================================================ */
function displayControlType()
{
    if(mControlVal == cCONTROL_MANUAL){
        mControlBtnId.innerText = "수동";
    }
    else{
        mControlBtnId.innerText = "자동";
    }
}

/* ============================================================
 * 
 * function : saveParam()
 * description
 * - 확인버튼 누르면 운행방식 값이 저장
 * param
 * return
 * 
 * ============================================================ */
function saveParam()
{
    mControlBtnId.style.background = "#ffffff"
    mControlBtnId.style.color="rgb(8, 8, 156)";
    mSaveFlag = true;
}
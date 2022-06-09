/* -----------------------------------------------------------------------
 *
 * File : diagnosis.js
 * Author : WJIS KEJ
 * PJ Description : 부산1호선 200량 화면설정기(PIC) HTML
 * File Description : diagnosis 화면에 대한 javascript
 * Date : 2022.06.09 ~
 *
 * ----------------------------------------------------------------------- */


// ==== constant ==============================================
const cSTATUS_OK = 0; // 상태 OK
const cSTATUS_NG = 1; // 상태 NG

// ==== variable ==============================================
var mCarStatus = [0,0,0,0,0,0,0,0];
var mCarClass = document.querySelectorAll(".carNumber button");


/* ============================================================
 * main
 * ============================================================ */
displayStatus(); // 각 차량의 상태를 읽어 OK(GREEN)/NG(RED) 색상 표시하기



/* ============================================================
 * 
 * function : displayStatus()
 * description
 * - 각 차량의 상태를 읽어 OK(GREEN)/NG(RED) 색상 표시하기
 * param
 * return
 * 
 * ============================================================ */ 
 function displayStatus()
{
    for(var i=0; i<8; i++){
        // -- NG ------------------------------
        if(mCarStatus[i] == cSTATUS_NG)      mCarClass[i].className = "ngColor";
        // -- OK ------------------------------
        else        mCarClass[i].className = "okColor";
    }
}

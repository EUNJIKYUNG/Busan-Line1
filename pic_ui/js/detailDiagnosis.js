/* -----------------------------------------------------------------------
 *
 * File : detailDiagnosis.js
 * Author : WJIS KEJ
 * PJ Description : 부산1호선 200량 화면설정기(PIC) HTML
 * File Description : detailDiagnosis 화면에 대한 javascript
 * Date : 2022.06.09 ~
 *
 * ----------------------------------------------------------------------- */


// ==== constant ==============================================
const cSTATUS_OK = 0; // 상태 OK
const cSTATUS_NG = 1; // 상태 NG

// ==== variable ==============================================

var mCarNum; // 몇호차인지
// [0]: 왼쪽위IDD1 [1]:왼쪽위IDD2 [2]: 오른쪽위IDD1 [3]:오른쪽위IDD2
// [4]: FTNDD [5]: PII
// [6]: 왼쪽아래IDD1 [7]:왼쪽아래IDD2 [8]: 오른쪽아래IDD1 [9]:오른쪽아래IDD2
var mDeviceStat = [1,0,0,0,0,0,0,0,0,1]; // 각 차의 상태



/* ============================================================
 * main
 * ============================================================ */

checkCarNum(); // 몇호차 정보를 볼 것인가
displayStatus();


/* ============================================================
 * 
 * function : checkCarNum()
 * description
 * - 몇호차인지 확인
 * param
 * return
 * 
 * ============================================================ */
function checkCarNum()
{
    /* ----------------------------------------------
     1) urlText을 '?'로 urlText[0], urlText[1]로 쪼갬
        /html/detailDiagnosis.html?car=1
        -------------------------- -----
                  urlText[0]        urlText[1]  
     2) urlText[1]을 '='로 car[0], car[1]로 쪼갬
        car=1
        --- --
        [0] [1]
     3) car[1]의 타입을 문자에서 숫자로 변경
     * ---------------------------------------------- */
    var urlText = location.href.split('?'); // 1)
    var car = urlText[1].split('='); // 2)

    mCarNum = parseInt(car[1]); // 3)
    if(mCarNum == 0)    mCarNum = 8;

    // -- 몇호차인지 표기 ---------------------------------------
    document.querySelector("#carNum").innerText = (mCarNum+"호차"); 

    // -- 0,1호차에만 FTNDD, PII 장치 표시하기 -------------------
    // if((mCarNum == 0) || (mCarNum == 1)){ 
    //     displayFtnddPii();
    // }

    console.log("몇호차????:"+mCarNum);

}
 
/* ============================================================
 * 
 * function : displayFtnddPii()
 * description
 * - 양 끝차만 FTNDD, PII 표시하기
 * param
 * return
 * 
 * ============================================================ */
function displayFtnddPii()
{   
    var ftnddId = document.getElementById("ftndd");
    var piiId = document.getElementById("pii");

    // ftnddId.style.visibility = 'visible'; // FTNDD visible
    // piiId.style.visibility = 'visible'; // PII visible

}

 
/* ============================================================
 * 
 * function : pushArrow()
 * description
 * - >> or << 버튼으로 이전차 다음차 선택하여 차량 상태 보기
 * param
 * - btn : <<(이전차) or >>(다음차)
 * return
 * 
 * ============================================================ */
function pushArrow(btn){
    var moveCar; // 이동할 차량(다음차?이전차?)

    // 버튼 << 누름 : 이전차로 이동
    if(btn == "prev"){
        if(mCarNum == 8){ // 0~7호차 까지
            moveCar = 7;
        }
        else{
            moveCar = (mCarNum-1); // 이전 열차
        }
    }
    // 버튼 >> 누름 : 다음차로 이동
    else if(btn == "next"){
        if(mCarNum == 8){ // 0~7호차 까지
            moveCar = 1;
        }
        else{
            moveCar = (mCarNum+1); // 다음 열차
        }
    }

    location.href = ("detailDiagnosis.html"+"?car="+moveCar); // 이동할 차의 url

}

/* ============================================================
 * 
 * function : displayStatus()
 * description
 * - 장치 상태에 따라 OK(GREEN)/NG(RED) 색상 표시하기
 * param
 * return
 * 
 * ============================================================ */
function displayStatus()
{
    var device = document.querySelectorAll("#detailDiag p");

    /* ----------------------------------------------------------------------
        [0]: 왼쪽위IDD1 [1]:왼쪽위IDD2 [2]: 오른쪽위IDD1 [3]:오른쪽위IDD2
        [4]: FTNDD [5]: PII
        [6]: 왼쪽아래IDD1 [7]:왼쪽아래IDD2 [8]: 오른쪽아래IDD1 [9]:오른쪽아래IDD2
     * ----------------------------------------------------------------------- */
    for(var i=0; i<10; i++){
        /* -- status OK ---------------------- */
        if(mDeviceStat[i] == cSTATUS_OK){
            device[i].classList.add("okColor");
        }
        /* -- status NG ---------------------- */
        else{
            device[i].classList.add("ngColor");
        }
    }
}

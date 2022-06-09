/* -----------------------------------------------------------------------
 *
 * File : powerSaving.js
 * Author : WJIS KEJ
 * PJ Description : 부산1호선 200량 화면설정기(PIC) HTML
 * File Description : powerSaving 화면에 대한 javascript
 * Date : 2022.06.09 ~
 *
 * ----------------------------------------------------------------------- */

// ==== constant ==============================================
const cALLSAVING = 0xFF;
const cALLNORMAL = 0x00;
const cSAVINGMODE = 1;
const cNORMALMODE = 0;

// ==== variable ==============================================
var savingMode = [0,0,0,0,0,0,0,0]; // 각 차의 절전모드(1)/일반모드(0) 값 
var carSel = document.querySelectorAll(".carNumber button");


/* ============================================================
 * main
 * ============================================================ */
displayMode();


/* ============================================================
 * 
 * function : displayMode()
 * description
 * - 각 차량의 절전기능값 읽어서 현시하기
 * param
 * return
 * 
 * ============================================================ */ 
function displayMode()
{
    for(var i=0; i<8; i++){
        // 절전모드 
        if(savingMode[i] == 1)      chgSavingMode(i);
        // 일반모드
        else        chgNormalMode(i);
    }
}
            

/* ============================================================
 * 
 * function : changeMode()
 * description
 * - 버튼 누르면 절전<->일반모드 변경
 * param
 * - car : 전체절전 or 전체절전해제 or 모드를 변경할 차 
 * return
 * 
 * ============================================================ */ 
function changeMode(car)
{
    
    // -- 전체 절전 해제 ------------------------------
    if(car == cALLNORMAL){ 
        for(var i=0; i<8; i++){ // 1호차부터 0호차까지
            savingMode[i] = cNORMALMODE;
            chgNormalMode(i); // 버튼 스타일 변경(일반모드로)
        }
    }

    // -- 전체 절전 -----------------------------------
    else if(car == cALLSAVING){ // ALL 절전
        for(var i=0; i<8; i++){ 
            savingMode[i] = cSAVINGMODE;
            chgSavingMode(i); // 버튼 스타일 변경(절전모드로)
            
            
        }
    }

    // -- 각각 상태 제어 -------------------------------
    else{ 

        // ID는 생성된 순서대로라서.. 0이 젤 처음 생성(1호차) 7이 젤 나중 생성(0호차)
        // 1호차 : car=1, ID=0, savingMode[0]데이터에 해당
        // 2호차 : car=2, ID=1, savingMode[1]데이터에 해당
        // 0호차 : car=8, ID=7, savingMode[6]데이터에 해당
        var carId = (car-1);
        savingMode[carId] ^= 1; // 비트 반전 1(절전) <-> 0(일반)

        // -- 절전모드 --------------------------- 
        if(savingMode[carId] == 1)      chgSavingMode(carId);

        // -- 일반모드 ---------------------------
        else        chgNormalMode(carId);
            
    }

    console.log("car status:" + savingMode);

}

   
/* ============================================================
 * 
 * function : chgSavingMode()
 * description
 * - 절전모드로 버튼 스타일 변경
 * - 클래스명 powerSaving로 변경
 * param
 * return
 * 
 * ============================================================ */ 
function chgSavingMode(id){
    carSel[id].className = "powerSaving";
}
            
            

/* ============================================================
 * 
 * function : chgNormalMode()
 * description
 * - 일반모드로 버튼 스타일 변경
 * - 클래스명 powerNormal 변경
 * param
 * return
 * 
 * ============================================================ */ 
function chgNormalMode(id){
    carSel[id].className = "powerNormal";
}

            

/* -----------------------------------------------------------------------
 *
 * File : formNumber.js
 * Author : WJIS KEJ
 * PJ Description : 부산1호선 200량 화면설정기(PIC) HTML
 * File Description : formNumber 화면에 대한 javascript
 * Date : 2022.06.09 ~
 *
 * ----------------------------------------------------------------------- */


 // ==== constant ==============================================
const cNUMBER_POSIT = 4; // 편성번호는 4자리수

 // ==== variable ==============================================
var mSetFlag = false; // 설정을 완료했다는 flag



/* ============================================================
 * 
 * function : inputNum(num)
 * description
 * - 숫자키로 편성번호 입력
 * param
 * - num : 숫자키(0~9)
 * return
 * 
 * ============================================================ */
function inputNum(num){
    if((num >= '0') && (num <= '9')){ // 0~9
        if(txtFormNum.value.length < cNUMBER_POSIT){ //4글자 모두 입력되지 않았으면
            txtFormNum.value += num; // 입력
        }
    }
    else if(num == 'b'){ // backspace
        if(mSetFlag == false){ // 설정이 완료되지 않은 경우에만 번호를 지울 수 있음
            txtFormNum.value = txtFormNum.value.substring('', txtFormNum.value.length-1); // 이전 글자 지우기                      
        }
    }
    else if(num == 'c'){ // clear
        txtFormNum.value = ''; 
        mSetFlag = false; // 설정 미완료 flag
        txtFormNum.style.color = "black"; // 설정 미완료(글자색 black)
    }
    else{ // error
        console.log("숫자, 백스페이스, Clear 외에 다른 문자")
    }

    // 편성번호 4글자 모두 완성해야 확인 버튼 활성화
    if(txtFormNum.value.length >= cNUMBER_POSIT){
        document.querySelector("#okBtn").disabled = false; // 활성화
    }
    else{
        document.querySelector("#okBtn").disabled = true; // 비활성화
    }
}

/* ============================================================
 * 
 * function : saveParam()
 * description
 * - 확인 버튼 누르면 설정한 편성번호 저장
 * param
 * return
 * 
 * ============================================================ */
function saveParam()
{
    txtFormNum.style.color = "red"; // 편성번호 설정되었다는 의미(글자색 RED)                   
    console.log("편성번호 : " + txtFormNum.value);
    mSetFlag = true; // 설정 완료 flag
}
/* -----------------------------------------------------------------------
 *
 * File : json.js
 * Author : WJIS KEJ
 * File Description : json형태 데이터를 처리하는 함수
 * - json 데이터에 type을 넣어 전체 업데이트, 업데이트, 삭제, 추가를 할 수 있도록 함
 * - 객체에 "list"라는 Key를 생성하여 리스트를 관리함
 * - list 내에 id는 리스트의 일련번호로 중복된 값을 갖을 수 없다.
 * - 1) 전체 업데이트 : list의 모든 데이터를 삭제하고 입력받은 데이터로 업데이트
 * - 2) 업데이트 : 입력한 데이터의 id와 일치하는 데이터의 정보를 입력받은 데이터로 수정
 * - 3) 삭제 : 입력한 데이터의 id와 일치하는 데이터 삭제
 * - 4) 추가 : 입력한 데이터의 id와 동일한 데이터가 없다면 추가, 있다면 무시함
 * Date : 2020.04.23 ~
 *
 * 데이터 예시
 * 
// updateall
{
	"type" : "updateall",
	"count" : 0,
	"list" : [
		{"id" : 1, "name" : "강동->마천"},
		{"id" : 2, "name" : "강동->상일동"},
		{"id" : 3, "name" : "군자->방화"},
		{"id" : 4, "name" : "군자->상일동"},
		{"id" : 5, "name" : "마천->강동"},
		{"id" : 6, "name" : "상일동->강동"},
		{"id" : 7, "name" : "마천->군자(능동)"},
		{"id" : 8, "name" : "상일동->군자(능동)"},
		{"id" : 9, "name" : "마천->방화"},
		{"id" : 10, "name" : "상일동->방화"},
		{"id" : 11, "name" : "마천->애오개행"},
		{"id" : 12, "name" : "상일동->애오개행"},
		{"id" : 13, "name" : "마천->여의도"},
		{"id" : 14, "name" : "상일동->여의도"},
        {"id" : 15, "name" : "마천->왕십리(성동구청)"},
        {"id" : 16, "name" : "마천->여의도"},
        {"id" : 17, "name" : "상일동->여의도"},
        {"id" : 18, "name" : "강동->마천"},
		{"id" : 19, "name" : "강동->상일동"},
		{"id" : 20, "name" : "군자->방화"},
		{"id" : 21, "name" : "군자->상일동"},
		{"id" : 22, "name" : "마천->강동"},
		{"id" : 23, "name" : "상일동->강동"}
	]
}

{
	"type" : "updateall",
	"count" : 0,
	"list" : [
		{"id" : 1, "name" : "강동->마천1"},
		{"id" : 2, "name" : "강동->상일동1"},
		{"id" : 3, "name" : "군자->방화1"},
		{"id" : 4, "name" : "군자->상일동1"},
		{"id" : 5, "name" : "마천->강동1"},
		{"id" : 6, "name" : "상일동->강동1"},
		{"id" : 7, "name" : "마천->군자(능동)1"}

	]

}

// update
{
	"type" : "update",
	"count" : 0,
	"list" : [
		{"id" : 1, "name" : "강동->마천2"},
		{"id" : 2, "name" : "강동->상일동2"},
		{"id" : 3, "name" : "군자->방화2"},
		{"id" : 20, "name" : "군자->상일동2"}
	]

}

// delete
{
	"type" : "delete",
	"count" : 0,
	"list" : [
		{"id" : 2}
	]

}

// insert
{
	"type" : "insert",
	"list" : [
        {"id" : 16, "name" : "상일동->왕십리(성동구청)"},
        {"id" : 17, "name" : "마천->화곡"},
        {"id" : 18, "name" : "상일동->화곡"},
        {"id" : 19, "name" : "방화->강동"}
	]

}

// select
{
    "type" : "select",
    "list" : [
        {"id" : 10}
    ]

}

 * ----------------------------------------------------------------------- */

/* ============================================================
 * 
 * function : parseJson(destData, srcData)
 * description
 * - json 데이터 
 * param
 * return
 * 
 * ============================================================ */
//function parseJson(destData, srcData) // 나중에 수정 !! json이 편집될 데이터와 통신으로 받은 데이터 입력 !!!!!!!!!!
function parseJson()
{
    var srcDataId = document.getElementById("jsonStr").value;
    var srcData; // text form에 입력한 문자열을 json형태 데이터로 파싱
    var destData; // 편집할 json data

    // -- 나중에 수정해주세요 !! -----------------------------------
    srcData = JSON.parse(srcDataId); // 통신으로 받은 데이터로 수정해야함
    destData = mLineData; 
    // -----------------------------------------------------------
    
    if(srcData.hasOwnProperty('list'))
    {

        // -- 전체 업데이트 -------------------------------
        if(srcData.type == "updateall"){
            console.log("updateall");
            updateAllJson(destData, srcData);
            
            uploadLineTable();  // 테이블 업데이트
        }
        // -- 일부 업데이트 -------------------------------
        else if(srcData.type == "update"){
            console.log("update");
            updateJson(destData, srcData);

            uploadLineTable();  // 테이블 업데이트
        }
        // -- 일부 삭제 -----------------------------------
        else if(srcData.type == "delete"){
            console.log("delete");
            deleteJson(destData, srcData);

            uploadLineTable();  // 테이블 업데이트
        }
        // -- 추가 ---------------------------------------
        else if(srcData.type == "insert"){
            console.log("insert");
            insertJson(destData, srcData);

            uploadLineTable();  // 테이블 업데이트
        }
        // -- 테이블 선택 ---------------------------------
        else if(srcData.type == "select"){
            console.log("select");
            selectJson(destData, srcData);

        }
        // -- 그 외는 무시 -------------------------------- 
        else{

        }
        mLineData.count = mLineData.list.length; // count = list 데이터의 총개수

    }

}
/* ============================================================
 * 
 * function : updateAllJson(destData, srcData)
 * description
 * - 리스트에 데이터 전체 업데이트
 * param
 * - destData : 목적 데이터               updateall
 * - srcData : 업데이트할 데이터  destData <------ srcData
 * return
 * 
 * ============================================================ */
function updateAllJson(destData, srcData)
{
    // -- all delete -------------------------------
    destData.list.splice(0, destData.list.length); // 0부터 length만큼 삭제

    // -- all insert -------------------------------
    insertJson(destData, srcData);
    
}

/* ============================================================
 * 
 * function : updateJson(destData, srcData)
 * description
 * - 리스트에 데이터 일부 업데이트
 * param
 * - destData : 목적 데이터                update
 * - srcData : 업데이트할 데이터  destData <------ srcData
 * return
 * 
 * ============================================================ */
function updateJson(destData, srcData)
{
    // -- 기존 json데이터 key "list" "id" 값을 임의의 배열에 모두 저장해둠 -----
    var tempArr = [];
    for(var i=0; i<destData.list.length; i++)
    {
        tempArr[i] = destData.list[i].id;
    }

    // -- 입력한 json 데이터의 id값이 기존에 있던 데이터와 중복되는지 확인 -------
    // 
    for(var i=0; i<srcData.list.length; i++)
    {
        var equlPos = tempArr.indexOf(srcData.list[i].id);

        // id가 동일한 값이 있으면 덮어쓰기 ------------------------------------
        if(equlPos != -1)
        {
            destData.list[equlPos] = srcData.list[i];
        }
    }

}

/* ============================================================
 * 
 * function : deleteJson(destData, srcData)
 * description
 * - 리스트에 데이터 삭제
 * param
 * - destData : 목적 데이터             delete
 * - srcData : 삭제할 데이터   destData <------ srcData
 * return
 * 
 * ============================================================ */
function deleteJson(destData, srcData)
{
    // -- 기존 json데이터 key "list" "id" 값을 임의의 배열에 모두 저장해둠 -----
    var tempArr = [];
    for(var i=0; i<destData.list.length; i++)
    {
        tempArr[i] = destData.list[i].id;
    }

    // -- 입력한 json 데이터의 id값이 기존에 있던 데이터와 중복되는지 확인 -------
    // 
    for(var i=0; i<srcData.list.length; i++)
    {
        var equlPos = tempArr.indexOf(srcData.list[i].id);

        // id가 동일한 값이 있으면 삭제 ------------------------------------
        if(equlPos != -1)
        {
            console.log("delete:" + destData.list[equlPos]);
            console.log("equlpos" + equlPos);
 
            destData.list.splice(equlPos, 1);
        }
    }

}

/* ============================================================
 * 
 * function : insertJson(destData, srcData)
 * description
 * - 리스트에 데이터 추가
 * param
 * - destData : 목적 데이터              add
 * - srcData : 추가할 데이터  destData <------ srcData
 * return
 * 
 * ============================================================ */
function insertJson(destData, srcData)
{
    // -- 기존 json데이터 key "list" "id" 값을 임의의 배열에 모두 저장해둠 -----
    var tempArr = [];
    for(var i=0; i<destData.list.length; i++)
    {
        tempArr[i] = destData.list[i].id;
    }

    // -- 입력한 json 데이터의 id값이 기존에 있던 데이터와 중복되는지 확인 -------
    // 
    for(var i=0; i<srcData.list.length; i++){
        var equlPos = tempArr.indexOf(srcData.list[i].id);

        // id가 동일한 값이 있으면 덮어쓰기 ------------------------------------
        if(equlPos == -1)
        {
            destData.list = destData.list.concat(srcData.list[i]);
        }
    }
    
}

/* ============================================================
 * 
 * function : selectJson(destData, srcData)
 * description
 * - 받은 id 테이블에서 선택 표시
 * param
 * - destData : 목적 데이터              
 * - srcData : 선택표시할 데이터의 id  
 * return
 * 
 * ============================================================ */
function selectJson(destData, srcData)
{
    
    var tempArr = [];

    // -- 리스트 선택은 1개만 할 수 있음 --------------------------------------
    console.log("srcData.list.length"+srcData.list.length);
    if(srcData.list.length != 1)    return;


    // -- 기존 json데이터 key "list" "id" 값을 임의의 배열에 모두 저장해둠 -----
    for(var i=0; i<destData.list.length; i++)
    {
        tempArr[i] = destData.list[i].id;
    }

    // -- 입력한 json 데이터의 id값이 기존에 있던 데이터와 중복되는지 확인 -------
    // 
    for(var i=0; i<srcData.list.length; i++)
    {
        var equlPos = tempArr.indexOf(srcData.list[i].id);
        console.log("equlPos"+equlPos);

        // id가 동일한 값이 있으면 select ------------------------------------
        if(equlPos != -1)
        {
            // 현재는 노선테이블에만 적용함. 
            // 데이터가 어디 테이블의 것인지 구분하여 테이블id도 구분해야함


            // -- 스크롤 이동 ----
            moveScroll(mLineTableId, equlPos);
            
            // -- select class -----
            mSelectedLine.classList.remove("select"); // 이전에 선택한 요소에 className 삭제
            mLineTableId.getElementsByTagName("tr")[equlPos].className = "select";
            mSelectedLine = mLineTableId.getElementsByTagName("tr")[equlPos];
            mLineScrollPos = equlPos;


            console.log("mLineTableId.scrollTop"+mLineTableId.scrollTop);

        }
    }


}

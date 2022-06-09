
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



#include <memory>
#include <iostream>
#include "CWebServer.h"
#include "CDevIdentify.h"
#include "CommManage.h"
#include "CDataManage.h"
#include "TablesDefine/TableManage.h"
#include "commonsocket/CDatagram.h"
#include "COperation.h"
#include <unistd.h>

std::shared_ptr<CCommManage> m_pCommManage;

int main(int argc, const char * argv[])
{
			CDevIdentify *pIdent = CDevIdentify::GetInstance();
			
			pIdent->InitDevTable();
			printf("%s %d\n",__FUNCTION__,__LINE__);
			CTableManage *pTM = CTableManage::GetInstance();
			printf("%s %d\n",__FUNCTION__,__LINE__);
			pTM->InitDatabase("/app/OPDATA.DB");
			printf("%s %d\n",__FUNCTION__,__LINE__);
			pTM->LoadDatabase();
			pTM->CloseDatabase();
			printf("%s %d\n",__FUNCTION__,__LINE__);
			CDataManage *pDM = CDataManage::GetInstance();
			pDM->InitDataManage();
			printf("%s %d\n",__FUNCTION__,__LINE__);
			CWebServer *pServer = CWebServer::GetInstance();
			printf("%s %d\n",__FUNCTION__,__LINE__);
			pServer->InitWebServer();
			printf("%s %d\n",__FUNCTION__,__LINE__);
			pServer->InitCheckFunc();
			printf("%s %d\n",__FUNCTION__,__LINE__);
			m_pCommManage = std::shared_ptr<CCommManage>(new CCommManage());
			printf("%s %d\n",__FUNCTION__,__LINE__);
			m_pCommManage->initManage(20050);
			printf("%s %d\n",__FUNCTION__,__LINE__);
			//CDatagram::startDispatch();
			COperation *pOper=COperation::GetInstance();
			while(1)
			{
				pOper->RunLoop();
				usleep(100000);
			}
			
    return 0;
}


#ifndef SQLDATA_HEADER_HPP
#define SQLDATA_HEADER_HPP
#include <algorithm>

template <typename _Pr>
void MakeRelationShip(std::vector<SHARED_PTRC(CSQLData)> *pParent, int nChildIndex, std::vector<SHARED_PTRC(CSQLData)> *pChild)
{
	int nParentIndexCol = 0;
	int nParentIndex;
	int nChildCount;
	std::vector<SHARED_PTRC(CSQLData)>::iterator nit;
	std::vector<SHARED_PTRC(CSQLData)>::iterator knit;
	if (pParent&&pChild&&pParent->size() && pChild->size())
	{
		nParentIndexCol = pChild->at(0)->GetParentIndexCol();
		if (nParentIndexCol != -1)
		{
			for (nit = pChild->begin(); nit != pChild->end(); nit++)
			{
				(*nit)->GetData(nParentIndexCol, &nParentIndex);
				_Pr tPR;
				bool bRet= tPR((*nit));
				knit = std::find_if(pParent->begin(), pParent->end(), findSQLData(nParentIndex));
				if ((knit != pParent->end())&& bRet)
				{
					nChildCount = pParent->at(0)->GetChildCount();
					if (nChildIndex < nChildCount)
					{
						(*knit)->m_vChildItem[nChildIndex].vSQLData.push_back(*nit);
					}
				}
			}
		}
	}
}

#endif


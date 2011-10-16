/**
*File:  RankManager.h
*Brief: ����������������
*Author:��η
*Date:  2010.03.08
*/
#pragma once

#include "../../../../Public/Common/RankDef.h"

class CRankManager
{
public:
    CRankManager(void);
    virtual ~CRankManager(void);

    void AddRankInfo(tagRankInfo *pRankInfo); 
    void ClearRankInfo();
    void RemoveRankInfo(long lRankID);

    // ͨ������������������Ϣ
    tagRankInfo* FindRankInfo(long lRankID);

    void ProcMsg(CMessage* pMsg);

private:
    map<long, tagRankInfo*> m_mapRankInfo;
 //   CRITICAL_SECTION    m_csRankInfo;           // ���а���Ϣ�ٽ����
};

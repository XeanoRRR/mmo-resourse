/**
*File:  RankManager.cpp
*Brief: ����������������
*Author:��η
*Date:  2010.03.08
*/
#include "Stdafx.h"
#include "RankManager.h"

CRankManager::CRankManager()
{

}

CRankManager::~CRankManager()
{
    ClearRankInfo();
}

void CRankManager::AddRankInfo(tagRankInfo *pRankInfo)
{
    if(pRankInfo == NULL)
        return;

    RemoveRankInfo(pRankInfo->lRankID);    
    m_mapRankInfo[pRankInfo->lRankID] = pRankInfo;
    
}


void CRankManager::ClearRankInfo()
{
    for(map<long, tagRankInfo*>::iterator it = m_mapRankInfo.begin(); it != m_mapRankInfo.end(); ++it)
    {
        SAFE_DELETE(it->second);
    }
    m_mapRankInfo.clear();
}

void CRankManager::RemoveRankInfo(long lRankID)
{
    map<long, tagRankInfo*>::iterator itor = m_mapRankInfo.find(lRankID);
    if(itor != m_mapRankInfo.end())
    {
        tagRankInfo* pRankInfo = itor->second;
        if(pRankInfo)
        {
            SAFE_DELETE(pRankInfo);
        }
    }
}


tagRankInfo* CRankManager::FindRankInfo(long lRankID)
{
    tagRankInfo *pRankInfo = NULL;

    map<long, tagRankInfo*>::iterator itor = m_mapRankInfo.find(lRankID);
    if(itor != m_mapRankInfo.end())
    {
        pRankInfo = itor->second;
    }  

    return pRankInfo;
}

void CRankManager::ProcMsg(CMessage* pMsg)
{
    if(pMsg == NULL)
        return;

    CGUID guidPlayer;
    vector<BYTE> vecByte;
    pMsg->GetGUID(guidPlayer);
    CMessage msg(MSG_W2S_QUEST_PLAYERRANKS);
    msg.Add(guidPlayer);
    
    eRankType eRT, eBakRT;
    eRT = (eRankType)pMsg->GetChar();  
    long lRankID = pMsg->GetLong();

    eBakRT = eRT;
    // ��ȡ��Ӧ������������Ϣ
    tagRankInfo *pRankInfo = GetGame().GetRankManager()->FindRankInfo(lRankID);
    if(pRankInfo == NULL)
    {
        // send back no match rank message
        eBakRT = RT_UPDATE;
        msg.Add((char)eBakRT);
        msg.Add((long)0);
        msg.SendToGS(pMsg->GetGSID());
        return;
    }

    // �����ܽ����ٽ������������أ����ȴ�
    if(pRankInfo->TryEnterCS())
    {  
        switch(eBakRT)
        {
        case RT_OWNER:
            {
                // ͨ�������������
                CDataObject *pEntity = new CDataObject();  
                char szPlayerName[128],szColName[128];
                pMsg->GetStr(szPlayerName, 128);
                pEntity->SetStringValue(szPlayerName, sizeof(szPlayerName));
                pMsg->GetStr(szColName, 128);    
                strlwr(szColName);
                if(!pRankInfo->AddToByteArray(&vecByte, pEntity, szColName))
                {
                    eBakRT = RT_NOMATCH;
                }
                else
                {
                    eBakRT = RT_OWNER;
                }

                SAFE_DELETE(pEntity);
            }
            break;

        case RT_OTHER:
            {
                // ��ѯĳҳ������
                long lPage = pMsg->GetLong();
                if(!pRankInfo->AddToByteArray(&vecByte, lPage))
                {
                    eBakRT = RT_NOMATCH;
                }
                else
                {
                    eBakRT = RT_OTHER;
                }
            }
            break;

        case RT_RANK:
            {
                // ��ѯ��n������ҳ������
                long lRank = pMsg->GetLong();
                long lPage = pRankInfo->FindPage(lRank - 1);
                if(!pRankInfo->AddToByteArray(&vecByte, lPage))
                {
                    eBakRT = RT_NOMATCH;
                }
                else
                {
                    eBakRT = RT_RANK;
                }
            }
            break;

        default:
            eBakRT = RT_UPDATE;
            break;
        }

        pRankInfo->LeaveCS();
    }
    else
    {
        eBakRT = RT_UPDATE;
    }

    msg.Add((char)eBakRT);
    msg.Add((long)vecByte.size());
    if(vecByte.size())
    {
        msg.AddEx(&vecByte[0], (long)vecByte.size() );
    }    
    
    msg.SendToGS(pMsg->GetGSID());
}

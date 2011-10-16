#include "StdAfx.h"
#include "FreezeBounsOper.h"
#include "../../../dbaccess/BounsDB/RsBouns.h"

extern long g_lFreezeBakNum;

CFreezeBounsOper::CFreezeBounsOper(eOperType eOt)
{
	SetOperType(eOt);
	m_pBInfo = new tagBounsInfo();
}

CFreezeBounsOper::~CFreezeBounsOper(void)
{
	SAFE_DELETE(m_pBInfo);
}

BOOL CFreezeBounsOper::DoOperator()
{
	// ��������Ĺ�����Ҫ���¶Խ���ID��ֵ������Client�����������ݱ��޸ģ�
	// ������ȡ�����Ľ���
    long lRet = -1;
	HRESULT hRt = GetGame()->GetRsBouns()->FreezeBouns(m_cnConnPtr, m_pBInfo->guid, m_pBInfo->dwBounsID, lRet);
    if(hRt != S_OK)
        return FALSE;

	CMessage msg(MSG_BDS2S_BOUNS_GET_RESPOND);
	msg.Add(lRet);
	msg.Add(m_guidPlayer);
	msg.Add(m_strPlayerIP.c_str());
	DBWriteSet setWriteDB;
	msg.GetDBWriteSet(setWriteDB);
	m_pBInfo->EncodeToDataBlock(setWriteDB);	
	msg.SendToSocket(m_lGSSocketID);

	g_lFreezeBakNum++;
	RefeashInfoText();	

	return TRUE;
}
#include "stdafx.h"
#include "MsgSendTimeLimitMgr.h"
#include "../../../../Public/Module/Tools.h"
//ʱ����
ulong MsgSendTimeLimitMgr::m_Interleave[MST_NUM] = {
	0,			//������ͽű�
	2000,		//���а��ѯ
	0,			//mainbar�����ü����ȼ�
	0,			//����
};

//�ϴη���ʱ��
ulong MsgSendTimeLimitMgr::m_LastSendTime[MST_NUM] = {
	0,			//������ͽű�
	0,			//���а��ѯ
	0,			//mainbar�����ü����ȼ�
	0,			//����
	
};

bool MsgSendTimeLimitMgr::IsTimeOut(eMsgSendType e)
{
	if( GetCurTickCount() - m_LastSendTime[e] < m_Interleave[e])
	{
		MessageBox(g_hWnd,"����һ��ʱ����ٷ���","ERROR",MB_OK);
		return false;
	}
	else
		return true;
}

void MsgSendTimeLimitMgr::UpdateLastSendTime(eMsgSendType e)
{
	m_LastSendTime[e] = GetCurTickCount();
}

MsgSendTimeLimitMgr::MsgSendTimeLimitMgr()
{

}

MsgSendTimeLimitMgr::~MsgSendTimeLimitMgr()
{

}
#include "stdafx.h"
#include "../dbentity/EntityManager.h"



void OnDBOPMessage(CMessage* pMsg)
{
	// ����Ϣ�ض�����һ��session��
	GetGame()->GetEntityManager()->ProcessMsg(pMsg);
}

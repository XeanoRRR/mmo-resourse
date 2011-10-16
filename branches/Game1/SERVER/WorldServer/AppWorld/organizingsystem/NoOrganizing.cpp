#include "StdAfx.h"
#include ".\noorganizing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CNoOrganizing::CNoOrganizing(void)
{
}

CNoOrganizing::~CNoOrganizing(void)
{
}

//�ް��ɳ�Ա����
bool CNoOrganizing::ApplyForJoin(long lPlayerID,long lLvl,long lOrgMemID /*= 0*/)
{
	//��������Ա
	return true;
}
//������Ա����
bool CNoOrganizing::Invite(long lInvitedID, long lBeInvitedID)
{
	return true;
}
//�˳�
bool CNoOrganizing::Exit(long lPlayerID)
{
	return true;
}
//����
bool CNoOrganizing::FireOut(long lFireID,long lBeFiredID)
{	
	return true;
}
//����ƺ�
bool CNoOrganizing::DubAndSetJobLvl(long lDubID,long lBeDubedID,string& strTitle,long lLvl)
{	
	return true;
}
////������������Ȩ��
//bool CNoOrganizing::EndueRightOfConMem(long lEndueID,long lBeEnduedID)
//{	
//	return true;
//}
////���迪��Ȩ��
//bool CNoOrganizing::EndueRightOfFireOut(long lEndueID,long lBeEnduedID)
//{	
//	return true;
//}
//����
bool CNoOrganizing::Pronounce(long lPlayerID,string& strPro,tagTime& Time)
{	
	return true;
}
//����
bool CNoOrganizing::LeaveWord(long lPlayerID,string& strWords,tagTime& Time)
{
	return true;
}
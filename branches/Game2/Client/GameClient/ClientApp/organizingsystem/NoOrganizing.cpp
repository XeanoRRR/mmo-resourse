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

//���л��Ա����
bool CNoOrganizing::ApplyForJoin(CGUID& PlayerID,long lLvl,long lOrgMemID /*= 0*/)
{
	//��������Ա
	return true;
}
//������Ա����
bool CNoOrganizing::Invite(const CGUID& InvitedID, const CGUID& BeInvitedID)
{
	return true;
}
//�˳�
bool CNoOrganizing::Exit(const CGUID& PlayerID)
{
	return true;
}
//����
bool CNoOrganizing::FireOut(const CGUID& FireID,const CGUID& BeFiredID)
{	
	return true;
}
//����ƺ�
bool CNoOrganizing::DubAndSetJobLvl(const CGUID& DubID,const CGUID& BeDubedID,string& strTitle,long lLvl)
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
bool CNoOrganizing::Pronounce(const CGUID& PlayerID,string& strPro,tagTime& Time)
{	
	return true;
}
//����
bool CNoOrganizing::LeaveWord(const CGUID& PlayerID,string& strWords,tagTime& Time)
{
	return true;
}
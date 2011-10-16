#pragma once
#include "organizing.h"

class CNoOrganizing :
	public COrganizing
{
public:
	CNoOrganizing(void);
	~CNoOrganizing(void);

	//����֯��صĹ��ܲ���
public:
	bool ApplyForJoin(CGUID& PlayerID,long lLvl,long lOrgMemID = 0);		//���л��Ա�������
	bool Invite(const CGUID& InvitedID, const CGUID& BeInvitedID);					//������Ա����
	bool Exit(const CGUID& PlayerID);											//�˳�
	bool FireOut(const CGUID& FireID,const CGUID& BeFiredID);							//����
	bool DubAndSetJobLvl(const CGUID& DubID,const CGUID& BeDubedID,string& strTitle,long lLvl);//����ƺ�
	//bool EndueRightOfConMem(long lEndueID,long lBeEnduedID);			//������������Ȩ��
	//bool EndueRightOfFireOut(long lEndueID,long lBeEnduedID);			//���迪��Ȩ��
	bool Pronounce(const CGUID& PlayerID,string& strPro,tagTime& Time);		//����
	bool LeaveWord(const CGUID& PlayerID,string& strWords,tagTime& Time);		//����
};

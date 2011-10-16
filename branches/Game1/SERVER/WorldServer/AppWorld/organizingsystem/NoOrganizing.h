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
	bool ApplyForJoin(long lPlayerID,long lLvl,long lOrgMemID = 0);		//�ް��ɳ�Ա�������
	bool Invite(long lInvitedID, long lBeInvitedID);					//������Ա����
	bool Exit(long lPlayerID);											//�˳�
	bool FireOut(long lFireID,long lBeFiredID);							//����
	bool DubAndSetJobLvl(long lDubID,long lBeDubedID,string& strTitle,long lLvl);//����ƺ�
	//bool EndueRightOfConMem(long lEndueID,long lBeEnduedID);			//������������Ȩ��
	//bool EndueRightOfFireOut(long lEndueID,long lBeEnduedID);			//���迪��Ȩ��
	bool Pronounce(long lPlayerID,string& strPro,tagTime& Time);		//����
	bool LeaveWord(long lPlayerID,string& strWords,tagTime& Time);		//����
};

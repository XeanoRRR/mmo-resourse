//Function:	������һ�����࣬�ṩ�����ϵͳ��Ҳ��һ���ӿ���,
			//�ṩ�˶���֯�Ļ���������ÿ�����������������Щ������
			//���磬�����������Ա����ɢ��������
//Author:	Joe
//Create Time:2004.6.11

#pragma once
#include<list>
#include<map>
#include<vector>
#include <deque>
#include <queue>
#include <set>
using namespace std;

#include "..\public\OrganizingDef.h"


class COrganizing
{
public:
	COrganizing(void);
	virtual ~COrganizing(void);

	//�ṩ��������̳�ʹ�õĹ������
public:
	virtual BOOL AddToByteArray(vector<BYTE>& ByteArray);											//! ����
	virtual BOOL Decode(BYTE* pData, LONG &lPos);													//! ����

	virtual BOOL ApplyForJoin(const CGUID& PlayerGuid) = 0;											//! �������
	virtual BOOL ClearApplyList(void) = 0;															//! ��������б�
	virtual BOOL DoJoin(const CGUID& ExecuteGuid, const CGUID& BeExecutedGuid,LONG lAgree) = 0;		//! ����һ�������Ա
	virtual BOOL Invite(const CGUID& InvitedGuid, const CGUID& BeInvitedGuid) = 0;					//! �������
	virtual BOOL Exit(const CGUID& PlayerGuid) = 0;													//! �˳�
	virtual BOOL FireOut(const CGUID& FireID,const CGUID& BeFiredID) = 0;							//! ����
	virtual BOOL Demise(const CGUID& MasterID,const CGUID& ID) = 0;									//! ��λ
	virtual BOOL DubAndSetJobLvl(const CGUID& DubID,const CGUID& BeDubedID,string& strTitle) = 0;	//! ����ƺ�
	virtual const CGUID& GetMasterID() = 0;															//! �õ��쵼��ID
	virtual const CGUID& IsMember(const CGUID& guid) = 0;											//! ��Ա�ж�
	virtual LONG GetMemberNum() = 0;																//! �õ���Ա��

	const CGUID&	GetExID();																		//! �õ���֯ID
	const CGUID&	SetExID(const CGUID& guid) ;													//! ������֯ID
	const char*		GetName()const;																	//! �õ���֯������
	void			SetName(const char* pName);														//! ���ð�������

	virtual BOOL Disband(const CGUID& guid) = 0;													//! ��ɢ
	virtual BOOL Upgrade(const CGUID& PlayerGuid) = 0;												//! ����
	virtual LONG GetLvl() = 0;																		//! �õ��ȼ�
	virtual LONG SetLvl(LONG lLvl) = 0;																//! ���õȼ�
	virtual BYTE GetCountry() = 0;																	//! �õ�����ID
	virtual void SetCountry(BYTE c) = 0;															//! ���ù���ID
	virtual const CGUID&  GetSuperior(void) = 0;													//! �õ��ϼ�
	virtual void  SetSuperior(const CGUID&) = 0;													//! �����ϼ�

	virtual LONG GetEstablishedTime()const = 0 ;													//! �õ��ý���ʱ��

	virtual BOOL Pronounce(const CGUID& PlayerGuid, const char *pPro) = 0;							//! ����
	virtual BOOL LeaveWord(const CGUID& PlayerGuid, const char *pWords) = 0;						//! ����
	virtual BOOL EditLeaveWord(const CGUID& PlayerGuid,const CGUID& WordGuid,eOperator eOP) = 0;	//! �༭����
	virtual void Talk(const CGUID& PlayerGuid, const char *pSourceName, const char *pTxt) = 0;		//! ��֯����

	virtual BOOL Initial() = 0;																		//! ��ʼ��
private:
	CGUID m_guid;
	char  m_szName[16];

};
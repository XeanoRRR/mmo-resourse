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

#include "../public/OrganizingDef.h"
#include "..\..\..\public\WSUseMemManage.h"
class CPlayer;

class COrganizing:public BaseMemObj
{
public:
	COrganizing(void);
	virtual ~COrganizing(void);

	//�ṩ��������̳�ʹ�õĹ������
public:
	virtual const CGUID&	GetExID(void) = 0;														//! �õ���֯ID
	virtual const CGUID&	SetExID(const CGUID& guid)  = 0;										//! ������֯ID
	virtual const char*		GetName(void)const = 0;													//! �õ���֯������
	virtual void			SetName(const char* pName) = 0;											//! ���ð�������

	virtual BOOL ApplyForJoin(const CGUID& PlayerGuid) = 0;											//! �������
	virtual BOOL ClearApplyList(void) = 0;															//! ��������б�
	virtual BOOL AddMeber(CPlayer *pPlayer, const CGUID &InviterGuid = NULL_GUID) = 0;				//! ��ӳ�Ա
	virtual BOOL Exit(const CGUID& PlayerGuid) = 0;													//! �˳�
	virtual BOOL FireOut(const CGUID& FireID,const CGUID& BeFiredID) = 0;							//! ����
	virtual BOOL Demise(const CGUID& MasterID,const CGUID& ID) = 0;									//! ��λ
	virtual BOOL DubAndSetJobLvl(const CGUID& DubID,const CGUID& BeDubedID,string& strTitle) = 0;	//! ����ƺ�
	virtual const CGUID& GetMasterID(void) = 0;														//! �õ��쵼��ID
	virtual const CGUID& IsMember(const CGUID& guid) = 0;											//! ��Ա�ж�
	virtual LONG GetMemberNum(void) = 0;															//! �õ���Ա��

	

	virtual BOOL Disband(const CGUID& guid) = 0;													//! ��ɢ
	virtual BYTE GetCountry(void) = 0;																//! �õ�����ID
	virtual void SetCountry(BYTE c) = 0;															//! ���ù���ID
	
	virtual LONG GetEstablishedTime(void)const = 0 ;												//! �õ��ý���ʱ��

	virtual BOOL Pronounce(CPlayer *pPlayer, const char *pPro) = 0;									//! ����
	virtual void Talk(const CGUID& PlayerGuid, const char *pSourceName, const char *pTxt) = 0;		//! ��֯����

	virtual BOOL Initial(void) = 0;																	//! ��ʼ��
};

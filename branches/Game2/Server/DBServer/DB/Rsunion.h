#pragma once
#include "..\myadobase.h"

#include "..\..\server\dbserver\appworld\DBUnion.h"
//#include "..\..\server\dbserver\appworld\Organizingsystem\organizingctrl.h"

class CRsUnion :
	public CMyAdoBase
{
public:
	CRsUnion(void);
	~CRsUnion(void);

public:
	//long AddConfederation(const CGUID& lMasterID,const char* strName);				//���һ���˵ļ�¼
	//bool DelConfederation(const CGUID& lConfederID,_ConnectionPtr& cn);		//ɾ��һ���˵ü�¼
	//bool LoadConfederation(CDBUnion* pConf);							//װ����
	//bool SaveConfederation(CDBUnion* pConf,_ConnectionPtr& cn);	//������
	//int LoadAllConfederation();									//װ��������
	//bool SaveAllConfederation();
	//bool LoadConfeMembers(CDBUnion *pConf,_ConnectionPtr& cn);			//װ���˳�Ա
	//bool GetUnionMemInfo(tagMemInfo& MemInfo,_ConnectionPtr& cn);//�õ�ͬ�˳�Ա��Ϣ
	//bool SaveConfeMembers(CDBUnion *pConf,_ConnectionPtr& cn);			//�����˳�Ա
};

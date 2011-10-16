
/*
*	file:		RsLinkmanManage.h
*	Brief:		����ϵͳ����
*	detail:		
*				
*				
*	Authtor:	�ſ���
*	Datae:		2008-02-21
*/
#pragma  once


#include "..\public\GUID.h"
#include "..\nets\netdb\message.h"
#include "../myadobase.h"
#include "../public/LinkmanDef.h"
#include <set>


class RsLinkmanManage
	: public CMyAdoBase
{
public:
	//!								�õ�Ψһʵ��
	static		RsLinkmanManage&	GetRsLinkmanManage(void);
	
public:
	//!								�����ݿ��ж�ȡ��ʼ������
	void							LoadPlayerInitData_FromDB(const CGUID &PlayerGuid, _ConnectionPtr &cn, vector<tagLinkman_DB>& vData);
	//!								����һ����ϵ�˼�¼
	BOOL							SaveLinkman(tagLinkman_DB &Linkman_DB, _ConnectionPtr &cn);
	//!								ɾ��һ����ϵ�˼�¼
	BOOL							DeleteLinkman(tagLinkman_DB &Linkman_DB, _ConnectionPtr &cn);

private:
	//!								Ҫ��ȡ�� AimGUID�ֶ�Ϊ����ID��GroupID�ֶ�ֵ
	set<LONG>						m_ExceptiveGroupID;

};
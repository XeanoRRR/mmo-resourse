

/*
*	file:		RsLinkmanManage.h
*	Brief:		����ϵͳ���ݿ��������
*	detail:		
*				
*				
*	Authtor:	�ſ���
*	Datae:		2008-02-25
*/

#pragma once

#include "..\..\..\dbaccess\worlddb\RsLinkman.h"

class DbLinkmanManage
{
public:
	DbLinkmanManage(void);
	~DbLinkmanManage(void);

	//!									�õ�Ψһʵ��
	static		DbLinkmanManage&		GetDbLinkmanManage				(void);

public:

	//!									����һ����ҵ���ϵ�����ݵ�WS
	void								InitLinkmanDataToWs				(const CGUID& PlayerGuid);
	//!									��ӦWS������д����Ϣ
	void								OnLinkmanWriteData				(CMessage *pMsg);


protected:
private:
	vector<tagLinkman_DB>	m_vData;
	
};
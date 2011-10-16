



#include "GameFaction.h"

#pragma once

class GameUnion
{
public:
	//! 
	GameUnion(void);
	//! 
	~GameUnion(void);

	//!	��������
	///////////////////////////////////////////////////////////////////////////////////////
public:
	//!										����
	BOOL									AddToByteArray(vector<BYTE>& ByteArray);
	//!										����
	BOOL									Decode(BYTE* pData, LONG &lPos);

	//!										���������Ϣ����Ϣĩβ
	void									AddBaseToMsg(CMessage *pMsg);
	//!										�����Ա��Ϣ����Ϣĩβ
	BOOL									AddOneMemberToMsg(const CGUID &FactionGuid, CMessage *pMsg);
	BOOL									AddOneMemberToMsg(CPlayer *pPlayer, CMessage *pMsg);
	//!										�����Ա��Ϣ����Ϣĩβ
	void									AddAllMemberToMsg(CMessage *pMsg);

public:
	//!										�õ���֯ID
	const	CGUID&							GetExID(void)const;
	//!										�õ���֯������
	const	char*							GetName(void)const;
	//!										�õ�����ID
	const	CGUID&							GetMasterID(void)const;
	//!										�õ�����������
	const	char*							GetMasterName(void)const;

	//!										����³�Ա
	BOOL									AddNewMember(GameFaction *pMember);
	//!										ɾ����Ա
	BOOL									DelMember(const CGUID& MemberGuid);
	//!										�жϳ�Ա
	BOOL									IsMember(const CGUID& FactionGuid);
	//!										�޸�����
	BOOL									ChangeMaster(const CGUID& NewMasterFacGuid, const char *pNewMasterName);
	//!										���ó�ԱȨ��
	BOOL									SetPurview(const CGUID& MemberGuid, DWORD dwNewPurview);
	//!										�õ���ԱȨ��
	DWORD									GetPurview(const CGUID& MemberGuid);
	//!										��ɢ
	BOOL									Disband(void);

	//!										��Ӧ���˽���
	void									OnCreate(void);


private:
	//!										������֯ID
	const	CGUID&							SetExID(const CGUID& guid);
	//!										���ð�������
	void									SetName(const char* pName);
	//!										��������ID
	const	CGUID&							SetMasterID(const CGUID& guid);
	//!										��������������
	void									SetMasterName(const char* pName);


	//!	For DBManager
	///////////////////////////////////////////////////////////////////////////////////////
public:
	//!										�õ�������Ϣ
	const	tagUnionBaseData&				GetUnionBaseData(void);
	//!										�õ���Ա�б�
	const	map<CGUID, tagUnionMemInfo>&	GetUnionMemInfoMap(void);



	//!	
	///////////////////////////////////////////////////////////////////////////////////////
private:
	//!										��������
	tagUnionBaseData						m_UnionBaseData;
	//!										��Ա�б�
	map<CGUID, tagUnionMemInfo>				m_mapUnionMemInfo;
};

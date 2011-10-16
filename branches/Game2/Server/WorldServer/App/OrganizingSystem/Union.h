class CFaction;

#pragma once

class CUnion
{
public:
	//! ���ڴ�DB���ݹ����Դ��ڵĶ���
	CUnion(void);
	//! ������Ϸ����Ҵ����µĶ���ʧ�ܻ��׳��쳣��
	CUnion(const CGUID &Guid, const char *pName, CFaction *pMaster);
	//! 
	~CUnion(void);

	//!	��������
	///////////////////////////////////////////////////////////////////////////////////////
public:
	//!										��һ�α���
	void									Insert(VOID);
	//!										����
	void									Save(VOID);
	//!										����
	BOOL									AddToByteArray(vector<BYTE>& ByteArray);
	//!										����
	BOOL									Decode(BYTE* pData, LONG &lPos);

public:
	//!										�õ���֯ID
	const	CGUID&							GetExID(void)const;
	//!										�õ���֯������
	const	char*							GetName(void)const;
	//!										�õ�����ID
	const	CGUID&							GetMasterID(void)const;
	//!										�õ�����������
	const	char*							GetMasterName(void)const;

	//!										������֯ID
	const	CGUID&							SetExID(const CGUID& guid);
	//!										���ð�������
	void									SetName(const char* pName);
	//!										��������ID
	const	CGUID&							SetMasterID(const CGUID& guid);
	//!										��������������
	void									SetMasterName(const char* pName);


	//!										����³�Ա
	BOOL									AddNewMember(CFaction *pMember, BOOL bIsMaster = FALSE);
	//!										ɾ����Ա
	BOOL									DelMember(const CGUID& MemberGuid);
	//!										�޸�����
	BOOL									ChangeMaster(const CGUID& NewMasterGuid);
	//!										���ó�ԱȨ��
	BOOL									SetPurview(const CGUID& MemberGuid, DWORD dwNewPurview);
	//!										��ɢ
	BOOL									Disband(VOID);
	

	
	
	


	//!	For DBManager
	///////////////////////////////////////////////////////////////////////////////////////
public:
	//!										�õ�������Ϣ
	tagUnionBaseData&						GetUnionBaseData(VOID)		{return m_UnionBaseData;}
	//!										�õ���Ա�б�
	map<CGUID, tagUnionMemInfo>&			GetUnionMemInfoMap(VOID)	{return m_mapUnionMemInfo;}



	//!	
	///////////////////////////////////////////////////////////////////////////////////////
private:
	//!										��������
	tagUnionBaseData						m_UnionBaseData;
	//!										��Ա�б�
	map<CGUID, tagUnionMemInfo>				m_mapUnionMemInfo;

public:
	//!										���ݱ�����
	DWORD									m_dwDataChanged;
};





#pragma once

/*
	����
*/
class HardInfoList
{
public:
	HardInfoList(DWORD dwSMID);
	~HardInfoList(VOID);

public:
	//!
	VOID							Release(VOID);

	//!								���һ��Ӳ����Ϣ�ڵ�
	BOOL							AddNode(tagHardwareInfo &HardwareInfo, BOOL bSendToUser = FALSE);
	//!								���һ��Ӳ����Ϣ�ڵ�
	BOOL							AddNodeList(tagHardwareInfo *pHardwareInfo, LONG lNum);

	//!								����������Ϣ���ͻ���
	VOID							SendInfoToSocket(LONG lSocket);
	//!								����������Ϣ�����пͻ���
	VOID							SendInfoToAll(VOID);

public:
	//!								��Ӧд�����
	static		VOID				OnDBWriteFinish(tagHardwareInfo *pHardwareInfo, DWORD dwNum, DWORD dwSMID = 0);

private:
	//! һ��Ӳ����Ϣ�ڵ�
	struct tagHardInfoGroup
	{
		tagHardwareInfo	*pHardwareInfo;
		DWORD			dwUseNum;

		tagHardInfoGroup(VOID){memset(this, 0, sizeof(tagHardInfoGroup));}
	};

private:
	//!
	VOID							AddGroup(tagHardInfoGroup &HardInfoGroup);
	//!								���������ݿ�д������
	VOID							RequestWriteDB(tagHardInfoGroup &HardInfoGroup);
	//!								�õ���ǰ�������������
	VOID							GetCacheAllInfo(tagHardInfoGroup &HardInfoGroup);

private:
	DWORD							m_dwSMID;
	//!
	tagHardInfoGroup				m_CurrGroup;
	//!
	list<tagHardInfoGroup>			m_queueCacheInfo;

private:
	//!										����д�����ݿ������
	static		set<tagHardwareInfo*>		m_setWriting;
};
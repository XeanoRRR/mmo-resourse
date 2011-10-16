


#pragma once

//! ǰ�ýṹ����
///////////////////////////////////////////////////////////////////////
//! ��������ڴ�����
struct tagMemSetup
{
	DWORD dwDataBlockNum;				//���ݿ�����
	DWORD dwDataBlockSize;				//���ݿ��С
	DWORD dwFreeMsgNum;					//Ԥ�������Ϣ����
};

//! �����������
struct tagServerSetup
{
	DWORD dwListenOrConnectPort;		// �����������Ӷ˿�
	DWORD dwFreeSockOperNum;			// �����������Ԥ��������
	DWORD dwFreeIOOperNum;				// ��ɶ˿���IO����Ԥ��������
	DWORD dwIOOperDataBufNum;			// Ĭ��IO������DataBuf����

	DWORD dwCheckNet;					// �Ƿ��������м��
	DWORD dwBanIPTime;					// ��ֹIP��ʱ��
	DWORD dwMaxMsgLen;					// ������������Ϣ����		
	DWORD dwMaxConnectNum;				// �ͻ��˵������������
	DWORD dwMaxClientsNum;				// ���Ŀͻ��˷��ͻ�������С
	DWORD dwPendingWrBufNum;			// ���ķ���IO����Buf�ܴ�С
	DWORD dwPendingRdBufNum;			// ���Ľ���IO����Buf�ܴ�С
	DWORD dwMaxSendSizePerSecond;		// ��ͻ���ÿ�뷢�͵�����ֽ���
	DWORD dwMaxRecvSizePerSecond;		// �ӿͻ��˽��ܵ�ÿ������ֽ���
	DWORD dwMaxBlockedSendMsgNum;		// ��������ķ�����Ϣ����
	DWORD dwConPendingWrBufNum;			// �ͻ������ķ���IO����Buf�ܴ�С
	DWORD dwConPendingRdBufNum;			// �ͻ������Ľ���IO����Buf�ܴ�С
	DWORD dwConMaxSendSizePerSecond;	// ����������͵�ÿ������ֽ���
	DWORD dwConMaxRecvSizePerSecond;	// �ӷ��������ܵ�ÿ������ֽ���
	DWORD dwConMaxBlockedSendMsgNum;	// ��������ķ�����Ϣ����
};

//! �������ù�����
//! ��ȡ��������������
////////////////////////////////////////////////////////////////////////
class NetSetup
{
public:
	NetSetup(VOID);
	~NetSetup(VOID);
	static						NetSetup&					GetInstance(VOID);

public:
	//!														��ȡ����
	BOOL													LoadSetup(const char *pXmlFilename);
	//!														�õ���������ڴ�����
	const						tagMemSetup*				GetMemSetup(VOID);
	//!														�õ������������
	const						tagServerSetup*				GetServerSetupByID(LONG lSetupID);


private:
	static						NetSetup					c_NetSetup;

private:
	//!														ֻ��һ���ڴ�����
	tagMemSetup												m_MemSetup;
	//!														������������������
	map<LONG, tagServerSetup>								m_mapServerSetup;
	//!														��ʼ����ɱ��
	BOOL													m_bIsReady;
};
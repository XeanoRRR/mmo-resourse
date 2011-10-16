

#include "stdafx.h"
#include "NetSetup.h"


NetSetup NetSetup::c_NetSetup;

NetSetup::NetSetup(VOID)
:m_bIsReady(FALSE)
{

}
NetSetup::~NetSetup(VOID)
{

}

NetSetup& NetSetup::GetInstance(VOID)
{
	return c_NetSetup;
}

//! �õ���������ڴ�����
const tagMemSetup* NetSetup::GetMemSetup(VOID)
{
	return (m_bIsReady) ? (&m_MemSetup) : NULL;
}

//! �õ������������
const tagServerSetup* NetSetup::GetServerSetupByID(LONG lSetupID)
{
	if(m_bIsReady)
	{
		map<LONG, tagServerSetup>::iterator ite = m_mapServerSetup.find(lSetupID);
		if(m_mapServerSetup.end() != ite)
			return &(ite->second);
	}
	return NULL;
}

//! ��ȡ����
BOOL NetSetup::LoadSetup(const char *pXmlFilename)
{
	if(NULL == pXmlFilename)
		return FALSE;

	TiXmlNode *pNode = NULL; 		

	TiXmlDocument* m_Tdoc = new TiXmlDocument(pXmlFilename);
	if(!m_Tdoc->LoadFile())
		return FALSE;

	pNode = m_Tdoc->FirstChild("NetSetup");

	if (pNode == NULL)
	{
		SAFE_DELETE(m_Tdoc);
		return FALSE;
	}

	TiXmlElement* pNetSetup = pNode->ToElement();//��ȡnode ��ָ��

	if (pNetSetup == NULL)
	{
		SAFE_DELETE(m_Tdoc);
		return FALSE;
	}

	////////////////////////////////////////////
	m_bIsReady = FALSE;
	m_mapServerSetup.clear();

	m_MemSetup.dwDataBlockNum	= atoi(pNetSetup->Attribute("DataBlockNum"));
	m_MemSetup.dwDataBlockSize	= atoi(pNetSetup->Attribute("DataBlockSize"));
	m_MemSetup.dwFreeMsgNum		= atoi(pNetSetup->Attribute("FreeMsgNum"));

	TiXmlElement* pServerSetup = pNetSetup->FirstChildElement("ServerSetup");
	for (; pServerSetup != NULL; pServerSetup = pServerSetup->NextSiblingElement("ServerSetup"))
	{
		tagServerSetup ServerSetup;
		LONG lID = atoi(pServerSetup->Attribute("ID"));

		ServerSetup.dwListenOrConnectPort		= atoi(pServerSetup->Attribute("ListenOrConnectPort"));		// �����������Ӷ˿�
		ServerSetup.dwFreeSockOperNum			= atoi(pServerSetup->Attribute("FreeSockOperNum"));			// �����������Ԥ��������
		ServerSetup.dwFreeIOOperNum				= atoi(pServerSetup->Attribute("FreeIOOperNum"));			// ��ɶ˿���IO����Ԥ��������
		ServerSetup.dwIOOperDataBufNum			= atoi(pServerSetup->Attribute("IOOperDataBufNum"));		// Ĭ��IO������DataBuf����

		ServerSetup.dwCheckNet					= atoi(pServerSetup->Attribute("CheckNet"));				// �Ƿ��������м��
		ServerSetup.dwBanIPTime					= atoi(pServerSetup->Attribute("BanIPTime"));				// ��ֹIP��ʱ��
		ServerSetup.dwMaxMsgLen					= atoi(pServerSetup->Attribute("MaxMsgLen"));				// ������������Ϣ����		
		ServerSetup.dwMaxConnectNum				= atoi(pServerSetup->Attribute("MaxConnectNum"));			// �ͻ��˵������������
		ServerSetup.dwMaxClientsNum				= atoi(pServerSetup->Attribute("MaxClientsNum"));			// ���Ŀͻ��˷��ͻ�������С
		ServerSetup.dwPendingWrBufNum			= atoi(pServerSetup->Attribute("PendingWrBufNum"));			// ���ķ���IO����Buf�ܴ�С
		ServerSetup.dwPendingRdBufNum			= atoi(pServerSetup->Attribute("PendingRdBufNum"));			// ���Ľ���IO����Buf�ܴ�С
		ServerSetup.dwMaxSendSizePerSecond		= atoi(pServerSetup->Attribute("MaxSendSizePerSecond"));	// ��ͻ���ÿ�뷢�͵�����ֽ���
		ServerSetup.dwMaxRecvSizePerSecond		= atoi(pServerSetup->Attribute("MaxRecvSizePerSecond"));	// �ӿͻ��˽��ܵ�ÿ������ֽ���
		ServerSetup.dwMaxBlockedSendMsgNum		= atoi(pServerSetup->Attribute("MaxBlockedSendMsgNum"));	// ��������ķ�����Ϣ����
		ServerSetup.dwConPendingWrBufNum		= atoi(pServerSetup->Attribute("ConPendingWrBufNum"));		// �ͻ������ķ���IO����Buf�ܴ�С
		ServerSetup.dwConPendingRdBufNum		= atoi(pServerSetup->Attribute("ConPendingRdBufNum"));		// �ͻ������Ľ���IO����Buf�ܴ�С
		ServerSetup.dwConMaxSendSizePerSecond	= atoi(pServerSetup->Attribute("ConMaxSendSizePerSecond"));	// ����������͵�ÿ������ֽ���
		ServerSetup.dwConMaxRecvSizePerSecond	= atoi(pServerSetup->Attribute("ConMaxRecvSizePerSecond"));	// �ӷ��������ܵ�ÿ������ֽ���
		ServerSetup.dwConMaxBlockedSendMsgNum	= atoi(pServerSetup->Attribute("ConMaxBlockedSendMsgNum"));	// ��������ķ�����Ϣ����

		m_mapServerSetup[lID] = ServerSetup;
	}
	
	////////////////////////////////////////////
	m_bIsReady = TRUE;
	SAFE_DELETE(m_Tdoc);
	return TRUE;
}

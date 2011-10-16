
#include "StdAfx.h"
#include "public.h"
#include "PlayerSetupOnServer.h"


PlayerSetupOnServer::PlayerSetupOnServer(void)
{
	Reset();
}
PlayerSetupOnServer::~PlayerSetupOnServer(void)
{

}



//! ����
void PlayerSetupOnServer::AddToByteArray(vector<BYTE> &ByteArray)
{
	_AddToByteArray(&ByteArray, &m_SetupOnServer, sizeof(tagSetupOnServer));
}

//! ����
void PlayerSetupOnServer::GetFormByteArray(BYTE *pByteArray, long &lPos)
{
	_GetBufferFromByteArray(pByteArray, lPos, &m_SetupOnServer, sizeof(tagSetupOnServer));
}

//!								����
void PlayerSetupOnServer::CodeToDataBlock(DBWriteSet& setWriteDB)
{
	setWriteDB.AddToByteArray(&m_SetupOnServer, sizeof(tagSetupOnServer));
}
//!								����
void PlayerSetupOnServer::DecodeFromDataBlock(DBReadSet& setReadDB)
{
	setReadDB.GetBufferFromByteArray(&m_SetupOnServer, sizeof(tagSetupOnServer));
}

//! ���ö���
void PlayerSetupOnServer::Reset(void)
{
	memset(&m_SetupOnServer, 0, sizeof(tagSetupOnServer));
}

//! ���һ�����ã�uType��
const tagSetupOnServer* PlayerSetupOnServer::GetSetup(void)
{
	return &m_SetupOnServer;
}

//! �޸�һ������
BOOL PlayerSetupOnServer::SetSetup(const tagSetupOnServer &SetupOnServer)
{
	//! ��ͬ�Ķ���ֵ����������
	if(0 == memcmp(&m_SetupOnServer, &SetupOnServer, sizeof(tagSetupOnServer)))
		return FALSE;

	memmove(&m_SetupOnServer, &SetupOnServer, sizeof(tagSetupOnServer));
	return TRUE;
}
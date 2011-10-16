
#include "StdAfx.h"
#include "PlayerSetupOnServer.h"


PlayerSetupOnServer::PlayerSetupOnServer(void)
{
	Reset();
}
PlayerSetupOnServer::~PlayerSetupOnServer(void)
{

}



//! ����
void PlayerSetupOnServer::AddToByteArray(vector<uchar> &ByteArray)
{
	_AddToByteArray(&ByteArray, &m_SetupOnServer, sizeof(tagSetupOnServer));
}

//! ����
void PlayerSetupOnServer::GetFormByteArray(uchar *pByteArray, long &lPos)
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
bool PlayerSetupOnServer::SetSetup(const tagSetupOnServer &SetupOnServer)
{
	//! ��ͬ�Ķ���ֵ����������
	if(0 == memcmp(&m_SetupOnServer, &SetupOnServer, sizeof(tagSetupOnServer)))
		return false;

	memmove(&m_SetupOnServer, &SetupOnServer, sizeof(tagSetupOnServer));
	return true;
}

/*
*	file:		PlayerSetupOnServer.h
*	Brief:		��ҵ���Ϸ���ã���Ҫ�ڷ���������Ĳ���
*	detail:		�ṩ����ı����ӿ�
*	Authtor:	�ſ���
*	Datae:		2008-06-30
*/

#pragma once

//! ���ݶ��󣨿ͻ��˿ɰ����޸ģ�
//###############################################################
struct tagSetupOnServer
{

};





//! ���ܶ���
//###############################################################
class PlayerSetupOnServer
{
public:
	PlayerSetupOnServer(void);
	~PlayerSetupOnServer(void);


public:
	//!								����
	void							AddToByteArray		(vector<BYTE> &ByteArray);
	//!								����
	void							GetFormByteArray	(BYTE *pByteArray, long &lPos);

	//!								����
	void							CodeToDataBlock		(DBWriteSet& setWriteDB);
	//!								����
	void							DecodeFromDataBlock	(DBReadSet& setReadDB);


	//!								���ö��󣨻ָ�Ĭ�����ã�
	void							Reset				(void);

	//!								�������
	const	tagSetupOnServer*		GetSetup			(void);
	//!								�޸�����
	BOOL							SetSetup			(const tagSetupOnServer &SetupOnServer);

private:
	//!								�Զ���ҩ����������
	tagSetupOnServer				m_SetupOnServer;

};
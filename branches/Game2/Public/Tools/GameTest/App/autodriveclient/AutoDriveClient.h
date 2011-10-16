/*****
*	File       : AutoDriveClient.h
*	Directions : Test����Client��ͷ�ļ�
*	Type	   : ������(����DriveGameClient�ࡢCMemclient��)
*	by		   : tanhaowen 
*	day		   : 2009.1.16 
*/

#pragma once 

#include "DriveGameClient.h"
#include "DrivePlayer.h"
#include "DriveGameClient.h"

#include "..\Player.h"
#include "..\ClientRegion.h"


class CDriveGameClient;
class CDrivePlayer;
class CDriveRegion;
class CMemServer;

class CDriveClient
{
private:
	map<string, CGameClient *>	     m_mapGameClientManager;												//�ͻ��˹���(���),�����ʺŽ��й��� 

public:	

	map<string, CGameClient *>      &_GetGameClientManager()			{ return m_mapGameClientManager; }  //�ͻ��˹�����
	CGameClient						*_FindGameClient(string cdkey);											//����
	void							 _DeleteGameClient(string cdkey);										//ɾ������
	void							 _AddGameClient(string cdkey, CGameClient *gameclient);					//��ӵ���	
	CGameClient						*_CreateGameClient(string cdkey, unsigned uID, Timer *timer = NULL);	//�����ͻ���,����GameClient ʹ�ö�̬	
	void							 _ReleaseGameClientManager();											//�ͷŹ�����,���						

	//***
	//	Mapping File
	//*
private:

	static CMemServer				*m_pMemServerManager;													//MappingFile����ָ�����,��Ҫ�ͷ�!	
	unsigned						 m_uID;																	//���洴����severӳ���ļ����߳�ID
public:

	CMemServer						*GetMemServerManager()		{ return m_pMemServerManager; }
	static CMemServer				*GetMemServerInstance();
public:

	//Create fileMapping
	unsigned						_CreateFileMapping(const char *pFileobj, 
		const char *pFileName,
		const char *pFileDir);
	//init fileMapping
	void							 _InitialFileMapping(CDataBlockAllocator *pDBAllocator);
	//Write fileMapping
	void							 _WriteFileMapping(unsigned uID, CBaseMessage *pMsg);
	//Get fileMappingMsgQueue
	CMsgQueue						*_GetRecvMsgs()				{ return m_pMemServerManager->GetRecvMsgs(); }
	//Release fileMaping resource
	void							 _ReleaseFileMapping();
	//get fileMapping uID
	unsigned						 _GetUId()					{ return m_uID; }

public:

	CDriveClient();
	~CDriveClient();
};
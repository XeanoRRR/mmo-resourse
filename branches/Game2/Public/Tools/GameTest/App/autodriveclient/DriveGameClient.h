/*****
*	File       : DriveGameClient.h
*	Directions : Test������GameClient��ʱ����������ݼ�����
*	Type	   : ����(��class CDriveClient����), ������DrivePlayer��DriveClientRegion
*	by		   : tanhaowen 
*	day		   : 2009.1.16 
*/
#ifndef _DriveGameClient_H_
#define _DriveGameClient_H_
#pragma once 
#include "../GameClient.h"
#include "DrivePlayer.h"
// 
// #include "..\GameClient.h"
// #include "..\Player.h"
// #include "..\ClientRegion.h"
// #include "..\..\Utility\TestMsgType.h"
// //#include "..\..\Net\MemServer.h"
// //#include "../../../nets/MemServer.h"
// //#include "AutoDriveClient.h"
// 
// class CDriveClientDlg;
// class CDriveGameClient;
// class CDrivePlayer;

//extern CRITICAL_SECTION g_csMapFile;

//���ڶ��Ǽ̳�Test�������ݣ�����ֱ�����Ϊʹ�ö�̬��������������
class CDriveGameClient : public CGameClient
{
private:
	//���������
	CPlayer			 *m_pDrivePlayer;
	//����Ϸ����			
	CClientRegion	 *m_pDriveRegion;

/*������Ϊ˽�У���ȷʵ��Ҫ�õ��ڹ���*/	
private:										
	//�����������������
	CPlayer			 *CreateMainPlayer()		{ return !m_pDrivePlayer ? new CDrivePlayer : m_pDrivePlayer; }
	//������������
	CClientRegion	 *CreateMainRegion()		{ return !m_pDriveRegion ? new CClientRegion : m_pDriveRegion; }							
	
	//�������������
	void			  SetDriveMainPlayer(CPlayer *pPlayer);  
	//��������Ϸ����
	void			  SetDriveMainRegion(CClientRegion *pRegion);

public:
	//��ȡ���������
	CPlayer			 *GetDriveMainPlayer()		{ return !m_pDrivePlayer ? m_pDrivePlayer : NULL; }
	//��ȡ����Ϸ����
	CClientRegion    *GetDriveMainRegion()		{ return !m_pDriveRegion ? m_pDriveRegion : NULL; }
	
//private:
	//����MSG_FILE msg
	//msgQueue		  MsgQueueMSG_FILE;

//public:
//	void		AddMsgQueueMSG_FILE(CBaseMessage *pMsg)		{ MsgQueueMSG_FILE.push_back(pMsg); }
//	msgQueue   *GetMsgQueueMSG_FILE()						{ return &MsgQueueMSG_FILE; }
//	void		ReleaseMapFileMsg();						
//��ʱ����Camera���ݲ�����Camera Matrix
//public:
//	float		m_fCameraPosX;
//	float		m_fCameraPosY;
//	float		m_fCameraPosZ;
//
//	D3DXVECTOR3 m_vecPosition;
//	D3DXVECTOR3 m_vecView;
//
//	D3DXMATRIX  m_ViewMatrix;
//	D3DXMATRIX  m_ProjectionMatrix;
	
//---------------------------------------------------------------------------
//��ʼ������
//----------------------------------------------------------------------------
public:
	//�����
	void			 InitPlayerData(CMessage *pMsg);
	//����
	void			 InitGameRegionData(CMessage *pMsg);
	//�������ӵ����и߶� ClientRegion::
	void			 InitGameRgionTileHeight(CMessage *pMsg);

//private:
	// MappingFile File Operation , Client to Test
	//void			 MsgFile(CMessage *pMsg);
	//void			 MsgFileWriteMappingFile(CMessage *pMsg);
	//void			 MsgFileClearMappingFile(CMessage *pMsg);
	//void			 MsgFileOpenMappingFile(CMessage *pMsg);
	//void			 MsgFileloseMappingFile(CMessage *pMsg);
	//
//------------------------------------------------------------------------------
//��Ϣ����
//------------------------------------------------------------------------------
public:
	//ʵ��ԭ�� ��̬����GameClient::OnMessage();
	void			 OnMessage(CMessage *pMsg);

	// MappingFile Msg, to do you function here
	//------------------------------------------
	
	// MSG_C2T_LOG
	void			 C2TLog(CMessage *pMsg);
	
	void			 C2TLogAnswerPlayerData(CMessage *pMsg);

	//MSG_C2T_RGN
	void			 C2TRgn(CMessage *pMsg);
				
	void			 C2TRgnAddShape(CMessage *pMsg);			// �ж�����볡��(��ӦMSG_S2C_RGN_ADDSHAPE = 783618)
	void			 C2TRgnDelShape(CMessage *pMsg);			// �ж����뿪����(��ӦMSG_S2C_RGN_DELSHAPE = 783620)
	void			 C2TRgnChange(CMessage *pMsg);				// ��������������л�����(��ӦMSG_S2C_RGN_CHANGE)
	void			 C2TRgnChangeServer(CMessage *pMsg);		// ��������������л�����������(��ӦMSG_S2C_RGN_CHANGESERVER)
	void			 C2TRgnChangeWeather(CMessage *pMsg);		// �ı�����
	void			 C2TRgnPlayAction(CMessage *pMsg);			// ���Ŷ���
	void			 C2TRgnChangeName(CMessage *pMsg);			// �ı䳡������(��ӦMSG_S2C_RGN_CHANGENAME)
	void			 C2TRgnClearDupRgnPlayer(CMessage *pMsg);	// ����������Ϣ(��ӦMSG_S2C_RGN_CLEARDUPRGNPLAYER)
	//MSG_L2T_Log
	void			L2TLog(CMessage *pMsg);
	//
	//MSG_C2T_LOGIC
	void			C2TLogic(CMessage *pMsg);
	
	//MSG_C2T_LOGIC_UIISOPENED
	void			C2TLogicUiIsOpend(CMessage *pMsg);
	
	// MSG_C2T_SHAPEE
	void			 C2TShape(CMessage *pMsg);
	
	void			 C2TShapeChangeDir(CMessage *pMsg);
	void			 C2TShapeSetPos(CMessage *pMsg);
	void			 C2TShapeForceMove(CMessage *pMsg);
	void			 C2TShapeMove(CMessage *pMsg);
	void			 C2TShapeStopMove(CMessage *pMsg);
	void			 C2TShapeMoveMode(CMessage *pMsg);
	void			 C2TShapeChangeState(CMessage *pMsg);
	void			 C2TShapeAtkDied(CMessage *pMsg);
	void			 C2TShapeSinState(CMessage *pMsg);
	void			 C2TShapeRefresh(CMessage *pMsg);
	void			 C2TShapeEmotion(CMessage *pMsg);
	
	void			 C2TShapeAtkBreak(CMessage *pMsg);
	void			 C2TShapeAtkDamage(CMessage *pMsg);
	void			 C2TShapeChangeProperties(CMessage *pMsg);
	
	
	// MSG_C2T_PLAYER
	void			 C2TPlayer(CMessage *pMsg);

	// MSG_C2T_GOODS
	void			 C2TGoods(CMessage *pMsg);
	
	//��Ϣ���� end
public:
	//����ClientҪ���ڴ�ӳ���ļ����в��������Դ���MappingFile�߳�ID
	CDriveGameClient(unsigned uID);
	CDriveGameClient(Timer *pTimer, unsigned uID);
	~CDriveGameClient();
	
};
#endif//_DriveGameClient_H_
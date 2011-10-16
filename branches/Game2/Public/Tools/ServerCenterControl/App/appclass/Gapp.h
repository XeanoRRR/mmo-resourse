

#include "../../../../SCMPublic/class/CGameInfo.h"
#include "DbOperator.h"

#pragma once

const DWORD MAX_GAME_INFO_NUM = 8L;

class Gapp : public tagGappBaseInfo
{
public:
	static		Gapp*				CreateGapp(tagGappBaseInfo &GappBaseInfo, DWORD dwSMID);
	static		VOID				ReleaseGapp(Gapp **ppGapp);

public:
	//!								���»�����Ϣ
	VOID							UpdateInfo(tagGappBaseInfo &GappBaseInfo){memcpy(this, &GappBaseInfo, sizeof(tagGappBaseInfo));}
	//!
	DWORD							GetID(VOID) {return _dwGappID;}
	//!
	const char*						GetVersion(VOID){return _szGappVersion;}
	VOID							SetVersion(LPCTSTR pNewVersion);
	//!
	DWORD							GetType(VOID){return _dwGappType;}

	//!								����״̬
	BOOL							ChangeRunState(DWORD dwNewState);
	//!								����echo�ӳ�ʱ��
	VOID							ChangeDelayTime(DWORD dwNewDelayTime);

	//!								����������Ϣ���ͻ���
	VOID							SendInfoToSocket(LONG lSocket);
	//!								����������Ϣ�����пͻ���
	VOID							SendInfoToAll(VOID);

	//!								���������Ϣ
	VOID							ShowAllInfo(VOID);

	//!								���롢�뿪����̨
	BOOL							InCtrlRoom(LONG lUserID, BOOL bIn);
	//!								�õ�������ID
	LONG							GetRoomUser(VOID){return m_lRoomUser;}

public:
	//!								���һ��������Ϣ
	VOID							AddGameInfo(tagDbGameInfo &GameInfoData);
	//!								��ӦDB�������
	static		VOID				OnDBWriteFinish(tagDbGameInfo *pGameInfoData, DWORD dwNum, BOOL bExecuteOK);

	//!								����һ��ʱ���ڵ�������Ϣ���û�
	VOID							FindGameInfoToUser(DWORD dwInfoType, DWORD dwBeginTime, DWORD dwEndTime, LONG lUserID);

private:
	//!								��������Ϣд��DB
	VOID							WriteGameInfoToDB(VOID);

private:
	DWORD							m_dwSMID;
	LONG							m_lRoomUser;

	//!								��Ϸ������Ϣ��Ŀ
	DWORD							m_dwNum;
	//!								��Ϸ������Ϣ
	tagDbGameInfo					*m_pDbGameInfo;
	

private:
	Gapp(tagGappBaseInfo &GappBaseInfo, DWORD dwSMID);
	~Gapp(VOID);
};
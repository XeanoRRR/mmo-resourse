#pragma once
#include "BaseAI.h"
#include "..\Player.h"
//

const INT	FIGHT_STATE_TIME = 7000;
class CPlayerAI :public CBaseAI
{
private:
	//����Ϣ��ֻ����һ����
	struct tagMessage					
	{
		//				������		����ĳ��		����ĳĿ��			ʹ�ø�ְҵ����
		enum MessageType{MT_NULL=0,MT_USESKILL};

		tagMessage(void);
		VOID Clean(void);

		MessageType	mtType;		//��Ϣ����

		LONG lSkillID;
		LONG lAttackX;
		LONG lAttackY;
		LONG lAttackType;
		CGUID AttackPlayerID;
		BOOL bIsRun;
	};

	tagMessage m_tNextAct;
	long	m_lCount;

protected:
	//������Ϊ
	virtual BOOL				OnActive			();
	//������Ϊ
	virtual BOOL				OnCycle				();
	//��ʧĿ��
	//virtual BOOL				OnLoseTarget		();			
	//׷��
	virtual INT					Tracing				();
	//����߶�
	BOOL						RandomRun			();
	//�ƶ���ĳ��
	BOOL						MoveTo				(float fDestX, float fDestY);
	//ǿ�ƹ���
	BOOL						ForceMove			(long lDir);
	//����
	virtual void				Attack				( long tID, LONG lX=0, LONG lY=0, CMoveShape* pTarget=NULL );
	//���ܽ���ʱ�����
	virtual BOOL				OnSkillEnd			(long lAddSub);
	//����������ʱ��
	virtual BOOL				WhenBeenHurted			(LONG lType,const CGUID& guid,DWORD a);

	//��ɱ����ʱ��
	virtual BOOL				WhenBeenKilled			(LONG lType,const CGUID& guid);

//////////////////////////////////////////////////////////////////////////
	//����һ����Ϣ������Ϣ��
	BOOL						CheckMessage		(tagMessage message);
	//�������Ϣ
	BOOL						CheckNextAct		();

	//�жϸø����Ƿ����ƶ�
	BOOL						IsCanMoveTo			(CRegion* pRegion,LONG lX, LONG lY);

	//�Ƿ����ʹ�ü���
	BOOL						CanUseSkill			();
	//�Ƿ����ʹ�ü���
	INT							CanUseSkill			(BOOL bRunUse);
public:
	CPlayerAI										();
	~CPlayerAI										();
//����ս��״̬���ָ�����ƽ״̬
public:
	//ע�᷵�غ�ƽ�¼�
	void						AddRetPeaceAct		();
	//ע�����غ�ƽ�¼�
	void						LogoutRetPeaceAct	();
	//�ָ�����ƽ״̬
	virtual BOOL				OnReturnPeace		();
	//����ս��
	VOID						ChangeStateToFight	(BOOL bUseSkill);

//////////////////////////////////////////////////////////////////////////

	//��ʼ���ܶ���
	//VOID						StartSkillAct		(LONG lTime);
	VOID						SetForceTarget		( LONG lType, const CGUID& TargetGuid );
	VOID						CleanForceTarget	();
	VOID						SetTarget			( LONG lType, const CGUID& TargetGuid );


	//
protected:
	LONG	m_iReturnPeaceTime;
	AI_EVENT m_aeRetPeace;
	BOOL	m_bFightUseSkill;

};

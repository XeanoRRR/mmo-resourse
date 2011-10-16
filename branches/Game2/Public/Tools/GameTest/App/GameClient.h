//=============================================================================
/**
*  file: Client.h
*
*  Brief:���һ���ͻ��˵İ�װ
*
*  Authtor:wangqiao
*	
*	Datae:20077-20
*/
//=============================================================================
#ifndef __Game_Client_H__
#define __Game_Client_H__
#pragma once
#include "../Utility/TimerDef.h"
#include "../Utility/FindPath.h"
#include "..\App\Goods.h"
#include "..\App\MoveShape.h"
#include "..\App\Monster.h"
#include "..\App\Player.h"
#include "..\App\CollectionGoods.h"
//#include "..\App\PetSystem\CFairyPet.h"
//#include "..\App\PetSystem\CFighterPet.h"
//#include "..\App\PetSystem\CSummonPet.h"
//#include "..\App\PetSystem\CPet.h"
//#include "..\App\PetSystem\CTrap.h"

//#include "..\Utility\PlayerDef.h"
#include "../App/CCDKey.h"
enum OPERATION_TYPE
{
	OT_ROLL_BACK		= 0,
	OT_MOVE_OBJECT,
	OT_NEW_OBJECT,
	OT_DELETE_OBJECT,
	OT_SWITCH_OBJECT,
	OT_SWITCH_FILL,
	OT_CHANGE_AMOUNT,
};

class CPlayer;
class CMoveShape;
class CClientRegion;
class CClientRgnObjMgr;

class CGameClient
{
public:
	CGameClient();
	CGameClient(Timer* pTimer);
	virtual ~CGameClient(void);
private:
	//�����ʵ��
	CPlayer*	   m_pMainPlayer;
	long			m_lReGameCode;

	//������ڵĳ���ʵ��
	CClientRegion* m_pRegion;

	//	�������������
	CClientRgnObjMgr*	m_pClientObjMgr;

	//��Ӧ��socketid
	long		   m_lSocketID;

	//cdkey
	string m_strCDkey;

	//ʱ�䶨ʱ��
	Timer*		   m_pTimer;
	long		   m_lTimeID;
	//  [8/11/2009 Square]
	//typedef list<CBaseObject*>		CListObject;
	//typedef CListObject::iterator	itListObject;
	//CListObject		m_listClientObject;
public:
	CBaseObject*	FindObjectFromClientRgn(long lType,const CGUID& guid);
	CBaseObject*	FindObjectFromClientRgn(const CGUID& guid);
	// 	virtual void DeleteChildObject(long typeID,const CGUID& gID);		// ɾ����OBJECT
	// 	virtual void DeleteChildObject(CBaseObject* p);						// ɾ����OBJECT
	// 	virtual void DeleteAllChildObject(CBaseObject* pExpect=NULL);		// ɾ��������OBJECT(pExpect = ����)
	void RemoveAllObject();
	void RemoveObjectFromRgn(CBaseObject* pObject);					// �Ƴ�OBJECT
	void AddObjectToClientRgn(CBaseObject* pObject);						// ���OBJECT

	void AddGoods(CGoods* pGoods,DWORD dwPos,LONG lExtendID);
	CGoods*	RemoveGoods(CGUID& guid, DWORD dwPos, LONG lExtendID, DWORD dwAmount);

	CShape* GetShape(long x, long y);
	CShape* GetShapeFromSpace(long x, long y, long lSize,OBJECT_TYPE eType, CShape* pShape);


	CClientRgnObjMgr*	GetClientRgnObjMgr()	{ return m_pClientObjMgr; }

protected:
	//MappingFile �߳�Id
	unsigned        m_uID;
public:
	void			SetMemServeruID(unsigned id)	{ m_uID = id; }
	unsigned		GetMemServeruID()				{ return m_uID; }

private:
	CGUID	m_FollowObjectGuid;				//���ٶ����GUID
	long	m_lDestX;						//�ƶ�Ŀ��X
	long	m_lDestY;						//�ƶ�Ŀ��Y
	long	m_lAimRegionID;					//Ŀ�곡��ID

	long m_lLineNum;
	long m_lCurSelLineID;
	long m_lLineID;

	long m_lKilMotrNum;



	vector<long>	m_vecTemplateID;
	vector<CGUID>	m_vecRegionGuid;
public:
	long GetTempID(long lIndex)	{
		if( lIndex>=0 && lIndex < m_vecTemplateID.size() )
			return m_vecTemplateID[lIndex];
		return -1;
	}

	const CGUID& GetRegionGuid(long lIndex){
		if(lIndex>=0&& lIndex<m_vecRegionGuid.size())
			return m_vecRegionGuid[lIndex];
		return CGUID::GUID_INVALID;
	}

	long GetLineID()	{return m_lLineID;}
	long GetCurSelLineID() { return m_lCurSelLineID; }
	long GetLineNum()	{ return m_lLineNum; }

	void SetLineID(long id)	{ m_lLineID = id; }
	void SetCurSelLineID(long id)	{ m_lCurSelLineID = id; }
	void SetLineNum(long l)	{ m_lLineNum = l; }

	void DecordLineFromDataBlock(DBReadSet& db_r);

	void SetKilMotrNum(long num)  { m_lKilMotrNum = num; }
	long GetKilMotrNum()		  { return m_lKilMotrNum;}

public:
	//
	void		   SetMainClientTimer( long l)				{ m_lTimeID = l;}
	long		   GetMainClientTimer()						{ return m_lTimeID;}
	void		   SetDestX(long x)							{ m_lDestX = x;}
	void		   SetDestY(long y)							{ m_lDestY = y;}
	long		   GetDestX()								{ return m_lDestX;}
	long		   GetDestY()								{ return m_lDestY;}
	void	       SetAimRegionID(long l)					{ m_lAimRegionID = l;}
	long	       GetAimRegionID()							{ return m_lAimRegionID;}
	void	       SetFollowObjectGuid(const CGUID& guid)	{ m_FollowObjectGuid = guid;}
	const CGUID   &GetFollowObjectGuid()					{ return m_FollowObjectGuid;}
	void		   SetMainPlayer(CPlayer* pPlayer)			
	{ 
		m_pMainPlayer = pPlayer;
//		m_pMainPlayer->SetIsSelf(true);
	}
	CPlayer		  *GetMainPlayer()							{ return m_pMainPlayer; }

	long	GetReGameCode()			{ return m_lReGameCode; }
	void	SetReGameCode(long l)	{ m_lReGameCode = l; }

	void		   SetClientRegion(CClientRegion* pRegion)	{ m_pRegion = pRegion;}
	CClientRegion *GetClientRegion()						{ return m_pRegion;}
	void		   SetSocketID(long lSocketID)				{ m_lSocketID = lSocketID;}
	long		   GetSocketID()							{ return m_lSocketID ; }

	void		   SetCDKey(const char* cdkey)				{ m_strCDkey = cdkey;}
	const char    *GetCDKey()								{ return m_strCDkey.c_str();}

	CFindPath	  *GetFindPath()							{ return m_pRegion->GetFindPath(); }
private:
	DWORD m_dwAimConPos;

	//////////////////////////////////////////////////////////////////////////
	//	��Ʒ�߼�����
	//////////////////////////////////////////////////////////////////////////
	int m_nCurOpenWindowsType;		//	�򿪵Ľ�������
	ePageType m_eCurOpenPageType;
public:
	void SetCurOpendWindowsType(int nType)	{ m_nCurOpenWindowsType = nType; }
	int GetCurOpendWindowsType()	{ return m_nCurOpenWindowsType; }

	void	  SetCurOpenPageType(ePageType e)		{ m_eCurOpenPageType = e; }
	ePageType GetCurOpenPageType()					{ return m_eCurOpenPageType; }

private:
	//����Ƿ����ƶ� for DriveClient
	// by MartySa 2009.2.7
	bool			  m_bIsPlayerMove;

public:
	void			  SetPlayerIsMove(bool b)	{ m_bIsPlayerMove = b; }
	bool			  GetPlayerIsMove()			{ return m_bIsPlayerMove; }
	//--------------------------------------------------------------------------------------
	//��Ʒ����
	//--------------------------------------------------------------------------------------
public:
	tagUpgradeStuff *GetUpgradeStuff(void)			{ return &m_UpgradeStuff; }
private:
	tagUpgradeStuff m_UpgradeStuff;
	//------------------------------------------------------------------------------------
	//�⿨
	//------------------------------------------------------------------------------------
private:
	tagEnchaseSession m_tagEnchaseSession;
	tagEnchaseGoods	 *m_pEnchaseGoods;

public:
	void SetEnchaseBaoShi(CGoods* pBaoShi)			{ m_pEnchaseGoods->pBaoShi = pBaoShi; }
	void SetEnchaseGoods(CGoods* pGoods)			{ m_pEnchaseGoods->pGoods = pGoods; }
	void SetEnchaseCard(CGoods* pCard,int nIndex)	{ m_pEnchaseGoods->pCard[nIndex] = pCard; }
	void SetEnchaseSessionGuid(const CGUID& guid)	{ m_tagEnchaseSession.m_SessionGuid = guid;}
	void SetEnchasePlayerID(long id)				{ m_tagEnchaseSession.lPlayerID = id; }
	void SetEnchasePageType(BYTE b)					{ m_tagEnchaseSession.iPageType = b; }
	void SetEnchaseAimConPosForMsg(DWORD msgpos)	{ m_dwAimConPos = msgpos; }		

	CGoods		*GetEnchaseBaoshi()					{ return m_pEnchaseGoods->pBaoShi; }
	CGoods		*GetEnchaseGoods()					{ return m_pEnchaseGoods->pGoods; }
	CGoods		*GetEnchaseCard(int nIndex)			{ return m_pEnchaseGoods->pCard[nIndex]; }
	const CGUID &GetEnchaseSessionGuid()			{ return m_tagEnchaseSession.m_SessionGuid; }
	long	     GetEnchasePlayerID()				{ return m_tagEnchaseSession.lPlayerID; }
	BYTE		 GetEnchasePageType()				{ return m_tagEnchaseSession.iPageType; }
	DWORD        GetEnchaseAimConPosForMsg()		{ return m_dwAimConPos; }
private:
	bool bOpenFacturePage;		//���������Ƿ��
	bool bOpenTalkBoxPage;		//TalkBox����
	bool bOpenTradePage;		//���׽���
	bool bOpenTreasureShopPage; //��ֵ�̵����
	bool bOpenNPCShopPage;		//NPC�̵����
	bool bOpenPlayerShopPage;	//�����̵����
	bool bOpenDepotPage;		//�ֿ����
	bool bOpenComPage;
	bool bOpenDecomPage;
	bool bOpenEquipUpgradePage;	//װ����������
	bool bOpenEquipMagicPage;	//װ��ħ������
	bool bOpenMagicDecomPage;	//װ��ħ���ֽ�
	//------------------------------------------------------------------------------------
	//��Ʒ�ϳ�
	//------------------------------------------------------------------------------------
	//! C2S��Ϣ�� �ϳ�������ṹ
	//����+5 ��Ϊ�˲��Է������Ĵ�����

private:
	tagSynthesizeGoods  m_tagSynthesizeGoods;	
	long			    m_lPrescriptionIndex;		//�䷽

public:
	tagSynthesizeGoods &GetSynthesizeGoods()										{ return m_tagSynthesizeGoods; }
	void			    SetSynthesizeGoods(tagGoods *pstGoods, int index);
	void				ClearSynthesizeGoods();		//���ͺϳ�Э��� ����ѡ����յ�ǰ�ṹ��
	void				SetSynPrescriptionIndex(long lIndex)						{ m_lPrescriptionIndex = lIndex; }
	long				GetSynPrescriptionIndex()									{ return m_lPrescriptionIndex; }
	//------------------------------------------------------------------------------------
	//���Ϻϳ�
	//------------------------------------------------------------------------------------
private:
	tagEvilStuff	m_tagEvilStuff;

public:
	tagEvilStuff	&GettagEvilStuff()								{ return m_tagEvilStuff; }
	void			 SettagEvilStuff(tagGoods *taggoods);
	void			 CleartagEvilStuff();			//���ͺϳ�Э��� ����ѡ����յ�ǰ�ṹ��
	//------------------------------------------------------------------------------------
	//// Ping Server add By MartySa
	//------------------------------------------------------------------------------------
private:

	DWORD m_dwLastPingTime;			// �ϴ�PingSrv��ʱ��
	DWORD m_dwBlankPingTime;		// PingSrv���
	bool  m_bIsCanPingSrv;			// �Ƿ���ping��Ϣ

public:

	void  SetBlankPingTime(DWORD Bktime) { m_dwBlankPingTime = Bktime; /*60*1000*10*/}	//���ü��ʱ��
	bool  GetIsCanPingSrv()	{
		DWORD dwNowTime = ::timeGetTime();
		if ( dwNowTime - m_dwLastPingTime > m_dwBlankPingTime )
		{
			m_dwLastPingTime = dwNowTime; 
			return true;
		}
		return false;
	}
	DWORD GetPingSrvSendVal();												//��ȡPingSrvֵ	

	///////
	//----------------------------------------------------------------------------------------------------------------
	// client S2C��Ϣ����
public:
	virtual void OnMessage(CMessage* pMsg);
	//	����
	void OnPetMessage(CMessage* pMsg);
	//
	void	OnShapeMessage(CMessage* pMsg);
	//	����
	void OnBounsMessage(CMessage* pMsg);
	//	
	void OnContainerMsg(CMessage* pMsg);
	//	
	void OnOtherMessage(CMessage* pMsg);

	void OnSkillMessage(CMessage* pMsg);

	void OnPlayerMessage(CMessage* pMsg);

	void OnGoodsMessage(CMessage* pMsg);

	//Player���S2C����
	void S2CPlayer(CMessage* pMsg);
	void PlayerLevelUp(CMessage* pMsg);
	void PlayerUpdateProperty(CMessage* pMsg);
	void PlayerAddSkill(CMessage* msg);
	void PlayerAnswerRelive(CMessage* msg);
	void PlayerUseItem(CMessage* msg);
	//
	void OnReginMessage(CMessage* pMsg);
	//	void S2CRegion(CMessage* pMsg);
	//	void RegionAddShape(CMessage* msg);
	//	void RegionDelShape(CMessage* msg);
	//	virtual void RegionChanged(CMessage *msg);
	//	virtual void RegionChangeServer(CMessage* msg);
	//

	//	void S2CContainer(CMessage* pMsg);
	//	void ContainerObjectMove(CMessage* msg);
	//	void ContainerObjectAmountChange(CMessage* msg);
	//
	//	void S2CSkill(CMessage* pMsg);
	//	void SkillUse(CMessage* msg);
	//	void SkillStateChange(CMessage* msg);
	//
	void S2COther(CMessage* msg);
	void AnswerNpcList(CMessage* msg);
	//
	void S2CGoods(CMessage* msg);
	void OpenGoodsEnchase(CMessage* msg);								//�򿪿�Ƭ��Ƕ���ܽ���
	void OpenGoodsHandleWindow(CMessage* msg);
	void AnswerDepotPwdInput(CMessage *msg);
	void OrderDepotClose(CMessage* msg);
	//�����Ϣ
	void OnTeamMessage(CMessage* pMsg);
	void S2CTeam(CMessage* msg);
	void TeamCreat(CMessage *msg);										//���������ʱ����
	void TeamData(CMessage *msg);		
	void TeamMemberData(CMessage *msg);
	void TeamQueryJion(CMessage* msg);
	void TeamMemAllInfo(CMessage* msg);
	void TeamMemberLeave(CMessage* msg);								//! ��Ա���
	void TeamMastterChange(CMessage* msg);								//! �ӳ��ı�
	void TeamRecruit(CMessage* msg);									//! ������ļ״̬
	void TeamRecruitedNum(CMessage* msg);								//! ��������ļ������
	void TeamAnswerJoin(CMessage* msg);								//�ظ��������
	void TeamSynchronize(CMessage* msg);								//ͬ���������Ϣ
	void TeamMatChangeRegion(CMessage* msg);
	void TeamChangeLeader(CMessage* msg);								//�ı�ӳ�
	void TeamChangeState(CMessage* msg);
	//��Ӻ�����Ϣ
	void S2CLinkMan(CMessage* msg);
	void LinkManInit(CMessage* msg);
	void LinkManAdd(CMessage* msg);
	void DelLinkman(CMessage* msg);
	void RemoveLinkMan(CMessage* msg);
	//����
	void PlayerTradeRequest(CMessage* msg);
	void PlayerTradeBegin(CMessage* msg);
	void PlayerTradeLock(CMessage* msg);
	void PlayerTradeFinish(CMessage* msg);
	//NPC�̵�
	void S2CShop(CMessage* msg);
	void ShopOpen(CMessage* msg);
	void ShopBuy(CMessage* msg);
	// ����,�л�[12/22/2008 Square]
	void OnFamilyMessage(CMessage* pMsg);
	void DecordFamilySetup(BYTE*pData,long& lPos);
	//��Ҹ����̵�
	void S2CPlayerShop(CMessage* msg);
	void PlayerShopCreate(CMessage* msg);
	void OnPlayerShopToBuyer(CMessage* pMsg);
	//�ʼ�
	void S2CMail(CMessage* msg);
	void MailReceiver(CMessage* msg);
	void MailSendFailed(CMessage* msg);
	void MailSend(CMessage* msg);
	void MailRead(CMessage* msg);
	void MailFailedMoveGoods(CMessage* msg);
	void MailMoveGoodsResult(CMessage* msg);
	void MailDeleteResult(CMessage* msg);
	void MailDeleteOk(CMessage* msg);
	void MailAgreedPay(CMessage* msg);
	void MailAgreedPresent(CMessage* msg);
	void MailGetGoldResult(CMessage* msg);
	void MailPersentFailed(CMessage* msg);

	//�����ֵ�̵�
	void S2CIncrementShop(CMessage* msg);
	void IncrementShopOpen(CMessage* msg);
	// С���� [12/30/2008 Square]
	void OnSpriteMessage(CMessage* pMsg);
	//�������
	void S2CPlayerManage(CMessage* msg);
	void PlayerManageAtk(CMessage* msg);
	//ѧϰ��ְҵ��������
	void OccuskillStudyBegin(CMessage* msg);
	void OccuskillStudySuccess(CMessage* msg);

	//�ش�����
	void AntiCheat(CMessage *msg);
	void AntiCheatQuestion(CMessage *msg);


	//////////////////////////////////////////////////////////////////////////
	//	Login
	void OnLoginMessage(CMessage* pMsg);
};
#endif
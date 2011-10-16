#pragma once

#include "../AppClient/Player.h"
#include "../AppClient/shape.h"
#include "../AppClient/organizingsystem/Faction.h"
#include "../AppClient/organizingsystem/Union.h"
#include "..\..\GameEngine/shadowvolume.h"
#include "..\..\GameEngine/gameflatshadow.h"
#include "..\..\GameEngine/gamemodel.h"
#include "..\..\..\Public\GoodsExSetup.h"
//#ifndef  MOVE_IN_TILE
//#define MOVE_IN_TILE
//#endif
class CMouse;
class CKeyBoard;
class CClient;
class CPlayer;
class CClientRegion;
class CModelList;
class AudioEngine;
class CAudioList;
class CPicList;
class CFaceActionList;
class WeatherManager;
class CFmodMusic;
class CClientResource;
class CMsgBoxEvent;
class CGameControl;
class CEffect;
class CDupliRegionSetup;
//===================================================
class CGuiEx;
class CGuangMGUIApp;
class COccupSkillList;
class CDataBlockAllocator;
class CSkillXml;
class CMemClient;
extern HWND g_hRetLb;

// ��Ϸ״̬
enum
{
	GS_LOGIN,		// �Α��ꑠ�B
	GS_PWD_CARD,	// �ܱ�������״̬
	GS_SELECTWS,     //ѡ��WS
	GS_SELECT,		// �Α��x���ɫ��B
	GS_CREATE,		// �Α�����ɫ��B
	GS_WAITING,		// �ȴ�����״̬
	GS_BLACK,		// ����״̬
	GS_RUN,			// �Α��\��
	GS_STAFF		// ������Ա
};

// ��굱ǰ״̬
enum MOUSE_CURSOR_TYPE
{
	MC_NORMAL,				// ������Ϸ
	MC_PLAYER_FOCUS,		// ָ�����
	MC_NPC_FOCUS,			// ָ��NPC(Ĭ��)
	MC_MONSTER_FOCUS,		// ָ�����
	MC_GOODS_FOCUS,			// ָ����Ʒ
	
	MC_COLLECTION_CAO,		// �ɼ���ݣ�
	MC_COLLECTION_KUANG,	// �ɼ����
	MC_COLLECTION_PI,		// �ɼ��Ƥ��
	MC_COLLECTION_RENWU,	// �ɼ������
	MC_STATE_TRADE,			// ������Ʒ״̬
	MC_TRYON_GOODS,			// ��װ״̬
	MC_NPC_QUEST,			// ����
	MC_MONEY,				// Ǯ��
	MC_GEAR,				// ����
	MC_GATE,				// ��

	MC_COUNT				// ����
};

// ���������� - ��Ӧ�������� CSCContainerObjectMove.h
enum PLAYER_EXTEND_ID
{
	PEI_PACKET					= 1,	// ����
	PEI_EQUIPMENT,						// װ��	
	PEI_WALLET,							// ���Ǯ��
	PEI_SILVERWALLET,					// ����Ǯ��
	PEI_GEM,							// ��ʯ
	PEI_YUANBAO,						// Ԫ��
	PEI_JIFEN,							// ����
	PEI_MEDAL,                        // ѫ������ 
	PEI_SUBPACK=10000,
	PEI_PACK1=10001,
	PEI_PACK2=10002,
	PEI_PACK3=10003,
	PEI_PACK4=10004,
	PEI_PACK5=10005,
	PEI_BUSSINESPACKFIELD = 20000,		// �����ʱ�������λID
	PEI_BUSSINESPACK = 20001			// ���ʱ�������ID
};

// �ж������Ƿ����ڱ�����
#define IS_PACKETCONTAINER(type) ((type) == PEI_PACKET || ((type) >= PEI_PACK1 && (type) <= PEI_PACK5))

struct SHADOWVERTEX
{
    D3DXVECTOR4 p;
    D3DCOLOR    color;

    static const DWORD FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
};

struct stTopStrInfo
{
	D3DCOLOR	d3dColor;	//��ʾ��ɫ
	string		strContend; //��ʾ����
};
//��Ļ�϶˵���ʾ
struct stTopInfo
{
	long				lID;
	long				lTimerFlag;			//����ʱ��ʽ(1:����,2:ʱ��)
	long				lParam;				//�����ʱ�����ʾ����ʱ����
	DWORD				dwStartTime;		//��ʼ��ʱʱ��
	list<stTopStrInfo>	strInfo;			//��ʾ��Ϣ��������
};

//��ǰ��ʾ��Ϣ����
struct stCurTopInfo
{
	long	lX;					//��ʾ����
	long	lY;
	long	lTotalLength;		//�ܳ���
	
	list<stTopStrInfo>   listCurInfo;	//��ʾ�б�
};

enum eNetFlag
{
	NF_LS_Client=0x001,
	NF_GS_Client=0x010,
};
extern int ORGIN_GAME_WIDTH;
extern int ORGIN_GAME_HEIGHT;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern POINT g_pointMousePos;

class CBitmapX;

// ������Ϸȫ��������
class CGame
{
public:
	CGame(void);
	virtual ~CGame(void);

	static bool	s_bExit;						// �Ƿ��˳�
	static bool IsExit()						{return s_bExit;}	

	static bool s_bReGame;						// �Ƿ����½���ѡ�˻���
	static bool IsReGame()						{return s_bReGame;}
	static void SetReGame(bool b)				{s_bReGame=b;}

	// �������ü����豸
	bool ReSetKeyboard();

	// ���壺��� CGoodsDescPageEx ����
	static vector<class CGoodsDescPageEx*>	m_vGoodsDescPageEx;

////////////////////////////////////////////////////
// ȫ�ֱ���
////////////////////////////////////////////////////
private:	

	D3DCOLOR m_colorBack;						// ������ɫ
	UINT    m_uGameServerPort;					// GameServer�������˿�
	string	m_strGameServerIP;					// GameServerIP
	UINT    m_uLoginServerPort;					// LoginServer�������˿�
	string	m_strLoginServerIP;					// longinServerIP

	CMouse		*m_pMouse;						// ���
	CKeyBoard	*m_pKeyboard;					// ����

	CAudioList	*m_pAudioList;					// ��Ч�б���������shape������

	CDataBlockAllocator	*m_pDBAllocator;		// ���ݿ�
    CClient* m_pNetClient;						// �ͻ�������
	//===========================================================================================
	CGuiEx *m_pGuiEx;
	COccupSkillList *m_pOccupSkillList;			// ְҵ�����б�
	//===========================================================================================

	CPlayer* m_pMainPlayer;						// ����
	CClientRegion* m_pRegion;					// ��ͼ

	long	m_lGameState;						// �Α�ǰ��B
	long	m_lMouseState;						// ���״̬
	string	m_strCDKEY;							// ���CDKEY
	vector<BYTE>	m_strPASSWORD;				// �������
	string	m_strWSName;						// ��½��WorldServer����

	CPicList	*m_pPicList;					// ͼƬ��Դ����

	CBitmapX	*m_pbitmapShapeTalk;			// ˵��ʱͷ�����ֵı���ͼƬ
	CBitmapX	*m_pbitmapTeamTalk;				// ����˵��ʱ�ı���ͼƬ

	CBitmapX	*m_pbitmapIntanateTimePot;		// ��������ʱ���ͼƬ
	CBitmapX	*m_pbitmapIntanateBarBack;		// ����������
	CBitmapX	*m_pbitmapIntanateBar;			// ������
	CBitmapX	*m_pbitmapHpBarBack;			// Ѫ���ı�������ͼ
	CBitmapX	*m_pbitmapPlayerHpBar;			// Ѫ����ͼ(��)
	CBitmapX	*m_pbitmapMonsterHpBar;			// ����Ѫ��(�죩
	CBitmapX	*m_pbitmapHpBar2;				// Ѫ����ͼ(��)
	CBitmapX	*m_pbitmapHpBarPet;				// ����Ѫ����ͼ
	CClientResource* m_pClientResource;			// �ͻ�����Դ
	CBitmapX	*m_pbitmapSuck;					// ����ͼ��
	CBitmapX	*m_pbitmapBlastSuck;			// ���ձ���ͼ��
	CBitmapX	*m_pbitmapBlastAttack;			// ����ͼ��
	CBitmapX	*m_pbitmapBlock;				// ��ͼ��
	CBitmapX	*m_pbitmapBlastBlock;			// �񵵱���ͼ��
	CBitmapX	*m_pbitmapRebound;				// ����ͼ��
	CBitmapX	*m_pbitmapBlastRebound;			// ��������ͼ��
	CBitmapX	*m_pbitmapFullMiss;				// ����ͼ��
	CBitmapX	*m_pbitmapBlastFullMiss;		// ���ܱ���ͼ��
	CBitmapX	*m_pbitmapParry;				// �м�ͼ��
	CBitmapX	*m_pbitmapBlastParry;			// �мܱ���ͼ��
	CBitmapX	*m_pbitmapResist;				// �ֿ�ͼ��
	CBitmapX	*m_pbitmapBlastResist;			// �мܱ���ͼ��
	CBitmapX	*m_pbitmapLost;					// ��ʧͼ��
	CBitmapX	*m_pbitmapExpTip;				// ����ֵ��ʾͼƬ
	CBitmapX	*m_pbitmapConTip;				// ��ѫ��ʾͼƬ

	bool		m_bIsGM;						// GM��־
	CMsgBoxEvent*		m_pMsgBoxEvent;			// MsgBox ���¼�
	CGameControl*		m_pGameControl;			// ��Ϸ������

	CBitmapX*	m_pBitMapFactionWarEnemy;		// ��ս�ĵжԷ�ͼ��
	CBitmapX*	m_pBitMapFactionWarUion;		// ��ս��ͬ�˷�ͼ��

	CBitmapX*	m_pBitMapCityWarEnmey;			// ��ս�жԷ���ͼ��
	CBitmapX*	m_pBitMapCityWarUnion;			// ��սͬ�˷���ͼ��

	CBitmapX*	m_pBitMapOwnedCity;				// ӵ�����ǵ���ʾͼ��
	CBitmapX*	m_pBitMapOwnedVillage;			// ӵ�оݵ����ʾͼ��

	CBitmapX*	m_pHandsBackIcon;				// ������Ʒ�����ܣ�ͼ��ı���

	GoodsExSetup		m_GoodsExSetup;			// ���壺Goods ��չ�����㷨
	CFactionManager		m_FactionManager;		// ���壺�л�������
	CUnionManager		m_UnionManager;			// ���壺���˹������


	long		m_lLoadingTipsIndex;			// loading����ʾ��������
	long		m_lLoadingDelayTime;			// ���볡�����ӳٿ�ʼʱ��
	CBitmapX*	m_pLoadingPic;					// ��½��loadingͼƬ
	void		DisPlayLoading(long lRegionID);	// ���ݳ���ID������Loading����

public:
	DWORD	m_dwFaceTexture;	//����ͼ��

private:
	int			m_iCurrentRenderTriangleCount;  //��ǰ��Ϸ��ʾ�Ķ���������������������ϵ���άģ�ͣ�

public:
	void        SetCurrentRenderTriangleCount(int iCount)  { m_iCurrentRenderTriangleCount = iCount;}

	CBitmapX*	m_pSelectShape;					// ѡ����һ������ʱ��ʾ���������ҵ�ͼƬ
	CBitmapX*	m_pPKIcon;						// PKͼ��
	CEffect*	m_pMouseShapeFoot;				// ���ָ��Ķ���ŵ׹��
	CEffect*	m_pMouseTeammerFoot;			// ���ָ��Ķ��ѽŵ׹��
	CEffect*	m_pLockAttackShapeFoot;			// ����Ŀ�����ŵ׹��

private:
	BOOL		m_bPlayerInWeatherEffectArea;	// ��ɫ������Ч��������
public:
	void		SetPlayerInWeatherEffectArea(BOOL bValue) { m_bPlayerInWeatherEffectArea = bValue;}
private:
	//---------------------------------------------------------------------------------
	// �����Ӱ��ֻ�����ǻ������Ӱ��

	BOOL m_b2SidedStencils;
	static int CONST MAX_VOL = 32;							// ����ÿ��ģ�������10������������Ӱ
	SHADOWVERTEX m_pShadowVertex[4];
	BOOL m_bSupportStencils;

	DWORD m_dwNumOfVol, m_dwNumOfShape;

	// ƽ����Ӱ
	GameFlatShadow m_FlatShadow;
	//---------------------------------------------------------------------------------

public:

	CGameControl*			GetGameControl()	{return m_pGameControl;}
	CMsgBoxEvent*			GetMsgBoxEvent()	{return m_pMsgBoxEvent;	}

	CClientResource*		GetCleintResource()	{return m_pClientResource;}
	CPicList* GetPicList()					    {return m_pPicList;}
	D3DCOLOR	GetBackColor()					{return m_colorBack;}
	void SetBackColor(BYTE a, BYTE r, BYTE g, BYTE B)	{m_colorBack=D3DCOLOR_ARGB(a,r,g,B);}
	void SetBackColor(D3DCOLOR color) { m_colorBack = color;}     
	CMouse*		GetMouse()						{return m_pMouse;}
	CKeyBoard*  GetKeyboard()					{return m_pKeyboard;}
	CAudioList*	GetAudioList()					{return m_pAudioList;}
	UINT   GetGameServerPort()					{return m_uGameServerPort;}
	void   SetGameServerPort( UINT uPort)		{m_uGameServerPort = uPort;}
	const char*  GetGameServerIP()				{return m_strGameServerIP.c_str();}
	void   SetGameServerIP( const char* chIP)	{m_strGameServerIP = chIP;}
	UINT   GetLoginServerPort()				    {return m_uLoginServerPort;}
	void   SetLoginServerPort( UINT uPort)		{m_uLoginServerPort = uPort;}
	const char*  GetloginServerIP()				{return m_strLoginServerIP.c_str();}
	void   SetLoginServerIP( const char* chIP)	{m_strLoginServerIP = chIP;}

	void SetNetClient(CClient* pNetClient)		{m_pNetClient = pNetClient;}
	CClient* GetNetClient()						{return m_pNetClient;	}
	//====================��ý�����չ�������========================
	CGuiEx * GetCGuiEx()						{return m_pGuiEx;}
	COccupSkillList * GetOccupSkillList()		{return m_pOccupSkillList;}
	//================================================================
	CPlayer* GetMainPlayer()					{return m_pMainPlayer;	}
	void SetMainPlayer(CPlayer* t)				{m_pMainPlayer = t;		}
	CClientRegion	*GetRegion(void)			{return m_pRegion;}

	long	GetWarContendTime()					{return m_lWarContendTime;}
	void	SetWarContendTime(long lTime)		{m_lWarContendTime = lTime;}

	long	GetDiedStateTime()					{return m_lDiedStateTime;}
	void	SetDiedStateTime(long lTime)		{m_lDiedStateTime = lTime;}

	long	GetGameState()						{return m_lGameState;	}
	void	SetGameState(long t);//				{m_lGameState = t;m_lQueuePos=0;}
	long	GetMouseState()						{return m_lMouseState;	}
	void	SetMouseState(long t)				{m_lMouseState = t;		}
	void SetCDKEY( const char* t)				{ m_strCDKEY = t;}
	const char* GetCDKEY()						{return m_strCDKEY.c_str();}
	void SetPASSWORD(vector<BYTE> bytePass)		{m_strPASSWORD = bytePass;}
	const vector<BYTE>& GetPASSWORD()			{return m_strPASSWORD;}
	void SetWSName( const char* t)				{m_strWSName = t;}
	const char* GetWSName()						{return m_strWSName.c_str();}

	CBitmapX* GetIntanateTimePotBitmap()		{return m_pbitmapIntanateTimePot;}
	CBitmapX* GetIntanateBarBackBitmap()		{return m_pbitmapIntanateBarBack;}
	CBitmapX* GetIntanateBarBitmap()			{return m_pbitmapIntanateBar;}
	CBitmapX* GetHpBarBackBitmap()				{return m_pbitmapHpBarBack;}
	CBitmapX* GetPlayerHpBarBitmap()			{return m_pbitmapPlayerHpBar;}
	CBitmapX* GetMonsterHpBarBitmap()			{return m_pbitmapMonsterHpBar;}
	CBitmapX* GetHpBarBitmap2()					{return m_pbitmapHpBar2;}
	CBitmapX* GetHpBarPetBitmap()				{return m_pbitmapHpBarPet;}
	CBitmapX* GetSuckBitmap()					{return m_pbitmapSuck;}
	CBitmapX* GetBlastSuckBitmap()				{return m_pbitmapBlastSuck;}
	CBitmapX* GetBlastAttackBitmap()			{return m_pbitmapBlastAttack;}
	CBitmapX* GetBlock()						{return m_pbitmapBlock;}
	CBitmapX* GetBlastBlock()					{return m_pbitmapBlastBlock;}
	CBitmapX* GetRebound()						{return m_pbitmapRebound;}
	CBitmapX* GetBlastRebound()					{return m_pbitmapBlastRebound;}
	CBitmapX* GetFullMissBitmap()				{return m_pbitmapFullMiss;}
	CBitmapX* GetBlastFullMiss()				{return m_pbitmapBlastFullMiss;}
	CBitmapX* GetParry()						{return m_pbitmapParry;}
	CBitmapX* GetBlastParry()					{return m_pbitmapBlastParry;}
	CBitmapX* GetResist()						{return m_pbitmapResist;}
	CBitmapX* GetBlastResist()					{return m_pbitmapBlastResist;}
	CBitmapX* GetLost()							{return m_pbitmapLost;}
	CBitmapX* GetExpTip()						{return m_pbitmapExpTip;}
	CBitmapX* GetConTip()						{return m_pbitmapConTip;}
	CBitmapX* GetFactionWarEnemyBitmap()		{return m_pBitMapFactionWarEnemy;}
	CBitmapX* GetFactionWarUnionBitmap()		{return m_pBitMapFactionWarUion;}
	CBitmapX* GetCityWarEnemyBitmap()			{return m_pBitMapCityWarEnmey;}
	CBitmapX* GetCityWarUnionBitmap()			{return m_pBitMapCityWarUnion;}
	CBitmapX* GetOwnedCityBitmap()				{return m_pBitMapOwnedCity;	}
	CBitmapX* GetOwnedVillageBitmap()			{return m_pBitMapOwnedVillage;}

	void SetGMMode( bool bIsGM = true )			{ m_bIsGM = bIsGM; }
	bool IsGM()									{ return m_bIsGM; }

	bool		IsFileExist(const char*	strFileName);
	void		FindFileList(const char* strFile, const char* strExt, list<string>& listFileName);

////////////////////////////////////////////////////
// �ӿں���
	////////////////////////////////////////////////////

	// ��Ϸִ�в���
	int Run(bool bControl=true);

	// ��Ϸ��Ϣ����
	void ProcessMessage();

	// ��Ϸ��ʾ����
	int Display(void);

	// ��Ϸ��ʼ��
	int Init(void);

	// ��Ϸ�ͷŴ���
	int Release(void);

	// ���¿�ʼǰ��׼������
	void ReNew(void);

	// �˳���Ϸ
	void QuitGame(void);

	// �Ͽ���������
	void CloseNetClient(void);

	//����LoginServer������
	bool ConnectLoginServer(const char* const logIP,long logPort,
		const char* const cdKey,const vector<BYTE>& password,const char *wsName="");

	// ����յ� MSG_LOG_ANSWER_PLAYER �� MSG_REGION_COMMOND_CHANGE ��������볡��
#ifdef MOVE_IN_TILE
	void QuestEnterRegion(long lRegionID,const CGUID& RegionID,long lResourceID, char* strRegionName, float lPlayerX, float lPlayerY, long lPlayerDir, long lUseGoods = 0);
#else
	void QuestEnterRegion(long lRegionID,const CGUID& RegionID,long lResourceID, char* strRegionName, long lPlayerX, long lPlayerY, long lPlayerDir, long lUseGoods = 0);
#endif

	//����GameServer������
	CClient* ConnectGameServer(const char* const gsIP,long gsPort,const CGUID& PlayerID);

	void OnServerLost();

	// ��ȡ������Ϣ
	void GetInputInfo();

	// new MainPlayer
	void NewMainPlayer();

	// ��ʾFPS
	int GetFps(void);

	void ShowFps(void);

	// �����������
	void SetMouseCursor(int mcType);

	// ��ȡ�������
	MOUSE_CURSOR_TYPE GetMouseType();

	// ��ȡ�����
	HCURSOR GetMouseCursor();

	////---------------------------------------------------------------------------------

	// ץȡ��Ļ
	void PrintScreen(const char* strFileName);

	//
	//---------------------------------------------------------------------------------

	////////////////////////////���������������Ϣ
	/*
	 *	LoginServer
	 */
	void C2L_CDKey_Send(const char *cdkey,const vector<BYTE>& password, const char *wsName="");//��loginserver CDKEY��֤
	void C2L_PlayerBase_Send(const char *wsName,const char *cdkey);//��loginserver�����������
	void C2L_DeleteRole_Send(const CGUID& ID);//��loginserver����ɾ��һ����ɫ
	
	void C2L_RestoreRole_Send(const CGUID& PlayerID);

	void C2L_AddRole_Send(const char *strName,char nOccupation, char nSex, BYTE lHair, BYTE lFace,BYTE lCountry,BYTE lConstellation,BYTE bRandCountry);//��loginserver�������һ����ɫ
	void C2L_QuestDetail_Send(const CGUID& ID);//��loginserver������ϸ��Ϣ

////////////////////////////////////////////////////
// tick
////////////////////////////////////////////////////
public:
	DWORD GetAITick()				{return s_lAITick;}
	void SetAITick(long l)			{s_lAITick=l;}
	void SetLastPingTime(DWORD l)	{s_lLastPingTime=l;}

	void SetScriptTimer(long lTime)	{m_lScriptTimer=lTime;}
	long GetScriptTimer()			{return m_lScriptTimer;	}

	void SetQueuePos(long lNum);
	long GetQueuePos()              {return m_lQueuePos;}

private:
	DWORD s_lAITick;				// ��ϷAI�����������ڹ����еĴ���
	DWORD s_lLastPingTime;			// �ϴ��յ�SERVER��PINGʱ��
	long  m_lScriptTimer;			// �ű��ļ�ʱ��
	long  m_lQueuePos;				// ��½����ʱ���ڶ����е�λ��
	DWORD m_dwLastUpdateQueueTime;	//��һ�ε�ʱ��
	DWORD m_dwCountDownTime;		// ���ӵ���ʱ
	long  m_lWarContendTime;		// �ݵ�����ս����ս���е�ʱ��ٷֱ�
	long  m_lDiedStateTime;			//����״̬ʣ��ʱ��
public:
	void CreateGraphices(void);
	void DestroyGraphices(void);

///////////////////////////////////////////////////
//��֤��Ϣ
///////////////////////////////////////////////////
public:
	void SetSignCode(long l) {m_lSignCode = l;};   //���ý�����Ϸ����֤��
	long GetSignCode(){return m_lSignCode;};         // ��ý�����Ϸ����֤��
	void SetChangeServerCode(long l) {m_lChangeServerCode = l;}; //�����л�����������֤��
	long GetChangeServerCode(){return m_lChangeServerCode;};  //����л�����������֤��
	void SetGameCDKEY(const char* str) {m_strGameCDKEY = str;};     //����m_strGameCDKEY
	const char* GetGameCDKEY() {return m_strGameCDKEY.c_str();};    //���m_strGameCDKEY
	void SetReGameCode(DWORD l,int i){m_lReGameCode[i] = l;};    //������֤С�˵Ĳ���
private:
	long m_lSignCode;              //������Ϸ����֤��
	long m_lChangeServerCode;      //�л�����������֤��
	string m_strGameCDKEY;         //����GAMESERVERʱ����cdkey
	DWORD m_lReGameCode[4];                //��֤С�˵Ĳ���

////////////////////////////////////////////////////
// ��������
////////////////////////////////////////////////////
public:
	void SetTradeAble(bool b)	{m_bTradeAble=b;}
	bool IsTradeAble()			{return m_bTradeAble;}

	void AddDisableChatter(const char* str)	{m_listDisableChatter.push_back(str);}
	void DelDisableChatter(const char* str)	{m_listDisableChatter.remove(str);}
	bool CheckDisableChatter(const char* str)	{return find(m_listDisableChatter.begin(), m_listDisableChatter.end(), str) != m_listDisableChatter.end();}

private:
	bool m_bTradeAble;							// �ɷ���
	list<string>		m_listDisableChatter;	// ��������������

//////////////////////////////////////////////////////////////////////////
//	��������
//////////////////////////////////////////////////////////////////////////
public:
	struct tagRegion
	{
		string strBgMusic;	//�������������ļ���
		string strLoadingPic;	//�����л���ͼƬ�ļ���
		int bgMusicID;	//������Ч�����������е�ID
		bool bgMusicPlaying;	//������Ч�Ƿ��ڲ���״̬
		string strFightMusic;	//ս����Ч�ļ���
		int FightMusicID;	//ս����Ч�����������е�ID
		bool bFightMusicPlaying;	//ս����Ч�Ƿ��ڲ���״̬
		bool bPlayerPointLight;		//�����Ƿ���ӵ��Դ
		float fPlayerPointLightRadius;	//���ǵ��Դ�İ뾶
		DWORD dwPlayerPointLightColor;	//���ǵ��Դ����ɫ
		bool bUnderWater;			// �Ƿ���ˮ�³���
	};

private:
	map<long/*regionID*/,tagRegion> m_mapRegion;
	vector<string>		m_vectorLoadingTips;	// Loadingʱ�����ʾ

/////////////////////////////////////////////////////////////////////////
// ��Ϸ����
/////////////////////////////////////////////////////////////////////////
public:
	struct tagSetup
	{
		tagSetup():bSound(true),bMusic(true),bTeamDisable(true){}

		bool bTeamDisable;
		bool bTradeDisable;
		bool bFriendDisable;

		BOOL	bFullscreen;
		long	lWidth;
		long	lHeight;
		long	lBitsPerPel;
		long	lFrequency;
		long	lTexturemode;
		bool	b2DLinear;		//�Ƿ���2D�����������ģʽ
		bool	b3DLinear;		//�Ƿ���3D�����������ģʽ
		long	lLight;
		long	lModel;
		long    lnotsimplemodelnum;
		long	lShadow;
		long	lTexture;
		bool	bRoleFlash;			//�Ƿ�����ɫ��������Ч��
		bool	bRoleShake;			//�Ƿ�����ɫ������Ч��
		bool	bShock;				//�Ƿ���ĳЩ�����ͷ�ʱ,��Ļ��Ч��
		long	lWeather;
		long	lControl;
		long	lViewRange;
		long    lAntiAliasing;      //ȫ�������
		bool	bSound;
		bool	bMusic;
		bool    bSpeech;
		float	fSoundVolume;
		float	fMusicVolume;
		float	fSpeechVolume;
		bool    bUseVSync;           //��ֱͬ��
		long    lFrameControl;        //֡������
		bool    bupdateonlostfoucs;
		bool    bfreecamera;
		bool    b360viewrangecamera;
		bool    bAotuRestorecamera;     //�Ƿ��Զ��ָ��ӽ�


		//����������ʾ
		bool	bOthersFootstep;			//�Ƿ񲥷�������ҵĽŲ���
		long	l3DSoundsRange;				//���Բ���������ҹ��������������루�������㣩

		// ���Ի��� - ���������ʱ���͹���
		float	fStr2MinAtk[OCC_Max];
		float	fStr2MaxAtk[OCC_Max];
		float	fStr2Burden[OCC_Max];
		float	fStr2Hit[OCC_Max];
		float	fDex2Hit[OCC_Max];
		float	fDex2Speed[OCC_Max];
		float	fDex2Flee[OCC_Max];
		float	fCon2MaxHp[OCC_Max];
		float	fCon2Def[OCC_Max];
		float	fInt2MaxMp[OCC_Max];
		float	fInt2Resistant[OCC_Max];

		//��Ʒ��ֵ��
		float	fBasePriceRate;

		//��Ʒ�ۼ���
		FLOAT	fTradeInRate;

		//��Ʒ����۸�ϵ��
		float	fRepairFactor;

		long	lHitTime;	// ����ʱ��
		WORD	wBaseRp_Lv1;
		WORD	wBaseRp_Lv2;

		//ɱһ���˶�Ӧ��PKֵ
		WORD	wPkCountPerKill;

		char	szTalkWorldGoodsName[32];	// ���纰����Ʒԭʼ��
		int		nTalkWorldGoodsNum;			// ���纰����Ʒ����
		DWORD	dwTalkWorldMoney;			// ���纰����Ǯ����

		char	szTalkCountryGoodsName[32];	// ���Һ�����Ʒԭʼ��
		int		nTalkCountryGoodsNum;		// ���Һ�����Ʒ����
		DWORD	dwTalkCountryMoney;			// ���Һ�����Ǯ����

		char	szTalkCountryUnionGoodsName[32];	// ���Һ�����Ʒԭʼ��
		int		nTalkCountryUnionGoodsNum;			// ���Һ�����Ʒ����
		DWORD	dwTalkCountryUnionMoney;			// ���Һ�����Ǯ����
	};
	struct	tagAutoRestorPlayerSetup
	{
		string	HpGoods1OriginName;
		string	HpGoods2OriginName;
		string	MpGoods1OriginName;
		string	MpGoods2OriginName;
		bool	AllHpCheck;
		bool	HpCheck1;
		int		Hp1Value;
		bool	HpCheck2;
		int		Hp2Value;
		bool	AllMpCheck;
		bool	MpCheck1;
		int		Mp1Value;
		bool	MpCheck2;
		int		Mp2Value;
	};
	struct tagGameViewSetup
	{
		float	ViewFrustumRange;			//D3D��ͼ��׶�Ƕ� Ӱ�쳡����Ұ
		float	ViewPlanformAngle;			//������������Ӹ߶�
	};
	struct tagTabSet 
	{
		// Tabѡ�������ر��
		bool    bEnemyPlayer;			// �й����
		bool    bPiratePlayer;			// �������
		bool    bMonster;				// ����
		bool    bNotTeamPlayer;			// �Ǳ������
	};
private:
	tagSetup		m_Setup;
	tagAutoRestorPlayerSetup	m_RestoreSetup;
	CDupliRegionSetup	*m_pDupliRegionSetup;
	tagGameViewSetup	m_GameViewSetup;
	tagTabSet		m_TabSet;

	//�ڴ�Ӱ���ļ�
	CMemClient	*m_pMemClient;
	//ÿ�����²����ļ�¼��ջ���ļ�����
	char m_pszRunStackFile[MAX_PATH];
public:
	tagSetup*	GetSetup()			{return &m_Setup;}
	tagTabSet*	GetTabSet()			{return &m_TabSet;}
	// Tab�����װ��
	void LoadTabSet();
	void SaveTabSet();

	void UpdateSetup();				// ˢ������
	void PresentScence();
	void LoadSetup();
	void SaveSetup();
	tagAutoRestorPlayerSetup *GetPlayerRestoreSetup() {return &m_RestoreSetup;}
	void LoadRegionData();
	map<long,tagRegion>* GetRegionSetup()		{return & m_mapRegion;}
	CDupliRegionSetup* GetDupliRegionSetup()	{return m_pDupliRegionSetup;}
	const char* GetStatckFileName()	{return m_pszRunStackFile;}

	tagGameViewSetup	*GetViewSetup()			{return & m_GameViewSetup;}
	void	LoadGameViewSetup();

	GoodsExSetup*		GetGoodsExSetup()			{return &m_GoodsExSetup;}
	CFactionManager*	GetFactionManager()			{return &m_FactionManager;}
	CUnionManager*		GetUnionManager()			{return &m_UnionManager;}

//////////////////////////////////////////////////////////////////////////
// �Զ���½
//////////////////////////////////////////////////////////////////////////
public:
	struct tagAutoLogin
	{
		string strCdkey;
		string strPassword;
		string strWorldName;
		long lPlayerPos;
	};

	bool IsAutoLogin()	{return m_bAutoLogin;}
	void SetAutoLogin(bool b)	{m_bAutoLogin=b;}
	tagAutoLogin* GetAutoLogin()	{return &m_stAutoLogin;}

private:
	tagAutoLogin m_stAutoLogin;
	bool m_bAutoLogin;

public:
	// �����ı���ʾ
	HRESULT DrawString(int x, int y, char* pString, D3DCOLOR Color, int nLength, int nLineSpace);
	HRESULT DrawStringEx(int x, int y, char* pString, D3DCOLOR Color, D3DCOLOR BkColor, D3DCOLOR bkBarColor, int nLength, int nLineSpace);	//���ƴ�������ַ���
	long GetStringLine(char* pString, int nLength);

	// modify by lpf 2007-11-27 - ȡ����fScale��bScale�����Ĵ���,��������һ��iStyleIndex����
	void TextOut(int iX, int iY, const char * pszString, int iLen, D3DCOLOR color, int iStyleIndex = 0);

	void DrawText(int iX, int iY, const char * pszString, PFONTINFO pFontInfo = NULL);					//����������Ϣ������ı�
	void DrawText(int iX, int iY, const char * pszString, int iLen, PFONTINFO pFontInfo = NULL);		//����������Ϣ�����ָ�����ȵ��ı�

	void GetFontInfo(PFONTINFO pFontInfo);				//�õ�2D��Ⱦ�㵱ǰ������Ⱦ��Ϣ
	void SetFontInfo(const PFONTINFO pFontInfo);		//�趨2D��Ⱦ�㵱ǰ������Ⱦ��Ϣ

	long GetFontWidth(int iStyleIndex);
	long GetFontHeight(int iStyleIndex);
	int	 CalcTextWidth(const char *pszTextBuffer, int iLen);

	void NumberOut(int iX,int iY,const char *pszString,int iLen,D3DCOLOR color,float fScale = 1.0f,bool bScale=true);
	void NumberOut1(int iX,int iY,const char *pszString,int iLen,D3DCOLOR color,float fScale = 1.0f,bool bScale=true);
	long GetNumberWidth();
	long GetNumberHeight();

	void DrawLine(int iStartX,int iStartY,int iEndX,int iEndY,D3DCOLOR color = 0XFFFFFFFF, bool bScale=true);
	void DrawSolidQuad(int iX,int iY,int iWidth,int iHeight,D3DCOLOR color = 0XFFFFFFFF);
	void DrawSolidQuadRect(int iX1,int iY1,int iX2,int iY2,D3DCOLOR color = 0xFFFFFFFF);

	void DrawWireQuadRect(int iX1,int iY1,int iX2,int iY2,D3DCOLOR color = 0XFFFFFFFF);

	void DrawToolTip(int nX,int nY,int nWidth,int nHeight,const char* pStr,D3DCOLOR textColor);

//////////////////////////////////////////////////////////////////////////
//	����
//////////////////////////////////////////////////////////////////////////
public:
	WeatherManager* GetWeatherManager()		{return m_pWeatherManager;}

protected:
	WeatherManager* m_pWeatherManager;

//////////////////////////////////////////////////////////////////////////
//	����ϵļ���ͼ��
//////////////////////////////////////////////////////////////////////////
public:
	void SetHandSkillIcon(WORD w)	{m_wHandSkillIcon=w;}
	WORD GetHandSkillIcon()			{return m_wHandSkillIcon;}
	GameModel::AnimInfo * GetHandSkillIconAnimInfo() { return &m_tAnimInfo;}

protected:
	WORD m_wHandSkillIcon;		// ������õļ���ͼ��
	GameModel::AnimInfo m_tAnimInfo;
	
//////////////////////////////////////////////////////////////////////////
//	��Ļ���ĵ�
//////////////////////////////////////////////////////////////////////////
public:
	float GetScrollCenterX()		{return m_fScrollCenterX;}
	void SetScrollCenterX(float f)	{m_fScrollCenterX=f;}
	float GetScrollCenterY()		{return m_fScrollCenterY;}
	void SetScrollCenterY(float f)	{m_fScrollCenterY=f;}

protected:
	float	m_fScrollCenterX;
	float	m_fScrollCenterY;


//////////////////////////////////////////////////////////////////////////
//	���һ�ε��NPC id
//////////////////////////////////////////////////////////////////////////
public:
	const CGUID&	GetLastTouchNpcID()			{return m_LastTouchNpcID;}
	void	SetLastTouchNpcID(const CGUID& ID)	{m_LastTouchNpcID=ID;}

private:
	CGUID	m_LastTouchNpcID;

//////////////////////////////////////////////////////////////////////////
//	����Shape������
//////////////////////////////////////////////////////////////////////////
public:
	//void UpdateMoveShapePosition();
	void UpdateMoveShapePositionEx();
	void UpdateAnimation();					//���¶���,��ֹʧȥ�����,���ж��󶯻���ͬ������

//////////////////////////////////////////////////////////////////////////
//	������������Ч�Ķ�Ӧ�б�
//////////////////////////////////////////////////////////////////////////
public:
	struct tagArcheryEffect
	{
		DWORD	dwWeaponGroundID;		//	��������ͼ��
		DWORD	dwArcheryEffect;		//	���ȥ����Ч
	};

	void LoadArcheryEffect(const char* filename);
	DWORD GetArcheryEffect(DWORD dwWeaponGroundID);

protected:
	list<tagArcheryEffect>		m_listArcheryEffect;	// ��������Ч�Ķ�Ӧ�б�

//////////////////////////////////////////////////////////////////////////
//	�����ļ���Ӧ�б�
//////////////////////////////////////////////////////////////////////////
public:
	struct tagFace
	{
		string strFaceCode;		//�������
		int iStartIndex;		//��ʼ���
		int iEndIndex;			//�������
		int iCurCycle;			//��ʼ��������
		int iCycle;				//��������
		int iCurIndex;			//��ǰ֡
	};
	void LoadFace(const char* filename);
	void UpdateFace();
	list<tagFace> m_listFace;	//�����Ӧ�б�

	//����ı���ṹ
	struct tagFaceInfo
	{
		int iX;			//x,y����
		int iY;
		int iWidth;		//��͸�
		int iHeight;
		RECT rc;		//���ε�����
	};

	// Server��ʱ��
	public:
		bool m_bMsgTimeStart;
		unsigned long m_ulServerTime;
		unsigned long m_ulClientTime;

//������ʾ��Ϣ
private:
	bool				m_bDisplayInfo;			// �Ƿ���������ʾ�׶�
	list<stTopInfo>		m_TopInfos;				// ������ʾ��Ϣ
	stCurTopInfo		m_stCurTopInfo;			// �����ĵ�ǰ��ʾ����
	long				m_lDisplaySpeed;		// ��ʾ���ֵ��ٶ�
	long				m_lDisplayIntel;		//��ʾʱ����
	DWORD				m_dwDisplayTime;		// ��ʾ��ʾ��Ϣʱ���
public:
	//�ͻ��˶�����ʾ��Ϣ����
	void RunTopInfo();
	//�����ʾ��Ϣ
	void AddTopInfo(long lTopInfoID,long lFlag,long lParam,string strInfo);
	//�õ���ʾ��Ϣ������
	bool GetCurTopInfo(stCurTopInfo& TopInfo);
	//��ʾ������Ϣ
	void DrawTopInfos();

//======================================================================================
private:
	struct tagWinMessageParam
	{
		UINT message;
		WPARAM wParam;
		LPARAM lParam;
	};
	//struct t
	list<tagWinMessageParam> m_quMessage;
public:
	bool PushWinMessage(UINT message, WPARAM wParam, LPARAM lParam);
	bool PopWinMessage();
//======================================================================================



//////////////////////////////////////////////////////////////////////////
// ��̬���(�����鶯��) add by lpf(2008.04.09)
// - ��������Ͷ�����������
//////////////////////////////////////////////////////////////////////////
private:
	CFaceActionList * m_pFaceActionList;						//��̬�����б�

public:
	CFaceActionList * GetFaceActionList()		{ return m_pFaceActionList; }		//�õ���̬�����б�

//=======================��ǰ���������Ϣ�е���Ʒ��Ϣ=====================
public:
	struct stChatGoodsInfo 
	{
		CGUID    idEx;
		CGoods * pGoods;
		stChatGoodsInfo()
		{
			idEx	= CGUID::GUID_INVALID;
			pGoods	= NULL;
		}
	};
private:
	list<stChatGoodsInfo> m_lsChatGoodsInfo;
public:
	list<stChatGoodsInfo> *GetChatGoodsInfoList()	{return &m_lsChatGoodsInfo;}
	CGoods * FindGoodsInfo(const CGUID &guid);
	//========================================================================

//////////////////////////////////////////////////////////////////////////
// NPC������Ч���ȼ� liuke (2008.12.03)
// - ������Ч�����ȼ��Լ����Զ�Ӧ����ЧID
//////////////////////////////////////////////////////////////////////////
	private:
		vector<long>	m_vecMonsterQuestEffect;		// ������Ч���ȼ��б�
		void LoadMonsterQuestEffect(const char *filename);
	public:
		long GetMonsterQuestEffectID(long lIndex);			// ���ݴ�������ȼ����õ���Ӧ����ЧID
		long GetMonsterQuestEffectLevel(long lEffectID);	// ���ݴ������ЧID���õ���Ӧ�����ȼ�

///////////////////////////////////////////////////////////////////////////
//д��ע�����Ϣ
//-���ͻ���Ŀ¼����Ϣд��ע��� zw(2008.12.10)
///////////////////////////////////////////////////////////////////////////
	private:
		bool m_bReg;                            //д��ע���ı�־����ʼ��Ϊfalse,д���Ϊtrue��
	public:
		bool WriteReg(string strPath,string KeyValue);                        //����Ϣд��ע���

	//------------���Գ�����Ϣ����------------
		void ProcessTestMessage();
//����ұ�־
	private:
		bool m_bIsBeCheateForbid;				//�Ƿ񱻷���� ��ֹ��½	
	public:
		void SetIsBeCheatForbid(bool bForbid)	{m_bIsBeCheateForbid = bForbid;}
		bool IsBeCheatForbid()					{return m_bIsBeCheateForbid;}
		void ResetCheatSystem();		//���÷����ϵͳΪ��ʼ״̬
////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
	public:
		void CloseIGW();                       //�ر�IGW
		void OpenIGW();                        //��ʼ��IGW
		LRESULT OnWindowProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);     //IGW���WINDOOWS��Ϣ�ص�
		void ShowLoginDialog();                     //��ʾȦȦ��½�Ի���
		void IGWBalance();                          //��ѯ���
		void IGWOnLostDevice();                     //��ʧ�豸
		void IGWOnResetDevice();                    //�����豸
		void NodifyAudioChanged();                  //�����ı�
		void ConnectLoginServer(const char* const logIP,long logPort,const char* SonId,string wsName="");   //ʢ��ͨ��֤��½ls����Ϣ����
		void SetPlayerInfo(const char* Name,int iSex);       //�����������
		void OpenUrl(const char* UrlId);            //�����Ѳ����ķ���
		void SetSndaid(const char* SndaId){m_strSndaid = SndaId;};   //����ʢ�������ʺ�
		const char *GetSndaid(){return m_strSndaid.c_str();};        //���ʢ�������ʺ�
		void SetIsSadoLog(bool l){m_bIsSadoLog=l;};                  //���ô˴ε�½�Ƿ���ʢ��ͨ��֤��½
		bool GetIsSadoLog(){return m_bIsSadoLog;}                    //��ô˴ε�½�Ƿ���ʢ��ͨ��֤��½
		void SetSonID(const char *t){m_strSonId = t;};               //�������ں�̨��֤��token��
		const char *GetSonID(){return m_strSonId.c_str();};          //������ں�̨��֤��token��
		void SDOLogOut();                                             //igw�ĵǳ�����
		void SetSDOFocus();		                                      //����igw�Ľ���
		void SetSadoLogPageOpen(bool l){m_bSadoLogPageOpen = l;};     //����ȦȦ��½�����Ƿ��
		bool GetSadoLogPageOpen(){return m_bSadoLogPageOpen;};        //���ȦȦ��½�����Ƿ��
		void ReleaseIGW();
		void ReadLogMode();                                           //�������ж�ȡ��½ģʽ
		void ReLoadGame();
		void SetAreaId(int i){m_iAreaID=i;};                          //������Ϸ��ID
		int GetAreaId(){return m_iAreaID;};                           //������Ϸ��ID
		void SetGroupId(int i){m_iGroupID=i;};                          //������Ϸ��ID
		int GetGroupId(){return m_iGroupID;};                           //������Ϸ��ID
		bool GetIGWFocus();                                           //IGW�Ƿ��н���
		bool GetIsShowSdo(){return m_bIsShowSdo;};
		void SetIsShowSdo(bool b){m_bIsShowSdo = b;};                    //�����Ƿ���ʾsdo����
		bool ServerlistDat(){return m_bServerlistDat;};
		bool AutoQuestTrace(){return m_bQuestTrace;};      

		void SaveLastVersion();
		void LoadLastVersion();
		bool CompareVersion();

		void LoadOtherExeIni();                                   //���ڶ�ȡ����exe��������������ļ�
		void RunOtherExe();                                       //������������������

	private:
		string m_strSonId;                         //���ں�̨��֤��token��
		string m_strSndaid;                        //ʢ��ͨ��֤�������ʺ�
		bool m_bIsSadoLog;                         //���ε�½�Ƿ���ʢ��ͨ��֤��½
		bool m_bSadoLogPageOpen;                   //ʢ��ͨ��֤��½�����Ƿ��
		int  m_iAreaID;                            //��Ϸ��ID
		int  m_iGroupID;                           //��Ϸ��ID
		bool m_bIsShowSdo;                         //�Ƿ���ʾsdo����

		bool m_bServerlistDat;                     //�Ƿ��ȡserverlist�ı����ļ� 
		bool m_bQuestTrace;                        //�Ƿ��Զ���ʾ����׷��
		bool m_bShowUserTreaty;                    //�Ƿ���ʾ�û�Э��

		string m_LastVersion;                      //�ϴοɻ��˰汾��

		vector<string>   m_VecOtherExeFile;        //����exe�ļ����б�
};



////////////////////////////////////////////////////
//	ͨ�ýӿ�
////////////////////////////////////////////////////
extern bool CreateGame();
extern bool DeleteGame();
extern CGame* GetGame();

extern CMouse* GetMouse();
extern CKeyBoard* GetKeyboard();
extern CGuangMGUIApp* GetGuangMGUIApp();

extern bool g_bDebug;
extern DWORD g_LoginTime;
extern bool g_bLogin;
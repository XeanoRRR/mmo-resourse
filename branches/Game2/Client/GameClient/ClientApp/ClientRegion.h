
#pragma once

#include "region.h"

#include "../../EngineExtend/Gamebuildlist.h"
#include "../../EngineExtend/Gameskybox.h"
#include "RegionCamera.h"
#include "Other/AlphaFadeObjectList.h"
#include "../../EngineExtend/CBitmapX.h"

class CRegionCamera;
class CMoveShape;
class CMonster;
class CMiniMap;
class CSkill;
class CEffect;
class GameMap;
class GameBaseEnvEffect;
class GameBaseEnvCreature;
class CFindPath;
class CPlayer;
class CGoods;

typedef enum eRegionWarType
{
	RWT_Normal=0,	//��ͨ����
	RWT_NormalWar,	//��ͨս��
	RWT_VillageWar,	//��ս
	RWT_CityWar		//��ս
}eRWT;

typedef enum eRegionPKType
{
	RPKT_None = 0,	//�ù���δ����
	RPKT_No_PK,		//����PK����,������PK
	RPKT_PK,		//����PK����,����PK,Ϊ����PK����
}eRPKT;

//�����ռ�����
typedef enum eRegiogSpaceType
{
	RST_Land=0,	//½��
	RST_Warter,	//ˮ��
	RST_Sky,	//���
}eRST;

enum REGION_LIGHT
{
	RL_OUTDOOR,			//����
	RL_CAVE				//��Ѩ
};

enum CONTROL_CAMEAR_OPERATION{ CCO_NONE,CCO_MOVE,CCO_ROTATE,CCO_SCALE,CCO_FLATMOVE};

struct PLAYERDIST 
{
	CPlayer* p;
	float dist;
	bool operator < (PLAYERDIST p){return dist < p.dist;}
};

// �ͻ���ר�õĳ�����
class CClientRegion :
	public CRegion
{
public:
	// �����������������Ų���
	typedef struct tagRegionSound
	{	
		float xpos;
		float zpos;
		bool bPlaying;
		string filename;
		int channel;
		GameBuildList::Build::tagSoundSource Sound;

	}REGION_SOUND,*REGION_SOUND_PTR;

	// ������Ч
	typedef struct tagEnvSound
	{
		bool bLoop;	
		bool bPlaying;
		int channel;
		string strFileName;
		long lMinTime;
		long lMaxTime;
		DWORD dwStartTime;
		long lIntervalTime;
		int	nLoops;

	}ENV_SOUND,*ENV_SOUND_PTR;

	// ***************************�ݵ���Ϣ��09.9.29���ƣ�************************
	struct tagSPointInfo 
	{
		BYTE		bID;				// �þݵ�ID
		BYTE		bState;				// �þݵ��״̬
		long		lPosX;				// �þݵ��ڱ������е�λ��
		long		lPosY;
		string		strPointName;		// �þݵ�����֣�ͬ������ID��
	};
	// **************************************************************************

public:
	CClientRegion(void);
	virtual ~CClientRegion(void);

//////////////////////////////////////////////////////////////////////////
//	ͼ�β���
//////////////////////////////////////////////////////////////////////////
public:

	void EnableFog(DWORD Color, float fDensity=0.05);	// ����
	void DisableFog();									// ����

	void UpdateCamera(void);							// �����ӽ�
	void UpdateRegionCamera(float x, float y);			// ���³������
	//�����������ʽ
	void TransformCamer(long lOperaMode,long lParam1,long lParam2,long lParam3);

	float CalcDirection(float sx, float sy, float sh, int dx, int dy);	// ����ָ���㵽���ķ���
	DWORD CalcMoveDirection(POINT &ptMouse);
	void  CalculateFinalTransformMatrix(D3DXMATRIX &matFinal);	// ��������ͷ�����ֵ�λ��
	bool CalcMousePoint(long mx, long my, float &x, float &y);	// ��ȡ���ָ��������
	CShape* GetHoverShape(void);								// ��ȡ���ָ���Shape
	render::Camera * GetCamera() { return m_pCamera;}			// ��������

	CRegionCamera * GetRegionCamera()							{ return m_pRegionCamera; }
	void			SetRegionCamera(CRegionCamera * pRegionCamera)		{ memcpy(m_pRegionCamera, pRegionCamera, sizeof(CRegionCamera)); }

	DWORD m_dwHoverGrid;

	render::Frustum* GetFrustum()	{return m_pFrustum;}
	float GetCellHeight(float x, float y);					// ������ӵĸ߶�
	void  Coordinate3DTo2D(float f3DX,float f3DY,float f3DH,long& l2DX,long &l2DY);//3ά����ת��2ά����

	/*
	*	���ܣ����һ���ݵ���Ϣ
	*	ϸ�������� ����ҽ��볡����ʱ�򣬽ӵ����������͵ĸó����ľݵ���Ϣ�����
	*	������ stSPInfo - �ݵ���Ϣ
	*	����ֵ�� ��
	*	����: ����
	*	��������: 09.09.29
	*	�޸���־: -
	*/
	void	AddSPointInfo(tagSPointInfo stSPInfo);

	/*
	*	���ܣ��ı�һ���ݵ��״̬
	*	ϸ�������� ��ĳ���ݵ��״̬�����仯֮�󣬷�����֪ͨ�ͻ��˸ı�
	*	������ szPointName - �ݵ�����
	*			bState		- �ݵ�״̬
	*	����ֵ�� ��
	*	����: ����
	*	��������: 09.09.29
	*	�޸���־: -
	*/
	void	ChangeSPointState(const char* szPointName, BYTE bState);
	
	
	/*
	*	���ܣ��õ�ĳ���ݵ������
	*	ϸ�������� ���ݴ���ľݵ����֣��õ���ǰ��ҿ����ĸþݵ�����
	*	������ szPointName - �ݵ�����
	*	����ֵ�� �ݵ�����
	*	����: ����
	*	��������: 09.09.29
	*	�޸���־: -
	*/
	void	GetDescTxtBySPState(string &strDesc, const char* szPointName);

	/*
	*	���ܣ���ñ������µ����оݵ���Ϣ
	*	ϸ�������� ����Ҫ��ʾ�����µľݵ���Ϣʱ�����
	*	������ ��
	*	����ֵ�� m_vecSPoint - �ݵ��б�
	*	����: ����
	*	��������: 09.09.29
	*	�޸���־: -
	*/
	map<string,tagSPointInfo> &	GetSPointInfo()		{return m_mapSPoint;}
	
	/*
	*	���ܣ���ñ������µ����оݵ���Ϣ
	*	ϸ�������� ����Ҫ��ʾ�����µľݵ���Ϣʱ�����
	*	������ ��
	*	����ֵ�� true������		false���ر�
	*	����: ����
	*	��������: 09.09.29
	*	�޸���־: -
	*/
	bool	IsOpenSPoint()		{return m_bOpenSPoint;}
	
	/*
	*	���ܣ���ñ������µ����оݵ���Ϣ
	*	ϸ�������� ����Ҫ��ʾ�����µľݵ���Ϣʱ�����
	*	������ bFlag = true������		bFlag = false���ر�
	*	����ֵ�� -
	*	����: ����
	*	��������: 09.09.29
	*	�޸���־: -
	*/
	void	SetIsOpenSPoint(bool bFlag)		{m_bOpenSPoint = bFlag;}

private:
	CBitmapX * m_pBossBackImage;							// Boss�Ĵ�Ѫ������ͼƬ
	CBitmapX * m_pBossImage;								// Boss�Ĵ�Ѫ��ͼƬ

	//ͼ�β���
	CRegionCamera * m_pRegionCamera;						//�������

	//ͼ�νӿ�
	CONTROL_CAMEAR_OPERATION m_eCamOpType;					//�������������
	render::Camera  * m_pCamera;							//�����
	render::Frustum * m_pFrustum;							

	GameMap * m_pGameMap;
	GameSkybox* m_pGameSkyBox;
	DWORD m_dwSkyBoxID;
	GameBaseEnvEffect   * m_pGameEnvEffect;
	GameBaseEnvCreature * m_apGameEnvCreature[4];			//��������ָ������

	list<REGION_SOUND> m_RegionSoundList;
	list<ENV_SOUND> m_EnvSoundList;

	D3DLIGHT9 m_RegionLight;

	REGION_LIGHT	m_eRegionLightType;
	//---------------------------------------------------------------------------------
	// �����Ӱ
	static int CONST MAX_SHAPE = 128;					//������Ļ�������50��ģ��
	CShape *m_pShapeList[MAX_SHAPE];					//��Ҫ�����ģ���б�
	DWORD m_dwNumOfShape;
	//---------------------------------------------------------------------------------
	long m_lTaxRate;									//˰��

	eRegiogSpaceType    m_eSpaceType;
	eRegionWarType		m_WarRegionType;				//ս����������
	eRegionPKType		m_ePKRegionType;				//ս����������

	bool m_bChangeEquip;								//�Ƿ���Ըı�װ��(Ŀǰ��Ҫ������׵���ʾ,���粻��ʾ)

	CAlphaFadeObjectList m_AlphaFadeObjectList;			//͸����������б�

	map<string,tagSPointInfo>	m_mapSPoint;			// �ݵ��б�09.9.29���ƣ�
	bool	m_bOpenSPoint;								// �������Ƿ����ݵ�

//////////////////////////////////////////////////////////////////////////
//	�ӿ�
//////////////////////////////////////////////////////////////////////////
public:
	virtual	void Run();
	virtual	void AI();
	virtual	bool Display();
	virtual	void ScrollCenter(float x, float y);
	virtual BOOL Load();								//��ȡ�ͻ�����Դ
	virtual BOOL Save();								//�洢
	void Init(void);
	void SyncUpdate();
	void Exit(void);

	// �Ƴ�OBJECT
	void RemoveObject(CBaseObject* pObject);
	void ShowShopName(CPlayer * pPlayer, float fPos);

	//#�����ӵ���Ʒ����ĺ���
	bool AddGoods(CGoods *pGoods,DWORD dwPos,LONG lExtendID);
	CGoods *RemoveGoods(CGUID& guID,DWORD dwPos,LONG lExtendID,DWORD dwAmount);
	bool ClearGoods();

	//�õ�һ�����ĵ㸽��û��MoveShape�ĸ���
	void GetNoShapeCell(long lCenterX,long lCenterY,long lRadius,long lFirstDir,long& lX,long& lY);
	//�õ�һ���ռ���ĳ�����ϵ�һ�����ӣ��ø����ϵ�MoveShape��������
	void GetLeastShapesCellFromSpace(int nObjCenterX,int nObjCenterY,
		int nObjFigureX,int nObjFigureY,int nMaxDistance,int nDiretion,
		long& lX,long& lY);

	//�õ�һ��Բ�οռ���ĳ�����ϵ�һ�����ӣ��ø����ϵ�MoveShape��������
	void GetLeastShapesCellFromCircularSpace(int nObjCenterX,int nObjCenterY,
		int nObjFigureX,int nObjFigureY,float nMaxDistance,long lDir,
		long& lX,long& lY);
	
	// ��ȡһ�������Ƿ���Shape
	CShape* GetShape(long x, long y);

	// ��ȡһ�������Ƿ���Shape
	CGoods* GetShapeGoods(long x, long y);

	// ��ȡһ�����ָ����Χ���Ƿ���Shape
	CShape* GetShapeFromSpace(long x, long y, long lSize);


	// ���һ�����ϵ����ж���    ADDBY�����ƣ�09.2.9��
	CShape* GetAllShapeByType(list<CShape *> & lsShape, long x, long y, long type = -1);

	/****************************************************************************************
	* ADDBY�����ƣ�09.2.5��
	* ��ȡһ����Χ�ڣ�һ����ָ���������ָ�����͵�shape
	* ����ж��shape��Ŀ���ľ��붼��С����˳ʱ���Ŀ���ָ���ķ����ϵ�shape��ʼ����
	****************************************************************************************/
	CShape* GetLeastShape(list<CShape *> & lsSelectedShape,long x,long y,long lDir,long lOffLen,long lType = -1);

	// ���������Χ������NPC������״̬
	void UpdataAllNpcQuestState();

	CAlphaFadeObjectList & GetAlphaFadeObjectList()		{ return m_AlphaFadeObjectList; }			//�õ�Alpha��������

	//==============07.10.16liuke���NPCͷ����ʾ������ʾ��Ϣ����==================
	void DisplayQuestInfoByNpc(CMonster *pMonster, long lEffectID);
	// ��ʾ����ĳƺ��Լ�ͼƬ
	void DisplayMonsterTitle(CMonster* pMonster, bool bDisplayTitle=false, float fDrawHeight = 2.5f);
	//============================================================================

	// ���������Ϣ�����ʾ����
	float DisplayHideInfo(CPlayer* pPlayer);

	float DisplayShapeName(CShape* pShape, bool bDisplayName=false, bool bDisplayBloodBar = false);		//��ʾShape����
	void DisplayShapeHoverArrow(CShape* pShape);
	void DisplayShapeCountryPirate(CPlayer * pPlayer, bool bDisplayCountry, bool bPirate);				//��ʾShape���Һͺ�����־

	void DisplayTextureProjective();


	GameMap * GetGameMap() { return m_pGameMap;}
	GameSkybox * GetGameSkyBox() { return m_pGameSkyBox;}
	void	SwitchDayOrNight();
	inline	REGION_LIGHT	GetClientReionLightType()	{return	m_eRegionLightType;}
	void	SetPointLight();
	void	SetDirectLight();
	list<REGION_SOUND>* GetRegionSoundList(){return &m_RegionSoundList;}
	list<ENV_SOUND>* GetEnvSoundList(){return &m_EnvSoundList;}

	D3DLIGHT9* GetRegionLight(){return &m_RegionLight;}
	void SetRegionLight(D3DLIGHT9 light){m_RegionLight = light;}

	// ������Ч
	CEffect *PlayEffect(long lGraphicsID, float x, float y, float hight=0.f, float fDir=-1.f,bool bAsyn=true);
	void PlayEffectFoot(CPlayer * pPlayer);			//���������Ч

	//==================07.12.20liuke====================
	// ����ָ����������Ч����
	CEffect	*FindEffectByIndex();
	//===================================================

	void	SetTaxRate(long lTaxRate)		{m_lTaxRate = lTaxRate;}
	long	GetTaxRate()					{return m_lTaxRate;	}
	
	void	SetWarType(eRWT eType)			{m_WarRegionType = eType;}
	eRWT GetWarType()						{return m_WarRegionType;}

	// add by Lpf at 2009.10.13
	void  SetPKRegionType(eRPKT eType)		{ m_ePKRegionType = eType; }
	eRPKT GetPKRegionType()					{ return m_ePKRegionType; }

	bool IsChangeEquip()					{ return m_bChangeEquip; }
	void SetChangeEquip(bool b)				{ m_bChangeEquip = b; }

	eRST GetSpaceType(){return m_eSpaceType;}
	void SetSpaceType(eRST eType){m_eSpaceType=eType;}

	//������ɫ���볡��
	void OnMainPlayerEnter();
/////////////////////////////////////////////////////////////////////
// ��Ϣ��Ӧ
/////////////////////////////////////////////////////////////////////
public:
	virtual void OnMessage(CMessage*);

//////////////////////////////////////////////////////////////////////////
//	С��ͼ
//////////////////////////////////////////////////////////////////////////
public:
	void DisplayMiniMap(int x, int y);
	CMiniMap* GetMiniMap()	{return m_pMiniMap;}

protected:
	CMiniMap *m_pMiniMap;

public:
	void _Debug_RenderMap(void);

//////////////////////////////////////////////////////////////////////////
//	����
//////////////////////////////////////////////////////////////////////////
public:

//��ʾ�����ļ
	void ShowPlayerRecruiting(CPlayer * pPlayer, float fPos);

//��ʾ���ͷ����Ϣ
	void ShowPlayerInfo(CPlayer *pPlayer);

	const char * CClientRegion::ResolveNameStr(const char * pszOriNameStr);			//���������ַ���

//////////////////////////////////////////////////////////////////////////
//	NPC�б�
//////////////////////////////////////////////////////////////////////////
public:
	void SetNpcListLoaded(bool b)		{m_bNpcListLoaded=b;}
	bool IsNpcListLoaded()				{return m_bNpcListLoaded;}

private:
	bool m_bNpcListLoaded;		// NPC�б��Ƿ��Ѿ���ȡ

//////////////////////////////////////////////////////////////////////////
//	Ѱ·ģ��
//////////////////////////////////////////////////////////////////////////
public:
	CFindPath* GetFindPath();

private:
	CFindPath *	m_pFindPath;	// Ѱ·ģ��
};

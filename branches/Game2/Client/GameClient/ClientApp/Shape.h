#pragma once

#include "baseobject.h"
#include "Other/AlphaFadeObjectList.h"
#include "../../Engine/Render.h"

class CEffect;
class CBitmapX;
class AnimInfo;
class CDisplayModel;
class CAlphaFadeObjectList;

static int _dir[8][2] =
{
	0,-1,	1,-1,	1,0,	1,1,
	0,1,	-1,1,	-1,0,	-1,-1
};

//�ڳ�����ʰȡ��Ʒ�ĸ���˳��
static int _Pick_Order[9][2] = 
{
	0,0,	//������ڵĸ���
	-1,0,	//��
	1,0,	//��	
	0,-1,	//��
	0,1,	//��
	-1,1,	//����
	-1,-1,	//����
	1,-1,	//����
	1,1		//����
};

// 8������Ļ���˳��
static int _slip_order[8][8] =
{
	0, 7, 1, 6, 2, 5, 3, 4,
	1, 0, 2, 7, 3, 6, 4, 5,
	2, 1, 3, 0, 4, 7, 5, 6,
	3, 2, 4, 1, 5, 0, 6, 7,
	4, 3, 5, 2, 6, 1, 7, 0,
	5, 4, 6, 3, 7, 2, 0, 1,
	6, 5, 7, 4, 0, 3, 1, 2,
	7, 6, 0, 5, 1, 4, 2, 3
};

#define RET_AI_CONTINUE		1		// ����ִ��AI
#define RET_AI_BREAK		0		// �ж�ִ��AI

class CShape :
	public CBaseObject
{
public:
	CShape(void);
	virtual ~CShape(void);

	// ״̬
	enum eSTATE
	{
		STATE_PEACE,			// ��ƽ״̬
		STATE_FIGHT,			// ս��״̬
		STATE_DIED,				// ����״̬
		STATE_FEIGNDEATH,		// ����״̬
		STATE_OPEN,				// ���Ŵ�״̬
	};

	// ����
	enum eACTION
	{
		ACT_NO = -1,			// û���κζ���			
		ACT_STAND,				// վ(������ֹ)
		ACT_IDLE,				// վ��idle
		ACT_WALK,				// ��
		ACT_RUN,				// ��
		ACT_BEATTACK,			// ������
		ACT_INTONATE,			// ����
		ACT_BLOCK,				// ��
		ACT_PARRY,				// �м�
		ACT_MISS,				// ����
		ACT_MEGRIM,				// ѣ��
		ACT_DIED,				// ����(��������)
		ACT_PICK,				// ʰȡ
		ACT_ALREADYDIED,		// �Ѿ�����		
		ACT_OPEN,				// ���Ŵ�
        ACT_ALREADYOPEN,        // �����Ѵ�
		ACT_FALLDOWN,           // ˤ��

		

		// ѡ�˽��涯��
		ACT_SEL_X0,				// ����վ��
		ACT_SEL_X1,				// �ν�
		ACT_SEL_X2,				// �ӽ�x
		ACT_SEL_X3,				// �ֽ�վ��x
		ACT_SEL_X4,				// �ս�x
		ACT_SEL_X5,				// �ָ�վ��
		
		ACT_PERFORM		  = 50,		// ʩ��
		ACT_MINE		  = 751,	// �ɿ�
		ACT_MINEMAX		  = 771,	// �ɿ����ֵ
		ACT_FACEACTION	  = 800,	// ��̬�����ĳ�ʼֵ
		ACT_FACEACTIONMAX = 899,	// ��̬���������ֵ
		ACT_ATTACK		  = 950,	// ����
		ACT_MAX_PERFORM	  = 999,	// ���ʩ������
		ACT_MAX			  = 1000	// �����ٸ�����
	};

	// ����
	enum eDIR
	{
		DIR_UP = 0,
		DIR_RIGHTUP,
		DIR_RIGHT,
		DIR_RIGHTDOWN,
		DIR_DOWN,
		DIR_LEFTDOWN,
		DIR_LEFT,
		DIR_LEFTUP,
	};

	// ��λ��
	enum eLOCATOR
	{
		LOCATOR_WIND			= MAKEFOURCC('0','0','0','0'),		// �ؿ�
		LOCATOR_HEAD			= MAKEFOURCC('0','0','0','1'),		// ͷ��
		LOCATOR_FOOTCENTER		= MAKEFOURCC('0','0','0','2'),		// �ŵ�����
		LOCATOR_LEFTHAND		= MAKEFOURCC('0','0','0','3'),		// ����
		LOCATOR_RIGHTHAND		= MAKEFOURCC('0','0','0','4'),		// ����
		LOCATOR_LEFTFOOT		= MAKEFOURCC('0','0','0','5'),		// ���
		LOCATOR_RIGHTFOOT		= MAKEFOURCC('0','0','0','6'),		// �ҽ�
		LOCATOR_BACK			= MAKEFOURCC('0','0','0','7'),		// ��
		LOCATOR_LEFTWAIST		= MAKEFOURCC('0','0','0','8'),		// ����	
		LOCATOR_RIGHTWAIST		= MAKEFOURCC('0','0','0','9'),		// ����	
		LOCATOR_LEFTARM         = MAKEFOURCC('0','0','1','0'),		// ���
		LOCATOR_RIGHTARM        = MAKEFOURCC('0','0','1','1'),		// �ұ�
		LOCATOR_LEFTBACK		= MAKEFOURCC('0','0','1','2'),		// ���
		LOCATOR_RIGHTBACK		= MAKEFOURCC('0','0','1','3'),		// �Һ�
		LOCATOR_BIGSWORD		= MAKEFOURCC('0','0','1','4'),		// �󱳳���
		LOCATOR_BACKBOW			= MAKEFOURCC('0','0','1','5'),		// �󱳹�
		LOCATOR_BACKARROW		= MAKEFOURCC('0','0','1','6'),		// �󱳼���
		LOCATOR_BACKSTAFF       = MAKEFOURCC('0','0','1','7'),		// ����,��
		LOCATOR_LEFTBOXING      = MAKEFOURCC('0','0','1','8'),		// ��ȭ
		LOCATOR_RIGHTBOXING     = MAKEFOURCC('0','0','1','9'),		// ��ȭ
		LOCATOR_LEFTHURTPOINT   = MAKEFOURCC('0','0','2','1'),		// �����˵�
		LOCATOR_RIGHTHURTPOINT  = MAKEFOURCC('0','0','2','2'),		// �����˵�
		LOCATOR_HOLDPOINT		= MAKEFOURCC('0','0','2','3'),		// ���˵�

	};

///////////////////////////////////////////////////////////////
// ����
///////////////////////////////////////////////////////////////
protected:
	long  m_lCurRgnType;//���ڳ���������
	long  m_lRegionID;	// ���ڳ�����id
	CGUID m_RegionID;	// ���ڳ�����guid
	float m_fPosX;		// λ��X
	float m_fPosY;		// λ��Y
	float m_fDir;		// ����(����)(0-360)	

	bool m_bIsSelected;	// �Ƿ�ѡ��
	float m_fSpeed;		// �ٶ�
	WORD m_wState;		// ״̬
	WORD m_wAction;		// ����
	DWORD m_dwFMFTime[8];//��ӡʱ����,���8...��...�ȣ�Ӧ�ù��˰ɣ�

	float m_fHeight;	// �߶�

public:
	virtual DWORD GetFMFTime(DWORD dwIndex) const { return m_dwFMFTime[dwIndex];}
	virtual void SetFMFTime(DWORD dwIndex,DWORD dwTime) { m_dwFMFTime[dwIndex] = dwTime;}
	virtual long GetRegionID(){return m_lRegionID;}
	virtual void SetRegionID(long l){m_lRegionID=l;}
	virtual const CGUID& GetRegionGUID()		{ return m_RegionID; }
	virtual void GetRegionGUID(const CGUID& id){ m_RegionID = id; }
	virtual void SetPosXY(float x, float y);
	virtual float GetPosX()			{ return m_fPosX; }
	virtual void SetPosX(float l)	{ SetPosXY(l, GetPosY()); /*m_fPosX = l;*/ }
	virtual float GetPosY()			{ return m_fPosY; }
	virtual void SetPosY(float l)	{ SetPosXY(GetPosX(), l); /*m_fPosY = l;*/ }
	virtual eDIR GetDir();
	virtual void SetDir(eDIR dir);
	//�õ������û��ȷ���Ľӿ�
	virtual float GetDirEx() { return m_fDir; }
	virtual void SetDirEx(float f) {m_fDir = f;}

	virtual float GetSpeed()		{ return m_fSpeed; }
	virtual void SetSpeed(float l)	{ m_fSpeed = l; }
	virtual WORD GetState()			{ return m_wState; }
	virtual void SetState(WORD l)	{ m_wState = l; }
	virtual WORD GetAction()		{return m_wAction;}
	virtual void SetAction(WORD l)	{ m_wAction = l; }
	virtual void OnActionChange(){}

	virtual float GetHeight()		{return m_fHeight;}
	virtual void SetHeight(float f)	{m_fHeight=f;}

	// add by lpf - 2007.12.17
	float GetPickBoxHeight();			//�õ�ģ��ʰȡ�оֲ�����ϵ���Ը߶�
	float GetSizeBoxHeight();			//�õ�ģ�ͳߴ�оֲ�����ϵ���Ը߶�

	// add by lpf - 2009.02.05
	POINT & GetPickBoxCenterOfScreenCoord(POINT & ptScreen);			//�õ�ģ��ʰȡ�����ĵ����Ļ����
	POINT & GetSizeBoxCenterOfScreenCoord(POINT & ptScreen);			//�õ�ģ�ͳߴ�����ĵ����Ļ����

	long GetTileX();
	long GetTileY();

	virtual BYTE GetFigure(eDIR dir=DIR_UP)	{return 0;}	// ����
	virtual void SetBlock(int x, int y, unsigned short block);

	virtual bool AddShapeToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�CByteArray
	virtual bool DecordShapeFromByteArray(BYTE* pByte, long& pos);	// ����

	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����

	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData = true);

	virtual void AI(void);
	virtual	bool Display(void);
	virtual void Display(render::Camera * pCamera);
	virtual void DisplayEffect();																						// ��ʾ��Ч
			void DrawSubsectionText(const char* str,D3DXVECTOR3 pos2,D3DCOLOR color,D3DCOLOR bkcolor,D3DCOLOR bkBarColor);		//���ֶ��ַ���
	virtual void DisplayText(const char* str, D3DCOLOR color, D3DCOLOR bkcolor, float fPos, D3DCOLOR bkBarColor=0, bool bIsDisplayHPBar = false);		// ��shape������ʾ���֣��ɷֶ���ʾ�����ֶ���ʾʱ������ַ����в��硰/�����ָ
	virtual void DisplayWarBitmap(){}								//��ʾս��״̬�����ͼ��
	virtual void DisplayHoverArrow(DWORD dwColor);
	virtual void DisplayMasterName(const char* str, D3DCOLOR color, D3DCOLOR bkcolor, float fPos, D3DCOLOR bkBarColor=0);		
	void DisplayChat(const char* str, D3DCOLOR color, D3DCOLOR bkcolor, float fPos, D3DCOLOR bkBarColor=0);				// ��shape������ʾ��������
	void DisplayHpBar(CBitmapX* pBitmap, long lHpBarLong, long lHp, long lMaxHp, float fPos);												// ��ʾѪ��
	// ��shape������ʾ��Ѫ�˺�����
	void DisplayDamage(const char* str, D3DCOLOR color, D3DCOLOR bkcolor, float fPos, float fX=-1.f, float fY=-1.f, float fScale=1.0);
	// ��shape������ʾ����ͼƬ����������
	void DisplaySuck(const char* str, D3DCOLOR color, D3DCOLOR bkcolor, float fPos, float fX=-1.f, float fY=-1.f, float fScale=1.0, bool bBlastAttck=false);
	// ��shape������ʾ�������ֺͱ���ͼƬ
	void DisplayNumber(const char* str, D3DCOLOR color, D3DCOLOR bkcolor, float fPos, float fX=-1.f, float fY=-1.f, float fScale=1.0, float fScale2=1.0, bool bBlastAttck=false);	
	// ��shape������ʾ���������ֺ͸�ͼƬ
	void DisplayBlock(const char* str, D3DCOLOR color, D3DCOLOR bkcolor, float fPos, float fX=-1.f, float fY=-1.f, float fScale=1.0, bool bBlastAttck=false);	
	// ��shape������ʾ�����������ֺͷ���ͼƬ
	void DisplayRebound(const char* str, D3DCOLOR color, D3DCOLOR bkcolor, float fPos, float fX=-1.f, float fY=-1.f, float fScale=1.0, bool bBlastAttck=false);	
	// ��shape������ʾ����ͼƬ
	void DisplayMiss(D3DCOLOR color, D3DCOLOR bkcolor, float fPos, float fX=-1.f, float fY=-1.f, float fScale=1.0, bool bBlastAttck = false);	
	// ��shape������ʾ�м�ͼƬ
	void DisplayParry(D3DCOLOR color, D3DCOLOR bkcolor, float fPos, float fX=-1.f, float fY=-1.f, float fScale=1.0, bool bBlastAttck=false);	
	// ��shape������ʾ�ֿ�ͼƬ
	void DisplayResist(D3DCOLOR color, D3DCOLOR bkcolor, float fPos, float fX=-1.f, float fY=-1.f, float fScale=1.0, bool bBlastAttck=false);
	// ��shape������ʾ��ʧͼƬ
	void DisplayLost(D3DCOLOR color, D3DCOLOR bkcolor, float fPos, float fX=-1.f, float fY=-1.f, float fScale=1.0, bool bBlastAttck=false);

	virtual bool IsNeedDisplay();	// ��shape�Ƿ���Ҫ��ʾ

public:
	virtual	DWORD	GetValue(const char* strName)	{return 0;};
	virtual	DWORD	SetValue(const char* strName, DWORD dwValue)	{return 0;};
	virtual	DWORD	ChangeValue(const char* strName, long lValue)		{return 0;};

///////////////////////////////////////////////////////////////
// ��������
///////////////////////////////////////////////////////////////
public:
	void GetFacePos(long& x, long& y);				// ��ȡ��Ե�����
	CShape* GetFaceShape();							// ��ȡ��Ե�Ŀ��
	void GetDirPos(long dir, long& x, long& y);		// ��ȡdir���������
	/***********************************************************************/
	/*  zhaohang 2010-11-15 add*/
	void GetDirPos(long dir,float& x,float& y);
	/***********************************************************************/
	void GetDirPosFromDistance(long dir, long& x, long& y, DWORD lCell);		//����ָ���ķ���;���õ���Ӧ������

	long GetFaceDir();								// ��ȡ��Ե�����
	void GetPosFromRelativeDir(eDIR Dir,long &x, long &y,DWORD lDis = 1);		//�����������Է����ϣ���ȡDir�����Ͼ���ΪlDis������
	/***********************************************************************/
	/*  zhaohang 2010-11-15 add*/
	void GetPosFromRelativeDir(eDIR Dir,float &x,float &y,DWORD lDis = 1);
	/***********************************************************************/

	void GetPosFromCameraView(eDIR Dir, long &x,long &y);	 //������������ӽǷ������������λ��
	virtual long Distance(CShape* pShape);			// ������Shape��ľ��루��λ�����ӣ�
	virtual long MinXYDistance(CShape* pShape);		// ��x,y�����ϵ���С���루��λ�����ӣ�

	FLOAT GetRealDistance( FLOAT fX, FLOAT fY );	// ���������RealDistance( FLOAT fX, FLOAT fY )����ͳһ
	FLOAT RealDistance( FLOAT fX, FLOAT fY );
	LONG  RealDistance( LONG lX, LONG lY );
	LONG  RealDistance( CShape* pTarget );

	static float GetLineDirF(float sx, float sy, float dx, float dy, float fdir = 0.0f);	// ��ȡ��(sx,sy)��(dx,dy)�ķ���(0-PI)
	static eDIR RadianDirTo8Dir(float fRadianDir);//�ӷ��ȷ���ֵת����8��ö��ֵ����

	void SetAppearTime(long l)				{ m_lAppearTime = l; }
	long GetAppearTime()					{ return m_lAppearTime; }
	void SetAlphaFadeInStartTime(long l)	{ m_lAlphaFadeInStartTime = l; }
	void SetAlphaFadeInTimeLength(long l)	{ m_lAlphaFadeInTimeLength = l; }
	long GetAlphaFadeInStartTime()			{ return m_lAlphaFadeInStartTime; }
	long GetAlphaFadeInTimeLength()			{ return m_lAlphaFadeInTimeLength; }
	void SetIsDeleted(bool bIs);
	bool GetIsDeleted();
	void SetIsNeedAlphaFade(bool bIs);											//�Ƿ���ҪAlpha����
	bool GetIsNeedAlphaFade()				{ return m_bIsNeedAlphaFade; }		//�Ƿ����Alpha����
	void SetAlphaFadeMode(long l)			{ m_eAlphaFadeMode = (CAlphaFadeObjectList::eALPHA_FADE_MODE)l; }			//�õ�Alpha����ģʽ
	bool GetIsAddAlphaFaded()				{ return m_bIsAddAlphaFaded; }
	void SetIsAddAlphaFaded(bool b)			{ m_bIsAddAlphaFaded = b; }
	long GetAlphaFadeMode()					{ return m_eAlphaFadeMode; }		//�õ�Alpha����ģʽ
	float GetAlphaValue()					{ return m_fAlphaValue; }
	void SetAlphaValue(float f)				{ m_fAlphaValue = f; }

	void SetDisplayFinish(bool b)	{m_bDisplayFinish=b;}
	BOOL IsDisplayFinish()			{return m_bDisplayFinish;}
	void SetInViewRange(bool b)		{m_bInViewRange=b;}
	bool IsInViewRange()			{return m_bInViewRange;}
	void SetActionToLastFrame();

	bool CopyShapeDisplayInfo(CShape *& pOutObj);		//������ǰ�������ʾ��Ϣ���µĶ�����

protected:
	long 			 m_lAppearTime;				// ����ʱ��
	long			 m_lAlphaFadeInStartTime;	// Alpha������ʼʱ��
	long			 m_lAlphaFadeInTimeLength;	// Alpha����ʱ�䳤��
	bool 			 m_bIsDeleted;  			// �Ƿ���һ��ִ��ɾ���Ķ���
	bool 			 m_bIsNeedAlphaFade;		// �ö����Ƿ���Ҫ����Alpha�������
	bool			 m_bIsAddAlphaFaded;		// �Ƿ��Ѿ����ö�����ӵ���Alpha�����б���
	float			 m_fAlphaValue;				// ��ǰAlphaֵ
	CAlphaFadeObjectList::eALPHA_FADE_MODE m_eAlphaFadeMode;			// �ö���Alpha�������ģʽ

	AnimInfo	  * m_pAnimInfo;
	CDisplayModel * m_pDisplayModel;//��ʾģ������	

	BOOL m_bDisplayFinish;		// �Ƿ񲥷Ž���
	bool m_bInViewRange;		// �Ƿ�����ʾ��Χ��

//////////////////////////////////////////////////////////////////////////
//	��ʾ����
//////////////////////////////////////////////////////////////////////////
public:
	void SetNowDir(float f)					{m_fNowDir=f;}
	float GetNowDir()						{return m_fNowDir;}
	void Set3DRotMatrix(D3DXMATRIX dir)					{m_matdir=dir;}
	D3DXMATRIX Get3DRotMatrix()						{return m_matdir;}
	//������ʾģ������
	AnimInfo * GetAnimInfo() {return m_pAnimInfo;}
	virtual void SetDisplayModel(long lType,long lID){}
	CDisplayModel* GetDisplayModel()	{return m_pDisplayModel;}

	//��ʼ����ʾģ��
	bool SetDisplayModel(bool bAsyn = true);
	//����ģ����ʾ��Group
	virtual void SetDisplayModelGroup(bool bAsyn=true);

	void UpdateAnimation();							//���¶���

protected:
	float	m_fNowDir;			// ��ǰ����ʾ����(0-360)
	D3DXMATRIX m_matdir;        // 3D��ת����     

//////////////////////////////////////////////////////////////////////////
//	ģ����ɫ
//////////////////////////////////////////////////////////////////////////
public:
	static D3DCOLOR s_colorModelDefault;
	D3DCOLOR m_colorModel;				// ģ����ɫ
	DWORD	 m_dwCurrentModelCol;		// ��ǰ���Ƶ�ģ����ɫ

public:
	D3DCOLOR GetModelColor()			{ return m_colorModel; }
	D3DCOLOR GetCurrentModelColor()		{ return m_dwCurrentModelCol; }
	void SetModelColor(D3DCOLOR color)	{ m_colorModel=color; }

//////////////////////////////////////////////////////////////////////////
//	��������Ч
//////////////////////////////////////////////////////////////////////////
protected:
	list<CEffect*>		m_listEffect;	// ��Ч�б�

public:
	CEffect* FindEffect(long lGraphicsID, long lID = 0);		// �����Ƿ�����Ч
	CEffect* FindEffectByNum(long lNum);		// �����Ƿ�����Ч
	const CGUID& AddEffect(long lGrahpicsID, float fx=-99999.f, float fy=-99999.f, float fHeight=-99999.f, 
		bool bLoop=false, float fDir=0.f, long lID = 0,bool bAsyn=true);		// �����Ч
	const CGUID& AddEffectLocator(long lGraphicsID, CShape* pHoster, eLOCATOR eLocator, bool bLoop=false, float fDir=0.f, long lID = 0,bool bAsyn = true, float fHeightAdjust = 0.0f);	// ������Ч(��λ��)
	void DelEffect(long lGrahpicsID, long lID = 0);			// ɾ����Ч
	void DelAllEffect();						// ɾ��������Ч

	//================07.12.19liuke==================
	CEffect* FindEffectByIndex(CGUID guid);		// ����ָ����������Ч
	void DelEffectByIndex(CGUID guid);				// ɾ��ָ����������Ч
	void RemoveEffectByIndex(CGUID guid);			// ��ָ����������Ч���б����Ƴ�
	//===============================================

	static eLOCATOR GetLocatorName(DWORD dwIndex);	// ����������ȡ��λ���ֵ
	BOOL GetLocatorPos(eLOCATOR eLocator, float &x, float &y, float &fHeight,DWORD dwFrame = -1);		// ��ȡ��λ������

	// ----------����ָ��shape������װ����Ϣ08.8.28-------------
	virtual void CopyAllEquip(CShape *pShape)	{};
	// ����ģ����Ϣ
	void CopyModelInfo(CShape * pShape);

private:
	static long lEffectID;						// ��ЧID������ÿ���һ��ID���ü����ۼ�1��

//////////////////////////////////////////////////////////////////////////
//	����������2���е�һ�ף�
//////////////////////////////////////////////////////////////////////////
public:
	void	SetAttackAction(char c)		{m_cAttackAction=c;}
	char	GetAttackAction()			{return m_cAttackAction;}
protected:
	char	m_cAttackAction;		// ��ǰ�Ĺ�������

	DWORD   m_iActionCount;				//��ǰ�������ŵĴ�������
};

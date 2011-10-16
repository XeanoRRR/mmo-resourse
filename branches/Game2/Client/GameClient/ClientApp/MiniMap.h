//---------------------------------------------------------------------------------
// ����     : ����
// ����ʱ�� : 2004��3��29�� 10:21:59
// ����ʱ�� : 2004��5��14�� 10:21:59
// ժҪ     : 
//            1. ��ʾС��ͼ���������š�
//            2. ���ݷ��������������ݣ���С��ͼ����ʾ��ǡ�
//            3. �����Ϊ���ģ���ʾ��С��ͼ������ƶ���
//---------------------------------------------------------------------------------
#pragma once

// С��ͼͼ�����ͣ���07.12.14�����޸ģ�
// ��С��ͼ����ʾ����ʾͼ�����ͣ�������ʹ�С����MT_OTHER��
// ��ʾ��Ҫ����Ӧ���ļ����¼��ض�ӦID��ͼ��
// ��08.10.18�����޸ģ����¶���������ͣ�

enum MODEL_TYPE
{
	MT_PLAYER = 0,			// ����Լ�
	MT_MONSTER,				// ����
	MT_LEAGNPC,				// ����NPC
	MT_UNLEAGNPC,			// ������NPC
	MT_PARTNER,				// ����
	MT_LEAGPLAYER,			// �������
	MT_UNLEAGPLAYER,		// ���������
	MT_RED,					// �������
	MT_PIRATES,				// �������
	MT_TASK,				// ���ͼ��
	MT_PET,					// ����ͼ��

	MT_OTHER,				// �������ͣ�һ��ָ�����ͼ�꣩
	MT_COLLECTION = 10000,	// �ɼ���
	MT_QUEST = 100000,		// ����
};

#define MAX_NUM 128
#define RADIUS_NUM 64

struct _tagShapeInfo
{
	float x, y;				// ����
	DWORD type;				// ����
	string strName;			// ����
};


class CBitmapX;

class CMiniMap
{
private:
	// ����ͼ��
	CBitmapX	*m_pMiniMap, 
				*m_PlayerIcon,				// ���ͼ��
				*m_MonsterIcon,				// ����ͼ��
				*m_LeagueNpcIcon,			// ����NPCͼ��
				*m_UnLeagueNpcIcon,			// ������NPCͼ��
				*m_PartnerIcon,				// ����ͼ��
				*m_LeaguePlayerIcon ,		// �����������ͼ��
				*m_UnLeaguePlayerIcon,		// �������������ͼ��
				*m_RedIcon,					// ����ͼ��
				*m_PiratesIcon,				// ����ͼ��
				*m_TaskIcon,				// ���ͼ��
				*m_PetIcon,					// ����ͼ��
				*m_pAlphaMap;				//�����ͼ��Alpha�ɰ�����ͼ

	// С��ͼͼƬ�ĸ߿�
	DWORD m_dwMiniMapWidth, m_dwMiniMapHeight;

	// ��ͼ�ĸ߿�
	DWORD m_dwMapWidth, m_dwMapHeight;

	// ��ɫ
	DWORD m_dwColor;

	// ���ű���
	FLOAT m_fScaleFactor;

	int m_iMainPlayerDir;
	int m_iMainPlayerX,m_iMainPlayerY;

	_tagShapeInfo m_pShapeInfo[MAX_NUM];		// ��Ҫ��С��ͼ�ϱ�ǵ�shape������������Լ���
	vector<_tagShapeInfo>	m_vecPartner;		// ����
	vector<_tagShapeInfo>	m_vecRedName;		// ����
	vector<_tagShapeInfo>	m_vecPirates;		// ����
	vector<_tagShapeInfo>	m_vecOther;			// �������ͣ������ʺ�->����̾��->��������->�ɼ���->�ΣУó�ν��
	vector<_tagShapeInfo>	m_vecNPC;			// NPC�����˷����ˣ�
	vector<_tagShapeInfo>	m_vecUnLeagPlayer;	// ��������ͨ���
	vector<_tagShapeInfo>	m_vecLeagPlayer;	// ������ͨ���

	vector<_tagShapeInfo>	m_vecPartnerForUI;	// ���ѵ���Ϣ�б����������λ����С��ͼ�ϣ�����ڽ���ģ���������ʾ��Ϣ��
	DWORD m_dwNumOfShape;

	// С��ͼ����ĸ߿�
	DWORD m_dwFrameWidth, m_dwFrameHeight;

	int m_iMapMode;		// С��ͼ��ͼƬģʽ��Ĭ��Ϊ1��512*512��2��1024*1024��

	// ���ͼ�ϵ�Ŀ����ʾ
	int m_nDestX;
	int m_nDestY;

	LPDIRECT3DTEXTURE9 m_pRenderTexture128;
	LPDIRECT3DSURFACE9 m_pDepthStencilSurface128;

public:
	CMiniMap(void);
	~CMiniMap(void);

	// ����������
	void Create(DWORD dwFrameWidth, DWORD dwFrameHeight, CONST TCHAR *filename);
	void Destroy();
	void DestroyDirect3DResource();					//����D3D�����Դ
	void ResetDirect3DResource();					//����D3D�����Դ

	// ����С��ͼ���������
	BOOL LoadMiniMap(CONST CHAR *filename, DWORD width, DWORD height);

	// ��ʾ
	void DrawMiniMap(DWORD x, DWORD y);
	//void DrawMiniMapOnWorldMap(DWORD x, DWORD y,CBitmapX *pBitmap=NULL,int index=0);
	void DrawAlphaMap();											//����Alpha�ɰ�
	void DrawFinalMiniMap(DWORD dwX, DWORD dwY);					//�������յ������ͼ����
	bool RenderMiniMapRotation(DWORD x,DWORD y);					//������תЧ���������ͼ
	bool RenderMiniMapNoRotation(DWORD x,DWORD y);					//����û����תЧ���������ͼ
	//void RenderMiniMapRotationOnWorldMap(DWORD x,DWORD y,CBitmapX *pBitmap=NULL,int index=0);
	//void RenderMiniMapNoRotationOnWorldMap(DWORD x,DWORD y,CBitmapX *pBitmap=NULL,int index=0);	

	//-----------------------08.3.12liuke----------------------
	// ����������ڳ�����������ݣ���������ͼͼƬ��Ч��ͼƬ��Ч�����Ŵ�����
	/***********************************************************************/
	/*  zhaohang 2010-11-16 fix λ�ò����޸�Ϊfloat��*/
	/***********************************************************************/
	void RenderOnWorldMap(CBitmapX *pMapBitmap,CBitmapX *pBitmap,int index, bool bShowPlayer=true,float x=0, float y=0);
	//---------------------------------------------------------

	// ����shapelist
	void AddToShapeList(float x, float y, DWORD dwType,const char*szName = NULL);		// ����һ��Shape��Ϣ����ʾ������ȥ
	void AddToPartnerList(float x, float y, const char*szName);							// ����һ��������Ϣ����ʾ�б���ȥ
	_tagShapeInfo* GetPartnerFromPos(DWORD x, DWORD y);									// ���ݴ���Ķ��������ö�����Ϣ�������С��ͼ�ϵĽ������꣩
	void CleanShapeList();
	DWORD GetShapeListCount();

	// ����С��ͼ
	void ScaleMiniMap(FLOAT fScaleFactor);
	FLOAT GetScaleFactor() { return m_fScaleFactor; }

	// ����С��ͼ����ɫ
	void SetMiniMapColor(DWORD color) { m_dwColor = color; }
	DWORD GetMiniMapColor() { return m_dwColor; }

	// (x, y) �Ƿ���rc ��
	BOOL InTheRect(RECT *rc, int x, int y);
	// (x,y) �Ƿ���Բ�С�centerX��centerYΪ���ĵ㡣angel����ƫ�Ʒ��򣬰뾶Ϊ64
	BOOL InTheCircinal(float centerX, float centerY, float nr, float &x, float &y, int &iDir);

	// ��õ�ǰ��ͼ��ģʽ
	int GetMapMode()	{return m_iMapMode;}

	void SetMainPlayer(int x,int y,int dir);
	DWORD GetMapWidth(){return m_dwMapWidth;}
	DWORD GetMapHeight(){return  m_dwMapHeight;}
	DWORD GetPicMapWidth(){return m_dwMiniMapWidth;}
	bool IsRotationAble();		// �Ƿ�֧����ת��ʾ

	// �����ڴ��ͼ����Ҫ��ʵ��ʾ��Ŀ���
	void SetDestPos(int x = -1,int y = -1);
};

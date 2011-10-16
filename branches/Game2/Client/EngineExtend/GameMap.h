#pragma once
#include "../Engine/utility.h"
#include "../Engine/render.h"

//#include "gamefootprint.h"
#include "gamebuildlist.h"

class GameSelectRay;
class GameBuildList;
class CDisplayModel;
class AnimInfo;

//#define MAX_TERRAIN_CACHE_SIZE 1024	* 6
#define MAX_MAP_LIGHT_SOURCE_ARRAY_SIZE 256

#define GRIDMASK_VISIBLE       0x0001 //�ɼ���־
#define GRIDMASK_WEATHEREFFECT 0x0002 //����Ч����־
#define GRIDMASK_ENVEFFECT	   0x0004 //����Ч����־
#define GRIDMASK_ENVCREATURE   0X0008 //���������־

class GameMap
{
	AnimInfo * m_pGlobalAnimInfoArray;
	class ONode
	{
		static int m_iNodeCount;

		render::BoundingBox * m_pBoundingBox;

		ONode    * m_pChildNodeArray;

		int		 * m_pGridIndexArray;//		  
		int		   m_iGridIndexCount;//

		int		 * m_pBuildGridIndexArray;
		int		   m_iBuildGridIndexCount;


	public:
		ONode(void);
		~ONode(void);

		render::BoundingBox * GetBoundingBox() { return m_pBoundingBox;}
		BOOL   IsLeaf();

		void _DEBUG_RenderBoundingBox();
		void _DEBUG_RenderBoundingBox(render::Frustum *pFrustum);

		void TestVisible(GameMap *pMap,render::Frustum *pFrustum);

		void Read(utility::File *pFile);
	};

	ONode * m_pRoot;

public:
	//�ɼ�����
	DWORD * m_pGridVisibleFlagArray;
	DWORD * m_pGridVisibleIndexArray;//��������
	DWORD	m_dwGridVisibleIndexCount;

	DWORD * m_pBuildGridVisibleFlagArray;
	DWORD * m_pBuildGridVisibleIndexArray;//��������
	DWORD	m_dwBuildGridVisibleIndexCount;

	DWORD   m_dwCurFlagValue; //��־

	//����ԭʼ����
	DWORD		  m_dwVertexCount;
	D3DXVECTOR3 * m_pVertexPosArray;
	D3DXVECTOR3 * m_pVertexNormalArray;
	D3DCOLOR    * m_pVertexColArray;

	D3DXVECTOR3 *m_pGridNormalArray;
	D3DMATERIAL9 m_material;

	//����֮��Ľ��
	DWORD		* m_pVertexColorRenderArray; 	

	////�ر���Ⱦ����
	////4 ��
	////15 x 4      
	//struct tagRenderCache
	//{
	//	DWORD dwTextureIndex;

	//	DWORD		  m_dwCacheUsage;
	//	D3DXVECTOR3 * m_pPosBuffer;
	//	D3DXVECTOR2 * m_pUV0Buffer;
	//	DWORD		* m_pColBuffer;
	//};

	//tagRenderCache *m_pRenderCacheArray;//4 x ������

	//��ͼ����
	float m_fVersion;
	DWORD m_dwWidth,m_dwDepth;
	DWORD m_dwGridCount;

	DWORD m_dwTerrainShaderGroupHandle;

private:
	//��ʾ�Ƿ��첽װ��Build
	bool m_bAsyncLoadBuild;
	//�����ݶ�
	DWORD m_dwTargetHideGridIndex;//Ҫ���صĽ�������
	//ƽ��ͷ��
	render::Frustum *m_pFrustum;


	//����
	DWORD       m_dwSunLightAmbientColorforBuild;
	DWORD       m_dwSunLightAmbientColorforMoveShap;
	DWORD       m_dwGridColorcoefficientforMoveShapR;
	DWORD       m_dwGridColorcoefficientforMoveShapG;
	DWORD       m_dwGridColorcoefficientforMoveShapB;
	DWORD       m_dwSunLightDiffuseColor;
	DWORD       m_dwSunLightSpecularColor;
	D3DXVECTOR3 m_vSunLightDirection;
	//�ر�����
	DWORD       m_dwTerrainAmbientColor;
	BOOL        m_bFogEnable;
	DWORD		m_dwFogColor;
	float		m_fFogStart;
	float		m_fFogEnd;
	DWORD       m_dwSkyBoxID;
	float       m_fSkyBoxSize;

	//��Դ
	struct tagLightSource
	{
		D3DXVECTOR3 vPosition;
		float		fRange;
		DWORD	    dwColor;
		DWORD		dwType;//0 add 1 multiply
	};

	tagLightSource * m_pLightSourceArray;
	DWORD m_dwLightSourceArrayUsage;

	//footprint
	//GameFootprint *m_pGameFootprint;

public:
	struct tagBuild:public LoadState
	{

		tagBuild(){pBulid = NULL;}
		~tagBuild(){};
		WORD  wID;
		//����ʵ��
		GameBuildList::Build* pBulid;
		D3DXVECTOR3 vRotation;
		D3DXVECTOR3	vScaling;
		D3DXVECTOR3 vPosOffset;
		//��ʱ��
		AnimInfo *pAnimInfo;
	};
	struct tagGrid
	{
		tagGrid();

		~tagGrid();

		DWORD dwVertexIndex[4];		 //4����������
		BYTE  byUVIndex[4];
		BYTE  byFlag;//������
		WORD wLayerTextureIndex;//0000 0000 0000 0000 4��,����Ϊ0xF(15)ʱ��Ч ˳��Ϊ3 2 1 0
		DWORD dwTargetHideGridIndex; //���ض������ڵĸ�������		
		tagBuild *pBuild;		
		std::vector<DWORD> listgridindex;
		std::vector<D3DCOLOR*> vecpDirColorBuffer;
		BOOL bDirColorCounted;
		//#pragma message (__FILE__ "�˴�Ҫ�޸�")
		//		BOOL bWeatherEffect;//��ʱ
	};



private:
	//����
	tagGrid *m_pGridArray;

	long m_lFootEffectIdArray[8];		//�������ƥ��������ЧID

	//
	GameBuildList *m_pBuildList;
public:
	GameMap(void);
	~GameMap(void);

	//��ȡ
	BOOL Create(const char *szPath,DWORD dwTextureProcision=0);
	//ͬ��װ�ؿɼ��Ľ���
	BOOL SyncLoadVisibleBuild();
	void Destroy();

	//��Ⱦ
	void Render(GameSelectRay *pCameraRay,render::Camera *pCamera,bool bdrawmodel = false,bool isForceEnableLightProcess = false);

	//���������
	void Update(render::Frustum *pFrustum,bool bAsyn=true);
	DWORD   GetSkyBoxID() const { return m_dwSkyBoxID;}
	float   GetSkyBoxSize() const { return m_fSkyBoxSize;}
	DWORD   GetSunLightAmbientColorforBuild() const { return m_dwSunLightAmbientColorforBuild;}
	DWORD   GetSunLightAmbientColorforMoveShap() const { return m_dwSunLightAmbientColorforMoveShap;}
	DWORD   GetGridColorcoefficientforMoveShapR() const { return m_dwGridColorcoefficientforMoveShapR;}
	DWORD   GetGridColorcoefficientforMoveShapG() const { return m_dwGridColorcoefficientforMoveShapG;}
	DWORD   GetGridColorcoefficientforMoveShapB() const { return m_dwGridColorcoefficientforMoveShapB;}
	
	DWORD   GetSunLightDiffuseColor() const { return m_dwSunLightDiffuseColor;}
	DWORD   GetSunLightSpecularColor() const { return m_dwSunLightSpecularColor;}
	DWORD   GetTerrainAmbientColor() const { return m_dwTerrainAmbientColor;}
	const	D3DXVECTOR3 * GetSunLightDirection() const { return &m_vSunLightDirection;}

	void   SetSkyBoxID(DWORD SkyBoxID) { m_dwSkyBoxID = SkyBoxID;}
	void   SetSkyBoxSize(float SkyBoxSize) { m_fSkyBoxSize = SkyBoxSize;}
	void   SetSunLightAmbientColorforBuild(DWORD dwColor) { m_dwSunLightAmbientColorforBuild = dwColor;}
	void   SetSunLightAmbientColorforMoveShap(DWORD dwColor) { m_dwSunLightAmbientColorforMoveShap = dwColor;}
	void   SetGridColorcoefficientforMoveShapR(DWORD dwcoefficientx)  { m_dwGridColorcoefficientforMoveShapR = dwcoefficientx;}
	void   SetGridColorcoefficientforMoveShapG(DWORD dwcoefficienty)  { m_dwGridColorcoefficientforMoveShapG = dwcoefficienty;}
	void   SetGridColorcoefficientforMoveShapB(DWORD dwcoefficientz)  { m_dwGridColorcoefficientforMoveShapB = dwcoefficientz;}
	void   SetSunLightDiffuseColor(DWORD dwColor) { m_dwSunLightDiffuseColor = dwColor;}
	void   SetSunLightSpecularColor(DWORD dwColor) { m_dwSunLightSpecularColor = dwColor;}
	void   SetTerrainAmbientColor(DWORD dwColor) { m_dwTerrainAmbientColor = dwColor;}
	void   SetSunLightDirection(const D3DXVECTOR3 pLightDirection) { m_vSunLightDirection = pLightDirection;}

	//�ɼ�������
	DWORD * GetGridVisibleIndexArray() const { return m_pGridVisibleIndexArray;}
	DWORD   GetGridVisibleIndexCount() const { return m_dwGridVisibleIndexCount;}

	DWORD * GetBuildGridVisibleIndexArray() const { return m_pBuildGridVisibleIndexArray;}
	DWORD   GetBuildGridVisibleIndexCount() const { return m_dwBuildGridVisibleIndexCount;}

	void    SetGridVisibleIndex(DWORD dwGridIndex);
	void    SetBuildGridVisibleIndex(DWORD dwGridIndex);

	tagGrid *  GetGrid(DWORD dwIndex) const { return &m_pGridArray[dwIndex];}
	tagGrid *  GetGrid(int x,int z) const { return &m_pGridArray[x + z * m_dwWidth];}
	D3DXVECTOR3 GetGridNormal(DWORD dwIndex){if(m_pGridNormalArray) return m_pGridNormalArray[dwIndex];else return D3DXVECTOR3(0.0f,0.0f,0.0f);}
	D3DXVECTOR3 GetGridNormal(int x,int z){if(m_pGridNormalArray) return m_pGridNormalArray[x + z * m_dwWidth];else return D3DXVECTOR3(0.0f,0.0f,0.0f);}

	DWORD	   GetGridColor(int x ,int z) const;
	void       GetTagMapBuffer(int x,int z,DWORD size,D3DXVECTOR3* posbuff,std::list<DWORD>& listrendbulid);
	BOOL    GetFogEnable(){ return  m_bFogEnable;}
	DWORD	GetFogColor() { return m_dwFogColor;}
	float	GetFogStart() { return m_fFogStart;}
	float	GetFogEnd()   { return m_fFogEnd;}

	void SetRoundGridProIndex(DWORD gridindex,int size);
	//GameFootprint * GetFootprint() { return m_pGameFootprint;}

	void   RestVisibleFlag();

	DWORD  GetWidth() const { return m_dwWidth;}
	DWORD  GetDepth() const { return m_dwDepth;}
	float  CalculateHeight(float x , float z) const ;
	D3DXVECTOR3 LineIntersectLine(float L1p1x, float L1p1y, 
		float L1p2x, float L1p2y,
		float L2p1x, float L2p1y, 
		float L2p2x, float L2p2y);

	BOOL   HitTestGrid(const D3DXVECTOR3 *pLine,D3DXVECTOR3 *pIntersection,D3DXVECTOR3 *pN);
	DWORD		 CalculateHoverGrid(GameSelectRay * pSelRay,D3DXVECTOR3 &point);
	void   _DEBUG_RenderGrid(DWORD dwCurSelGridIndex);
	void   _DEBUG_RenderBoundingBox();
	void   _DEBUG_RenderBoundingBox(render::Frustum *pFrustum);

	D3DXVECTOR3 * GetTerrainPositionArray() { return m_pVertexPosArray;}
	tagGrid     * GetGridArray()			{ return m_pGridArray; } 

	long GetFootEffectId(unsigned short usType);			//�õ�ָ��������Ҫ���ŵ������ЧID

	GameBuildList * GetBuildList(void);

	void AddPointLightSource(const D3DXVECTOR3 * pLightPosition, DWORD dwLightType,float fLightRange,DWORD dwLightColor);
	void CalculateLightSourceLighting(void);
private:
	//������ջ��ʱ���һ���������
	inline void AddPoint(DWORD,DWORD,DWORD,DWORD,tagLightSource *,BYTE);
	inline void SubPoint(DWORD,DWORD,DWORD,DWORD,tagLightSource *,BYTE);
	inline void MulPoint(DWORD,DWORD,DWORD,DWORD,tagLightSource *,BYTE);
public:
	void SetTargetHideGridIndex(DWORD dwGridIndex);


	// ����ĺ��������������ʸ�� 
	D3DXVECTOR3 Vector(D3DXVECTOR3 vPoint1, D3DXVECTOR3 vPoint2) 
	{ 
		D3DXVECTOR3 vVector; 
		vVector.x = vPoint1.x - vPoint2.x; 
		vVector.y = vPoint1.y - vPoint2.y; 
		vVector.z = vPoint1.z - vPoint2.z; 
		return vVector; 
	} 
	// ����ĺ�������ʸ�������� 
	D3DXVECTOR3 DivideVectorByScaler(D3DXVECTOR3 vVector1, float Scaler) 
	{
		if (Scaler == 0.0f)
			return vVector1;

		D3DXVECTOR3 vResult; 
		vResult.x = vVector1.x / Scaler; 
		vResult.y = vVector1.y / Scaler; 
		vResult.z = vVector1.z / Scaler; 
		return vResult; 
	} 

	// ����ĺ������ڼ������ķ����� 
	void ComputeNormals();

	//GameFootprint::Triangle *triangleArray;
};


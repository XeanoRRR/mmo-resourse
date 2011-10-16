//---------------------------------------------------------------------------------
// ����     : ����
// ����ʱ�� : 2004��1��7�� 11:29:18
// ����ʱ�� : 
// ժҪ     :
//---------------------------------------------------------------------------------

//������2005/1/17 ����
#pragma once

#include "..\..\frostengine\utility.h"
#include "..\..\frostengine\console.h"
#include "..\..\frostengine\render.h"

class EditMap;

// ���������UV�Ķ�Ӧ��
static FLOAT CodeUVTable[32][8] = 
{
	// v0            v1              v2              v3
	// �ұ�16�������
	{0.002f, 0.002f, 0.123f, 0.002f, 0.123f, 0.248f, 0.002f, 0.248f},
	{0.127f, 0.002f, 0.248f, 0.002f, 0.248f, 0.248f, 0.127f, 0.248f},
	{0.252f, 0.002f, 0.373f, 0.002f, 0.373f, 0.248f, 0.252f, 0.248f},
	{0.377f, 0.002f, 0.498f, 0.002f, 0.498f, 0.248f, 0.377f, 0.248f},
														 		 	
	{0.002f, 0.252f, 0.123f, 0.252f, 0.123f, 0.498f, 0.002f, 0.498f},
	{0.127f, 0.252f, 0.248f, 0.252f, 0.248f, 0.498f, 0.127f, 0.498f},
	{0.252f, 0.252f, 0.373f, 0.252f, 0.373f, 0.498f, 0.252f, 0.498f},
	{0.377f, 0.252f, 0.498f, 0.252f, 0.498f, 0.498f, 0.377f, 0.498f},
														 		 	
	{0.002f, 0.502f, 0.123f, 0.502f, 0.123f, 0.748f, 0.002f, 0.748f},
	{0.127f, 0.502f, 0.248f, 0.502f, 0.248f, 0.748f, 0.127f, 0.748f},
	{0.252f, 0.502f, 0.373f, 0.502f, 0.373f, 0.748f, 0.252f, 0.748f},
	{0.377f, 0.502f, 0.498f, 0.502f, 0.498f, 0.748f, 0.377f, 0.748f},
														 		 	
	{0.002f, 0.752f, 0.123f, 0.752f, 0.123f, 0.998f, 0.002f, 0.998f},
	{0.127f, 0.752f, 0.248f, 0.752f, 0.248f, 0.998f, 0.127f, 0.998f},
	{0.252f, 0.752f, 0.373f, 0.752f, 0.373f, 0.998f, 0.252f, 0.998f},
	{0.377f, 0.752f, 0.498f, 0.752f, 0.498f, 0.998f, 0.377f, 0.998f},

	// �����滻�������UV
	//// ���2�����Ϊ1111�������
	//{0.002f, 0.002f, 0.123f, 0.002f, 0.123f, 0.248f, 0.002f, 0.248f},
	//{0.377f, 0.752f, 0.498f, 0.752f, 0.498f, 0.998f, 0.377f, 0.998f},

	// �ұ�16�������
	{0.502f, 0.002f, 0.623f, 0.002f, 0.623f, 0.248f, 0.502f, 0.248f},
	{0.627f, 0.002f, 0.748f, 0.002f, 0.748f, 0.248f, 0.627f, 0.248f},
	{0.752f, 0.002f, 0.873f, 0.002f, 0.873f, 0.248f, 0.752f, 0.248f},
	{0.877f, 0.002f, 0.998f, 0.002f, 0.998f, 0.248f, 0.877f, 0.248f},
								 								 	 	
	{0.502f, 0.252f, 0.623f, 0.252f, 0.623f, 0.498f, 0.502f, 0.498f},
	{0.627f, 0.252f, 0.748f, 0.252f, 0.748f, 0.498f, 0.627f, 0.498f},
	{0.752f, 0.252f, 0.873f, 0.252f, 0.873f, 0.498f, 0.752f, 0.498f},
	{0.877f, 0.252f, 0.998f, 0.252f, 0.998f, 0.498f, 0.877f, 0.498f},
								 								 	 	
	{0.502f, 0.502f, 0.623f, 0.502f, 0.623f, 0.748f, 0.502f, 0.748f},
	{0.627f, 0.502f, 0.748f, 0.502f, 0.748f, 0.748f, 0.627f, 0.748f},
	{0.752f, 0.502f, 0.873f, 0.502f, 0.873f, 0.748f, 0.752f, 0.748f},
	{0.877f, 0.502f, 0.998f, 0.502f, 0.998f, 0.748f, 0.877f, 0.748f},
						 		 								 	 	
	{0.502f, 0.752f, 0.623f, 0.752f, 0.623f, 0.998f, 0.502f, 0.998f},
	{0.627f, 0.752f, 0.748f, 0.752f, 0.748f, 0.998f, 0.627f, 0.998f},
	{0.752f, 0.752f, 0.873f, 0.752f, 0.873f, 0.998f, 0.752f, 0.998f},
	{0.877f, 0.752f, 0.998f, 0.752f, 0.998f, 0.998f, 0.877f, 0.998f},
};

enum LOCATION {BOTTOM_LEFT = 0, BOTTOM_RIGHT, TOP_RIGHT, TOP_LEFT};
static BYTE TexCode[4] = { 2, 1, 4, 8 };


// ��ˢ�뾶
enum BRUSHSIZE 
{
	BS_TINY   = 0,                  
	BS_SMALL  = 1,                  
	BS_MEDIUM = 2,                  
	BS_LARGE  = 4,                  
	BS_HUGE   = 8                   
};

// ��ˢ����
enum BRUSHTYPE
{
	BT_CIRCLE = 0,                  // Բ��
	BT_QUAD                         // ����
};

// �༭����
enum EDITTYPE
{
	ET_ADD,                         // ���
	ET_REPLACE,                     // �滻
	ET_NOISE,                       // �������
	ET_SMOOTH                       // ƽ��
};


#define MAXTERRAINTRIANGLECACHESIZE 131072

class TerrainMesh
{
	//-----------------------------------------------------------------------------
	// ��Ա����
	//����
	DWORD m_dwTextureCount;//��������

	render::Interface::Layer3D::_tagTriangle * m_pTriangleCache;
	DWORD m_dwTriangleCacheUsage;

	struct tagRenderLayer
	{
		render::Interface::Layer3D::_tagTriangle * pRenderHeader;
	};

	struct tagRenderChain
	{
		tagRenderLayer pRenderLayer[16];//���16������
	};

	tagRenderChain m_pRenderChain[4];//4 ��

	

public:
	struct _tagVertex
	{
		DWORD       dwVertexIndex;    // ��������
		D3DXVECTOR3 vPosition;
		D3DXVECTOR3 vNormal;
		D3DCOLOR    cColor;
		D3DCOLOR	cAddColor;
		BYTE        byTexID;          // ÿ�����㶼���Լ�������ID

		DWORD       dwBelongToNum;    // ��ʹ�õĴ���
		DWORD       dwBelongToGrid[4];// ʹ���˸ö����4�����������
	};

	struct _tagGrid
	{
		DWORD       dwGridIndex;      // ��������
		DWORD       dwVertexIndex[4]; // ʹ�õĶ�������
		BYTE        byTexCode[4];     // 4�������������룬�ݴ˲��õ���Ӧ���������ꡣ
		BYTE        byTexIndex[4];    // 4�����㰴����������˳��
		BYTE        uv[4];			  // 4�������UV��������
	};

	struct _tagProcessData            // ��������
	{
		BYTE  bBrushSize;             // ��ˢ��С
		BYTE  bBrushType;             // ��ˢ����
		INT	  nCurrentTextureID;      // ��ǰѡ��������
		DWORD dwSelectedGridIndex;    // ѡ�е���������
		DWORD dwSelectedVertexIndex;  // ѡ��������ѡ�ж��������
	};

private:
	_tagVertex  *m_ptVertexList;        // �����б�	
	DWORD        m_dwNumVertex;			// ������
	
	_tagGrid*    m_ptGrid;              // �����б�
	DWORD        m_dwNumGrid;           // ������

	DWORD        m_dwWidth;             // �ر����
	DWORD        m_dwDepth;

	DWORD		 m_dwShaderGroupHandle;
	DWORD        m_dwTexSetIndex;       // ��¼ʹ�õ�����

	BOOL         m_bIntersectFlag;      // �������Ƿ�λ��������

    BOOL         m_bDrawBrushFlag;      // �Ƿ���ƻ�ˢ

	INT          m_nDefaultBackTextureID;   // Ĭ�ϵĵײ�����ID

	_tagProcessData m_tProcessData; 

	D3DXVECTOR3 *QuadVertex, *GridVertex;

	//-----------------------------------------------------------------------------
	// ��Ա����
public:
	TerrainMesh(void);
	~TerrainMesh(void);

	// ����Shader
	DWORD CreateShader(TCHAR* pszFilename);

	// �����ر�����
	BOOL CreateTerrain(DWORD dwWidth, DWORD dwDepth);

	// ��������
	VOID Destroy();

	// ��Ⱦ����
	void Render(BOOL bDrawNormal,BOOL bShowTexture, render::Camera * pCamera);
	void RenderAll();

	// ��ȾС��ͼ
	BOOL RenderMiniMap(TCHAR *filename);

	// ��Ⱦ��ˢ
	VOID DrawBrush();

	//���ظ���
	_tagGrid * GetGrid(DWORD dwGridIndex);
	_tagGrid * GetGridList() { return m_ptGrid; }

	DWORD GetGridNum() { return m_dwNumGrid; }

	//���ظ�������
	void GetGridCenter(DWORD dwGridIndex,D3DXVECTOR3 *pCenter);
	DWORD GetGridColor(DWORD dwGridIndex);

	//���ض���
	_tagVertex * GetVertex(DWORD dwVertexIndex);
	_tagVertex * GetVertexList() { return  m_ptVertexList;}
	DWORD GetVertexNum() { return m_dwNumVertex; }

	// �жϵ��ʱ�Ƿ����������
	BOOL ProcessLBDown(render::Camera *pCamera, BOOL bChangeTex = TRUE);

	// �趨��ǰѡ��������
	VOID SetSelectedTextureID(INT nTextureID);

	// ȡ��ShaderHandle
	DWORD GetShaderHandle()	{ return m_dwShaderGroupHandle; }

	// ���ñ�ˢ��С
	VOID SetBrushSize(BYTE bBrushSize);
	BYTE GetBrushSize() { return m_tProcessData.bBrushSize; }

	// ���ñ�ˢ����
	VOID SetBrushType(BYTE bBrushType);

	// ����Ĭ�ϵĵײ�����ID
	INT SetDefaultBackTextureID(INT nDefaultBackTextureID = 0);

	// ���ָ�����������
	VOID FillVertex(std::vector<_tagVertex> vOldVertexArray, BOOL bRecalculateTex = TRUE);

	// ���¼������������
	VOID RecalculateGridTexture(DWORD dwGridIndex, DWORD dwVertexIndex, std::vector<_tagVertex> &vOldVertexArray);

	// ��ȡ��������
	VOID GetProcessData(_tagProcessData* pData);

	// ���ô�������
	VOID SetProcessData(_tagProcessData* pData);

	// �����Ƿ���ƻ�ˢ
	BOOL SetDrawBrushFlag(BOOL bFlag = TRUE);

	// �������θ߶�
	VOID ModifyTerrainHeight(std::vector<_tagVertex> &vOldVertexArray, DWORD dwGridIndex, DWORD dwVertexIndex, 
		FLOAT fOffsetY = 0.0f, EDITTYPE et = ET_ADD);

	// ����ѡ������ķ���
	VOID CalculateGridNormal(DWORD dwGridIndex);

	// ����ָ��λ�õĶ���ķ���
	VOID CalculateVertexNormal(DWORD row, DWORD column);

	// load and save file
	BOOL SaveTerrainMesh(FILE* pFile);
	BOOL LoadTerrainMesh(FILE* pFile,bool bnew);

	// ����Ϊ�ͻ�������
	BOOL ImportTerrainMesh(FILE* pFile);

	// access texture set
	DWORD GetTexSetIndex() { return m_dwTexSetIndex; }
	VOID SetTexSetIndex(DWORD dwTexSetIndex) { m_dwTexSetIndex = dwTexSetIndex; }

	// ��������Ƿ�ѡ�ж���
	BOOL CalculateIntersect();

	// ����ͼ��ȡɫ����Ӧ�õ������ϡ�
	void ApplyColorMap(TCHAR *filename);

	// �Ӹ߶�ͼ�õ���������
	void ImportHeightMap(TCHAR *filename);
	// ���������ݵ���Ϊ�߶�ͼ
	void ExportHeightMap(TCHAR *filename);

private:
	// ���ȡ�����UVֵ
	BYTE GetRandomUV();

	// ���������������ת��ΪUV��
	VOID Code2UV(_tagGrid* pGrid);

	// ����ѡ������λ�ô�������������滻
	VOID ProcessGridTexture(_tagGrid* pGrid, INT nLocation);

	// �ж������Ƿ���Ч
	BOOL ValidIndex(INT nIndex, BOOL bGridFlag = TRUE);

	// �ж������Ƿ���Ч
	BOOL ValidLocation(INT nRow, INT nColumn, BOOL bGridFlag = TRUE);

	// ���������������ָ����Χ��
	FLOAT Clamp(FLOAT &num, FLOAT field);

	render::Interface::Layer3D::_tagTriangle * GetFreeTriangleInCache();

	void AddToRenderChain(DWORD dwLayer,DWORD dwTexIndex,render::Interface::Layer3D::_tagTriangle *pTriangle);
	void ClearRenderCache();

public:
	void ExportColorMap(const char * pszFileName);
	inline DWORD MultiplyColor(DWORD dwColorA,DWORD dwColorB);

	DWORD GetTriangleCacheUsage() const { return m_dwTriangleCacheUsage;}

	DWORD GetWidth(){return m_dwWidth;}
	DWORD GetDepth(){return m_dwDepth;}
};

//---------------------------------------------------------------------------------
// ����     : ����
// ����ʱ�� : 2004��4��2�� 15:04:51
// ����ʱ�� : 
// ժҪ     : ���������Ӱ���������
//---------------------------------------------------------------------------------
#pragma once

#include "..\frostengine\render.h"
#include "gamemodel.h"

//ģ���еıߵı�ʾ�ṹ
struct MeshEdge 
{
	//��������
	int v[2];
	//����������
	int face[2];
};

class CShadowVolume
{
private:
	static const int MAX_VERTICES = 2048 * 3 * 6;
	static const int MAX_EDGES    = 2048 * 3;
	static const int MAX_TRIANGLES= 2048;
    D3DXVECTOR3	*m_pVertices;
    DWORD		 m_dwNumVertices;
	MeshEdge	*m_pEdges;
	DWORD		 m_dwNumEdges;
	BOOL		*m_pFaceLights;

	D3DXMATRIX	 m_matWorld;
	BOOL		 m_bAvailable;
	BOOL		 m_bOccluded;
public:
	CShadowVolume(void);
	~CShadowVolume(void);

    void    Reset();
	//��ģ�ͼ���Ӱ����������ģ���Ƿ�յģ����÷��ģ��Ӱ��������㷽����������÷Ƿ��ģ�͵ļ��㷽��
    BOOL    BuildFromMesh(model::Buffer *pBuffer, D3DXVECTOR3 &vLight);
    void	Render();
	void	SetWorldMatrix(D3DXMATRIX *pm);
	DWORD	GetTriangleNum() { return (m_dwNumVertices / 3); };
private:
	BOOL    AnalyseMesh(const model::Buffer *pBuffer);//Ԥ����ģ������
	bool	AddFaceEdge(int face,int v1,int v2);//���ͳ��һ����,�������߲�������������ص㣬����false
	bool	IsOccludedMesh(const model::Buffer *);//����ģ���Ƿ��Ƿ�յ�����������
	void	BuildFromAccludedMesh(const model::Buffer *pBuffer,const D3DXVECTOR3 &vLight);//�ӷ�յ�ģ�͹���Ӱ�����
	void	BuildFromNonAccludedMeshMesh(const model::Buffer *pBuffer,const D3DXVECTOR3 &vLight);//�ӷǷ�յ�ģ�͹���Ӱ�����
};

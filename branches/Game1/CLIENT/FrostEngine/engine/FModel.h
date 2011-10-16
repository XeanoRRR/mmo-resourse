/*

�������
��                                      ��
����    ��� ��     ���    ����  ����
��  ��      ���    ��    ��  ��        ��
��          ��      ��    ��     ��     ��
����      ����       ���    ����    ���

"FROST" 3D GRAPHICS RENDERING ENGINE.

WRITTEN BY JIATAO.
--------------------------------------------------------------------
COPYRIGHT 2002-2003. 

����: ��ɫ����֧��ģ��
����: ����
����: 2003/10/28
�޸�: 2003/10/31
�޸�: 2005/4/14
*/

#pragma once

#include "futility.h"
#include "frender.h"

class model
{
public:

	class AnimVisibility;
	class AnimTexture;
	class AnimJoint;
	class AnimUV;
	class AnimColor;
	class BindPose;
	class Mesh;

	class AnimJoint
	{
	public:
		struct tagFrame
		{
			D3DXVECTOR3 vScale;
			D3DXQUATERNION qRot;
			D3DXVECTOR3 vTranslation;
		};

		struct tagJoint
		{
			char szName[128];
			tagFrame * pFrameArray;
		}; 
	private:
		//�̶�����
		DWORD	  m_dwFrameCount;
		DWORD	  m_dwTimeLength;//������ʱ��
		DWORD	  m_dwJointCount;
		tagJoint *m_pJointArray;

	public:
		AnimJoint();
		~AnimJoint();

		DWORD GetTimeLength(void)	const { return m_dwTimeLength;}
		void  SetTimeLength(DWORD dwTimeLength)   { m_dwTimeLength = dwTimeLength;}

		DWORD GetFrameCount(void)	const {	return m_dwFrameCount;}

		const tagJoint * GetJointArray()	const { return m_pJointArray;}

		BOOL Create(const char *pszFileName);
		
		void Destroy();

		void _DEBUG_Render(const D3DXMATRIX *pWorldMatrix,DWORD dwTimeElapse);
	};


	//��Ⱦ����
	class Buffer
	{
		const Mesh  *m_pAttachMesh;

		//�������ݻ���
		D3DXVECTOR3 *m_pPosBuffer;
		D3DXVECTOR3 *m_pNmlBuffer;
		D3DXVECTOR2 *m_pUV0Buffer;
		D3DCOLOR	*m_pColBufferDiffuse;//��ɫ����
		D3DCOLOR	*m_pColBufferSpecular;//��ɫ����

		DWORD m_dwPNCCount;
		DWORD m_dwUV0Count;
		//DWORD m_dwUV1Count;

	public:
		//��Ⱦ
		void Render(DWORD dwShadeGroupHandle,DWORD dwMode);
		void RenderTransparent(DWORD dwShadeGroupHandle,DWORD dwMode);	// ��Ⱦ͸��ģ��
		void Render(DWORD dwShadeGroupHandle,DWORD dwShaderIndex,DWORD dwMode);
		void DrawShadow(DWORD dwShadeGroupHandle);
		void Draw(DWORD dwShadeGroupHandle,DWORD dwMode = 0);
		void DrawTransparent(DWORD dwShaderGroupHandle);
		void _DEBUG_RenderNormal();

		void operator =( const model::Buffer &buffer);

		//���ʻ���ָ��
		D3DXVECTOR3 * GetPosBuffer() const { return m_pPosBuffer;}
		D3DXVECTOR3 * GetNmlBuffer() const { return m_pNmlBuffer;}
		D3DXVECTOR2 * GetUV0Buffer() const { return m_pUV0Buffer;}
		D3DCOLOR    * GetColBufferDiffuse() const { return m_pColBufferDiffuse;}
		D3DCOLOR    * GetColBufferSpecular() const { return m_pColBufferSpecular;}

		inline DWORD GetPNCCount() const { return m_dwPNCCount;}
		inline DWORD GetUV0Count() const { return m_dwUV0Count;}
		//inline DWORD GetUV1Count() const { return m_dwUV1Count;}

		const Mesh  *GetAttachMesh() const { return m_pAttachMesh;}

		Buffer();
		~Buffer();
		//����������
		void Create(const Mesh *pMesh);
		void Destroy();
	};

	//����ģ��
	class Mesh
	{
		friend Buffer;
		render::BoundingBox *m_pBoundingBox;

		//��������---
		//�����,�����,��ɫ��
		D3DXVECTOR3 * m_pPosBuffer;
		D3DXVECTOR3 * m_pNmlBuffer;
		D3DCOLOR    * m_pColBufferDiffuse;
		DWORD m_dwPNCCount;
		//����0��
		D3DXVECTOR2 * m_pUV0Buffer;
		DWORD m_dwUV0Count;

		DWORD m_dwShaderIndex;

		struct tagGroup
		{
			DWORD dwJointIndex;
			DWORD dwPNCIndexStart;
			DWORD dwPNCIndexCount;
		};

		tagGroup * m_pGroupArray;

		DWORD m_dwGroupCount;

	public:
		struct tagTriangle
		{                       		
			DWORD dwPNCIndex[3];
			DWORD dwUV0Index[3];
			//DWORD dwUV1Index[3];
		};
	private:

		//������
		tagTriangle * m_pTriangleBuffer;
		DWORD m_dwTriangleCount;

		void GetAnimFrameByTime(const model::AnimJoint *pAnimJoint,DWORD dwJointIndex,D3DXVECTOR3 *pTrs,D3DXVECTOR3 *pScl,D3DXQUATERNION *pRot,DWORD dwTimeElapse);
	public:
		//�ӿ�
		D3DXVECTOR3 * GetPosBuffer() const { return m_pPosBuffer;}
		D3DXVECTOR3 * GetNmlBuffer() const { return m_pNmlBuffer;}
		D3DCOLOR    * GetColBufferDiffuse() const { return m_pColBufferDiffuse;}
		D3DXVECTOR2 * GetUV0Buffer() const { return m_pUV0Buffer;}
		tagTriangle * GetTriangleBuffer() const { return m_pTriangleBuffer;}
		DWORD		  GetShaderIndex()    const { return m_dwShaderIndex;}

		render::BoundingBox * GetBoundingBox() const { return m_pBoundingBox;}

		DWORD GetPNCCount() const { return m_dwPNCCount;}
		DWORD GetUV0Count() const { return m_dwUV0Count;}
		//DWORD GetUV1Count() const { return m_dwUV1Count;}
		DWORD GetTriangleCount() const { return m_dwTriangleCount;}

		void CopyPosition(model::Buffer *pBuffer,const D3DXMATRIX *pWorldMatrix);
		void CopyColor(model::Buffer *pBuffer);
		void CopyUV(model::Buffer *pBuffer);
		void CalculateVertexNormal();

		void ProcessAnimJoint(model::Buffer *pBuffer,const D3DXMATRIX *pWorldMatrix,model::BindPose *pBindPose,AnimJoint *pAnimJoint,DWORD dwAnimTimeElapse);
		void ProcessAnimJoint(model::Buffer *pBuffer,const D3DXMATRIX *pWorldMatrix,model::BindPose *pBindPose,AnimJoint *pAnimJoint,DWORD dwAnimTimeElapse,D3DXMATRIX& matrixworld,D3DXMATRIX& matrixmodel);
		void ProcessAnimJointInterpolation(model::Buffer *pBuffer,const D3DXMATRIX *pWorldMatrix,model::BindPose *pBindPose,AnimJoint *pAnimJointA,DWORD dwTimeElapseA,AnimJoint *pAnimJointB,DWORD dwTimeElapseB,float fRatio);
		void ProcessAnimJointInterpolation(model::Buffer *pBuffer,const D3DXMATRIX *pWorldMatrix,model::BindPose *pBindPose,AnimJoint *pAnimJointA,DWORD dwTimeElapseA,AnimJoint *pAnimJointB,DWORD dwTimeElapseB,float fRatio,D3DXMATRIX& matrixworld,D3DXMATRIX& matrixmodel);

		void ProcessAnimTexture(model::Buffer *pBuffer,const AnimTexture *pAnimTexture,DWORD dwTimeElapse,DWORD dwTimeCircle);
		void ProcessAnimVisibility(model::Buffer *pBuffer,const AnimVisibility *pAnimVisibility,DWORD dwTimeElapse,DWORD dwTimeCircle);
		void ProcessAnimColor(model::Buffer *pBuffer,const AnimColor *pAnimColor,DWORD dwTimeElapse,DWORD dwTimeCircle);
		void ProcessAnimUV(model::Buffer *pBuffer,const AnimUV *pAnimUV,DWORD dwTimeElapse,DWORD dwTimeCircle);
		void ProcessEnvironmentMaps(model::Buffer *pBuffer,const D3DXMATRIX *pViewMatrix);

		BOOL Create(const char *pszFileName);
		void Destroy();        		

		void _DEBUG_Render();

		Mesh();
		~Mesh();
	};

	class BindPose
	{
		DWORD	    m_dwJointCount;
		D3DXMATRIX *m_pBindPoseArray;

	public:
		BindPose();
		~BindPose();

		BOOL Create(const char *pszFileName);
		void Destroy();

		const D3DXMATRIX *GetBindPoseArray() { return m_pBindPoseArray;}
	};


	class AnimVisibility
	{
	public:

	private:
		float    * m_pFrameArray;
		//�̶�����
		DWORD	  m_dwFrameCount;

	public:
		AnimVisibility();
		~AnimVisibility();

		DWORD GetFrameCount(void)	const {	return m_dwFrameCount;}
		float * GetFrameArray() const { return m_pFrameArray;}

		BOOL Create(const char *pszFileName);
		void Destroy();
	};

	class AnimColor
	{
	public:

	private:
		DWORD     * m_pColorArray;
		DWORD	  m_dwFrameCount;

	public:
		AnimColor();
		~AnimColor();

		DWORD   GetFrameCount(void)	const {	return m_dwFrameCount;}
		DWORD * GetColorArray() const { return m_pColorArray;}
		BOOL Create(const char *pszFileName);
		void Destroy();

	};

	class AnimUV
	{
		float m_fUOffset,m_fVOffset;
		//��ǰ���ݴ�ȡ

	public:
		AnimUV();
		~AnimUV();

		BOOL Create(const char *pszFileName);
		void Destroy();

		float GetU() const { return m_fUOffset;}
		float GetV() const { return m_fVOffset;}
	};
	class AnimTexture
	{
		DWORD m_dwRows,m_dwColumns;

	public:
		AnimTexture();
		~AnimTexture();

		BOOL Create(const char *pszFileName);
		void Destroy();

		DWORD GetRows()		   const { return m_dwRows;}
		DWORD GetColumns()     const { return m_dwColumns;}
	};
};

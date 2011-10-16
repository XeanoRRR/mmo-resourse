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

����: ����ϵͳģ��
����: ����
����: 2004/02/14
����޸�: 2005/01/15
*/
#pragma once

#include <list>
#include <d3d9.h>
#include <d3dx9.h>
#include "fsingleton.h"

class particle
{

public:
	class Manager;

public:
	class Emitter
	{
		enum PARTICLE_TYPE { PARTICLE_QUAD = 0,PARTICLE_BELT};
		friend class Manager;

		static const int MAX_EMITTER_NAME = 64;

		struct tagParticle
		{
			//-----------------------------------------
			//��������
			D3DXVECTOR3 vPosition;			    //��ǰλ��()
			float		fVisibility;		    //��ǰ�ɼ���	
			DWORD       dwColor;	  		    //��ǰ��ɫ
			int			iAnimTextureRectIndex;	//��ǰUV��������ͼ�е�λ��0..n ; n = rows * columns
			DWORD		dwAnimTextureStartTime;	//���������ѭ����ʼʱ��
			float		fScaleWidth;
			float		fScaleHeight;

			//-----------------------------------------
			//����ʱ���ݣ����䣩
			D3DXVECTOR3 vPosition0;
			D3DXVECTOR3 vVelocity0;			//���ٶ�
			DWORD		dwLifeTimeStart;	//����ʱ�俪ʼ(��λ: ����)

			//-----------------------------------------
			//�ڵ�
			tagParticle * pNext;				//�ڵ�
			//��Ⱦ
			tagParticle * pNextRender;			//��Ⱦ��
		};

		struct tagParameter
		{
			char		    szName[MAX_EMITTER_NAME];//����������

			PARTICLE_TYPE   eParticleType;			  //��������

			float		  	fInitWidth;				  //���
			float		  	fInitHeight;			  //�߶�
			float			fInitWidthRange;				
			float			fInitHeightRange;

			float			*pfVisibilityFrameArray;  //����ֵ�б� 0 ... 1.0f
			DWORD			dwVisibilityFrameCount;	  //֡���б� ...

			DWORD			*pdwColorFrameArray;	  //��ɫֵ�б� 0 ... 
			DWORD			dwColorFrameCount;		  //��ɫ���� ...

			DWORD			dwInitColor;			  //������ɫ

			DWORD			dwLifeTimeLength;		  //��ʼ����ʱ����ʼ(����)

			int				iInitT;				      //��ʼ����(����������ʼʱ��0����)����

			D3DXVECTOR3  	vAcceleration;			  //���ٶ�

			float        	fInitVelocity;		      //��ʼ�ٶ�

			float        	fPitchAngleStart;		  //��ʼ�Ƕ�
			float        	fPitchAngleRange;	      //��ֱ��Χ

			DWORD			dwEmitInterval;			  //������
			DWORD			dwEmitNumber;			  //ÿ�η�������

			DWORD			dwAnimTextureRows;		  //��������
			DWORD			dwAnimTextureColumns;	  //��������			 
			DWORD			dwAnimTextureTimeLength;  //������ѭ��ʱ��

			BOOL			bBillboard;				  //billboard��־
			BOOL			bBillboardY;			  //billboardY��־	
			float			fAngleX;				  //��X��ת

			BOOL            bfollowedowner;              //�Ƿ��������
		}m_tParameter;

		DWORD m_dwShaderGroupHandle;				  //��Ⱦ��

		tagParticle * m_pParticleUsed;
		tagParticle * m_pParticleFree;

		//��Ⱦͷ
		tagParticle * m_pParticleRenderHead;

		//����
		tagParticle * NewParticle(void);
		void  Update(void);
		void  Render(void);

		//�м�����

		//�����м�����
		D3DXVECTOR3 m_vPosition;		//��������ǰλ��
		D3DXMATRIX  m_matBillboard;	
		D3DXMATRIX  m_matBillboardY;	

		BOOL	    m_bEmited;			//�Ƿ�������
		//Ŀǰ�ı任����
		D3DXMATRIX m_matNowMatrix;

	public:
		void  CreateFromFile(const char *pszFileName);
		void  Create(const char *pszParameter,const char *pszResourcePath);
		void  Destroy();

		void  SetBillboard(const D3DXMATRIX *pBillboard);
		void  SetBillboardY(const D3DXMATRIX *pBillboardY);
		void  SetPosition(const D3DXVECTOR3 * pPosition);

		float InterpolationVisibility(DWORD dwTimeElapse);
		DWORD InterpolationColor(DWORD dwTimeElapse);
		void  SetNowMatrix(D3DXMATRIX* mat){m_matNowMatrix = *mat;}
		D3DXMATRIX& GetNowMatrix() {return m_matNowMatrix;}
		BOOL  UpdateTime(DWORD dwLastTime,DWORD dwCurTime);		
		void  Emission(void);

		void  Clear(void);
	};                        

	class Manager
	{
		DECLARE_SINGLETON(Manager);

		//Emitter�б�
		std::list<Emitter *> m_lEmitterList;

		DWORD m_dwEmitterCount;

	public:

		BOOL Create();
		void Destroy();

		void Add(Emitter *pEmitter);
		void Remove(Emitter *pEmitter);

		void Update(void);
		void Render(void);
	};
};

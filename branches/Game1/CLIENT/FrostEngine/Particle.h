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
#include "singleton.h"

class particle
{

public:
	class Manager;

public:
	class Emitter:public LoadState
	{
		enum PARTICLE_TYPE { PARTICLE_QUAD = 0,PARTICLE_BELT};
		friend class Manager;

		static const int MAX_EMITTER_NAME = 64;

		struct tagParticle
		{
			tagParticle()
			{
				ZeroMemory(this,sizeof(tagParticle));
			}
			~tagParticle()
			{

			}
			//-----------------------------------------
			//��������
			D3DXVECTOR3 vPosition;			    //��ǰλ��()
			float		fVisibility;		    //��ǰ�ɼ���	
			DWORD       dwColor;	  		    //��ǰ��ɫ
			int			iAnimTextureRectIndex;	//��ǰUV��������ͼ�е�λ��0..n ; n = rows * columns
			DWORD		dwAnimTextureStartTime;	//���������ѭ����ʼʱ��
			float		fScaleWidth;
			float		fScaleHeight;
			float       frotex;
			float       frotey;
			float       frotez;

			//-----------------------------------------
			//����ʱ���ݣ����䣩
			D3DXVECTOR3 vPosition0;
			D3DXVECTOR3 vVelocity0;			//���ٶ�
			DWORD		dwLifeTimeStart;	//����ʱ�俪ʼ(��λ: ����)
			D3DXVECTOR3 vSrcPosition;		//û���κι����仯ʱ��λ��
			D3DXMATRIX  matstartstate;

			//-----------------------------------------
			//�ڵ�
			tagParticle * pNext;				//�ڵ�
			//��Ⱦ
			tagParticle * pNextRender;			//��Ⱦ��
			bool m_bDrawToScteen;
			bool m_bRenderToTarger;
			D3DXMATRIX m_matOwnZoneMatrix;

		};

		struct tagParameter
		{
			tagParameter()
			{
				pfVisibilityFrameArray = NULL;
				pdwColorFrameArray = NULL;

			}
			~tagParameter()
			{

			}
			char		    szName[MAX_EMITTER_NAME];//����������

			PARTICLE_TYPE   eParticleType;			  //��������

			float		  	fInitWidth;				  //���
			float		  	fInitHeight;			  //�߶�
			float			fInitWidthRange;				
			float			fInitHeightRange;
			int m_iroatdir;
			int m_iroatspeed;

			float			*pfVisibilityFrameArray;  //����ֵ�б� 0 ... 1.0f
			DWORD			dwVisibilityFrameCount;	  //֡���б� ...

			DWORD			*pdwColorFrameArray;	  //��ɫֵ�б� 0 ... 
			DWORD			dwColorFrameCount;		  //��ɫ���� ...

			DWORD			dwInitColor;			  //������ɫ

			DWORD			dwLifeTimeLength;		  //��ʼ����ʱ����ʼ(����)

			int				iInitT;				      //��ʼ����(����������ʼʱ��0����)����

			D3DXVECTOR3  	vAcceleration;			  //���ٶ�

			float        	fInitVelocity;		      //��ʼ�ٶ�

			float        	fPitchAngleStartx;		  //��ʼPITCH�Ƕ�
			float        	fPitchAngleStarty;		  //��ʼYAW�Ƕ�
			float        	fPitchAngleRange;	      //��ֱ��Χ

			DWORD			dwEmitInterval;			  //������
			DWORD			dwEmitNumber;			  //ÿ�η�������

			DWORD			dwAnimTextureRows;		  //��������
			DWORD			dwAnimTextureColumns;	  //��������			 
			DWORD			dwAnimTextureTimeLength;  //������ѭ��ʱ��

			BOOL			bBillboard;				  //billboard��־
			BOOL			bBillboardY;			  //billboardY��־	
			float			fAngleX;				  //��X��ת
			float			fAngleY;				  //��Y��ת
			float			fAngleZ;				  //��Z��ת


			float           fSelfRoteSpeedX;		  //��X��ת�ٶ�	
			float           fSelfRoteSpeedY;		  //��Y��ת�ٶ�
			float           fSelfRoteSpeedZ;		  //��Z��ת�ٶ�

			BOOL            bfollowedowner;              //�Ƿ��������
			BOOL            bfollowedEmitter;              //�Ƿ���淢����
			BOOL            btransinownzone;              //�Ƿ�ֻ������ռ�仯
			BOOL            bstaytofirstframe;              //�Ƿ�ֻ������ռ�仯
			std::vector<float> vfsize;				  //��С
		}m_tParameter;

		char m_pszShaderFileName[MAX_PATH];
		DWORD m_dwShaderGroupHandle;				  //��Ⱦ��

		tagParticle * m_pParticleUsed;
		tagParticle * m_pParticleFree;

		//��Ⱦͷ
		tagParticle * m_pParticleRenderHead;

		//����
		tagParticle * NewParticle(void);
		void  Update(float fscal = 1.0f);
		void  Render(float fscal = 1.0f,bool bsendtobuffer = true);


		//�м�����

		//�����м�����
		D3DXVECTOR3 m_vPosition;		//��������ǰλ��
		D3DXVECTOR3 m_vSrcPosition;     //��������������˶���λ��
		D3DXMATRIX  m_matBillboard;	
		D3DXMATRIX  m_matBillboardY;	

		BOOL	    m_bEmited;			//�Ƿ�������
		//Ŀǰ�ı任����
		D3DXMATRIX m_matNowMatrix;
		//�̶�������ռ�任����
		D3DXMATRIX m_matOwnZoneMatrix;
		//�Ƿ��ڽ�������ʾ
		bool m_bDrawToScreen;
		bool m_bInDraw;
		bool m_bRenderToTarger;
		bool m_bReadyToDie;
		bool m_bDied;


	public:
		Emitter();
		~Emitter();
		void  CloneEmitter(Emitter* pEmiiter);
		void  SetDrawToScreen(bool bdrawtoscreen){m_bDrawToScreen = bdrawtoscreen;}
		void  SetRenderToTarget(bool brendertotarget){m_bRenderToTarger = brendertotarget;}
		void  Draw(float fscal,bool bdrawtoscreen = false,bool brendertotarget = false);
		const char*GetShaderFileName()	{return m_pszShaderFileName;}
		DWORD GetShaderGroupHandle()	{return m_dwShaderGroupHandle;}
		tagParameter* GetParameter()	{return &m_tParameter;}
		void  CreateFromFile(const char *pszFileName);
		void  Create(CRFile *pFile);
		void  Create(const char *pszParameter,const char *pszResourcePath);
		void  CreateParam(const char *pszParameter);
		BOOL CreateShaderGroup(CRFile* pFile,DWORD dwTextureProcision=0);

		void  Destroy();

		void  SetBillboard(const D3DXMATRIX *pBillboard);
		void  SetBillboardY(const D3DXMATRIX *pBillboardY);
		void  SetPosition(const D3DXVECTOR3 * pPosition);
		void  SetSrcPosition(const D3DXVECTOR3 * pPosition);
		void  SetReadyToDie(){m_bReadyToDie = true;}
		bool  GetIsDead(){return m_bDied;}

		float InterpolationVisibility(DWORD dwTimeElapse);
		float InterpolationSize(DWORD dwTimeElapse);
		DWORD InterpolationColor(DWORD dwTimeElapse);
		void  SetNowMatrix(D3DXMATRIX* mat){m_matNowMatrix = *mat;}
		D3DXMATRIX& GetNowMatrix() {return m_matNowMatrix;}
		void  SetOwnZoneMatrix(D3DXMATRIX* mat){m_matOwnZoneMatrix = *mat;}
		D3DXMATRIX& GetOwnZoneMatrix() {return m_matOwnZoneMatrix;}
		BOOL  UpdateTime(DWORD dwLastTime,DWORD dwCurTime);		
		void  Emission(void);
		void  Clear(void);
	};                        

	class Manager
	{
		DECLARE_SINGLETON(Manager);

		//Emitter�б�for displaymodel
		std::list<Emitter *> m_lEmitterList;

		//Emitter�б�for gamemodel
		std::list<Emitter *> m_lEmitterListSource;

		DWORD m_dwEmitterCount;

	public:

		BOOL Create();
		void Destroy();

		void Add(Emitter *pEmitter);
		void AddSourcelist(Emitter *pEmitter);
		void Remove(Emitter *pEmitter);

		void Update(float fscal = 1.0f,bool bdrawtoscreen = false,bool brendertotarget = false);
		void Render(float fscal = 1.0f,bool bsendtobuffer = true);
		void Draw(float fscal = 1.0f,bool bdrawtoscreen = false,bool brendertotarget = false);
	};
};

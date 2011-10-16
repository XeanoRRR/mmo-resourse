/******************************����ͶӰϵͳģ��******************************/
/****BUILD BY ��ǫ**/
/****2007-10-26*****/
/******************************����ͶӰϵͳģ��******************************/
#pragma once

#include <list>
#include <d3d9.h>
#include <d3dx9.h>
#include "DisplayModel.h"
class CTextureProjective:public LoadState
{
public:
	CTextureProjective();
	~CTextureProjective();
	//DWORD     GetType(){return m_dwtype;}
	void	  SetPosDirAlphaNowtime(float x,float y,float z,float diry,float alpha,DWORD time);
	void      UpdateMatView();
	void      SetLookatPos(D3DXVECTOR3 *pos);
	void      SetNowDirY(float fdiry){m_fdir = fdiry;}
	void      SetNowAlpha(float falpha){m_falpha = falpha;}
	void      SetNowTime(DWORD* time);
	float     GetPosX(){return m_pvlookatpoint->x;};
	float     GetPosZ(){return m_pvlookatpoint->z;};
	//void      SetName(char* name){strcpy(m_sname,name);}
	//char*      GetName(){return m_sname;}
	//D3DVECTOR GetPos(){return m_pvlookatpoint;}

	int       GetSize(){return m_dwsize;};
	void      SetSize(DWORD dwsize);
	void	  SetAlive(BOOL alive){m_balive = alive;}
	//float	  GetSize(){return m_size;}
	void      SetLifeTime(long time);
	//DWORD     GetLifeTime(){return m_dwlifetime;}
	//DWORD     GetNowTime(){return m_dwnowtime;}
	void      AddRendMesh(D3DXVECTOR3* pos,DWORD size);
	void      AddNewTextureProj(CTextureProjective* t);
	//void      UpdateMapArray();
	DWORD     GetMyTpSize(){return (DWORD)m_listmytp.size();}
	void      SetVisiable(BOOL visable);
	BOOL      IsTypeOf(DWORD type){return (type & m_dwtype);}
	BOOL      IsBillboardY()      {return m_billboardy;}
	BOOL      IsAlive()           {return m_balive;}
	BOOL      IsVisable()		  {return m_bvisable;}
	void      CreateFromFile(CRFile* pfile);
	void      SetStartTime(DWORD time){m_dwstarttime = time;}
	DWORD     GetLiveTime(){return m_ilifetime;}
	DWORD GetShaderGroupHandle()	{return m_dwtexturegroup;}
	BOOL  CreateShaderGroup(CRFile* pFile,DWORD dwTextureProcision=0);
	const char*GetShaderFileName()	{return m_pszShaderFileName;}
	struct POSDIRALPHATIME
	{
		D3DXVECTOR3 pos;
		float fdiry;
		float falpha;
		DWORD time;
	};
	std::list<POSDIRALPHATIME> m_listpostime;
	struct BUILDMESH
	{
		D3DXVECTOR3* pos;
		DWORD num;
	};
	std::list<BUILDMESH>m_listmesh;

	//���ж���������Ⱦ
	//static void RendTextureProjctive(render::Camera* pcamera);
	static std::list<CTextureProjective*>& GetAllCTextureProjectiveList(){return m_listalltp;}
	static CTextureProjective* CreateTextureProjctive();
	static void SetBufferSize(DWORD size);
	static D3DXVECTOR3* GetBuffer(){return m_pposbuffer;}
	static void DestroyTextureProjctive(CTextureProjective* ptp);
	static void SetState();
	static void SetTextureState();
	static void EndState();
	static void CheckReSetRelease();
	static void CheckReSetReBuild();
	static void Destroy();
	//������Ⱦ
	void SetMixedModel();
	void Rend(D3DXVECTOR3* pcameraposition,D3DXMATRIX matworld,D3DXMATRIX matview,D3DXMATRIX matproj);
	//�������ж�����Ⱦ
	//void Display();
protected:
	//**************************��������************************************//
	char m_pszShaderFileName[MAX_PATH];
	//��������
	DWORD m_dwtype;
	//��ʼʱ��
	DWORD m_dwstarttime;
	//����ʱ��(-1Ϊ����)
	DWORD m_ilifetime;
	//Ŀǰʱ��
	DWORD* m_dwnowtime;
	//����������ͼ����
	D3DXVECTOR3  m_vseepoint;
	D3DXVECTOR3* m_pvlookatpoint;
	D3DXVECTOR3  m_vheaddir;
	//��ͼ����
	D3DXMATRIX m_matview;
	//����任����
	D3DXMATRIX m_mattexture;
	//����ͶӰ����
	D3DXMATRIX m_matproj;
	//��������ƽ�ƾ���
	D3DXMATRIX m_mattranscal;
	//����������
	DWORD  m_dwtexturegroup;
	DWORD  m_dwtexturegroupindex;
	//�Ƿ�BILLBOARDY
	BOOL	  m_billboardy;
	//�Ƿ�ALPHA
	BOOL      m_balpha;
	//�Ƿ���Ч
	BOOL      m_balive;
	//�뾶(1��Ϊ��λ)
	DWORD	  m_dwsize;
	//char      m_sname[255];

	//�Ƿ���ʾ
	BOOL      m_bvisable;
	//�������
	std::list<CTextureProjective * > m_listmytp;
	std::list<CTextureProjective * >::iterator m_itmytp;
	//���ж�������
	static std::list<CTextureProjective * > m_listalltp;
	//Ҫ��Ⱦ�Ķ�������BUFFER
	static D3DXVECTOR3* m_pposbuffer;
	static DWORD m_dwbuffersize;
	D3DCOLOR  m_mixcolor;
	//D3DCOLOR* m_pcolor;
	//D3DXVECTOR2* m_puv;
	DWORD m_vectornum;
	DWORD m_usenum;
	WORD m_mixedmodel;
	BOOL m_bfollowedowner;
	float m_fdir;
	float m_falpha;
	//**************************��������*****************************//


	//**************************���Ŷ�������************************************//
	//�ֶ������б�
	std::vector<float> m_vsize;
	//������ʱ��
	DWORD m_dwsizetime;
	//**************************���Ŷ�������*************************//


	//**************************��ת��������************************************//
	//��ת��ʱ��
	DWORD m_dwrotationtime;
	//��ת����(1:˳ʱ��,-1:��ʱ��)
	DWORD m_dwrotadir;
	//**************************��ת��������*************************//


	//**************************��ɫ��������************************************//
	//�ֶα�ɫ�б�
	std::vector<D3DCOLOR> m_vcolor;
	//��ɫ��ʱ��
	DWORD m_dwcolortime;
	//**************************��ɫ��������*************************//


	//**************************�ɼ��ȶ�������************************************//
	//�ֶν����б�
	std::vector<float> m_vvisiblity;
	//������ʱ��
	DWORD m_dwvisibilitytime;
	//**************************�ɼ��ȶ�������*************************//

	//**************************����֡��������************************************//
	/*DWORD m_dwuvtimeperframe;
	DWORD m_dwuvtime;
	DWORD m_dwrow;*/
	//**************************����֡��������*************************//


private:
	DWORD InterpolationColor(DWORD dwTimeElapse);
	float InterpolationSize(DWORD dwTimeElapse);
	float InterpolationVisibility(DWORD dwTimeElapse);
	//D3DXMATRIX  UpdateUVAnim(DWORD dwTimeElapse);
	void SetRotation(DWORD dwTimeElapse);
	void SetDir(float dir);
	void SetDiffuseColor(DWORD color);
	void SetAlpha(float alpha);


	//��Ӱ��Ⱦ����
public:
	struct SHADOWTEXTURE  
	{
		SHADOWTEXTURE()
		{
			pShadowTexture = NULL;
		}
		~SHADOWTEXTURE(){};
		LPDIRECT3DTEXTURE9       pShadowTexture;
		D3DXVECTOR3 pos;
		DWORD size;
		void Release()
		{
			SAFERELEASE(pShadowTexture);
		}
	};
	// ��Ӽ�ʱ��Ӱ
	// ���ܹ���DISPLAYMODEL��ָ���ANIMINFO��ӵ�����ͳһ��Ⱦ,
	// ��Ϊ��2��ָ���������ʱ�ڱ任,ֻ��������Ⱦ������,
	// ÿ�ζ�Ҫ����״̬,�п��ܶ�Ч�ʲ���Ӱ��,����Ӱ��̶Ȳ�ȷ��
	static void BeginRendShadow(D3DXVECTOR3 pos,DWORD size);
	static void AddRendShadow(CDisplayModel* pdmodel,AnimInfo* pAnimInfo);
	static void EndRendShadow();
	static void RendShadow(SHADOWTEXTURE* p,D3DXVECTOR3 dir,D3DXMATRIX matworld,D3DXMATRIX matview,D3DXMATRIX matproj);
	static std::list<SHADOWTEXTURE>& GetShadowTextureList(){return m_listshadow;}
	static UINT GetUsedTextureNum(){return m_ushadowusednum;}
	static HRESULT CreateShadowTexture();
	static void SetLightPos(D3DXVECTOR3 lightpos);
	static void SetShadowState();
	static void ClearNum();
	static void ReSetShadowRelease();
	static void ReSetShadowReBuild();
	static void SetSimpleShadow(bool b = false);
	static D3DXMATRIX GetShadowVPSMatrix(D3DXMATRIX matworld){return matworld * m_matshadowview * m_matshadowproj * m_matshadowscanf;}
	D3DXMATRIX GetTPVPSMatrix(D3DXMATRIX matworld){return matworld * m_matview * m_matproj * m_mattranscal;}
	static LPDIRECT3DSURFACE9		m_pTextureSurf;	
	//static void 
protected:

	static D3DXMATRIX m_matshadowtexture,m_matshadowview,m_matshadowproj,m_matshadowscanf;
	static D3DXVECTOR3 m_LightPos;

	// ����������Ӱ��ͼ�ı���

	// ����������Ӱ��ͼ��ȶԱȵ���Ȼ�����
	static LPDIRECT3DSURFACE9		m_pShadowZ;	
	// ����RTT����Աȵ���Ȼ�����
	static LPDIRECT3DSURFACE9		m_pRTTZ;	
	// ���ڱ���ԭ���ĺ󱸻���������Ȼ�����
	static LPDIRECT3DSURFACE9		m_pOldBackBuffer, m_pOldZBuffer;	
	// ���ڱ���ԭ�����ӿ�
	static D3DVIEWPORT9				m_oldViewPort;		
	// ���ڱ���ԭ��������ת������ͼ��͸�Ӿ���
	static D3DXMATRIX				m_oldMatWorld,m_oldMatView, m_oldMatProj;
	// ������Ӱ���ӿ�
	static D3DVIEWPORT9				m_shadowViewPort;							
	// ��Ⱦ��ʱ��Ӱ����Ҫ������
	static std::list<SHADOWTEXTURE> m_listshadow;
	static SHADOWTEXTURE m_simpleshadow;
	static bool m_bsimpleshadow;
	// ��ǰ֡��Ӱ�����ʹ������
	static UINT m_ushadowusednum;

	static D3DXMATRIX m_mattempview;
	static D3DXMATRIX m_mattempprojective;
	static SHADOWTEXTURE* GetCanUseShadowTexture();
	//��Ӱ��Ⱦ����
public:
	// ״̬
	enum eTYPE
	{
		TYPE_BODY,			
		TYPE_UPBODYLEFT,
		TYPE_UPBODYRIGHT,
		TYPE_HEAD,			
		TYPE_HUGEBODY,
		TYPE_HEADHEADWEARL,
		TYPE_UPBODYHEADWEARL,
		TYPE_UPBODYHEADWEARR,
	};
	enum eALPHATYPE
	{
		ALPHATYPE_MAINPLAYER,
		ALPHATYPE_CIRCLE,
		ALPHATYPE_NONE,
	};

	struct RTTRESOURCE
	{
		RTTRESOURCE()
		{
			used = false;
			bupdated = false;
		}
		void Release()
		{
			SAFERELEASE(pRttTexture);
		}
		LPDIRECT3DTEXTURE9 pRttTexture;
		bool used;
		bool bupdated;

	};
	//�����ڶ���Alphaͼ(����)
	static LPDIRECT3DTEXTURE9 m_pRttAlphaMainPlayer;
	//�����ڶ���Alphaͼ(����)
	static LPDIRECT3DTEXTURE9 m_pRttAlphaCircle;
	//��Ⱦ������
	//�����б�
	static std::vector<RTTRESOURCE> m_vecrtt;
	//�����������ؾ��
	static int CreateRTTTexture();
	//��ʼ��Ⱦ������(type:0ȫ����,1��ͷ��)
	static bool BeginRendToTexture(int& texhandle);
	static bool RendToTexture(CDisplayModel* pmodel,AnimInfo* pani,int type = 0);
	static void EndRendToTexture();
	static void DrawTextureToScreen(int x,int y,int s,int handle,int alpha = 0);
	static void SetRttUsedFalse(int i);
	static void ReSetRTTRelease();
	static void ReSetRTTReBuild();
    static LPDIRECT3DTEXTURE9 GetRTTtexture(int texHandle);
protected:





};



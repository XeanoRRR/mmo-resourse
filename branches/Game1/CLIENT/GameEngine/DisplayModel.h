//=============================================================================
/**
*  file: DisplayModel.h
*
*  Brief:����һ��������Ҫ��ʾģ�͵���Դ
*
*  Authtor:wangqiao
*	
*	Datae:2007-10-25
*/
//=============================================================================
#ifndef __DISPLAY_MODEL__
#define __DISPLAY_MODEL__

#pragma once

#include "GameModel.h"

class CGameModelManager;

class AnimInfo
{ 
public:
	struct tagActionInfo
	{
		friend class CDisplayModel;

	public:
		DWORD dwCurActionStartTime;			//��ʼʱ��
		DWORD dwCurActionLoopCount;			//����ѭ������
		DWORD dwCurAction;					//��ǰ�������
		DWORD dwCurActionElapseTime;		//��ǰ��������ʱ��
		float fCurActionSpeedRate;			//�Զ��嵱ǰ��������
		BOOL  bCurActionLooped;				//�Ƿ�ѭ����־(��ѭ����ֹͣ�����һ֡)

		BOOL  bInterpolation;		    	//�ڲ��־
		bool  bStayToLastFrame;				//ͣ�������һ֡

		DWORD dwInterpolationElapseTime;	//��ʧʱ��
		DWORD dwInterpolationStartTime;		//��ʼʱ��
		DWORD dwInterpolationTimeLength;	

		DWORD dwOldAction;					//��һ������
		DWORD dwOldActionTimeElapse;		//��һ������ֹͣ��ʱ��
		float fOldActionSpeedRate;			//��һ������������
	}m_tActionInfo;

private:
	friend CDisplayModel;

	//��ǰ����Ⱦ����
	D3DXMATRIX m_matWorld;
	float m_fDirY;			
	float m_fAlpha;
	D3DXMATRIX m_matview;
	D3DXMATRIX m_matproj;
	D3DXMATRIX m_matscal;
	D3DXMATRIX m_matfortp;

	D3DXMATRIX m_matBillboard;
	D3DXMATRIX m_matBillboardY;	   

	//��������,��������ʱ��
	DWORD m_dwCurrentTime;		//��ǰʱ��

	//���ӷ�����ʱ��
	DWORD m_dwParticleEmitterLastTimeArray[GAMEMODEL_MAXPARTICLEEMITTER];

	//��β�����������б�
	GameModel::TrailContainer * m_pTrailContainerArray;

	//���������б�
	D3DXVECTOR3  m_vec3JointArrayPos[3];

	//�Ƿ������λ�ñ任
	bool						m_bIsProcessSelfAnim;

	//ȫ�ֶ���ʱ���
	DWORD m_dwtextureprojectivetime,m_dwAnimColorStartTime,m_dwAnimUVStartTime,m_dwAnimVisibilityStartTime,m_dwAnimTextureStartTime;
	DWORD m_dwAnimWaterStartTime;

public:
	tagActionInfo * GetActionInfo() { return &m_tActionInfo;}
	const tagActionInfo * GetActionInfo() const { return &m_tActionInfo;}

	inline const D3DXMATRIX * GetWorldMatrix() const { return &m_matWorld;}
	inline const D3DXMATRIX * GetViewMatrix() const { return &m_matview;}
	inline const D3DXMATRIX * GetProjectedMatrix() const { return &m_matproj;}
	inline const D3DXMATRIX * GetMatrixForTP() const { return &m_matfortp;}
	inline const D3DXMATRIX * GetScalMatrix() const { return &m_matscal;}
	inline const D3DXMATRIX * GetBillboard()   const { return &m_matBillboard;}
	inline const D3DXMATRIX * GetBillboardY()    const { return &m_matBillboardY;}
	inline const float GetDirY() const {return m_fDirY;}
	inline const float GetTextureProjectiveAlpha() const {return m_fAlpha;}

	inline void SetWorldMatrix(const D3DXMATRIX * pWorldMatrix)
	{ 
		m_matWorld = *pWorldMatrix;
		//memcpy(&m_matWorld,pWorldMatrix,sizeof(D3DXMATRIX));
	}
	inline void SetViewMatrix(const D3DXMATRIX * pViewMatrix)
	{ 
		m_matview = *pViewMatrix;
		//memcpy(&m_matview,pViewMatrix,sizeof(D3DXMATRIX));
	}
	inline void SetScalMatrix(const D3DXMATRIX * pScalMatrix)
	{ 
		m_matscal = *pScalMatrix;
		/*if (&m_matscal != pScalMatrix)
		{
		memcpy(&m_matscal,pScalMatrix,sizeof(D3DXMATRIX));
		}*/

	}
	inline void SetMatrixForTP(const D3DXMATRIX * matfortp)
	{ 
		m_matfortp = *matfortp;
		/*if (&m_matscal != pScalMatrix)
		{
		memcpy(&m_matscal,pScalMatrix,sizeof(D3DXMATRIX));
		}*/

	}
	inline void SetProjectedMatrix(const D3DXMATRIX * pProjMatrix)
	{ 
		m_matproj = *pProjMatrix;
		/*if (&m_matscal != pScalMatrix)
		{
		memcpy(&m_matscal,pScalMatrix,sizeof(D3DXMATRIX));
		}*/

	}
	inline void SetBillboard(const D3DXMATRIX * pBillboardMatrix)
	{ 
		m_matBillboard = *pBillboardMatrix;
		//memcpy(&m_matBillboard,pBillboardMatrix,sizeof(D3DXMATRIX));
	}
	inline void SetBillboardY(const D3DXMATRIX * pBillboardMatrixY)
	{ 
		m_matBillboardY = *pBillboardMatrixY;
		//memcpy(&m_matBillboardY,pBillboardMatrixY,sizeof(D3DXMATRIX));
	}
	inline void SetDirY(float fy){m_fDirY = fy;}
	inline void SetTextureProjectiveAlpha(float fa){m_fAlpha = fa;}

	void SetCurrentTime(DWORD dwCurrentTime) { m_dwCurrentTime = dwCurrentTime;}
	DWORD GetCurrentTime() const { return m_dwCurrentTime;}
	DWORD GetTextureProjectiveTime()const {return m_dwtextureprojectivetime;}
	void  SetTextureProjectiveTime(DWORD time){m_dwtextureprojectivetime = time;}
	void SetAnimTime(DWORD dwTime)
	{
		m_dwtextureprojectivetime = 
			m_dwAnimColorStartTime = 
			m_dwAnimTextureStartTime = 
			m_dwAnimUVStartTime = 
			m_dwAnimVisibilityStartTime =
			m_dwAnimWaterStartTime = dwTime;
	}
	AnimInfo();
	~AnimInfo();
	void ClearTrailContainer(void);
	void DeleteTrailContainer(void);
	void SetTrailContainerArray(GameModel::TrailContainer * p)	{ m_pTrailContainerArray = p; }
	DWORD* GetParticleEmitterLastTimeArray()		{ return m_dwParticleEmitterLastTimeArray; }
	void SetupAnimInfo(float fX, float fY, float fH, float fDir, render::Camera * pCamera);
	void SetupAnimInfoRotMatrix(float fX, float fY, float fH, D3DXMATRIX mat3DDir, render::Camera * pCamera);
	void SetCurActionSpeedRate(float fSpeedRate = 1.0f)		{ m_tActionInfo.fCurActionSpeedRate = fSpeedRate; }			//�趨��ǰ��������
	void CaculateJointPos(D3DXVECTOR3 *vecSrcPos, D3DXVECTOR3 *vecTargetPos, DWORD dwjointCount);
	bool IsSelfAnimJoint() const	{return	m_bIsProcessSelfAnim;}
	void EnableProcessSelfAnimJoint(bool b)	{m_bIsProcessSelfAnim = b;}
	const D3DXVECTOR3 *GetSelfJointArrayPos()	const			{return m_vec3JointArrayPos;};
};


class CDisplayModel
{
public:
	CDisplayModel(CGameModelManager* pGameModelManager);
	virtual ~CDisplayModel(void);
private:
	//gamemodel������
	CGameModelManager* m_pGameModelManager;
	//ID
	DWORD m_dwModelID;
	//GameModel ID
	DWORD m_dwGameModelID;
	//�Ƿ���ʾ����Group,��ѡ��ȱʡ��Part��Lvl
	BOOL  m_bAllGroup;
	DWORD m_dwAnimColorCircle;
	DWORD m_dwAnimUVCircle;
	DWORD m_dwAnimVisibilityCircle;
	DWORD m_dwAnimTextureCircle;
public:
	class CDisplayGroup
	{
	private:
		DWORD m_dwPartName;//Part����
		DWORD m_dwPartLevel;//Part�ȼ�		

		//�ȼ���Ϣ
		GameModel::Group::Part::Level *m_pLevel;
	public:
		CDisplayGroup(void);
		~CDisplayGroup(void);

		void SetPartNameLevel(DWORD dwPart,DWORD dwLevel){m_dwPartName=dwPart;m_dwPartLevel=dwLevel;}
		DWORD GetPartName(void)	{return m_dwPartName;}
		DWORD GetPartLevel(void)	{return m_dwPartLevel;}

		void ReleaseRenderMesh();
		void ReleaseLevelInfo();

		GameModel::Group::Part::Level *GetLevelInfo(){return m_pLevel;}
		void SetLevelInfo(GameModel::Group::Part::Level *pLevel);

		//�õ������θ���
		DWORD GetTriangleCount(void);

		/*void SetRegionAmbient(DWORD dwShaderGroupHandle,DWORD dwcolor);
		void SetAmbient(DWORD dwShaderGroupHandle,DWORD dwcolor);*/
		//void SetLightEnable(DWORD dwShaderGroupHandle,bool byselflightenable = true,bool lightenable = true);
		void ForceToEnableLight(void);
		void SetDirLightEnable(bool enable);
		void SetUseNormal(bool busenormal);
		void SetColor(DWORD dwColor);
		void SetColorCompel(DWORD dwColor);			//�ܿ�ģ����ʾ���Ƿ��ܵƹ�Ч��Ӱ�������,ǿ��Ϊģ����ʾ���趨��ɫ
		void MultiplyColor(DWORD dwColor);
		void MultiplyAlpha(float fRatio);

		//��������Ϣ
		void  ProcessAnimJoint(const AnimInfo *pAnimInfo,model::AnimJoint * pOldAnimJoint,
			model::AnimJoint *pCurAnimJoint,
			DWORD dwOldActionTimeElapse,DWORD dwCurActionTimeElapse,
			model::BindPose *pBindPose,float fRatio);
		void ProcessLevelAnimVisibility(DWORD dwShaderHandle,DWORD dwTimeElapse,DWORD dwTimeCircle);
		void ProcessLevelAnimTexture(DWORD dwTimeElapse,DWORD dwTimeCircle);
		void ProcessLevelAnimColor(DWORD dwTimeElapse,DWORD dwTimeCircle);
		void ProcessLevelAnimUV(DWORD dwTimeElapse,DWORD dwTimeCircle);
		void ProcessEnvironmentMaps(const D3DXMATRIX *pViewMatrix,bool bnormalize = false);
		//������Դ
		void  ProcessPointLight(const D3DXVECTOR3 * pLightPosition,
			float fIntensity, DWORD dwColor);
		//�������
		void  ProcessDirectionalLight(const D3DXVECTOR3 *pCameraPosition,
			const D3DXVECTOR3 *pLightDirection,DWORD dwColor);
		//����Buffer����VECBUFFER�е���ɫ���ҽ�BUFFER��ֵ������ȥ
		void CopyBufferToColor(std::vector<D3DCOLOR*>& vecbuffer);
		//����VECBUFFER�е���ɫ��BUFFER��ȥ
		void CopyColorToBuffer(std::vector<D3DCOLOR*>& vecbuffer);

		void  CalculateBillboard(const model::Mesh *pMesh,model::Buffer * pBuffer,
			const D3DXMATRIX *pBillboarMatrix,const D3DXMATRIX* pmatscal = NULL);



		void GetEffectTypeAndMacoDecla(model::Buffer *pBuffer,render::Interface::Layer3D::RendMesh* prendmesh,render::Interface::Layer3D::RendMesh::TAGMESH* ptagmesh,DWORD& dwEffectType,DWORD& dwMacoDecla);
		//��Ⱦģ��
		void Render(DWORD dwShaderGroupHandle);
		void RenderTransparent(DWORD dwShaderGroupHandle);																			//��Ⱦ͸����
		void RenderModelFlash(DWORD dwShaderGroupHandle);

		void RenderByShaderState(DWORD dwShaderGroupHandle, const render::Interface::Layer3D::_tagShaderState * pShaderState);		//ʹ�ö�̬��ɫ����������Ⱦ



		void DrawAlphaTestOnly(DWORD dwShaderGroupHandle);
		void DrawAlphaTestByColorOp(DWORD dwShaderGroupHandle, DWORD dwColorOp, DWORD dwColorArg1, DWORD dwColorArg2);			//alpha����ʹ����ɫ����ֱ�ӻ���ģ��
		void DrawAlphaBlendOnly(DWORD dwShaderGroupHandle);
		void DrawAlphaBlendByColorOp(DWORD dwShaderGroupHandle, DWORD dwColorOp, DWORD dwColorArg1, DWORD dwColorArg2);			//alpha���ʹ����ɫ����ֱ�ӻ���ģ��

		void DrawTransparent(DWORD dwShaderGroupHandle);
		void DrawWithoutState(DWORD dwShaderGroupHandle);
		void DrawWithoutStateAndTexture(DWORD dwShaderGroupHandle);


		//����
		void  _DEBUG_RenderNormal(void);
	};	

	typedef map<DWORD,CDisplayGroup*> mapDisplayGroups;
	typedef map<DWORD,CDisplayGroup*>::iterator itDisplayGroup;

	typedef map<DWORD,GameModel::Locator*> mapLocators;
	typedef map<DWORD,GameModel::Locator*>::iterator itLocator;

	typedef map<DWORD,GameModel::TrailEmitter*> TrailEmitters;
	typedef map<DWORD,GameModel::TrailEmitter*>::iterator itTrailEmitter;

	typedef map<DWORD,BOOL>	TrailEnables;
	typedef map<DWORD,BOOL>::iterator itTrailEnable;

	typedef map<DWORD,GameModel::ParticleEmitter*> ParticleEmitters;
	typedef map<DWORD,GameModel::ParticleEmitter*>::iterator itParticleEmitter;

	typedef map<DWORD,GameModel::TextureProjective*> TextureProjectives;
	typedef map<DWORD,GameModel::TextureProjective*>::iterator itTextureProjectives;

	typedef map<DWORD,BOOL>	ParticleEnables;
	typedef map<DWORD,BOOL>::iterator itParticleEnable;

private:
	//��ɫ����
	DWORD m_dwShaderGroupHandle;

	//��Ҫ��Ⱦ����ӴGroup
	mapDisplayGroups m_DisplayGroups;
	//��λ���б�
	mapLocators m_Locators;
	//��λ��
	model::BindPose * m_pBindPose;
	//��Ҫ��β��
	TrailEmitters m_TrailEmitters;
	TrailEnables m_TrailEnables;
	//������β���Ĳ�����ʾ(1:������ʾ,0:���в���ʾ,-1:����m_TrailEnables����
	short	m_nAllTrailEnable;

	//��Ҫ��ʾ������
	ParticleEmitters m_ParticleEmitters;
	ParticleEnables m_ParticleEnables;

	BOOL m_bRendShadow;
	//ͶӰ����
	TextureProjectives m_TextureProjectives;

	// ˮ��
	Water::CWater * m_pWater;

	//������β���Ĳ�����ʾ(1:������ʾ,0:���в���ʾ,-1:����m_ParticleEnables����
	short	m_nAllParticleEnable;
	//��ģ�͵ķ�Χ��
	render::BoundingBox *m_pPickBox;
	//��ģ�͵�ѡ���
	render::BoundingBox *m_pSizeBox;
	//��ģ�͵ķ�ΧMesh
	GameModel::PickMesh *m_pPickMesh;
	bool m_brendsimplemesh;

private:
	void SetAllParticleEmitter(BOOL bValue);
	void SetAllTextureProjective(BOOL bValue);

	void SetParticleEmitter(DWORD dwName,BOOL bValue);
	void SetTrailEmitter(DWORD dwName,BOOL bValue);

	BOOL IsParticleEnable(DWORD dwName);
	BOOL IsTrailEnable(DWORD dwName);
	//�ͷ���Դ
	void ReleaseAll();

public:
	DWORD GetID()	{return m_dwModelID;}
	void SetID(DWORD dwID) {m_dwModelID = dwID;}

	DWORD GetGameModelID()	{return m_dwGameModelID;}
	void SetGameModelID(DWORD dwID) {m_dwGameModelID = dwID;}

	BOOL IsShowAllGroup() {return m_bAllGroup;}

	void SetShaderGroupHandle(DWORD dwID){m_dwShaderGroupHandle=dwID;}

	void SetAnimCircleTime(DWORD dwColorCircle,DWORD dwUVCircle,
		DWORD dwVisiCircle,DWORD TextureCircle)
	{
		m_dwAnimColorCircle=dwColorCircle;m_dwAnimUVCircle=dwUVCircle;
		m_dwAnimVisibilityCircle=dwVisiCircle;m_dwAnimTextureCircle=TextureCircle;
	}

	//CDisplayModel::AnimInfo* GetAnimInfo()	{return m_pAnimInfo;}
	//DWORD GetCurActionLoopCount()	{return m_pAnimInfo->m_tActionInfo.dwCurActionLoopCount;}

	//�õ�Group->Part�ĵȼ�����
	BOOL IsGroupPartShow(DWORD dwGroup,DWORD dwPart);
	DWORD GetLvlOfGroupPart(DWORD dwGroup,DWORD dwPart);
	BOOL IsGroupPartLevelShow(DWORD dwGroup,DWORD dwPart,DWORD dwLevel);
	//����group��Ϣ
	void ShowGroup(DWORD dwGroup,DWORD dwPart,DWORD dwLevel,bool bAsyn=true);
	//����group��Ϣ
	void ShowAllGroupbyLevel(DWORD dwLevel,bool bAsyn=true);
	//��ʾȫ��Group,��ѡ��ȱʡ��Part�͵ȼ�
	void ShowAllGroup(bool bAsyn=true);
	//����ĳ��Group
	void HideGroup(DWORD dwGroup);
	// ��������Group
	void HideAllGroup();

	mapDisplayGroups& GetGroups()	{return m_DisplayGroups;}

	//����Group->Part->��Ӧ�ȼ���Ϣ
	BOOL SetLeveInfo(DWORD dwGroup,DWORD dwPart,DWORD dwLevel,
		GameModel::Group::Part::Level *pLevel);


	void SetLocatorArray(mapLocators& Locators);
	void SetBindPos(model::BindPose *pBindPose){m_pBindPose=pBindPose;}

	//�õ�group��partѡ�еĵȼ�
	DWORD GetSelectedLvl(DWORD dwGroup,DWORD dwPart);
	//�õ������θ���
	DWORD GetTriangleCount(void);

	//����ID�õ�locator
	GameModel::Locator* GetLocator(DWORD dwLocator);

	void SetParticleEmitters(ParticleEmitters& partiEmitters);
	void SetTrailEmitters(TrailEmitters& trailEmitters);

	void SetTextureProjective(TextureProjectives& ptextureprojective);

	void EnableTextureProjective(BOOL bValue);

	void SetWater(Water::CWater * pWater)		{ m_pWater = pWater; }			//�趨ˮ��
	Water::CWater * GetWater()					{ return m_pWater; }			//�õ�ˮ��

	void EnableParticleEmitter(DWORD dwName,BOOL bValue);
	void EnableTrailEmitter(DWORD dwName,BOOL bValue);
	void SetAllTrailEmitter(BOOL bValue);

	void EnableParticleEmitter(BOOL bValue);
	void EnableTrailEmitter(BOOL bValue);

	void SetRendShadow(BOOL bRendShadow){m_bRendShadow = bRendShadow;}
	BOOL GetRendShadow(){return m_bRendShadow;}

	//ģ����ɫ����
	/*void SetRegionAmbient(DWORD dwcolor);
	void SetAmbient(DWORD dwcolor);*/
	void ForceToEnableLight(void);						//ǿ��ģ���ܹ�������
	void SetDirLightEnable(bool lightenable = true);
	void SetUseNormal(bool busenormal);
	void SetModelColor(DWORD dwColor);
	void SetModelColorCompel(DWORD dwColor);			//�ܿ�ģ���Ƿ��ܵƹ�Ч��Ӱ�������,ǿ��Ϊģ���趨��ɫ
	void MultiplyModelColor(DWORD dwColor);
	void MultiplyModelAlpha(float fRatio);

	//����ģ����ʾ,����ʱ�� -> ���㴦�� -��Ч������ -> ��Ⱦ
	void ProcessAnimLoop(AnimInfo *pAnimInfo);			//��������ģ�Ͷ���ʱ��ѭ��
	void ProcessAnimJoint(AnimInfo *pAnimInfo);			//����ģ�͵Ĺ���������Ϣ
	void ProcessVisibility(AnimInfo *pAnimInfo);
	void ProcessAnimUV(AnimInfo *pAnimInfo);
	void ProcessAnimTexture(AnimInfo *pAnimInfo);
	void ProcessAnimColor(AnimInfo *pAnimInfo);
	void ProcessLocator(AnimInfo *pAnimInfo);
	void ProcessPickMesh(AnimInfo *pAnimInfo);
	void ProcessParticleEmitter(AnimInfo *pAnimInfo,bool bdrawtoscreen = false,bool brendtotarget = false);
	void ProcessTrailEmitter(AnimInfo *pAnimInfo);
	void ProcessEnvironmentMaps(const D3DXMATRIX *pViewMatrix,bool bnormalize = false);
	void ProcessTextureProjective(AnimInfo *pAnimInfo);
	void ProcessWater(AnimInfo * pAnimInfo);												//����ˮ����Ч
	void SetRendSimpleMesh(bool brendsimesh){m_brendsimplemesh = brendsimesh;}

	//���ռ���
	void ProcessDirectionalLight(const D3DXVECTOR3 *pCameraPosition,
		const D3DXVECTOR3 *pLightDirectiion,DWORD dwColor);
	void CopyBufferToColor(std::vector<D3DCOLOR*>& vecbuffer);
	void CopyColorToBuffer(std::vector<D3DCOLOR*>& vecbuffer);
	void ProcessPointLight(const D3DXVECTOR3 *pLightPosition,
		float fIntensity,DWORD dwColor);
	//��Ⱦģ��
	void RenderModel();
	void RenderModelTransparent();					//��Ⱦ͸��ģ��
	void RenderModelFlash(DWORD dwFlashColor);		//��Ⱦ����ģ��



	void DrawModel(void);							//ֱ�ӻ�ģ��
	void DrawModel(DWORD Group);					//ֱ�ӻ�ģ��ĳһ����
	void DrawModelWithoutState();					//ֱ�ӻ�ģ��,�Լ�����״̬
	void DrawModelWithoutStateAndTexture();			//ֱ�ӻ�ģ��,״̬�����Լ��趨
	void DrawModelByColorOp(DWORD dwColorOp, DWORD dwColorArg1, DWORD dwColorArg2);			//ʹ����ɫ����ֱ�ӻ���ģ��

	//��Ⱦ��Icon
	void DisplayIcon(long lX,long lY,float fScale,float fRotation,AnimInfo *pAnimInfo);
	void DrawModelByAniminfo(AnimInfo *pAnimInfo);

	//����
	void Dump(void);
	void _DEBUG_RenderNormal();
	void _DEBUG_RenderLocator();
	void _DEBUG_RenderPickBox(const D3DXMATRIX *pWroldMatrix,D3DCOLOR dwColor = 0xA000FF00);
	void _DEBUG_RenderSizeBox(const D3DXMATRIX *pWroldMatrix,D3DCOLOR dwColor = 0xA0FF0000);
	void _DEBUG_RenderAction(AnimInfo *pAnimInfo);

	//��ģ�ʹ���
private:
	struct tagChildModelInfo
	{
		//��λ��
		GameModel::Locator* pLocator;
		set<CDisplayModel*> setChildModels;
	};

	//��Ӧ��Ӧ������Ҫ��ʾ����ģ��
	typedef map<DWORD,tagChildModelInfo>	ChildDisplayModels;
	typedef map<DWORD,tagChildModelInfo>::iterator itChildModel;
	ChildDisplayModels m_ChildDisplayModels;

public:
	void AddChildModel(DWORD dwLocatorID,CDisplayModel* pDisplayModel);
	void RemoveChildeModel(DWORD dwLocatorID,CDisplayModel* pDisplayModel);


	/////////////////////////////////////////////////////////////////
	//�߼���Ӧ�ýӿ�
private:
	void ProcessLocator(AnimInfo *pAnimInfo,GameModel::Locator* pLocator);
	void ProcessLocatorByFrame(AnimInfo *pAnimInfo,GameModel::Locator* pLocator,DWORD dwFrame);

	//Group��װ��״̬
	eLoadState m_eGroupLoadState;
	//���ӵ�װ��״̬
	eLoadState m_eParticleLoadState;

	//���ģ���ļ���Shader�Ƿ��ȡ���
	eLoadState CheckBaseModelLoadState();
public:
	//�õ�װ�����(��Ҫ��Group)
	eLoadState GetLoadState();
	//�������Group��װ�����
	void CheckGroupLoadState();
	//������ӵ�װ�����
	void CheckParticleLoadState();
	//����
	void Destroy(void);
	//�õ���λ��λ��
	BOOL GetLocatorPos(AnimInfo *pAnimInfo,DWORD dwLocator,
		float &fX, float &fY, float &fH,DWORD dwFrame = -1);
	//�õ�ʰȡ��
	render::BoundingBox *GetPickBox() { return m_pPickBox;}
	void SetPickBox(render::BoundingBox *pBox) {m_pPickBox=pBox;}
	render::BoundingBox *GetSizeBox() { return m_pSizeBox;}
	void SetSizeBox(render::BoundingBox *pBox){m_pSizeBox=pBox;}
	//�õ�ʰȡMESH
	GameModel::PickMesh *GetPickMesh() { return m_pPickMesh;}
	void SetPickMesh(GameModel::PickMesh *pmesh) {m_pPickMesh=pmesh;}
	//��ʾģ������
	void Display(AnimInfo *pAnimInfo);
	//��ʾģ�Ͱ󶨵�����ģ�͵İ󶨵�
	void GetAnimInfobyLocator(AnimInfo *pOwnAnimInfo,CDisplayModel *pParentModel,AnimInfo *pParentAnimInfo,DWORD parentlocatorname);
	bool CheckStatic();
};

#endif

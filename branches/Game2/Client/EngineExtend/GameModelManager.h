//=============================================================================
/**
*  file: GameModelManager.h
*
*  Brief:����һ��ģ�͵�������Դ
*
*  Authtor:wangqiao
*	
*	Datae:2007-10-25
*/
//=============================================================================
#ifndef __GAME_MODEL_MANAGER__
#define __GAME_MODEL_MANAGER__
#pragma once

#include "GameModel.h"

class CDisplayModel;
class CRFile;

const int GModelArraySize =	2000;
const int DModelArraySize =	20000;

const DWORD ErrorModelID = 0xFFFFFFFF;

class CGameModelManager
{

private:
	static CGameModelManager* instance;
public:
	static void InitInstance();
	static CGameModelManager* GetInstance();
	static void ReleaseInstance();
public:
	CGameModelManager(void);
	virtual ~CGameModelManager(void);

	void Initial();
	void Release();

	//DECLARE_SINGLETON(CGameModelManager);

	typedef enum eAsynReadFileType
	{
		FileType_Model=0,
		FileType_ModelShaderGroup,
		FileType_Part,
		FileType_AnimColor,
		FileType_AnimUV,
		FileType_AnimVisibility,
		FileType_AnimTexture,
		FileType_RenderMesh,
		FileType_Texture,
		FileType_Action,
		FileType_BindPose,
		FileType_Locator,
		FileType_PickMesh,
		FileType_ParticleMesh,
		FileType_ParticleEmitter,
		FileType_ParticleShaderGroup,
		FileType_TrailMesh,
		FileType_TrailEmitter,
		FileType_TextureProjective,
		FileType_TextureProjectiveShaderGroup,
		FileType_Water,								//ˮ���ļ�����
		FileType_WaterShaderGroup					//ˮ��ShaderGroup�ļ�����
	}eARFT;
	typedef struct tagAsynReadFileParam
	{
		DWORD dwLoadVersion;
		//�ļ�����
		eARFT FileType;
		char pszFileName[MAX_PATH];
		//ģ��ID
		DWORD dwModelID;
		DWORD dwGroup;
		DWORD dwPart;
		DWORD dwLevel;

		int nParam;
		tagAsynReadFileParam(DWORD dwVersion,eARFT type,const char* pFileName,
			DWORD ModelID,DWORD Group=0,DWORD Part=0,DWORD Level=0,int Param=0)
			:dwLoadVersion(dwVersion),FileType(type),dwModelID(ModelID)
			,dwGroup(Group),dwPart(Part),dwLevel(Level),nParam(Param)
		{
			strncpy(pszFileName,pFileName,MAX_PATH);
		}
	}tagAsynRFParam;
private:
	
	//ģ�������ݺ���ʾģ������
	struct tagGameModelInfo
	{
		//Ψһ��ʶ��
		DWORD dwID;
		//ģ�͵�·������
		char strFileName[MAX_PATH];
		//��ģ��
		GameModel* pGameModel;
		//��������ʾģ���б�
		set<DWORD> DisplayModels;
		tagGameModelInfo(DWORD ID,const char* pszName,GameModel* pModel)
			:dwID(ID),pGameModel(pModel)
		{
			strncpy(strFileName,pszName,MAX_PATH);
			DisplayModels.clear();
		}
	};

	struct tagDisplayModelInfo
	{
		//Ψһ��ʾ
		DWORD dwID;
		//gamemodel id
		DWORD dwGameModelID;
		//��ʾģ��
		CDisplayModel* pDisplayModel;

		tagDisplayModelInfo(DWORD ID,DWORD dwGMID, CDisplayModel* pModel)
			:dwID(ID),dwGameModelID(dwGMID),pDisplayModel(pModel)
		{
		}
	};

	//GameModelģ������
	tagGameModelInfo* m_GameModels[GModelArraySize];
	//DisplayModelģ������
	tagDisplayModelInfo* m_DisplayModels[DModelArraySize];

	//typedef hash_map<DWORD,tagGameModelInfo*>	HMGameModels;
	//typedef hash_map<DWORD,tagGameModelInfo*>::iterator itGameModel;
	////����ʹ�õ���Ϸģ���б�
	//HMGameModels	m_GameModels;

	typedef hash_map<string,DWORD>	HMStringDword;
	typedef hash_map<string,DWORD>::iterator iHMsd;
	//��ģ�͵��ַ�����ID��ӳ��
	HMStringDword m_GameModelSToD;

	DWORD m_dwTextureProcision;
	///�������ļ���
	char m_pszTextureSharFolder[MAX_PATH];
private:
	//ͬ������Ĭ�ϵ�Model��Դ
	GameModel *LoadModel(DWORD dwModelID,const string& strModelPath,
							const string& strFileName,bool bAsyn=true);
	//װ��bindpos
	BOOL LoadBindPos(GameModel *pGameModel,const string& strModelPath,bool bAsyn=true);
	//װ��locator
	BOOL LoadLoactor(GameModel *pGameModel,const string& strModelPath,bool bAsyn=true);
	//װ��PickMesh
	BOOL LoadPickMesh(GameModel *pGameModel,const string& strModelPath,bool bAsyn=true);
	//װ�����Ӳ���
	BOOL LoadParticleEmitter(GameModel *pGameModel,const string& strModelPath,bool bAsyn=true);
	//װ������ͶӰ����
	BOOL LoadTextureProjective(GameModel *pGameModel,const string& strModelPath,bool bAsyn=true);
	//װ��ˮ�����
	BOOL LoadWater(GameModel * pGameModel, const string & strModelPath, bool bAsyn = true);
	//װ��β������
	BOOL LoadTrailEmitter(GameModel *pGameModel,const string& strModelPath,bool bAsyn=true);

	//װ��Part����
	BOOL LoadPart(GameModel::Group::Part* pPart,
							DWORD dwModelID,DWORD dwGroup,DWORD dwPart,
							const string& strModelPath,bool bAsyn=true);
	//װ��Part��ĳ���ȼ�����
	BOOL LoadPartLevel(GameModel::Group::Part::Level *pLevel,
						DWORD dwModelID,DWORD dwGroup,DWORD dwPart,DWORD dwLevel,
						const string& strModelPath,bool bAsyn=true);

	model::AnimColor* LoadAnimColor(DWORD dwModelID,const string& strModelPath,
									const char* pszFileName,bool bAsyn=true);
	model::AnimUV* LoadAnimUV(DWORD dwModelID,const string& strModelPath,
									const char* pszFileName,bool bAsyn=true);
	model::AnimVisibility* LoadAnimVisibility(DWORD dwModelID,const string& strModelPath,
									const char* pszFileName,bool bAsyn=true);
	model::AnimTexture* LoadAnimTexture(DWORD dwModelID,const string& strModelPath,
									const char* pszFileName,bool bAsyn=true);

	BOOL LoadRenderMesh(DWORD dwModelID,DWORD dwGroup,DWORD dwPart,
		GameModel::Group::Part::tagRenderMesh* pRenderMesh,
						const string& strModelPath,bool bAsyn=true);

	BOOL LoadMesh(DWORD dwModelID,GameModel::Group::Part::tagRenderMesh* pRenderMesh,
						CRFile* pFile,bool bAsyn=true);

	BOOL LoadShaderInfo(DWORD dwShaderGroup,DWORD dwShaderIndex,bool bAsyn=true);

	BOOL LoadTexture(render::Interface::tagTextureInfo* pTextureInfo,
					DWORD dwTextureID,bool bAsyn=true);

	BOOL LoadAction(DWORD dwGameModelID,GameModel::Action* pAction,
					const string& strModelPath,bool bAsyn=true);

	//�ͷ�RenderMesh��Դ
	void ReleaseRenderMesh(DWORD dwGameModelID,
						GameModel::Group::Part::tagRenderMesh* pRenderMesh);
	//�ͷ�Shader��Ϣ
	void ReleaseShaderInfo(DWORD dwShaderGroup,DWORD dwShaerIndex);
public:
	const char * GetGameModelInfoFileName(DWORD dwID);
	//CDisplayModel* GetDisplayModel(DWORD dwID);
	GameModel* GetGameModel(DWORD dwID);
	//�����ļ����õ�GameModelID
	DWORD GetGameModelID(const char* pszModeFileName);

	void SetTextureShareFolder(const char* pszFolderName)
	{strncpy(m_pszTextureSharFolder,pszFolderName,MAX_PATH);}
	//����type��graphicid,���һ����ʾģ��ʵ��
	CDisplayModel* CreateDisplayModel(const string& strFileName,bool bAsyn=true);
	//�ͷ�һ����ʾģ��
	void ReleaseDisplayModel(CDisplayModel* pDisplayModel);
	//�ͷ�����GameModel
	void ReleaseAllDisplayModel();
	//�ͷŵ���ģ��
	void ReleaseGameModel(DWORD dwIndex);
	//�ͷ�����ģ��
	void ReleaseAllGameModel();
	//��ʾĳ��DisplayModel��ȫ��Group,��ѡ��ȱʡ��Part�͵ȼ�
	void ShowDisplayModelAllGroup(CDisplayModel* pDisplayModel,bool bAsyn=true);
	//��ʾĳ��DisplayModel��ȫ��Group,��ѡ��ȱʡ��Part�͵ȼ�
	void ShowDisplayModelAllGroupbyLevel(CDisplayModel* pDisplayModel,DWORD dwlevel,bool bAsyn=true);
	//����Group->Part��Lvl��Ϣ
	GameModel::Group::Part::Level *
		AcquireLevelInfo(DWORD dwModelID,DWORD dwGroup,
								 DWORD dwPart,DWORD & dwLevel,bool bAsyn=true);	
	//�ͷ�Level��Դ
	void ReleaseLevelInfo(DWORD dwGameModelID,DWORD dwGroup,DWORD dwPart,
							DWORD dwLevel,GameModel::Group::Part::Level *pLevel);
	
	//���Action��Ϣ
	GameModel::Action* GetActionAnim(DWORD dwModelID,DWORD dwActionID,bool bAsyn=false);
		
	//����DisplayModel��Group
	//void HideGroup(DWORD dwDisplayModelID,DWORD dwGroup);
	//����DisplayModel�Ķ���
	//void SetAction(DWORD dwDisplayModelID,DWORD dwActionID,BOOL bInterpolation=FALSE,bool bAsyn=false);

	void OnModelReadCompleted(tagGameModelInfo* pGameModelInfo,
							tagAsynRFParam* pParam,CRFile* prfFile);
	void OnBindPosReadCompleted(tagGameModelInfo* pGameModelInfo,
							tagAsynRFParam* pParam,CRFile* prfFile);
	void OnLoacatorReadCompleted(tagGameModelInfo* pGameModelInfo,
							tagAsynRFParam* pParam,CRFile* prfFile);
	void OnPickMeshReadCompleted(tagGameModelInfo* pGameModelInfo,
							tagAsynRFParam* pParam,CRFile* prfFile);
	void OnShaderReadCompleted(tagGameModelInfo* pGameModelInfo,
							tagAsynRFParam* pParam,CRFile* prfFile);
	void OnPartReadCompleted(tagGameModelInfo* pGameModelInfo,
							tagAsynRFParam* pParam,CRFile* prfFile);

	void OnAnimColorReadCompleted(tagGameModelInfo* pGameModelInfo,
									tagAsynRFParam* pParam,CRFile* prfFile);
	void OnAnimUVReadCompleted(tagGameModelInfo* pGameModelInfo,
								tagAsynRFParam* pParam,CRFile* prfFile);
	void OnAnimVisiReadCompleted(tagGameModelInfo* pGameModelInfo,
								tagAsynRFParam* pParam,CRFile* prfFile);
	void OnAnimTextureReadCompleted(tagGameModelInfo* pGameModelInfo,
									tagAsynRFParam* pParam,CRFile* prfFile);

	void OnRenderMeshReadCompleted(tagGameModelInfo* pGameModelInfo,
									tagAsynRFParam* pParam,CRFile* prfFile);
	void OnTextureReadCompleted(tagAsynRFParam* pParam,CRFile* prfFile);

	void OnActionReadCompleted(tagGameModelInfo* pGameModelInfo,
									tagAsynRFParam* pParam,CRFile* prfFile);

	void OnParticleMeshReadCompleted(tagGameModelInfo* pGameModelInfo,
									tagAsynRFParam* pParam,CRFile* prfFile);
	void OnParticleEmitterReadCompleted(tagGameModelInfo* pGameModelInfo,
									tagAsynRFParam* pParam,CRFile* prfFile);
	void OnParticleShaderGroupReadCompleted(tagGameModelInfo* pGameModelInfo,
									tagAsynRFParam* pParam,CRFile* prfFile);
	void OnTextureProjectiveReadCompleted(tagGameModelInfo* pGameModelInfo,
		tagAsynRFParam* pParam,CRFile* prfFile);
	void OnTextureProjectiveShaderGroupReadCompleted(tagGameModelInfo* pGameModelInfo,
									tagAsynRFParam* pParam,CRFile* prfFile);
	void OnTrailMeshReadCompleted(tagGameModelInfo* pGameModelInfo,
									tagAsynRFParam* pParam,CRFile* prfFile);
	void OnTrailEmitterReadCompleted(tagGameModelInfo* pGameModelInfo,
									tagAsynRFParam* pParam,CRFile* prfFile);
	void OnWaterReadCompleted(tagGameModelInfo * pGameModelInfo, tagAsynRFParam * pParam, CRFile * prfFile);				//ˮ���ȡ���
	void OnWaterShaderGroupReadCompleted(tagGameModelInfo * pGameModelInfo, tagAsynRFParam * pParam, CRFile * prfFile);		//ˮ��ShaderGroup��ȡ���

	void OnDataReadCompleted(tagAsynRFParam* pParam,CRFile* prfFile);
	//�첽��ȡ�Ļص�����
	static void ReadModelCallBack(CRFile* prfFile,void* pData);

//��Դ�ͷŲ��Ժͽ�
	//��Ҫ�Ƕ�Model��Group��Action��Ϣ���й���
private:
	//�ͷ���Դ���
	DWORD m_dwReleaseIntel;
	//��ǰ����������
	DWORD m_dwModelOperIndex;
	//��ǰ�����������
	DWORD m_dwTextureOperIndex;
	//��һ���ͷ�ʱ��
	DWORD m_dwLastReleaseTime;
	//��Դ�ӳ��ͷ�ʱ��,���ü���Ϊ0ʱ��ʼ��ʱ
	DWORD m_dwDelayReleaseTime;
	//��һ������װ�ص�ʱ��
	DWORD m_dwLastReloadTime;
	//��ǰģ�͵�װ�ذ汾
	DWORD m_dwLoadVersion;
public:
	//�����ͷŲ���
	void SetReleaseStrategy(DWORD dwReleaseIntel,DWORD dwLastReleaseTime,DWORD dwDelayReleaseTime)
	{
		m_dwReleaseIntel = dwReleaseIntel;m_dwLastReleaseTime = dwLastReleaseTime;
		m_dwDelayReleaseTime = dwLastReleaseTime;
	}
	//�ͷ���Դ
	void ReleaseModelResource();
	//����װ��������Դ
	void ReloadAllModel();

	void Run(void);
};

#endif

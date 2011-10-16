#pragma once


class CDisplayModel;
class GameSelectRay;

// ģ�͹�����
class GameBuildList
{
private:
	static GameBuildList* intance;
public:
	static void InitInstance();
	static GameBuildList* GetInstance();
	static void ReleaseInstance();
public:
	GameBuildList(void);
	virtual ~GameBuildList(void);

	// ����ṹ
	class Build:public RCObject,public LoadState
	{
	public:
		struct tagLightSource
		{
			//��Դ����                      
			DWORD		dwColor;
			float		fRadius;
			//��̬����
			BOOL		bDynamic;
			float		fMinIntensity;
			float		fMaxIntensity;
			DWORD		dwCycleTime;	

			float		fHeight;
		};

		struct tagSoundSource
		{
			//��Դ����
			WORD  wID;	//�ļ�����
			float fHeight;
			float fRolloff;
			float fReferenceDistance;
			float fMaxGain;
			float fMinGain;  
		};

		struct tagBlockData
		{										
			BYTE byType;//TYPE NORMAL HIGH
		  	int iXMin;
			int iXMax;
			int iZMin;
			int iZMax;
		};

		struct tagModel
		{
			tagModel();
			~tagModel();
			BOOL  bAutoHide;
			BOOL  bLighted;
			DWORD  dwGlobalTimer;
			CDisplayModel *pGameModel;
			DWORD dwTrianglCount;
			BOOL bprojected;
			DWORD dwprojectedsize;
			
		};
	private:
		//����
		WORD            m_wID;			// ������� 0x0000    0 type   000 id
		char m_pszFilePath[MAX_PATH];

		tagModel	    *m_pModel;		
		tagLightSource  *m_pLightSource;
		tagSoundSource  *m_pSoundSource;	
		tagBlockData    *m_pBlockData;
	

		//�赲վλ���ݣ�δ���㣩[����һ�����������]
	
	public:
		inline WORD   GetID() const { return m_wID;}
		void   SetID(WORD wID) { m_wID = wID;}
		void SetFilePath(const char* pszPath)
		{strncpy(m_pszFilePath,pszPath,MAX_PATH);}

		tagLightSource * GetLightSource() const { return m_pLightSource;}
		tagSoundSource * GetSoundSource() const { return m_pSoundSource;}
		tagBlockData   * GetBlockData()   const   { return m_pBlockData;}
		tagModel	   * GetModel()		  const { return m_pModel;}

		BOOL Create(const char *pszFileName,DWORD dwTextureProcision=0);
		BOOL Create(CRFile* pFile,bool bAsyn=true);
		void Destroy();

		Build();
		~Build();
	};

protected:
	typedef map<WORD,Build*> mapBuild;
	typedef map<WORD,Build*>::iterator itBuild;
	mapBuild m_mapBuild;					// ����ģ���б�
	
public:
	
	void  Initial();
	void  Release();
//��Դ��ȡ��������ͷŽӿ�
private:
	//ͬ������Ĭ�ϵ�Model��Դ
	Build * LoadBuild(WORD wID,bool bAsyn=true);
public:
	//����һ��Build
	Build *AcquireBuild(WORD wID,bool bAsyn=true);
	//�ͷ�һ��Build
	void ReleaseBuild(WORD wID,Build* pbuild);
	void OnReadBuildCompleted(CRFile* prfFile,void* pData);
	//�첽��ȡ�Ļص�����
	static void ReadBuilCallBack(CRFile* prfFile,void* pData);
};

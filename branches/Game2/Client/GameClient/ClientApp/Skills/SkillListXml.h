#pragma once

class CSkillListXml
{
public:
	/// ״̬
	enum eSKILLTYPE
	{
		///ֱ��ʩ��
		SKILLTYPE_PLAY = 1,			
		/// ����ʩ��
		SKILLTYPE_INTONATE,			
		/// ����
		SKILLTYPE_PASSIVE,			
	};

	CSkillListXml(void);
	virtual ~CSkillListXml(void);
	/// ְҵ��������
	struct tagSkillOcc
	{
		/// ְҵ����
		string strOccName;						
		/// ְҵ���
		DWORD	dwOccId;						
	};
	/// ���ܻ�����������
	struct tagLevelValue 
	{
		/// �ؼ���
		string	strKeyWord;						
		/// �ַ�ֵ
		string	strValue;						
		/// ����ID
		DWORD	dwID;							
		/// ֵ
		DWORD	dwValue;						
		tagLevelValue()
		{
			strKeyWord = "";
			strValue = "";
			dwID = 0;
			dwValue = 0;
		}
	};
	/// ���ܵȼ���Ϣ
	struct tagLevel
	{
		/// ���ܵȼ�
		DWORD	dwSkill_Level;					
		/// ���ܵȼ�����
		string	strSkillLevelDesc;
		/// ���ܵȼ�����������������һ���Ļ�����Ϣ�ȣ�
		string	strSkillLevelDescEx;
		/// ���ȼ�����AI
		DWORD	dwSkillLevelAI;					
		/// ���ͼ�����֤�׶�
		vector<tagLevelValue> vecStepSend;		
		/// ���ܿ�ʼ�׶�
		vector<tagLevelValue> vecStepBegin;		
		/// ����ʹ�óɹ��׶�
		vector<tagLevelValue> vecStepRun;	
		/// ���ܽ����׶�
		vector<tagLevelValue> vecStepEnd;	
		/// �ٻ����ܽ׶�
		vector<tagLevelValue> vecStepSummon;	
		/// ���ܳ���״̬��Ч
		//vector<tagLevelValue> vecStepState;		
		tagLevel()
		{
			dwSkill_Level = 0;
			strSkillLevelDesc = "";
			dwSkillLevelAI = 0;
		}
		~tagLevel()
		{
			vecStepBegin.clear();
			vecStepSend.clear();
			vecStepEnd.clear();
			vecStepRun.clear();
			vecStepSummon.clear();
		}
		void UpdateValue(const char* strKey,long value);
	};
	/// ����������Ϣ
	struct tagSkill
	{
		/// ���ܱ��
		DWORD	dwSkillID;	
		/// ��������
		string	strSkillName;	
		/// ��������
		string	strSkillDesc;
		/// ����״̬��һ�㼼�ܡ��⻷�༼�ܣ�
		DWORD	dwSkillState;	
		/// ��������
		DWORD	dwSkillElement;	
		/// ��������(�������ͷ�)
		DWORD	dwSkillType;	
		/// ����ʹ�����ͣ�����ħ����
		DWORD	dwSkillUseType;	
		/// �����ü���ʱ�������λ����ʾ����ЧID���������˸�ֵ����ʾΪ����ķ�Χѡ���ܣ�
		DWORD	dwEffectID;		
		/// �Ƿ�����Ŀ��
		bool	bLockTarget;	
		/// �Ƿ�����������
		bool	bLockSkill;		
		/// �Ƿ�ӵ�ж���
		bool	bHasAct;	
		/// �Ƿ���Զ��������Ķ���ʹ��
		bool	bIsUseToDiedObj;	
		/// �Ƿ��������༼��(�ͷŽ׶ε���������)
		bool	bIsLead;		
		/// ����ְҵ�б�
		vector<tagSkillOcc>		vecUseByOcc;	
		/// ���ܵȼ�����
		vector<tagLevel>		vectorLevels;	
		tagSkill()
		{
			dwSkillID		= 0;
			strSkillName	= "";
			strSkillDesc	= "";
			dwSkillState	= 0;
			dwSkillElement	= 0;
			dwSkillType		= 0;
			dwSkillUseType	= 0;
			dwEffectID		= 0;
			bLockSkill		= false;
			bLockTarget		= false;
			bHasAct			= false;
			bIsUseToDiedObj = false;
			bIsLead			= false;
		}
		~tagSkill()
		{
			vectorLevels.clear();
			vecUseByOcc.clear();
		}
		///��ȡָ���ȼ������ݽṹ
		tagLevel* GettagLevel(uint level);
	};
private:
	/// ���м������ݶ���
	static map<DWORD, tagSkill> m_mapSkillListXml;	
	///ͬ��SkillAttrIniSetup���ݵ�CSkillListXml
	static void SynchroData();
public:
	/// ��ȡ���м��������ļ�,������ٶ�ȡIni���ã��޸Ķ�Ӧ������ֵ
	static BOOL LoadSkillListXml();	
	/// ��ȡ�������������ļ�
	//static BOOL LoadOneSkillXml(const char* filename);			
	static void Release();
	/// ͨ�����(����ID)ȡ������
	static tagSkill* GetProperty(DWORD dwIndex);
	/// ͨ������ȡ������
	static tagSkill* GetPropertyByName(const char* strName);	
	/// �жϴ���ļ��ܣ��Ƿ���ָ��ְҵ�¿���ʹ��
	static BOOL OccuJudgeStudy(DWORD dwID,long lOccupID );		
	
};

///ȫ�ֺ��������¼���ģ������,skillID����ID��skillleve���ܵȼ���vecStr���Թؼ����б�vecValue�ؼ����б��Ӧ������ֵ�б�
void UpdateSkillXmlList(uint skillID,uint skilllevel,std::vector<string>& vecStr,std::vector<long>& vecValue);

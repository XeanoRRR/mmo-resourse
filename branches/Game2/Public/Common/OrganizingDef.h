
#pragma once

#pragma pack(push, 4)

//!				������ֳ���
const	ulong 	MAX_MEMBER_NAME_SIZE	= 21U;


//! class COrganizing
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!				�����������
const	ulong	MAX_LeaveWordNum			= 256;
//!				ÿ�����Ե�����ַ�����
const	ulong	MAX_PerWordCharNum			= 256;
//!				���Ե�����ַ�����
const	ulong	MAX_PronounceCharNum		= 2048;
//!				���ḣ�������Ӿ���ļ��ʱ��
const	ulong	ADD_EXP_SPACE_TIME			= 60 * 1000;

enum eOperator
{
	OP_Delete						=0,										//ɾ��
	OP_Add,																	//���
	OP_Update,																//����
};

//���԰��ÿ�����Խṹ236
struct tagOrgLeaveWord
{
	CGUID							Guid;									//GUID
	CGUID							MemberGuid;								//���Ե����
	char							szName[MAX_MEMBER_NAME_SIZE];			//�����˵�����
	long							Time;									//���Ե�ʱ��
	char							szBody[MAX_PerWordCharNum];				//���Ե�����

	tagOrgLeaveWord(void){memset(this, 0, sizeof(tagOrgLeaveWord));}
};

//���Խṹ2084
struct tagOrgPronounceWord
{
	CGUID							MemberGuid;								//���Ե����
	char							szName[MAX_MEMBER_NAME_SIZE];			//�����˵�����
	long							Time;									//���Ե�ʱ��
	char							szBody[MAX_PronounceCharNum];			//���Ե�����

	tagOrgPronounceWord(void){memset(this, 0, sizeof(tagOrgPronounceWord));}
};


//! class CFaction
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!				�������Ƴ���
const	long	MAXSIZE_FactionName			= 17;		
//!				����������Ա�б�����
const	long	MAX_ApplyPersonNum			= 256;
//!				����������ֽ���(32k)
const	long	MAX_ICON_SIZE				= 1024 * 32;
//!				���ٽ�����
const	long	MAXNUM_JobLvl				= 5;
//!				����ٽף���ʾ�᳤��
const	long	Exceptive_JobLvl			= -1;

//! ���������������
enum eUpgradeConditionType
{
	eUCT_Money						= 1,									//! ��Ǯ
	eUCT_LeaderLevel,														//! �쵼�ߵȼ�
	eUCT_FacLevel,															//! ��������ȼ�
	eUCT_FacRes,															//! ������Դ����
	eUCT_FacExp,															//! ���ᾭ������
	eUCT_Goods,																//! ��Ʒ
	eUCT_BaseNum,															//! ռ��ĳ���
};

//! ��������
enum eUpgradeType
{
	eUT_FactionLevel,														//! ����ȼ�
	eUT_ExpLevel,															//! ���鸣���ȼ�
	eUT_SpecialityLevel,													//! רҵ�ȼ�

	eUT_BattleLevel,														//! ս���ȼ�
	eUT_BussinessLevel,														//! ��ҵ�ȼ�
	eUT_NobleLevel,															//! ����ȼ�
};

//! ��������
struct tagUpgradeCondition
{
	long 							lConditionType;							//! ��������eUpgradeConditionType
	long 							lValue;									//! ����ֵ
	tagUpgradeCondition(void){memset(this, 0, sizeof(tagUpgradeCondition));}
};

//! ���ȼ�������ýṹ
struct tagFacLevelSetup
{
	long							lLevel;									//! �ȼ�
	long							lMaxMemNum;								//! ��Ա�����
	long							lDisbandMemNum;							//! ��ɢ����
	
	vector<tagUpgradeCondition>		vCondition;								//! ����������
};

//! ��ᾭ��ȼ�
struct tagFacExpLevelSetup
{
	long							lLevel;									//! �ȼ�
	float							fBaseExp;								//! ������������

	vector<tagUpgradeCondition>		vCondition;								//! ����������
};

//! ��ᾭ��ȼ�
struct tagFacDepotLevelSetup
{
	long							lLevel;									//! �ȼ�
	long							lCellNum;								//! �ֿ������

	vector<tagUpgradeCondition>		vCondition;								//! ����������
};

//! ͨ�ø��ӵȼ����ã����˵ȼ�������֮��û���������ݣ�
struct tagFacSubLevelSetup
{
	long							lLevel;									//! �ȼ�

	vector<tagUpgradeCondition>		vCondition;								//! ����������
};

//! ���Զ���ȼ���ص�����
struct tagFacSetWithLevel
{
	long							lLevel;									//! �ȼ���С
	long							lExpPerMinute;							//! �õȼ��³�Աÿ���ӻ�õľ���
	tagFacSetWithLevel(void){memset(this, 0, sizeof(tagFacSetWithLevel));}
};

//! ������
typedef	vector<tagUpgradeCondition> VecCondition;


//! ����ȫ������
struct tagGlobalSetup
{
	long							lDemiseTime;							//! �᳤�����ߵ��ʱ�䣬�������Զ�ת�û᳤
	long							lDisbandFactionTime;					//! ��ɢС�ڹ̶������İ����ӳ�ʱ��
	long							lNegativeTime;							//!	��ԴΪ����ʱ��
	long							lBaseBattleStart;						//! �ݵ�ս��ʼʱ��,Сʱ
	long							lBaseBattleLast;						//!	�ݵ�ս����ʱ�䣬Сʱ
	uchar							byClearBaseTime;						//!	��վݵ��ʱ��(����)
	tagGlobalSetup(void){memset(this, 0, sizeof(tagGlobalSetup));}
};

//! ���Ȩ�޵�˳��
enum eFactionPurviewIndex
{
	//! ǰ�������ԱȨ����ͬ�Ĳ���
	eFPI_SetAffiche,														//! ���ù���
	eFPI_Authorize,															//! ��׼�»�Ա
	eFPI_Fire,																//! ������Ա
	eFPI_TakeOut,															//! �����ֿ�
	eFPI_OpenQuest,															//! ������������
	eFPI_Invite,															//! �����»�Ա
	eFPI_EditTitle,															//! ���óƺ�
	eFPI_EditIcon,															//! �ϴ�����
	eFPI_BeachheadWar,														//! �ݵ�ս����
	eFPI_ResWar,															//! ������Դս��

	//! �����Ƕ��е�
	eFPI_Disband,															//! ��ɢ�л�
	eFPI_Demise,															//! �᳤ְλת��
	eFPI_SetMemberPurview,													//! ���û�ԱȨ��
	
	MAXNUM_FactionPurview,													//! ���Ȩ������
};

//!				��ԱȨ������
const	long	MAXNUM_MemberPurview		= eFPI_Disband;

//! ��Ա������Ȩ��,PV: purview
enum eMemberPurview
{
	eMP_Null						= 0,									//! û��Ȩ��

	eMP_SetAffiche					= 1 << 0,								//! ���ù���
	eMP_Authorize					= 1 << 1,								//! ��׼�»�Ա
	eMP_Fire						= 1 << 2,								//! ������Ա
	eMP_TakeOut						= 1 << 3,								//! �ֿ�ȡ��
	eMP_OpenQuest					= 1 << 4,								//! ������������
	eMP_Invite						= 1 << 5,								//! �����»�Ա
	eMP_EditTitle					= 1 << 6,								//! ���óƺ�
	eMP_EditIcon					= 1 << 7,								//! �ϴ�����
	eMP_BeachheadWar				= 1 << 8,								//! �ݵ�ս����
	eMP_ResWar						= 1 << 9,								//! ������Դս��

	//! ��Ա���Ȩ��
	eMP_MemberTiptop				= 0x0FFFFFFF,
	//! ȫ��Ȩ�ޣ�masterר�ã�
	eMP_All							= 0xFFFFFFFF,
};

//! ������Ϣ����
struct tagFacPublicData
{
	CGUID							guid;									//! ���ID
	char							szName[MAXSIZE_FactionName];			//! �������
	long							lCurrSkillID;							//! ��ǰʹ�õļ���ID
	long							lLevel;									//! ���ȼ�

	tagFacPublicData(void){memset(this, 0, sizeof(tagFacPublicData));}
};

//! �������ʱ���ܿ�������Ϣ
struct tagApplyInfo
{
	CGUID							guid;									//! ���ID
	char							szName[MAXSIZE_FactionName];			//! �������
	long							lLevel;									//! ���ȼ�
	long							lMemberNum;								//! ��Ա����
	long							lMaxMemberNum;							//! ����Ա����
	bool							bIsRecruiting;							//! �Ƿ�������ļ
	long							lCurrRes;								//! ��ǰӵ����Դ
	
	tagApplyInfo(void){memset(this, 0, sizeof(tagApplyInfo));}
    void operator = (const tagApplyInfo & info)
    {
        guid = info.guid;
        lLevel=info.lLevel;
        lMemberNum=info.lMemberNum;
        lMaxMemberNum=info.lMaxMemberNum;
        bIsRecruiting=info.bIsRecruiting;
        lCurrRes = info.lCurrRes;
        strcpy_s( szName , info.szName );
    }
};

//! ����ʱ�򿴵��ģ����Ӱ���������ʾ
struct tagApplyInfoList 
{
    tagApplyInfoList(tagApplyInfo& ApplyInfo,const char* Pronounce)
    {
        info = ApplyInfo;
        strcpy_s(szPronounce,Pronounce);
    }

    tagApplyInfo   info;
    char           szPronounce[MAX_PronounceCharNum];
};

//! ������Ϣ����
struct tagFacBaseData
{
	long							lCountryID;								//! ��������ID

	CGUID							MasterGuid;								//! ����ID
	char							szMasterName[MAX_MEMBER_NAME_SIZE];		//! ��������

	bool							bOpenResWar;							//! �Ƿ������Դս���Ŀ���
	long							lCurrRes;								//! ��ǰӵ����Դ
	long							lCurrExp;								//! ��ǰ�Ĺ��ᾭ��

	long							lSubExpLevel;							//! ���Ӿ���ȼ�(���帣��)
	long							lSpecialityLevel;						//! ����ֿ�ȼ�(��Ϊ����רҵ)
	long							lBattleLevel;							//! ����ȼ�(��Ϊ����ս����
	long							lBussinessLevel;						//! ����ȼ�(��Ϊ������ҵ)
	long							lNoblelevel;							//! ���ҵȼ�(��Ϊ�����Թ�)

	long							CreateTime;								//! ����ʱ��
	CGUID							SuperiorGuid;							//! �ϼ���ID
	char							arr_szJobName[MAXNUM_JobLvl][MAX_MEMBER_NAME_SIZE];		//! ���йٽ׵�Ȩ��
	long							arrJobPurview[MAXNUM_JobLvl];			//! ���йٽ׵�Ȩ��
	long							lMemberNum;								//! ��Ա��������������ʹ�ø�ֵ������ͻ��˷���ʱ��ֵ��

	bool							bIsRecruiting;							//! �Ƿ�������ļ
	long							lFacBattle;								//! ս����ֵ
	long							lFacArmyInvest;							//! ����Ͷ����ֵ




	tagFacBaseData(void){memset(this, 0, sizeof(tagFacBaseData));bIsRecruiting = true;}
};


//! ����֯��ÿ����Ա����Ϣ
struct  tagFacMemInfo
{
	CGUID							MemberGuid;								//! ��ԱID
	char							szName[MAX_MEMBER_NAME_SIZE];			//! ��Ա����
	char							szTitle[MAX_MEMBER_NAME_SIZE];			//! �ƺ�����
	long							lLvl;									//! ��Ա�ȼ�
	long							lOccu;									//! ��Աְҵ
	long							lJobLvl;								//! �ó�Ա��ְ��
	long 							LastOnlineTime;							//! ���һ������ʱ��
	long							lContribute;							//! ����ֵ
	bool							bIsOnline;								//! �Ƿ�����
	char							szRegion[MAX_MEMBER_NAME_SIZE];			//! ���ڵ�ͼ
	uchar							cHide;									//! �Ƿ�Ϊ������
	long							lBattle;								//! ս��ֵ
	long							lArmyInvest;							//! ����Ͷ��ֵ
	bool							bJoinBattle;							//! ��ȡ�ݵ�ս��Ʊ��Ȩ��
	tagFacMemInfo(void){memset(this, 0, sizeof(tagFacMemInfo));}
};


//! ������Ա�ṹ
struct tagFacApplyPerson
{
	CGUID							PlayerGuid;
	char							szName[MAX_MEMBER_NAME_SIZE];
	long							lOccu;
	long							lLvl;
	long							lTime;

	tagFacApplyPerson(void){memset(this, 0, sizeof(tagFacApplyPerson));}
};


//! �������ݱ仯��ǣ�����Ҳʹ�ã�
enum eFacDataChangeFlag
{
	eFDCF_Not						= 0,									//! û�иı�

	eFDCF_BaseInfo					= 1 << 0,								//! �������ݸı�
	eFDCF_Member					= 1 << 1,								//! ��Ա�ı�
	eFDCF_Apply						= 1 << 2,								//! ������Ա�ı�
	eFDCF_LeaveWord					= 1 << 3,								//! ���Ըı�

	eFDCF_All						= 0xFFFFFFFF,							//! ȫ���ı�
};

//! �����ɹ�
const long PLAYER_OPT_SUCCEED	= 1;
//! ����ʧ��
const long PLAYER_OPT_FAILED	= 0;

//! �л����������Ϣ
enum eFactionOptErrInfo
{
	//! --------������ʹ�õ�ֵ--------
	eFOEI_NotErr,

	//! ���Ҵ���
	eFOEI_Err_Country,
	//! �����л�
	eFOEI_Err_HadFaction,

	//! �л������ظ����յ����������Լ��������µ����֣��Ự��δ�жϣ�
	eFOEI_Err_ReName,
	//! �����к��������ַ�
	eFOEI_Err_SensitiveWord,
	//! ��ҵȼ�����
	eFOEI_Err_Level,
	//! ��Ǯ����
	eFOEI_Err_Money,
	//! ��ʯ����
	eFOEI_Err_Gem,
	//! ��Ʒ����
	eFOEI_Err_Goods,
	//! ��֯��Դ����
	eFOEI_FacRes_NotEnough,
	//! ��֯���鲻��
	eFOEI_FacExp_NotEnough,
	//! ���û�ȡ��δ���
	eFOEI_Err_SetupLoading,

	//! ���ķ��������󣬴���ʧ��
	eFOEI_CoreServerError,

	//! ���ȼ�����
	eFOEI_Err_Level_Noenough,
	//! ���Ȩ�޲���
	eFOEI_Err_Purview_Noenough,
	//! ���ܲ�����ͬ��Ȩ�޵ĳ�Ա
	eFOEI_Err_SamePurview,
	//! Ŀ�겻����
	eFOEI_Err_AimAbsent,
	//! Ŀ���Ѳ�����
	eFOEI_Err_AimNoBeing,
	//! Ŀ�������л�
	eFOEI_Err_AimHasFac,
	//! �����Ѵﵽ��������
	eFOEI_Err_MemberFull,
	//! Ŀ��ܾ�
	eFOEI_Err_AimRefuse,
	//! �����ظ�������룬����ȡ��֮ǰ��������ڽ��в���
	eFOEI_Err_ReJoin,
	//! ����ʱ���ѵ��������µ��NPC
	eFOEI_Err_SessionLost,

	//! û�м����л�
	eFOEI_Err_NotInFaction,
	//! ԭ��������������ͬ
	eFOEI_Err_SameValue,
	//! Ŀ��ȼ�����
	eFOEI_Err_AimLevelTooLow,

	//! ��������
	eFOEI_Err_HadUnion,
	//! �Է���������
	eFOEI_Err_AimHadUnion,
	//! Ƶ����������
	eFOEI_Err_ContinualInvite,


	//! �������Ѿ�����
	eFOEI_Err_InviterOff,

	//! �Է���������
	eFOET_Err_AimHideName,

	//! ռ�������������
	eFOET_Err_BaseNum,

	//! δ֪���󣬷������
	eFOEI_Err_Unknown,
};


//! class CUnion
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! ��Ա������Ȩ��,PV: purview
enum eUnionMemberPurview
{
	//! û��Ȩ��
	eUMP_Null						= 0,

	//! ���ù���
	eUMP_SetAffiche					= 1 << 0,
	//! ������Ա
	eUMP_Fire						= 1 << 2,
	//! ��������
	eUMP_OpenQuest					= 1 << 4,
	//! ������Ա
	eUMP_Invite						= 1 << 5,
	//! ���óƺ�
	eUMP_EditTitle					= 1 << 6,

	//! ��Ա���Ȩ��
	eUMP_MemberTiptop				= 0x0FFFFFFF,
	//! ȫ��Ȩ�ޣ�masterר�ã�
	eUMP_All						= 0xFFFFFFFF,							
};



//! ���˻�������
struct  tagUnionBaseData
{
	CGUID							guid;									//! ID
	char							szName[MAXSIZE_FactionName];			//! ����
	long							lCountryID;								//! ��������ID
	CGUID							MasterGuid;								//! ����ID��������Player��
	CGUID							MasterFacGuid;							//! �������ڹ���
	char							szMasterName[MAX_MEMBER_NAME_SIZE];		//! ��������	
	long							CreateTime;								//! ����ʱ��

	tagUnionBaseData(void){memset(this, 0, sizeof(tagUnionBaseData));}
};


//! ������ÿ����Ա����Ϣ
struct  tagUnionMemInfo
{
	CGUID							MemberGuid;								//! ��ԱID����Ա��Faction��
	char							szName[MAX_MEMBER_NAME_SIZE];			//! ��Ա����
	long							lJoinDate;								//! ����ʱ��
	long							lPurview;								//! ��ԱȨ��
	

	tagUnionMemInfo(void){memset(this, 0, sizeof(tagUnionMemInfo));}
};

//! ����ȼ���Ӧ�ļ���Ƽ���߼�
struct tagTechInfo
{
	long	_lSubExpLv;     //�����ȼ�
	long	_lSpecialityLv; //רҵ�ȼ�
	long	_lBussinessLv;  //��ҵ�ȼ�
	long	_lBattleLv;     //ս���ȼ�
	long	_lNoblelv;      //����ȼ�
	tagTechInfo(){ZeroMemory(this,sizeof(tagTechInfo));}
};

struct tagBaseConsume
{
	long	_BaseVal; //����
	long	_Critical;//�ٽ�ֵ
};

//һ���ݵ����Ϣ
struct tagBaseInfo
{
	char		_cBaseName[32]; //�ݵ�����
	long		_lTickets;      //��Ʊ��
	long		_lBattleVal;    //ս��ֵ
	CGUID		_FacGuid;       //ռ������GUID
	tagBaseInfo(){ZeroMemory(this,sizeof(tagBaseInfo));}
};
//ս������
enum eBattleType
{
	eFactionWar=0,
	eBaseWar,
	eCityWar,

	eMaxWar,
};

//�жԼ�����Ϣ
struct tagEnemy
{
	char _cName[MAXSIZE_FactionName];//�жԼ�������
	ulong _dwStartTime;              //ս����ʼʱ��
	ulong _dwEndTime;
};

#pragma pack(pop)
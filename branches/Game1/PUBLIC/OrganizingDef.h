#include "Date.h"
#include "GUID.h"

#pragma once

#pragma pack(push, 4)

//!				������ֳ���
const	DWORD 	MAX_MEMBER_NAME_SIZE	= 21U;


//! class COrganizing
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//!				�����������
const	DWORD	MAX_LeaveWordNum			= 256;
//!				ÿ�����Ե�����ַ�����
const	DWORD	MAX_PerWordCharNum			= 256;
//!				���Ե�����ַ�����
const	DWORD	MAX_PronounceCharNum		= 2048;

//!				���ḣ�������Ӿ���ļ��ʱ��
const	DWORD	ADD_EXP_SPACE_TIME			= 60 * 1000;

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
	LONG							Time;									//���Ե�ʱ��
	char							szBody[MAX_PerWordCharNum];				//���Ե�����

	tagOrgLeaveWord(void){memset(this, 0, sizeof(tagOrgLeaveWord));}
};

//���Խṹ2084
struct tagOrgPronounceWord
{
	CGUID							MemberGuid;								//���Ե����
	char							szName[MAX_MEMBER_NAME_SIZE];			//�����˵�����
	LONG							Time;									//���Ե�ʱ��
	char							szBody[MAX_PronounceCharNum];			//���Ե�����

	tagOrgPronounceWord(void){memset(this, 0, sizeof(tagOrgPronounceWord));}
};


//! class CFaction
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!				�������Ƴ���
const	LONG	MAXSIZE_FactionName			= 17;		
		
//!				����������Ա�б�����
const	LONG	MAX_ApplyPersonNum			= 256;
//!				����������ֽ���(32k)
const	LONG	MAX_ICON_SIZE				= 1024 * 32;

//!				���ٽ�����
const	LONG	MAXNUM_JobLvl				= 5;
//!				����ٽף���ʾ�᳤��
const	LONG	Exceptive_JobLvl			= -1;




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
	eUT_BussinessLevel,														//! ��ҵ�ȼ�
	eUT_BattleLevel,														//! ս���ȼ�
	eUT_NobleLevel,															//! ����ȼ�
};

//! ��������
struct tagUpgradeCondition
{
	LONG 							lConditionType;							//! ��������eUpgradeConditionType
	LONG 							lValue;									//! ����ֵ
	tagUpgradeCondition(void){memset(this, 0, sizeof(tagUpgradeCondition));}
};

//! ���ȼ�������ýṹ
struct tagFacLevelSetup
{
	LONG							lLevel;									//! �ȼ�
	LONG							lMaxMemNum;								//! ��Ա�����
	LONG							lDisbandMemNum;							//! ��ɢ����
	
	vector<tagUpgradeCondition>		vCondition;								//! ����������
};

//! ��ᾭ��ȼ�
struct tagFacExpLevelSetup
{
	LONG							lLevel;									//! �ȼ�
	float							fBaseExp;								//! ������������

	vector<tagUpgradeCondition>		vCondition;								//! ����������
};

//! ��ᾭ��ȼ�
struct tagFacDepotLevelSetup
{
	LONG							lLevel;									//! �ȼ�
	LONG							lCellNum;								//! �ֿ������

	vector<tagUpgradeCondition>		vCondition;								//! ����������
};

//! ͨ�ø��ӵȼ����ã����˵ȼ�������֮��û���������ݣ�
struct tagFacSubLevelSetup
{
	LONG							lLevel;									//! �ȼ�

	vector<tagUpgradeCondition>		vCondition;								//! ����������
};

//! ���Զ���ȼ���ص�����
struct tagFacSetWithLevel
{
	LONG							lLevel;									//! �ȼ���С
	LONG							lExpPerMinute;							//! �õȼ��³�Աÿ���ӻ�õľ���
	tagFacSetWithLevel(void){memset(this, 0, sizeof(tagFacSetWithLevel));}
};

//! ������
typedef	vector<tagUpgradeCondition> VecCondition;


//! ����ȫ������
struct tagGlobeSetup
{
	LONG							lDemiseTime;							//! �᳤�����ߵ��ʱ�䣬�������Զ�ת�û᳤
	LONG							lDisbandFactionTime;					//! ��ɢС�ڹ̶������İ����ӳ�ʱ��
	LONG							lNegativeTime;							//!	��ԴΪ����ʱ��
	LONG							lBaseBattleStart;						//! �ݵ�ս��ʼʱ��,Сʱ
	LONG							lBaseBattleLast;						//!	�ݵ�ս����ʱ�䣬Сʱ
	BYTE							byClearBaseTime;						//!	��վݵ��ʱ��(����)
	tagGlobeSetup(void){memset(this, 0, sizeof(tagGlobeSetup));}
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
const	LONG	MAXNUM_MemberPurview		= eFPI_Disband;

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
	LONG							lCurrSkillID;							//! ��ǰʹ�õļ���ID
	LONG							lLevel;									//! ���ȼ�

	tagFacPublicData(void){memset(this, 0, sizeof(tagFacPublicData));}
};

//! �������ʱ���ܿ�������Ϣ
struct tagApplyInfo
{
	CGUID							guid;									//! ���ID
	char							szName[MAXSIZE_FactionName];			//! �������
	LONG							lLevel;									//! ���ȼ�
	LONG							lMemberNum;								//! ��Ա����
	LONG							lMaxMemberNum;							//! ����Ա����
	BOOL							bIsRecruiting;							//! �Ƿ�������ļ
	LONG							lCurrRes;								//! ��ǰӵ����Դ
	

	tagApplyInfo(void){memset(this, 0, sizeof(tagApplyInfo));}
};

//! ������Ϣ����
struct tagFacBaseData
{
	LONG							lCountryID;								//! ��������ID

	CGUID							MasterGuid;								//! ����ID
	char							szMasterName[MAX_MEMBER_NAME_SIZE];		//! ��������

	BOOL							bOpenResWar;							//! �Ƿ������Դս���Ŀ���
	LONG							lCurrRes;								//! ��ǰӵ����Դ
	LONG							lCurrExp;								//! ��ǰ�Ĺ��ᾭ��

	LONG							lSubExpLevel;							//! ���Ӿ���ȼ�(���帣��)
	LONG							lSpecialityLevel;						//! ����ֿ�ȼ�(��Ϊ����רҵ)
	LONG							lBattleLevel;							//! ����ȼ�(��Ϊ����ս����
	LONG							lBussinessLevel;						//! ����ȼ�(��Ϊ������ҵ)
	LONG							lNoblelevel;							//! ���ҵȼ�(��Ϊ�����Թ�)

	LONG							CreateTime;								//! ����ʱ��
	CGUID							SuperiorGuid;							//! �ϼ���ID
	char							arr_szJobName[MAXNUM_JobLvl][MAX_MEMBER_NAME_SIZE];		//! ���йٽ׵�Ȩ��
	LONG							arrJobPurview[MAXNUM_JobLvl];			//! ���йٽ׵�Ȩ��
	LONG							lMemberNum;								//! ��Ա��������������ʹ�ø�ֵ������ͻ��˷���ʱ��ֵ��

	BOOL							bIsRecruiting;							//! �Ƿ�������ļ
	LONG							lFacBattle;								//! ս����ֵ
	LONG							lFacArmyInvest;							//! ����Ͷ����ֵ




	tagFacBaseData(void){memset(this, 0, sizeof(tagFacBaseData));bIsRecruiting = TRUE;}
};


//! ����֯��ÿ����Ա����Ϣ
struct  tagFacMemInfo
{
	CGUID							MemberGuid;								//! ��ԱID
	char							szName[MAX_MEMBER_NAME_SIZE];			//! ��Ա����
	char							szTitle[MAX_MEMBER_NAME_SIZE];			//! �ƺ�����
	LONG							lLvl;									//! ��Ա�ȼ�
	LONG							lOccu;									//! ��Աְҵ
	LONG							lJobLvl;								//! �ó�Ա��ְ��
	LONG 							LastOnlineTime;							//! ���һ������ʱ��
	LONG							lContribute;							//! ����ֵ
	BOOL							bIsOnline;								//! �Ƿ�����
	char							szRegion[MAX_MEMBER_NAME_SIZE];			//! ���ڵ�ͼ
	BYTE							cHide;									//! �Ƿ�Ϊ������
	LONG							lBattle;								//! ս��ֵ
	LONG							lArmyInvest;							//! ����Ͷ��ֵ
	BOOL							bJoinBattle;							//! ��ȡ�ݵ�ս��Ʊ��Ȩ��
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
const LONG PLAYER_OPT_SUCCEED	= 1;
//! ����ʧ��
const LONG PLAYER_OPT_FAILED	= 0;

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
	LONG							lCountryID;								//! ��������ID
	CGUID							MasterGuid;								//! ����ID��������Player��
	CGUID							MasterFacGuid;							//! �������ڹ���
	char							szMasterName[MAX_MEMBER_NAME_SIZE];		//! ��������	
	LONG							CreateTime;								//! ����ʱ��

	tagUnionBaseData(void){memset(this, 0, sizeof(tagUnionBaseData));}
};


//! ������ÿ����Ա����Ϣ
struct  tagUnionMemInfo
{
	CGUID							MemberGuid;								//! ��ԱID����Ա��Faction��
	char							szName[MAX_MEMBER_NAME_SIZE];			//! ��Ա����
	LONG							lJoinDate;								//! ����ʱ��
	LONG							lPurview;								//! ��ԱȨ��
	

	tagUnionMemInfo(void){memset(this, 0, sizeof(tagUnionMemInfo));}
};

//! ����ȼ���Ӧ�ļ���Ƽ���߼�
struct tagTechInfo
{
	LONG	_lSubExpLv;//�����ȼ�
	LONG	_lSpecialityLv;//רҵ�ȼ�
	LONG	_lBussinessLv;//��ҵ�ȼ�
	LONG	_lBattleLv;//ս���ȼ�
	LONG	_lNoblelv;//����ȼ�
	tagTechInfo(){ZeroMemory(this,sizeof(tagTechInfo));}
};

struct tagBaseConsume
{
	LONG	_BaseVal;//����
	LONG	_Critical;//�ٽ�ֵ
};

//һ���ݵ����Ϣ
struct tagBaseInfo
{
	char		_cBaseName[32];//�ݵ�����
	LONG		_lTickets;//��Ʊ��
	LONG		_lBattleVal;//ս��ֵ
	CGUID		_FacGuid;//ռ������GUID
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
	DWORD _dwStartTime;//ս����ʼʱ��
	DWORD _dwEndTime;
};

#pragma pack(pop)
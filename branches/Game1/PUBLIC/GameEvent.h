#ifndef __GAME_EVENT__
#define __GAME_EVENT__
class CMessage;
#include "../public/GUID.h"

//��ʱ����
enum eChildObjType
{
	ChildObjType_AI,
	ChildObjType_Buff,
	ChildObjType_DeBuff,
	ChildObjType_ActiveSkill,
	ChildObjType_PassiveSkill,
	ChildObjType_OnOffSkill,
	ChildObjType_GoodsTimeout,	 // ��ʱ��Ʒ��ʱ
	ChildObjType_BuyLimitClear,
	ChildObjType_BuySellClear,
	ChildObjType_PriceUpdate,
	ChildObjType_CreateRgnQueue, // ����ŶӴ�������
	ChildObjType_CreatePRgn,	 // ��Ҵ������˸���ʱ����
	ChildObjType_CreateTRgn,	 // ��Ҵ�����Ӹ���ʱ����
	ChildObjType_DeleteRgn,		 // ����������ɾ����������
	ChildObjType_DeleteObj,		 //	ɾ��������OBJECT
	ChildObjType_ReBornObj,		//  ����object����
	ChildObjType_AddObj,		//	����object������
	ChildObjType_AddShapeToClient,//��Ӷ��󵽿ͻ��˵Ķ�ʱ��
	ChildObjType_InvestLimit,	 // Ͷ��ʱ�����ƵĽ��
	ChildObjType_BusinessUpdate,
	ChildObjType_WSMsg,			 // WS����Ϣ��ʱ��ʱ��
	ChildObjType_PlayerHint,	 // ��ʾ����
	ChildObjType_RunScript,		 // ִ�нű�
	ChildObjType_AreaHiberate,
	ChildObjType_DelGoods,
	ChildObjType_PKValue,		 //pkֵ
	ChildObjType_PvPValue,		 //����ֵ
	ChildObjType_AutoRecoverHpMp,//�ָ�Hp,Mp
	ChildObjType_AutoRecoverEnergy,//�ָ�����
	ChildObjType_Beneficiary,	 //�������¼�
	ChildObjType_UpdateMercQuest,//���ˢ��ٸ������
	ChildObjType_Mail,			//�ʼ�
	ChildObjType_SysMail,		//ϵͳ�ʼ�;
	ChildObjType_Weather,		//��������
	ChildObjType_CollectionDied,//�ɼ�������
	ChildObjType_UpdateAttr,	//�������Զ�ʱ��
	ChildObjType_RgnKickPlayerTime,	 // ��Ҹ�������ʱ����
	COT_PET_EVENT,				// �����¼�(��������PET�д���)
	ChildObjType_Mail_CheckTime,// �ʼ���ʱ���
	ChildObjType_Fcm_KickPlayerTime,// ������ϵͳ���������
	ChildObjType_ReliveTimer,//���������ʱ
	ChildObjType_GoodsExTimeOut, /// ͳһ�����Ʒ��ʱ��
	ChildObjType_ChangeMdTimeOut, //������װ��ʱ��
};

//#define _DEBUG_TIMER_VAR_


//��ʱ����(void* var)�����ṹ
struct tagTimerVar
{
private:
#ifdef _DEBUG_TIMER_VAR_
	tagTimerVar(long Flag):DebugFlag(Flag){lvar=0;lvar1=0;pvar=NULL;pvar_size=0;}
	long DebugFlag;
	~tagTimerVar(){}
#else
	tagTimerVar(){lvar=0;lvar1=0;pvar=NULL;pvar_size=0;}
#endif
	friend class CBaseObject;
public:	
	eChildObjType TimerType;
	long lvar;
	long lvar1;
	void *pvar;
	long pvar_size;
	
};

//�¼�����
enum eEventType
{
	//�ƶ�
	ET_Move,
	//������
	ET_BeAttack,
	//����
	ET_Attack,
	//�˺�
	ET_Damage,
	//���˺�
	ET_BeenDamage,
	//װ��
	ET_EquipMount,
	//жװ��
	ET_EquipUnmount,
	//����˳���Ϸ
	ET_ExitGame,
	//�뿪����
	ET_ExitRegion,
	//��״̬�¼�
	ET_AddState,
	//ʹ�ü����¼�
	ET_UseSkill,
	//��ҽ�����Ϸ
	ET_EnterGame,
	//תְ
	ET_ChangeOccu,	
	//�������ս������
	ET_ResetBaseFightProperty,
	//����
	ET_Trade,
	//����
	ET_OpenShop,
	//ɱ��Ŀ��
	ET_Kill,
	//��ɱ��
	ET_BeenKilled,
	//����Ŀ��
	ET_Blast,
	//������
	ET_BeenBlast,
	//��Ŀ��
	ET_Block,
	//��Ŀ���
	ET_BeenBlocked,
	//����Ŀ��
	ET_Dodge,
	//��Ŀ������
	ET_BeenDodged,
	//�м�Ŀ��
	ET_Parry,
	//���м�
	ET_BeenParried,
	//�ֿ�Ŀ��
	ET_Resist,
	//���ֿ�
	ET_BeenResisted,
	//����ս��״̬
	ET_Fighting,
	//�����ƽ״̬
	ET_Peace,
	//�����¼�
	ET_SendMail,
	//�жϼ���
	ET_BreakSkill,
	//״̬�ж�
	ET_StateBreak,
	//�¼��ж�
	ET_EventBreak,
	//���볡���¼�
	ET_EnterRegion,
	//���ܳɹ�
	ET_SuccessSkill,
	//�����˺�
	ET_DamageEx,
	//�����˺�
	ET_BeenDamageEx,
	//��λ�¼�
	ET_Homing,
	//DOTA�˺�
	ET_SAttack,
	//��DOTA�˺�
	ET_BeSAttack,
};


//�¼�ע�����
struct tagEvenRegVar
{
	long lEventID;
	//���ĸ��¼���һ������(player,monster,regiongood)
	long lType1;
	CGUID ID1;
	//���ĸ��¼��Ķ�������(buff,skill,playergood)
	long lType2;
	long lID2;
	
	tagEvenRegVar()
		:lEventID(0),lType1(0),ID1(NULL_GUID),lType2(0),lID2(0)
	{
	}
	
	tagEvenRegVar(long EventID,long Type1,const CGUID& guid,long Type2,long ID2)
		:lEventID(EventID),lType1(Type1),ID1(guid),lType2(Type2),lID2(ID2)
	{
	}
};

//������̲���
struct tagProcVar
{
	//���ĸ��¼����Ӷ���
	long lType;
	long lID;
	
	//�Զ������
	long lVar1;
	long lVar2;
	long lVar3;
	long lVar4;
	void* pVar;
};


class CGameEvent
{
public:
	//������Ϣ��Ӧ
	virtual void OnMessage(CMessage*)=0;
	//��ʱ��Ӧ
	virtual long OnTimeOut(DWORD timerid,DWORD curtime,const void* var)=0;
	//����ʱ����ȡ���ĵ���
	virtual void OnTimerCancel(DWORD timerid,const void* var)=0;
	//����ʵ�����Ϣ����
	virtual long Proc(eEventType type,const tagProcVar *pVar)=0;
};

#endif

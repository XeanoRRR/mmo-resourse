//=============================================================================
/**
*  file: BuffSkillDef.h
*
*  Brief:��ȡbuff,skillģ��Ļ�������
*
*  Authtor:wangqiao
*	
*	Datae:2007-8-31
*/
//=============================================================================

#ifndef __BUFFF_SKILL_DEF_H__
#define __BUFFF_SKILL_DEF_H__

#pragma once

class CMoveShape;
class CScript;
class CVariableList;
struct tagEvenRegVar;
struct tagProcVar;

//��ģ�麯������ֵ����
enum eModuleRet
{
	MR_No = 0,//δ֪

	MR_Continue=1,//����ִ��
	MR_Return,	//����
	MR_Proc_Next,//��һ��������
	MR_Step_End,//Step����
	MR_Proc_End,//���������
	MR_Modu_End,//ģ�����
};

//����ģ������
enum ModuleType
{
	ModuType_Buff,			//buff
	ModuType_DeBuff,		//debuff(�����˺�����)
	ModuType_Skill_Active,	//��������
	ModuType_Skill_Passive,	//��������
	ModuType_Skill_OnOff,	//����(�ж�)����	
};
//������������־��
enum eAddSub
{
	eNo_Type=0,
	eAdd_Type=1,
	eSub_Type,
};
//������־��
enum eAttack
{
	eNoAttack=0,
	eYesAttack,
};
//Ŀ���־��
enum eTarget
{
	eNoTarget=0,
	eYesTarget,
};
//�������
enum eSkillUseType
{
	SKILL_TYPE_NO=0,
	SKILL_TYPE_PHYSICAL=1,//������
	SKILL_TYPE_MAGIC,	//ħ����
};


struct stTimerParam : BaseMemObj
{
	//��ʱ��ID
	long lTimerID;
	//Ԥ������ʱ��
	long lFuture;
	//���ʱ��
	long lInter;
	//���ô���
	DWORD nCount;
	//�Ѿ����ô���
	DWORD nCalledCount;
	//��ʼʱ��
	DWORD nStartTime;
	//��ʾ����ID
	long lFlag;		
	//�ö�ʱ�����ĵ�Ŀ��
	long lDestType;
	CGUID DestID;

	void GetRemainTime(DWORD &dwFutue,DWORD &dwCount);
};

struct stEventParam : public BaseMemObj
{
	//�¼�����
	long lEventType;	
	//��ʾ����ID
	long lFlag;	
	tagEvenRegVar RegVar;
};

//ģ��ľ�̬����
struct stStaticModuParam : public BaseMemObj
{
	//��ȴʱ��
	long lCoolDownTime;
	//���ĵķ���
	long lConsMp;
	//��С��������
	long lMinAtkDistance;
	//��󹥻�����
	long lMaxAtkDistance;
	//����ʱ��
	long lActTime;
	//
	list<long> lBuffID;
	//
	list<long> lNBuffID;
	//�ű�����
	list<long> lScriptParam;
};

//�ۺϲ����ṹ��
struct stModuParam : public BaseMemObj
{
	//����
	ModuleType eType;
	//Ψһid
	DWORD dwGlobleID;
	//ID�͵ȼ�
	long nID;
	int nLvl;
	//���
	eSkillUseType eUseType;
	//�������
	eAddSub eAddSubType;	
	//������־��
	eAttack eTagAttack;	

	//ִ�нű������б�
	CVariableList *pVariableList;

	//��ʼ���ű�������
	string strFileName;
	//��������ID
	DWORD dwCreateID;
	//ʩ����
	int nFirerType;
	CGUID FirerID;
	CMoveShape *pFirer;

	//Ŀ���־��
	eTarget eTagTarget;
	//��ȴʱ��
	long lCoolDownTime;
	//���б�־��
	bool bRun;
	BOOL bTime;
	

	//������
	CMoveShape *pUser;
	//����
	long nMouseX;
	long nMouseY;
	//�����е�shape����
	int nMouseShapeType;
	CGUID MouseShapeID;
	CMoveShape* pMouseShape;

	//Ŀ������
	long lDestX;
	long lDestY;

	//��ǰ�����Ķ�ʱ��id
	long lCurTimerID;
	//��ǰ�������¼�����
	long lCurEventType;
	//��ǰ�������¼�����
	tagProcVar *pEventVar;
	//��ǰִ�е�procedure���к�
	long nProceNum;
	//�׶κ�ʱ
	long lUseTime;
	//�������ܵ���ƷID
	long lGoodsIndex;
	//��Ʒ����ID
	long lContainerID;
	//λ��
	long lPos;
	//��ƷGUID
	CGUID GoodsGuid;
	//��ǰprocedure���õ�Ŀ�����
	list<stDestObj*>	OperObjs;
	//��������
	 int	nPosition;
	 //Ԫ������
	 int nElement;

	//��ǰprocedure����ʹ�õĶ�ʱ��
	typedef list<stTimerParam*>::iterator itTimer;
	list<stTimerParam*> Timers;

	//��ǰprocedureע����¼�
	typedef list<stEventParam*>::iterator itEvent;
	list<stEventParam*> Events;

	//�˺�ֵ����
	typedef map<long,long>::iterator itDamage;
	//Ŀ���˺�ֵ�б�
	map<long,long> DestDamages;
	//�Լ��˺�ֵ�б�
	map<long,long> SelfDamages;
	//Ŀ���Ѫ�б�
	map<long,long> DestIncHP;
	//�Լ���Ѫ�б�
	map<long,long> SelfIncHP;

	

	//������Ϣ����ҵ��б�
	hash_set<CGUID,hash_guid_compare> SendMsgPlayers;

	//����TimerID�ҵ���Ӧ�Ĳ���
	stTimerParam* FindTimerParam(long lTimerID);
	//�������ò���������Ŀ��Ѱ�Ҷ�ʱ������
	stTimerParam* FindTimerParam(long lFlag,long lDestType,const CGUID& DestID);
	//��ʱ����
	void OnTimerOut(long lTimerID);
	//��ȡID
	long				GetID()				{return nID;}		
	//��ȡ�ȼ�	
	int					GetLv()				{return nLvl;}	
	//��ȡȫ��Ψһ��־��
	DWORD				GetGlobleID()		{return dwGlobleID;}  
	//����
	ModuleType			GetModType()		{return eType;}		
	
	//����ID
	void				SetContainerID(long lID){lContainerID = lID;}
	long				GetContainerID()		{return lContainerID;}
	//λ��
	void				SetPos(long pos)		{lPos = pos;}		
	//��Ʒguid
	void				SetGoodGuid(CGUID Guid){GoodsGuid=Guid;}
	//ʩ��������
	int					GetFirerType()		{return nFirerType;}  
	//ʩ����guid
	CGUID				GetFirerID()		{return FirerID;}		
	//ʩ����
	CMoveShape			*GetFirer()			{return pFirer;}
	//�׶κ�
	long				GetProcNum()		{return nProceNum;}		
	
	//���
	eSkillUseType		GetUseType()		{return eUseType;}	
	//Ԫ������	
	int					GetElemnet()		{return nElement;}	
	//��ǰ�������¼�����
	tagProcVar			*GetEventProc()		{return pEventVar;}
	//��ʱ��־
	BOOL				GetTime()			{return bTime;}		


	
	

	DWORD				GetCreateID()		{return dwCreateID;}  //����ID
	eAddSub				GetAddSub()			{return eAddSubType;}
	eAttack				GetAttack()			{return eTagAttack;}	//������־��
	eTarget				GetTarget()			{return eTagTarget;}	//Ŀ���־��
	bool				GetbRun()			{return bRun;}		//�Ƿ�������־��


	long				GetGoodsIndex()		{return lGoodsIndex;} //��ƷID
	
	
	long				GetPos()			{return lPos;}
	
	CVariableList*		GetVariableList()	{return pVariableList;}	//�����б�
	
	void				SetbRun(bool b)		{bRun = b;}			//���б�־��
	
	
	
	void				SetGoodsIndex(long l){lGoodsIndex = l;}

	//����Ŀ������
	void SetDesObjs(long lMouseX,long lMouseY,long lMouseShapeType,const CGUID& ShapeID);
	void SetDestObjByEvent(CMoveShape* pShape)		{pMouseShape = pShape; }

	//��ֵ
	void				SetGlobleID(DWORD dwGID)			{dwGlobleID=dwGID;}
	//�ű�����
	long GetScriptParam(int nPos);	
	//������������
	void Reset(CMoveShape *pOwner,CMoveShape *pFire);
	//������ȴʱ��
	void SetCooldownTime();		
	//����¼��Ͷ�ʱ��
	void ClearTimerAndEvent();
	//�ͷŶ���
	void ClearOperObj();
	//�ͷŽӿ�
	void Clear();

	void ProcNumSub();
	// ��ӵ�CByteArray
	bool AddToByteArray(vector<BYTE>* pByteArray);
	// ��ӵ�CByteArrayΪ���͵��ͻ���
	bool AddToByteArrayForClient(vector<BYTE>* pByteArray,ModuleType eType);
	// ����
	bool DecordFromByteArray(long lProceNum,list<stTimerParam*>& Timers);

	// ��ӵ�DataBlock
	bool CodeToDataBlock(DBWriteSet& setWriteDB);
	// ����ӵ�DataBlockΪ�ͻ���
	bool CodeToDataBlockForClient(DBWriteSet& setWriteDB,ModuleType eType);
	// ����
	bool DecodeFromDataBlock(long lProceNum,list<stTimerParam*>& lTimers);

};

enum eObj
{
	Obj_Self=0x1,//ʹ�����Լ�
	Obj_Dest=0x2,//���ö���
	Obj_Mouse=0x3,//��������
	Obj_Firer=0x4,//ʩ����
};

enum eModuleOper
{
	//�Ӳ���
	MODOP_Add,
	//������
	MODOP_Sub,
	//���ò���
	MODOP_Set,
	//���
	MODOP_Clear,
	//��Ѫ
	MODOP_Inc,
};

typedef list<CMoveShape*>::iterator itObj;
typedef list<stDestObj*>::iterator itDestObj;

#endif

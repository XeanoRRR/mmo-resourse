///////////////////////////////////////////////////////////////////////////////
///  NewSkillList.h
///
///  PATH: E:\alienbrainWork\Game_Program_Develop\guangmang\SERVER\setup
///
///  CREATED: 08/06/2007 13:49:01 PM by ���Ƚ�
///
///  PURPOSE:   ������
//////////////////////////////////////////////////////////////////////
#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_NEWSKILLLIST_INCLUDED
#define _INC_NEWSKILLLIST_INCLUDED
#include <vector>
#include <list>
#include <string>
#include <map>
using namespace std;

class CNewSkillProperties
{
public:

	//��������
	struct tagPropertyCondition 
	{
		DWORD  dwValue;		//����ֵ	
		string strObject;	//������
		string strName;		//��������
		string cmpareOp;	//�Ƚϲ�����		
	};
	//��Ʒ��������
	struct tagGoodAddProperty
	{
		DWORD dwAddProType;	//������������
		DWORD dwValue1;		//ֵ1
		DWORD dwValue2;		//ֵ2	
	};
	//��Ʒ����
	struct tagGoodCondition 
	{
		DWORD  dwValue;		//����
		DWORD  dwPlace;		//�ص�
		DWORD  dwPos;		//λ��
		DWORD  dwType;		//��Ʒ���
		string strName;		//��Ʒԭʼ		
		string strCmpFun;	//������
		string strObject;   //������
		
		list<tagGoodAddProperty*> ltgGoodAddProperty;
	};
	//״̬����
	struct tagBuffCondition
	{	
		DWORD  dwID;		//״̬ID
		DWORD  dwLevel;		//�ȼ�
		DWORD  dwNum;		//����
		string strObject;	//������	
	};
	//��������
	struct tagSkillCondition 
	{		
		DWORD  dwID;		//����ID
		DWORD  dwLevel;		//�ȼ�	
		string strObject;   //������
	};
	//��������
	struct tagDistanceCondition
	{
		DWORD  dwValue;		//ֵ
		string strCmpFun;	//������
		string strRefObj;	//���ն���
	};

	//�¼�����
	struct tagEventCondition 
	{
		DWORD  dwFlag;
		DWORD  dwVar1;
		DWORD  dwVar2;
		DWORD  dwVar3;				//����
		DWORD  dwVar4;				//С����
		string strEventName;	
		string strName;		
	};
	struct tagBlockCondition
	{	
		DWORD  dwBlockType;//�赲����
		DWORD  dwShapeType;//shape����
		string strType;	   //����
		string strDir;	   //����
	};
	struct tagFaceCondition 
	{
		string strFaceType;//1��ʾĿ������Լ�
	};
	struct tagValidObjectCondition
	{
		string       strType;		 //�������ͣ�����PLAYER OR MONSTER��
		list<string> lObjType;		 //�����������
	};

	struct tagRegionCondition 
	{
		string strName;
		DWORD dwType;
	};
	struct tagOccupationCondition
	{
		list<long>	lOccu;
	};


	//����������
	struct tagConProperty
	{	
		list<tagPropertyCondition*> ltgProtertyConditon;	
	};

	//��Ʒ������
	struct tagConGood 
	{
		list<tagGoodCondition*>	  ltgGoodCondition;			
	};

	//״̬������
	struct tagConBuff
	{
		list<tagBuffCondition*>	  ltgBuffCondition;			
	};

	//����������
	struct tagConSkill 
	{
		list<tagSkillCondition*>	  ltgSkillCondition;
	};

	//����������
	struct tagConDistance 
	{
		list<tagDistanceCondition*> ltgDistanceCondition;		
	};
	//�¼�������
	struct tagConEvent 
	{
		list<tagEventCondition*>	  ltgEventCondition;
	};
	//ʱ��
	struct tagConTimer
	{
		list<DWORD>					 ldwConTimer;
	};
	//����������
	struct tagConValidObject 
	{	
		list<tagValidObjectCondition*> ltgValidObjectCondition;		
	};
	//ְҵ����
	struct tagConOccupation
	{
		list<tagOccupationCondition*>	ltgOccupationCondition;
	};
	//�赲����
	struct tagConBlock
	{
		list<tagBlockCondition*>	 ltgBlockCondition;
	};
	//��������
	struct tagConFace 
	{
		list<tagFaceCondition*>		ltgFaceCondition;
	};
	//��������
	struct tagConRegion 
	{
		list<tagRegionCondition*>   ltgRegionCondition;
	};

	//�ű�������
	struct tagConScript
	{
		string						strScript;
	};


	struct tagCondition 
	{
		DWORD  dwLength;
		string strType;		
		void*  pVoidCondition;
	};


	//���ö���
	struct tagOperateObject 
	{
		DWORD  dwID;					//ID
		DWORD  dwSkillID;				//����ID
		DWORD  dwSkillLevel;			//�ȼ�
		DWORD  dwConGroupID;			//������ID
		DWORD  dwNumObject;				//�������
		DWORD  dwDistance;				//����
		DWORD  dwRadius;				//�뾶
		string strType;					//����
		string strPosition;				//λ��		
		string strCenterObject;			//���Ķ���		
		string strObject;				//����
	};

	struct tagBaseChange
	{
		string strChangeType;
		string strObj;
		virtual void GetTyp(){}
	};
	//�ı�����
	struct tagChangeProperty :public tagBaseChange
	{
		DWORD  dwValue;					//ֵ
		string strName;					//��������
		string strOperator;				//������		
		string strScriptVarName;		//�ű�������	
	};
	//�ı�״̬
	struct tagChangeBuff :public tagBaseChange
	{
		DWORD dwID;						//״̬ID
		DWORD dwLevel;					//�ȼ�		
		DWORD dwCount;					//����
		string strSIDName;				//ID�ű�������		
		string strSLvName;				//�ȼ��ű�������	
		string strSCountName;			//�����ű�������
		string strOperator;				//������
	};

	//λ���޸�
	struct tagChangePos :public tagBaseChange
	{		
		string strRefObject;			//���λ��
		string strDir;					//����
		string strStyle;				//��ʽ
		string strSpeed;					//�ƶ��ٶ�
	};

	//��ȴʱ��
	struct tagCoolDownChange :public tagBaseChange
	{
		DWORD  dwValue;					//ֵ	
		string strType;					//����	
		string strSVarianame;			//�ű�������
	};
	//ע��ʱ��
	struct tagRegisterTimer :public tagBaseChange
	{
		DWORD  dwFlag;					//����	
		DWORD  dwIntelTime;				//ʱ��		
		DWORD  dwFuture;				//����ʱ��		
		DWORD  dwCount;					//����
		string strSIntelName;		
		string strSFutvarname;			
		string strSCountName;
		
	};

	//������Ϣ
	struct tagSendMsg :public tagBaseChange
	{
		DWORD  dwValue;					//ֵ
		string strType;					//����
		string strRange;				//��Χ				
		string strSVName;				//�ű�������
		int	   nAdd;					//��ӱ�־����0Ϊ�¼ӣ�1Ϊ����(��Ҫ���״̬������)��
	};

	//ע���¼�
	struct tagRegisterEvent :public tagBaseChange
	{
		DWORD  dwFlag;					//����
		string strName;					//ע���¼�����
	};
	//�˺��仯
	struct tagDamageChange :public tagBaseChange
	{
		DWORD  dwValue;					//��ֵ
		string strOperator;				//������
		string strType;					//�˺�����
		string strSVarianame;			//�˺�ֵ�ű�����			
	};	
	//do �˺�
	struct tagDoDamage :public tagBaseChange
	{
		DWORD dwResult;					//�˺����
		DWORD dwBlastAttack;			//����ֵ
		string strSResVariName;			//�˺�����Ľű�����
		string strSBlastVariName;		//�����ű�����
	};
	//��Ʒ
	struct tagGoodChange :public tagBaseChange
	{
		DWORD  dwValue;					//����
		string strOperator;				//������
		DWORD  dwPlace;					//����ID
		DWORD  dwPos;					//��λ
		string strName;					//ԭʼ��
		string strSValname;				//�����ű�����
		DWORD  dwGoodsType;				//��Ʒ���
		DWORD  dwWeaponType;			//�������
	};

	//����
	struct  tagStepConditon 
	{
		DWORD dwConditionGroup;			//������ID
		string strTrueRet;			    //����
		string strFalseRet;			    //�ٽ��
	};


	struct tagAttackChangeObject :public tagBaseChange 
	{
		string strAttackChangeObject;
	};

	//Ŀ�����
	struct tagDestObjChange :public tagBaseChange 
	{
		string strDestObjChange;
	};

	//�ű�
	struct tagScriptID :public tagBaseChange 
	{
		DWORD dwScriptID;
	};

	//ע��ʱ��
	struct tagUnRegisterTimer :public tagBaseChange 
	{
		DWORD dwUnRegisterTimer;
	};

	//ע���¼�
	struct tagUnRegisterEvent :public tagBaseChange 
	{
		DWORD dwUnRegisterEvent;
	};
	//����
	struct tagDirection :public tagBaseChange
	{		
		string strDir;
	};
	//����
	struct tagSkillChange :public tagBaseChange 
	{
		string strType;
		string strOpera;
		long   lID;
		long   lLv;
		string strSIDName;				//ID�ű�������		
		string strSLvName;				//�ȼ��ű�������	
	};




	struct tagCDo;
		//����
	struct tagDoProcess
	{
		list<tagBaseChange*>			ltgBaseChange;
		list<tagCDo*>					ltgCDo;
	};

	struct tagCDo
	{
		tagStepConditon					tgCondition;	//����
		list<tagDoProcess*>				ltgTrueDoProcess;//�����
		list<tagDoProcess*>				ltgFalseDoProcess;//�ٲ���
	};


	//Do����
	struct tagDo
	{
		DWORD						dwOperobjid;				//���ö���ID
		string						strRet;						//���ؽ��
		list<tagCDo*>				ltgCDo;
	};



	//ִ�в���
	struct tagStep
	{
		string							strName;				//����
		list<tagDo*>					ltgDo;					//Ϊ�����				
	};



	//����ִ�й���
	struct tagProcess 
	{
		string						strName;					//�������� 	
		list<tagStep*>				ltgStep;					//ִ�в���
	};

	//ģ��ľ�̬����
	struct stStaticParam
	{
		
		long lCoolDownTime;//��ȴʱ��		
		long lConsMp;//���ĵķ���	
		long lMinAtkDistance;	//��С��������		
		long lMaxAtkDistance;//��󹥻�����		
		long lActTime;		//����ʱ��	
		string strBuffID;	//״̬id		
		string strNBuffID;//NoBuffID		
		list<long> lBuffID;//״̬id		
		list<long> lNBuffID;//NBuffID		
		string strScriptParam;//�ű�����		
		list<long> lScriptParam;//�ű�����
	};

	//������Ҫ�ṹ
	struct tagSkill
	{
		DWORD					 dwLevel;						//�ȼ�	
		stStaticParam*			 pStaticParam;				    //��̬����
		map<DWORD,tagCondition*> mapCondition;					//������ 	
		list<tagProcess*>		 ltgExecuteStep;				//�׶�
		map<string,string>		 mapVarible;					//�����б�		
	};

	//������
	struct tagConditionGroup
	{
		string			strMode;
		list<DWORD>		lConditonGroup;
	};

	//�ű�
	struct tagExpression
	{
		string strFileName;//�ļ���
		string strContent;//��������
	};

protected:


	DWORD							m_dwID;						//����ID	
	string							m_strName;					//��������
	string							m_strInitSricpt;			//��ʼ���ű������ļ���
	DWORD							m_dwElement;				//Ԫ������
	string							m_strType;					//����
	BYTE							m_bUseType;					//���
	string							m_strAddSub;				//����ͼ���ı�־��Add��ʾ����Sub��ʾ����	
	BYTE							m_bOnly;					//Ψһ��־��1��Ψһ0���Ψһ
	DWORD							m_dwOnlyType;				//Ψһ����
	BYTE							m_bNameOnly;				//ͬ��Ψһ��־��
	BYTE							m_bRoleOnly;				//��ɫΨһ��־��
	BYTE							m_bAttack;					//�����ͷǹ������ܱ�־��1��ʾ����0��ʾ�ǹ���
	BYTE							m_bTarget;					//Ŀ���־��
	BYTE							m_bCooldown;				//��ȴ�����־��1�Ǳ���0������
	BYTE							m_bDeadUse;					//�ܷ������Ŀ��ʹ�ü��ܱ�־��0��ʾ����ʹ��1��ʾ����ʹ��
	BYTE							m_bDeadEnd;					//�����Ƿ����״̬0��ʾ����1��ʾ����
	BYTE							m_bBreak;					//�жϼ���ʩ�ű�־��(1��ʾ�����ж�0��ʾ���ж�)
	BYTE							m_bPetType;					//���＼������
	BYTE							m_bTime;					//״̬��ʱ��־��(1��ʾ��������״̬��ʱ0��ʾ������״̬����ʱ)
	BYTE							m_bSystem;					//ϵͳ״̬��־��
	DWORD							m_dwTotalNum;				//ͬ��״̬�����Ӹ���
	list<tagSkill*>					m_lSkill;					//�����б�
	map<DWORD,tagConditionGroup*>	m_mapConditionGroup;		//������
	map<DWORD,tagCondition*>		m_mapCondition;				//������ 
	map<DWORD,tagExpression>		m_mapFormat;				//���㹫ʽ			
	map<DWORD,tagOperateObject*>	m_mapOperateObject;		    //�������� 
	
public:
	CNewSkillProperties(void);
	CNewSkillProperties(DWORD dwSkillID);
	virtual ~CNewSkillProperties(void);

	void	SetNewSkillID		( DWORD dwID )											{ m_dwID = dwID;}						//����ID								
	void	SetNewSkillName		( string strName)										{ m_strName = strName;}					//��������								
	void	SetDescription		( string strDescription )								{ m_strInitSricpt = strDescription;}	//���ó�ʼ���ű��������ļ���			
	void	SetElement			( DWORD dwElement)										{ m_dwElement = dwElement;}				//����Ԫ������							
	void	SetSkillType		( string strType )										{ m_strType = strType; }				//����ģ������							
	void	SetUseType			( BYTE bUseType)										{ m_bUseType = bUseType; }				//����ģ�����							
	void	SetOnly				( BYTE bOnly)											{ m_bOnly = bOnly;}						//����Ψһ��־��						
	void	SetOnlyType			( DWORD dwOnlyType)										{ m_dwOnlyType = dwOnlyType;}			//����Ψһ����							
	void	SetNameOnly			( BYTE bNameOnly)										{ m_bNameOnly = bNameOnly;}				//����Ψһ��־��						
	void	SetRoleOnly			( BYTE bRoleOnly)										{ m_bRoleOnly = bRoleOnly;}				//��ɫΨһ��־��						
	void	SetTotalNum			( DWORD dwNum)											{ m_dwTotalNum = dwNum;}				//�ۼ�����																			
	void	SetAddSub			( string str)											{ m_strAddSub = str;}					//��������
	void	SetAttack			( BYTE bAttack)											{ m_bAttack = bAttack;}					//�������ܱ�־��
	void	SetTarget			( BYTE bTarget)											{ m_bTarget = bTarget;}					//Ŀ���־��	
	void	SetCoolDown			( BYTE bCooldown)										{ m_bCooldown = bCooldown;}				//��ȴʱ��
	void	SetDeadUse			( BYTE bDeadUse)										{ m_bDeadUse = bDeadUse;}				//������Ŀ��ʹ�ü��ܱ�־��
	void	SetDeadEnd			( BYTE bDeadEnd)										{ m_bDeadEnd = bDeadEnd;}
	void	SetBreak			( BYTE bBreak)											{ m_bBreak	= bBreak;}					//�жϱ�־��
	void	SetPetType			( BYTE bType)											{ m_bPetType = bType;}					//���＼������
	void	SetTime				( BYTE bTime)											{ m_bTime	= bTime;}					//��ʱ��־��
	void	SetSystem			( BYTE bSystem)											{ m_bSystem = bSystem;}					//ϵͳ��־��

	void	SetFormat			( DWORD dwID, string& strFileName,string& strContend); //�ű�
	void    SetOperateObject	( DWORD dwObjectID,tagOperateObject *ptgOperateObject)	{m_mapOperateObject[dwObjectID] = ptgOperateObject;}							//���ö���

	void    AddNewSkill			( tagSkill *pSkill);
	void	SetCondition		( DWORD dwID,tagCondition *tgCondtition);
	void	SetConGroup			( DWORD dwGroupID,tagConditionGroup *ptgConditionGroup);		
	
	tagSkill						*GetNewSkill(DWORD dwLevel);
	map<DWORD, tagOperateObject*>	GetOperateObject	()  							{return m_mapOperateObject;}												
	map<DWORD,tagExpression>&		GetFormat			()								{return m_mapFormat;}		
	map<DWORD,tagCondition*>		GetCondition		()								{return m_mapCondition;}	 	
	map<DWORD,tagConditionGroup*>	GetConditionGroup	()								{return m_mapConditionGroup;}	
	string							GetSkillType		()								{return m_strType;}
	DWORD							GetNewSkillID		()								{return m_dwID;}
	BYTE							GetOnly				()								{return m_bOnly;}
	DWORD							GetOnlyType			()								{return m_dwOnlyType;}	
	//BYTE							GetSkillAdd			()								{return m_bSkillAdd;}
	string							GetAddSub			()								{return m_strAddSub;}
	BYTE							GetUseType			()								{return m_bUseType;}
	string							GetInitScriptName	()								{return m_strInitSricpt;}
	int								GetElement			()								{return m_dwElement;}
	BYTE							GetNameOnly			()								{return m_bNameOnly;}
	BYTE							GetRoleOnly			()								{return m_bRoleOnly;}
	DWORD							GetTotalNum			()								{return m_dwTotalNum;}
	BYTE							GetAttack			()								{return m_bAttack;}
	BYTE							GetTarget			()								{return m_bTarget;}
	BYTE							GetCoolDown			()								{return m_bCooldown;}
	BYTE							GetDeadUse			()								{return m_bDeadUse;}
	BYTE							GetDeadEnd			()								{return m_bDeadEnd;}
	BYTE							GetBreak			()								{return m_bBreak;}
	BYTE							GetPetType			()								{return m_bPetType;}
	BYTE							GetTime				()								{return m_bTime;}
	BYTE							GetSystem			()								{return m_bSystem;}
	stStaticParam					*GetStaticParam		(DWORD dwLevel);
	
};

#endif//_INC_NEWSKILLLIST_INCLUDED
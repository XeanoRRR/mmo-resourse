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
        string paramvalue;  //�ַ�������
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
        string strOper;     //���㷽ʽ,��Ҫ��������϶�ȡ�ȼ����ý��бȽ�
		string strObject;	//������
        string strBufObject;//��֤buf���ڶ���
        string strLevel;    //�ȼ�������
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
        float value;
		string strValueName;    //ֵ
		string strCmpFun;	    //������
		string strRefObj;	    //���ն���
	};
    //��������
    struct tagOddsCondition 
    {
        short   value;          //����ֵ
        string  strCmpFun;      //���ʱȽϺ���
        string  strValueName;   //����ֵ����
    };
    //��ɢ����
    struct tagDispelCondition 
    {
        short  Num;             //��ɢ����
        string strDispelNum;    //��ɢ�ַ���
        string strDispelType;   //��ɢ����
    };

	//�¼�����
	struct tagEventCondition 
	{
		DWORD  dwFlag;
		DWORD  dwVar1;
		DWORD  dwVar2;
		DWORD  dwVar3;				//����
		DWORD  dwVar4;				//С����
        string dwSkillId;           //���ڼ���¼����ض��ض�����ע��
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
		list<tagSkillCondition*>   ltgSkillCondition;
	};

    //����������
    struct tagConOdds 
    {
        list<tagOddsCondition*>    ltgOddsCondition;
    };

    //��ɢ������
    struct tagConDispel
    {
        list<tagDispelCondition*>  ltgDispelCondition;
    };

	//����������
	struct tagConDistance 
	{
		list<tagDistanceCondition*> ltgDistanceCondition;		
	};
	//�¼�������
	struct tagConEvent 
	{
		list<tagEventCondition*>	ltgEventCondition;
	};
	//ʱ��
	struct tagConTimer
	{
		list<DWORD>					ldwConTimer;
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
        BYTE   IsWithIn;                //�Ƿ��Ŀ��ķ�Χ��
		DWORD  dwID;					//ID
		DWORD  dwSkillID;				//����ID
		DWORD  dwSkillLevel;			//�ȼ�
		DWORD  dwConGroupID;			//������ID
		DWORD  dwNumObject;				//�������
		float  distance;				//����
		float  radius;				    //�뾶
        float angle;                    //���νǶ�
        float width;                    //���ο��
		string strType;					//����
		string strPosition;				//λ��		
		string strCenterObject;			//���Ķ���		
		string strObject;				//����
        string NumObjectName;           //����������������
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
        string paramname;               //���������
	};
	//�ı�״̬
	struct tagChangeBuff :public tagBaseChange
	{
		DWORD dwID;						//״̬ID
		DWORD dwLevel;					//�ȼ�		
		DWORD dwCount;					//����
        string strSIDName;				//ID������		
        string strSLvName;				//�ȼ�������	
        string strSCountName;			//����������
		string strOperator;				//������
        string strSkillType;            //��������
	};

	//λ���޸�
	struct tagChangePos :public tagBaseChange
	{		
		string strRefObject;			//���λ��
		string strDir;					//����
		string strStyle;				//��ʽ
		string strSpeed;					//�ƶ��ٶ�
        float dis;
	};

	//��ȴʱ��
	struct tagCoolDownChange :public tagBaseChange
	{
		DWORD  dwValue;					//ֵ	
		string strType;					//����	
		string strSVarianame;			//�ű�������
        string paramname;               //������
	};
	//ע��ʱ��
	struct tagRegisterTimer :public tagBaseChange
	{
		DWORD  dwFlag;					//����	
		DWORD  dwIntelTime;				//ʱ��		
		DWORD  dwFuture;				//����ʱ��		
		DWORD  dwCount;					//����
		string strSIntelName;		    //���ʱ��ű�����
		string strSFutvarname;			//����ʱ��ű�����
		string strSCountName;           //��ʱ�����ű�����
        string strVFuturename;          //���ʱ�����
		string strVIntername;           //����ʱ�����
        string strVCountname;           //��������
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
        string strDamagename;           //�˺�������
	};	
	//do �˺�
	struct tagDoDamage :public tagBaseChange
	{
		DWORD dwResult;					//�˺����
		DWORD dwBlastAttack;			//����ֵ
		string strSResVariName;			//�˺�����Ľű�����
		string strSBlastVariName;		//�����ű�����
        string strDamagename;           //�˺�������
        string strBlastname;            //����������
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
    //�����츳
    struct tagGeniusChange:public tagBaseChange
    {
        string strGenius;
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
		tagStepConditon					tgCondition;	  //����
		list<tagDoProcess*>				ltgTrueDoProcess; //�����
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

	struct tagSkill
	{
		DWORD					 dwTemplateId;					//����Complateid	
		map<DWORD,tagCondition*> mapCondition;					//������ 	
		list<tagProcess*>		 ltgExecuteStep;				//�׶�	
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
    string                          m_strAttType;               //���ܹ�������
    DWORD                           m_CDControl;                //CD����ʱ��
	uchar							m_bUseType;					//���
	string							m_strAddSub;				//����ͼ���ı�־��Add��ʾ����Sub��ʾ����	
	uchar							m_bOnly;					//Ψһ��־��1��Ψһ0���Ψһ
	DWORD							m_dwOnlyType;				//Ψһ����
	uchar							m_bNameOnly;				//ͬ��Ψһ��־��
	uchar							m_bRoleOnly;				//��ɫΨһ��־��
	uchar							m_bAttack;					//�����ͷǹ������ܱ�־��1��ʾ����0��ʾ�ǹ���
	uchar							m_bTarget;					//Ŀ���־��
	uchar							m_bCooldown;				//��ȴ�����־��1�Ǳ���0������
	uchar							m_bDeadUse;					//�ܷ������Ŀ��ʹ�ü��ܱ�־��0��ʾ����ʹ��1��ʾ����ʹ��
	uchar							m_bDeadEnd;					//�����Ƿ����״̬0��ʾ����1��ʾ����
	uchar							m_bBreak;					//�жϼ���ʩ�ű�־��(1��ʾ�����ж�0��ʾ���ж�)
	uchar							m_bPetType;					//���＼������
	uchar							m_bTime;					//״̬��ʱ��־��(1��ʾ��������״̬��ʱ0��ʾ������״̬����ʱ)
	uchar							m_bSystem;					//ϵͳ״̬��־��
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
    void    SetSkillAttType     ( string strAttType)                                    { m_strAttType = strAttType;}           //���ü��ܹ�������
    void    SetCDControl        ( DWORD  Time)                                          { m_CDControl  = Time;      }           //CD����ʱ��
	void	SetUseType			( uchar bUseType)										{ m_bUseType = bUseType; }				//����ģ�����							
	void	SetOnly				( uchar bOnly)											{ m_bOnly = bOnly;}						//����Ψһ��־��						
	void	SetOnlyType			( DWORD dwOnlyType)										{ m_dwOnlyType = dwOnlyType;}			//����Ψһ����							
	void	SetNameOnly			( uchar bNameOnly)										{ m_bNameOnly = bNameOnly;}				//����Ψһ��־��						
	void	SetRoleOnly			( uchar bRoleOnly)										{ m_bRoleOnly = bRoleOnly;}				//��ɫΨһ��־��						
	void	SetTotalNum			( DWORD dwNum)											{ m_dwTotalNum = dwNum;}				//�ۼ�����																			
	void	SetAddSub			( string str)											{ m_strAddSub = str;}					//��������
	void	SetAttack			( uchar bAttack)										{ m_bAttack = bAttack;}					//�������ܱ�־��
	void	SetTarget			( uchar bTarget)										{ m_bTarget = bTarget;}					//Ŀ���־��	
	void	SetCoolDown			( uchar bCooldown)										{ m_bCooldown = bCooldown;}				//��ȴʱ��
	void	SetDeadUse			( uchar bDeadUse)										{ m_bDeadUse = bDeadUse;}				//������Ŀ��ʹ�ü��ܱ�־��
	void	SetDeadEnd			( uchar bDeadEnd)										{ m_bDeadEnd = bDeadEnd;}
	void	SetBreak			( uchar bBreak)											{ m_bBreak	= bBreak;}					//�жϱ�־��
	void	SetPetType			( uchar bType)											{ m_bPetType = bType;}					//���＼������
	void	SetTime				( uchar bTime)											{ m_bTime	= bTime;}					//��ʱ��־��
	void	SetSystem			( uchar bSystem)											{ m_bSystem = bSystem;}					//ϵͳ��־��

	void	SetFormat			( DWORD dwID, string& strFileName,string& strContend); //�ű�
	void    SetOperateObject	( DWORD dwObjectID,tagOperateObject *ptgOperateObject)	{m_mapOperateObject[dwObjectID] = ptgOperateObject;}							//���ö���

	void    AddNewSkill			( tagSkill *pSkill);
	void	SetCondition		( DWORD dwID,tagCondition *tgCondtition);
	void	SetConGroup			( DWORD dwGroupID,tagConditionGroup *ptgConditionGroup);		
	
	map<DWORD, tagOperateObject*>	GetOperateObject	()  							{return m_mapOperateObject;}												
	map<DWORD,tagExpression>&		GetFormat			()								{return m_mapFormat;}		
	map<DWORD,tagCondition*>		GetCondition		()								{return m_mapCondition;}	 	
	map<DWORD,tagConditionGroup*>	GetConditionGroup	()								{return m_mapConditionGroup;}	
	string							GetSkillType		()								{return m_strType;}
    string                          GetSkillAttType     ()                              {return m_strAttType;}
    DWORD                           GetCDControl        ()                              {return m_CDControl; }
	DWORD							GetNewSkillID		()								{return m_dwID;}
	uchar							GetOnly				()								{return m_bOnly;}
	DWORD							GetOnlyType			()								{return m_dwOnlyType;}	
	//uchar							GetSkillAdd			()								{return m_bSkillAdd;}
	string							GetAddSub			()								{return m_strAddSub;}
	uchar							GetUseType			()								{return m_bUseType;}
	string							GetInitScriptName	()								{return m_strInitSricpt;}
	int								GetElement			()								{return m_dwElement;}
	uchar							GetNameOnly			()								{return m_bNameOnly;}
	uchar							GetRoleOnly			()								{return m_bRoleOnly;}
	DWORD							GetTotalNum			()								{return m_dwTotalNum;}
	uchar							GetAttack			()								{return m_bAttack;}
	uchar							GetTarget			()								{return m_bTarget;}
	uchar							GetCoolDown			()								{return m_bCooldown;}
	uchar							GetDeadUse			()								{return m_bDeadUse;}
	uchar							GetDeadEnd			()								{return m_bDeadEnd;}
	uchar							GetBreak			()								{return m_bBreak;}
	uchar							GetPetType			()								{return m_bPetType;}
	uchar							GetTime				()								{return m_bTime;}
	uchar							GetSystem			()								{return m_bSystem;}

	void	ClearCDo(tagCDo* ptgCDo);																					//�ͷ�CDo	
	void	ClearDo(tagBaseChange *ptgBaseChange);
	void	ClearCondition( void *pCondition,string strType);											//�ͷ�����
};

#endif//_INC_NEWSKILLLIST_INCLUDED
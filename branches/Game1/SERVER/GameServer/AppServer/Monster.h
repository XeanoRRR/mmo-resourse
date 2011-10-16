#pragma once
#include "moveshape.h"
#include "setup\monsterlist.h"
#include "..\..\..\public\monsterdef.h"
#include "MasterInfo.h"
#include <deque>
#include "..\..\..\public\\MultiHpBar.h"

class CMonsterTaming;
class CVariableList;

//��������
enum SKILLTYPE{SKILLTYPE_BASE=0,SKILLTYPE_NORMAL,SKILLTYPE_CYCLE,};

class CMonster :
	public CMoveShape
{
public:
	CMonster(void);
	virtual ~CMonster(void);

public:

	// ����
	struct stProperty
	{
		string			strOrginName;		//	ԭʼ����
		DWORD			dwHP;				//	��ǰ����ֵ

		WORD			wSign;				//  ���ڵ�ͼ��Ψһ��ʶ
		WORD			wLeaderSign;		//  �쵼��Ψһ��ʶ
		WORD			wLeaderDistance;	//  ���쵼�߱��ֵ�������
		long			lLeaderType;		//	�쵼��type
		long			lLeaderID;			//	�쵼��id

		long			lLiveTime;			//	����ʱ�䣨-1����һֱ���棬����ʱ����λ���룩
		bool			bDiedRemove;		//	������ɾ����������
		long			lRefeashIndex;		//	ˢ�±��(��ӦRegion�ϵ�tagMonster)
	};
	struct tagSkill
	{
		WORD	wID;					//���
		WORD	wLevel;					//�ȼ�
	};

	struct tagSkillRecord
	{
		tagSkillRecord(){wID=0; wLevel=0; dwCool=0;dwCoolDownTime=0;iActModify=0;}
		WORD wID;
		WORD wLevel;
		DWORD dwCool;
		DWORD dwCoolDownTime;
		int iActModify;
	};

public:
	const char* GetOriginName(void) { return m_strOrginName.c_str(); }
	void SetOriginName(const char* name) { m_strOrginName = name; }
	const char* GetTitleName(void)	{return m_strTitleName.c_str();}
private:
	string			m_strOrginName;		//	ԭʼ����
	string			m_strTitleName;		//	��ν��
    map<string, tagPty>		    m_mapNameValue;		// �������ֶ�Ӧ�ı�����ַ
	typedef map<string, tagPty>::iterator mapNameValueItr; 

    // ���������� ����ͻ��˷���ʱ���»�ȡ��������
    set<string>  m_mapDirtyAttrData;
	// ��Χ�����Ҫ���µ�����
	static set<string>  m_mapOtherUpdateDirtyAttrData;
    void    PushDirtyAttrData(string attrName) { m_mapDirtyAttrData.insert(attrName); }
    string  PopDirtyAttrData() 
    { 
        string tmp = *m_mapDirtyAttrData.begin(); 
        m_mapDirtyAttrData.erase(m_mapDirtyAttrData.begin());
        return tmp; 
    }
    long    DirtyAttrDataSize(void) { return m_mapDirtyAttrData.size(); }

	tagMonsterProperty			m_tgMonsterProperty;			//��������
	tagMonsterFightProperty		m_tgMonsterFightProperty;		//����ս������
	tagMonsterFightProperty		m_tgBaseMonsterFightProperty;	//�������ս������
	tagPlayerElementProperty	m_arrElementProperty[MT_Max];	//Ԫ������
	


	string						m_strBornScript;				//�����ű�
	string						m_strFightScript;				//ս���ű�
	string						m_strGBHScript;					//���˽ű�
	string						m_strDeadScript;				//�����ű�
	string						m_strCycleScript;				//���ڴ����ű�
	string						m_strSearchEnemyScript;			//ѰĿ��ű�
	string						m_strFindTargetScript;			//�ҵ�Ŀ��ű�
	string						m_strKillTargetScript;			//ɱ��Ŀ��ű�

public:
	void InitNameValueMap();	// ��ʼ��
	static void InitUpdateDirtyAttrData();

    virtual long GetValue(const char* strName);
    virtual long SetValue(const char* strName, long dwValue);

	// ȡ�õ�ǰֵ�ĺϷ�ֵ
	virtual long GetCurrentValue(const tagPty& tagValue, long evalue/*�ı�ֵ*/, long bvalue/*����ֵ*/);

	void Talk(const char* strContent);
	void Talk(LONG TextType,LONG TextIndex);
	void Init(); //����ĳ�ʼ��
	virtual void InitAI();	//AI�ĳ�ʼ��
	void InitProperty(CMonsterList::tagMonster *p);  //��ʼ����������

	BOOL ChangeAI();
	VOID ResumeAll();
	BOOL Relive();

	void SetPosXY(float fX,float fY );
	void SetBlock(int x, int y, unsigned short block);
//��������ս����Ϣ
	void SendLeaveFightMessage(long lAimX,long lAimY,long lTime);
	void SendLeaveFightEndMessage();

	long OnTimeOut(DWORD timerid,DWORD curtime,const void* var);

//����Ķ�Ѫ������	
private:
	CMultiHpBar					m_MultiHpBar;
public:
	void AddHpBar(int HpRange, const char* ScripntName);
	int  GetHpLevel(int Hp);
	bool OnHpChanged(int Hp, int ChangeHp);
	void CleanHpBar(){m_MultiHpBar.Clean();}
	
//////////////////////////////////////////////////////////////////////////
//	�ӿ�
//////////////////////////////////////////////////////////////////////////
    // [070608 AHC]
    virtual bool  SetAttribute(std::string& attrName, long value);
	virtual bool  SetAttribute(char* attrName, long value);
    virtual long  GetAttribute(std::string attrName);
    virtual bool  ChangeAttribute(std::string& attrName, long value);
    virtual long  UpdateAttributeToClient();
	long  UpdateAttribute(BOOL bImmediately=0);
	long  OnUpdateAttributeTimeOut();
	//void          AddChangeAttrToMsgArray(std::vector<BYTE>* bArray);
	void          CodeChangeAttrToDataBlock(DBWriteSet& setWriteDB);
	//��ӱ�������������
	void  AddAttributeToDirty(string attrName);
public:
	virtual bool CodeToDataBlock(DBWriteSet& setWriteDB, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData = true);	// ����

	void SetLockAttackImmediately(bool b)	{m_bLockAttackImmediately=b;}

private:

	DWORD	m_dwAICount;
	bool	m_bLockAttackImmediately;	// ������������һ��

	//////////////////////////////////////////////////////////////////////////
	//	����
	//////////////////////////////////////////////////////////////////////////
private:
	typedef vector<tagSkill>::iterator itSkill;
	//���＼���б�
	vector<tagSkillRecord> m_vBaseSkills;
	//�����������
	vector<tagSkillRecord> m_vSkills;
	//�������ڹ̶������б�
	vector<tagSkillRecord> m_vCycleSkills;
public:
	virtual void AddSkill(CMonsterList::tagSkillInfo& info,SKILLTYPE SkillType);
	virtual bool DelSkill(long id);
	void CountAtkRange(vector<tagSkillRecord>& vSkills);
	void CountAtkRange(int iRagMax,int iRagMin);
	bool DelSkillOnList(vector<tagSkillRecord>& vSkills, long SkillID);
	long GetActModify(long SkillID);
public:
	vector<tagSkillRecord>& GetSkillList(){return m_vSkills;}
	vector<tagSkillRecord>& GetBaseSkillList(){return m_vBaseSkills;}
	vector<tagSkillRecord>& GetCycleSkillList(){return m_vCycleSkills;}
	VOID InitCycleSkill();

	// Fox@20081126 �������
private:
	DWORD									m_dwTameAttemptCount;

public:
	BOOL			IsTamable				();
	VOID			IncreaseTameAttemptCount();

//////////////////////////////////////////////////////////////////////////
//	ս��
//////////////////////////////////////////////////////////////////////////
public:
	//�����������	

	//��ν��ɫ
	virtual	WORD		GetTitleColor()						{return m_tgMonsterProperty.wTitleColor;}
	//��νͼƬid
	virtual	WORD		GetTitlePicID()						{return m_tgMonsterProperty.wTitlePicID;}
	//����ֵ
	virtual DWORD		GetExp()							{return  m_tgMonsterProperty.dwExp;}
	//ɱ���õ��ľ���ֵ
	virtual DWORD		GetExpDead()						{return m_tgMonsterProperty.dwExp;}	
	//ְҵ����
	virtual DWORD		GetOccuPoint()						{return m_tgMonsterProperty.dwOccuPoint;}
	//�����������
	virtual DWORD		GetIndex()							{return m_tgMonsterProperty.dwIndex;}
	//ͼ��ID
	virtual DWORD		GetPicID()							{return	m_tgMonsterProperty.dwPicID;}
	//ͼ�εȼ�
	virtual	DWORD		GetPicLevel()						{return m_tgMonsterProperty.dwPicLevel;}
	//������ɫ���,��Ӧ���ͻ�������
	virtual	DWORD		GetNameColor()						{return m_tgMonsterProperty.dwNameColor;}
	//�Ƿ����赲
	virtual BYTE		GetBlockType()						{return m_tgMonsterProperty.byBlockType;}
	//Ѫ����ɫ��0����ʾ����ʾѪ�� 1����ɫ 2����ɫ
	virtual DWORD		GetHpBarColor()						{return m_tgMonsterProperty.dwHpBarColor;}
	//����ID
	virtual	DWORD		GetSounID()							{return m_tgMonsterProperty.dwSoundID;}
	//�ȼ�
	virtual	WORD		GetLevel()							{return m_tgMonsterProperty.wLevel;}
	//����ID
	virtual	BYTE		GetCountry()						{return m_tgMonsterProperty.byCountry;}
	//�Ƿ����ѱ��
	virtual	BYTE		GetTamable()						{return m_tgMonsterProperty.byTamable;}
	//�����ѱ������
	virtual	DWORD		GetMaxTameCount()					{return m_tgMonsterProperty.dwMaxTameCount;}
	//���ҹ���ֵ
	virtual	DWORD		GetCountryContriValue()				{return m_tgMonsterProperty.wCountryContriValue;}
	//SP����ֵ
	virtual	WORD		GetSPContriValue()					{return m_tgMonsterProperty.wSPContriValue;}
	//Ԫ�����
	virtual	eEleType	GetEleType()						{return m_tgMonsterProperty.eEType;}
	//Npc��� (NNPC,Monster,Guard)
	virtual	eNpcType	GetNpcType()						{return m_tgMonsterProperty.eNType;}
	//boss�ȼ�
	virtual WORD		GetBossLevel()						{return m_tgMonsterProperty.wBossLevel;}
	//����
	virtual	eClass		GetClass()							{return m_tgMonsterProperty.eClass_;}
	//��Ⱥ(AIʹ��)
	virtual	WORD		GetRace()							{return m_tgMonsterProperty.wRace;}
	//�����ƶ��ĸ���(����)
	virtual	WORD		GetMoveRandomValue()				{return m_tgMonsterProperty.wMoveRandomValue;}
	//ͣ����ʱ��(��һ�������м�ͣ��֡��)
	virtual	WORD		GetStopTime()						{return m_tgMonsterProperty.wStopTime;}
	//AI����ʽ
	virtual	WORD		GetAIHangupType()						{return m_tgMonsterProperty.wAIHangupType;}			
	//����(0:1x1 1:3*3 2:5*5)*
	virtual	WORD		GetFigure(eDIR dir=DIR_UP)			{return m_tgMonsterProperty.wFigure;}
	//��ǰ����ֵ
	virtual	DWORD		GetHP()								{return m_tgMonsterProperty.dwHp;}
	virtual WORD		GetMP() {return 0xFFFF;}
	//AI����
	virtual DWORD		GetAIType()							{return m_tgMonsterProperty.dwAIType;}
	//��Сͣ������
	virtual WORD		GetMinStopTimes()					{return m_tgMonsterProperty.wMinStopTimes;}	
		//���ͣ������
	virtual WORD		GetMaxStopTimes()					{return m_tgMonsterProperty.wMaxStopTimes;}
	//�������߲���
	virtual WORD		GetMinRunTimes()					{return m_tgMonsterProperty.wMinRunTimes;}
	//������߲���
	virtual WORD		GetMaxRunTimes()					{return m_tgMonsterProperty.wMaxRunTimes;}
	//��������
	virtual LONG		GetRefeashIndex()					{return m_tgMonsterProperty.lRefeashIndex;}
	//���˱���
	virtual float       GetHurtProportion()                 {return m_tgMonsterProperty.fHurtProportion;}
	//����ʱ��
	virtual DWORD       GetGuardTime()                      {return m_tgMonsterProperty.dwGuardTime;}
	//���ܵ���
	virtual float       GetEscapePoint()                    {return m_tgMonsterProperty.fEscapePoint;}
	//���ܸ���
	virtual WORD		GetSkillProportion()                {return m_tgMonsterProperty.wSkillProportion;}
	//���ܼ��
	virtual DWORD       GetSkillSpace()                     {return m_tgMonsterProperty.dwSkillSpace;}
	//ai�ο�
	virtual	DWORD       GetAIReferrence(int lpos)           {if(lpos<0 || lpos>=MONSTER_AI_REFERRENCE_NUM) return 0;
	                                                         return m_tgMonsterProperty.dwAIReferrence[lpos];}
	//ת��Ϊ��ƽʱ���Ƿ�ɻ�ȥ
	virtual BYTE		GetIsFlyToPeace()					{return m_tgMonsterProperty.byIsFlyToPeace;}
	//ת��Ϊ��ƽʱ���Ƿ���ȫ�ָ�
	virtual BYTE		GetIsResumeAll()					{return m_tgMonsterProperty.byIsResumeAll;}	
	virtual WORD		GetReturnRange()					{return m_tgMonsterProperty.wReturnRange;}
	virtual WORD		GetPeaceMoveRange()					{return m_tgMonsterProperty.wPeaceMoveRange;}
	virtual DWORD		GetChaseTime()						{return m_tgMonsterProperty.dwChaseTime;}

////////////////////////////////////////////////////////////////////////////////////////////
//���ù�������
////////////////////////////////////////////////////////////////////////////////////////////
	//��ν��ɫ
	virtual	void		SetTitleColor(WORD l)						{m_tgMonsterProperty.wTitleColor=l;}
	//��νͼƬid
	virtual	void		SetTitlePicID(WORD l)						{m_tgMonsterProperty.wTitlePicID=l;}
	//	����ֵ
	virtual void		SetExp(DWORD l)						{m_tgMonsterProperty.dwExp = l;}
	//ְҵ����
	virtual void		SetOccuPoint(DWORD l)				{m_tgMonsterProperty.dwOccuPoint = l;}
	//�����������
	virtual void		SetIndex(DWORD l)					{m_tgMonsterProperty.dwIndex = l;}
	//ͼ��ID
	virtual void		SetPicID(DWORD l)					{m_tgMonsterProperty.dwPicID = l;}
	//ͼ�εȼ�
	virtual	void		SetPicLevel(DWORD l)				{m_tgMonsterProperty.dwPicLevel = l;}
	//�����Ƿ����赲
	virtual void		SetBlockType(BYTE l)				{m_tgMonsterProperty.byBlockType = l;}
	//������ɫ���,��Ӧ���ͻ�������
	virtual	void		SetNameColor(DWORD l)				{m_tgMonsterProperty.dwNameColor = l;}
	//Ѫ����ɫ��0����ʾ����ʾѪ�� 1����ɫ 2����ɫ
	virtual void		SetHpBarColor(DWORD l)				{m_tgMonsterProperty.dwHpBarColor = l;}
	//����ID
	virtual	void		SetSounID(DWORD l)					{m_tgMonsterProperty.dwSoundID = l;}
	//�ȼ�
	virtual	void		SetLevel(WORD l)					{m_tgMonsterProperty.wLevel = l;}
	//����ID
	virtual	void		SetCountry(BYTE l)					{m_tgMonsterProperty.byCountry = l;}
	//�Ƿ����ѱ��
	virtual	void		SetTamable(BYTE l)					{m_tgMonsterProperty.byTamable = l;}
	//�����ѱ������
	virtual	void		SetMaxTameCount(DWORD l)			{m_tgMonsterProperty.dwMaxTameCount = l;}
	//���ҹ���ֵ
	virtual	void		SetCountryContriValue(DWORD l)		{m_tgMonsterProperty.wCountryContriValue = l;}
	//SP����ֵ
	virtual	void		SetSPContriValue(DWORD l)			{m_tgMonsterProperty.wSPContriValue = l;}
	//Ԫ�����
	virtual	void		SetEleType(eEleType l )				{m_tgMonsterProperty.eEType = l;}
	//Npc��� (NNPC,Monster,Guard)
	virtual	void		SetNpcType(eNpcType l)				{m_tgMonsterProperty.eNType = l;}
	//boss�ȼ�
	virtual void		SetBossLevel(WORD l)				{m_tgMonsterProperty.wBossLevel=l;}
	//����
	virtual	void		SetClass(eClass l)					{m_tgMonsterProperty.eClass_ = l;}
	//��Ⱥ(AIʹ��)
	virtual	void		SetRace(WORD l)						{m_tgMonsterProperty.wRace = l;}
	//�����ƶ��ĸ���(����)
	virtual	void		SetMoveRandomValue(WORD l)			{m_tgMonsterProperty.wMoveRandomValue = l;}
	//ͣ����ʱ��(��һ�������м�ͣ��֡��)
	virtual	void		SetStopTime(WORD l)					{m_tgMonsterProperty.wStopTime = l;}
	//AI����ʽ
	virtual void		SetAIHangupType(WORD l)				{m_tgMonsterProperty.wAIHangupType=l;}			
	//����(0:1x1 1:3*3 2:5*5)*
	virtual	void		SetFigure(WORD l)					{m_tgMonsterProperty.wFigure = l;}
	//��ǰ����ֵ
	virtual	void		SetHP(DWORD l)
	{
		m_tgMonsterProperty.dwHp= l;
		if(m_tgMonsterProperty.dwHp > GetMaxHP())
		{
			m_tgMonsterProperty.dwHp = GetMaxHP();
		}
	}
	//AI����
	virtual void		SetAIType(DWORD l)					{m_tgMonsterProperty.dwAIType = l;}
	//��Сͣ������
	virtual void		SetMinStopTimes(WORD l)				{m_tgMonsterProperty.wMinStopTimes=l;}		
	//���ͣ������
	virtual void		SetMaxStopTimes(DWORD l)			{m_tgMonsterProperty.wMaxStopTimes=l;}	
	//�������߲���
	virtual void		SetMinRunTimes(DWORD l)				{m_tgMonsterProperty.wMinRunTimes=l;}
	//������߲���
	virtual void		SetMaxRunTimes(DWORD l)				{m_tgMonsterProperty.wMaxRunTimes=l;}
	//��������
	virtual void		SetRefeashIndex(LONG l)				{m_tgMonsterProperty.lRefeashIndex=l;}
	//���˱���
	virtual void        SetHurtProportion(float l)          {m_tgMonsterProperty.fHurtProportion=l;}
	//����ʱ��
	virtual void        SetGuardTime(DWORD l)               {m_tgMonsterProperty.dwGuardTime=l;}
	//���ܵ���
	virtual void        SetEscapePoint(float l)             {m_tgMonsterProperty.fEscapePoint=l;}
	//���ܸ���
	virtual void		SetSkillProportion(WORD l)          {m_tgMonsterProperty.wSkillProportion=l;}
	//���ܼ��
	virtual void        SetSkillSpace(DWORD l)            {m_tgMonsterProperty.dwSkillSpace=l;}
	//ai�ο�
	virtual	void        SetAIReferrence(int lpos,DWORD l) {if(lpos<0 || lpos>=MONSTER_AI_REFERRENCE_NUM) return ;
	                                                          m_tgMonsterProperty.dwAIReferrence[lpos]=l;}
	//ת��Ϊ��ƽʱ���Ƿ�ɻ�ȥ
	virtual void		SetIsFlyToPeace(BYTE l)				{m_tgMonsterProperty.byIsFlyToPeace=l;}
	//ת��Ϊ��ƽ��ʱ���Ƿ���ȫ�ָ�
	virtual void		SetIsResumeAll(BYTE l)				{m_tgMonsterProperty.byIsResumeAll=l;}
	virtual void		SetReturnRange(WORD l)				{m_tgMonsterProperty.wReturnRange=l;}
	virtual	void		SetPeaceMoveRange(WORD l)			{m_tgMonsterProperty.wPeaceMoveRange=l;}
	virtual void		SetChaseTime(DWORD l)				{m_tgMonsterProperty.dwChaseTime=l;}
//////////////////////////////////////////////////////////////////////////

	virtual long	GetStopFrame(); 							// ��ȡ�ڸ�������ͣ��������
	virtual bool	IsAttackAble(CMoveShape* pAttacker);			// �ɷ񹥻�

	//////////////////////////////////////////////////////////////////////////
	//��ȡ����ս������
	//////////////////////////////////////////////////////////////////////////
	//�����������
	virtual long GetAckRangeMin()					{return m_tgMonsterFightProperty.wNeaAtkDis;}
	//��Զ��������
	virtual long GetAckRangeMax()					{return m_tgMonsterFightProperty.wFarAtkDis;}
	//���䷶Χ
	virtual long GetGuardRange()					{return m_tgMonsterFightProperty.wGuardRange;}
	//׷����Χ
	virtual long GetChaseRange()					{return m_tgMonsterFightProperty.wChaseRange;}	
	//��������
	virtual DWORD GetMaxHP()
	{
		long lMaxHp = m_tgMonsterFightProperty.dwMaxHp+m_tgBaseMonsterFightProperty.dwMaxHp;
		if(lMaxHp<0)	return 0;
		return lMaxHp;
	}	
	//�����ָ��ٶ�
	virtual WORD GetHpRecoverSpeed()				{return m_tgMonsterFightProperty.wHpRecoverSpeed;}
	//��С������
	virtual WORD GetMinAtk()						{return m_tgMonsterFightProperty.wMinAtk;}
	//��󹥻���
	virtual WORD GetMaxAtk()						{return m_tgMonsterFightProperty.wMaxAtk;}
	//��С����������
	virtual WORD GetMinMAtk()						{return m_tgMonsterFightProperty.wMinMAtk;}
	//�����������
	virtual WORD GetMaxMAtk()						{return m_tgMonsterFightProperty.wMaxMAtk;}
	//��������
	virtual WORD GetHit()							{return m_tgMonsterFightProperty.wHit;}
	//������
	virtual WORD GetDef()							{return m_tgMonsterFightProperty.lDef;}
	//ħ��������
	virtual WORD GetMDef()							{return m_tgMonsterFightProperty.lMDef;}
	//��������
	virtual WORD GetDodge()							{return m_tgMonsterFightProperty.wDodge;}
	//��
	virtual WORD GetBlock()							{return m_tgMonsterFightProperty.wBlock;}
	//�м�
	virtual WORD GetParry()							{return m_tgMonsterFightProperty.wParry;}	

	//�ƶ��ٶ�
	virtual float GetMoveSpeed()					{return m_tgMonsterProperty.fMoveSpeed;}
	//ս���ƶ��ٶ�
	virtual float GetFightMoveSpeed()				{return m_tgMonsterProperty.fFightMoveSpeed;}
	//ת���ٶ�
	virtual float GetTurningSpeed()					{return m_tgMonsterProperty.fTurningSpeed;}
	//��־����
	virtual long GetWillDef()						{return m_tgMonsterFightProperty.lWillDef;}
	//���ʿ���
	virtual long GetConstiDef()						{return m_tgMonsterFightProperty.lConstiDef;}
	//Debuff����
	virtual long GetDeBuffDef(WORD lpos)			{if(lpos<0) {lpos = 0;}else if((lpos+1)>DET_Max) lpos = DET_Max -1; return m_tgMonsterFightProperty.lDeBuffDef[lpos];}
	//����Ԫ�ؿ���
	virtual long GetElemDef(WORD lpos)				{if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1; return m_tgMonsterFightProperty.lElemDef[lpos];}
	//D�Ը��������˺�
	virtual WORD GetClassDam(WORD lpos)				{if(lpos<0) {lpos = 0;}else if((lpos+1)>C_Max) lpos = C_Max -1; return m_tgMonsterFightProperty.wClassDam[lpos];}
	//�Ը���Ԫ���˺�
	virtual WORD GetElemDam(WORD lpos)				{if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1; return m_tgMonsterFightProperty.wElemDam[lpos];}
	//��������
	virtual WORD GetCri()							{return m_tgMonsterFightProperty.wCri;}
	//�������˺�
	virtual WORD GetCriDamg()						{return m_tgMonsterFightProperty.wCriDamg;}
	//����������
	virtual long GetCriDef()						{return m_tgMonsterFightProperty.lCriDef;}
	//ħ��������
	virtual WORD GetMCri()							{return m_tgMonsterFightProperty.wMCri;}
	//ħ�������˺�
	virtual WORD GetMCriDamg()						{return m_tgMonsterFightProperty.wMCriDamg;}
	//ħ����������
	virtual long GetMCriDef()						{return m_tgMonsterFightProperty.lMCriDef;}
	//�Ƿ��ܶԻ�
	virtual WORD GetCanTalk()						{return m_tgMonsterProperty.wCanTalk;}

	//////////////////////////////////////////////////////////////////////////
	//���ù���ս������
	//////////////////////////////////////////////////////////////////////////
	
	//�����������
	virtual void SetNeaAtkDis(WORD l);					
	//��Զ��������
	virtual void SetFarAtkDis(WORD l);					
	//��������
	virtual void SetMaxHp(DWORD l);						
	//��С������
	virtual void SetMinAtk(WORD l);						
	//��󹥻���
	virtual void SetMaxAtk(WORD l);						
	//��С����������
	virtual void SetMinMAtk(WORD l);						
	//�����������
	virtual void SetMaxMAtk(WORD l);						
	//�����ָ��ٶ�
	virtual void SetHpRecoverSpeed(WORD l);
	//���䷶Χ
	virtual void SetGuardRange(WORD l);					
	//׷����Χ
	virtual void SetChaseRange(WORD l);					
	//��������
	virtual void SetHit(WORD l);							
	//������
	virtual void SetDef(long l)	;
	//ħ��������
	virtual void SetMDef(long l);
	//��������
	virtual void SetDodge(WORD l);						
	//��
	virtual void SetFightBlock(WORD l);					
	//�м�
	virtual void SetParry(WORD l);						
	//�ƶ��ٶ�
	virtual void SetMoveSpeed(float f);					
	//ս���ƶ��ٶ�
	virtual void SetFightMoveSpeed(float f);				
	//ת���ٶ�
	virtual void SetTurningSpeed(float f)					{m_tgMonsterProperty.fTurningSpeed=f;}
	//��־����
	virtual void SetWillDef(long l)	;
	//���ʿ���
	virtual void SetConstiDef(long l);
	//Debuff����
	virtual void SetDeBuffDef(WORD lpos,long l);			
  
	//����Ԫ�ؿ���
	virtual void SetElemDef(WORD lpos,long l);			

	//D�Ը��������˺�
	virtual void SetClassDam(WORD lpos,WORD l);	
 
	//�Ը���Ԫ���˺�
	virtual void SetElemDam(WORD lpos,WORD l);	
  
	//��������
	virtual void SetCri(WORD l);
	//�������˺�
	virtual void SetCriDamg(WORD l);
	//����������
	virtual void SetCriDef(long l);
	//ħ��������
	virtual void SetMCri(WORD l);
	//ħ�������˺�
	virtual void SetMCriDamg(WORD l);
	//ħ����������
	virtual void SetMCriDef(long l);
	//�Ƿ��ܶԻ�
	virtual void SetCanTalk(WORD l);


	//////////////////////////////////////////////////////////////////////////
	//��ȡ�������ս������
	//////////////////////////////////////////////////////////////////////////
	//�����������
	virtual long GetBaseNeaAtkDis()						{return m_tgBaseMonsterFightProperty.wNeaAtkDis;}
	//��Զ��������
	virtual long GetBaseFarAtkDis()						{return m_tgBaseMonsterFightProperty.wFarAtkDis;}
	//���䷶Χ
	virtual long GetBaseGuardRange()					{return m_tgBaseMonsterFightProperty.wGuardRange;}
	//׷����Χ
	virtual long GetBaseChaseRange()					{return m_tgBaseMonsterFightProperty.wChaseRange;}	
	//��������
	virtual long GetBaseMaxHp()							{return m_tgBaseMonsterFightProperty.dwMaxHp;}	
	//�����ָ��ٶ�
	virtual WORD GetBaseHpRecoverSpeed()				{return m_tgBaseMonsterFightProperty.wHpRecoverSpeed;}
	//��С������
	virtual WORD GetBaseMinAtk()						{return m_tgBaseMonsterFightProperty.wMinAtk;}
	//��󹥻���
	virtual WORD GetBaseMaxAtk()						{return m_tgBaseMonsterFightProperty.wMaxAtk;}
	//��С����������
	virtual WORD GetBaseMinMAtk()						{return m_tgBaseMonsterFightProperty.wMinMAtk;}
	//�����������
	virtual WORD GetBaseMaxMAtk()						{return m_tgBaseMonsterFightProperty.wMaxMAtk;}
	//��������
	virtual WORD GetBaseHit()							{return m_tgBaseMonsterFightProperty.wHit;}
	//������
	virtual WORD GetBaseDef()							{return m_tgBaseMonsterFightProperty.lDef;}
	//ħ��������
	virtual WORD GetBaseMDef()							{return m_tgBaseMonsterFightProperty.lMDef;}
	//��������
	virtual WORD GetBaseDodge()							{return m_tgBaseMonsterFightProperty.wDodge;}
	//��
	virtual WORD GetBaseBlock()							{return m_tgBaseMonsterFightProperty.wBlock;}
	//�м�
	virtual WORD GetBaseParry()							{return m_tgBaseMonsterFightProperty.wParry;}	
	//��־����
	virtual long GetBaseWillDef()						{return m_tgBaseMonsterFightProperty.lWillDef;}
	//���ʿ���
	virtual long GetBaseConstiDef()						{return m_tgBaseMonsterFightProperty.lConstiDef;}
	//Debuff����
	virtual long GetBaseDeBuffDef(WORD lpos)			{if(lpos<0) {lpos = 0;}else if((lpos+1)>DET_Max) lpos = DET_Max -1; return m_tgMonsterFightProperty.lDeBuffDef[lpos];}
	//����Ԫ�ؿ���
	virtual long GetBaseElemDef(WORD lpos)				{if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1; return m_tgMonsterFightProperty.lElemDef[lpos];}
	//D�Ը��������˺�
	virtual WORD GetBaseClassDam(WORD lpos)				{if(lpos<0) {lpos = 0;}else if((lpos+1)>C_Max) lpos = C_Max -1; return m_tgMonsterFightProperty.wClassDam[lpos];}
	//�Ը���Ԫ���˺�
	virtual WORD GetBaseElemDam(WORD lpos)				{if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1; return m_tgMonsterFightProperty.wElemDam[lpos];}
	//��������
	virtual WORD GetBaseCri()							{return m_tgBaseMonsterFightProperty.wCri;}
	//�������˺�
	virtual WORD GetBaseCriDamg()						{return m_tgBaseMonsterFightProperty.wCriDamg;}
	//����������
	virtual long GetBaseCriDef()						{return m_tgBaseMonsterFightProperty.lCriDef;}
	//ħ��������
	virtual WORD GetBaseMCri()							{return m_tgBaseMonsterFightProperty.wMCri;}
	//ħ�������˺�
	virtual WORD GetBaseMCriDamg()						{return m_tgBaseMonsterFightProperty.wMCriDamg;}
	//ħ����������
	virtual long GetBaseMCriDef()						{return m_tgBaseMonsterFightProperty.lMCriDef;}
	//�ܷ�Ի�
	//////////////////////////////////////////////////////////////////////////
	//���ù������ս������
	//////////////////////////////////////////////////////////////////////////
	//�����������
	virtual void SetBaseNeaAtkDis(WORD l)					{m_tgBaseMonsterFightProperty.wNeaAtkDis = l; }
	//��Զ��������
	virtual void SetBaseFarAtkDis(WORD l)					{m_tgBaseMonsterFightProperty.wFarAtkDis = l;}	
	//��������
	virtual void SetBaseMaxHp(DWORD l)						{m_tgBaseMonsterFightProperty.dwMaxHp = l; }		
	//��С������
	virtual void SetBaseMinAtk(WORD l)						{m_tgBaseMonsterFightProperty.wMinAtk = l; }
	//��󹥻���
	virtual void SetBaseMaxAtk(WORD l)						{m_tgBaseMonsterFightProperty.wMaxAtk = l;}
	//��С����������
	virtual void SetBaseMinMAtk(WORD l)						{m_tgBaseMonsterFightProperty.wMinMAtk = l;}
	//�����������
	virtual void SetBaseMaxMAtk(WORD l)						{m_tgBaseMonsterFightProperty.wMaxMAtk = l;}
	//�����ָ��ٶ�
	virtual void SetBaseHpRecoverSpeed(WORD l)				{m_tgBaseMonsterFightProperty.wHpRecoverSpeed = l;}

	//���䷶Χ
	virtual void SetBaseGuardRange(WORD l)					{m_tgBaseMonsterFightProperty.wGuardRange = l;}
	//׷����Χ
	virtual void SetBaseChaseRange(WORD l)					{m_tgBaseMonsterFightProperty.wChaseRange = l;}	
	//��������
	virtual void SetBaseHit(WORD l)							{m_tgBaseMonsterFightProperty.wHit = l;}
	//������
	virtual void SetBaseDef(long l)							{m_tgBaseMonsterFightProperty.lDef = l;}
	//ħ��������
	virtual void SetBaseMDef(long l)						{m_tgBaseMonsterFightProperty.lMDef = l;}
	//��������
	virtual void SetBaseDodge(WORD l)						{m_tgBaseMonsterFightProperty.wDodge = l;}
	//��
	virtual void SetBaseBlock(WORD l)						{m_tgBaseMonsterFightProperty.wBlock = l;}
	//�м�
	virtual void SetBaseParry(WORD l)						{m_tgBaseMonsterFightProperty.wParry = l;}	
	//����
	virtual void SetBaseImmunity( long value )				{m_tgBaseMonsterFightProperty.lImmunity = value; }
	//��͸
	virtual void SetBasePierce( short value )				{m_tgBaseMonsterFightProperty.wPierce = value; }
	//��־����
	virtual void SetBaseWillDef(long l)						{m_tgBaseMonsterFightProperty.lWillDef = l;}
	//���ʿ���
	virtual void SetBaseConstiDef(long l)					{m_tgBaseMonsterFightProperty.lConstiDef = l;}
	
	//Debuff����
	virtual void SetBaseDeBuffDef(WORD lpos,long l)			
    {
		if(lpos<0) {lpos = 0;}else if((lpos+1)>DET_Max) lpos = DET_Max -1;
        m_tgBaseMonsterFightProperty.lDeBuffDef[lpos] = l;
    }
	//����Ԫ�ؿ���
	virtual void SetBaseElemDef(WORD lpos,long l)	
    {
		if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1;
        m_tgBaseMonsterFightProperty.lElemDef[lpos] = l;
    }
	//D�Ը��������˺�
	virtual void SetBaseClassDam(WORD lpos,WORD l)	
    {
		if(lpos<0) {lpos = 0;}else if((lpos+1)>C_Max) lpos = C_Max -1;
        m_tgBaseMonsterFightProperty.wClassDam[lpos] = l;
    }
	//�Ը���Ԫ���˺�
	virtual void SetBaseElemDam(WORD lpos,WORD l)	
    {
		if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1;
        m_tgBaseMonsterFightProperty.wElemDam[lpos] = l;
    }
	//��������
	virtual void SetBaseCri(WORD l)							{m_tgBaseMonsterFightProperty.wCri = l;}
	//�������˺�
	virtual void SetBaseCriDamg(WORD l)						{m_tgBaseMonsterFightProperty.wCriDamg = l;}
	//����������
	virtual void SetBaseCriDef(long l)						{m_tgBaseMonsterFightProperty.lCriDef = l;}
	//ħ��������
	virtual void SetBaseMCri(WORD l)						{m_tgBaseMonsterFightProperty.wMCri = l;}
	//ħ�������˺�
	virtual void SetBaseMCriDamg(WORD l)					{m_tgBaseMonsterFightProperty.wMCriDamg = l;}
	//ħ����������
	virtual void SetBaseMCriDef(long l)						{m_tgBaseMonsterFightProperty.lMCriDef = l;}
	//�ű����ֵĻ�ȡ
	const string& GetBornScriptName()						{return m_strBornScript;}
	const string& GetFightScriptName()						{return m_strFightScript;}
	const string& GetGBHScriptName()						{return m_strGBHScript;}
	const string& GetDeadScriptName()						{return m_strDeadScript;}
	const string& GetCycleScript()							{return m_strCycleScript;}				//���ڴ����ű�
	const string& GetSearchEnemyScript()					{return m_strSearchEnemyScript;}//ѰĿ��ű�
	const string& GetFindTargetScript()						{return m_strFindTargetScript;}			//�ҵ�Ŀ��ű�
	const string& GetKillTargetScript()						{return m_strKillTargetScript;}			//ɱ��Ŀ��ű�

	virtual void OnDied();								// ��������	

	//##����ֵ�����ʵ�ֺ���
	LONG		CalculateExperienceQuota			( CPlayer* pPlayer, LONG lPosX, LONG lPosY );
	//����ְҵ������亯��
	LONG		CalculateOccuExpQuota				( CPlayer* pPlayer, LONG lPosX, LONG lPosY );
	LONG		CalculateOccuPointQuota				( CPlayer* pPlayer, LONG lPosX, LONG lPosY );
	//##��ȡ������
	CPlayer*	GetBeneficiary						();

	virtual VOID OnBeenHurted( LONG lAttackerType, const CGUID& AttackerGuid ); //���˺��Ĵ���

	DWORD GetAICount(){return m_dwAICount;}
	void SetAICount(DWORD dwCount){m_dwAICount=dwCount;}
	virtual void GetBeAttackedPoint(long lAttackerX,long lAttackerY,long& lTileX,long& lTileY);

//////////////////////////////////////////////////////////////////////////
//	�����ű�
//////////////////////////////////////////////////////////////////////////
public:
	const char* GetScriptFile()			{return m_strScriptFile.c_str();}
	void SetScriptFile(const char* str)	{m_strScriptFile=str;}

private:
	string m_strScriptFile;		// �ű��ļ���
//////////////////////////////////////////////////////////////////////////
//  ̸���ű�
//////////////////////////////////////////////////////////////////////////
public:
	const char* GetTalkScriptFile(){return m_strTalkScriptFile.c_str();}
	void SetTalkScriptFile(const char* str){m_strTalkScriptFile=str;}
private:
	string m_strTalkScriptFile;
//////////////////////////////////////////////////////////////////////////
//  ̸���ű�
//////////////////////////////////////////////////////////////////////////
public:
	void ExpAction(long ActID,long DesType=0,CGUID DesGUID=NULL_GUID);
////////////////////////////////////////////////////////////////////////////
////	������
////////////////////////////////////////////////////////////////////////////
public:
	void OnLoseBeneficiary();			//��ʧ�������¼�
	void StartRecordBeneficiary(CGUID guidFirst,OBJECT_TYPE typeFirst,DWORD time);				//��ʼ��¼������
	bool CheckDelete();
//------------------------------------------------------------------------
//	�̳���CMoveShape��Ҫʵ�������麯��
//------------------------------------------------------------------------
	virtual VOID			UpdateCurrentState()	{}

//---------------------------------------------------------------------
//  �ڳ�����ˢ��ʱ���Ƿ�����
//---------------------------------------------------------------------
public:
	virtual BOOL	BeenCollectedAll();
//Ϊnpc�ṩ
public:
		void SetShowList(bool b)		{m_bShowList=b;}
		bool IsShowList()				{return m_bShowList;}
private:
	bool m_bShowList;


//---------------------------------------------------------------------
//  ��Ⱥ
//---------------------------------------------------------------------
public:
	DWORD	GetColonyID(){return m_dwColonyID;}
	WORD	GetMemberType(){return m_wMemberType;}
	VOID	AddToColony(DWORD dwColonyID, WORD wMemberType);
	VOID	ExitColony();
	WORD	GetColonyState(){return m_bColonyState;}
	BOOL	HasColony(){return m_dwColonyID==0? FALSE:TRUE;}
	DWORD	GetColonyDistance(){return m_dwColonyDistance;}

	VOID	SetColonyDistance(DWORD dis){m_dwColonyDistance= dis;}
	VOID	SetColonyID(DWORD dwColonyID){m_dwColonyID= dwColonyID;}
	VOID	SetMemberType(WORD type){m_wMemberType= type;}
	VOID	SetColonyState(BOOL bState){m_bColonyState= bState;}
private:
	DWORD m_dwColonyID;
	DWORD m_dwColonyDistance;
	WORD  m_wMemberType;
	BOOL  m_bColonyState;
//�ı�����״̬
public:
	void						ChangeWeaknessState(bool l);

//---------------------------------------------------------------------
// ע����ʧ
//---------------------------------------------------------------------
public:
	// ���ؽӿ�
	virtual int SetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, const char* value);
	virtual int SetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, long lArray=0, double value=0);
	virtual const char* GetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName);
	virtual double GetScriptVarValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, long lArray);
	virtual int SetScriptGuidValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName, const CGUID& guid);
	virtual const CGUID& GetScriptGuidValue(CScript* scriptPtr/*���õĽű�ָ��*/, const char* varName);
	virtual CVariableList* GetVariableList();
private:
	CVariableList* m_pVariableList;
};

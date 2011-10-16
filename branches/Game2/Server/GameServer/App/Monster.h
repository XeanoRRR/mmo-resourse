#pragma once

#include "MoveShape.h"

#include "../../../Public/Common/MonsterDef.h"
#include "../../../Public/Common/MultiHpBar.h"
#include "../../../Public/Common/DeputyOccu.h"
#include "../../../Public/Setup/MonsterList.h"
#include "../../../Public/Setup/GoodsSetup.h"
#include "../../../Public/Setup/GlobalSetup.h"
#include "../../../Public/Setup/PlayerList.h"

class CMonsterTaming;
class CVariableList;
class TauntHandler;

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
    long    DirtyAttrDataSize(void) { return (long)m_mapDirtyAttrData.size(); }

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
	string						m_strCatchScript;				//����ű�

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
	void ResumeAll();
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

	//virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	//virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����

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
	//int		m_iBaseSkillNum;
public:
	virtual void AddSkill(CMonsterList::tagSkillInfo& info,SKILLTYPE SkillType);
	virtual bool DelSkill(long id);
	void CountAtkRange(vector<tagSkillRecord>& vSkills);
	void CountAtkRange(DisType ragMax,DisType ragMin);
	bool DelSkillOnList(vector<tagSkillRecord>& vSkills, long SkillID);
	long GetActModify(long SkillID);
public:
	vector<tagSkillRecord>& GetSkillList(){return m_vSkills;}
	vector<tagSkillRecord>& GetBaseSkillList(){return m_vBaseSkills;}
	vector<tagSkillRecord>& GetCycleSkillList(){return m_vCycleSkills;}
	void InitCycleSkill();

	// Fox@20081126 �������
private:
	//DWORD									m_dwTameAttemptCount;
	/// ����ĵ�ǰ������
	ulong m_dwCatchFastness;
	/// ���ﵱǰ�Ƿ񱻲����־
	bool m_bCatchFlag;

public:
	/// �ܷ񱻲���
	bool IsCatchable();
	//void IncreaseTameAttemptCount();
	/// ���¹���Ĳ�����
	void DecreaseCatchFastness(int catchFastness);
	/// ȡ�ù���ĵ�ǰ������
	ulong GetCatchFastness(void) {return m_dwCatchFastness;}
	/// ���ù������󲶻���
	void ResetMaxCatchFastness(void) {m_dwCatchFastness = GetMaxCatchFastness();}
	/// ȡ�ù����Ӧ���̴�ID
	virtual ulong GetServantID(void) {return m_tgMonsterProperty.dwServantID;}
	/// ���ò����־
	void SetCatchFlag(bool b) {m_bCatchFlag=b;}
	/// ȡ�ò����־
	bool GetCatchFlag(void){return m_bCatchFlag;}
	/// ȡ�ò���ű��趨
	const string& GetCacthScript() {return m_strCatchScript;}
private:
	/// ���ù����Ӧ�̴�ID
	virtual	void SetServantID(ulong l) {m_tgMonsterProperty.dwServantID = l;}

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
	//ɱ���õ�����Ϊ����ֵ
	virtual DWORD		GetDExpDead()						{return m_tgMonsterProperty.dwDExp;}
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
	//�Ƿ���Բ���
	virtual	BYTE		GetCatchable()						{return m_tgMonsterProperty.byCatchable;}
	//��󲶻���
	virtual	DWORD		GetMaxCatchFastness()				{return m_tgMonsterProperty.dwMaxCatchFastness;}
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
	//AI��϶
//	virtual	WORD		GetAIInterval()						{return m_tgMonsterProperty.wAIInterval;}
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
	virtual DWORD       GetSkillSpace()                   {return m_tgMonsterProperty.dwSkillSpace;}
	//ai�ο�
	virtual	DWORD       GetAIReferrence(int lpos)         {if(lpos<0 || lpos>=MONSTER_AI_REFERRENCE_NUM) return 0;
	                                                         return m_tgMonsterProperty.dwAIReferrence[lpos];}
	//ת��Ϊ��ƽʱ���Ƿ�ɻ�ȥ
	virtual BYTE		GetIsFlyToPeace()					{return m_tgMonsterProperty.byIsFlyToPeace;}
	//ת��Ϊ��ƽʱ���Ƿ���ȫ�ָ�
	virtual BYTE		GetIsResumeAll()					{return m_tgMonsterProperty.byIsResumeAll;}	
	virtual DisType		GetReturnRange()					{return m_tgMonsterProperty.ReturnRange;}
	virtual DisType		GetPeaceMoveRange()					{return m_tgMonsterProperty.PeaceMoveRange;}
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
	//  ��Ϊ����ֵ
	virtual void		SetDExp(DWORD l)					{m_tgMonsterProperty.dwDExp = l;}
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
	//�Ƿ���Բ���
	virtual	void		SetCatchable(BYTE l)					{m_tgMonsterProperty.byCatchable = l;}
	//��󲶻���
	virtual	void		SetMaxCatchFastness(DWORD l)			{m_tgMonsterProperty.dwMaxCatchFastness = l;}
	//���ҹ���ֵ
	virtual	void		SetCountryContriValue(DWORD l)		{m_tgMonsterProperty.wCountryContriValue = (ushort)l;}
	//SP����ֵ
	virtual	void		SetSPContriValue(DWORD l)			{m_tgMonsterProperty.wSPContriValue = (ushort)l;}
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
	//AI��϶
//	virtual	void		SetAIInterval(WORD l)				{m_tgMonsterProperty.wAIInterval = l;}
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
	virtual void		SetMaxStopTimes(DWORD l)			{m_tgMonsterProperty.wMaxStopTimes=(ushort)l;}	
	//�������߲���
	virtual void		SetMinRunTimes(DWORD l)				{m_tgMonsterProperty.wMinRunTimes=(ushort)l;}
	//������߲���
	virtual void		SetMaxRunTimes(DWORD l)				{m_tgMonsterProperty.wMaxRunTimes=(ushort)l;}
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
	virtual void		SetReturnRange(DisType l)				{m_tgMonsterProperty.ReturnRange=l;}
	virtual	void		SetPeaceMoveRange(DisType l)			{m_tgMonsterProperty.PeaceMoveRange=l;}
	virtual void		SetChaseTime(DWORD l)				{m_tgMonsterProperty.dwChaseTime=l;}
//////////////////////////////////////////////////////////////////////////

	virtual long	GetStopFrame(); 							// ��ȡ�ڸ�������ͣ��������
	virtual bool	IsAttackAble(CMoveShape* pAttacker);			// �ɷ񹥻�

	//////////////////////////////////////////////////////////////////////////
	//��ȡ����ս������
	//////////////////////////////////////////////////////////////////////////
	//�����������
	virtual DisType GetAtkRangeMin()					{return m_tgMonsterFightProperty.NeaAtkDis;}
	//��Զ��������
	virtual DisType GetAtkRangeMax()					{return m_tgMonsterFightProperty.FarAtkDis;}
	//������Χ(�Թ���Ŀ��Ϊ����0:1x1 1:3*3 2:5*5)
//	virtual long GetFightRange()					{return m_tgMonsterFightProperty.wAtkRange;}
	//���䷶Χ
	virtual DisType GetGuardRange()					{return m_tgMonsterFightProperty.GuardRange;}
	//׷����Χ
	virtual DisType GetChaseRange()					{return m_tgMonsterFightProperty.ChaseRange;}	
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
	virtual WORD GetDef()							{return m_tgMonsterFightProperty.wDef;}
	//ħ��������
	virtual WORD GetMDef()							{return m_tgMonsterFightProperty.wMDef;}
	//��������
	virtual WORD GetDodge()							{return m_tgMonsterFightProperty.wDodge;}
	//��
	virtual WORD GetBlock()							{return m_tgMonsterFightProperty.wBlock;}
	//�м�
	virtual WORD GetParry()							{return m_tgMonsterFightProperty.wParry;}	
	//�����ٶ�
//	virtual WORD GetAtcSpeed()						{return m_tgMonsterFightProperty.wAtcSpeed;}
	//�ƶ��ٶ�
	virtual float GetMoveSpeed()					{return m_tgMonsterProperty.fMoveSpeed;}
	//ս���ƶ��ٶ�
	virtual float GetFightMoveSpeed()				{return m_tgMonsterProperty.fFightMoveSpeed;}
	//ת���ٶ�
	virtual float GetTurningSpeed()					{return m_tgMonsterProperty.fTurningSpeed;}
	//��־����
	virtual WORD GetWillDef()						{return m_tgMonsterFightProperty.wWillDef;}
	//���ʿ���
	virtual WORD GetConstiDef()						{return m_tgMonsterFightProperty.wConstiDef;}
	//Debuff����
	virtual WORD GetDeBuffDef(WORD lpos)			{if(lpos<0) {lpos = 0;}else if((lpos+1)>DET_Max) lpos = DET_Max -1; return m_tgMonsterFightProperty.wDeBuffDef[lpos];}
	//����Ԫ�ؿ���
	virtual WORD GetElemDef(WORD lpos)				{if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1; return m_tgMonsterFightProperty.wElemDef[lpos];}
	//D�Ը��������˺�
	virtual WORD GetClassDam(WORD lpos)				{if(lpos<0) {lpos = 0;}else if((lpos+1)>C_Max) lpos = C_Max -1; return m_tgMonsterFightProperty.wClassDam[lpos];}
	//�Ը���Ԫ���˺�
	virtual WORD GetElemDam(WORD lpos)				{if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1; return m_tgMonsterFightProperty.wElemDam[lpos];}
	//��������
	virtual WORD GetCri()							{return m_tgMonsterFightProperty.wCri;}
	//�������˺�
	virtual WORD GetCriDamg()						{return m_tgMonsterFightProperty.wCriDamg;}
	//����������
	virtual WORD GetCriDef()						{return m_tgMonsterFightProperty.wCriDef;}
	//ħ��������
	virtual WORD GetMCri()							{return m_tgMonsterFightProperty.wMCri;}
	//ħ�������˺�
	virtual WORD GetMCriDamg()						{return m_tgMonsterFightProperty.wMCriDamg;}
	//ħ����������
	virtual WORD GetMCriDef()						{return m_tgMonsterFightProperty.wMCriDef;}
	//�Ƿ��ܶԻ�
	virtual WORD GetCanTalk()						{return m_tgMonsterProperty.wCanTalk;}

	//////////////////////////////////////////////////////////////////////////
	//���ù���ս������
	//////////////////////////////////////////////////////////////////////////
	
	//�����������
	virtual void SetNearAtkDis(DisType l);//					{m_mapDirtyAttrData.push_back("wNeaAtkDis");m_tgMonsterFightProperty.wNeaAtkDis = l;}
	//��Զ��������
	virtual void SetFarAtkDis(DisType l);//					{m_mapDirtyAttrData.push_back("wFarAtkDis");m_tgMonsterFightProperty.wFarAtkDis = l;}	
	//��������
	virtual void SetMaxHp(DWORD l);//						{m_mapDirtyAttrData.push_back("dwMaxHp");m_tgMonsterFightProperty.dwMaxHp = l; }		
	//��С������
	virtual void SetMinAtk(WORD l);//						{m_mapDirtyAttrData.push_back("wMinAtk");m_tgMonsterFightProperty.wMinAtk = l; }
	//��󹥻���
	virtual void SetMaxAtk(WORD l);//						{m_mapDirtyAttrData.push_back("wMaxAtk");m_tgMonsterFightProperty.wMaxAtk = l;}
	//��С����������
	virtual void SetMinMAtk(WORD l);//						{m_mapDirtyAttrData.push_back("wMinMAtk");m_tgMonsterFightProperty.wMinMAtk = l;}
	//�����������
	virtual void SetMaxMAtk(WORD l);//						{m_mapDirtyAttrData.push_back("wMaxMAtk");m_tgMonsterFightProperty.wMaxMAtk = l; }
	//�����ָ��ٶ�
	virtual void SetHpRecoverSpeed(WORD l);//				{m_mapDirtyAttrData.push_back("wHpRecoverSpeed");m_tgMonsterFightProperty.wHpRecoverSpeed = l;}
	//������Χ(�Թ���Ŀ��Ϊ����0:1x1 1:3*3 2:5*5)
//	virtual void SetAtkRange(WORD l);//					{m_mapDirtyAttrData.push_back("wAtkRange");m_tgMonsterFightProperty.wAtkRange = l;}
	//���䷶Χ
	virtual void SetGuardRange(DisType l);//					{m_mapDirtyAttrData.push_back("wGuardRange");m_tgMonsterFightProperty.wGuardRange = l;}
	//׷����Χ
	virtual void SetChaseRange(DisType l);//					{m_mapDirtyAttrData.push_back("wChaseRange");m_tgMonsterFightProperty.wChaseRange = l;}	
	//��������
	virtual void SetHit(WORD l);//							{m_mapDirtyAttrData.push_back("wHit");m_tgMonsterFightProperty.wHit = l;}
	//������
	virtual void SetDef(WORD l)	;//						{m_mapDirtyAttrData.push_back("wDef");m_tgMonsterFightProperty.wDef = l;}
	//ħ��������
	virtual void SetMDef(WORD l);//
	//��������
	virtual void SetDodge(WORD l);//						{m_mapDirtyAttrData.push_back("wDodge");m_tgMonsterFightProperty.wDodge = l;}
	//��
	virtual void SetFightBlock(WORD l);//						{m_mapDirtyAttrData.push_back("wBlock");m_tgMonsterFightProperty.wBlock = l;}
	//�м�
	virtual void SetParry(WORD l);//						{m_mapDirtyAttrData.push_back("wParry");m_tgMonsterFightProperty.wParry = l;}	
	//�����ٶ�
	//virtual void SetAtcSpeed(WORD l);//					{m_mapDirtyAttrData.push_back("wAtcSpeed");m_tgMonsterFightProperty.wAtcSpeed = l;}
	//�ƶ��ٶ�
	virtual void SetMoveSpeed(float f);//					{m_mapDirtyAttrData.push_back("wMoveSpeed");m_tgMonsterFightProperty.wMoveSpeed = l;}
	//ս���ƶ��ٶ�
	virtual void SetFightMoveSpeed(float f);//				{m_mapDirtyAttrData.push_back("wFightMoveSpeed");m_tgMonsterFightProperty.wFightMoveSpeed = l;}
	//ת���ٶ�
	virtual void SetTurningSpeed(float f)					{m_tgMonsterProperty.fTurningSpeed=f;}
	//��־����
	virtual void SetWillDef(WORD l)	;//					{m_mapDirtyAttrData.push_back("wWillDef");m_tgMonsterFightProperty.wWillDef = l;}
	//���ʿ���
	virtual void SetConstiDef(WORD l);//					{m_mapDirtyAttrData.push_back("wConstiDef");m_tgMonsterFightProperty.wConstiDef = l;}
	//Debuff����
	virtual void SetDeBuffDef(WORD lpos,WORD l);			

	//����Ԫ�ؿ���
	virtual void SetElemDef(WORD lpos,WORD l);			

	//D�Ը��������˺�
	virtual void SetClassDam(WORD lpos,WORD l);	

	//�Ը���Ԫ���˺�
	virtual void SetElemDam(WORD lpos,WORD l);	

	//��������
	virtual void SetCri(WORD l);//							{m_tgMonsterFightProperty.wCri = l;}
	//�������˺�
	virtual void SetCriDamg(WORD l);
	//����������
	virtual void SetCriDef(WORD l);//						{m_tgMonsterFightProperty.wCriDef = l;}
	//ħ��������
	virtual void SetMCri(WORD l);
	//ħ�������˺�
	virtual void SetMCriDamg(WORD l);
	//ħ����������
	virtual void SetMCriDef(WORD l);//						{m_tgMonsterFightProperty.wMCriDef = l;}
	//�Ƿ��ܶԻ�
	virtual void SetCanTalk(WORD l);


	//////////////////////////////////////////////////////////////////////////
	//��ȡ�������ս������
	//////////////////////////////////////////////////////////////////////////
	//�����������
	virtual DisType GetBaseNearAtkDis()						{return m_tgBaseMonsterFightProperty.NeaAtkDis;}
	//��Զ��������
	virtual DisType GetBaseFarAtkDis()						{return m_tgBaseMonsterFightProperty.FarAtkDis;}
	//������Χ(�Թ���Ŀ��Ϊ����0:1x1 1:3*3 2:5*5)
//	virtual long GetBaseAtkRange()						{return m_tgBaseMonsterFightProperty.wAtkRange;}
	//���䷶Χ
	virtual DisType GetBaseGuardRange()					{return m_tgBaseMonsterFightProperty.GuardRange;}
	//׷����Χ
	virtual DisType GetBaseChaseRange()					{return m_tgBaseMonsterFightProperty.ChaseRange;}	
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
	virtual WORD GetBaseDef()							{return m_tgBaseMonsterFightProperty.wDef;}
	//ħ��������
	virtual WORD GetBaseMDef()							{return m_tgBaseMonsterFightProperty.wMDef;}
	//��������
	virtual WORD GetBaseDodge()							{return m_tgBaseMonsterFightProperty.wDodge;}
	//��
	virtual WORD GetBaseBlock()							{return m_tgBaseMonsterFightProperty.wBlock;}
	//�м�
	virtual WORD GetBaseParry()							{return m_tgBaseMonsterFightProperty.wParry;}	
	//�����ٶ�
//	virtual WORD GetBaseAtcSpeed()						{return m_tgBaseMonsterFightProperty.wAtcSpeed;}
	//�ƶ��ٶ�
	//virtual WORD GetBaseMoveSpeed()						{return m_tgBaseMonsterFightProperty.wMoveSpeed;}
	//ս���ƶ��ٶ�
	//virtual WORD GetBaseFightMoveSpeed()				{return m_tgBaseMonsterFightProperty.wFightMoveSpeed;}
	//��־����
	virtual WORD GetBaseWillDef()						{return m_tgBaseMonsterFightProperty.wWillDef;}
	//���ʿ���
	virtual WORD GetBaseConstiDef()						{return m_tgBaseMonsterFightProperty.wConstiDef;}
	//Debuff����
	virtual WORD GetBaseDeBuffDef(WORD lpos)			{if(lpos<0) {lpos = 0;}else if((lpos+1)>DET_Max) lpos = DET_Max -1; return m_tgMonsterFightProperty.wDeBuffDef[lpos];}
	//����Ԫ�ؿ���
	virtual WORD GetBaseElemDef(WORD lpos)				{if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1; return m_tgMonsterFightProperty.wElemDef[lpos];}
	//D�Ը��������˺�
	virtual WORD GetBaseClassDam(WORD lpos)				{if(lpos<0) {lpos = 0;}else if((lpos+1)>C_Max) lpos = C_Max -1; return m_tgMonsterFightProperty.wClassDam[lpos];}
	//�Ը���Ԫ���˺�
	virtual WORD GetBaseElemDam(WORD lpos)				{if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1; return m_tgMonsterFightProperty.wElemDam[lpos];}
	//��������
	virtual WORD GetBaseCri()							{return m_tgBaseMonsterFightProperty.wCri;}
	//�������˺�
	virtual WORD GetBaseCriDamg()						{return m_tgBaseMonsterFightProperty.wCriDamg;}
	//����������
	virtual WORD GetBaseCriDef()						{return m_tgBaseMonsterFightProperty.wCriDef;}
	//ħ��������
	virtual WORD GetBaseMCri()							{return m_tgBaseMonsterFightProperty.wMCri;}
	//ħ�������˺�
	virtual WORD GetBaseMCriDamg()						{return m_tgBaseMonsterFightProperty.wMCriDamg;}
	//ħ����������
	virtual WORD GetBaseMCriDef()						{return m_tgBaseMonsterFightProperty.wMCriDef;}
	//�ܷ�Ի�
	//virtual	WORD GetBaseCanTalk()						{return m_tgBaseMonsterFightProperty.wCanTalk;}
	//////////////////////////////////////////////////////////////////////////
	//���ù������ս������
	//////////////////////////////////////////////////////////////////////////
	//�����������
	virtual void SetBaseNearAtkDis(DisType l)					{m_tgBaseMonsterFightProperty.NeaAtkDis = l; }
	//��Զ��������
	virtual void SetBaseFarAtkDis(DisType l)					{m_tgBaseMonsterFightProperty.FarAtkDis = l;}	
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
	//������Χ(�Թ���Ŀ��Ϊ����0:1x1 1:3*3 2:5*5)
//	virtual void SetBaseAtkRange(WORD l)					{m_tgBaseMonsterFightProperty.wAtkRange = l;}
	//���䷶Χ
	virtual void SetBaseGuardRange(DisType l)					{m_tgBaseMonsterFightProperty.GuardRange = l;}
	//׷����Χ
	virtual void SetBaseChaseRange(DisType l)					{m_tgBaseMonsterFightProperty.ChaseRange = l;}	
	//��������
	virtual void SetBaseHit(WORD l)							{m_tgBaseMonsterFightProperty.wHit = l;}
	//������
	virtual void SetBaseDef(WORD l)							{m_tgBaseMonsterFightProperty.wDef = l;}
	//ħ��������
	virtual void SetBaseMDef(WORD l)						{m_tgBaseMonsterFightProperty.wMDef = l;}
	//��������
	virtual void SetBaseDodge(WORD l)						{m_tgBaseMonsterFightProperty.wDodge = l;}
	//��
	virtual void SetBaseBlock(WORD l)						{m_tgBaseMonsterFightProperty.wBlock = l;}
	//�м�
	virtual void SetBaseParry(WORD l)						{m_tgBaseMonsterFightProperty.wParry = l;}	
	//����
	virtual void SetBaseImmunity( short value )				{m_tgBaseMonsterFightProperty.wImmunity = value; }
	//��͸
	virtual void SetBasePierce( short value )				{m_tgBaseMonsterFightProperty.wPierce = value; }
	//�����ٶ�
//	virtual void SetBaseAtcSpeed(WORD l)					{m_tgBaseMonsterFightProperty.wAtcSpeed = l;}
	//�ƶ��ٶ�
	//virtual void SetBaseMoveSpeed(WORD l)					{m_tgBaseMonsterFightProperty.wMoveSpeed = l;}
	//ս���ƶ��ٶ�
	//virtual void SetBaseFightMoveSpeed(WORD l)				{m_tgBaseMonsterFightProperty.wFightMoveSpeed = l;}
	//��־����
	virtual void SetBaseWillDef(WORD l)						{m_tgBaseMonsterFightProperty.wWillDef = l;}
	//���ʿ���
	virtual void SetBaseConstiDef(WORD l)					{m_tgBaseMonsterFightProperty.wConstiDef = l;}
	
	//Debuff����
	virtual void SetBaseDeBuffDef(WORD lpos,WORD l)			
    {
		if(lpos<0) {lpos = 0;}else if((lpos+1)>DET_Max) lpos = DET_Max -1;
        m_tgBaseMonsterFightProperty.wDeBuffDef[lpos] = l;
    }
	//����Ԫ�ؿ���
	virtual void SetBaseElemDef(WORD lpos,WORD l)	
    {
		if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1;
        m_tgBaseMonsterFightProperty.wElemDef[lpos] = l;
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
	virtual void SetBaseCriDef(WORD l)						{m_tgBaseMonsterFightProperty.wCriDef = l;}
	//ħ��������
	virtual void SetBaseMCri(WORD l)						{m_tgBaseMonsterFightProperty.wMCri = l;}
	//ħ�������˺�
	virtual void SetBaseMCriDamg(WORD l)					{m_tgBaseMonsterFightProperty.wMCriDamg = l;}
	//ħ����������
	virtual void SetBaseMCriDef(WORD l)						{m_tgBaseMonsterFightProperty.wMCriDef = l;}
	//�Ƿ��ܶԻ�
	//virtual void SetBaseCanTalk(WORD l)						{m_tgBaseMonsterFightProperty.wCanTalk=l;}
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
//	virtual void DropGoods();						

	//##����ֵ�����ʵ�ֺ���
	LONG		CalculateExperienceQuota			( CPlayer* pPlayer, LONG lPosX, LONG lPosY );
	//����ְҵ������亯��
	LONG		CalculateOccuExpQuota				( CPlayer* pPlayer, LONG lPosX, LONG lPosY );
	LONG		CalculateOccuPointQuota				( CPlayer* pPlayer, LONG lPosX, LONG lPosY );
	//DWORD		CalculateExperienceCorrective		( CPlayer* pPlayer, DWORD dwExperience );
	//##��ȡ������
	CPlayer*	GetBeneficiary						();

	virtual void OnBeenHurted( LONG lAttackerType, const CGUID& AttackerGuid ); //���˺��Ĵ���

	DWORD GetAICount(){return m_dwAICount;}
	void SetAICount(DWORD dwCount){m_dwAICount=dwCount;}
	virtual void GetBeAttackedPoint(long lAttackerX,long lAttackerY,long& lTileX,long& lTileY);

//////////////////////////////////////////////////////////////////////////
//	�����ű�
//////////////////////////////////////////////////////////////////////////
public:
	const char* GetScriptFile()			{return m_strScriptFile.c_str();}
	void SetScriptFile(const char* str)	{m_strScriptFile=str;}

	virtual void Evanish();

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
//	virtual void			EnterCombatState()		{}
//	virtual void			EnterPeaceState()		{}
	virtual void			UpdateCurrentState()	{}


	//##��ȡ��ȭ��ʱ��
	DWORD GetStrikeOutTime();
//---------------------------------------------------------------------
//  �ڳ�����ˢ��ʱ���Ƿ�����
//---------------------------------------------------------------------
//public:
//	BOOL	GetCanReburn(){return m_bCanReburn;}
//	void	SetCanReburn(BOOL canreburn){m_bCanReburn=canreburn;}
//private:
//	BOOL m_bCanReburn;
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
	//enum{COLONY_STATE_NULL,COLONY_STATE_NOLEADER, COLONY_STATE_PEACE, COLONY_STATE_FIGHT};
	DWORD	GetColonyID(){return m_dwColonyID;}
	//DWORD	GetMemberID(){return m_dwMemberID;}
	WORD	GetMemberType(){return m_wMemberType;}
	void	AddToColony(DWORD dwColonyID, WORD wMemberType);
	void	ExitColony();
	WORD	GetColonyState(){return m_bColonyState;}
	BOOL	HasColony(){return m_dwColonyID==0? FALSE:TRUE;}
	DWORD	GetColonyDistance(){return m_dwColonyDistance;}

	void	SetColonyDistance(DWORD dis){m_dwColonyDistance= dis;}
	void	SetColonyID(DWORD dwColonyID){m_dwColonyID= dwColonyID;}
	//void	SetMemberID(DWORD dwMemberID){m_dwMemberID= dwMemberID;}
	void	SetMemberType(WORD type){m_wMemberType= type;}
	void	SetColonyState(BOOL bState){m_bColonyState= bState;}
private:

private:
	DWORD m_dwColonyID;
	//DWORD m_dwMemberID;
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
	//void OnDelEvent();			// ��Ӧɾ���¼�


public:

private:
	static BOOL CheckDropGoods(string MonsterName, INT RegionIndex, INT Level,
		INT LevelBalances,vector<CGoods*> &pOut, BYTE drop_flag = 0 );
	static BOOL CheckOneDrop(CMonsterList::tagDropGoods* pDrop,int LevelBalance, vector<CGoods*> &pOut);

public:
    TauntHandler &GetTauntHandler() { return *m_tauntHandler; }
private:
    TauntHandler *m_tauntHandler;
};

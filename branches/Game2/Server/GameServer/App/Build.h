////////////////////////////////////////////////////////////////////////////
//build.h///////////////////////////////////////////////////////////////////
//�����࣬�������������ͷ�������AI
//Author:Joe
//Create Time:2004.721
////////////////////////////////////////////////////////////////////////////
#pragma once
#include "MoveShape.h"
//#include "ServerRegion.h"

class CBuild :
	public CMoveShape
{
public:
	CBuild(void);
	~CBuild(void);

protected:
	enum eBuildState
	{
		STATE_CLOSE,
		STATE_OPEN,
		STATE_DESTORY,
		STATE_REFRESH,
	};
	long			m_lIndex;				//���

	long			m_lMaxHP;				//  �������ֵ 
	long			m_lHP;					//	��ǰ����ֵ
	long			m_lDef;					//  ������
	long			m_lMagicDef;			//  ħ��������

	long			m_lNameColor;			//������ɫ
	long			m_lLenth;				//����
	long			m_lWidth;				//���
	long			m_lLevel;				//�ȼ�
	string			m_strAttack;			//�ܹ����ű�
	string			m_strDestory;			//��ٽű�
	string			m_strRefresh;			//��ˢ�ű�	
	string			m_strHitSound;			//������Ч
	string			m_strDestorySound;		//�����Ч
	BYTE			m_byBlockType;			//�赲����
public:
	virtual void SetAction(WORD l);


	virtual void SetIndex(long x)			{m_lIndex=x;}
	void SetBlock(int x, int y, unsigned short block);
	virtual void SetMaxHP(long lmaxhp)		{m_lMaxHP=lmaxhp;}
	virtual void SetHP(long lhp);
	virtual void SetDef(long ldef)			{m_lDef=ldef;}
	virtual void SetMagicDef(long lmagicdef){m_lMagicDef=lmagicdef;}
	virtual void SetNameColor(long lcolor)	{m_lNameColor=lcolor;}
	virtual void SetLenth(long llenth)		{m_lLenth=llenth;}
	virtual void SetWidth(long lwidth)		{m_lWidth=lwidth;}
	virtual void SetLevel(long level)		{m_lLevel=level;}	
	virtual void SetAttackScp(string str)	{m_strAttack=str;}
	virtual void SetDestoryScp(string str)	{m_strDestory=str;}
	virtual void SetRefreshScp(string str)	{m_strRefresh=str;}
	virtual void SetHitSound(string str)	{m_strHitSound=str;}
	virtual void SetDestorySound(string str){m_strDestorySound=str;}
	virtual long GetIndex()					{return m_lIndex;}
	virtual DWORD GetHP()					{return m_lHP;}
	virtual string &GetDestory()			{return m_strDestory;}
	virtual string &GetRefresh()			{return m_strRefresh;}
	void SetBlockType(BYTE byBlock)			{m_byBlockType = byBlock;}
	BYTE GetBlockType()						{return m_byBlockType;}
	//virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	//virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����
	virtual bool CodeToDataBlock(DBWriteSet& setWriteDB, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData = true);	// ����

	virtual void OnBeenAttacked			( tagAttackInformation& ai );
	virtual void ApplyFinalDamage		( tagAttackInformation& ai, vector<tagDamage*>& vRet );
	virtual void OnDied();															// ������ʱ����
	virtual void OnBeenMurdered( LONG lMurdererType, const CGUID& MurdererGuid );
	virtual void OnBeenHurted( LONG lAttackerType, const CGUID& AttackerGuid ); 
	

	virtual DWORD	GetMaxHP()						{return m_lMaxHP;}
	virtual long	GetLenth()						{return m_lLenth;}
	virtual long	GetWidth()						{return m_lWidth;}
	WORD	GetDef()								{return static_cast<WORD>(m_lDef);}
	virtual long OnRefresh();
	virtual bool IsAttackAble(CMoveShape* pAttackShape);								// �ɷ񹥻�
	virtual WORD GetFigure(eDIR dir=DIR_UP);										// ���ݷ���õ�����
	virtual long GetAttackerDir(long lAttackerX,long lAttackerY,long ldx,long ldy);
	virtual void GetBeAttackedPoint(long lAttackerX,long lAttackerY,long& lTileX,long& lTileY);

	void MoveAroundPlayer();
	map<string, tagPty>		    m_mapNameValue;		// �������ֶ�Ӧ�ı�����ַ
	typedef map<string, tagPty>::iterator mapNameValueItr; 
	//������
	set<string> m_mapDirtyAttrData;
	void SetBuild(WORD l);
	void AddAttributeToDirty(string attrName);
	void InitNameValueMap();
	bool SetAttribute(std::string& attrName, long value);
	bool SetAttribute(char* attrName, long value);
	long GetAttribute( std::string attrName );
	virtual long GetCurrentValue(const tagPty& tagValue, long evalue/*�ı�ֵ*/, long bvalue/*����ֵ*/);
	virtual bool  ChangeAttribute(std::string& attrName, long value);
	virtual long  UpdateAttributeToClient();
	long  UpdateAttribute(BOOL bImmediately=0);
	long  OnUpdateAttributeTimeOut();

	virtual long GetValue(const char* strName);
	virtual long SetValue(const char* strName, long dwValue);
	long GetCurrentValue();
	//void AddChangeAttrToMsgArray(std::vector<BYTE>* bArray);
	void CodeChangeAttrToDataBlock(DBWriteSet& setWriteDB);
public:

//------------------------------------------------------------------------
//	�̳���CMoveShape��Ҫʵ�������麯�� -- Added By Jar
//------------------------------------------------------------------------
 
	virtual void			UpdateCurrentState()	{}
private:
	void SetPosXY(float fX,float fY);
};

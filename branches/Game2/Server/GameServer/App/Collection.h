//		�ļ���:	Collection.h
//		����:	heliangchen
//		ʱ��:	2008/1/26
//		���:	�ɼ������ʵ��.�ڳ�������Ӳɼ���,����ְҵ �� ����ɼ���

#include "MoveShape.h"

#include "../../../Public/Common/DeputyOccu.h"

class CCollection:public CMoveShape
{
public:
	CCollection();
	~CCollection();
public:

	//��Ӧˢ��id
	DWORD				GetRefreshIndex	()						{return m_dwRefreshIndex;}
	void				SetRefreshIndex	(DWORD l)				{m_dwRefreshIndex=l;}

	//hp
	virtual DWORD		GetHP()									{return 1;}
	virtual void		SetHP(DWORD l)							{}
	BYTE				GetBlockType()							{return m_byBlockType;}
	void				SetBlockType(BYTE l)					{m_byBlockType= l;}

public:
	BOOL Init(const CCollectionLayout* p);
	//virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ѹ��
	//virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����

	virtual bool CodeToDataBlock(DBWriteSet& setWriteDB, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData = true);	// ����


	virtual void				UpdateCurrentState	(){}
	virtual bool IsAttackAble(CMoveShape* pAttacker/*������*/){return false;};	// �ɷ񹥻�
	//���ɾ���¼�
	//BOOL AddDeleteEvent(DWORD dwTime);
	//��������¼�
	//BOOL AddReliveEvent(DWORD dwTime);
	//��ʱ������
	//virtual long OnTimeOut(DWORD timerid,DWORD curtime,const void* var);
	//����ȫ�ɼ�
	virtual BOOL BeenCollectedAll();
//
	void OnLoseBeneficiary();			//��ʧ�������¼�
	void StartRecordBeneficiary(CGUID guidFirst,OBJECT_TYPE typeFirst,DWORD dwBeneficiaryTime);				//��ʼ��¼������
	//
	const string& GetStartCollectScript(){return m_strStartCollectScript;}
	void	SetStartCollectScript(const string & strScriptName){m_strStartCollectScript= strScriptName;}

	const string& GetEndCollectScript(){return m_strEndCollectScript;}
	void	SetEndCollectScript(const string & strScriptName){m_strEndCollectScript= strScriptName;}

	void SetPosXY(float fX,float fY);
	void SetBlock(int x, int y, unsigned short block);
protected:
	long m_lDiedEvent;
	void AddDiedEvent();
	void OnDiedEvent();
	bool LogoutDiedEvent();
	void DelDiedEvent();
protected:
	////�¼�����
	//BOOL AddEvent(eChildObjType dwType,DWORD dwTime);
	//BOOL LogoutEvent(LONG lTimerID);
	//BOOL DeleteEvent(LONG lTimerID);
	//�¼���Ӧ
	//void OnDelEvent();
	BOOL OnRelive();
protected:

	//ˢ������
	DWORD	m_dwRefreshIndex;

	//��¼timerid
	LONG	m_lTimerID;
	BYTE	m_byBlockType;

	//��ʼ�ɼ��ű�
	string m_strStartCollectScript;
	//�ɼ������ű��ű�
	string m_strEndCollectScript;


};
//////////////////////////////////////////////////////////////////////////
//serverwarregion.h/
//ս�����᳡���Ļ���
//Author��Joe
//Create Date:2005.7.5
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "serverregion.h"

//����״̬�Ľṹ
struct tagContend
{
	long lID;				//��ʾID
	string strName;			//��־������
	CGUID PlayerID;			//��������
	const CGUID& FactionID;		//���ID
	long lCurTime;			//���е�ʱ��(����)
	long lMaxTime;			//����ʱ��
	DWORD dwStartTime;		//��һ�μ�ʱʱ��
};

class CServerWarRegion :
	public CServerRegion
{
public:
	CServerWarRegion(void);
	CServerWarRegion(const CGUID& id);
	CServerWarRegion(long id);
	~CServerWarRegion(void);

	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����

private:
	list<tagContend>		m_listContend;				//����
	typedef list<tagContend>::iterator itContend;
	typedef map<long,CGUID>::iterator itFacSym;
protected:
	map<long,CGUID>			m_FacWinSymbol;				//��־�ﱻռ��İ��
	long	m_lSymbolTotalNum;							//��־��������
	long	m_lWinVicSymbolNum;							//��ʤ��ȡ��־�������
	long	m_lVicSymbolNum;							//��ֵ�����ʤ�ı�־������
public:
	virtual void OnWarDeclare(long lWarNum);				//��ʼ��ս
	virtual void OnWarStart(long lWarNum);					//���ǿ�ʼ
	virtual void OnWarEnd(long lWarNum);					//���ǽ���
	virtual void OnWarMass(long lWarNum);					//���Ǽ���
	virtual void OnFactionVictory(const CGUID& FactionID,const CGUID& UnionID);	//��һ������ʤʱ
	virtual void ClearRegion();								//����ʱ�峡��

	void ReSetWarState(long lWarID,eCityState eState);

	virtual bool IsOwner(const CGUID& FactionID)	{return false;}
	virtual void OnEnterContend(CPlayer* pPlayer,long lID,const char* chName,long lTime,string strGood1,string strGood2,
								string strGood3,string strGood4);
    
	void OnPlayerDamage(CPlayer* pPlayer,long lDamHp);		//����ҹ����ܵ��˺�
	void AddContend(CPlayer* pPlayer,long lID,const char* chName,long lTime);	//���һ������
	void DecContendTime(CPlayer* pPlayer,long lDecTime);	//����ʱ��
	bool CancelContendByPlayerID(CPlayer* pPlayer);			//ȡ��ĳ����ҵ�����
	void CancelContendBySymbolID(long lSymbolID);			//ȡ��������Ҷ�ĳ����־�������
	void SetFacWinSymbol(const CGUID& FactionID,const CGUID& UnionID,long lSymbol);//����ĳ�����Ӯ��ĳ����־��
	virtual void AddNeedGood(string& strGoodName){}			//�����Ҫ�ĵ�����
	virtual void OnContendTimeOver(tagContend Contend);		//ĳ������ʱ�䵽
	bool GetIsFacWinSymbol(const CGUID& FactionID,long lSymbolID);	//�ж�ĳ������Ƿ�Ӯ����ĳ����־��
	bool GetIsPlayerIsContendSymbol(const CGUID& PlayerID,long lSymbolID);//�ж�ĳ������Ƿ���������ĳ����־����
	virtual void OnFactionWinOneSymbol(const CGUID& FactionID,long lSymbolID){}//��һ�����Ӯ����һ����־�ﴥ��

	void UpdateContentTime(const CGUID& PlayerID,long lPercent);	//ˢ������״̬ʱ�䵽�ͻ���
	void UpdateContendPlayer();								//��������İ������������
};

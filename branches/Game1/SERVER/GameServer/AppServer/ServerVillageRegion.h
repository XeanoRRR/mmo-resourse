//////////////////////////////////////////////////////////////////////////
//Servervillageregion.h
//��սս��ͼ�࣬�����ս�����ݴ���
//Author��Joe
//Create Date:2005.7.5
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "serverwarregion.h"

class CServerVillageRegion :
	public CServerWarRegion
{
public:
	CServerVillageRegion(void);
	CServerVillageRegion(const CGUID& id);
	CServerVillageRegion(long id);
	~CServerVillageRegion(void);

	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����

private:
	list<string>	m_listGoods;				//��������״̬��Ҫ����Ʒ
	CGUID			m_FlagOwnerFacID;			//��־��ӵ���ߵ�ID
public:
	bool IsOwner(const CGUID& FactionID);				//�ж�ĳ������Ƿ�������
	void OnWarDeclare(long lWarNum);			//��ʼ��ս
	void OnContendTimeOver(tagContend Contend);	//ĳ������ʱ�䵽
	void OnWarStart(long lWarNum);				//���忪ʼ
	void OnWarTimeOut(long lWarNum);			//����ʱ�����
	void OnWarEnd(long lWarNum);				//ս������
	void AddNeedGood(string& strGoodName);		//�����Ҫ�ĵ�����
	void OnFactionWinOneSymbol(const CGUID& FactionID,long lSymbolID);	//��һ�����Ӯ����һ����־�ﴥ��

	void OnFactionVictory(const CGUID& FactionID,const CGUID& UnionID);	//��һ������ʤʱ
	void ClearRegion();										//����ʱ�峡��

	bool IsApplyWarFacsMem(const CGUID& FactionID);
};

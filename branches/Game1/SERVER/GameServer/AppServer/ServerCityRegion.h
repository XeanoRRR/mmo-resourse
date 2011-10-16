///////////////////////////////////////////////////////////////
//Serverregion.h///////////////////////////////////////////////
//����ս��ͼ�࣬���𹥳�ս�Ĵ���
//Author��Joe
//Create Date:2004.7.19
#pragma once
#include "serverwarregion.h"
#include <map>
class CBuild;
class CCityGate;
class CMoveShape;
class CMonster;
using namespace std;

class CServerCityRegion :
	public CServerWarRegion
{
public:
	CServerCityRegion(void);
	CServerCityRegion(const CGUID& id);
	CServerCityRegion(long id);
	~CServerCityRegion(void);
	//Build
	struct tagBuild
	{
		long lID;				// ���ŵı��
		long lPicID;			// ͼƬID
		long lDir;				// ����
		long lAction;			// ��ʼAction
		long lMaxHP;			// �����������
		long lDef;				// ������
		long lElementResistant;	//Ԫ�ؿ���
		long lTitleX;			// ����
		long lTitleY;
		long lWidthInc;			// ����
		long lHeightInc;

		string strName;			// ����
		string strScript;		// ����ִ�нű�
	};

	struct tagCityGate
	{
		long lID;
		CGUID CityGateID;
		CCityGate* pCityGate;
		string strName;
	};
private:
	CBuild*					m_pSymbol;					//��־��
	map<long,tagCityGate>	m_CityGates;				//����
	tagRegionSetup			m_DefenceSideRS;			//���ط���������


	set<CGUID>				m_GurdMonsters;				//�����еĻ������
	list<long>				m_GuardIndexs;				//������ˢ���б�

	long	m_lAttackCityGateType;
	long	m_lAttackCityGateID;
	CGUID	m_DefSideRSFactionID;					//�ڲ�����㸴��İ��ID

	//////////////////////////////////////////////////////////////////////////
	//	�ӿ�
	//////////////////////////////////////////////////////////////////////////
public:
	BOOL Save();	// ��ȡ

	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����
	
	void	SetAttackCityGateMoveShape(long lType,long lID)
	{m_lAttackCityGateType=lType;m_lAttackCityGateID=lID;}

public:
	
	virtual void SetEnterPosXY(CPlayer*pPlayer);
	void SetOwnedCityOrg(const CGUID& FactionID,const CGUID& UnionID);

	CCityGate* AddCityGate(tagBuild&);											// ���һ������
	virtual void AddGurdMonster(const CGUID& guid);
	virtual void DelGurdMonster(const CGUID& guid){m_GurdMonsters.erase(guid);}
	virtual void	AddGuardIndex(long lIndex);
	void	RefreshGuard();

	virtual bool OperatorCityGate(long lCityGateID,eOperCityGate eOp);		// �Գ��Ž��в��� 
	bool OperatorCityGate(CCityGate* pCityGate,eOperCityGate eOp);
	virtual void UpdateCityGateToClient(long lCityGateID);					//���³�����Ϣ���ͻ���
	virtual bool OperatorSymbol(long lSymbolID,eOperCityGate eOp);				// �Ա�־����в��� 

	virtual	void OnSymbolDestroy(long lSymID,long lPlayerID,long lFacID,const CGUID& UnionID);	//����־��ݻ�ʱ 
	virtual void OnWarDeclare(long lWarNum);											//��ʼ��ս
	virtual void OnWarStart(long lWarNum);												//���ǿ�ʼ
	virtual void OnWarTimeOut(long lWarNum);											//����ʱ�����
	virtual void OnWarEnd(long lWarNum);												//���ǽ���
	virtual void OnWarMass(long lWarNum);												//���Ǽ���
	virtual void OnClearOtherPlayer(long lWarNum);										//���������Ա
	virtual void OnRefreshRegion(long lWarNum);											//ˢ�³���
	virtual bool SymbolIsAttackAble(CMoveShape* pAttackShape);
	virtual bool GuardIsAttackAble(CMoveShape* pAttackShape);
	bool	CityGateIsClose(long lCityGateID);											//�ж�ĳ�������Ƿ���Թر�
	bool	CityGateIsClose(CCityGate* pCityGate);										//�ж�ĳ�������Ƿ���Թر�
	virtual long	GetCityGateState(long lCityGateID);									//�õ�ĳ�����ŵ�״̬

	bool IsOwner(const CGUID& FactionID);														//�ж�ĳ������Ƿ�������
	void OnContendTimeOver(tagContend Contend);											//ĳ������ʱ�䵽
	void OnFactionVictory(const CGUID& FactionID,const CGUID& UnionID);								//��һ������ʤʱ
	void ClearRegion();																	//����ʱ�峡��
	virtual void OnFactionWinOneSymbol(const CGUID& FactionID,long lSymbolID);					//��һ�����Ӯ����һ����־�ﴥ��

	virtual void GetReturnPoint(CPlayer* pPlayer, long &lRegion, long &l, long &t, long &r, long &b, long &d);
	virtual CRegion::eSecurity GetSecurity(int iTileX, int iTileY);
	virtual long GetDiedStateTime();

	bool IsApplyWarFacsMem(const CGUID& FactionID);
};

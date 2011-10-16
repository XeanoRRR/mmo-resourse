#pragma once
#include "baseobject.h"

extern long AREA_WIDTH;		// ����Ŀ��
extern long AREA_HEIGHT;	// ����ĸ߶�
const int AREA_HIBERNATE_DELAYTIME= 15000; //�ӳ�����ʱ��

//! ��Ʒ������ʰȡ�жϷ���ֵ
enum eGoodsPickValue
{
	eGPV_Cant			= 0,		//! ����ʰȡ
	eGPV_Allow_NoOwner	= 0xFF00,	//! ����ʰȡ��������Ʒ
	eGPV_Allow_IsYours,				//! ����ʰȡ����Ʒֻ���ڱ��������
	eGPV_Allow_IsYourTeams,			//! ����ʰȡ����Ʒ���ڱ�����������ڶ���
};


class CGoods;
class CShape;
class CArea :
	public CBaseObject
{
private:
	//��Ʒ�����Ͷ�ʱɾ��
	enum PROTECTION_LEVEL
	{
		PL_PLAYER,			// ���
		PL_TEAM,			// ���
	};

	struct tagGoodsProtection
	{
		tagGoodsProtection(){lTimerID=-1;dwTimeStamp=0;}
		//��ʱ��id
		long				lTimerID;
		//ʱ���
		DWORD				dwTimeStamp;
		//��Ʒ�������ͣ���ң� ���飩
		PROTECTION_LEVEL	plLevel;
		//
		CGUID				guid;
	};

	long m_lX;		// ��ͼ�е�x����
	long m_lY;		// ��ͼ�е�y����

	hashsetGUID	m_setPlayers;
	hashsetGUID	m_setActivedMonsters;
	hashsetGUID	m_setSleepedMonsters;
	hashsetGUID	m_setGoods;	
	hashsetGUID	m_setPets;
	hashsetGUID	m_setBuild;
	hashsetGUID	m_setCollection;


	map<CGUID, tagGoodsProtection,guid_compare>	m_mGoodsProtection;
	typedef map<CGUID, tagGoodsProtection,guid_compare>::iterator itGoodsPro;
	//����
	int m_iPlayerAreaAround;
	BOOL m_bIsHangUp;
	long m_lTimerID;

public:
	long GetX()			{return m_lX;}
	long GetY()			{return m_lY;}
	void SetX(long l)	{m_lX=l;}
	void SetY(long l)	{m_lY=l;}

	CArea(void);
	virtual ~CArea(void);

	VOID				PlayerEnter		();
	VOID				PlayerLeave		();
	hashsetGUID&		GetAllPlayers	();

	VOID				WakeUpMonsters	();

	//##����ˢ�¹����ʱ��
	VOID				OnRefreshMonster(long lRefeashIndex);

	virtual void RemoveObject(CBaseObject* pObject);			// �Ƴ�OBJECT
	virtual void AddObject(CBaseObject* pObject);				// ���OBJECT

	//##������Ʒ�����ĺ���
	VOID SetGoodsProtection( CPlayer* pPlayer, CGoods* pGoods,BOOL bTeamProtected=TRUE);

	eGoodsPickValue CanPickUpGoods( CPlayer* pPlayer, CGoods* pGoods );

	//������Ʒ������ʱɾ��
	//�����Ʒɾ��ʱ��
	VOID	AddGoodsDelTime(CGoods *pGoods,tagGoodsProtection& pGoodsProtect);
	VOID	AddGoodsDelTime(CGoods *pGoods);
	//ȡ����Ʒɾ��ʱ��
	VOID	LogoutGoodsDelTime(CGoods* pGoods);
	//������ʱ��ִ��ɾ��
	VOID	OnGoodsDelTimeOut(stDestObj* pDestObj);

	//##���Ҷ���ĺ���
	BOOL FindShapes(long lType,hashsetGUID& setOut);
	BOOL FindShapes			( LONG lType, vector<CShape*>& vOut );
	BOOL GetAllShapes		( vector<CShape*>& vOut );

	//##
	DWORD GetNumShapes		();

	//��ʱ����
	virtual long OnTimeOut(DWORD timerid,DWORD curtime,const void* var);
	//ע��,ɾ����ע�� �¼�
	BOOL AddEvent(DWORD timerid,DWORD timer);
	BOOL DelEvent();
	BOOL LogoutEvent(long& timerid);
	//����֪ͨ
	VOID HibernateCall				();
	//����֪ͨ
	VOID WakeUpCall					();
	//���߱�����
	VOID Hibernate					();
	//���ѱ�����
	VOID WakeUp						();
	//�Ƿ�����
	BOOL IsHibernate				(){return m_bIsHangUp;}
};

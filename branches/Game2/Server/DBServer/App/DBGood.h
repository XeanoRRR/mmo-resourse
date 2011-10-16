#ifndef DB_GOOD_H
#define DB_GOOD_H
#include "BaseDBEntity.h"
#include <list>
#include "..\..\..\public\GUID.h"
#include "../public/DBDefine.h"

using namespace std;


class CDBGoods : public CBaseDBEntity
{
public:
	//��Ʒ���
	enum DB_GOODS_TYPE
	{
		//������
		GT_USELESS,
		//����Ʒ
		GT_CONSUMABLE,
		//װ��
		GT_EQUIPMENT,
		//����
		GT_PACK
	};

	enum GOODS_PLACE
	{		
		PLACE_PACKET					= 1,	//��ͨ����
		PLACE_EQUIP,//װ��
		//PLACE_HAND,//����
		PLACE_WALLET,//Ǯ��
		PLACE_SILVERWALLET,//����
		PLACE_GEM, //! ��ʯ
		PLACE_YUANBAO,//Ԫ��
		PLACE_JIFEN,//����
		PLACE_EQUIPMENT_UPGRADING,			//װ������
		//PLACE_BANK,//����
		PLACE_DEPOT, //�ֿ�
		PEI_ENHANCEMENT,//

		PLACE_PACK =10000,
		PLACE_PACK1=10001,
		PLACE_PACK2=10002,
		PLACE_PACK3=10003,
		PLACE_PACK4=10004,
		PLACE_PACK5=10005
	};

private:
	CDBGoods();
public:
	CDBGoods(const CGUID& guid);
	virtual ~CDBGoods();

	void SetID(long id) { m_ID = id; }
	long GetID(void)    { return m_ID; }

	// virtual interfaces
	virtual void ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);
	void Save(long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);

	// Get Interfaces
	const char*	GetSZID(void)				{ return &m_szID[0];}
	const char*	GetGoodsID(void)		{ return &m_szGoodsID[0];}
	
	//const CGUID& GetExID(void)			{ return m_guExID;}
	long GetGoodsType(void)	{ return m_GoodsType;}
	// Set Interfaces
	void SetSZID					(const char* buf, long size)		{ memcpy(m_szID, buf, strlen(buf));}
	void SetGoodsID				(const char* buf, long size)		{ memcpy(m_szGoodsID, buf, strlen(buf));}
	
	//void SetExID				(CGUID& guid)			{ m_guExID = guid;}
	void SetGoodsType			(long type)	{ m_GoodsType = type;}

	
	// ��Player��Ϣ�ж�ȡ����
	void ReadDataFromMsgForPlayer(BYTE* msgBuf, long& msgBufPos);

	void AddEntityDataToMsg(DB_OPERATION_TYPE opType,vector<BYTE>& pBA);

	CDBGoods& operator=(CDBGoods& right)
	{
		CBaseDBEntity::operator=(right);
		if(this != &right)
		{
			memcpy(m_szID, right.m_szID, 64);
			memcpy(m_szGoodsID, right.m_szGoodsID, 64);
			m_lBasePropertiesIndex = right.m_lBasePropertiesIndex;
			m_GoodsType = right.m_GoodsType;
			m_ID = right.m_ID;
			b_IsFirstCreated = right.b_IsFirstCreated;
		}

		return *this;
	}

	// �Ƿ��Ǹմ�����δ��Load/Save�̴߳�����Ķ���
	bool	GetIsFirstCreated(void) { return b_IsFirstCreated; }
	void	SetIsFirstCreated(bool flag) { b_IsFirstCreated = flag; }

	// ��ʼ����������
	void				InitProperty();

	// �������������Ƿ���ָ��������������
	bool				FindTableUpdateFlag(const string& tableName);

private:
	// ��Ʒ���ݿ���
	char	m_szID[64];
	char	m_szGoodsID[64];
	long    m_lBasePropertiesIndex;
	long	m_GoodsType;

	long m_ID;

	bool b_IsFirstCreated;
};

#endif//DB_GOOD_H
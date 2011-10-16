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
	CDBGoods();
	CDBGoods(const CGUID& guid);
	~CDBGoods();

	// virtual interfaces
	virtual void ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);

	void Load(long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);

	// Get Interfaces
	const char*	GetSZID(void)				{ return &m_szID[0];}
	const char*	GetGoodsID(void)		{ return &m_szGoodsID[0];}
	
	// Set Interfaces
	void SetSZID					(const char* buf, long size)		{ memcpy(m_szID, buf, strlen(buf));}
	void SetGoodsID				(const char* buf, long size)		{ memcpy(m_szGoodsID, buf, strlen(buf));}

	// ��Player��Ϣ�ж�ȡ����
	void ReadDataFromMsgForPlayer(BYTE* msgBuf, long& msgBufPos, long msgid);

	// ��װ��Ϣ
	void AddEntityDataToMsg(DB_OPERATION_TYPE opType, vector<BYTE>& pBA);

	// ��ʼ����������
	void				InitProperty();

	CDBGoods& operator=(CDBGoods& right)
	{
		CBaseDBEntity::operator=(right);
		if(this != &right)
		{
			memcpy(m_szID, right.m_szID, 64);
			memcpy(m_szGoodsID, right.m_szGoodsID, 64);
		}

		return *this;
	}

private:
	// ��Ʒ���ݿ���
	char	m_szID[64];
	char	m_szGoodsID[64];
	long    m_lBasePropertiesIndex;
};

#endif//DB_GOOD_H
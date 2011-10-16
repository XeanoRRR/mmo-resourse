#pragma once
#ifndef _INC_GOODSID_H
#define _INC_GOODSID_H

class CGoodsID
{
private:
	CGoodsID(void);
	virtual ~CGoodsID(void);

public:
	static CGoodsID* GetInstance();
	static void Release();

private:
	static CGoodsID *instance;

public:
	// ��Ʒ��ʼID��Ϣ
	struct tagGoodsID
	{
		bool	bUsed;				// �Ƿ��Ѿ������ȥ��
		DWORD	dwGoodsID;			// ��ʼID
		DWORD	dwGameServerIndex;	// �������GameServer
	};

public:
	static const DWORD GOODSID_ZONE_SIZE = 0x10000000;	// ��Ʒ�����С(һ��16������)

	/*static*/ DWORD GetGoodsID(int nIndex);											// ��ȡһ����Ʒ��ʼID
	/*static*/ void SetGoodsID(int nIndex, DWORD dwVal, bool bUsed=false);				// ������ƷID����ʼֵ
	/*static*/ void SetGoodsIDGameServerIndex(int nIndex, DWORD dwGameServerIndex);	// ������ƷID���������GameServer
	/*static*/ void AddGoodsID(int nIndex);											// �����Ʒ����ʼID
	/*static*/ int AssignFirstFreeGoodsID(DWORD dwGameServerIndex);					// ��ȡ��һ�����Է������ƷIDλ��(����0������),�����ϱ��
	/*static*/ void FreeGoodsIDByGameServerIndex(DWORD dwGameServerIndex);				// �ͷ�һ��ָ��GameServerIndex����ƷID

public:
	/*static*/ tagGoodsID m_stGoodsID[16];							// ��Ʒ��ʼID
};


#endif // _INC_GOODSID_H

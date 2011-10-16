// Copyright (C) 1991 - 1999 Rational Software Corporation

#pragma once


#include "../Listener/CContainerListener.h"

#include "CGoodsContainer.h"

class CBaseObject;
class CGoods;
class CContainerListener;

class CContainer;


//##ModelId=41C92B7703A9
class CWallet : public CGoodsContainer, public CContainerListener
  
{
public:
	//##ModelId=41CA26DA000F
	virtual bool OnObjectAdded(CContainer* pContainer, CBaseObject* pObj, DWORD dwAmount, LPVOID pVoid);

	//##ModelId=41CA26E101A5
	virtual bool OnObjectRemoved(CContainer* pContainer, CBaseObject* pObj, DWORD dwAmount, LPVOID pVoid);

	//##ModelId=41C92C0300EA
	CWallet(DWORD dwGoodsIndex = 0);

	//##ModelId=41C92C0700DA
	//CWallet(LONG lID);

	//##ModelId=41C92C0C0196
	~CWallet();

	//##ModelId=41C92C2301F4
	virtual bool Add(CBaseObject* pObject, tagPreviousContainer* pPreviousContainer, LPVOID pVoid = NULL);

	//�ú�����λ�ý�������
	//##ModelId=41C92C390148
	virtual bool Add(DWORD dwPosition, CGoods* pObject, tagPreviousContainer* pPreviousContainer, LPVOID pVoid = NULL);

	//�ú�����λ�ý�������
	//##ModelId=41C92C7000BB
	virtual CBaseObject* Remove(DWORD dwPosition, DWORD dwAmount, LPVOID pVoid = NULL);

	//##ModelId=41C92CBB0186
	virtual void Clear(LPVOID pVoid = NULL);

	//##ModelId=41C92CC2008C
	virtual void Release();

	//���������ļ�,�����Ƿ�����.
	//##ModelId=41C92CD70128
	virtual bool IsFull();
	//! �õ��������
	long GetMaxCapacity(void);

	//##ModelId=41C92D030186
	virtual void TraversingContainer(CContainerListener* pListener);

	//Ŀǰ��Ǯû������
	//##ModelId=41C92D0A009C
	virtual DWORD GetContentsWeight();

	//�ú������ж���Ʒ�Ƿ����ڽ�Ǯ,�����,����Ƿ��㹻.Ȼ��
	//��true��FALSE��λ����Զ����0��
	//##ModelId=41C92D340167
	virtual bool QueryGoodsPosition(CGoods* pGoods, DWORD& dwPosition);

	//�ú�����Զ����FALSE
	//##ModelId=41C92DBA0157
	virtual bool QueryGoodsPosition(const CGUID& guid, DWORD& dwPosition);

	//##ModelId=41C92DD80213
	virtual bool IsGoodsExisted(DWORD dwGoodsBasePropertiesIndex);

	//##ModelId=41C92DDF0119
	virtual CGoods* GetTheFirstGoods(DWORD dwGoodsBasePropertiesIndex);

	//##ModelId=41C92DF4030D
	virtual CGoods* GetGoods(DWORD dwPosition);

	//##ModelId=41C92E0603C8
	virtual void GetGoods(DWORD dwGoodsBasePropertiesIndex, vector<CGoods*> vOut);

	//ûǮ��ʱ��,����0,��Ǯ��ʱ�򷵻�1.
	//##ModelId=41C92E1A0222
	virtual DWORD GetGoodsAmount();
	virtual DWORD GetGoodsAmountByName(const char* strName);
	virtual DWORD GetGoodsAmountByType(int nType);

	virtual bool			Serialize				( DBWriteSet& setWriteDB, bool b = true );
	virtual bool			Unserialize				( DBReadSet& setReadDB, bool b = true );
	//##ModelId=41CA6A2203C8
	DWORD GetGoldCoinsAmount();

	//##ModelId=41CA6A4203D8
	bool AddGoldCoins(DWORD dwAmount, LPVOID pVoid);

	//##ModelId=41CA6A6F003E
	bool DecreaseGoldCoins(DWORD dwAmount, LPVOID pVoid);

	void	SetoldCoinsGood(CGoods* pGoods)	{m_pGoldCoins = pGoods;}

	CGoods*	GetGoldGoods(){return m_pGoldCoins;}


protected:
	//�ú���û���κ����ã���Զ����NULL
	//##ModelId=41C92C18006D
	virtual CBaseObject* Find(const CGUID& guid);

	//�ú���û���κ����ã���Զ����NULL
	//##ModelId=41C92C510177
	virtual CBaseObject* Remove(const CGUID& guid, LPVOID pVoid = NULL);

private:
	//##ModelId=41CA270102CE
	CGoods* m_pGoldCoins;
    DWORD   m_dwWalletIndex;

};



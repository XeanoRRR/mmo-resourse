#ifndef _CYuanBao_h_
#define _CYuanBao_h_

#include "..\Listener\CContainerListener.h"

#include "CGoodsContainer.h"
#include "..\..\..\Setup\GlobeSetup.h"


class CBaseObject;
class CGoods;
class CContainerListener;

class CContainer;

//##ModelId=41C92B7703A9
class CYuanBao : public CGoodsContainer, public CContainerListener
  
{
public:
	//##ModelId=41CA26DA000F
	virtual BOOL OnObjectAdded(CContainer* pContainer, CBaseObject* pObj, LPVOID pVoid);

	//##ModelId=41CA26E101A5
	virtual BOOL OnObjectRemoved(CContainer* pContainer, CBaseObject* pObj, LPVOID pVoid);

	//##ModelId=41C92C0300EA
	CYuanBao();

	//##ModelId=41C92C0700DA
	//CYuanBao(LONG lID);

	//##ModelId=41C92C0C0196
	~CYuanBao();

	//##ModelId=41C92C2301F4
	virtual BOOL Add(CBaseObject* pObject, LPVOID pVoid = NULL);

	//�ú�����λ�ý�������
	//##ModelId=41C92C390148
	virtual BOOL Add(DWORD dwPosition, CGoods* pObject, LPVOID pVoid = NULL);

	virtual BOOL AddFromDB( CGoods* pObject, DWORD dwPosition  = 0 );

	//�ú�����λ�ý�������
	//##ModelId=41C92C7000BB
	virtual CBaseObject* Remove(DWORD dwPosition, DWORD dwAmount, LPVOID pVoid = NULL);

	//##ModelId=41C92CBB0186
	virtual VOID Clear(LPVOID pVoid = NULL);

	//##ModelId=41C92CC2008C
	virtual VOID Release();

	//���������ļ�,�����Ƿ�����.
	//##ModelId=41C92CD70128
	virtual BOOL IsFull();

	//##ModelId=41C92D030186
	virtual VOID TraversingContainer(CContainerListener* pListener);

	//Ŀǰ��Ǯû������
	//##ModelId=41C92D0A009C
	virtual DWORD GetContentsWeight();

	//�ú������ж���Ʒ�Ƿ����ڽ�Ǯ,�����,����Ƿ��㹻.Ȼ��
	//��TRUE��FALSE��λ����Զ����0��
	//##ModelId=41C92D340167
	virtual BOOL QueryGoodsPosition(CGoods* pGoods, DWORD& dwPosition);

	//�ú�����Զ����FALSE
	//##ModelId=41C92DBA0157
	virtual BOOL QueryGoodsPosition(const CGUID& guid, DWORD& dwPosition);

	//##ModelId=41C92DD80213
	virtual BOOL IsGoodsExisted(DWORD dwGoodsBasePropertiesIndex);

	//##ModelId=41C92DDF0119
	virtual CGoods* GetTheFirstGoods(DWORD dwGoodsBasePropertiesIndex);

	//##ModelId=41C92DF4030D
	virtual CGoods* GetGoods(DWORD dwPosition);

	//##ModelId=41C92E0603C8
	virtual VOID GetGoods(DWORD dwGoodsBasePropertiesIndex, vector<CGoods*> vOut);

	//ûǮ��ʱ��,����0,��Ǯ��ʱ�򷵻�1.
	//##ModelId=41C92E1A0222
	virtual DWORD GetGoodsAmount();

	virtual BOOL			Serialize				( vector<BYTE>* pStream, BOOL b = TRUE );

	virtual BOOL			Unserialize				( BYTE* pStream, LONG& lOffset, BOOL b = TRUE );

	virtual BOOL			Serialize				( DBWriteSet& setWriteDB, BOOL b = TRUE );
	virtual BOOL			Unserialize				( DBReadSet& setReadDB, BOOL b = TRUE );

	//##ModelId=41CA6A2203C8
	DWORD GetGoldCoinsAmount();

	//##ModelId=41CA6A4203D8
	BOOL AddGoldCoins(DWORD dwAmount, LPVOID pVoid);

	//##ModelId=41CA6A6F003E
	BOOL DecreaseGoldCoins(DWORD dwAmount, LPVOID pVoid);

	//�ú���û���κ����ã���Զ����NULL
	//##ModelId=41C92C18006D
	virtual CBaseObject* Find(const CGUID& guid);
	virtual CBaseObject*	Find					( LONG oType, const CGUID& guid );
	//�ú���û���κ����ã���Զ����NULL
	//##ModelId=41C92C510177
	virtual CBaseObject* Remove(const CGUID& guid, LPVOID pVoid = NULL);

private:
	//##ModelId=41CA270102CE
	CGoods* m_pGoldCoins;
public:
	///*virtual*/ BOOL AddGoldCoinOfLargess(CGoods* pObject, DWORD dwPosition=0, LPVOID pVoid=0);
};

#endif /* _INC_CYuanBao_41C92B7703A9_INCLUDED */

/**
*	file:		SpecialEquipContainer.h
*	Brief:		����װ������
*	detail:		����װ����λ�� ���֡����֡�����
*	Authtor:	��η
*	Date:		2010-08-24
*/
#pragma once
#include "CGoodsContainer.h"
#include "../listener/CContainerListener.h"

class CSpecialEquipContainer 
    : public CGoodsContainer
    , public CContainerListener
{
public:
    CSpecialEquipContainer(void);
    ~CSpecialEquipContainer(void);

	/// ���÷���װ����λ����
	void SetTrumpType(eTrumpType eType) { m_eTrumpType = eType; }
	/// ��ȡ����װ����λ����
	eTrumpType GetTrumpType(void) { return m_eTrumpType; }

    virtual bool Add(CBaseObject* pObject, tagPreviousContainer* pPreviousContainer, LPVOID pVoid = NULL);

    virtual bool Add(DWORD dwPostion, CGoods* pObject, tagPreviousContainer* pPreviousContainer, LPVOID pVoid = NULL);

	virtual CBaseObject* Find(const CGUID& guid);

    virtual CBaseObject* Remove(const CGUID& guid, LPVOID pVoid  = NULL );

    virtual CBaseObject* Remove(CBaseObject* pObject, LPVOID pVoid = NULL );

    virtual void Clear(LPVOID pVoid  = NULL );

    virtual void Release(void);
    
    virtual void TraversingContainer(CContainerListener* pListener);

    virtual bool IsGoodsExisted(DWORD dwGoodsBasePropertiesIndex);

    virtual bool OnObjectAdded(CContainer* pContainer, CBaseObject* pObj, DWORD dwAmount, LPVOID pVoid);

    virtual bool OnObjectRemoved(CContainer* pContainer, CBaseObject* pObj, DWORD dwAmount, LPVOID pVoid);

    virtual void AI();

    virtual bool IsFull();

    virtual bool QueryGoodsPosition(const CGUID& guid, DWORD& dwPosition);

    virtual bool QueryGoodsPosition(CGoods* pGoods, DWORD& dwPositon);

    virtual CGoods* GetGoods(DWORD dwPosition);

    DWORD GetGoodsAmount();

    bool FindPositionForGoods(CGoods* pGoods, DWORD& dwPosition);

    CGoods* GetTheFirstGoods(DWORD dwGoodsBasePropertiesIndex) { return NULL;}
    void GetGoods(DWORD dwGoodsBasePropertiesIndex, vector<CGoods*> vOut) {}
    DWORD GetGoodsAmountByName(const char* strName) { return 0; }
    DWORD GetGoodsAmountByOriginName(const char* strOriginName) { return 0; }
    DWORD GetGoodsAmountByType(int nType)   { return 0; }

    bool Swap(DWORD dwPosition, CGoods* pIn, CGoods** pOut, LPVOID pVoid = NULL);

    virtual bool Serialize(DBWriteSet& setWriteDB, bool b  = true  );
    bool SerializeForClient(DBWriteSet& setWriteDB, bool b  = true);
    
    virtual bool Unserialize(DBReadSet& setReadDB, bool b  = true  );


private:
	eTrumpType m_eTrumpType;			///< ����װ������

    static DWORD s_dwEquipLimit;        // ����
    vector<CGUID> m_vCells;
    hash_map<CGUID,CGoods*,hash_guid_compare>	m_mGoods;
    typedef hash_map<CGUID,CGoods*,hash_guid_compare>::iterator goodsIt;

};

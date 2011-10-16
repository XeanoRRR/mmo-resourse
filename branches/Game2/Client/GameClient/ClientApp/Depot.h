#pragma once
#include "Player.h"
#include "../../Public/Common/DepotDef.h"
class CGoods;
class CMessage;
// �ӱ����ĺ��������
#define		SUBPACK_ITEMS_H_MAXNUM					5U
// �ӱ����������������
#define		SUBPACK_ITEMS_V_MAXNUM					4U
// �ӱ��������и�������
#define		SUBPACK_ITEMS_MAXNUM					SUBPACK_ITEMS_H_MAXNUM * SUBPACK_ITEMS_V_MAXNUM


// �ֿ���ӱ�����λ����
#define		DEPOT_SUBPACK_MAXNUM					DEPOT_SUBPACK_NUM
// �ֿ����չ��λ ��λ��
#define		DEPOT_SUBPACK_BASEPOS					10001U

// �ֿ�������ӱ�����������
#define		DEPOT_SUBPACK_ITEMS_MAXNUM				DEPOT_SUBPACK_MAXNUM * SUBPACK_ITEMS_MAXNUM

// �ֿ����������������
#define		DEPOT_PRIMARY_MAXNUM					PRIMARY_SIZE

// �ֿ����ϵĺ����������
#define		DEPOT_CONFORMITY_H_MAXNUM				10U
// �ֿ����ϵ������������
#define		DEPOT_CONFORMITY_V_MAXNUM				13U
// �ֿ����ϵĸ�������
#define		DEPOT_CONFORMITY_ITEMS_MAXNUM			DEPOT_PRIMARY_MAXNUM + DEPOT_SUBPACK_ITEMS_MAXNUM

// �ֿ�����и�������
#define		DEPOT_ALL_ITEMS_MAXNUM					DEPOT_PRIMARY_MAXNUM + DEPOT_SUBPACK_ITEMS_MAXNUM + DEPOT_SUBPACK_MAXNUM

//**************************************************************************************
// ��Ʒ����
//**************************************************************************************
class	CGoodsContainer
{
    std::map<DWORD,CGoods*>		m_mapGoods;					// Goods ʵ���

    DWORD						m_dwContainerID;			// ����ID
    DWORD						m_numCubage;				// ����

public:
    CGoodsContainer();
    virtual ~CGoodsContainer();

    DWORD		GetContainerID()								{ return m_dwContainerID;			}
    void		SetContainerID( DWORD dwContainerID )			{ m_dwContainerID = dwContainerID;	}

    DWORD		GetCubage()										{ return m_numCubage;				}
    void		SetCubage( DWORD numCubage )					{ m_numCubage = numCubage;			}

    CGoods*		GetGoods( DWORD pos );
    CGoods*		GetGoods( const CGUID& guid );

    bool		AddGoods( CGoods* pGoods, DWORD pos );
    CGoods*		DelGoods( const CGUID& guidSrc, const CGUID& guidDest, long num );

    DWORD		GetPos( const CGUID& guid );
    DWORD		GetAmount();

    void		Decode( DBReadSet& setReadDB );
    void		Neaten( CMessage* pMsg );

    void		Clear();
};


//**************************************************************************************
// ��Ʒ��������
//**************************************************************************************
class	CGCMana
{
    std::map<DWORD,CGoodsContainer*>		m_mapGC;		// Goods������

public:
    bool				AddGC( DWORD id, DWORD numCubage );
    bool				DelGC( DWORD id );
    CGoodsContainer*	GetGC( DWORD id );

    bool				AddGoods( CGoods* pGoods, DWORD id, DWORD pos );
    CGoods*				DelGoods( const CGUID& guidSrc, const CGUID& guidDest, DWORD num );
    CGoods*				GetGoods( DWORD id, DWORD pos );
    CPlayer::tagGoods	GetGoods( const CGUID& guid );

    DWORD				GetAmount( DWORD id );

    void				Clear( bool bGoodsOnly = false );

    // ��������
    static CGCMana*		Inst()		{ return &m_Instance; }

private:
    CGCMana();
    virtual ~CGCMana();

    static	CGCMana		m_Instance;		// ����ʵ��
};



//**************************************************************************************
// ���й���
//**************************************************************************************
class	CDepot
{
    bool					m_bHasPassport;							// �Ƿ����õ�ͨ��֤
    tagPlayerDepotDataHead	m_stDepotInfo;							// �ֿ���Ϣ�ṹ

    char					m_szOldPW[ DEPOT_PASSWORD_ARRAY_LEN ];	// ����������
    char					m_szNewPW[ DEPOT_PASSWORD_ARRAY_LEN ];	// ����������

public:
    void					SetPassport( bool bPassport )	{ m_bHasPassport = bPassport;					}
    bool					HasPassport()					{ return m_bHasPassport;						}
    bool					HasPassWord()					{ return m_stDepotInfo.lHasPassword != 0;		}
    long					GetFreezeTime()					{ return m_stDepotInfo.lThawDate;				}
    long					GetNumOfSubItem()				{ return m_stDepotInfo.lUsableSubbackNum;		}
    long					GetPriceOfSubItem( long c )		{ return m_stDepotInfo.arr_lSubbackPrice[ c ];	}

    bool					IsDepotGC( DWORD id );

    void					SetOldPW( LPCTSTR szPW )		{ lstrcpy( m_szOldPW, szPW );					}
    void					SetNewPW( LPCTSTR szPW )		{ lstrcpy( m_szNewPW, szPW );					}
    LPCTSTR					GetOldPW()						{ return m_szOldPW;								}
    LPCTSTR					GetNewPW()						{ return m_szNewPW;								}

    //
    CGUID					GetGoldGUID();
    CGUID					GetSilverGUID();

    long					GetGoldAmount();
    long					GetSilverAmount();

    DWORD					GetSubPackCubage( DWORD idSubPack );

    // ����\���� ��Ϣ
    void					SendEnterMsg( LPCTSTR pText );
    void					RecvEnterMsg( CMessage* pMsg );

    void					SendSetPWMsg( LPCTSTR pText );
    void					RecvSetPWMsg( CMessage* pMsg );

    void					SendBuyItemMsg();
    void					RecvBuyItemMsg( CMessage* pMsg );

    void					RecvFreezeMsg( CMessage* pMsg );

    void					DecodeDepotHeadInfo( DBReadSet& setReadDB );
    void					DecodeDepotInfo( DBReadSet& setReadDB );

    //
    static bool				Compare( const CPlayer::tagGoods* pstGoods1, const CPlayer::tagGoods* pstGoods2 );

    // ��������
    static CDepot*			Inst()		{ return &m_Instance; }

private:
    CDepot();
    virtual ~CDepot();

    static	CDepot			m_Instance;		// ����ʵ��
};
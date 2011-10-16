/*DepotPageEx.cpp
*����: ����
*��������:2008-1-18
*/

#include "stdafx.h"
#include "Depot.h"
#include "Goods/Goods.h"
#include "../ClientApp/Other/GlobeSetup.h"
#include "../GameClient/Game.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//**************************************************************************************
// ��Ʒ����
//**************************************************************************************


/* 
*���ܣ����캯��
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-18
*/
CGoodsContainer::CGoodsContainer()
{
    m_dwContainerID	= 0xffffffff;
    m_numCubage	= 0;
}


/*
*���ܣ���������
*ϸ�������� �ͷ���Դ��
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-18
*/
CGoodsContainer::~CGoodsContainer()
{
    this->Clear();
}


/*
*���ܣ��������
*ϸ�������� �ͷ���Դ��
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-18
*/
void	CGoodsContainer::Clear()
{
    for( std::map<DWORD,CGoods*>::iterator  p = m_mapGoods.begin(); p != m_mapGoods.end(); p++ )
    {
        SAFE_DELETE( p->second );
    }

    m_mapGoods.clear();
}




/*
*���ܣ���ȡGoods
*ϸ�������� 
*������ pos
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-18
*/
CGoods*	CGoodsContainer::GetGoods( DWORD pos )
{
    std::map<DWORD,CGoods*>::iterator  p = m_mapGoods.find( pos );

    if( p != m_mapGoods.end() )
    {
        return p->second;
    }

    return NULL;
}



/*
*���ܣ���ȡGoods
*ϸ�������� 
*������ guid
*����ֵ�� Goods
*�޸�ʱ�䣺2008-1-18
*/
CGoods*	CGoodsContainer::GetGoods( const CGUID& guid )
{
    for( std::map<DWORD,CGoods*>::iterator  p = m_mapGoods.begin(); p != m_mapGoods.end(); p++ )
    {
        if( p->second->GetExID() == guid )
        {
            return p->second;
        }
    }

    return NULL;
}


/*
*���ܣ���ȡλ��
*ϸ�������� 
*������ guid
*����ֵ�� λ��
*�޸�ʱ�䣺2008-1-18
*/
DWORD	CGoodsContainer::GetPos( const CGUID& guid )
{
    for( std::map<DWORD,CGoods*>::iterator  p = m_mapGoods.begin(); p != m_mapGoods.end(); p++ )
    {
        if( p->second->GetExID() == guid )
        {
            return p->first;
        }
    }

    return 0xffffffff;
}



/*
*���ܣ���ȡ����
*ϸ�������� 
*������ ��
*����ֵ�� Goods����
*�޸�ʱ�䣺2008-1-18
*/
DWORD	CGoodsContainer::GetAmount()
{
    return (DWORD)m_mapGoods.size();
}



/*
*���ܣ����Goods
*ϸ�������� 
*������ pGoods	��Ʒ
*		pos		λ��
*����ֵ�� �Ƿ�ɹ�
*�޸�ʱ�䣺2008-1-18
*/
bool	CGoodsContainer::AddGoods( CGoods* pGoods, DWORD pos )
{
    if( pGoods )
    {
        // ��� ԭλ�÷��� ��ͬ����Ʒ�������
        CGoods* pOldGoods = this->GetGoods( pos );

        if( pOldGoods
            && pOldGoods->GetIndex() == pGoods->GetIndex()
            && ( pOldGoods->GetIndex() == CGlobeSetup::GetSetup()->dwMoneyIndex
            || pOldGoods->GetIndex() == CGlobeSetup::GetSetup()->dwSilverMoneyIndex
            || pOldGoods->GetNum() + pGoods->GetNum() <= pGoods->GetEffectVal( GAP_MAXOVERLAP, 1 ) ) )
        {
            pOldGoods->SetNum( pOldGoods->GetNum() + pGoods->GetNum() );

            SAFE_DELETE( pGoods );
        }
        // �����������Ʒ
        else
        {
            m_mapGoods[ pos ] = pGoods;
        }

        return true;
    }

    return false;
}



/*
*���ܣ�ɾ��Goods
*ϸ�������� 
*������ guidSrc		Ҫɾ����ƷID
*		guidDest	���������ƷID
*		num			ɾ������
*����ֵ�� ɾ������Ʒ
*�޸�ʱ�䣺2008-1-18
*/
CGoods*	CGoodsContainer::DelGoods( const CGUID& guidSrc, const CGUID& guidDest, long num )
{
    // ������Ʒ
    std::map<DWORD,CGoods*>::iterator  p;
    for( p = m_mapGoods.begin(); p != m_mapGoods.end(); p++ )
    {
        if( p->second->GetExID() == guidSrc )
            break;
    }

    // û�ҵ��򷵻�
    if( p == m_mapGoods.end() )
        return NULL;

    // 
    CGoods*	pNewGoods = NULL;
    CGoods* pOldGoods = p->second;

    // ��� �������ڵ�ǰ���õ���Ʒ ���ּ���
    if( pOldGoods && pOldGoods->GetNum() > (long)num )
    {
        // �Ƴ��Ĳ���
        pNewGoods = new CGoods( *pOldGoods );
        pNewGoods->SetNum( num );
        pNewGoods->SetExID( guidDest );

        // �����Ĳ���
        pOldGoods->SetNum( pOldGoods->GetNum() - num );
    }
    // ����ֱ��ȫ���Ƴ�
    else
    {
        pNewGoods = pOldGoods;

        m_mapGoods.erase( p );
    }

    return pNewGoods;
}




/*
*���ܣ�ͨ������Ʒ����
*ϸ�������� 
*������ ��
*����ֵ�� 
*�޸�ʱ�䣺2008-1-18
*/
void	CGoodsContainer::Decode( DBReadSet& setReadDB )
{
    // �����
    this->Clear();

    // ��ȡ ����
    long num = setReadDB.GetLongFromByteArray();

    for( long c = 0; c < num; c++ )
    {
        CPlayer::tagGoods	stGoods;

        stGoods.bChange	= setReadDB.GetByteFromByteArray() != 0;
        stGoods.lNum	= setReadDB.GetWordFromByteArray();
        stGoods.lPos	= setReadDB.GetByteFromByteArray();
        stGoods.pGoods	= new CGoods;

        // ���� Goods
        if( stGoods.pGoods->DecodeFromDataBlock( setReadDB ) )
        {
            // ����Goods
            this->AddGoods( stGoods.pGoods, stGoods.lPos );

            // ������װ
            WORD SuitID = (WORD)stGoods.pGoods->GetEffectVal( GAP_SUITID, 1 );			
            if( SuitID )
            {
                string stOrName = stGoods.pGoods->GetProperty()->strOrginName;					
                GetGame()->GetMainPlayer()->AddSuit( SuitID,stOrName );	
            }
        }
        // ʧ�ܾ�ɾ��
        else
        {
            SAFE_DELETE( stGoods.pGoods );
        }
    }
}





/*
*���ܣ����½��ӱ����ڵ���Ʒ�������г�����
*ϸ�������� 
*������ pMsg
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-23
*/
void CGoodsContainer::Neaten( CMessage* pMsg )
{
    if( CGoodsContainer* pGC = CGCMana::Inst()->GetGC( m_dwContainerID ) )
    {
        std::map<DWORD,CGoods*> m_mapGoodsTmp;

        // �����GOODS����
        DWORD num = pMsg->GetDWord();

        for( DWORD c = 0; c < num; c++ )
        {
            CGUID guidGoods;
            pMsg->GetGUID( guidGoods );

            if( CGoods* pGoods = pGC->GetGoods( guidGoods ) )
            {
                m_mapGoodsTmp[ pMsg->GetWord() ] = pGoods;
            }
            else
            {
                return;
            }
        }

        m_mapGoods.clear();
        m_mapGoods = m_mapGoodsTmp;

    }
}



//**************************************************************************************
// ��Ʒ��������
//**************************************************************************************

// ����
CGCMana	CGCMana::m_Instance;


/* 
*���ܣ����캯��
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-18
*/
CGCMana::CGCMana()
{
}


/*
*���ܣ���������
*ϸ�������� �ͷ���Դ��
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-18
*/
CGCMana::~CGCMana()
{
    this->Clear();
}


/*
*���ܣ���պ���
*ϸ�������� �ͷ���Դ��
*������ bGoodsOnly
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-18
*/
void	CGCMana::Clear( bool bGoodsOnly )
{
    for( std::map<DWORD,CGoodsContainer*>::iterator  p = m_mapGC.begin(); p != m_mapGC.end(); p++ )
    {
        if( p->second )
        {
            if( bGoodsOnly )
            {
                p->second->Clear();
            }
            else
            {
                SAFE_DELETE( p->second );
            }
        }
    }
}


/*
*���ܣ���ȡ����
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-18
*/
DWORD	CGCMana::GetAmount( DWORD id )
{
    if( CGoodsContainer* pGC = this->GetGC( id ) )
    {
        return pGC->GetAmount();
    }

    return 0;
}


/*
*���ܣ������Ʒ����
*ϸ�������� 
*������ id
*		numCubage
*����ֵ�� 
*�޸�ʱ�䣺2008-1-18
*/
bool	CGCMana::AddGC( DWORD id, DWORD numCubage )
{
    std::map<DWORD,CGoodsContainer*>::iterator  p = m_mapGC.find( id );

    if( p == m_mapGC.end() )
    {
        m_mapGC[ id ] = new CGoodsContainer;
        m_mapGC[ id ]->SetContainerID( id );
        m_mapGC[ id ]->SetCubage( numCubage );

        return true;
    }

    return false;
}



/*
*���ܣ�ɾ����Ʒ����
*ϸ�������� 
*������ id
*����ֵ�� 
*�޸�ʱ�䣺2008-1-18
*/
bool	CGCMana::DelGC( DWORD id )
{
    std::map<DWORD,CGoodsContainer*>::iterator  p = m_mapGC.find( id );

    if( p != m_mapGC.end() )
    {
        SAFE_DELETE( p->second );

        m_mapGC.erase( p );

        return true;
    }

    return false;
}



/*
*���ܣ���ȡ��Ʒ����
*ϸ�������� 
*������ id
*����ֵ�� 
*�޸�ʱ�䣺2008-1-18
*/
CGoodsContainer*	CGCMana::GetGC( DWORD id )
{
    std::map<DWORD,CGoodsContainer*>::iterator  p = m_mapGC.find( id );

    if( p != m_mapGC.end() )
    {
        return p->second;
    }

    return NULL;
}



/*
*���ܣ������Ʒ
*ϸ�������� 
*������ pGoods
*		id
*		pos
*����ֵ�� 
*�޸�ʱ�䣺2008-1-18
*/
bool	CGCMana::AddGoods( CGoods* pGoods, DWORD id, DWORD pos )
{
    if( CGoodsContainer* pGC = this->GetGC( id ) )
    {
        return pGC->AddGoods( pGoods, pos );
    }

    return false;
}



/*
*���ܣ�ɾ��Goods
*ϸ�������� 
*������ guidSrc		Ҫɾ����ƷID
*		guidDest	���������ƷID
*		num			ɾ������
*����ֵ�� ɾ������Ʒ
*�޸�ʱ�䣺2008-1-18
*/
CGoods*	CGCMana::DelGoods( const CGUID& guidSrc, const CGUID& guidDest, DWORD num )
{
    for( std::map<DWORD,CGoodsContainer*>::iterator  p = m_mapGC.begin(); p != m_mapGC.end(); p++ )
    {
        if( CGoods* pGoods = p->second->DelGoods( guidSrc, guidDest, num ) )
            return pGoods;
    }

    return NULL;
}


/*
*���ܣ���ȡ��Ʒ
*ϸ�������� 
*������ id
*		pos
*����ֵ�� 
*�޸�ʱ�䣺2008-1-18
*/
CGoods*	CGCMana::GetGoods( DWORD id, DWORD pos )
{
    std::map<DWORD,CGoodsContainer*>::iterator  p = m_mapGC.find( id );

    if( p != m_mapGC.end() )
    {
        return p->second->GetGoods( pos );
    }

    return NULL;
}


/*
*���ܣ���ȡ��Ʒ
*ϸ�������� 
*������ guid
*����ֵ�� 
*�޸�ʱ�䣺2008-1-18
*/
CPlayer::tagGoods	CGCMana::GetGoods( const CGUID& guid )
{
    CPlayer::tagGoods	stGoods;

    for( std::map<DWORD,CGoodsContainer*>::iterator  p = m_mapGC.begin(); p != m_mapGC.end(); ++p )
    {
        CGoodsContainer* pGC = p->second;

        if( stGoods.pGoods = pGC->GetGoods( guid ) )
        {
            stGoods.lType = pGC->GetContainerID();
            stGoods.lPos = pGC->GetPos( guid );
            stGoods.lNum = stGoods.pGoods->GetNum();
            break;
        }
    }

    return stGoods;
}


//**************************************************************************************
// ���й���
//**************************************************************************************

// ����
CDepot	CDepot::m_Instance;


/* 
*���ܣ����캯��
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-18
*/
CDepot::CDepot()
{
    CGCMana::Inst()->AddGC( eDCT_Primary, DEPOT_PRIMARY_MAXNUM );
    CGCMana::Inst()->AddGC( eDCT_Secondary, DEPOT_SUBPACK_MAXNUM );
    CGCMana::Inst()->AddGC( eDCT_Gold, 1 );
    CGCMana::Inst()->AddGC( eDCT_Silver, 1 );

    for( long c = 0; c < DEPOT_SUBPACK_MAXNUM; c++ )
    {
        CGCMana::Inst()->AddGC( eDCT_Secondary1 + c, 0 );
    }
}


/*
*���ܣ���������
*ϸ�������� �ͷ���Դ��
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-18
*/
CDepot::~CDepot()
{
}


/*
*���ܣ� �Ƿ��� �ֿ�����
*ϸ�������� 
*������ id
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-23
*/
bool	CDepot::IsDepotGC( DWORD id )
{
    return id > DEPOT_CONTAINER_TYPE_BEGIN_VALUE
        && id < DEPOT_CONTAINER_TYPE_END_VALUE;
}


/*
*���ܣ� 
*ϸ�������� 
*������ 
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-23
*/
CGUID	CDepot::GetGoldGUID()
{
    CGoods* pGoodsGold = CGCMana::Inst()->GetGoods( eDCT_Gold, 0 );

    return pGoodsGold ? pGoodsGold->GetExID() : NULL_GUID;
}



/*
*���ܣ� 
*ϸ�������� 
*������ 
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-23
*/
CGUID	CDepot::GetSilverGUID()
{
    CGoods* pGoodsSilver = CGCMana::Inst()->GetGoods( eDCT_Silver, 0 );

    return pGoodsSilver ? pGoodsSilver->GetExID() : NULL_GUID;
}



/*
*���ܣ� 
*ϸ�������� 
*������ 
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-23
*/
long	CDepot::GetGoldAmount()
{
    CGoods* pGoodsGold = CGCMana::Inst()->GetGoods( eDCT_Gold, 0 );

    return pGoodsGold ? pGoodsGold->GetNum() : 0;
}


/*
*���ܣ� 
*ϸ�������� 
*������ 
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-23
*/
long	CDepot::GetSilverAmount()
{
    CGoods* pGoodsSilver = CGCMana::Inst()->GetGoods( eDCT_Silver, 0 );

    return pGoodsSilver ? pGoodsSilver->GetNum() : 0;
}



/*
*���ܣ� ��ȡ�ӱ�������
*ϸ�������� 
*������ idSubPack	�ӱ������������
*����ֵ�� �ӱ���UI
*�޸�ʱ�䣺2008-1-25
*/
DWORD	CDepot::GetSubPackCubage( DWORD idSubPack )
{
    if( CGoods* pGoods =  CGCMana::Inst()->GetGC( eDCT_Secondary )->GetGoods( idSubPack - eDCT_Secondary1 + DEPOT_SUBPACK_BASEPOS ) )
    {
        return pGoods->GetEffectVal( GAP_PACK_CAPACITY, 1 );
    }

    return 0;
}



/*
*���ܣ� ���� �ֿ�ͷ��Ϣ
*ϸ�������� 
*������ setReadDB	����Դ
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-23
*/
void	CDepot::DecodeDepotHeadInfo( DBReadSet& setReadDB)
{
    // ��òֿ���Ϣͷ������������
    setReadDB.GetBufferFromByteArray( &m_stDepotInfo, sizeof(m_stDepotInfo) );

    // ��λ
    m_bHasPassport = false;
    ZeroMemory( m_szOldPW, DEPOT_PASSWORD_ARRAY_LEN );
    ZeroMemory( m_szNewPW, DEPOT_PASSWORD_ARRAY_LEN );
}


/*
*���ܣ� ���� �ֿ���Ϣ
*ϸ�������� 
*������ setReadDB	����Դ
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-23
*/
void	CDepot::DecodeDepotInfo( DBReadSet& setReadDB )
{
    // ��ȡ ���ֿ� ��Ʒ
    CGCMana::Inst()->GetGC( eDCT_Primary )->Decode( setReadDB );

    // ��չ��λ�� ����� ��Ҫͳһ��
    CGCMana::Inst()->GetGC( eDCT_Secondary )->Clear();

    BYTE numSubItemGoods = setReadDB.GetByteFromByteArray();

    for( BYTE c = 0; c < numSubItemGoods; c++ )
    {
        CGoods* pGoods	= new CGoods;

        // ����
        if( pGoods->DecodeFromDataBlock( setReadDB ) )
        {
            DWORD pos = DEPOT_SUBPACK_BASEPOS + (DWORD)setReadDB.GetByteFromByteArray();

            CGCMana::Inst()->GetGC( eDCT_Secondary )->AddGoods( pGoods, pos );
        }
        else	// ʧ�ܾ�ɾ��
        {
            setReadDB.GetByteFromByteArray();

            SAFE_DELETE( pGoods );
        }
    }

    // ��ȡ �ӱ��� ��Ʒ
    for( long c = 0; c < DEPOT_SUBPACK_MAXNUM; c++ )
    {
        CGCMana::Inst()->GetGC( eDCT_Secondary1 + c )->Decode( setReadDB );
    }


    // ���
    CGCMana::Inst()->GetGC( eDCT_Gold )->Clear();

    long numGold = setReadDB.GetLongFromByteArray();
    CGUID guidGold;
    setReadDB.GetBufferFromByteArray( guidGold );

    if( numGold && NULL_GUID != guidGold )
    {
        CGoods* pGoodsGold	= new CGoods;
        pGoodsGold->SetExID( guidGold );
        pGoodsGold->SetIndex( CGlobeSetup::GetSetup()->dwMoneyIndex );
        pGoodsGold->GetProperty()->dwType = GBT_MONEY;
        pGoodsGold->SetNum( numGold );

        CGCMana::Inst()->GetGC( eDCT_Gold )->AddGoods( pGoodsGold, 0 );
    }

    // ����
    CGCMana::Inst()->GetGC( eDCT_Silver )->Clear();

    long numSilver = setReadDB.GetLongFromByteArray();
    CGUID guidSilver;
    setReadDB.GetBufferFromByteArray( guidSilver );

    if( numSilver && NULL_GUID != guidSilver )
    {
        CGoods* pGoodsSilver	= new CGoods;
        pGoodsSilver->SetExID( guidSilver );
        pGoodsSilver->SetIndex( CGlobeSetup::GetSetup()->dwSilverMoneyIndex );
        pGoodsSilver->GetProperty()->dwType = GBT_SILVERMONEY;
        pGoodsSilver->SetNum( numSilver );

        CGCMana::Inst()->GetGC( eDCT_Silver )->AddGoods( pGoodsSilver, 0 );
    }
}




/*
*���ܣ� ���� ��Э��
*ϸ�������� 
*������ pText	�����ַ���
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-21
*/
void	CDepot::SendEnterMsg( LPCTSTR pText )
{
    CMessage msg( MSG_C2S_DEPOT_INPUT_PWD );

    msg.AddEx( (void*)pText, DEPOT_PASSWORD_SIZE );

    msg.Send();
}



/*
*���ܣ� ���� ��������Э��
*ϸ�������� 
*������ pText	�����ַ���
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-21
*/
void	CDepot::SendSetPWMsg( LPCTSTR pText )
{
    CMessage msg( MSG_C2S_DEPOT_REQUEST_CHANGE_PWD );

    msg.AddEx( (void*)m_szOldPW, DEPOT_PASSWORD_SIZE );
    msg.AddEx( (void*)pText, DEPOT_PASSWORD_SIZE );

    msg.Send();
}

/*
*���ܣ� ���� ��Э��
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-21
*/
void	CDepot::RecvEnterMsg( CMessage* pMsg )
{
    //CDepotPageEx* pDepotPageEx = GetGame()->GetCGuiEx()->GetDepotPageEx();

    // ��� �ɹ�ʧ��
    if( eDMC_Succeed == pMsg->GetLong() )
    {
        // ���ͨ��֤
        m_bHasPassport = true;

        // ����ֿ���Ʒ����
        DBReadSet setReadDB;
        pMsg->GetDBReadSet( setReadDB );
        this->DecodeDepotInfo( setReadDB );

        //pDepotPageEx->Open();
    }
    else
    {
        // �������"������󣬴�ʧ��"
        //GetGame()->GetCGuiEx()->GetMessageBoxPageEx()->m_lCallerID = -1;
        //GetGame()->GetCGuiEx()->GetMessageBoxPageEx()->Open( NULL, pDepotPageEx->GetStrRes( eIDS_PasswordError ), CMessageBoxPageEx::MSG_OK, true );

       // GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddOtherInfo( pDepotPageEx->GetStrRes( eIDS_PasswordError ) );
    }
}

/*
*���ܣ� ���� ��������Э��
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-21
*/
void	CDepot::RecvSetPWMsg( CMessage* pMsg )
{
   
    // ��� �ɹ�ʧ��
    if( eDMC_Succeed == pMsg->GetLong() )
    {
        // �������ַ������ʾû������
        m_stDepotInfo.lHasPassword = (long)strlen( this->GetNewPW() );

        this->SetOldPW( this->GetNewPW() );

        // "�������óɹ�"
    }
    else
    {
        // �������"��������ʧ��"
    }
}




/*
*���ܣ� ���� ������λЭ��
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-21
*/
void	CDepot::SendBuyItemMsg()
{
    CMessage msg( MSG_C2S_DEPOT_REQUEST_BUY_SUBPACK );

    msg.Send();
}



/*
*���ܣ� ���� ������λЭ��
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-1-21
*/
void	CDepot::RecvBuyItemMsg( CMessage* pMsg )
{
   

    // ��� �ɹ�ʧ��
    if( eGTC_Succeed == pMsg->GetLong() )
    {

    }
    else
    {
        // �������"������λʧ��"

    }
}



/*
*���ܣ� ���� ����Э��
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-2-2
*/
void	CDepot::RecvFreezeMsg( CMessage* pMsg )
{
    m_stDepotInfo.lThawDate = pMsg->GetLong();

    // "���ڶ������������룬�ֿ��ѱ����ᣬ��ȴ��ⶳ"
}



/*
*���ܣ� ����Ƚ�
*ϸ�������� 
*������ pstGoods1
*		pstGoods2
*����ֵ�� bool
*�޸�ʱ�䣺2009-1-1
*/
bool	CDepot::Compare( const CPlayer::tagGoods* pstGoods1, const CPlayer::tagGoods* pstGoods2 )
{
    if( !pstGoods1 || !pstGoods2 || !pstGoods1->pGoods || !pstGoods2->pGoods )
        return false;

    if( pstGoods1->pGoods->GetIndex() != pstGoods2->pGoods->GetIndex() )
    {
        LPCSTR	szOrginName1 = pstGoods1->pGoods->GetOrginName();
        LPCSTR	szOrginName2 = pstGoods2->pGoods->GetOrginName();

        if( szOrginName1 && szOrginName2 )
            return lstrcmp( szOrginName1, szOrginName2 ) < 0;
    }
    else if( pstGoods1->lNum != pstGoods2->lNum )
    {
        return pstGoods1->lNum > pstGoods2->lNum;
    }

    return false;
}

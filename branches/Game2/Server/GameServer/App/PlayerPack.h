/**
*	file:		PlayerPack.h
*	Brief:		�����༰������
*	detail:		���ڱ������ӱ��������ҵȵ�һЩ�������
*	Authtor:	��η
*	Date:		2010-07-05
*/

#pragma once
#include "Container/CVolumeLimitGoodsContainer.h"
#include "Container/CSubpackContainer.h"
#include "Container/CWallet.h"

class CPlayerPack
{
public:
    CPlayerPack(CPlayer* pPlayer);
    ~CPlayerPack(void);
    void Release();

    void SetOwner(CPlayer* pPlayer);

    void CodeToDataBlockForServer(DBWriteSet& setWriteDB);
    void CodeToDataBlockForClient(DBWriteSet& setWriteDB);

    void DecordeDataBlockFromServer(DBReadSet& setReadDB);

    BOOL SerializeSubpack(DBWriteSet& setWriteDB,CS2CContainerObjectMove::PLAYER_EXTEND_ID extendId);


    CSubpackContainer::tagSubpack* GetSubpack(int i) { return m_pSubpackContainer->GetSubpack(i); }
    CVolumeLimitGoodsContainer* GetOriginPack() {return m_pOrigGoodsContainer;}
    CSubpackContainer* GetSubpackContainer()    {return m_pSubpackContainer;}
    CWallet& GetWallet()        { return m_cWallet; }
    CWallet& GetSilverWallet()  { return m_cSilverWallet; }
    CWallet& GetGem()           { return m_cGem; }

private:
    CPlayer*                    m_pPlayer;                          // �������
    CVolumeLimitGoodsContainer* m_pOrigGoodsContainer;              // ����
    CSubpackContainer*          m_pSubpackContainer;                // �ӱ�������

   
    CWallet				m_cWallet;          // Ǯ��   (��Ԫ) 
    CWallet				m_cSilverWallet;    // ����Ǯ�� (�󶨹�Ԫ�����֮�䲻�ɽ���)   
    CWallet				m_cGem;             // ��ʯ
    CWallet             m_cSenbei;          // �ɱ�

};
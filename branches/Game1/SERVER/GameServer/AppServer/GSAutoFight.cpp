#include "StdAfx.h"
#include "GSAutoFight.h"
#include "Player.h"
#include "ServerRegion.h"


CTimerQueue<GSAutoFight*>	GSAutoFight::s_TimerQueue(4, Auto_StableTime_Space);
CTimerQueue<GSAutoFight*>   GSAutoFight::s_CommonTimerQueue(4,Auto_Common_Space);

GSAutoFight::GSAutoFight(void)
:m_pOwner(NULL)
,m_bTimerRunning(FALSE)
,m_dwDurabilityZeroAI(0)
,m_CommonTimerID(0)
,m_bCommonRunning(FALSE)
,m_bProtected(FALSE)
,m_dwTimerAI(0)
,m_dwStableHookReliveAI(0)
{

}

GSAutoFight::~GSAutoFight()
{
    CloseTimer();
    CloseCommonTimer();

    //�����ȶ��һ�
    tagAutoFightNode AutoFightNode;
    AutoFightNode.bOpen = 0;
    AutoFightNode.lFightTypeIndex = eAHT_STABLE;
    SetOneSetup(eAHT_STABLE,AutoFightNode);

    //������ͨ�һ�
    tagAutoFightNode AutoFightCommontNode;
    AutoFightCommontNode.bOpen = 0;
    AutoFightCommontNode.lFightTypeIndex = eAHT_COMMON;
    SetOneSetup(eAHT_COMMON,AutoFightCommontNode);

    m_bProtected = FALSE;
    m_bTimerRunning = FALSE;
    m_dwTimerAI = 0;
}

void GSAutoFight::SetOwner(CPlayer* pMainPlayer)
{
    if (pMainPlayer)
    {
        m_pOwner = pMainPlayer;
    }
}

CTimerQueue<GSAutoFight*>& GSAutoFight::GetTimerQueue()
{
    return s_TimerQueue;
}

CTimerQueue<GSAutoFight*>& GSAutoFight::GetCommonTimerQueue()
{
    return s_CommonTimerQueue;
}
//�ı丨���һ�����
LONG GSAutoFight::ChangeSetup(LONG lType, tagAutoFightNode &AutoFightNode, BOOL bSysOpt /* = FALSE */)
{
    LONG lRetValue = eASET_NotErr;
    if (NULL != m_pOwner)
    {
        switch(lType)
        {
        case eAHT_COMMON:
            {
                CServerRegion* pRegion = dynamic_cast<CServerRegion*>(m_pOwner->GetFather());
                if (pRegion != NULL)
                {
                    if (!pRegion->GetCommonHook())
                    {
                        lRetValue = eASET_NotMap;
                        return lRetValue;
                    }

                    //����
                    if (AutoFightNode.bOpen)
                    {
                        OpenCommonTimer();
                    }
                    else
                    {
                        if (m_bTimerRunning)
                        {
                            m_dwTimerAI = 0;
                            CloseTimer();
                        }
                        if (m_bCommonRunning)
                        {
                            CloseCommonTimer();
                            m_dwDurabilityZeroAI = 0;
                        }
                    }
                }
            }
            break;
        case eAHT_STABLE:
            {
                CServerRegion *pRegion = dynamic_cast<CServerRegion*>(m_pOwner->GetFather());
                if (pRegion != NULL)
                {
                    if (!pRegion->GetStableHook())
                    {
                        lRetValue = eASET_NotMap;
                        return lRetValue;
                    }
                    if (!AutoFightNode.bOpen)
                    {   
                        //�رգ������ȶ�ʱ��
                        if (m_bTimerRunning)
                            CloseTimer();
                        //�رչ��ж�ʱ��
                        if (m_bCommonRunning)
                            CloseCommonTimer();
                        //if (GetStableTime() != 0)
                        //    SetStableTime(GetStableTime() - m_dwTimerAI);
                        m_dwTimerAI = 0;
                        m_dwDurabilityZeroAI = 0;
                    }
                    else
                    {
                        //����
                        if (GetStableTime() != 0)
                        {
                            if (!m_bTimerRunning)
                                OpenTimer();
                            //�������ж�ʱ��
                            OpenCommonTimer();
                        }
                        else
                        {
                            //û���ȶ�ʱ��
                            AutoFightNode.bOpen = 0;
                            lRetValue = eASET_NoStableTime;
                            return lRetValue;
                            //AutoFightNode.lFightTypeIndex = eAHT_STABLE;
                            //lRetValue = SetOneSetup(lType,AutoFightNode);
                        }
                    }
                }
            }
            break;
        case eAHT_REPAIR:
            {
                if (AutoFightNode.bOpen)
                {
                    if (m_pOwner->GetSilver() > MAX_REPAIRMONMEY)
                    {
                        break;
                    }
                    else
                    {
                        //û�дﵽ���ҵ���
                        AutoFightNode.bOpen = 0;
                        lRetValue = eASET_NoMoney;
                        return lRetValue;
                    }
                }
            }
            break;
        default:
            break;
        }
        lRetValue = SetOneSetup(lType,AutoFightNode);
    }
    return lRetValue;
}

//!								��ʱ����ʱ��Ӧ
long GSAutoFight::OnTimeOut(DWORD timerid,DWORD curtime,const void* var)
{
    if (NULL != m_pOwner)
    {
        if (timerid != -1)
        {
            if (timerid == m_CommonTimerID)
            {
                BeginWork();
                return timerid;
            }
        }
        //����
        if (m_pOwner->IsDied())
        {
           m_dwTimerAI = 0;
           if (m_bProtected)
           {
               //�Ѿ��������ȶ�����״̬
               if (m_dwStableHookReliveAI==Auto_DieReliveTime)
               {
                   //ԭ�ظ���
                   m_pOwner->OnRelive(1);
                   m_dwStableHookReliveAI = 0;
               }
               else
               {
                   DWORD dwFrColor = 4294309170;
                   DWORD dwBkColor = 3435973836;
                   char strContend[256] = "";
                   _snprintf(strContend, 256, CStringReading::LoadString(IDS_GS_SERVERMESSAGE,STR_GS_SERVERMESSAGE_RELIVETIMER),Auto_DieReliveTime-m_dwStableHookReliveAI);

                   //����  PostPlayerInfo
                   CMessage msg(MSG_S2C_OTHER_ADDPLAYERINFO);
                   msg.Add((LONG)0);
                   msg.Add(dwFrColor);
                   msg.Add(dwBkColor);
                   msg.Add(strContend);
                   msg.SendToPlayer(m_pOwner->GetExID());

                    m_dwStableHookReliveAI++;
               }
           }
           return 0;
        }

        if (!GetStableTime())
        {
            //ʱ�䵽��
            //if (m_bProtected)
            //{      
                m_bProtected = FALSE;
                //�ȶ�ʱ��������û�дﵽ�ȶ�����״̬��ʱ�䣬�ͻ����Զ��л�����ͨ ���ر��ȶ�����״̬
                CMessage msg(MSG_S2C_SPRITE_AutoFight_ChangeStableTime);
                msg.Add( m_pOwner->GetType() );
                msg.Add( m_pOwner->GetExID() );
                msg.Add( "StableProtectedStatus" );
                msg.Add( (long)m_bProtected );
                msg.SendToPlayer(m_pOwner->GetExID());	// ���͵��ͻ���*/

           /* }
            else
            {
                //�ȶ�ʱ��������û�дﵽ�ȶ�����״̬��ʱ�䣬�л�����ͨ
                CMessage msg(MSG_S2C_SPRITE_AutoFight_ChangeStableTime);
                msg.Add(m_pOwner->GetType());
                msg.Add(m_pOwner->GetExID());
                msg.Add("ChangeToCommonMode");
                msg.SendToPlayer(m_pOwner->GetExID());
            }*/
            CloseTimer();
            CloseCommonTimer();
            
            m_dwDurabilityZeroAI = 0;
            m_dwTimerAI = 0;
            m_bTimerRunning = FALSE;

            //�����ȶ��һ�
             tagAutoFightNode AutoFightNode;
            AutoFightNode.bOpen = 0;
            AutoFightNode.lFightTypeIndex = eAHT_STABLE;
            SetOneSetup(eAHT_STABLE,AutoFightNode);

            SetStableTime(0);
            //���ñ����ʶ
            m_pOwner->SetSavePartFlag(m_pOwner->GetSavePartFlag()|SAVE_DETAIL_INNERHOOK);
        }
        else
        {
            if (m_dwTimerAI == Auto_EnterStableTime)
            {
                m_bProtected = TRUE;
                //�����ȶ�����״̬
                CMessage msg(MSG_S2C_SPRITE_AutoFight_ChangeStableTime);
                msg.Add( m_pOwner->GetType() );
                msg.Add( m_pOwner->GetExID() );
                msg.Add( "StableProtectedStatus" );
                msg.Add( (long)m_bProtected );
                msg.SendToPlayer(m_pOwner->GetExID());	// ���͵��ͻ���*/
            }
            m_dwTimerAI++;
            SetStableTime(GetStableTime() - 1);
            //���ñ����ʶ
            m_pOwner->SetSavePartFlag(m_pOwner->GetSavePartFlag()|SAVE_DETAIL_INNERHOOK);
        }
    }
    return 0;
}


void GSAutoFight::BeginWork()
{
    if (m_pOwner != NULL)
    {
        if (m_pOwner->IsDied())
        {
            return;
        }

        CGoods* pGoods = NULL;
        // ----------------------------
        //	16:����
        pGoods = m_pOwner -> m_cEquipment.GetGoods(CEquipmentContainer::EC_WEAPON );
        if (pGoods != NULL)
        {
            //��ǰ���;�ֵ
            LONG lCurrentDurability	= pGoods -> GetAddonPropertyValues( GAP_GOODS_MAXIMUM_DURABILITY, 2 );
            if (lCurrentDurability == 0)
            {
                //�������;�ֵΪ0���Ȳ鿴�ܲ��ܽ����Զ�����(�������Զ�����û���Լ����ҹ�����)
                const tagAutoFightNode* pAutoFightNode;
                pAutoFightNode = GetOneSetup((LONG)eAHT_REPAIR);
                if (pAutoFightNode->bOpen)
                {
                    //���Ҳ����ж�
                    if (CanAutoRepair())
                    {
                        return;
                    }
                }
                //û�п�����Ǯ�������Ͳ��ܽ����Զ������ˣ� 3���Ӻ��Զ��س�
                if (m_dwDurabilityZeroAI == Auto_ReturnCity)
                {
                    //�رն�ʱ��
                    CloseTimer();
                    CloseCommonTimer();

                    //���͹رտ������ͻ���
                    //�����ȶ��һ�
                    tagAutoFightNode AutoFightNode;
                    AutoFightNode.bOpen = 0;
                    AutoFightNode.lFightTypeIndex = eAHT_STABLE;
                    LONG lReValue = SetOneSetup(eAHT_STABLE,AutoFightNode);

                    CMessage msg(MSG_S2C_SPRITE_AutoFight_ChangeSetup_Re);
                    if (lReValue == eASET_NotErr)
                    {
                        msg.Add( (LONG)lReValue);
                        msg.Add(AutoFightNode.lFightTypeIndex);
                        msg.AddEx(&AutoFightNode, sizeof(tagAutoFightNode));
                        msg.SendToPlayer(m_pOwner->GetExID(), false);
                    }

                    CMessage msg1(MSG_S2C_SPRITE_AutoFight_ChangeSetup_Re);
                    AutoFightNode.lFightTypeIndex = eAHT_COMMON;
                    lReValue = SetOneSetup(eAHT_COMMON,AutoFightNode);
                    if (lReValue == eASET_NotErr)
                    {
                        msg1.Add( (LONG)lReValue);
                        msg1.Add(AutoFightNode.lFightTypeIndex);
                        msg1.AddEx(&AutoFightNode, sizeof(tagAutoFightNode));
                        msg1.SendToPlayer(m_pOwner->GetExID(), false);
                    }

                    CMessage msg2(MSG_S2C_SPRITE_AutoFight_ChangeSetup_Re);
                    AutoFightNode.lFightTypeIndex = eAHT_REPAIR;
                    lReValue = SetOneSetup(eAHT_REPAIR,AutoFightNode);
                    if (lReValue == eASET_NotErr)
                    {
                        msg2.Add( (LONG)lReValue);
                        msg2.Add(AutoFightNode.lFightTypeIndex);
                        msg2.AddEx(&AutoFightNode, sizeof(tagAutoFightNode));
                        msg2.SendToPlayer(m_pOwner->GetExID(), false);
                    }

                    m_dwTimerAI = 0;
                    //�س�
                    m_pOwner->SetState(CShape::STATE_DIED);
                    m_pOwner->OnRelive(0);
                    m_dwDurabilityZeroAI = 0;
                }
                else
                {
                    DWORD dwFrColor = 4294309170;
                    DWORD dwBkColor = 3435973836;
                    char strContend[256] = "";
                    _snprintf(strContend, 256, CStringReading::LoadString(IDS_GS_SERVERMESSAGE,STR_GS_SERVERMESSAGE_RETURNCITYTIMER),Auto_ReturnCity-m_dwDurabilityZeroAI);

                    //����  PostPlayerInfo
                    CMessage msg(MSG_S2C_OTHER_ADDPLAYERINFO);
                    msg.Add((LONG)0);
                    msg.Add(dwFrColor);
                    msg.Add(dwBkColor);
                    msg.Add(strContend);
                    msg.SendToPlayer(m_pOwner->GetExID());

                    m_dwDurabilityZeroAI+=3;
                }
            }
        }
    }
}

void GSAutoFight::OnTimerCancel(DWORD timerid,const void* var)
{
}

void GSAutoFight::OpenTimer()
{
    if(!m_bTimerRunning)
    {
        GetTimerQueue().Schedule(this, NULL, Auto_StableTime_Space, Auto_StableTime_Space);
        m_bTimerRunning = TRUE;
    }
}

void GSAutoFight::OpenDieReLiveTimer(DWORD timeValue)
{
    if (!m_bTimerRunning)
    {
        GetTimerQueue().Schedule(this, NULL, timeGetTime()+timeValue*60*1000,0);
        m_bTimerRunning = TRUE;
    }
}

void GSAutoFight::OpenCommonTimer()
{
    if (!m_bCommonRunning)
    {
        m_CommonTimerID = GetCommonTimerQueue().Schedule(this, NULL, Auto_Common_Space, Auto_Common_Space);
        m_bCommonRunning = TRUE;
    }
}
void GSAutoFight::CloseTimer()
{
    m_bTimerRunning = FALSE;
    m_bProtected = FALSE;

    GetTimerQueue().Cancel(this);
}

void GSAutoFight::CloseCommonTimer()
{
    m_bCommonRunning = FALSE;
    m_CommonTimerID = 0;
    GetCommonTimerQueue().Cancel(this);
}

//�Զ�����װ�������ж�
bool GSAutoFight::CanAutoRepair()
{
    if (m_pOwner)
    {
        if (m_pOwner->GetSilver() > MAX_REPAIRMONMEY)
        {
            return true;
        }
    }
    return false;
}
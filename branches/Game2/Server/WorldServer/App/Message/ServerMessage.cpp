#include "StdAfx.h"

#include "../AH/OrderMgr.h"
#include "../AntiCheat/QuestionManager.h"
#include "../Business/BusinessManager.h"
#include "../Country/CountryParam.h"
#include "../Country/CountryHandler.h"
#include "../DBEntity/Entity.h"
#include "../DBEntity/EntityGroup.h"
#include "../DBEntity/EntityManager.h"
#include "../FBusiness/FBConfiger.h"
#include "../GlobalRgnManager.h"
#include "../Goods/GoodsFactory.h"
#include "../Goods/IncrementShopManager.h"
#include "../Linkman/LinkmanSystem.h"
#include "../NewSkill/NewSkillFactory.h"
#include "../Pet/PetCtrl.h"
#include "../Player.h"
#include "../PlayerDepot.h"
#include "../PlayerMan/PlayerMan.h"
#include "../OrganizingSystem/OrganizingCtrl.h"
#include "../Rank/RankManager.h"
#include "../Script/VariableList.h"
#include "../Session/Session.h"
#include "../Session/SessionFactory.h"
#include "../WorldRegion.h"               
#include "../../../../Public/Common/OccupSkillList.h"
#include "../../../../Public/Common/StudySkillCondition.h"
#include "../../../../Public/Common/CreditSystem.h"
#include "../../../../Public/Common/DupliRegionSetup.h"
#include "../../../../Public/ServerPublic/Server/Common/WordsFilter.h"
#include "../../../../Public/DefSetup.h"
#include "../../../../Public/ServerPublic/Server/Common/VariableList.h"
#include "../../../../Public/ServerPublic/Server/Common/VarListUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HANDLE g_hSavingThread;

// ��Ӧ������Ϣ
void OnServerMessage(CMessage* pMsg)
{
    long lSocketID = pMsg->GetSocketID();
    switch(pMsg->GetType())
    {
    case MSG_DB2W_SERVER_SENDSETUP:
        {
            // �����ԭ������
            std::map<string, tagEntityBuildInfo*>::iterator deItr = GetGame().GetEntityManager()->GetObjAttrDef().begin();
            for(; deItr != GetGame().GetEntityManager()->GetObjAttrDef().end(); deItr++)
            {
                delete deItr->second;
            }
            GetGame().GetEntityManager()->GetObjAttrDef().clear();

            long objAttrDefNum = pMsg->GetLong();
            long bufSize = pMsg->GetLong();
            BYTE* buf = new BYTE[bufSize];
            pMsg->GetEx(buf, bufSize);
            long pos = 0;
            for(int i=0; i<objAttrDefNum; i++)
            {
                tagEntityBuildInfo* tBuildInfo = new tagEntityBuildInfo;
                tBuildInfo->DecodeFromByteArray(buf, pos, bufSize);
                GetGame().GetEntityManager()->GetObjAttrDef()[tBuildInfo->strComFlag] = tBuildInfo;
            }
            SAFE_DELETE_ARRAY(buf);

            GetNetHandler().ServerReady(NF_DB_SERVER);

            // ��ȡ��ʼ������
            if(!GetGame().GetInitRegionLoadFlag())
                GetGame().GetEntityManager()->CreateLoadRegionSession();
            if(!GetGame().GetInitPersonalHouseLoadFlag())
                GetGame().GetEntityManager()->CreateLoadPhRgnSession();
            if(!GetGame().GetInitVarLoadFlag())
                GetGame().GetEntityManager()->CreateLoadGlobalVarSession();
            if(!GetGame().GetInitFactionLoadFlag())
                GetGame().GetEntityManager()->CreateLoadFactionGroupSession();
            if(!GetGame().GetInitUnionLoadFlag())
                GetGame().GetEntityManager()->CreateLoadUnionGroupSession();

            if( !GetInst( AH::OrderMgr ).IsDBLoaded() )
            {
                GetGame().GetEntityManager()->CreateLoadAgentOrdersSession();
            }

            // ����Ƿ�����GS��������ɲ���DBS׼�����, �ɹ�����saveplayer������������
            bool isSaveFlag = CMessage::IsGSReady(-1);

            // ͬ��DBS��Ϸ����
            GetGame().DoSaveData(true);

            if(isSaveFlag && GetNetHandler().IsServerReady(NF_DB_SERVER))
            {	
                //DoSaveData();
                GetGame().ProcessDelaySavePlayer();
                GetGame().SendCdkeyToLoginServer(TRUE);
            }
            else
                GetGame().SendCdkeyToLoginServer(FALSE);

            // �����ȡ��ҵ���ݵ�����(֮ǰδ��DBװ��)
            if( GetInst( Business::CBusinessManager ).IsEnable() && 
                !GetInst( Business::CBusinessManager ).IsDBLoaded() )
            {
                GetGame().GetEntityManager()->CreateLoadBusinessSession();
            }

            GetGame().GetEntityManager()->CreateLoadLimitGoodsRecordSession();
        }
        break;
    case MSG_DB2W_SERVER_QUEST_RECONNECTION:
        {
            long msgid = pMsg->GetLong();
            // ɾ����ʱ��
            GetGame().DelMsgTimer(msgid);
           Log4c::Trace(ROOT_MODULE,GetText("WS_SVRMSG_1"));
        }
        break;
    case MSG_DB2W_SERVER_INIT_PLAYERDATABUFFER:
        {
            // ��ʼ�����а�����
            //      DWORD dwBegin = timeGetTime();
            bool bData = pMsg->GetChar() ? true:false;
            if(bData)
            {
                // update all     
                long lSize = pMsg->GetLong();
                for(long l=0; l<lSize; l++)
                {                
                    long lByteLen = pMsg->GetLong();
                    if(lByteLen)
                    {
                        BYTE *pData = new BYTE[lByteLen];
                        pMsg->GetEx(pData, lByteLen);
                        tagRankInfo *pRankInfo = new tagRankInfo();
                        long lPos = 0;
                        pRankInfo->DecordFromByteArray(pData, lPos);
                        SAFE_DELETE_ARRAY(pData);
                        if(pRankInfo == NULL)
                            continue;
                        GetGame().GetRankManager()->AddRankInfo(pRankInfo);      
                    }                                  
                }
            }
            else
            {
                // update single
                long lByteLen = pMsg->GetLong();
                if(lByteLen)
                {
                    BYTE *pData = new BYTE[lByteLen];
                    pMsg->GetEx(pData, lByteLen);
                    tagRankInfo *pRankInfo = new tagRankInfo();
                    long lPos = 0;
                    pRankInfo->DecordFromByteArray(pData, lPos);
                    SAFE_DELETE_ARRAY(pData);
                    if(pRankInfo)
                    {
                        GetGame().GetRankManager()->AddRankInfo(pRankInfo);                    
                    }
                }
            }
        }
        break;

        // ����server��Ϣ��WorldServer
    case MSG_S2W_SERVER_SENDSERVERINFO:
        {
            static long lGsSleepTime = 0; // Minutes

            // ����gameserver��ʶ
            bool reconnect = pMsg->GetChar() ? true:false;
            long msgLen = 0;
            DWORD port = pMsg->GetLong();
            char gameServerIP[256];
            pMsg->GetStr(gameServerIP, 256);

            GSConnection* gs = GetNetHandler().GetGS(gameServerIP, port);
            if (!gs)
            {
                Log4c::Trace(ROOT_MODULE,FormatText("WS_SVRMSG_2", gameServerIP, port));
                break;
            }

            char str[256];
            sprintf(str, "%s [%d]", gameServerIP, port);
            AppFrame::AddToList("list", str);

            if (reconnect)
            {
                // ȡ�����ݿ�
                DBReadSet setReadDB;
                pMsg->GetDBReadSet(setReadDB);

                // ����GSȫ�ֱ���ֵ
                BYTE byVarFlag = setReadDB.GetByteFromByteArray();
                if(1 == byVarFlag)
                {
                    VariableList *varList = GetGame().GetGeneralVariableList();
                    VarList::Decode( varList, setReadDB );
                }

                // ���������������
                long playerNum = setReadDB.GetLongFromByteArray();
                for(int i=0; i<playerNum; i++)
                {
                    bool bFlag = setReadDB.GetByteFromByteArray() == 0 ? false : true;
                    if(bFlag)
                    {
                        if (gs)
                        {
                            ++gs->receivedPlayerData;
                            ++gs->playerNum;
                        }

                        CGUID lPlayerID;
                        setReadDB.GetBufferFromByteArray(lPlayerID);

                        GetGame().DeleteMapPlayer(lPlayerID);
                        CPlayer* pPlayer = GetGame().NewRawPlayer(ePlayerGsReconnectOne);
                        if(pPlayer)
                        {
                            pPlayer->SetExID(lPlayerID);
                            pPlayer->DecodeFromDataBlock(setReadDB);
                            GetGame().AppendMapPlayer(pPlayer);
                            pPlayer->SetGsSocket(lSocketID);
                            if(GetNetHandler().IsServerReady(NF_DB_SERVER))
                            {
                                GetGame().GetEntityManager()->CreateSavePlayerDataSession(pPlayer->GetExID(), SAVE_DETAIL_ALL);

                                Account* pAcc = NULL;
                                if(pPlayer->GetAccount() != 0)
                                {
                                    pAcc = new Account;
                                    if (pAcc)
                                    {
                                        CGUID accGuid;
                                        CGUID::CreateGUID(accGuid);
                                        pAcc->SetGUID(accGuid);
                                        pAcc->SetName(pPlayer->GetAccount());
                                        if(!GetGame().GetEntityManager()->AddAccount(pAcc))
                                        {
                                            SAFE_DELETE(pAcc);
                                        }
                                        else
                                        {
                                            pAcc->SetCurGamedPlayerGuid(pPlayer->GetExID());
                                            pAcc->SetPlayerState(PLAYER_STATE_ENTERED_GAME);
                                        }
                                    }
                                }

                                CEntityGroup* lPlayer = (CEntityGroup*)GetGame().GetEntityManager()->NewBaseEntity(string("[loginplayer]"), pPlayer->GetExID());
                                if(pPlayer && pAcc)
                                {
                                    pAcc->AddLoginPlayer(lPlayer);
                                    GetGame().GetEntityManager()->CreateSaveLoginPlayerDataSession(pPlayer->GetExID());
                                }
                            }

                            GetOrganiCtrl()->OnPlayerEnterGame(pPlayer->GetExID());
                            LinkmanSystem::GetInstance().OnPlayerOnline(pPlayer);
                        }
                    }
                }
                Log4c::Trace(ROOT_MODULE,FormatText("WS_SVRMSG_3", gs->index, playerNum));

                // ���������������
                playerNum = setReadDB.GetLongFromByteArray();
                for(int i=0; i<playerNum; i++)
                {
                    bool bFlag = setReadDB.GetByteFromByteArray()==0?false:true;
                    if(bFlag)
                    {
                        CGUID lPlayerID;
                        setReadDB.GetBufferFromByteArray(lPlayerID);
                        GetGame().DeleteMapPlayer(lPlayerID);

                        CPlayer* pPlayer=GetGame().NewRawPlayer(ePlayerGsReconnectTwo);
                        if(pPlayer)
                        {
                            pPlayer->SetExID(lPlayerID);
                            pPlayer->DecodeFromDataBlock(setReadDB);
                            GetGame().PushSavePlayer(pPlayer);
                            pPlayer->SetGsSocket(lSocketID);
                        }
                    }
                }

                Log4c::Trace(ROOT_MODULE,FormatText("WS_SVRMSG_4", gs->index, playerNum));
                // ���븱������
                long rgnNum = setReadDB.GetLongFromByteArray();
                for(int i=0; i<rgnNum; i++)
                {
                    BYTE* RgnDataBuf = NULL;
                    // rgn data size
                    long dataSize = setReadDB.GetLongFromByteArray();
                    RgnDataBuf = new BYTE[dataSize];
                    setReadDB.GetBufferFromByteArray(RgnDataBuf, dataSize);
                    BYTE* RgnVarBuf = NULL;
                    // varlist size
                    long varSize = setReadDB.GetLongFromByteArray();
                    RgnVarBuf = new BYTE[varSize];
                    setReadDB.GetBufferFromByteArray(RgnVarBuf, varSize);
                    // �滻rgn
                    CWorldRegion* rgn = new CWorldRegion;
                    if(rgn)
                    {
                        long pos = 0;
                        rgn->DecordFromByteArray(RgnDataBuf, pos);
                        //rgn->Load();
                        CGlobalRgnManager::tagRegion* pTemplateRgn = NULL;
                        switch(rgn->GetRgnType())
                        {
                        case RGN_PERSONAL_HOUSE:
                            {
                                pTemplateRgn = GetGame().GetGlobalRgnManager()->GetTemplatePersonalHouseRegion(rgn->GetID());
                            }
                            break;
                        case RGN_PERSONAL:
                            {
                                pTemplateRgn = GetGame().GetGlobalRgnManager()->GetTemplatePersonalRegion(rgn->GetID());
                            }
                            break;
                        case RGN_TEAM:
                            {
                                pTemplateRgn = GetGame().GetGlobalRgnManager()->GetTemplateTeamRegion(rgn->GetID());
                            }
                            break;
                        }
                        if(pTemplateRgn && pTemplateRgn->pRegion)
                            rgn->CloneAllData(pTemplateRgn->pRegion);

                        pos = 0;
                        rgn->GetVariableList()->RemoveAll();
                        VarList::Decode( rgn->GetVariableList(), RgnVarBuf, varSize, pos );
                        GetGame().GetGlobalRgnManager()->GetRgnMap()[rgn->GetExID()] = rgn;
                        // gsid
                        rgn->SetGsid(gs->index);
                        if(rgn->GetRgnType() == RGN_PERSONAL)
                            GetGame().GetGlobalRgnManager()->AddPersonalRgnGUID(rgn->GetOwnerGUID(), rgn->GetExID());
                    }
                    SAFE_DELETE_ARRAY(RgnDataBuf);
                    SAFE_DELETE_ARRAY(RgnVarBuf);
                }
                Log4c::Trace(ROOT_MODULE,FormatText("WS_SVRMSG_5", gs->index));

                // ���븱����Ʒ����
                long PHRgnNum = setReadDB.GetLongFromByteArray();
                long bufSize = setReadDB.GetLongFromByteArray();
                BYTE* goodsBuf = new BYTE[bufSize];
                setReadDB.GetBufferFromByteArray(goodsBuf, bufSize);
                long pos = 0;
                for(int j=0; j<PHRgnNum; j++)
                {
                    CGUID phGUID;
                    _GetBufferFromByteArray(goodsBuf, pos, phGUID);
                    long goodsNum = _GetLongFromByteArray(goodsBuf, pos);
                    if(goodsNum)
                    {
                        CEntityGroup* dupRgn = NULL;
                        CEntityManager::DBEntityComponentMapItr dupItr = GetGame().GetEntityManager()->GetBaseEntityMap().find(string("[phrgngroup]"));
                        if(dupItr == GetGame().GetEntityManager()->GetBaseEntityMap().end())
                        {
                            Log4c::Warn(ROOT_MODULE, GetText("WS_SVRMSG_11"));
                            return;
                        }

                        if(dupItr->second.size() != 1) return;
                        CEntityGroup* pPhRgnGroup = dupItr->second.begin()->second;
                        map<CGUID, CBaseEntity*>::iterator rgnItr = pPhRgnGroup->GetEntityGroupMap().find(phGUID);
                        if(rgnItr == pPhRgnGroup->GetEntityGroupMap().end())
                        {
                            dupRgn = (CEntityGroup*)GetGame().GetEntityManager()->NewBaseEntity(string("[phrgn]"), phGUID);
                            pPhRgnGroup->GetEntityGroupMap()[phGUID] = dupRgn;
                        }
                        else
                            dupRgn = (CEntityGroup*)rgnItr->second;

                        if(dupRgn)
                        {
                            // ������˷��ݸ����ڵ���Ʒ
                            CEntityGroup* pGoodsGroup = (CEntityGroup*)GetGame().GetEntityManager()->FindLeafByComFlag(dupRgn, string("[phrgngoodsgroup]"));
                            if(pGoodsGroup)
                                pGoodsGroup->ReleaseChilds();
                        }

                        for(int i=0; i<goodsNum; i++)
                        {
                            struct PHGoods
                            {
                                // ��ƷGUID
                                CGUID guid;
                                // ��Ʒ����
                                char name[64];
                                // ����
                                int dir;
                                // X����
                                int width;
                                // Y����
                                int height;
                                // ����
                                float posX;
                                float posY;
                            };
                            PHGoods tGoods;
                            _GetBufferFromByteArray(goodsBuf, pos, &tGoods, sizeof(PHGoods));

                            CEntityGroup* pGoodsGroup = (CEntityGroup*)GetGame().GetEntityManager()->FindLeafByComFlag(dupRgn, string("[phrgngoodsgroup]"));
                            if(pGoodsGroup)
                            {
                                CEntity* goods = (CEntity*)GetGame().GetEntityManager()->NewBaseEntity(string("[phrgngoods]"), tGoods.guid);

                                goods->SetGuidAttr(string("guid"),  tGoods.guid);
                                goods->SetStringAttr(string("Name"), tGoods.name);
                                goods->SetLongAttr(string("Dir"), tGoods.dir);
                                goods->SetLongAttr(string("Width"), tGoods.width );
                                goods->SetLongAttr(string("Height"), tGoods.height);
                                goods->SetLongAttr(string("PosX"), (long)tGoods.posX);
                                goods->SetLongAttr(string("PosY"), (long)tGoods.posY);
                                goods->SetGuidAttr(string("DupRgnGUID"), dupRgn->GetGUID());

                                pGoodsGroup->AddChild(goods);
                            }
                        }	
                    }
                }
                SAFE_DELETE_ARRAY(goodsBuf);
                CMessage::MapGSIDSocket(gs->index, lSocketID);

                // ����Ƿ�����GS��������ɲ���DBS׼�����, �ɹ�����saveplayer������������
                if(CMessage::IsGSReady(-1) && GetNetHandler().IsServerReady(NF_DB_SERVER))
                {
                    GetGame().ProcessDelaySavePlayer();
                    GetGame().SendCdkeyToLoginServer(TRUE);
                }
                else
                {
                    GetGame().SendCdkeyToLoginServer(FALSE);
                }
            }
            else // �״�����WS
            {
                uint id = gs->index;
                CMessage::MapGSIDSocket(id,lSocketID);
                GetGame().InitPersonalHouseDataOnGS(id);

                // �ı���Դ
                GetGame().LoadEx(LOAD_TEXTRES, LOADFLAG_NIL, id);

                // �ɱ�����
                GetGame().LoadEx(LOAD_AHCONF, LOADFLAG_NIL, id);

                // �����
                //GetGame().LoadEx(LOAD_ANTICHEAT, LOADFLAG_NIL, id);

                // ��ҵ
                GetGame().LoadEx(LOAD_BUSINESS, LOADFLAG_NIL, id);

                // ����
                GetGame().LoadEx(LOAD_CONTACTS, LOADFLAG_NIL, id);

                // ����
                GetGame().LoadEx(LOAD_CONTRIBUTE, LOADFLAG_NIL, id);

                // ����
                GetGame().LoadEx(LOAD_COUNTRY, LOADFLAG_NIL, id);

                // �ֿ�����
                GetGame().LoadEx(LOAD_DEPOT, LOADFLAG_NIL, id);

                // ������Ϣ
                GetGame().LoadEx(LOAD_EMOTE, LOADFLAG_NIL, id);

                // ǿ��
                GetGame().LoadEx(LOAD_ENCHASE, LOADFLAG_NIL, id);

                // ��������
                GetGame().LoadEx(LOAD_FBUSINESS, LOADFLAG_NIL, id);

                // ����
                GetGame().LoadEx(LOAD_GATES, LOADFLAG_NIL, id);

                // ͨ�ñ���
                GetGame().LoadEx(LOAD_GENVARS, LOADFLAG_NIL, id);

                // ȫ�ֲ���
                GetGame().LoadEx(LOAD_GLOBALSETUP, LOADFLAG_NIL, id);

                // GM�б�
                GetGame().LoadEx(LOAD_GMLIST, LOADFLAG_NIL, id);

                // ��Ʒ��չ����
                GetGame().LoadEx(LOAD_GOODSEX, LOADFLAG_NIL, id);

                // ԭʼ��Ʒ�б�
                GetGame().LoadEx(LOAD_GOODSLIST, LOADFLAG_NIL, id);

                // �����䷽
                GetGame().LoadEx(LOAD_GOODSTAILOR, LOADFLAG_NIL, id);

                // ��������
                GetGame().LoadEx(LOAD_HITLEVEL, LOADFLAG_NIL, id);

                // ��ֵ�̵�
                GetGame().LoadEx(LOAD_INCREMENTSHOP, LOADFLAG_NIL, id);

                // ����
                GetGame().LoadEx(LOAD_INSTANCE, LOADFLAG_NIL, id);

                // ��־ϵͳ
                GetGame().LoadEx(LOAD_LOGSYS, LOADFLAG_NIL, id);

                // ������Ϣ
                GetGame().LoadEx(LOAD_SKILL, LOADFLAG_NIL, id);

                // ��������
                GetGame().LoadEx(LOAD_MONSTER, LOADFLAG_NIL, id);

                // ����AI
                GetGame().LoadEx(LOAD_MONSTERAI, LOADFLAG_NIL, id);

                // NPC�̵�
                GetGame().LoadEx(LOAD_NPCSHOP, LOADFLAG_NIL, id);

                // ����
                GetGame().LoadEx(LOAD_PET, LOADFLAG_NIL, id);

                // ��λ
                GetGame().LoadEx(LOAD_PHASE, LOADFLAG_NIL, id);

                // �츳
                GetGame().LoadEx(LOAD_GENIUSSYS, LOADFLAG_NIL, id);

                // ���
                GetGame().LoadEx(LOAD_PLAYER, LOADFLAG_NIL, id);

                // ��ҹ���
                GetGame().LoadEx(LOAD_PLAYERMGR, LOADFLAG_NIL, id);

                // ����
                GetGame().LoadEx(LOAD_QUEST, LOADFLAG_NIL, id);

                // רҵ����
                GetGame().LoadEx(LOAD_PROFESSION, LOADFLAG_NIL, id);

                // �������
                GetGame().LoadEx(LOAD_RELIVEGOODS, LOADFLAG_NIL, id);

                //������������
                GetGame().LoadEx(LOAD_REPUTATION, LOADFLAG_NIL, id);

                // �ű�
                GetGame().LoadEx(LOAD_SCRIPTS, LOADFLAG_NIL, id);

                // �ű�AI
                GetGame().LoadEx(LOAD_SCRIPTAI, LOADFLAG_NIL, id);

                // ���۹���
                GetGame().LoadEx(LOAD_SPEECH, LOADFLAG_NIL, id);

                // ����
                GetGame().LoadEx(LOAD_SPRITE, LOADFLAG_NIL, id);

                // ��װ
                GetGame().LoadEx(LOAD_SUIT, LOADFLAG_NIL, id);

                //ϵͳ�ʼ�
                GetGame().LoadEx(LOAD_SYSMAIL, LOADFLAG_NIL, id);

                // ���߳�������
                GetGame().LoadEx(SYNC_BRANCHRGN, LOADFLAG_NIL, id);

                // GS��Ϣ
                GetGame().LoadEx(SYNC_GSINFO, LOADFLAG_NIL, id);

                // ��ֵ�̵깺������
                GetGame().LoadEx(SYNC_INCLIMIT, LOADFLAG_NIL, id);

                // ����ģ��
                GetGame().LoadEx(SYNC_INSTANCEDATA, LOADFLAG_NIL, id);

                // �ű�
                GetGame().LoadEx(SYNC_SCRIPT, LOADFLAG_NIL, id);

                // ����
                GetGame().LoadEx(SYNC_VARIABLES, LOADFLAG_NIL, id);

                // WS��Ϣ
                GetGame().LoadEx(SYNC_WSINFO, LOADFLAG_NIL, id);

                // �����б�
                GetGame().LoadEx(LOAD_REGIONLIST, LOADFLAG_NIL, id);

                // ��������
                GetGame().LoadEx(LOAD_REGIONSETUP, LOADFLAG_NIL, id);

                // // ��Ŀ����
                //GetInst( AntiCheat::CQuestionManager ).SendQuestionsToGS( lSocketID );

                GetGame().LoadEx(LOAD_GLOBALSETUP, LOADFLAG_NIL, id);
                GetOrganiCtrl()->OnGsConnect(lSocketID);
                GetOrganiCtrl()->SendTeamSetupToGS(lSocketID);
                GetOrganiCtrl()->SendTeamDataToGS(lSocketID);

                // ������GameServer��ʽ��������Ϣ
                CMessage msg(MSG_W2S_SERVER_SENDSETUP);
                msg.Add((long)SYNC_END);
                msg.Add((long)lGsSleepTime);
                msg.SendToSocket(lSocketID);

                lGsSleepTime++;

                // ����Ƿ�����GS��������ɲ���DBS׼�����, �ɹ�����saveplayer������������
                bool isSaveFlag = CMessage::IsGSReady(-1);

                // ��������
                if (lGsSleepTime > CMessage::GetGSNum()) lGsSleepTime = 0;

                if(isSaveFlag && GetNetHandler().IsServerReady(NF_DB_SERVER))
                {
                    //DoSaveData();
                    GetGame().ProcessDelaySavePlayer();
                    GetGame().SendCdkeyToLoginServer(TRUE);
                }
                else
                    GetGame().SendCdkeyToLoginServer(FALSE);
            }
        }
        break;

    case MSG_W2W_SERVER_LOGINSERVERCLOSE:
        {
            Log4c::Warn(ROOT_MODULE, GetText("WS_SVRMSG_12"));
            GetNetHandler().CreateConnectThread(NF_LOGIN_SERVER);
        }
        break;

        // �Ʒѷ������Ͽ�
    case MSG_W2W_SERVER_COSTSERVERCLOSE:
        {
            Log4c::Warn(ROOT_MODULE, GetText("WS_SVRMSG_13"));
        }
        break;

    case MSG_W2W_RECONNECTLOGINSERVEROK:
        {

        }
        break;

    case MSG_W2W_RECONNECTCOSTSERVEROK:
        {

        }
        break;


        // �л�������
    case MSG_S2W_SERVER_CHANGEREGION:
        {
            // RgnType
            eRgnType rgnType = (eRgnType)pMsg->GetLong();
            // ��ͨ������ģ��ID���Բ��ҵ�GUID
            long templateID = pMsg->GetLong(); 

            CGUID guid; pMsg->GetGUID(guid);
            CGUID lRegionID;
            pMsg->GetGUID(lRegionID);

            CPlayer* pPlayer = GetGame().GetMapPlayer(guid);
            if(!pPlayer)
            {
                char szGuid[128];
                guid.tostring(szGuid);
               Log4c::Warn(ROOT_MODULE,FormatText("WS_SVRMSG_14", szGuid));
                // �л�����ʧ�� 
                CMessage msg(MSG_W2S_SERVER_CHANGEREGION);
                // �����֤����
                msg.Add((long)0xFFFFFFFF);
                msg.Add((char)0);	// ʧ��
                msg.Add(guid);
                msg.SendToSocket(lSocketID);
                return;
            }

            long lTileX = pMsg->GetLong();
            long lTileY = pMsg->GetLong();
            long lDir = pMsg->GetLong();
            long lUseGoods = pMsg->GetLong();
            long lRange = pMsg->GetLong();

            DBReadSet setReadDB;
            pMsg->GetDBReadSet(setReadDB);
            pPlayer->DecodeFromDataBlock(setReadDB);

            const char* szCdkey = pPlayer->GetAccount();
            Account* tLoginInfo = GetGame().GetEntityManager()->FindAccount(szCdkey);
            if(!tLoginInfo)
            {
                Log4c::Warn(ROOT_MODULE,FormatText("WS_SVRMSG_15", szCdkey));
                // �л�����ʧ�� 
                CMessage msg(MSG_W2S_SERVER_CHANGEREGION);
                // �����֤����
                msg.Add((long)0xFFFFFFFF);
                msg.Add((char)0);	// ʧ��
                msg.Add(guid);
                msg.SendToSocket(lSocketID);
                return;
            }
            if(tLoginInfo)
            {
                // ������֤��
                long lTmpSignCode = 0;
                GetGame().GenerateSignCodeByTime(lTmpSignCode);
                tLoginInfo->SetSignCode(lTmpSignCode);
                tLoginInfo->SetPlayerState(PLAYER_STATE_CHANGING_SERVER);
                tLoginInfo->SetLoginTimeoutValue(timeGetTime());
            }
            else
            {
                Log4c::Warn(ROOT_MODULE,FormatText("WS_SVRMSG_16", szCdkey));
                return;
            }

            // ��ͨ����
            if(rgnType == RGN_NORMAL)
            {
                CGlobalRgnManager::tagRegion* tRgn = GetGame().GetGlobalRgnManager()->GetTemplateNormalRegion(templateID);
                if(tRgn)
                    lRegionID = tRgn->pRegion->GetExID();
            }

            CWorldRegion* pRegion = GetGame().GetGlobalRgnManager()->FindRgnByGUID(lRegionID);
            long changeGSID = -1;
            if(pRegion)
                changeGSID = pRegion->GetGsid();

            if (!pRegion)
            {
                // �л�����ʧ�� 
                CMessage msg(MSG_W2S_SERVER_CHANGEREGION);
                // �����֤����
                msg.Add((long)tLoginInfo->GetSignCode());
                msg.Add((char)0);	// ʧ��
                msg.Add(guid);
                msg.SendToSocket(lSocketID);

                // ����һ�α�����Ϣ
                GetGame().GetEntityManager()->CreateSavePlayerDataSession(pPlayer->GetExID(), SAVE_DETAIL_ALL);
                GetGame().GetEntityManager()->CreateSaveLoginPlayerDataSession(pPlayer->GetExID());

                char szGuid[40] = {0};
                guid.tostring(szGuid);
//                 char pszLogingInfo[512]="";
//                 _snprintf(pszLogingInfo,512, AppFrame::GetText("WS_SERVERMESSAGE_CHANGERGNNORGNONGS"), templateID, szGuid);
//                 PutStringToFile("Login_WS_Info",pszLogingInfo);

                Log4c::Trace(ROOT_MODULE,FormatText("WS_SERVERMESSAGE_CHANGERGNNORGNONGS", templateID, szGuid));
                return;
            }

            GSConnection* gs = GetNetHandler().GetGS(changeGSID);
            if (!CMessage::IsGSReady(changeGSID))
            {
                // ����״̬
                tLoginInfo->SetPlayerState(PLAYER_STATE_BACKTOLOGIN);

                // ����һ�α�����Ϣ
                GetGame().GetEntityManager()->CreateSavePlayerDataSession(pPlayer->GetExID(), SAVE_DETAIL_ALL);
                GetGame().GetEntityManager()->CreateSaveLoginPlayerDataSession(pPlayer->GetExID());

                // �л�����ʧ��
                CMessage msg(MSG_W2S_SERVER_CHANGEREGION);
                // �����֤����
                msg.Add((long)tLoginInfo->GetSignCode());
                msg.Add((char)0);	// ʧ��
                msg.Add(guid);
                msg.SendToSocket(lSocketID);
                char szGuid[40] = {0};
                guid.tostring(szGuid);
//                 char pszLogingInfo[512]="";
//                 _snprintf(pszLogingInfo,512, AppFrame::GetText("WS_SERVERMESSAGE_CHANGERGNGSNOTRUN"), templateID, szGuid, changeGSID);
//                 PutStringToFile("Login_WS_Info",pszLogingInfo);

                Log4c::Trace(ROOT_MODULE,FormatText("WS_SERVERMESSAGE_CHANGERGNGSNOTRUN", templateID, szGuid, changeGSID));
                return;
            }

            pPlayer->SetRegionExID(lRegionID);
            pPlayer->SetPosXY(lTileX+0.5f, lTileY+0.5f);
            pPlayer->SetDir(lDir);
            pPlayer->SetRegionID(pRegion->GetID());
            pPlayer->SetCurRgnType(rgnType);

            // Online->Login
            // ��ӵ���½�б���
            pPlayer->SetPlayerLogin(true);
            pPlayer->SetFirstLogin(false);
            pPlayer->SetLoginTime(timeGetTime());

            // ����һ�α�����Ϣ
            GetGame().GetEntityManager()->CreateSavePlayerDataSession(pPlayer->GetExID(), SAVE_DETAIL_ALL);
            GetGame().GetEntityManager()->CreateSaveLoginPlayerDataSession(pPlayer->GetExID());

            // �л������ɹ�
            CMessage msg(MSG_W2S_SERVER_CHANGEREGION);
            // �����֤����
            msg.Add((long)tLoginInfo->GetSignCode());
            msg.Add((char)1);	// �ɹ�
            msg.Add(guid);
            msg.Add(gs->con.ip.c_str());
            msg.Add(gs->con.port);
            msg.SendToSocket(lSocketID);
        }
        break;
    case MSG_S2W_SERVER_ADDINFO:
        /**## ��������Ϣ������GameServer����� */
        {
            char	strName[24];
            char	strMsg[1024];
            DWORD	dwColor = 0;
            DWORD	dwBkColor = 0;
            CGUID	SenderGuid;

            /**## ������ʼ�� */
            ZeroMemory( strName, sizeof( strName ) );
            ZeroMemory( strMsg,  sizeof( strMsg )  );

            pMsg->GetStr( strName,24 );
            pMsg->GetStr( strMsg,1024 );
            dwColor = pMsg->GetDWord();
            dwBkColor = pMsg->GetDWord();
            pMsg->GetGUID(SenderGuid);

            CPlayer* pPlayer;
            pPlayer = GetGame().GetMapPlayer( strName );
            if( pPlayer )
                /**## �ҵ��û���������Ϣ���û����ڵķ����� */
                /**## �ɹ���LONG Reciver, char* Msg, DWORD Color, char* Sender    
                ## ʧ�ܣ�LONG Reciver(=0), char* Reciver, LONG Sender */
            {
                /**## ��ȡ�û���ǰ���ڵ�GameServer */
                long lGameServer =
                    GetGame().GetGameServerNumber_ByPlayerID( pPlayer->GetExID() );
                if( lGameServer )
                {
                    CMessage msg( MSG_W2S_SERVER_ADDINFO );
                    msg.Add( pPlayer->GetExID() );
                    msg.Add( strMsg );
					//��ɫֱ�Ӽ���strMsg�У��ͻ��˻��Զ�������ɫ
                    //msg.Add( dwColor );
                    //msg.Add( dwBkColor );
                    /**## ��д�����ߵ����� */
                    CPlayer* pSender = GetGame().GetMapPlayer( SenderGuid );
                    if( pSender )
                    {
                        msg.Add( const_cast<char*>( pSender->GetName() ) );
                    }
                    else
                    {
                        char strSender[256];
                        char szSenderGuid[40] = {0};
                        SenderGuid.tostring(szSenderGuid);
                        sprintf( strSender, "uid[%s]", szSenderGuid );
                        msg.Add( strSender );
                    }
                    msg.SendToGS( lGameServer );
                }
            }
            else
                /**## �Ҳ����û���������Ϣ�������� */
            {
                CPlayer* pSender = GetGame().GetMapPlayer( SenderGuid );
                if( pSender )
                {
                    long lGameServer =
                        GetGame().GetGameServerNumber_ByPlayerID( pSender->GetExID() );
                    if( lGameServer )
                    {
                        CMessage msg( MSG_W2S_SERVER_ADDINFO );
                        msg.Add( static_cast<long>(0) );
                        msg.Add( strName );
                        msg.Add( SenderGuid );
                        msg.SendToGS( lGameServer );
                    }
                }
            }
        }
        break;

        // ����ȫ�ֽű�������ֵ
    case MSG_S2W_SERVER_CHANGE_SCRIPT_VARIABLE:
        {
            char strVarName[256] = {0};
            pMsg->GetStr(strVarName, 256);
            if( strVarName )
            {
                long lPos = 0;
                bool ret = false;
                double nValue = 0;
                LONG64 llRet = 0;
                BYTE nFlag = 0;
                char szStr[1024] = {0};
                CGUID guid;

                nFlag = pMsg->GetByte(); // 0 ��ֵ
                if( eVarNumber == nFlag )
                {
                    lPos = pMsg->GetLong();
                    llRet = pMsg->GetLONG64();
                    nValue = *(double*)&llRet;
                    ret = GetGame().GetGeneralVariableList()->SetVarValue( strVarName, nValue, lPos );
                }
                else if( eVarString == nFlag ) // 1 �ַ���
                {
                    pMsg->GetStr(szStr, 1024);
                    ret = GetGame().GetGeneralVariableList()->SetVarValue(strVarName, tString( szStr ));
                }
                else if( eVarGuid == nFlag ) // 2 guid
                {
                    pMsg->GetGUID(guid);
                    ret = GetGame().GetGeneralVariableList()->SetVarValue(strVarName, guid);
                }
                CEntityManager::DBEntityComponentMapItr varMapItr = GetGame().GetEntityManager()->GetBaseEntityMap().find(string("[scriptvargroup]"));
                if( varMapItr == GetGame().GetEntityManager()->GetBaseEntityMap().end() )
                {
                    Log4c::Warn(ROOT_MODULE, GetText("WS_SVRMSG_17"));
                    return;
                }
                map<CGUID, CEntityGroup*>::iterator enItr = varMapItr->second.begin();
                if( enItr == varMapItr->second.end() )
                {
                    Log4c::Warn(ROOT_MODULE, GetText("WS_SVRMSG_18"));
                    return;
                }
                CEntityGroup* pVarGroup = (CEntityGroup*)enItr->second;
                if( pVarGroup )
                {
                    pVarGroup->ReleaseChilds();
                    GetGame().GetEntityManager()->CGlobalVarGroupToCGDBlobalVar(pVarGroup);
                }

                if( ret )
                {
                    CMessage msg(MSG_W2S_SERVER_CHANGE_SCRIPT_VARIABLE);
                    msg.Add(strVarName);
                    msg.Add((BYTE)nFlag);
                    if( eVarNumber == nFlag )
                    {
                        msg.Add((long)lPos);
                        msg.Add((LONG64)llRet);
                    }
                    else if( eVarString == nFlag )
                        msg.Add(szStr);
                    else if( eVarGuid == nFlag )
                        msg.Add(guid);
                    msg.SendToAllGS();
                }
            }
        }
        break;

        //##��Ϸ��������������������;ٱ�ɱ���ߵ���Ϣ
    case MSG_S2W_SERVER_REPORT_MURDERER:
        {
            LONG lSuffererType			= pMsg->GetLong();
            LONG lSuffererID			= pMsg->GetLong();
            LONG lMurdererType			= pMsg->GetLong();
            CGUID MurdererID;
            pMsg->GetGUID(MurdererID);
            //##��ѯ������Ǹ�������
            long lGameServer	= GetGame().GetGameServerNumber_ByPlayerID( MurdererID );
            if( lGameServer )
            {
                pMsg->SetType( MSG_W2S_SERVER_SYNC_MURDERER_COUNT );
                pMsg->SendToGS( lGameServer );
            }
            //##���������,�����ڱ��ز���
            else 
            {
                CPlayer* pPlayer = GetGame().GetMapPlayer( MurdererID );
                if( pPlayer )
                {
                    pPlayer->SetPkCount( pPlayer->GetPkCount() + 1 );
                    pPlayer->SetPkValue( pPlayer->GetPkValue() + 1 );
                }
                //##����Ҳ���,�Ƿ�ֱ���޸����ݿ�?
                //##Jar@2004-7-16
            }
        }
        break;

    case MSG_S2W_SERVER_UpdateRegionInfo:
        {
            long lRegionID = pMsg->GetLong();
            //CGame::tagRegion* ptagRegion = GetGame().GetRegion(lRegionID);
            CGlobalRgnManager::tagRegion* ptagRegion = GetGame().GetGlobalRgnManager()->GetTemplateNormalRegion(lRegionID);
            if (ptagRegion && ptagRegion->pRegion)
            {
                long lBufSize = pMsg->GetLong();
                BYTE *pRegionData = new BYTE[lBufSize];
                pMsg->GetEx(pRegionData,lBufSize);
                long lPos = 0;
                ptagRegion->pRegion->DecordFromByteArray(pRegionData,lPos);
                SAFE_DELETE_ARRAY(pRegionData);
            }
        }
        break;
        // DBServer�Ķ�ʱping
    case MSG_DB2W_SERVER_QUEST_PING:
        {
            GetGame().EndPingDS();

            if(GetGame().GetPingGsNum() >= CMessage::GetGSNum() && GetGame().FinishPingDS())
            {
                CMessage msg(MSG_W2L_SERVER_ANSWER_PING);
                msg.Add((long)1);
                msg.SendToServer(NF_LOGIN_SERVER);
            }
        }
        break;
        // LoginServer�Ķ�ʱping
    case MSG_L2W_SERVER_QUEST_PING:
        {
            GetGame().SetInPing(true);
            GetGame().ClearPingGameServerInfo();
            GetGame().SetLastPingGameServerTime(timeGetTime());
            GetGame().BeginPingDS();

            CMessage msg(MSG_W2S_SERVER_QUEST_PING);
            msg.Add((DWORD)pMsg->GetType());
            msg.SendToAllGS();

            CMessage msgDB(MSG_W2DB_SERVER_QUEST_PING);
            msgDB.SendToServer(NF_DB_SERVER);
            GetNetHandler().PlayerNumToServer();
        }
        break;

        // �յ�GameServer�ķ���
    case MSG_S2W_SERVER_ANSWER_PING:
        {
            char strIP[256];
            CGame::tagPingGameServerInfo stPingGame;
            stPingGame.dwPlayerNum = pMsg->GetLong();

            DWORD flag = pMsg->GetDWord();

            DWORD dwIP = pMsg->GetIP();
            sprintf(strIP, "%d.%d.%d.%d", LOBYTE(LOWORD(dwIP)), HIBYTE(LOWORD(dwIP)), LOBYTE(HIWORD(dwIP)), HIBYTE(HIWORD(dwIP)));

            GSConnection* gs = GetNetHandler().GetGS(pMsg->GetGSID());
            if(gs)
            {
                stPingGame.strIP = strIP;
                stPingGame.dwPort = gs->con.port;
                gs->playerNum = stPingGame.dwPlayerNum;
            }
            if(flag == MSG_L2W_SERVER_QUEST_PING)
            {
                GetGame().AddPingGameServerInfo(stPingGame);

                if(GetGame().GetPingGsNum() >= CMessage::GetGSNum() && GetGame().FinishPingDS())
                {
                    CMessage msg(MSG_W2L_SERVER_ANSWER_PING);
                    msg.Add((long)1);
                    msg.SendToServer(NF_LOGIN_SERVER);
                }
            }
        }
        break;
    case MSG_S2W_SERVER_CREATEMONSTER:
        {
            CGUID lRegionID;
            pMsg->GetGUID(lRegionID);

            pMsg->SetType( MSG_W2S_SERVER_CREATEMONSTER );
            long lIndex = -1;
            CWorldRegion* rgn = GetGame().GetGlobalRgnManager()->FindRgnByGUID(lRegionID);
            if(rgn)
                lIndex = rgn->GetGsid();
            pMsg->SendToGS(lIndex);
        }
        break;
    case MSG_S2W_SERVER_CREATEPHGOODS:
    case MSG_S2W_SERVER_UPDATEPHGOODS:
        {
            BYTE bType = pMsg->GetByte();

            CGUID playerID = NULL_GUID;

            CGUID guid;

            CGUID phRgnID;

            char name[64];

            long dir = 0;
            long width = 0;
            long height = 0;
            long posX = 0;
            long posY = 0;
            long templateID = 0;

            if(bType == 1) // ���ش���
            {
                pMsg->GetGUID(guid);
                pMsg->GetGUID(phRgnID);
                pMsg->GetStr(name, 64);
                dir = pMsg->GetLong();
                width = pMsg->GetLong();
                height = pMsg->GetLong();
                posX = pMsg->GetLong();
                posY = pMsg->GetLong();
                templateID = pMsg->GetLong();

                pMsg->SetType( MSG_W2S_SERVER_CREATEPHGOODS );
                long lIndex = pMsg->GetGSID();
                CWorldRegion* rgn = GetGame().GetGlobalRgnManager()->FindRgnByGUID(phRgnID);
                if(rgn)
                    playerID = rgn->GetOwnerGUID();
            }
            else
            {
                pMsg->GetGUID(guid);
                pMsg->GetGUID(phRgnID);
                pMsg->GetStr(name, 64);
                dir = pMsg->GetLong();
                width = pMsg->GetLong();
                height = pMsg->GetLong();
                posX = pMsg->GetLong();
                posY = pMsg->GetLong();
                templateID = pMsg->GetLong();

                CGUID phRgnID = NULL_GUID;
                CPlayer* player = GetGame().GetMapPlayer(phRgnID);
                if(player == NULL) return;

                phRgnID = player->GetPersonalHouseRgnGUID();

                pMsg->SetType( MSG_W2S_SERVER_CREATEPHGOODS );
                long lIndex = -1;
                CWorldRegion* rgn = GetGame().GetGlobalRgnManager()->FindRgnByGUID(phRgnID);
                if(rgn)
                {
                    lIndex = rgn->GetGsid();
                    playerID = rgn->GetOwnerGUID();
                }

                // �����ڷ�����Ϣ��GS�ϴ���goods
                pMsg->SendToGS(lIndex);
            }

            // ����PHGoods����
            CEntityGroup* dbDupRgn = NULL;
            CEntityManager::DBEntityComponentMapItr itr = GetGame().GetEntityManager()->GetBaseEntityMap().find(string("[phrgngroup]"));
            if(itr != GetGame().GetEntityManager()->GetBaseEntityMap().end())
            {
                if(itr->second.size() != 1)
                {
                    Log4c::Warn(ROOT_MODULE, GetText("WS_SVRMSG_19"));
                    return;
                }

                map<CGUID, CBaseEntity*>::iterator rgnItr = itr->second.begin()->second->GetEntityGroupMap().find(phRgnID);
                if(rgnItr != itr->second.begin()->second->GetEntityGroupMap().end())
                {
                    dbDupRgn = (CEntityGroup*)rgnItr->second;
                }
            }
            else
            {
                dbDupRgn = (CEntityGroup*)GetGame().GetEntityManager()->NewBaseEntity(string("[phrgn]"), phRgnID);
                itr->second.begin()->second->AddChild(dbDupRgn);
            }

            if(dbDupRgn)
            {
                CEntity* rgnGoods = (CEntity*)GetGame().GetEntityManager()->NewBaseEntity(string("[phrgngoods]"), NULL_GUID);
                rgnGoods->SetGUID(guid);
                rgnGoods->SetGuidAttr(string("DupRgnGUID"),		dbDupRgn->GetGUID());
                rgnGoods->SetGuidAttr(string("guid"),			guid);
                rgnGoods->SetStringAttr(string("Name"),			name);
                rgnGoods->SetLongAttr(string("Dir"),			dir);
                rgnGoods->SetLongAttr(string("Width"),			width);
                rgnGoods->SetLongAttr(string("Height"),			height);
                rgnGoods->SetLongAttr(string("PosX"),			posX);
                rgnGoods->SetLongAttr(string("PosY"),			posY);
                if(dbDupRgn->GetEntityGroupMap().size() != 1)
                {
                    Log4c::Warn(ROOT_MODULE, GetText("WS_SVRMSG_20"));
                    return;
                }
                CEntityGroup* pRgnGoodsGroup = (CEntityGroup*)GetGame().GetEntityManager()->FindLeafByComFlag(dbDupRgn, string("[phrgngoodsgroup]"));
                if(pRgnGoodsGroup)
                    pRgnGoodsGroup->AddChild(rgnGoods);
            }
        }
        break;
    case MSG_S2W_SERVER_DELETEPHGOODS:
        {
            BYTE bType = pMsg->GetByte();
            CGUID lRegionID;

            CGUID guid;

            char name[64];
            pMsg->GetStr(name, 64);
            long dir = 0;
            long width = 0;
            long height = 0;
            long posX = 0;
            long posY = 0;

            pMsg->GetGUID(lRegionID);
            pMsg->GetGUID(guid);
            pMsg->GetStr(name, 64);
            //dir = pMsg->GetLong();
            width = pMsg->GetLong();
            height = pMsg->GetLong();
            posX = pMsg->GetLong();
            posY = pMsg->GetLong();

            long lIndex = -1;
            CWorldRegion* rgn = GetGame().GetGlobalRgnManager()->FindRgnByGUID(lRegionID);
            if(rgn)
                lIndex = rgn->GetGsid();

            // ��PHGoods������ɾ��
            bool isFindFlag = false;
            CEntityGroup* dbDupRgn = NULL;
            CEntityManager::DBEntityComponentMapItr itr = GetGame().GetEntityManager()->GetBaseEntityMap().find(string("[phrgngroup]"));
            if(itr != GetGame().GetEntityManager()->GetBaseEntityMap().end())
            {
                if(itr->second.size() == 0)
                {
                    Log4c::Warn(ROOT_MODULE, GetText("WS_SVRMSG_21"));
                    return;
                }
                map<CGUID, CEntityGroup*>::iterator rgnItr = itr->second.find(lRegionID);
                if(rgnItr != itr->second.end())
                {
                    dbDupRgn = rgnItr->second;
                }
            }
            else
            {
                Log4c::Warn(ROOT_MODULE, GetText("WS_SVRMSG_22"));
            }
            if(dbDupRgn)
            {
                if(dbDupRgn->GetEntityGroupMap().size() == 0)
                {
                    Log4c::Warn(ROOT_MODULE, GetText("WS_SVRMSG_23"));
                    return;
                }
                CEntityGroup* pRgnGoodsGroup = (CEntityGroup*)GetGame().GetEntityManager()->FindLeafByComFlag(dbDupRgn, string("[phrgngoodsgroup]"));
                if(pRgnGoodsGroup)
                    pRgnGoodsGroup->ReleaseLeaf(guid);
            }
            pMsg->SetType( MSG_W2S_SERVER_DELETEPHGOODS );
            pMsg->SendToGS(lIndex);
        }
        break;
    case MSG_S2W_SERVER_SAVEPERSONALHOUSE:
        {
            long bufSize = pMsg->GetLong();
            BYTE *byteArray = new BYTE[bufSize];
            pMsg->GetEx(byteArray, bufSize);

            long pos = 0;
            CGUID rgnGUID;
            _GetBufferFromByteArray(byteArray, pos, rgnGUID); // ������ID

            // �ҵ���Ӧ��GS��RgnManager
            CWorldRegion* rgn = GetGame().GetGlobalRgnManager()->FindRgnByGUID(rgnGUID);

            long GSID = CMessage::GetGSIDBySocket(lSocketID);

            if(rgn)
            {	
                CGUID guid;
                _GetBufferFromByteArray(byteArray, pos, guid, bufSize);
                char name[128];
                memset(name, 0, sizeof(name));
                _GetStringFromByteArray(byteArray, pos, name, bufSize);

                CGUID ownerID;
                _GetBufferFromByteArray(byteArray, pos, ownerID, bufSize);
                long templateID = _GetLongFromByteArray(byteArray, pos, bufSize);

                long lResourceID = _GetLongFromByteArray(byteArray, pos, bufSize);
                VarList::Decode( rgn->GetVariableList(), byteArray, bufSize, pos );

                CEntityGroup* dupRgn = NULL;
                CEntityManager::DBEntityComponentMapItr phRgnMapItr = GetGame().GetEntityManager()->GetBaseEntityMap().find(string("[phrgngroup]"));
                if(phRgnMapItr != GetGame().GetEntityManager()->GetBaseEntityMap().end())
                {
                    if(phRgnMapItr->second.size() == 0)
                    {
                        Log4c::Warn(ROOT_MODULE, GetText("WS_SVRMSG_24"));
                        return;
                    }
                    map<CGUID, CEntityGroup*>::iterator rgnItr = phRgnMapItr->second.find(rgnGUID);
                    if(rgnItr != phRgnMapItr->second.end())
                    {
                        dupRgn = rgnItr->second;
                    }
                    else
                    {
                        dupRgn = (CEntityGroup*)GetGame().GetEntityManager()->NewBaseEntity(string("[phrgn]"), rgnGUID);
                        GetGame().GetEntityManager()->CPhRgnToCDBPhRgn(rgn, dupRgn);
                        dupRgn->SetGuidAttr(string("OwnerGUID"),  ownerID);
                        dupRgn->SetGuidAttr(string("guid"),  rgnGUID);
                    }
                }
                else
                {
                    Log4c::Warn(ROOT_MODULE, GetText("WS_SVRMSG_25"));
                }

                if(dupRgn != NULL)
                {
                    // ������Ʒ
                    long goodsNum = _GetLongFromByteArray(byteArray, pos, bufSize);
                    if (goodsNum != 0)
                    {
                        struct PHGoods
                        {
                            // ��ƷGUID
                            CGUID guid;
                            // ��Ʒ����
                            char name[64];
                            // ����
                            int dir;
                            // X����
                            int width;
                            // Y����
                            int height;
                            // ����
                            float posX;
                            float posY;
                        };

                        for(int i=0; i<goodsNum; i++)
                        {
                            CEntity* tdrGoods = (CEntity*)GetGame().GetEntityManager()->NewBaseEntity(string("[phrgngoods]"), NULL_GUID);
                            PHGoods phGoods;
                            _GetBufferFromByteArray(byteArray, pos, &phGoods, sizeof(PHGoods), bufSize);
                            tdrGoods->SetGuidAttr(string("guid"),  phGoods.guid);
                            tdrGoods->SetGuidAttr(string("DupRgnGUID"),  dupRgn->GetGUID());
                            tdrGoods->SetStringAttr(string("Name"),  phGoods.name);
                            tdrGoods->SetLongAttr(string("Dir"),  phGoods.dir);
                            tdrGoods->SetLongAttr(string("Width"),  phGoods.width);
                            tdrGoods->SetLongAttr(string("Height"),  phGoods.height);
                            tdrGoods->SetLongAttr(string("PosX"),  (long)phGoods.posX);
                            tdrGoods->SetLongAttr(string("PosY"),  (long)phGoods.posY);
                            tdrGoods->SetGUID(phGoods.guid);

                            CEntityGroup* pRgnGoodsGroup = (CEntityGroup*)GetGame().GetEntityManager()->FindLeafByComFlag(dupRgn, string("[phrgngoodsgroup]"));
                            if(pRgnGoodsGroup)
                                pRgnGoodsGroup->AddChild(tdrGoods);
                        }
                    }
                    dupRgn->SetLongAttr(string("GSID"),  GSID);
                    Log4c::Trace(ROOT_MODULE,FormatText("WS_SVRMSG_26", goodsNum));
                }

                GetGame().GetEntityManager()->CreateSavePhRgnSession(dupRgn);
            }

            SAFE_DELETE_ARRAY(byteArray);
        }
        break;
    case MSG_S2W_SERVER_LOADPERSONALHOUSE:
        {
            CGUID OwnerID;
            pMsg->GetGUID(OwnerID);
            long GSID = CMessage::GetGSIDBySocket(lSocketID);
            //			GetGame().SendLoadPersonalHouse(GSID, OwnerID);
        }
        break;
        // GS�Ľű����͵���Ϣ,��Ҫ��Ӧ����
    case MSG_S2W_SERVER_PLAYER_PERSONALHOUSEGUID:
        {
            long scriptID = pMsg->GetLong();// Script ID

            // player name
            char playerName[128];
            pMsg->GetStr(playerName, 128);

            // PH GUID
            CGUID guid;

            char szPHGuid[128]; // ���ص�PH GUID ������
            pMsg->GetStr(szPHGuid, 128);

            // �����ڴ��в���
            CPlayer* pPlayer = GetGame().GetMapPlayer(playerName);
            if(pPlayer)// ��ͬ����
            {
                // ���ظ�GS
                CMessage msg(MSG_W2S_SERVER_PLAYER_PERSONALHOUSEGUID);
                msg.Add(scriptID);// Script ID
                msg.Add(playerName); // player name
                msg.Add(szPHGuid);// ���ص�PH GUID ������
                msg.Add(pPlayer->GetPersonalHouseRgnGUID());// // PH GUID
                msg.SendToSocket(lSocketID);
                break;
            }
        }
        break;
    }
}

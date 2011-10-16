#include "stdafx.h"

#include "SmManage.h"
#include "UserManage.h"
#include "PackManage.h"
#include "GmSystem.h"

#include "../../../../../Public/ServerPublic/Server/BaseDef/InfoType.h"

VOID OnSucJsonMessage(CMessage* pMsg)
{
    Json::Value root;
    char szTmp[1024];
    pMsg->GetEx(szTmp,1024);
    Json::Reader jReader;
    jReader.parse(szTmp, root);
    switch( pMsg->GetType())
    {
        //! �����½
    case MSG_SUC2SCC_LOGIN_Request_Login:
        {
            tagUserLogin UserLoginInfo;           
            UserLoginInfo.dwVersion = root["version"].asInt();
            strcpy(UserLoginInfo.szName, root["cdkey"].asString().c_str());
            strcpy((char*)UserLoginInfo.szMD5Pwd, root["psw"].asString().c_str());      

            UserManage::GetInstance().OnUserLogin(UserLoginInfo, pMsg->GetSocketID());
        }
        break;
    case MSG_SUC2SCC_LOGIN_Request_ChangePwdQuest:
        {
            tagChangePWD ChangePWD;
            strcpy((char*)ChangePWD.szMD5PwdNew, root["newPsw"].asString().c_str());
            strcpy((char*)ChangePWD.szMD5PwdOld, root["oldPsw"].asString().c_str());

            UserManage::GetInstance().OnUserChangePwd(ChangePWD, pMsg->GetSocketID());
        }
        break;
    case MSG_SUC2SCC_INFO_Request_Show:
        {
            LONG lUserID = pMsg->GetSocketID();
            OnlineUser *pUser = UserManage::GetInstance().FindUser(lUserID);
            if(NULL == pUser) return;
            if(!pUser->HasPurview(ePT_Scout2))
            {
                pUser->SendInfo("Purview not enough!");
                return;
            }

            //! ��ȡ��Ϣ
            DWORD	dwSmID		= root["smID"].asInt();
            DWORD	dwGappID	= root["gappID"].asInt();
            DWORD	dwInfoType	= root["infoType"].asInt();
            DWORD	dwBeginTime	= root["beginTime"].asDouble();
            DWORD	dwEndTime	= root["endTime"].asDouble();

            SM *pSM = SmManage::GetInstance().FindSM(dwSmID);
            if(NULL != pSM)
            {
                Gapp *pGapp = pSM->FindGapp(dwGappID);
                if(NULL != pGapp)
                {
                    pGapp->FindGameInfoToUser(dwInfoType, dwBeginTime, dwEndTime, lUserID);
                    return;
                }
            }

            pUser->SendInfo("can't find aim!");
        }
        break;
    case MSG_SUC2SCC_INFO_Request_New:
        {
            LONG lUserID = pMsg->GetSocketID();
            OnlineUser *pUser = UserManage::GetInstance().FindUser(lUserID);
            if(NULL == pUser) return;
            if(!pUser->HasPurview(ePT_Scout2))
            {
                pUser->SendInfo("Purview not enough!");
                return;
            }

            //! ��ȡ��Ϣ
            DWORD	dwSmID		= root["smID"].asInt();
            DWORD	dwGappID	= root["gappID"].asInt();
            DWORD	dwInfoType	= root["infoType"].asInt();

            SM *pSM = SmManage::GetInstance().FindSM(dwSmID);
            if(NULL != pSM)
            {
                Gapp *pGapp = pSM->FindGapp(dwGappID);
                if(NULL != pGapp)
                {
                    CMessage msg(MSG_SCC2SM_INFO_SUC_Request_GameInfo);
                    msg.Add(lUserID);
                    msg.Add(dwGappID);
                    msg.Add(dwInfoType);
                    msg.SendToSm(pSM->GetSocket());
                    return;
                }
            }

            pUser->SendInfo("can't find aim!");
        }
        break;
        //! ���롢�˳����ƿռ�
    case MSG_SUC2SCC_CTRL_Request_InCtrlRoom:
        {
            //! �ҵ��Ϸ��û�
            LONG lUserID = pMsg->GetSocketID();
            OnlineUser *pUser = UserManage::GetInstance().FindUser(lUserID);
            if(NULL == pUser) return;
            if(!pUser->HasPurview(ePT_ServerControl2))
            {
                pUser->SendInfo("Purview not enough!");
                return;
            }

            //! ��ȡ��Ϣ            
            DWORD	dwSmID		= root["smID"].asInt();
            DWORD	dwGappID	= root["gappID"].asInt();
            BOOL	bIn	        = root["isIn"].asBool();
            //! 
            SM *pSM = SmManage::GetInstance().FindSM(dwSmID);
            if(NULL != pSM)
            {
                Gapp *pGapp = pSM->FindGapp(dwGappID);
                if(NULL != pGapp)
                {
                    if(pGapp->InCtrlRoom(lUserID, bIn))
                    {
                        CMessage msg(MSG_SCC2SUC_CTRL_Re_InCtrlRoom);
                        /*msg.Add((DWORD)TRUE);
                        msg.Add((DWORD)bIn);
                        msg.Add(dwSmID);
                        msg.Add(dwGappID);*/                        
                        Json::Value var;
                        var["result"] = true;
                        var["bIn"] = bIn;
                        var["smID"] = (int)dwSmID;
                        var["gappID"] = (int)dwGappID;
                        msg.AddJson(var);
                        msg.SendToSuc(lUserID);


                        if(bIn)
                        {
                            pUser->AddCtrlGapp(dwGappID, dwSmID);
                            ActionLogManage::GetInstance().ActionLog(dwSmID, dwGappID, UserManage::GetInstance().FindUserName(lUserID).c_str(), "user[%s] login gapp console.", pUser->GetName());
                        }
                        else
                        {
                            pUser->DelCtrlGapp(dwGappID);
                            ActionLogManage::GetInstance().ActionLog(dwSmID, dwGappID, UserManage::GetInstance().FindUserName(lUserID).c_str(), "user[%s] login gapp console.", pUser->GetName());
                        }
                        return;
                    }
                }
            }

            pUser->SendInfo("can't find aim!");
        }
        break;
        //! �������������ر�GAPP
    case MSG_SUC2SCC_CTRL_Request_SHUTDOWN:
    case MSG_SUC2SCC_CTRL_Request_START:
    case MSG_SUC2SCC_CTRL_Request_RESTART:
        {
            //! �ҵ��Ϸ��û�
            LONG lUserID = pMsg->GetSocketID();
            const OnlineUser *pUser = UserManage::GetInstance().FindUser(lUserID);
            if(NULL == pUser) return;
            if(!pUser->HasPurview(ePT_ServerControl2))
            {
                {
                    pUser->SendInfo("Purview not enough!");
                    return;
                }
            }

            //! ��ȡ��Ϣ            
            DWORD	dwSmID		= root["smID"].asInt();
            DWORD	dwGappID	= root["gappID"].asInt();
            DWORD	dwCtrlType	= root["ctrlType"].asInt();

            //! ȷ������ͻظ�����Ϣ����
            DWORD dwReType		= 0;
            DWORD dwRequestType = 0;

            const char *pCtrl = NULL;
            switch( pMsg->GetType())
            {
            case MSG_SUC2SCC_CTRL_Request_SHUTDOWN:
                {
                    dwReType		= MSG_SCC2SUC_CTRL_Re_SHUTDOWN;
                    dwRequestType	= MSG_SCC2SM_CTRL_Request_SHUTDOWN;
                    pCtrl = "turn off";
                }
                break;
            case MSG_SUC2SCC_CTRL_Request_START:
                {
                    dwReType		= MSG_SCC2SUC_CTRL_Re_START;
                    dwRequestType	= MSG_SCC2SM_CTRL_Request_START;
                    pCtrl = "turn on";
                }
                break;
            case MSG_SUC2SCC_CTRL_Request_RESTART:
                {
                    dwReType		= MSG_SCC2SUC_CTRL_Re_RESTART;
                    dwRequestType	= MSG_SCC2SM_CTRL_Request_START;
                    pCtrl = "reset";
                }
                break;
            default:
                return;
            }

            SM *pSM = SmManage::GetInstance().FindSM(dwSmID);
            if(NULL != pSM)
            {
                Gapp *pGapp = pSM->FindGapp(dwGappID);
                if(NULL != pGapp)
                {					
                    if (lUserID == pGapp->GetRoomUser())
                    {
                        CMessage msgRequest(dwRequestType);
                        msgRequest.Add(dwGappID);
                        msgRequest.SendToSm(pSM->GetSocket());

                        CMessage msg(dwReType);
                        /*msg.Add((DWORD)TRUE);
                        msg.Add(dwSmID);
                        msg.Add(dwGappID);
                        msg.Add(dwCtrlType);*/
                        Json::Value var;
                        var["result"] = true;
                        var["smID"] = (int)dwSmID;
                        var["gappID"] = (int)dwGappID;
                        var["ctrlType"] = (int)dwCtrlType;
                        msg.AddJson(var);
                        msg.SendToSuc(lUserID);

                        ActionLogManage::GetInstance().ActionLog(dwSmID, dwGappID, UserManage::GetInstance().FindUserName(lUserID).c_str(), "user[%s] %s GAPP[]", pUser->GetName(), pCtrl);
                        return;
                    }
                }
            }

            pUser->SendInfo("can't find aim!");
        }
        break;
        //! �ϴ�һ�����°�
    case MSG_SUC2SCC_CTRL_Request_UpLoadPack:
        {
            //! �ҵ��Ϸ��û�
            LONG lUserID = pMsg->GetSocketID();
            const OnlineUser *pUser = UserManage::GetInstance().FindUser(lUserID);
            if(NULL == pUser) return;
            if(!pUser->HasPurview(ePT_Update)) return;
            
            DBReadSet dbReadSet;
            pMsg->GetDBReadSet(dbReadSet);
            PackManage::GetInstance().OnUserUploadData(dbReadSet, pMsg->GetSocketID());
        }
        break;
        //! ����һ��GAPP
    case MSG_SUC2SCC_CTRL_Request_UpdateGapp:
        {
            //! �ҵ��Ϸ��û�
            LONG lUserID = pMsg->GetSocketID();
            const OnlineUser *pUser = UserManage::GetInstance().FindUser(lUserID);
            if(NULL == pUser) return;
            if(!pUser->HasPurview(ePT_ServerControl2))
            {
                pUser->SendInfo("Purview not enough!");
                return;
            }

            //! ��ȡ��Ϣ            
            DWORD	dwSmID		= root["smID"].asInt();
            DWORD	dwGappID	= root["gappID"].asInt();
            BYTE	cToNew		= root["toNew"].asInt();

            SM *pSM = SmManage::GetInstance().FindSM(dwSmID);
            if(NULL != pSM)
            {
                Gapp *pGapp = pSM->FindGapp(dwGappID);
                if(NULL != pGapp)
                {					
                    if (lUserID == pGapp->GetRoomUser())
                    {
                        //! ���͵�SM
                        CMessage msgToSM(MSG_SCC2SM_CTRL_Request_UpdateGapp);
                        msgToSM.Add(dwGappID);
                        msgToSM.Add(cToNew);
                        msgToSM.SendToSm(pSM->GetSocket());

                        //! ���͵�SUC
                        CMessage msg(MSG_SCC2SUC_CTRL_Re_UpdateGapp);
                        /*msg.Add((DWORD)TRUE);
                        msg.Add(dwSmID);
                        msg.Add(dwGappID);*/
                        Json::Value var;
                        var["result"] = true;
                        var["smID"] = (int)dwSmID;
                        var["gappID"] = (int)dwGappID;
                        msg.AddJson(var);
                        msg.SendToSuc(lUserID);
                        return;
                    }
                }
            }

            pUser->SendInfo("can't find aim!");
        }
        break;
        //! �����ض�Gappԭʼ�ļ���
    case MSG_SUC2SCC_CTRL_Request_ReLoadGapp:
        {
            //! �ҵ��Ϸ��û�
            LONG lUserID = pMsg->GetSocketID();
            const OnlineUser *pUser = UserManage::GetInstance().FindUser(lUserID);
            if(NULL == pUser) return;
            if(!pUser->HasPurview(ePT_ServerControl2)) 
            {
                pUser->SendInfo("Purview not enough!");
                return;
            }

            //! ��ȡ��Ϣ            
            DWORD	dwSmID		= root["smID"].asInt();
            DWORD	dwGappID	= root["gappID"].asInt();

            SM *pSM = SmManage::GetInstance().FindSM(dwSmID);
            if(NULL != pSM)
            {
                Gapp *pGapp = pSM->FindGapp(dwGappID);
                if(NULL != pGapp)
                {					
                    if (lUserID == pGapp->GetRoomUser())
                    {
                        //! SUC��Ϣ
                        CMessage msgSuc(MSG_SCC2SUC_CTRL_Re_ReLoadGapp);
                        Json::Value var;
                      //  msgSuc.Add(dwSmID);
                      //  msgSuc.Add(dwGappID);
                        var["smID"] = (int)dwSmID;
                        var["gappID"] = (int)dwGappID;

                        //! SM��Ϣ
                        CMessage msg(MSG_SCC2SM_CTRL_Re_ServerPack);
                        DBWriteSet dbWriteSet;
                        msg.GetDBWriteSet(dbWriteSet);

                        dbWriteSet.AddToByteArray(dwGappID);
                        dbWriteSet.AddToByteArray(pGapp->GetType());
                        if(PackManage::GetInstance().FindAndGetHighestPack(pGapp->GetType(), dbWriteSet))
                        {
                          //  msgSuc.Add((BYTE)TRUE);
                            var["result"] = true;
                        }
                        else
                        {
                          //  msgSuc.Add((BYTE)FALSE);
                            var["result"] = false;
                        }
                        msgSuc.AddJson(var);

                        msg.SendToSm(pSM->GetSocket());
                        msgSuc.SendToSuc(lUserID);
                    }
                }
            }

            pUser->SendInfo("can't find aim!");
        }
        break;
    case MSG_SUC2SCC_CTRL_Request_GappAction:
        {
            //! �ҵ��Ϸ��û�
            LONG lUserID = pMsg->GetSocketID();
            const OnlineUser *pUser = UserManage::GetInstance().FindUser(lUserID);
            if(NULL == pUser) return;
            if(!pUser->HasPurview(ePT_ServerControl2))
            {
                pUser->SendInfo("Purview not enough!");
                return;
            }

            //! ��ȡ��Ϣ            
            DWORD	dwSmID		= root["smID"].asInt();
            DWORD	dwGappID	= root["gappID"].asInt();
			DWORD	dwType		= root["nType"].asInt();
			DWORD dwLoop = 0, dwBKIndex =0;
			char szText[1024];
			DWORD dwFlag = 0;
			if(dwType == WS_TOPNEWS)
			{
				dwLoop = root["nLoop"].asDouble();
				strcpy((char*)szText, root["news"].asString().c_str());
				dwBKIndex = root["BKIndex"].asDouble();
				
			}
			else if(dwType == WS_RELOADFILE)
			{
				// flag ��ӦWS����ض�ö��ֵ
				dwFlag = root["flag"].asInt();
			}

            SM *pSM = SmManage::GetInstance().FindSM(dwSmID);
            if(NULL != pSM)
            {
                Gapp *pGapp = pSM->FindGapp(dwGappID);
                if(NULL != pGapp)
                {					
                    if (lUserID == pGapp->GetRoomUser())
                    {
						CMessage msg(MSG_SCC2SM_CTRL_Request_GappAction);
						msg.Add(dwSmID);
						msg.Add(dwGappID);
						msg.Add(dwType);
						if(dwType == WS_TOPNEWS)
						{
							msg.Add(dwLoop);
							msg.AddEx(szText, 1024);
							msg.Add(dwBKIndex);

						}
						else if(dwType == WS_RELOADFILE)
						{
							msg.Add(dwFlag);
						}
						msg.SendToSm(pSM->GetSocket());                        
                        return;
                    }
                }
            }

            pMsg->SetType(MSG_SCC2SUC_CTRL_Re_GappAction);
            pMsg->SendToSuc(lUserID);
        }
        break;
        //! �ϴ�һ���ļ�
    case MSG_SUC2SCC_CTRL_Request_UploadFile:
        {
            LONG lUserID = pMsg->GetSocketID();
            const OnlineUser *pUser = UserManage::GetInstance().FindUser(lUserID);
            if(NULL == pUser) return;
            if(!pUser->HasPurview(ePT_Update)) return;

            DBReadSet dbReadSet;
            pMsg->GetDBReadSet(dbReadSet);

            PackManage::GetInstance().OnUserUploadFile(dbReadSet, pMsg->GetSocketID());
        }
        break;
        //! ��ȡ�����ļ��б�
    case MSG_SUC2SCC_INFO_Request_UpdateFile:
        {
            //! �ҵ��Ϸ��û�
            LONG lUserID = pMsg->GetSocketID();
            const OnlineUser *pUser = UserManage::GetInstance().FindUser(lUserID);
            if(NULL == pUser) return;
            if(!pUser->HasPurview(ePT_Update))
            {
                pUser->SendInfo("Purview not enough!");
                return;
            }

            DWORD dwGappID = pMsg->GetDWord();
            PackManage::GetInstance().SendFileInfoListToClient(dwGappID, pMsg->GetSocketID());
        }
        break;
    case MSG_SUC2SCC_CTRL_Request_SMAction:
        {
            //! �ҵ��Ϸ��û�
            LONG lUserID = pMsg->GetSocketID();
            const OnlineUser *pUser = UserManage::GetInstance().FindUser(lUserID);
            if(NULL == pUser) return;
            if(!pUser->HasPurview(ePT_ServerControl2))
            {
                pUser->SendInfo("Purview not enough!");
                return;
            }

            //! ��ȡ��Ϣ            
            DWORD	dwSmID		= root["smID"].asInt();
            DWORD	dwGappID	= root["gappID"].asInt();

            SM *pSM = SmManage::GetInstance().FindSM(dwSmID);
            if(NULL != pSM)
            {
                Gapp *pGapp = pSM->FindGapp(dwGappID);
                if(NULL != pGapp)
                {					
                    if (lUserID == pGapp->GetRoomUser())
                    {
                        pMsg->SetType(MSG_SCC2SM_CTRL_Request_SMAction);
                        pMsg->SendToSm(pSM->GetSocket());
                        return;
                    }
                }
            }
        }
        break;
        //! GM��ع���
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //! Gm������˳�Ws������
    case MSG_SUC2SCC_MANAGE_Request_GmToWs:
        {
            //! �ҵ��Ϸ��û�
            LONG lUserID = pMsg->GetSocketID();
            const OnlineUser *pUser = UserManage::GetInstance().FindUser(lUserID);
            if(NULL == pUser) return;

            
            BOOL IsIn = root["isIn"].asBool();      
            if (IsIn)
            {
                DWORD	dwSmID		= root["smID"].asInt();
                DWORD	dwGappID	= root["gappID"].asInt();

                SM *pSM = SmManage::GetInstance().FindSM(dwSmID);
                if(NULL != pSM)
                {
                    Gapp *pGapp = pSM->FindGapp(dwGappID);
                    if(NULL != pGapp)
                    {					
                        if (eGT_WS == pGapp->GetType())
                        {
                            GmSystem::GetInstance().GmInWs(dwSmID, dwGappID, pUser);
                        }
                    }
                }
            }
            else
            {
                GmSystem::GetInstance().GmOutWs(lUserID);
            }
        }
        break;
        //! �鿴�ٱ�����
    case MSG_SUC2SCC_MANAGE_Request_VeiwReport:
        {
            LONG UserID = pMsg->GetSocketID();
            GmSystem::GetInstance().OnViewReport(UserID);
        }
        break;
        //! �鿴����������
    case MSG_SUC2SCC_MANAGE_Request_VeiwResult:
        {
            LONG UserID = pMsg->GetSocketID();
            tm tmDate;
            memset(&tmDate, 0, sizeof(tm));            
            tmDate.tm_year = root["year"].asInt();
            tmDate.tm_mon = root["month"].asInt();
            tmDate.tm_mday = root["day"].asInt();

            DWORD BeginDate = (DWORD)mktime(&tmDate);
            GmSystem::GetInstance().OnViewResult(UserID, BeginDate);
        }
        break;
        //! ���������Ϊ����
    case MSG_SUC2SCC_MANAGE_Request_ScoutPlayer:
        {
            LONG UserID = pMsg->GetSocketID();
            
            bool IsWatch = root["isWatch"].asBool();

            if(IsWatch)
            {
                char szPlayerName[GAPP_NAME_SZIE] = {0};
                strcpy((char*)szPlayerName, root["playerName"].asString().c_str());
                GmSystem::GetInstance().OnScoutPlayer(szPlayerName, IsWatch, UserID);
            }
            else
                GmSystem::GetInstance().OnScoutPlayer("", IsWatch, UserID);
        }
        break;
        //! �����������
    case MSG_SUC2SCC_MANAGE_Request_OptPlayer:
        {
            LONG UserID = pMsg->GetSocketID();
            
            tagReportResultInfo ReportResultInfo;
            ReportResultInfo._strWhys = root["whys"].asString();
            ReportResultInfo._strOptLog = root["optLog"].asString();

            GmSystem::GetInstance().OnOptPlayer(ReportResultInfo, UserID);
        }
        break;
        //! ���͹�������
    case MSG_SUC2SCC_MANAGE_Request_SendAffiche:
        {
            LONG UserID = pMsg->GetSocketID();
            
            DWORD SmID = root["smID"].asInt();
            DWORD GappID = root["gappID"].asInt();

            SM *pSM = SmManage::GetInstance().FindSM(SmID);
            if(NULL != pSM)
            {
                Gapp *pGapp = pSM->FindGapp(GappID);
                if(NULL != pGapp)
                {					
                    if (eGT_WS == pGapp->GetType())
                    {
                        char szAffiche[MAX_AFFICHE_SIZE] = {0};
                        pMsg->GetStr(szAffiche, MAX_AFFICHE_SIZE);
                        GmSystem::GetInstance().OnSendAffiche(szAffiche, UserID, SmID, GappID);
                    }
                }
            }
        }
        break;
        //! ���Ͳ���������ҵ�GM������������ö�٣�DWORD��+ ��Ϣ�ַ�����512 STR����
    case MSG_SUC2SCC_MANAGE_Request_Gm_Command:	
        {
            LONG UserID = pMsg->GetSocketID();            

            DWORD GmCommand = root["gmCommand"].asInt();
            std::string strAffiche = root["affiche"].asString();
            GmSystem::GetInstance().OnCommandToPlayer(GmCommand, strAffiche.c_str(), UserID);
        }
        break;
    default:
        break;
    }

}
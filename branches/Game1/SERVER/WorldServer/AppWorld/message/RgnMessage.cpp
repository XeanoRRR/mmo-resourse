#include "stdafx.h"
#include "..\nets\networld\message.h"
#include "..\nets\Servers.h"
#include "../player.h"
#include "../WorldRegion.h"
#include "..\..\worldserver\game.h"
#include "../setup/GlobeSetup.h"
#include "../setup/QuestSystem.h"
#include "../public/DeputyOccu.h"
#include <MMSystem.h>
#include "../setup/ContributeSetup.h"
#include "../GlobalRgnManager.h"
#include "../script/VariableList.h"



// ��ӦRGN��Ϣ
void OnRgnMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_S2W_RGN_ANSWERGSUPDATEPLAYERRGNLIST:
		{
			CGUID scriptID;
			pMsg->GetGUID(scriptID);
			CGUID playerID;
			pMsg->GetGUID(playerID);
			long gsid = pMsg->GetLong();
			CMessage msg(MSG_W2S_RGN_ANSWERPLAYERRGNLIST);
			msg.Add(scriptID);
			msg.Add(playerID);
			BYTE flag = pMsg->GetByte();
			if(flag == RET_TEAMRGN_LOCAL_OK)
			{
				msg.Add((BYTE)RET_TEAMRGN_REMOTE_OK); // Զ�̸��³ɹ�
			}
			else
			{
				msg.Add((BYTE)RET_TEAMRGN_REMOTE_ERROR); // Զ�̸���ʧ��
			}
			msg.SendGSID(gsid);
		}
		break;
	case MSG_S2W_RGN_ANSWERWSQUESTPLAYERRGNLIST:
		{		
			CGUID scriptID;
			pMsg->GetGUID(scriptID);
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			long gsid = pMsg->GetLong();
			long queryRgnNum = pMsg->GetLong();	
			
			CMessage msg(MSG_W2S_RGN_QUESTGSUPDATEPLAYERRGNLIST);
			msg.Add(scriptID);
			msg.Add(PlayerID);
			msg.Add(gsid);

			if(queryRgnNum != 0)
			{
				msg.Add((long)queryRgnNum);
				for(int i=0; i<queryRgnNum; i++)
				{
					long templateID = pMsg->GetLong();
					msg.Add((long)templateID);
					CGUID rgnID;
					pMsg->GetGUID(rgnID);
					msg.Add(rgnID);
					if(GetGame()->GetGlobalRgnManager()->FindRgnByGUID(rgnID))
						msg.Add((BYTE)1); // ��Ӹ�����¼����
					else
						msg.Add((BYTE)0); // ��Ӹ�����¼������
				}
			}
			else
			{
				msg.Add((long)0);
			}
			msg.SendToSocket(pMsg->GetSocketID());
		}
		break;
	case MSG_S2W_RGN_QUESTPLAYERRGNLIST:
		{
			CGUID scriptID;
			pMsg->GetGUID(scriptID);
			CGUID playerID;
			pMsg->GetGUID(playerID);
			BYTE flag = pMsg->GetByte();
			if(flag == 1) // ��ǰGS��ѯ
			{
				long num = pMsg->GetLong();
				CMessage msg(MSG_W2S_RGN_ANSWERPLAYERRGNLIST);
				msg.Add(scriptID);
				msg.Add(playerID);
				msg.Add((BYTE)RET_TEAMRGN_LOCAL_OK);
				msg.Add((long)num);
				CGUID rgnID;
				for(int i=0; i<num; i++)
				{
					long templateID = pMsg->GetLong();
					msg.Add((long)templateID);
					pMsg->GetGUID(rgnID);
					msg.Add(rgnID);
					CWorldRegion* rgn = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(rgnID);
					if(rgn)
						msg.Add((BYTE)1); // ��Ӹ�����¼����
					else
						msg.Add((BYTE)0); // ��Ӹ�����¼������
				}
				msg.SendToSocket(pMsg->GetSocketID());
			}
			else
			{
				CPlayer* player = GetGame()->GetMapPlayer(playerID);
				if(player)
				{
					CMessage msg(MSG_W2S_RGN_QUESTGSQUESTPLAYERRGNLIST);
					msg.Add(scriptID);
					msg.Add(playerID);
					msg.Add((long)pMsg->GetGSID());
					msg.SendToSocket(player->GetGsSocket());
				}
				else
				{
					CMessage msg(MSG_W2S_RGN_ANSWERPLAYERRGNLIST);
					msg.Add((long)-1);
					msg.Add(playerID);
					msg.Add((BYTE)RET_TEAMRGN_LOCAL_ERROR);
					msg.SendToSocket(pMsg->GetSocketID());
				}
			}
		}
		break;
	case MSG_S2W_RGN_ANSWERVIEWGSPLAYERNUM:
		{
			CGUID rgnGuid;
			pMsg->GetGUID(rgnGuid);
			long num = pMsg->GetLong();
			CGUID ownerID;
			pMsg->GetGUID(ownerID);// �鿴��GUID
			CGUID scriptID;
			pMsg->GetGUID(scriptID); // script id
			char szRet[128];
			pMsg->GetStr(szRet, 128);
			long gsid = pMsg->GetLong();

			CMessage msg(MSG_W2S_RGN_VIEWGSPLAYERNUM);
			msg.Add((long)num); // ����
			msg.Add(ownerID);
			msg.Add(scriptID);
			msg.Add(szRet);
			msg.SendGSID(gsid); // ��Ҫ��Ӧ��GSID
		}
		break;
	case MSG_S2W_RGN_VIEWGSPLAYERNUM:
		{
			CGUID RgnID;
			pMsg->GetGUID(RgnID); // ����TemplateID
			CGUID ownerID;
			pMsg->GetGUID(ownerID);// �鿴��GUID
			CGUID scriptID;
			pMsg->GetGUID(scriptID); // script id
			char szRet[128];
			pMsg->GetStr(szRet, 128);
			CWorldRegion* pRgn = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(RgnID);
			if(pRgn)
			{
				CMessage msg(MSG_W2S_RGN_QUESTVIEWGSPLAYERNUM);
				msg.Add(RgnID);
				msg.Add(ownerID);
				msg.Add(scriptID);
				msg.Add(szRet);
				msg.Add((long)pMsg->GetGSID()); // ��Ҫ��Ӧ��GSID
				msg.SendGSID(pRgn->GetGsid());
			}
			else
			{
				CMessage msg(MSG_W2S_RGN_VIEWGSPLAYERNUM);
				msg.Add((long)-1); // WSδ�ҵ���TemplateID�ĳ���
				msg.Add(ownerID);
				msg.Add(scriptID);
				msg.Add(szRet);
				msg.SendToSocket(pMsg->GetSocketID());
			}
		}
		break;

	case MSG_S2W_RGN_CREATE_QUEST: // �յ�GS1�Ļ�ӦGS0����Ϣ��ת����GS0
		{
			// ���±���Player����
			// srcGSID
			long gsid = pMsg->GetLong();

			// owner ID
			CGUID ownerID;
			pMsg->GetGUID(ownerID);
			CPlayer* pPlayer = GetGame()->GetMapPlayer(ownerID);
			if(pPlayer)
			{
				// Rgn Type
				long rgnType = pMsg->GetLong(); 
				long templateRgnID = pMsg->GetLong(); // Rgn TemplateID
				long lResourceID = pMsg->GetLong(); // Rgn ResourceID
				// Rgn GUID
				CGUID RgnID;
				pMsg->GetGUID(RgnID);
				CGUID scriptID;
				pMsg->GetGUID(scriptID);

				char szVarStr[128] = {0};
				pMsg->GetStr(szVarStr, sizeof(szVarStr));
				char szRgnIDStr[128] = {0};
				pMsg->GetStr(szRgnIDStr, sizeof(szRgnIDStr));
				long Flag = pMsg->GetLong(); // Flag
				if(Flag >= 1)
				{
					GetGame()->GetGlobalRgnManager()->AddPersonalRgnGUID(ownerID, RgnID);
				}
				else
				{
					if(Flag == C_RET_THESE_RGNS_FULL_ERROR) // GS�����������Ѿ�����
					{
						// ɾ���ø����ӹ�����
						CGlobalRgnManager::MapRgnItr rgnItr = GetGame()->GetGlobalRgnManager()->GetRgnMap().find(RgnID);
						if(rgnItr != GetGame()->GetGlobalRgnManager()->GetRgnMap().end())
						{
							MP_DELETE(rgnItr->second);
							GetGame()->GetGlobalRgnManager()->GetRgnMap().erase(rgnItr);
						}
					}
				}
			}
			
			if(gsid != pMsg->GetGSID())
			{
				pMsg->SetType(MSG_W2S_RGN_CREATE_ANSWER);
				pMsg->SendGSID(gsid);// src GSID
			}
		}
		break;
	case MSG_S2W_RGN_CREATE:
		{	
			long templateRgnID = pMsg->GetLong();// ģ��Rgn��ID
			pMsg->Add(templateRgnID); 
			long Type = pMsg->GetLong(); // ģ��Rgn��Type
			CGUID ownerID;		// �����ߵ�GUID
			pMsg->GetGUID(ownerID);

			CGUID scriptID;
			pMsg->GetGUID(scriptID);
			char szRetVarName[128];
			pMsg->GetStr(szRetVarName, 128);

			char szRetRgnIDName[128];
			pMsg->GetStr(szRetRgnIDName, 128);
		
			BYTE isDupRgnDataFlag = pMsg->GetByte(); // �ø����Ƿ��л�������
			if( !GetGame()->GetGlobalRgnManager()->CreateGSRgn(templateRgnID, (eRgnType)Type, ownerID, scriptID, pMsg->GetGSID(), szRetVarName, szRetRgnIDName, 0 != isDupRgnDataFlag) )
			{
				// ������
			}
		}
		break;


	case MSG_S2W_RGN_DELETE:
		{
			CGUID ownerID;
			pMsg->GetGUID(ownerID);
			CGUID templateRgnID;
			pMsg->GetGUID(templateRgnID);// ģ��Rgn��ID
			//pMsg->Add(templateRgnID); 
			long Type = pMsg->GetLong(); // ģ��Rgn��Type
			CGUID scriptID;
			pMsg->GetGUID(scriptID);
	
			char szRetVarName[128];
			pMsg->GetStr(szRetVarName, 128);

			char szRetRgnIDName[128];
			pMsg->GetStr(szRetRgnIDName, 128);
			if( !GetGame()->GetGlobalRgnManager()->DeleteGSRgn((eRgnType)Type, ownerID, templateRgnID, scriptID, pMsg->GetGSID(), szRetVarName, szRetRgnIDName) )
			{
				// ������
			}
		}
		break;
	
	case MSG_S2W_RGN_VIEWRGNVAR:
		{
			eRgnType rType = (eRgnType)pMsg->GetLong();
			CGUID guid;
			pMsg->GetGUID(guid);
			CWorldRegion* rgn = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(guid);
		}
		break;
	case MSG_S2W_RGN_VAR_ADD:
		{
			BYTE srcGSID = pMsg->GetByte();
			CGUID scriptID;
			pMsg->GetGUID(scriptID);
			long gsid = 0;
			eRgnType rType = (eRgnType)pMsg->GetLong();
			CGUID guid;
			pMsg->GetGUID(guid);
		
			CWorldRegion* rgn = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(guid);
			if(!rgn)
			{
				char szGuid[128];
				guid.tostring(szGuid);
				char pszRgnInfo[1024];
				_snprintf(pszRgnInfo,1024,"MSG_S2W_RGN_VAR_ADD: Rgn[%s] is NULL!.",szGuid);
				PutStringToFile("Login_WS_Info",pszRgnInfo);
				CMessage msg(MSG_W2S_RGN_VAR_ADD);
				msg.Add((long)0);
				msg.Add(scriptID);
				msg.SendToSocket(pMsg->GetSocketID());
				return;
			}
						
			gsid = rgn->GetGsid();
			
			char varName[128];
			pMsg->GetStr(varName, 128);
			if(varName[0] == '#') // �ַ���
			{
				long size = pMsg->GetLong();
				BYTE* buf = (BYTE*)M_ALLOC(size+1);
				pMsg->GetEx(buf, size);
				buf[size] = '\0';
				rgn->GetVariableList()->AddVar(varName, (char*)buf);
				M_FREE(buf,size+1);
			}
			else if(varName[0] == '$') // ����
			{
				long size = pMsg->GetLong();
				BYTE* buf = (BYTE*)M_ALLOC(size);
				pMsg->GetEx(buf, size);
				rgn->GetVariableList()->AddVar(varName, *(double*)buf);
				M_FREE(buf,size);
			}
			else if(varName[0] == '@')
			{
				CGUID guid;
				pMsg->GetGUID(guid);
				rgn->GetVariableList()->AddGuid(varName, guid);
			}

			// ת����Rgn����GS
			if(gsid != pMsg->GetGSID())
			{
				pMsg->SetType(MSG_W2S_RGN_VAR_ADD_QUEST);		
				pMsg->SendGSID(gsid);
			}
		}
		break;
	case MSG_S2W_RGN_VAR_ADD_ANSWER:
		{
			long flag = pMsg->GetLong();
			CGUID scriptID;
			pMsg->GetGUID(scriptID);
			CGUID guid;
			pMsg->GetGUID(guid);
			BYTE gsid = pMsg->GetByte();

			pMsg->SetType(MSG_W2S_RGN_VAR_ADD);
			pMsg->SendGSID(gsid);
		}
		break;
	case MSG_S2W_RGN_VAR_DEL:
		{
			BYTE srcGSID = pMsg->GetByte();
			CGUID scriptID;
			pMsg->GetGUID(scriptID);
			long gsid = 0;
			eRgnType rType = (eRgnType)pMsg->GetLong();
			CGUID guid;
			pMsg->GetGUID(guid);
			CWorldRegion* rgn = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(guid);
			if(!rgn)
			{
				char szGuid[128];
				guid.tostring(szGuid);
				char pszRgnInfo[1024];
				_snprintf(pszRgnInfo,1024,"MSG_S2W_RGN_VAR_DEL: Rgn[%s] is NULL!.",szGuid);
				PutStringToFile("Login_WS_Info",pszRgnInfo);

				CMessage msg(MSG_W2S_RGN_VAR_DEL);
				msg.Add((long)0);
				msg.Add(scriptID);
				msg.SendToSocket(pMsg->GetSocketID());

				return;
			}
			gsid = rgn->GetGsid();

			char varName[128];
			pMsg->GetStr(varName, 128);
			if(varName[0] == '#') // �ַ���
			{
				rgn->GetVariableList()->RemoveVar(varName);
			}
			else if(varName[0] == '$') // ����
			{
				rgn->GetVariableList()->RemoveVar(varName);
			}
			else if(varName[0] == '@')
			{
				rgn->GetVariableList()->RemoveGuid(varName);
			}

			// ת����Rgn����GS
			if(gsid != pMsg->GetGSID())
			{
				pMsg->SetType(MSG_W2S_RGN_VAR_DEL_QUEST);		
				pMsg->SendGSID(gsid);
			}
		}
		break;
	case MSG_S2W_RGN_VAR_DEL_ANSWER:
		{
			long flag = pMsg->GetLong();
			CGUID scriptID;
			pMsg->GetGUID(scriptID);
			CGUID guid;
			pMsg->GetGUID(guid);
			BYTE gsid = pMsg->GetByte();
			pMsg->SetType(MSG_W2S_RGN_VAR_DEL);
			pMsg->SendGSID(gsid);
		}
		break;
	case MSG_S2W_RGN_VAR_SET:
		{
			BYTE srcGSID = pMsg->GetByte();
			CGUID scriptID;
			pMsg->GetGUID(scriptID);
			long gsid = 0;
			eRgnType rType = (eRgnType)pMsg->GetLong();
			CGUID guid;
			pMsg->GetGUID(guid);
			CWorldRegion* rgn = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(guid);
			if(!rgn)
			{
				char szGuid[128];
				guid.tostring(szGuid);
				char pszRgnInfo[1024];
				_snprintf(pszRgnInfo,1024,"MSG_S2W_RGN_VAR_SET: Rgn[%s] is NULL!.",szGuid);
				PutStringToFile("Login_WS_Info",pszRgnInfo);

				CMessage msg(MSG_W2S_RGN_VAR_SET);
				msg.Add((long)0);
				msg.Add(scriptID);
				msg.SendToSocket(pMsg->GetSocketID());

				return;
			}
				
			gsid = rgn->GetGsid();

			char varName[128];
			pMsg->GetStr(varName, 128);
			if(varName[0] == '#') // �ַ���
			{
				long size = pMsg->GetLong();
				BYTE* buf = (BYTE*)M_ALLOC(size+1);
				pMsg->GetEx(buf, size);
				buf[size] = '\0';
				rgn->GetVariableList()->AddVar(varName, (char*)buf);
				M_FREE(buf,size+1);
			}
			else if(varName[0] == '$') // ����
			{
				long size = pMsg->GetLong();
				BYTE* buf = (BYTE*)M_ALLOC(size);
				pMsg->GetEx(buf, size);
				rgn->GetVariableList()->AddVar(varName, *(double*)buf);
				M_FREE(buf,size);
			}
			else if(varName[0] == '@')
			{
				CGUID guid;
				pMsg->GetGUID(guid);
				rgn->GetVariableList()->AddGuid(varName, guid);
			}

			// ת����Rgn����GS
			if(gsid != pMsg->GetGSID())
			{
				pMsg->SetType(MSG_W2S_RGN_VAR_SET_QUEST);		
				pMsg->SendGSID(gsid);
			}			
		}
		break;
	case MSG_S2W_RGN_VAR_SET_ANSWER:
		{
			long flag = pMsg->GetLong();
			CGUID scriptID;
			pMsg->GetGUID(scriptID);
			CGUID guid;
			pMsg->GetGUID(guid);
			BYTE gsid = pMsg->GetByte();
			pMsg->SetType(MSG_W2S_RGN_VAR_SET);
			pMsg->SendGSID(gsid);
		}
		break;
	case MSG_S2W_RGN_VAR_GET:
		{
			BYTE srcGSID = pMsg->GetByte();
			CGUID scriptID;
			pMsg->GetGUID(scriptID);
			long gsid = 0;
			eRgnType rType = (eRgnType)pMsg->GetLong();
			CGUID guid;
			pMsg->GetGUID(guid);
			CWorldRegion* rgn = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(guid);
			if(!rgn)
			{
				char szGuid[128];
				guid.tostring(szGuid);
				char pszRgnInfo[1024];
				_snprintf(pszRgnInfo,1024,"MSG_S2W_RGN_VAR_SET: Rgn[%s] is NULL!.",szGuid);
				PutStringToFile("Login_WS_Info",pszRgnInfo);

				CMessage msg(MSG_W2S_RGN_VAR_GET);
				msg.Add((long)0);
				msg.Add(scriptID);
				msg.SendToSocket(pMsg->GetSocketID());

				return;
			}

			gsid = rgn->GetGsid();

			// ת����Rgn����GS
			if(gsid != pMsg->GetGSID())
			{			
				pMsg->SetType(MSG_W2S_RGN_VAR_GET_QUEST);		
				pMsg->SendGSID(gsid);
			}
		}
		break;
	case MSG_S2W_RGN_VAR_GET_ANSWER:
		{
			long flag = pMsg->GetLong();
			CGUID scriptID;
			pMsg->GetGUID(scriptID);
			BYTE srcGSID = pMsg->GetByte();
			long gsid = 0;
			CGUID guid;
			pMsg->GetGUID(guid);

			CWorldRegion* rgn = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(guid);
			if(rgn)
			{
				char varName[128];
				pMsg->GetStr(varName, 128);
				if(varName[0] == '#') // �ַ���
				{
					char str[1024];
					pMsg->GetStr(str, sizeof(str));
					rgn->GetVariableList()->AddVar(varName, str);
				}
				else if(varName[0] == '$') // ����
				{
					long size = pMsg->GetLong();
					BYTE* buf = (BYTE*)M_ALLOC(size);
					pMsg->GetEx(buf, size);
					rgn->GetVariableList()->AddVar(varName, *(double*)buf);
					M_FREE(buf,size);
				}
				else if(varName[0] == '@')
				{
					CGUID guid;
					pMsg->GetGUID(guid);
					rgn->GetVariableList()->AddGuid(varName, guid);
				}
			}

			pMsg->SetType(MSG_W2S_RGN_VAR_GET);		
			pMsg->SendGSID(srcGSID);
		}
		break;
	case MSG_S2W_RGN_VIEWRGNNUM:
		{
			long rgnType = pMsg->GetLong();
			CGUID viewerID;
			pMsg->GetGUID(viewerID);

			// ���ظ�������������
			char szRetNumVar[64];
			pMsg->GetStr(szRetNumVar, 64);
			// ����Ľű�ID
			CGUID scriptID;
			pMsg->GetGUID(scriptID);

			CMessage msg(MSG_W2S_RGN_VIEWRGNNUM);
			msg.Add((long)GetGame()->GetGameServerList()->size()); //GS num

			long totalDupRgnNum = 0;
			CGlobalRgnManager::MapRgnItr itr = GetGame()->GetGlobalRgnManager()->GetRgnMap().begin();
			for(; itr != GetGame()->GetGlobalRgnManager()->GetRgnMap().end(); itr++)
			{
				if(itr->second->GetGsid() == pMsg->GetGSID())
				{
					totalDupRgnNum++;
				}
			}
			msg.Add(viewerID);
			msg.Add(scriptID);
			if(szRetNumVar)
				msg.Add(szRetNumVar);
			else
				msg.Add("");
			msg.Add(totalDupRgnNum);
			msg.SendGSID(pMsg->GetGSID());
		}
		break;
	case MSG_S2W_RGN_VIEWRGNDATA:
		{
			long gsid = -1;
			CGUID guid;
			pMsg->GetGUID(guid);
			long rgnType = pMsg->GetLong();
			long pos = pMsg->GetLong();
			CGUID viewerID;
			pMsg->GetGUID(viewerID);

			long dataType = pMsg->GetLong();
			char retVarName[128];
			pMsg->GetStr(retVarName, 128);
			CGUID scriptID;
			pMsg->GetGUID(scriptID);

			CMessage msg(MSG_W2S_RGN_VIEWRGNDATA);
			msg.Add(gsid);
			msg.Add(rgnType);
			msg.Add(pos);

			CWorldRegion* worldRgn = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(guid);

			if(worldRgn)
			{
				msg.Add((BYTE)1); // �鿴��־
				msg.Add(worldRgn->GetID()); // template id
				msg.Add(worldRgn->GetExID()); // guid
				msg.Add(worldRgn->GetName()); // name
				msg.Add(worldRgn->GetOwnerGUID());   // owner id
			}
			else
			{
				msg.Add((BYTE)0); // �鿴��־
			}
			
			msg.Add(viewerID);
			msg.Add(dataType);
			if(retVarName)
				msg.Add(retVarName);
			else
				msg.Add("");
			msg.Add(scriptID);
			msg.SendGSID(pMsg->GetGSID());
		
		}
		break;
	case MSG_S2W_RGN_VIEWRGNAVLID:
		{
			CMessage msg(MSG_W2S_RGN_VIEWRGNAVLID);
			CWorldRegion* retRgn = NULL;
			CGUID rgnID;
			CGUID senderID;
			pMsg->GetGUID(rgnID);
			pMsg->GetGUID(senderID);

			msg.Add(rgnID);
			msg.Add(senderID);

			char retFlagStr[128];
			pMsg->GetStr(retFlagStr, 128);
			CGUID scriptID;
			pMsg->GetGUID(scriptID);

			long gsid = -1;
			
			retRgn = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(rgnID);
			
			if(retRgn)
			{
				msg.Add((BYTE)1); //�ҵ�����
				msg.Add(retFlagStr); // ret falg string
				msg.Add(scriptID); // script id
				msg.Add((BYTE)gsid); //gsid
			}
			else
			{
				msg.Add((BYTE)0); //�ҵ�����
				msg.Add(retFlagStr); // ret falg string
				msg.Add(scriptID); // script id
			}
			msg.SendGSID(pMsg->GetGSID());
		}
		break;
	case MSG_S2W_RGN_VIEWPLAYERPID:
		{
			CGUID playerGUID;
			pMsg->GetGUID(playerGUID);
			char szRet[128];
			pMsg->GetStr(szRet, sizeof(szRet));
			CGUID scriptID;
			pMsg->GetGUID(scriptID);
			CGUID pGUID;
			CPlayer* player = GetGame()->GetMapPlayer(playerGUID);
			if(player)
			{
				pGUID = player->GetPersonalRgnGUID();
				CMessage msg(MSG_W2S_RGN_VIEWPLAYERPID);
				msg.Add(playerGUID);
				if(szRet)
					msg.Add(szRet);
				else
					msg.Add("");
				msg.Add(scriptID);
				msg.Add(pGUID);
				msg.SendGSID(pMsg->GetGSID());
			}
		}
		break;
	case MSG_S2W_RGN_VIEWPLAYERPHID:
		{
			CGUID playerGUID;
			pMsg->GetGUID(playerGUID);
			char szRet[128];
			pMsg->GetStr(szRet, sizeof(szRet));
			CGUID scriptID;
			pMsg->GetGUID(scriptID);
			CGUID pGUID;
			CPlayer* player = GetGame()->GetMapPlayer(playerGUID);
			if(player)
			{
				pGUID = player->GetPersonalHouseRgnGUID();
				CMessage msg(MSG_W2S_RGN_VIEWPLAYERPHID);
				msg.Add(playerGUID);
				if(szRet)
					msg.Add(szRet);
				else
					msg.Add("");
				msg.Add(scriptID);
				msg.Add(pGUID);
				msg.SendGSID(pMsg->GetGSID());
			}
		}
		break;
	case MSG_S2W_RGN_VIEWPLAYERTID:
		{
			CGUID playerGUID;
			pMsg->GetGUID(playerGUID);
			LONG lTemplateID = pMsg->GetLong();
			char szRet[128];
			pMsg->GetStr(szRet, sizeof(szRet));
			CGUID scriptID;
			pMsg->GetGUID(scriptID);
			CGUID pGUID;
			CPlayer* player = GetGame()->GetMapPlayer(playerGUID);
			if(player)
			{
				pGUID = GetGame()->GetGlobalRgnManager()->GetTeamRgnGUID(playerGUID, lTemplateID);
				CMessage msg(MSG_W2S_RGN_VIEWPLAYERTID);
				msg.Add(playerGUID);
				msg.Add(lTemplateID);
				if(szRet)
					msg.Add(szRet);
				else
					msg.Add("");
				msg.Add(scriptID);
				msg.Add(pGUID);
				msg.SendGSID(pMsg->GetGSID());
			}
		}
		break;
	case MSG_S2W_RGN_GETTEMPLATEID:
		{
			CGUID playerGUID;
			pMsg->GetGUID(playerGUID);
			long rgnType = pMsg->GetLong();
			CGUID rgnGUID;
			pMsg->GetGUID(rgnGUID);
			char szRet[128];
			pMsg->GetStr(szRet, sizeof(szRet));
			CGUID scriptID;
			pMsg->GetGUID(scriptID);

			long rgnTID = 0;
			CWorldRegion* rgn = NULL;
			rgn = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(rgnGUID);
			if(rgn)
				rgnTID = rgn->GetID();

			// ������Ϣ��GS
			CMessage msg(MSG_W2S_RGN_GETTEMPLATEID);
			msg.Add(playerGUID);
			msg.Add((long)rgnType);
			msg.Add(rgnGUID);
			msg.Add(szRet);
			msg.Add(scriptID);
			msg.Add(rgnTID);
			msg.SendGSID(pMsg->GetGSID());
		}
		break;
	case MSG_S2W_RGN_GETDUPTYPE:
		{
			CGUID playerGUID;
			pMsg->GetGUID(playerGUID);
			CGUID rgnGUID;
			pMsg->GetGUID(rgnGUID);
			char szRet[128];
			pMsg->GetStr(szRet, sizeof(szRet));
			CGUID scriptID;
			pMsg->GetGUID(scriptID);

			long rgnType = 0;

			CWorldRegion* wRgn = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(rgnGUID);

			// ������Ϣ��GS
			CMessage msg(MSG_W2S_RGN_GETDUPTYPE);
			msg.Add(playerGUID);
			msg.Add(rgnGUID);
			msg.Add(szRet);
			msg.Add(scriptID);
			if(wRgn)
				msg.Add((long)wRgn->GetRgnType());
			else
				msg.Add((long)0);
			msg.SendGSID(pMsg->GetGSID());
		}
		break;
	case MSG_S2W_RGN_GETDUPRGNONWER:
		{
			CGUID playerGUID;
			pMsg->GetGUID(playerGUID);
			CGUID rgnGUID;
			pMsg->GetGUID(rgnGUID);
			char szRet[128];
			pMsg->GetStr(szRet, sizeof(szRet));
			CGUID scriptID;
			pMsg->GetGUID(scriptID);

			CWorldRegion* wRgn = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(rgnGUID);

			// ������Ϣ��WS
			CMessage msg(MSG_W2S_RGN_GETDUPRGNONWER);
			msg.Add(playerGUID);
			msg.Add(rgnGUID);
			msg.Add(szRet);
			msg.Add(scriptID);
			if(wRgn)
				msg.Add(wRgn->GetOwnerGUID());
			else
				msg.Add(NULL_GUID);
			msg.SendGSID(pMsg->GetGSID());
		}
		break;
	case MSG_S2W_RGN_UPDATE_PLAYER_TEAM_RGNGUID:
		{
			DBReadSet setReadDB;
			pMsg->GetDBReadSet(setReadDB);
			CGUID ownerGuid;
			setReadDB.GetBufferFromByteArray(ownerGuid);
			GetGame()->GetGlobalRgnManager()->RgnGuidDecode(ownerGuid, setReadDB);

			pMsg->SetType(MSG_W2S_RGN_UPDATE_PLAYER_TEAM_RGNGUID);
			pMsg->SendAll();
		}
		break;
	case MSG_S2W_RGN_CAMP_PLAYER_NUM_QUEST: // GS��ѯ����
		{
			long srcGSID = pMsg->GetLong(); // ���Ͷ�GSID
			CGUID rgnGUID;
			pMsg->GetGUID(rgnGUID);
			// ��ȡ��������GSID
			long gsid = GetGame()->GetGlobalRgnManager()->FindGSIDByRgnID(rgnGUID);
			pMsg->SetType(MSG_W2S_RGN_CAMP_PLAYER_NUM_QUEST);
			pMsg->SendGSID(gsid);
		}
		break;
	case MSG_S2W_RGN_CAMP_PLAYER_NUM_ANSWER: // GS��ѯ��Ӧ
		{
			long srcGSID = pMsg->GetLong(); // ���Ͷ�GSID
			CGUID rgnGUID;
			pMsg->GetGUID(rgnGUID);
			pMsg->SetType(MSG_W2S_RGN_CAMP_PLAYER_NUM_ANSWER);
			pMsg->SendGSID(srcGSID);
		}
		break;
	}
}
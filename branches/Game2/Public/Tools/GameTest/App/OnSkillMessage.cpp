#include "stdafx.h"
#include "GameClient.h"
#include "RgnObjMgr.h"
#include "SkillMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



static const char szSkillUseRet[]	= "SkillUse";

void CGameClient::OnSkillMessage(CMessage* pMsg)
{
	long lType = pMsg->GetType();
	switch( lType )
	{
	case MSG_S2C_SKILL_USE:
		{
			//����ʹ�ú���������ص�S2C��Ϣ
			CHAR cRet = pMsg->GetChar();	//�Ƿ�ɹ�,�����0��ʾ�ɹ�
			switch( cRet )
			{
			case SKILL_USE_RESULT_FAILED:
				{
					long lType = pMsg->GetLong();
					CGUID guid;
					pMsg->GetGUID(guid);

					long lSkillID = pMsg->GetLong();		//ʹ�õļ���ID
					long lLevel = pMsg->GetLong();

					long lSkillKey = pMsg->GetLong();
					long lFaildRet = pMsg->GetLong();

					CMoveShape* pMoveShape = (CMoveShape*)m_pClientObjMgr->FindObject(guid);
					if( pMoveShape == m_pMainPlayer )
					{
						switch( lFaildRet)
						{
						case SKILL_USE_FAILED_INVALID_STATE:
							PutStringToFile(szSkillUseRet,"��Ч״̬");
							break;
						case SKILL_USE_FAILED_INVALID_HP:
							PutStringToFile(szSkillUseRet,"Hp ����");
							break;
						case SKILL_USE_FAILED_INVALID_MP:
							PutStringToFile(szSkillUseRet,"mp����");
							break;
						case SKILL_USE_FAILED_INVALID_RP:
							PutStringToFile(szSkillUseRet,"Rp����");
							break;
						case SKILL_USE_FAILED_INVALID_YP:
							PutStringToFile(szSkillUseRet,"yp����");
							break;
						case SKILL_USE_FAILED_INVALID_TARGET:
							PutStringToFile(szSkillUseRet,"��ЧĿ��");
							break;
						case SKILL_USE_FAILED_INVALID_DISTANT:
							PutStringToFile(szSkillUseRet,"��Ч����");
							break;
						case SKILL_USE_FAILED_INVALID_SKILL:
							PutStringToFile(szSkillUseRet,"��Ч����");
							break;
						case SKILL_USE_FAILED_INVALID_TIME:
							PutStringToFile(szSkillUseRet,"��ȴδ��");
							break;
						case SKILL_USE_FAILED_INVALID_EQUIP:
							PutStringToFile(szSkillUseRet,"װ����Ч");
							break;
						case SKILL_USE_FAILED_BLOCKED:
							PutStringToFile(szSkillUseRet,"���赲");
							break;
						case SKILL_USE_FAILED_BREAK:
							PutStringToFile(szSkillUseRet,"��Ϣһ��");
							break;
						case SKILL_USE_FAILED_NOOBJECT:
							PutStringToFile(szSkillUseRet,"û�ж���");
							break;
						case SKILL_USE_FAILED_INVALID_INTONATE:
							PutStringToFile(szSkillUseRet,"��Ч����");
							break;
						case SKILL_USE_FAILED_INVALID_OCCUPATION:
							PutStringToFile(szSkillUseRet,"��Чְҵ");
							break;
						default:
							PutStringToFile(szSkillUseRet,"�ɼ�����");
							break;
						}
					}
				}
			}
		}
		break;
	case MSG_S2C_SKILL_STATE_CHANGE:
		{
			long lSocketID = pMsg->GetSocketID();
			LONG lType = pMsg->GetLong();
			CGUID guid;
			pMsg->GetGUID(guid);
			DWORD	dwHp = pMsg->GetDWord();
			WORD	wMp	 = pMsg->GetWord();
			WORD	eg = pMsg->GetWord();
			//
			if(lType == TYPE_PLAYER)
			{
				if( m_pMainPlayer->GetExID() == guid )
				{
// 					m_pMainPlayer->SetHp(dwHp);
// 					m_pMainPlayer->SetMp(wMp);
// 					m_pMainPlayer->SetEnergy(eg);
					if( dwHp <= 50)
					{
						CMessage* pSendMsg = (CMessage*)CBaseMessage::AllocMsg();
						pSendMsg->Reset();
						pSendMsg->Init(MSG_C2S_OTHER_TALK);

						pSendMsg->Add((BYTE)0);
						pSendMsg->Add((long)RUN_SCRIPT);
						pSendMsg->Add((long)TYPE_PLAYER);
						pSendMsg->Add(guid);
						pSendMsg->Add((long)0);
						pSendMsg->Add("SetMe(\"E_Hp\",60000");
						pSendMsg->SendBySocket(lSocketID);
						CBaseMessage::FreeMsg(pSendMsg);
					}
					if( wMp <= 50)
					{
						CMessage* pSendMsg = (CMessage*)CBaseMessage::AllocMsg();
						pSendMsg->Reset();
						pSendMsg->Init(MSG_C2S_OTHER_TALK);

						pSendMsg->Add((BYTE)0);
						pSendMsg->Add((long)RUN_SCRIPT);
						pSendMsg->Add((long)TYPE_PLAYER);
						pSendMsg->Add(guid);
						pSendMsg->Add((long)0);
						pSendMsg->Add("SetMe(\"E_Mp\",60000");
						pSendMsg->SendBySocket(lSocketID);
						CBaseMessage::FreeMsg(pSendMsg);
						pSendMsg->Reset();
					}
				}
			}
		}
		break;
	case MSG_S2C_OCCUSKILL_STUDY_BEGIN:
		{
			return;
			m_pMainPlayer->GetSkillMgr()->ClearOccuSkill();
//			(m_pMainPlayer->GetAllOccuSkillID()).clear();	//���PLAYER�б������ѧϰ�ĸ�ְҵ�������ܵļ���ID����
			CGUID npcguid;
			pMsg->GetGUID(npcguid);	//npc��GUID

			long num = pMsg->GetDWord();	//numΪ���е��������ܵ���Ŀ
//			m_pMainPlayer->SetOccuSkillNum(num);	
			for(DWORD i=0; i<num; ++i)
			{
				DWORD dwSkillID = pMsg->GetDWord();	//����ID
				DWORD dwSkillCost = pMsg->GetDWord();	//ѧϰ��������Ҫ���ѵĽ��

				m_pMainPlayer->GetSkillMgr()->AddOccuSkill(dwSkillID);
//				m_pMainPlayer->SetAllOccuSkillID(dwSkillID);
			}
		}
		break;
	case MSG_S2C_OCCUSKILL_STUDY:
		{
			long lBeSucceed = pMsg->GetLong();	//�Ƿ�ɹ�,0Ϊ�ɹ�
			DWORD dwFactureID = pMsg->GetDWord();	//�������ܵ�id
			if (lBeSucceed==1)
			{
//				m_pMainPlayer->SetOccuSkillHaveStudied();
			}
		}
		break;
	case MSG_S2C_SKILL_ADD_PET:
		{
			//CGUID guid;
			//pMsg->GetGUID(guid);

			//tagPetSkill skill;
			//pMsg->GetEx(&skill,sizeof(tagPetSkill));
			//pMsg->GetLong();
			//pMsg->GetLong();
			//pMsg->GetLong();
			//pMsg->GetLong();

			//CPet* pPet = dynamic_cast<CPet*>(m_pClientObjMgr->FindObject(TYPE_PET,guid));
			//if( pPet )
			//{
			//	if( skill.lPos == 0 )
			//		skill.bAuto = true;
			//	pPet->AddSkill(skill);
			//}
		}
		break;
	default:
		break;
	}
}
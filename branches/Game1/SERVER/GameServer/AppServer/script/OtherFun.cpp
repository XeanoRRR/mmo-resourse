//=============================================================================
/**
 *  file: OtherFun.cpp
 *
 *  Brief:�����˹���ϵͳ��ս��ϵͳ�����̡��ʼ��������Ľű�����
 *			
 *
 *  Authtor:wangqiao
 *	
 *	Create Datae:2008-11-06
 */
//=============================================================================

#include "StdAfx.h"
#include "mmsystem.h"
#include "script.h"
#include "variablelist.h"
#include "../player.h"
#include "../serverregion.h"
#include "../PKSys.h"
#include "../public/StrType.h"
#include "../public/StrUnifyInput.h"
#include "../../../setup/GlobeSetup.h"
#include "../../../setup/LogSystem.h"
#include "../../../setup/MailList.h"
#include "../Country/Country.h"
#include "../Country/CountryHandler.h"
#include "../business/CBusinessManager.h"
#include "../OrganizingSystem/GameOrganizingCtrl.h"
#include "../public/kl_tp_utility.h"
#include "appserver/FBusiness/ShopManager.h"
#include "../../../setup/counttimerlist.h"


//�����Ľű���غ���
//-----------------------------------------------------------------------------------
//1��������غ���
//..............................................................................
//-----------------------------------------------------------------------------------
//1��ս��ϵͳ��غ���
//.........................................................................
//-----------------------------------------------------------------------------------
//2������ϵͳ��غ���
double CScript::OpenFBusiness( const char *cmd, char *retStr )
{
	if( p_SrcShape == NULL || p_DesShape == NULL )
	{
		return 0;
	}
	
	CPlayer *pPlayer = dynamic_cast<CPlayer*>( p_SrcShape );
	if( pPlayer == NULL )
	{
		return 0;
	}
	
	// ��������
	if( pPlayer->GetCurrentProgress() != CPlayer::PROGRESS_NONE )
	{
		return 0;
	}

	pPlayer->SetCurrentProgress( CPlayer::PROGRESS_FBUSINESS );
	pPlayer->SetMoveable( FALSE );

	int opType = (int) GetIntParam( cmd, 0 );
	long shopID = (long) GetIntParam( cmd, 1 );
	if( opType == ERROR_CODE || opType == INVILID_VALUE || shopID == ERROR_CODE || shopID == INVILID_VALUE )
	{
		return 0;
	}

	if( opType == 0 )
	{
		GetInst( FBusiness::CShopManager ).OpenShopForBuy( shopID, pPlayer->GetExID(), p_DesShape->GetExID() );
	}
	else if( opType == 1 )
	{
		GetInst( FBusiness::CShopManager ).OpenShopForSell( shopID, pPlayer->GetExID(), p_DesShape->GetExID() );
	}

	return 0;
}

double CScript::OpenBusiness(const char* CmdStr, char* retStr)			//������Ʒ		
{
	if( p_SrcShape == NULL || p_DesShape == NULL )
	{
		return 0;
	}

	CPlayer *pPlayer = static_cast<CPlayer*>( p_SrcShape );
	
	int opType = GetIntParam( CmdStr, 0 );
	int SpotId = GetIntParam( CmdStr, 1 );
	if( opType == ERROR_CODE || opType == INVILID_VALUE || SpotId == ERROR_CODE || SpotId == INVILID_VALUE )
	{
		return 0;
	}
	
	Business::CTradeSpot *pSpot = GetInst( Business::CBusinessManager ).GetTradeSpot( SpotId );
	if( pSpot == NULL )
	{
		return 0;
	}

	CMessage msg( MSG_S2C_BUSINESS_OPEN );
	msg.Add( (BYTE)opType );
	msg.Add( p_DesShape->GetExID() );
	msg.Add( (DWORD) SpotId );
	DBWriteSet db;
	msg.GetDBWriteSet( db );
	if( opType == Business::BUSINESS_BUYPAGE )
	{		
		pSpot->GetGoodsList( pPlayer, db );
	}
	else if( opType == Business::BUSINESS_SELLPAGE )
	{
		pSpot->GetGoodsSellPrice( pPlayer, db );
	}
	else
	{
		return 0;
	}

	msg.SendToPlayer( pPlayer->GetExID() );
	return 1;
}

double CScript::IsBusinessState( const char *cmd, char *retStr )
{
	char *player_id_str = GetStringParam( cmd, 0 );
	if( player_id_str == NULL || strlen( player_id_str ) == 0 )
	{
		M_FREE( player_id_str, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	// get the variable value
	const CGUID &player_id = GetScriptGuidValue( static_cast<CMoveShape*>( p_SrcShape ), player_id_str );
	// find the player
	CPlayer *pPlayer = GetGame()->FindPlayer( player_id );
	double ret = 0;
	if( pPlayer != NULL )
	{
		ret = pPlayer->IsBusinessState() ? 1 : 0;
	}
	
	M_FREE( player_id_str, sizeof(char)*MAX_VAR_LEN );
	return ret;
}

double CScript::UseHiggleSkill( const char *cmd, char *retStr )
{
	char* player_id_str = GetStringParam( cmd, 0 );
	char* npc_id_str = GetStringParam( cmd, 1 );
	int iBuyPriceQuo = GetIntParam( cmd, 2 );
	int iSellPriceQuo = GetIntParam( cmd, 3 );
	int iSuccessOdds = GetIntParam( cmd, 4 );

	if( !player_id_str || strlen( player_id_str ) == 0 ||
		!npc_id_str || strlen( npc_id_str ) == 0 ||
		iBuyPriceQuo == ERROR_CODE ||
		iSellPriceQuo == ERROR_CODE ||
		iSuccessOdds == ERROR_CODE )
	{
		M_FREE( player_id_str, sizeof(char)*MAX_VAR_LEN );
		M_FREE( npc_id_str, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	const CGUID &player_id = GetScriptGuidValue( static_cast<CMoveShape*>( p_SrcShape ), player_id_str );
	const CGUID &npc_id = GetScriptGuidValue( static_cast<CMoveShape*>( p_SrcShape ), npc_id_str ); 
	int iSpotID = GetInst( Business::CBusinessManager ).GetSpotByNPC( npc_id );
	Business::CTradeSpot *pSpot = GetInst( Business::CBusinessManager ).GetTradeSpot( iSpotID );
	if( pSpot == NULL )
	{
		BUSINESS_LOG_WARNING( Business::FMT_STR( "Invalid trade spot [%d]", iSpotID ) );
		M_FREE( player_id_str, sizeof(char)*MAX_VAR_LEN );
		M_FREE( npc_id_str, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	pSpot->QueryStateGroup().UseHiggleSkill( player_id, iBuyPriceQuo, iSellPriceQuo, iSuccessOdds );
	M_FREE( player_id_str, sizeof(char)*MAX_VAR_LEN );
	M_FREE( npc_id_str, sizeof(char)*MAX_VAR_LEN );
	return 1;
}

double CScript::UseTradeSkill( const char *cmd, char *retStr )
{
	return 0;
}
double CScript::UseCheckingSkill( const char *cmd, char *retStr )
{
	using namespace kl_common;
	auto_array_ptr<char> player_id_str = GetStringParam( cmd, 0 );
	auto_array_ptr<char> npc_id_str = GetStringParam( cmd, 1 );
	int iSkillLevel = GetIntParam( cmd, 2 );
	if( !player_id_str.valid() || strlen( player_id_str ) == 0 ||
		!npc_id_str.valid() || strlen( npc_id_str ) == 0 ||
		iSkillLevel == ERROR_CODE )
	{
		return 0;
	}

	const CGUID &player_id = GetScriptGuidValue( static_cast<CMoveShape*>( p_SrcShape ), player_id_str );
	CPlayer *pPlayer = GetGame()->FindPlayer( player_id );
	if( pPlayer == NULL )
	{
		return 0;
	}
	const CGUID &npc_id = GetScriptGuidValue( static_cast<CMoveShape*>( p_SrcShape ), npc_id_str ); 
	int iSpotID = GetInst( Business::CBusinessManager ).GetSpotByNPC( npc_id );
	Business::CTradeSpot *pSpot = GetInst( Business::CBusinessManager ).GetTradeSpot( iSpotID );
	if( pSpot == NULL )
	{
		BUSINESS_LOG_WARNING( Business::FMT_STR( "Invalid trade spot [%d]", iSpotID ) );
		return 0;
	}

	CMessage msg( MSG_S2C_BUSINESS_OPEN );
	msg.Add( (BYTE) Business::BUSINESS_CHECKPAGE );
	msg.Add( player_id );
	msg.Add( (DWORD)iSpotID );
	DBWriteSet db;
	msg.GetDBWriteSet( db );
	pSpot->GetGoodsChecking( pPlayer, db );
	msg.SendToPlayer( pPlayer->GetExID() );

	return 0;
}

double CScript::OpenInvest(const char* CmdStr, char* retStr)
{
	CPlayer* pPlayer=(CPlayer*)p_SrcShape;
	if(!pPlayer)
		return 0;
	int nSpotId=GetIntParam(CmdStr,0);
	if(nSpotId==0)
		return 0;	
	CMessage msg(MSG_S2C_INVEST_OPEN);
	msg.Add((DWORD)nSpotId);
	msg.SendToPlayer(pPlayer->GetExID());
	return 1;
}

double CScript::UseChafferSkill(const char* cmd,char* retStr)
{
	CPlayer* pPlayer=(CPlayer*)p_SrcShape;
	if(!pPlayer)
		return 0;

	int nRan=::random(100);
	if(nRan>70)
	{		
		//״̬���
		long lRet=pPlayer->GetStateParamByID(5102,"Num");
		if(lRet==0)
		{
			pPlayer->AddNewState(pPlayer,5102,1,22);
		}	
		pPlayer->SendNotifyMessage("��������ʹ�óɹ�");
		return 1;
	}
	pPlayer->SendNotifyMessage("��������ʹ��ʧ��");	
	return 0;

}

double CScript::RemoveBusinessState(const char* CmdStr,char* retStr)
{
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	long lRet=0;
	if(pPlayer)
	{
		lRet=pPlayer->GetStateParamByID(5101,"Num");
		if(lRet>0)
		{
			//�ҵ����̱�������Ƿ�Ϊ��
			DWORD dwPos=0;
			CSubpackContainer::tagSubpack* pSubpack=pPlayer->getSubpackContainer()->GetBusinessPackTag(dwPos);
			if(pSubpack)
			{
				if(pSubpack->pSubpackContainer->GetGoodsAmount()!=0)
				{
					//�����ǿ�
					pPlayer->SendNotifyMessage("���̱�����Ϊ��,���ܽ������״̬");
					return 0;
				}
				else
				{
					int extendId=-1;
					CVolumeLimitGoodsContainer* pContainer=NULL;
					if(!pPlayer->getOriginPack()->IsFull())
					{
						extendId=CS2CContainerObjectMove::PEI_PACKET;
						pContainer=pPlayer->getOriginPack();
					}
					else if(!pPlayer->getSubpackContainer()->GetSubpack(0)->pSubpackContainer->IsFull())
					{
						extendId=CS2CContainerObjectMove::PEI_PACK1;
						pContainer=pPlayer->getSubpackContainer()->GetSubpack(0)->pSubpackContainer;
					}
					else if(!pPlayer->getSubpackContainer()->GetSubpack(1)->pSubpackContainer->IsFull())
					{
						extendId=CS2CContainerObjectMove::PEI_PACK2;
						pContainer=pPlayer->getSubpackContainer()->GetSubpack(1)->pSubpackContainer;
					}
					else if(!pPlayer->getSubpackContainer()->GetSubpack(2)->pSubpackContainer->IsFull())
					{
						extendId=CS2CContainerObjectMove::PEI_PACK3;
						pContainer=pPlayer->getSubpackContainer()->GetSubpack(2)->pSubpackContainer;
					}
					else if(!pPlayer->getSubpackContainer()->GetSubpack(3)->pSubpackContainer->IsFull())
					{
						extendId=CS2CContainerObjectMove::PEI_PACK4;
						pContainer=pPlayer->getSubpackContainer()->GetSubpack(3)->pSubpackContainer;
					}
					else if(!pPlayer->getSubpackContainer()->GetSubpack(4)->pSubpackContainer->IsFull())
					{
						extendId=CS2CContainerObjectMove::PEI_PACK5;
						pContainer=pPlayer->getSubpackContainer()->GetSubpack(4)->pSubpackContainer;
					}
					if(extendId==-1 || extendId==dwPos)
					{
						pPlayer->SendNotifyMessage("������Ϊ��,���ܽ������״̬");
						return 0;
					}
					CBaseObject* pObj=pPlayer->getSubpackContainer()->Remove(dwPos,1);
					CGoods* pGoods=dynamic_cast<CGoods*>(pObj);		
					CContainer::tagPreviousContainer tPreviousContainer;
					CS2CContainerObjectMove cResult;
					pContainer->Add(pGoods,&tPreviousContainer,&cResult);
					cResult.SetOperation(CS2CContainerObjectMove::OT_MOVE_OBJECT);
					cResult.SetSourceContainer(TYPE_PLAYER,pPlayer->GetExID(),dwPos);
					cResult.SetSourceObjectAmount(1);
					cResult.SetSourceContainerExtendID(CS2CContainerObjectMove::PEI_PACK);
					DWORD dwNewPos=0;
					pContainer->QueryGoodsPosition(pGoods,dwNewPos);
					cResult.SetDestinationContainer(TYPE_PLAYER,pPlayer->GetExID(),dwNewPos);
					cResult.SetDestinationContainerExtendID(extendId);

					cResult.Send(pPlayer->GetExID());
					pPlayer->DelNewState(5101,1);
					pPlayer->SendNotifyMessage("�ɹ��������״̬��");
				}
			}
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------------
//1���ʼ�ϵͳ����
double CScript::ReceiveMail(const char* CmdStr, char* retStr)
{
	CPlayer *pPlayer = NULL;
	char* strGuid = GetStringParam(CmdStr,0);
	if(strGuid != NULL)
	{		
			CGUID shapeID = GetVariableList()->GetGuidValue(strGuid);
			CMoveShape *pMoveShape = GetGame() -> FindShape(TYPE_PLAYER, shapeID );
			CPlayer *pPlayer = dynamic_cast<CPlayer*>(pMoveShape);
			if (pPlayer)
			{
				//���ͽ����ʼ�
				GameManager::GetInstance()->GetMailManager()->SendReceiveMail(pPlayer);		
			}
	}
	M_FREE( strGuid, sizeof(char)*MAX_VAR_LEN );
	return 1.0;
}

double CScript::SendSysMail(const char* CmdStr, char* retStr)
{
	DWORD dwSysMailID = GetIntParam(CmdStr,0);	
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);

	if (pPlayer &&  dwSysMailID!=ERROR_CODE && dwSysMailID!=INVILID_VALUE)
	{
		tagMailParam *ptgMailParam = CMailList::GetSysMail(dwSysMailID);
		char *strReceiver = (char*)(p_SrcShape->GetName());
		char *strWriter =	GetStringParam(CmdStr,1);
		bool bReceiver = false;


		if (pPlayer->CheckSysMail(dwSysMailID))
		{
			M_FREE( strWriter, sizeof(char)*MAX_VAR_LEN );
			return 0.0;
		}
		
		pPlayer->InsertSysMail(dwSysMailID,1);

		if (ptgMailParam)
		{
			if (ptgMailParam->strReceiverList.size()==0)
			{
				bReceiver = true;
				ptgMailParam->strReceiverList.push_back(strReceiver);		
			}
			
			ptgMailParam->WriterGuid = NULL_GUID;

			if (NULL==strWriter)
			{
				ptgMailParam->strWriter = CStringReading::LoadString(IDS_WGS_SYSMAIL,STR_WS_SYSMAIL_THIRD);
			}
			else
			{
				ptgMailParam->strWriter = strWriter;
								
			}
			
			tagMailParam *pMailParam = OBJ_CREATE(tagMailParam);
			GameManager::GetInstance()->GetMailManager()->InitSysMailParam(pMailParam,ptgMailParam);	

			if (bReceiver)
				ptgMailParam->strReceiverList.clear();

			pMailParam->lType = dwSysMailID;

			CMail *pMail = MP_NEW CMail(pMailParam);
			GameManager::GetInstance()->GetMailManager()->SendSysMailByScript(pMail);
			
		}
		if (NULL!=strWriter)
		{
			M_FREE( strWriter, sizeof(char)*MAX_VAR_LEN );
		}		
		return 1.0;
	} 
	else
	{
		return 0.0;
	}	
	return 0.0;
}

double CScript::ReceiveSysMail(const char* CmdStr, char* retStr)
{
	CMessage msg(MSG_S2W_MAIL_RECEIVE);
	msg.Add(p_SrcShape->GetExID());

	msg.Send();
	return 1.0;
}

double CScript::OpenBouns(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) 
		return 0;

	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);

	if(pPlayer == NULL)
		return 0.0;


	if(pPlayer->GetCurrentProgress() != CPlayer::PROGRESS_NONE)
		return 0.0;

	DWORD dwCurTime = timeGetTime();
	if(dwCurTime - pPlayer->GetBounsTime() < CGlobeSetup::GetSetup()->lOperBounsTime)
	{
		return 0.0;
	}
	pPlayer->SetBounsTime(dwCurTime);

	CMessage msg(MSG_S2BDS_BOUNS_LOAD);
	msg.Add(pPlayer->GetAccount());
	msg.Add(pPlayer->GetExID());
	msg.Add(GetGame()->GetWSNumber());
	msg.SendToBounsServer();
	return 1.0;
} 

// ��ʱ��������ؽӿ�
// ��ʱ�������¼��ʱ����Ϣ�����ؿͻ��˽����Ϣ
double CScript::SetTimer(const char* cmd, char* retStr)
{
	CPlayer* player = dynamic_cast<CPlayer*>(p_SrcShape);
	if(!player) return 0;

	if(player->GetCountTimerNum()>=CCountTimerList::GetMaxTimerNum())
		return 0;

	int countType = GetIntParam(cmd, 0); // ��ʱ����
	if(countType==ERROR_CODE) return 0;

	int timeNum = GetIntParam(cmd, 1); // ��ʱʱ��
	if(timeNum==ERROR_CODE) return 0;

	int timerType = GetIntParam(cmd, 2); // ��ʱ������
	if(timerType==ERROR_CODE) return 0;

	const tagCountTimerParam& param = CCountTimerList::GetCountTimerParam(timerType);
	if(param.m_dwType==-1) return 0;

	CPlayer::tagCountTimer timer;
	player->SetCurCountTimerID(player->GetCurCountTimerID()+1);
	timer.m_dwID = player->GetCurCountTimerID();// ��ʱ��ID
	timer.m_bCountType = (0 != countType);//��ʱ���ͣ�0��˳�� 1������
	time_t curTime;
	time(&curTime);
	timer.m_dwCountTime = curTime;// ��ʱ��ʼ�㣬 ��λ����
	timer.m_dwRetTime = timeNum; // ��ʱʣ��ֵ,��λ����
	timer.m_dwStartRetTime = timer.m_dwRetTime;
	timer.m_dwTimerType = timerType;
	player->AddCountTimer(timer);

	// ������Ϣ���ͻ���
	CMessage msg(MSG_S2C_OTHER_SETTIMER);
	vector<BYTE> pBA;
	_AddToByteArray(&pBA, (char)timer.m_bCountType);
	_AddToByteArray(&pBA, (DWORD)timer.m_dwID);
	_AddToByteArray(&pBA, (DWORD)timer.m_dwRetTime);
	_AddToByteArray(&pBA, (DWORD)timer.m_dwStartRetTime);
	_AddToByteArray(&pBA, param.m_strPicPath.c_str());
	_AddToByteArray(&pBA, param.m_strBackPicPath.c_str());
	_AddToByteArray(&pBA, param.m_strTextPath.c_str());
	_AddToByteArray(&pBA, param.m_strText.c_str());
	_AddToByteArray(&pBA, param.m_strTimeoutText.c_str());
	_AddToByteArray(&pBA, (DWORD)param.m_dwTextTime);
	_AddToByteArray(&pBA, (DWORD)param.m_dwTextViewTime);
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToPlayer(player->GetExID());
	return timer.m_dwID;
}
// ��ʱ���رգ����践�ؿͻ�����Ϣ
double CScript::CloseTimer(const char* cmd, char* retStr)
{
	CPlayer* player = dynamic_cast<CPlayer*>(p_SrcShape);
	if(!player) return 0;
	int id = GetIntParam(cmd, 0); // ��ʱ��ID
	if(id==ERROR_CODE) return 0;
	player->DelCountTimer(id);

	CMessage msg(MSG_S2C_OTHER_CLOSETIMER);
	msg.Add((DWORD)id);
	msg.SendToPlayer(player->GetExID());
	return 1;
}
// ȡ����Ҷ�ʱ��״̬
double CScript::GetPlayerTimer(const char* cmd, char* retStr)
{
	CPlayer* player = dynamic_cast<CPlayer*>(p_SrcShape);
	if(!player) return 0;
	return player->GetCountTimerNum();
}
// ȡ�ö�ʱ��ʣ��ʱ��
double CScript::GetTimer(const char* cmd, char* retStr)
{
	CPlayer* player = dynamic_cast<CPlayer*>(p_SrcShape);
	if(!player) return 0;
	int id = GetIntParam(cmd, 0); // ��ʱ��ID
	if(id==ERROR_CODE) return -1;
	CPlayer::tagCountTimer& timer = player->GetCountTimer(id);
	if(timer.m_dwID==-1) return -1;

	time_t curTime;
	time(&curTime);
	DWORD retTime = timer.m_dwRetTime - curTime + timer.m_dwCountTime;
	if(((long)retTime) < 0) retTime = 0;
	return retTime; // ��λ����
}
double CScript::GMAddSkill(const char* CmdStr, char* retStr)
{
	CPlayer *pPlayer=NULL;
	char* strName = GetStringParam(CmdStr,0);
	DWORD lSkillID = GetIntParam(CmdStr,1);
	long lSkillLv = GetIntParam(CmdStr,2);
	if (lSkillLv==ERROR_CODE || lSkillLv==INVILID_VALUE)
	{
		lSkillLv = 1;
	}
	if(strName!=NULL && lSkillID!=ERROR_CODE && lSkillID!=INVILID_VALUE)
	{
		pPlayer = GetGame() -> FindPlayer(strName);
		if (pPlayer)
		{
			pPlayer->AddSkill(lSkillID,lSkillLv);
			pPlayer->OnBeginSkill(lSkillID,lSkillLv);
		}			
	}

	M_FREE( strName, sizeof(char)*MAX_VAR_LEN );
	return 1.0;
}
double CScript::GMDelSkill(const char* CmdStr, char* retStr)
{
	CPlayer *pPlayer=NULL;
	char* strName = GetStringParam(CmdStr,0);
	DWORD lSkillID = GetIntParam(CmdStr,1);
	long lSkillLv = GetIntParam(CmdStr,2);
	if (lSkillLv==ERROR_CODE || lSkillLv==INVILID_VALUE)
	{
		lSkillLv = 1;
	}
	if(strName!=NULL && lSkillID!=ERROR_CODE && lSkillID!=INVILID_VALUE)
	{
		pPlayer = GetGame() -> FindPlayer(strName);
		if (pPlayer)
		{
			pPlayer->DelSkill(lSkillID,lSkillLv);			
		}			
	}
	M_FREE( strName, sizeof(char)*MAX_VAR_LEN );
	return 1.0;
}
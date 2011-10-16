#include "stdafx.h"
#include "..\nets\networld\message.h"
#include "..\player.h"
#include "..\..\WorldServer\Game.h"
#include "../dbentity/entityManager.h"
#include "../dbentity/entitygroup.h"
#include "../PetCtrl.h"
#include "../Linkman/LinkmanSystem.h"



void OnPlayerDetailSaveMessage(CMessage *pMsg)
{
	// player GUID
	CGUID playerGUID;
	char pszSaveInfo[512]="";
	pMsg->GetGUID(playerGUID);

	switch(pMsg->GetType())
	{
	case MSG_S2W_PLAYER_DETAIL_SAVE_PROPERTY:
		{
			// decode MSG_S2W_PLAYER_DETAIL_SAVE_PROPERTY ��Ϣ
			DWORD bitValue = pMsg->GetDWord();

			char pszStackInfo[1024]="";
			char playguid[128]="";
			playerGUID.tostring(playguid);

			DBReadSet setReadDB;
			pMsg->GetDBReadSet(setReadDB);
#ifdef _RUNSTACKINFO1_
			//�������ջ���	
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"OnPlayerDetailSaveMessage() Start");
#endif
			CPlayer* pPlayer = GetGame()->GetMapPlayer(playerGUID);
			if(!pPlayer)
			{
#ifdef _RUNSTACKINFO1_
				//�������ջ���
				char szOutStr[1024];
				sprintf(szOutStr, "OnPlayerDetailSaveMessage() Find Player[%s] is NULL!", playguid);
				CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), szOutStr);
#endif
				return;
			}
			// ��bit������Ϣ
			if( bitValue & SAVE_DETAIL_ATTRPROPERTY)// ��������:��Ǯ,�ȼ�...��
			{
				if(!GetGame()->DecodeSavePropertyMsg(pPlayer, setReadDB))
					return;
			}
			if( bitValue & SAVE_DETAIL_ATTREQUIPMENTGOODS)// װ����Ʒ
			{
				if(!GetGame()->DecodeSaveEquipmentGoodsMsg(pPlayer, setReadDB))
					return;
			}
			if( bitValue & SAVE_DETAIL_ATTRPACKGOODS)// ��������Ʒ
			{
				if(!GetGame()->DecodeSavePackGoodsMsg(pPlayer, setReadDB))
					return;
			}
			if( bitValue & SAVE_DETAIL_ATTRSUBPACKGOODS1)// �ӱ���1��Ʒ
			{
				if(!GetGame()->DecodeSaveSubPackGoodsMsg(0, pPlayer, setReadDB))
					return;
			}
			if( bitValue & SAVE_DETAIL_ATTRSUBPACKGOODS2)// �ӱ���2��Ʒ
			{
				if(!GetGame()->DecodeSaveSubPackGoodsMsg(1, pPlayer, setReadDB))
					return;
			}
			if( bitValue & SAVE_DETAIL_ATTRSUBPACKGOODS3)// �ӱ���3��Ʒ
			{
				if(!GetGame()->DecodeSaveSubPackGoodsMsg(2, pPlayer, setReadDB))
					return;
			}
			if( bitValue & SAVE_DETAIL_ATTRSUBPACKGOODS4)// �ӱ���4��Ʒ
			{
				if(!GetGame()->DecodeSaveSubPackGoodsMsg(3, pPlayer, setReadDB))
					return;
			}
			if( bitValue & SAVE_DETAIL_ATTRSUBPACKGOODS5)// �ӱ���5��Ʒ
			{
				if(!GetGame()->DecodeSaveSubPackGoodsMsg(4, pPlayer, setReadDB))
					return;
			}
			if( bitValue & SAVE_DETAIL_ATTRWALLETGOODS)// Ǯ����Ʒ
			{
				if(!GetGame()->DecodeSaveWalletGoodsMsg(pPlayer, setReadDB))
					return;
			}
			if( bitValue & SAVE_DETAIL_ATTRSILVERGOODS)// ������Ʒ
			{
				if(!GetGame()->DecodeSaveSivlerGoodsMsg(pPlayer, setReadDB))
					return;
			}
			//! �ֿ�
			/////////////////////////////

			if( bitValue & SAVE_DETAIL_ATTRDEPOTGOLD)// �ƽ�ֿ�
			{
				if(!GetGame()->DecodeDepotGoldMsg(pPlayer, setReadDB))
					return;
			}
			if( bitValue & SAVE_DETAIL_ATTRDEPOTSILVER)// ���Ҳֿ�
			{
				if(!GetGame()->DecodeDepotSilverMsg(pPlayer, setReadDB))
					return;
			}
			if( bitValue & SAVE_DETAIL_ATTRPRIMARY)// ���ֿ�
			{
				if(!GetGame()->DecodeDepotGoodsMsg(pPlayer, setReadDB))
					return;
			}		
			if( bitValue & SAVE_DETAIL_ATTRSUBDEPOT1)// �Ӳֿ�1
			{
				if(!GetGame()->DecodeSaveSubDepotGoodsMsg(0, pPlayer, setReadDB))
					return;
			}
			if( bitValue & SAVE_DETAIL_ATTRSUBDEPOT2)// �Ӳֿ�2
			{
				if(!GetGame()->DecodeSaveSubDepotGoodsMsg(1, pPlayer, setReadDB))
					return;
			}
			if( bitValue & SAVE_DETAIL_ATTRSUBDEPOT3)// �Ӳֿ�3
			{
				if(!GetGame()->DecodeSaveSubDepotGoodsMsg(2, pPlayer, setReadDB))
					return;
			}
			if( bitValue & SAVE_DETAIL_ATTRSUBDEPOT4)// �Ӳֿ�4
			{
				if(!GetGame()->DecodeSaveSubDepotGoodsMsg(3, pPlayer, setReadDB))
					return;
			}

			if( bitValue & SAVE_DETAIL_ATTRBUSINESSPACK ) // ��ҵ����
			{
				pPlayer->GetBusinessPackage().DecodeFromGS( pPlayer, setReadDB );
			}

			if( bitValue & SAVE_DETAIL_ATTRVARLIST)// �����б�
			{
				if(!GetGame()->DecodeSaveVarListMsg(pPlayer, setReadDB))
					return;
			}
			if( bitValue & SAVE_DETAIL_ATTRQUESTLIST)// �����б�
			{
				if(!GetGame()->DecodeSaveQuestListMsg(pPlayer, setReadDB))
					return;
			}
			
			if( bitValue & SAVE_DETAIL_ATTRSKILLLIST)// �����б�
			{ 
				if(!GetGame()->DecodeSaveSkillListMsg(pPlayer, setReadDB))
					return;
			}
			if( bitValue & SAVE_DETAIL_ATTRSTATELIST)// ״̬�б�
			{	
				if(!GetGame()->DecodeSaveStateListMsg(pPlayer, setReadDB))
					return;
			}
			if( bitValue & SAVE_DETAIL_ATTRDEPUTYOCCU)// ��ְҵ
			{
				if(!GetGame()->DecodeSaveDeputyOccuListMsg(pPlayer, setReadDB))
					return;
			}
	
			if(bitValue & SAVE_DETAIL_ATTRMERQUESTLIST)// ٸ�������б�
			{	
				if(!GetGame()->DecodeSaveMerQuestListMsg(pPlayer, setReadDB))
					return;
			}

			if(bitValue & SAVE_DETAIL_PETS )			// �����б�
			{	
				GetInst(CPetCtrl).DecodeFromDataBlock(setReadDB, pPlayer);
			}

            if (bitValue & SAVE_DETAIL_INNERHOOK)      //С����ϵͳ
            {
                if (!GetGame()->DecodeSaveSpriteSystemMsg(pPlayer,setReadDB))
                    return;
            }

#ifdef _RUNSTACKINFO1_
			//�������ջ���	
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"SendSavePlayerMsg() Start.");
#endif
			// ����SaveSession
			GetGame()->GetEntityManager()->CreateSavePlayerDataSession(playerGUID, bitValue);

#ifdef _RUNSTACKINFO1_
			//�������ջ���	
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"SendSavePlayerMsg() End.");
#endif

#ifdef _RUNSTACKINFO1_
	//�������ջ���	
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"OnPlayerDetailSaveMessage() End");
#endif
		}
		break;
	default:
		break;
	}
}
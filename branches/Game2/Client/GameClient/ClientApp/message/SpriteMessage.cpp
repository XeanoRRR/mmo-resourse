#include "stdafx.h"
#include "Player.h"
#include "Message.h"
#include "Effect.h"
#include "ClientRegion.h"
#include "Skills/SkillXml.h"
#include "../../../Public/Common/SpriteDef.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ��ӦС���������Ϣ
void OnSpriteMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	  case MSG_S2C_SPRITE_AutoSupply_ChangeSetup_Re:
			if(pMsg->GetLong())    //���óɹ�
			{
				//GetGame()->GetCGuiEx()->GetAutoPageEx()->SetNoErr(pMsg->GetLong());
			}
			else{                  //����ʧ��
				//GetGame()->GetCGuiEx()->GetAutoPageEx()->ReSetDate(pMsg->GetLong());
				//GetGame()->GetCGuiEx()->GetAutoPageEx()->SetGoodsChange();
				long Index=pMsg->GetLong();
				long ErrSc = pMsg->GetLong();   //ʧ��ԭ��
                //����ʧ��ԭ��
				if(ErrSc==eASET_NotErr)
				{
					//���óɹ�
				}
				else if(ErrSc==eASET_TypeWithout)
				{
					//�������ͳ�����Χ
					//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords("�������ͳ�����Χ"); 
				}
				else if(ErrSc==eASET_ValueErr)
				{
					//���õ�ֵ���Ϸ�
					//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords("���õ�ֵ���Ϸ�");
					
				}
				else if(ErrSc==eASET_NotChange)
				{
					//û�������ı�
					//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords("û�������ı�");
				}
				else if(ErrSc==eASET_UnknowErr)
				{
					//δ֪����
					//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords("δ֪����");
		
				}
			}
			break;
	  case MSG_S2C_SPRITE_AutoSupply_UseItem_Notify:

		   long indexNum =pMsg->GetLong(); //��λ���
		   //long ItemIndex = GetGame()->GetCGuiEx()->GetAutoPageEx()->GetItemIndex(indexNum);
		   //long ItemNum =  GetGame()->GetCGuiEx()->GetAutoPageEx()->GetItemAounmt(indexNum); //��ø���λ���

		   //if(ItemNum !=0)
		   //{
			  // CGoods *pGoods=new CGoods();
			  // CGoodsList::FillGoodsInfo(ItemIndex,pGoods);
			  // if(pGoods==NULL)
				 //  return;
			  // CPlayer * pPlayer = GetGame()->GetMainPlayer();
			  // if(pPlayer==NULL)
				 //  return;
			  // pPlayer->SetGoodsCoolDownStartTime(ItemIndex,timeGetTime());
			  // if(ItemNum<5)
			  // {
				 //  char Text[256]; 
				 //  wsprintf(Text,"%s%s: %d",CStringReading::LoadText(eIDS_SPRITE,eIDS_SPRITE_WEARING),pGoods->GetProperty()->strName.c_str(),ItemNum); 
				 //  //GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(CStringReading::LoadText(9,5));
				 //  GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_LEFT_BOTTOM,Text); 

			  // }
			  // SAFE__DELETE(pGoods);
		   //}else{
			  //     GetGame()->GetCGuiEx()->GetMainBarPageEx()->SetOccuButtonSpecialState(true,4);   //���ð�ť������
		   //}


		    break;
	}
	
}

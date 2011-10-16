#include "stdafx.h"
#include "RankSystem.h"
#include "../../ClientApp/Player.h"
#include "../../GameClient/Game.h"
#include "../../../Net/Message.h"
#include "../Other/MsgSendTimeLimitMgr.h"

/////////////////////////////////////////////////
// zhaohang  2010/3/23 
// ���а���Ϣ�����ຯ��ʵ��
void RankMsgProc::Query(eRankType type,long RankID,const char* QueryPlayerName,const char* QueryContent,long PageOrRank /* = 0 */)
{
	if(!QueryContent)
		return;
	MsgSendTimeLimitMgr& mstlm = GetInst(MsgSendTimeLimitMgr);
	if( mstlm.IsTimeOut(MST_TYPE_RANK_TIME) == false)
		return;
	CMessage msg(MSG_C2S_QUEST_PLAYERRANKS);
	CPlayer* pPlayer = GetGame()->GetMainPlayer();
	if(!pPlayer)
		return;
	msg.Add(pPlayer->GetExID());
	msg.Add((char)type);
	msg.Add((long)RankID);
	if(type == RT_OWNER)
	{	
		msg.Add(QueryPlayerName);
		msg.Add(QueryContent);
	}
	else if( type == RT_OTHER)
	{
		msg.Add(PageOrRank);
	}
	else if( type == RT_RANK)
	{
		msg.Add(PageOrRank);
	}
	msg.Send();
	mstlm.UpdateLastSendTime(MST_TYPE_RANK_TIME);
}
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// zhaohang  2010/3/25 
// ���а��ѯ����
bool RankQueryConfig::ReadConfig()
{
	ifstream stream;

	stream.open("setup/UISetup/RankSet.ini");
	if( stream.is_open() )
	{	
		uint mainMenuNum;
		//��ȡ���˵���Ŀ
		stream >> mainMenuNum;
		
		for(uint i = 0 ; i < mainMenuNum ; ++i)
		{
			tagMenuQueryConfig mqc;
			//��ȡ���˵��ı� �� ID
			stream >>  mqc.strMainMenu >> mqc.mainMenuID;
			//��ȡ�Ӳ˵���Ŀ
			uint subItemNum;
			stream >> subItemNum;
			for(uint j= 0 ; j < subItemNum ; ++j)
			{
				tagSubItem si;
				//��ȡ�Ӳ˵�lable��ID �Ͳ�ѯ����
				stream >> si.strItem >> si.itemID >> si.strQueryContent;
				//��ȡ�Ӳ˵�����ʾ�б���Ŀ
				uint listNum;
				stream >> listNum;
				//��ȡ�б�����
				tagRowInfo ri;
				for(uint k = 0 ; k < listNum ; ++k)
				{
					char temp[256];
					stream >> ri.strRowHeader >> temp;
					//��strRowDataHeaderת��ΪСд
					ri.strRowDataHeader = strlwr(temp);
					si.rowInfo.push_back(ri);
				}
				mqc.vecSubItem.push_back(si);
			}
			vecMQC.push_back(mqc);
		}
		return true;
	}
	else
	{
		MessageBox(g_hWnd, "Can't find file setup/RankSet.ini", "message", MB_OK);
		return false;
	}
}
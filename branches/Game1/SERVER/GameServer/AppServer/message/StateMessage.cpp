#include "stdafx.h"
#include "..\Player.h"
//#include "../../../nets/netserver/Message.h"

//#include "../NewState/NewState.h"
//#include "..\nets\netserver\message.h"



// ��ӦSTATE��Ϣ
void OnStateMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_C2S_STATE_END:
		{
		//	//����״̬
		//	DWORD dwStateID =  pMsg->GetDWord();
		//	CPlayer *pPlayer = pMsg->GetPlayer();

		//	map<DWORD, CState*> mapState = pPlayer->GetStatesList();
		//	
		//	for (map<DWORD, CState*>::iterator it=mapState.begin(); it!=mapState.end(); ++it)
		//	{
		//		CNewState *pNewState = dynamic_cast<CNewState*>(it->second);
	
		//		if(pNewState)
		//		{
		//			if (pNewState->GetID() == dwStateID)
		//			{
		//				pNewState->End();

		//				//pPlayer->RemoveState(pNewState);
		//			}
		//		}
		//	}
		}
		break;

	}
}
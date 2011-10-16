#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_MSG_C2S_CONTAINER_OBJECT_MOVE_INCLUDED
#define _INC_MSG_C2S_CONTAINER_OBJECT_MOVE_INCLUDED

class CBaseObject;
class CGoods;
class CS2CContainerObjectMove;

class CC2SContainerObjectMove
{
public:
	LONG								m_lSourceContainerType;
	CGUID								m_SourceContainerID;
	LONG								m_lSourceContainerExtendID;
	DWORD								m_dwSourceContainerPosition;

	LONG								m_lDestinationContainerType;
	CGUID								m_DestinationContainerID;
	LONG								m_lDestinationContainerExtendID;
	DWORD								m_dwDestinationContainerPosition;

	LONG								m_lObjectType;
	
	CGUID								m_guObjectID;
	DWORD								m_dwAmount;

	CPlayer*	pClientPlayer;
	//##��ǰ��������Ϣ������rollbackʱʹ�á�
	//CContainer::tagPreviousContainer	m_tPreviousContainer;

public:	
	CC2SContainerObjectMove				();
	~CC2SContainerObjectMove			();

	BOOL								Receive( CMessage* pMessage );
	BOOL								Move( CS2CContainerObjectMove* pResult);

private:
	CBaseObject*						GetSource( LONG lType = 0, const CGUID& guid = NULL_GUID );
	CBaseObject*						GetDestination();
	
	//##�ú��������Ƿ���ڸ���Ʒ������Ʒ�������Ƿ���ϣ������Ƿ�������Ȼ��Ѹ���Ʒ�Ƴ�������
	CGoods*								GetGoods( CS2CContainerObjectMove* pResult );

	CGoods*								RemoveSrcGoods(CS2CContainerObjectMove* pMessage);

	CGoodsContainer*					GetCGoodsContainer(CPlayer *pPlayer, long lContainerExtendID);

};

#endif

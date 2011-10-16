#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_MSG_S2C_CONTAINER_OBJECT_MOVE_INCLUDED
#define _INC_MSG_S2C_CONTAINER_OBJECT_MOVE_INCLUDED

class CShape;

class CS2CContainerObjectMove
{
public:
	enum OPERATION_TYPE
	{
		OT_ROLL_BACK				= 0,
		OT_MOVE_OBJECT,
		OT_NEW_OBJECT,
		OT_DELETE_OBJECT,
		OT_SWITCH_OBJECT,
		OT_SWITCH_FILL,
		OT_CHANGE_AMOUNT,
		OT_UNKNOWN
	};

	enum PLAYER_EXTEND_ID
	{
		PEI_PACKET					= 1,	//��ͨ����
		PEI_EQUIPMENT,						//װ����		
		PEI_WALLET,							//��Ǯ
		PEI_SILVERWALLET,					//����Ǯ��
		PEI_GEM,							//��ʯ
		PEI_YUANBAO,						//Ԫ��
		PEI_JIFEN,							//����
		PEI_MEDAL_CONTAINER,				// ѫ������		
        PEI_EQUIP_GOD,
        PEI_EQUIP_LAND,
        PEI_EQUIP_HUMAN,
		PEI_PACK =10000,					//�ӱ�������
		PEI_PACK1=10001,					//�ӱ�����λ1
		PEI_PACK2=10002,					//�ӱ�����λ2
		PEI_PACK3=10003,					//�ӱ�����λ3
		PEI_PACK4=10004,					//�ӱ�����λ4
		PEI_PACK5=10005,					//�ӱ�����λ5
		PEI_BUSINESS = 10006,				//��ҵ����
		PEI_BUSSINESPACKFIELD = 20000,		// �����ʱ�������λID
		PEI_BUSSINESPACK = 20001			// ���ʱ�������ID

	};

public:
	OPERATION_TYPE						m_otType;

	LONG								m_lSourceContainerType;
	CGUID								m_SourceContainerID;
	long								m_lSourceContainerExtendID;
	DWORD								m_dwSourceContainerPosition;

	LONG								m_lDestinationContainerType;
	CGUID								m_DestinationContainerID;
	long								m_lDestinationContainerExtendID;
	DWORD								m_dwDestinationContainerPosition;

	LONG								m_lSourceObjectType;
	CGUID								m_guSourceObjectID;
	DWORD								m_dwSourceObjectAmount;

	LONG								m_lDestinationObjectType;
	CGUID								m_guDestinationObjectID;
	LONG								m_dwDestinationObjectAmount;

	vector<BYTE>						m_vStream;

public:
	CS2CContainerObjectMove				();
	~CS2CContainerObjectMove			();

	void								Send( const CGUID& Playerguid );
	void								SendToAround( CShape* pShape );
	void								SendToSession( const CGUID& Sessionguid );

	void								SetOperation( OPERATION_TYPE ot );

	void								SetSourceContainer( LONG lType, const CGUID& guid, DWORD dwPosition );
	void								SetDestinationContainer( LONG lType, const CGUID& guid, DWORD dwPosition );
	void								SetSourceContainerExtendID( long lID );
	void								SetDestinationContainerExtendID( long lID );
	
	void								SetSourceObject( LONG lType,const CGUID& guid );
	void								SetSourceObjectAmount( DWORD dwAmount );

	void								SetDestinationObject( LONG lType,const CGUID& guid);
	void								SetDestinationObjectAmount( DWORD dwAmount );
	
	OPERATION_TYPE						GetOperation();

	LONG								GetSourceContainerType();
	LONG								GetDestinationContainerType();
	LONG								GetSourceContainerExtendID();
	LONG								GetDestinationContainerExtendID();

	vector<BYTE>&						GetObjectStream();

	LONG								GetSourceObjectType();
	CGUID&								GetguSourceObjectID();
	DWORD								GetSourceObjectAmount();

	LONG								GetDestinationObjectType();
	CGUID&								GetguDestinationObjectID();
	DWORD								GetDestinationObjectAmount();

	CPlayer*							GetDestPlayer(void);
};


#endif

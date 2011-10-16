

/*
*	file:		GoodsTailorSystem.h
*	Brief:		�Ǹ�ְҵ����ϵͳ
*	detail:		
*	Authtor:	�ſ���
*	Datae:		2009-1-9
*/



#pragma once

//!-------------------------------------------------------------------------------------------------
//! һ�������䷽�������ֻ��һЩ�����ϵķ�װ�����ı�������ݣ�
class GsOneMakeTable : public OneMakeTable
{
public:
	//!		������䷽Ϊ���������Ʒ
	DWORD	MakeGoods(CPlayer *pPlayer) const;
};



//!-------------------------------------------------------------------------------------------------


//! ����ϵͳ
class GoodsTailorSystem
{
public:
	GoodsTailorSystem(VOID){}
	~GoodsTailorSystem(VOID){}
	static	GoodsTailorSystem&	GetInstance					(VOID){ return c_GoodsTailorSystem; }

public:
	//!							��ҿ���һ����Ʒ����������
	BOOL						PlayerMakeBeginByGoods		(const CGUID &PlayerGuid, const CGUID &GoodsGuid, LONG lMakeTableID, BOOL bDelGoodsWhenFinish);
	//!							��ҿ���һ��NPC����������
	BOOL						PlayerMakeBeginByNpc		(const CGUID &PlayerGuid, const CGUID &RegionGuid, LONG lRadius, LONG lMakeTableID);
	//!							��������������
	BOOL						PlayerMakeFinish			(const CGUID &PlayerGuid);
	//!							�����������ȡ��
	BOOL						PlayerMakeCancel			(const CGUID &PlayerGuid);

private:
	//! ������Ϣ
	struct tagMakeInfo
	{
		//! ������֤��Ϣ
		LONG					lType;				//! ��������
		CGUID					PlayerGuid;			//! ������
		const GsOneMakeTable	*pGsOneMakeTable;	//! �����䷽

		//! ������֤��Ϣ
		CGUID					GoodsGuid;			//! ������Ʒ
		BOOL					bDelGoodsWhenFinish;//! ������ɺ�ɾ��������
		CGUID					RegionGuid;			//! �Ϸ���ͼ
		float					fPosX;				//! �Ϸ�λ��X
		float					fPosY;				//! �Ϸ�λ��Y
		LONG					lRadius;			//! ��Ч�뾶
	};

	//!							��������
	BOOL						MakeGoods					(const tagMakeInfo& MakeInfo);

private:
	


	//!							�������������
	map<CGUID, tagMakeInfo>		m_mapMakingPlayer;

private:
	static GoodsTailorSystem	c_GoodsTailorSystem;
};
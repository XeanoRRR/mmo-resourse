///
/// @file CBusinessManager.h
/// @brief ��ҵϵͳ����ģ�鶥��ӿ�
///
#pragma once

#include "CTradeArea.h"
#include "CTradeSpot.h"
#include "CBusinessGoods.h"
#include "BusinessCommon.h"

class TiXmlElement;

namespace Business
{

	class CBusinessManager
	{
	public:
		typedef std::map<int, CTradeArea*> TradeAreaTable;
		typedef std::map<int, CTradeSpot*> TradeSpotTable;
		typedef std::vector<tagCreditDesc> CreditDescList;
		/// ����NPC-ó�׵�֮���ӳ��
		typedef std::map<CGUID, int, guid_compare> NPCSpotTable;
	public:
		CBusinessManager()
		{
			m_Setup.enable = 0;
		}

		~CBusinessManager();

		void Release();
		
		void DecodeFromDataBlock( DBReadSet &db );

		///
		/// ����ˢ���޹��ʱ�����л�����ʱ���á��޹����ˢ�������л�������һ��ʱ��֮����еġ�
		///
		void StartRefreshBuyLimitTimer( int RegionResID, const CGUID &player_id );

		///
		/// ������ҵϵͳ����Ҫ�ǿ���һЩ��ʱ��
		///
		void Startup();
		void Shutdown();

		/// Query the credit level by the credit value
		int QueryCreditLevel( int credit );

		///
		/// ������ҵ���ݿ�浵����
		///
		void EncodeToSave( DBWriteSet &db );

		///
		/// ���ʹ浵��Ϣ
		///
		void SendSaveMsg();

		///
		/// ���NPC-ó�׵�ӳ��
		///
		void AddNPCSpotRecord( const CGUID &npc_id, int spot_id );

		///
		/// ����NPC ID���Ҷ�Ӧ��ó�׵�ID
		///
		int GetSpotByNPC( const CGUID &npc_id );

		///
		/// Get the trade spot by its id
		///
		CTradeSpot *GetTradeSpot( int id );

		///
		/// query the global system setup
		///
		const tagSystemSetup &GetSetup() const
		{
			return m_Setup;
		}

		///
		/// �ж���ҵϵͳ�Ƿ񱻿���
		///
		bool IsEnable() const
		{
			return m_Setup.enable == 1;
		}
	private:
		TradeAreaTable m_Areas;
		TradeSpotTable m_Spots;
		CreditDescList m_CreditDesc;
		tagSystemSetup m_Setup;
		NPCSpotTable m_NpcSpotTable;
	};
}
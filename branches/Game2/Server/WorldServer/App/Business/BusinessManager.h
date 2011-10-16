///
/// @file CBusinessManager.h
/// @brief ����ϵͳ����ӿ���
///
#pragma once

#include "TradeArea.h"
#include "TradeSpot.h"
#include "BusinessCommon.h"


class TiXmlElement;
class CEntityGroup;
namespace Business
{

	class CBusinessManager
	{
	public:
		typedef std::map<int, CTradeArea*> TradeAreaTable;
		typedef std::map<int, CTradeSpot*> TradeSpotTable;
		typedef std::vector<tagCreditDesc> CreditDescList;
	public:
		CBusinessManager()
		{
			m_bDBLoaded = false;
			memset( &m_Setup, 0, sizeof( m_Setup ) );
		}

		~CBusinessManager();

		/// called by the dtor automatically.
		void Release();

		bool Load();

		///
		/// ������������
		/// @param GSId Ϊָ��GS��������
		///
		void EncodeToDataBlock( DBWriteSet &db, int GSId );

		///
		/// �����entity group���������ݿ�洢
		///
		void EncodeToEntityGroup( CEntityGroup *entity );

		///
		/// �����ݿ�����������
		///
		void DecodeFromEntityGroup( long ret, CEntityGroup *entity );

		///
		/// ��GS����浵����
		///
		void DecodeFromGS( DBReadSet &db );

		///
		/// get the specified trade spot
		///
		CTradeSpot *GetTradeSpot( int id )
		{
			TradeSpotTable::iterator it = m_Spots.find( id );
			return it != m_Spots.end() ? it->second : NULL;
		}

		///
		/// �ж���ҵϵͳ�Ƿ񱻿���
		///
		bool IsEnable() const
		{
			return m_Setup.enable == 1;
		}

		/// 
		/// ��ȡ���ݿ�װ�ر�־
		///
		/// @see m_bDBLoaded
		///
		bool IsDBLoaded() const
		{
			return m_bDBLoaded;
		}
	private:
		bool LoadCreditDesc( TiXmlElement *elm );
		bool LoadTradeArea( TiXmlElement *elm );
		bool LoadTradeSpot( TiXmlElement *elm, int AreaId );

	private:
		TradeAreaTable m_Areas;
		TradeSpotTable m_Spots;
		CreditDescList m_CreditDesc;
		tagSystemSetup m_Setup;
		/// ��־�Ƿ��Ѿ���DB��ȡ�˴浵���ݣ�ֻ�иñ�־Ϊtrueʱ���ŻὫ��ҵ���ݱ����GS��
		/// ͬʱ��ֻ��Ϊtrueʱ���ſ�������ҵ�½��Ϸ(logmesssage.cpp)��
		/// �ڵ�һ�α�Ϊtrueʱ���ñ�־����Զ����true����Ҳ��ζ��WS�������ڼ䲻��ڶ���
		/// ��DBS�������ݣ�������Ա�֤��WS�ϵ���ҵ���ݽ�����DBS���˱�־�����Ծ���DBS
		/// ����ʱ�Ƿ��DBSװ������
		bool m_bDBLoaded;
	};
}
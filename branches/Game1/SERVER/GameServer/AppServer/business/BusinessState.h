///
/// @file BusinessState.h
/// @brief ά���������̲������������б�
///
#pragma once

namespace Business
{
	class CTradeSpot;

	///
	/// ��Ҵ������ֲ�������
	///
	enum 
	{
		OP_NONE,
		OP_BUY,
		OP_SELL,
	};

	///
	/// ��Ҵ��ڴ���/���Ƚ���ʱ��Ҫ��¼����Ϣ
	///
	struct tagState
	{
		int iState;
		DWORD dwHiggleCount;	///< ʹ�á��ּۻ��ۡ����������ɹ�����
		/// ��¼���ּۻ��ۡ�������Ϣ
		struct 
		{
			int iPriceQuo;
		};
	};


	///
	/// CStateGroup
	/// ͬsession���ƣ�������������̲������������б�������ҪΪĳЩ�����ṩ�ĸ������ݡ�
	/// ���磬������������������Ҫʵʱ�ظ��µ�ǰ��Ʒ�۸񣬻���ҪΪ����������������
	/// ����ĳЩ��ҵ���ܵ������ɹ�������
	///
	class CStateGroup
	{
	public:
		typedef stdext::hash_map<CGUID, tagState, hash_guid_compare> PlayerStateTable;
	public:
		CStateGroup() : m_pSpot( NULL )
		{
		}

		~CStateGroup()
		{
		}

		void SetTradeSpot( CTradeSpot *pSpot )
		{
			m_pSpot = pSpot;
		}

		/// 
		/// ��۱仯ʱ�������¼۸�����۲�����ʱ������
		///
		void SendUpdatePriceMsg();
		void SendUpdateBuyPriceMsg( CPlayer *player );
		void SendUpdateSellPriceMsg( CPlayer *player );

		///
		/// ������
		///
		void AddPlayer( const CGUID &player_id, int iOperType );

		///
		/// �Ƴ���ң��ɿͻ��˷�����Ϣ����
		///
		void RemovePlayer( const CGUID &player_id );

		/// 
		/// ��ȡĳ��Ҳ���״̬
		///
		const tagState &GetPlayerState( const CGUID &player_id );
		
		///
		/// ��ȡ�仯�۸�
		///
		/// ����۸�ͨ���������������أ��缼���ڵ�ǰ�۸��ϵ��µı仯
		///
		DWORD AdjustPrice( CPlayer *pPlayer, DWORD dwBasePrice );

		///
		/// ��¼���ּۻ��ۡ�����ʹ�ý��
		///
		void SetHiggleRecord( const CGUID &player_id, int iPriceQuo, bool bSuccess );

		///
		/// ʹ�á��ּۻ��ۡ�����
		///
		bool UseHiggleSkill( const CGUID &player_id, int iBuyQuo, int SellQuo, int iSuccessOdds );

	private:
		tagState &QueryPlayerState( const CGUID &player_id );
	private:
		PlayerStateTable m_PlayerTable;
		CTradeSpot *m_pSpot;
	};
}
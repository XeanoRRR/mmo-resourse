///
/// @file CBusinessPackage.h
/// @brief ���̱�������
///
#pragma once

class CVolumeLimitGoodsContainer;
class CGoods;

namespace Business
{
	class CPackage
	{
	public:
		CPackage();

		~CPackage();

		///
		/// ���³־�
		/// @param dwProfit ĳ�ʽ��׻�õ�����
		/// @return ��ǰ�;�
		///
		DWORD UpdateEndure( DWORD dwProfit );
		
		///
		/// ���³־õ��ͻ���
		///
		void UpdateEndureToClient( const CGUID &player_id );

		/// ����Ƿ���Ч������Ϊ�����Ƿ�װ������ҵ������־��
		bool IsValid() const;

		/// �õ���ǰ���������ɵ���Ʒ����
		DWORD GetLeftGoodsCount() const
		{
			return m_dwMaxGoodsCount - m_dwGoodsCount;
		}

		void AddGoodsCount( DWORD dwAdd )
		{
			m_dwGoodsCount += dwAdd;
		}

		///
		/// ����ָ����Ʒ�ļ۸񣬵�װ���µ���Ʒʱ��Ҫ����
		///
		/// @return ����۸�
		DWORD UpdateGoodsPrice( DWORD index, DWORD amount, DWORD price );
		DWORD UpdateGoodsPrice( CGoods *pGoods );

		///
		/// ������ҵ�������ݴ浵
		///
		void EncodeToSave( DBWriteSet &db );

        void CodeForClient( DBWriteSet& db);

		///
		/// �����ݿ����
		///
		void DecodeFromDB( DBReadSet &db );

		/// ��õ�ǰ��ҵ��������Ʒ����
		DWORD GetCurGoodsAmount();
		
		/// ��õ�ǰ��ҵ����ʹ���˶��ٸ���
		DWORD GetUsedVolume();
		
		/// ����һ����ҵ����
		BOOL MountBusinessPackGoods( CGoods* pGoods );

		///
		/// ����Ƿ�����滻��ҵ����
		///
		/// @param pPackGoods ��ҵ������Ʒ
		///
		BOOL CanSwap( CGoods *pPackGoods );

		///
		/// ��������
		///
		/// @return 1:SWITCH_OBJECT 2:SWITCH_FILE
		int Swap( CGoods *pPackGoods );

		///
		/// ��鱳���Ƿ���Ч�������Ч����ʧ
		///
		bool CheckValid();

		///
		/// ��ȡ������ǰ�;�
		///
		LONG GetCurDurability();

	public:
		CVolumeLimitGoodsContainer *m_pContainer;
		CGoods *m_pGoods;
	private:
		// ��ǰ��Ʒ����
		DWORD m_dwGoodsCount;
		// ������ɵ���Ʒ����
		DWORD m_dwMaxGoodsCount;
	};
}
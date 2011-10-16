///
/// @file GoodsHelper.h
/// @brief To provide some helper functions to query goods properties.
///
#ifndef ___FBUSINESS_GOODSHELPER_H_
#define ___FBUSINESS_GOODSHELPER_H_

#include <string>
#include <vector>

class CGoods;
class CPlayer;

namespace FBusiness
{
	namespace GoodsHelper
	{
		/// @return -1 indicate an error
		long GetPrice( long lIndex );

		struct GoodsInfo
		{
			long lIndex;
			long lAmount;
		};

		typedef std::vector<GoodsInfo> GoodsListType;

		/// �������Ƿ���Լ���������Ʒ������Ʒ�������ƣ�
		bool CanPlayerBuy( CPlayer *pPlayer, const GoodsListType &goods_list );

		/// �����Ʒ�Ƿ�����Ʒ
		bool IsBusinessGoods( long lIndex );

		/// ��ȡ��ұ����ڵ���Ʒ�б�
		size_t GetPlayerBusinessGoods( CPlayer *pPlayer, std::vector<CGoods*> &goods_list );
	}
}

#endif

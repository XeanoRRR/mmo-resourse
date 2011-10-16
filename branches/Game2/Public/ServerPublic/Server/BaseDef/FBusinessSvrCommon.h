///
/// @file FBusinessSvrCommon.h
/// @brief public definitions in FBusiness(faction business) module for WS/GS.
///
#ifndef ___FBUSINESS_SVRCOMMON_H_
#define ___FBUSINESS_SVRCOMMON_H_

#include <vector>

namespace FBusiness
{
	/// �չ�����Ʒ
	struct BuyGoods
	{
		long lIndex;
		float fPriceQuoMin;
		float fPriceQuoMax;
	};
	typedef std::vector<BuyGoods> BuyGoodsListT;

	/// ���۵���Ʒ
	struct SellGoods
	{
		long lIndex;
	};
	typedef std::vector<SellGoods> SellGoodsListT;

	/// ȫ�ֲ�������
	struct GlobalCfg
	{
		ulong uGoodsUpdateInter;
		long lBuyCount;
		long lCoinGoodsIndex;
		long lEnable;
	};

#ifdef _GAME_SERVER
#define LOG_FILE "FBusinessGS"
#elif defined _WORLD_SERVER_
#define LOG_FILE "FBusinessWS"
#endif
}

#endif

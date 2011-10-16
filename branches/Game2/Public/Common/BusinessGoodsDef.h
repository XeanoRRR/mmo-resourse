///
/// @file BusinessGoodsDef.h
/// @brief ����ϵͳ-��Ʒ��������
///
#ifndef ___BUSINESS_GOODS_DEF_H_
#define ___BUSINESS_GOODS_DEF_H_

#include <string>

namespace Business
{
#define MAX_STR_LEN 512
	///
	/// ��Ʒ��ֵ����
	///
	enum eValueType
	{
		VT_NORMAL = 0,
		VT_SPECIAL,
		VT_LUXURY
	};

	///
	/// ��Ʒ����
	///
	struct tagGoods
	{
		ulong dwIndex;					/// ����
		char strOrigName[MAX_STR_LEN];	/// ԭʼ��
		uchar VTType;					/// ��ֵ����
		ulong dwCredit;					/// ����Ҫ��
		ulong dwPersonalInvest;			/// ����Ͷ�ʶ�
		ulong dwCountryInvest;			/// ����Ͷ�ʶ�
		ulong dwSpotDevelop;			/// ó�׵㿪����
		uchar Local;					/// �Ƿ��Ǳ�����Ʒ(1/0)
		ulong dwBaseBuyLimit;			/// �����޹���
		ulong dwVolume;					/// ������
		float fVolumeQuo;				/// �������淢չ�ȱ仯ϵ��
		ulong dwBasePrice;				/// �����۸�
		float fBuyMinQuo;				/// �������ϵ��
		float fBuyMaxQuo;				/// �������ϵ��
		float fSellMinQuo;				/// �������ϵ��
		float fSellMaxQuo;				/// �������ϵ��
		long dwMinVol;					/// ��С������
		ulong dwMaxVol;					/// ��������
		float fBuyMinAddonQuo;			/// ��С�������ٱ仯ϵ��
		float fSellMaxAddonQuo;			/// ���������ٱ仯ϵ��
		ulong dwFallPrice;				/// ˲�䱩�����
		float fCurVolumeQuo;			/// ��ǰ��������ʱ��仯ϵ��
	};

	///
	/// ���͸��ͻ��˵���Ʒ���ԣ��������ʹ��(S2C)
	///
	struct tagGoodsClient
	{
		ulong dwIndex;		/// ����е�ID
		ulong dwBuyLimit;	/// ��ǰ�޹���
		ulong dwPrice;		/// �����۸�
		int PriceScale;		/// ��۱� PriceScale%
	};

	/// �ýṹ��Ҫ����������������������Ʒ�ĵ�λ���ݣ�C2S��
	struct tagGoodsInfoForSell
	{
		CGUID	guid;						// ��Ʒ��GUID
		ulong	dwCurNum;					// ����Ʒ������������
		ulong	dwPrice;					// �����������ļ۸�
	};

	///
	/// ���۽����з��͸��ͻ��˵���Ʒ�۸����ԣ�������ʾ(S2C)
	///
	struct tagGoodsSellClient
	{
		ulong dwIndex;
		ulong dwPrice;
	};

	///
	/// �۸�仯����
	///
	enum 
	{
		PRICE_EQUAL,
		PRICE_GREAT,		
		PRICE_LESS,
	};

	///
	/// ���ָ����ѯ�����������Ʒ��Ϣ
	///
	struct tagGoodsCheckingClient
	{
		ulong dwIndex;
		ulong dwPrice;
		int iPriceScale;		/// ��۱� PriceScale%
		int iPriceState;		
	};

	///
	/// ���������̵�����Ϣ����
	///
	enum 
	{
		BUSINESS_BUYPAGE = 0,
		BUSINESS_SELLPAGE,
		BUSINESS_CHECKPAGE,

		BUSINESS_PAGENUM,
	};
}

#endif // ___BUSINESS_GOODS_DEF_H_
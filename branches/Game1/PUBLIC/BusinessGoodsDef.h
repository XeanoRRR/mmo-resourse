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
		DWORD dwIndex;					/// ����
		char strOrigName[MAX_STR_LEN];	/// ԭʼ��
		BYTE VTType;					/// ��ֵ����
		DWORD dwCredit;					/// ����Ҫ��
		DWORD dwPersonalInvest;			/// ����Ͷ�ʶ�
		DWORD dwCountryInvest;			/// ����Ͷ�ʶ�
		DWORD dwSpotDevelop;			/// ó�׵㿪����
		BYTE Local;						/// �Ƿ��Ǳ�����Ʒ(1/0)
		DWORD dwBaseBuyLimit;			/// �����޹���
		DWORD dwVolume;					/// ������
		float fVolumeQuo;				/// �������淢չ�ȱ仯ϵ��
		DWORD dwBasePrice;				/// �����۸�
		float fBuyMinQuo;				/// �������ϵ��
		float fBuyMaxQuo;				/// �������ϵ��
		float fSellMinQuo;				/// �������ϵ��
		float fSellMaxQuo;				/// �������ϵ��
		long dwMinVol;					/// ��С������
		DWORD dwMaxVol;					/// ��������
		float fBuyMinAddonQuo;			/// ��С�������ٱ仯ϵ��
		float fSellMaxAddonQuo;			/// ���������ٱ仯ϵ��
		DWORD dwFallPrice;				/// ˲�䱩�����
		float fCurVolumeQuo;			/// ��ǰ��������ʱ��仯ϵ��
	};

	///
	/// ���͸��ͻ��˵���Ʒ���ԣ��������ʹ��(S2C)
	///
	struct tagGoodsClient
	{
		DWORD dwIndex;		/// ����е�ID
		DWORD dwBuyLimit;	/// ��ǰ�޹���
		DWORD dwPrice;		/// �����۸�
		int PriceScale;		/// ��۱� PriceScale%
	};

	/// �ýṹ��Ҫ����������������������Ʒ�ĵ�λ���ݣ�C2S��
	struct tagGoodsInfoForSell
	{
		CGUID	guid;						// ��Ʒ��GUID
		DWORD	dwCurNum;					// ����Ʒ������������
		DWORD	dwPrice;					// �����������ļ۸�
	};

	///
	/// ���۽����з��͸��ͻ��˵���Ʒ�۸����ԣ�������ʾ(S2C)
	///
	struct tagGoodsSellClient
	{
		DWORD dwIndex;
		DWORD dwPrice;
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
		DWORD dwIndex;
		DWORD dwPrice;
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
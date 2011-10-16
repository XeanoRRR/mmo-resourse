///
/// @file BusinessCommon.h
/// @brief ����ϵͳ��������
///
#pragma once

namespace Business
{
	/// ó�׵��������
	struct tagTradeSpotProperty
	{
		int areaId;
		int Id;
		int GsId;
		/// ���ڳ�������ԴID
		int RegionResID;
		/// Ӱ��ͬ����Ʒ���ϵ��
		float fCategoryVolumeQuo;
		/// Ӱ��������Ʒ���ϵ��
		float fAllVolumeQuo;
	};

	/// �����ȼ�
	enum eCreditLevel
	{
		CL_BLAND = 1,
		CL_NORMAL,
		CL_KINDLY,
		CL_RESPECT,
		CL_HONOR,
	};

	/// ��������
	struct tagCreditDesc
	{
		int min;
		int max;
		char desc[256];
		int level;
	};

	/// ó�������������
	struct tagTradeAreaProperty
	{
		int Id;
		char name[256]; // even it's hard code
	};

	/// ϵͳȫ������
	struct tagSystemSetup
	{
		DWORD dwUpdatePriceInterval; ///< ��۲�����ʱ�����
		DWORD dwUpdateBuyLimitInterval; ///< �л������೤ʱ�������޹���
		float fEndureQuo; ///< �;ü���ϵ�����;ý���ֵ=װ����Ʒ�۸�*EndureQuo
		BYTE enable; ///< �Ƿ�����ҵϵͳ(1/0)
	};

//#define LOG_FILE_NAME "business_log_ws"
//#define BUSINESS_LOG_INFO( LogInfo ) PutoutLog( LOG_FILE_NAME, LT_DEFAULT, LogInfo )
//#define BUSINESS_LOG_WARNING( LogInfo ) PutoutLog( LOG_FILE_NAME, LT_WARNING, LogInfo )
//#define BUSINESS_LOG_ERROR( LogInfo ) PutoutLog( LOG_FILE_NAME, LT_ERROR, LogInfo )
// To enable/disable arg list support
#define SUPPORT_ARG_LIST

#ifdef SUPPORT_ARG_LIST
	inline const char *FMT_STR( const char *fmt, ... )
	{
		static char buf[2056];
		va_list arg_list;
		va_start( arg_list, fmt );
		vsprintf( buf, fmt, arg_list );
		va_end( arg_list );
		return buf;
	}
#else
#define FMT_STR( info ) info
#endif
}
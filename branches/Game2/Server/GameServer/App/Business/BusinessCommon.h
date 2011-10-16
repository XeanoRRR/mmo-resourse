///
/// @file BusinessCommon.h
/// @brief ����ϵͳ��������
///
#pragma once

#include <stdarg.h>

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
	
	///
	/// ��ʱ������
	///
	enum
	{
		TIMER_UPDATE_PRICE = 1,		///< ��۲���
		TIMER_UPDATE_BUYLIMIT,		///< ˢ���޹���
	};

	inline const char *GUID_STR( const CGUID &id )
	{
		static char s[50];
		id.tostring( s );
		return s;
	}

#define BUSINESS_LOG
#ifndef BUSINESS_LOG
#undef PutoutLog
#define PutoutLog( file, level, info )  
#endif

#define LOG_FILE_NAME "business_log_gs"
//#define BUSINESS_LOG_INFO( LogInfo ) PutoutLog( LOG_FILE_NAME, LT_DEFAULT, LogInfo )
//#define BUSINESS_LOG_WARNING( LogInfo ) PutoutLog( LOG_FILE_NAME, LT_WARNING, LogInfo )
//#define BUSINESS_LOG_ERROR( LogInfo ) PutoutLog( LOG_FILE_NAME, LT_ERROR, LogInfo )
// To enable/disable arg list support
#define SUPPORT_ARG_LIST

//#ifdef SUPPORT_ARG_LIST
//	inline const char *FMT_STR( const char *fmt, ... )
//	{
//		static char buf[2048];
//		va_list arg_list;
//		va_start( arg_list, fmt );
//		sprintf_s( buf, 2048, fmt, arg_list );
//		va_end( arg_list );
//		return buf;
//	}
//#else
//#define FMT_STR( info ) info
//#endif

//#define BUSINESS_DUMP
#ifdef BUSINESS_DUMP
#define dump_spot_state( spot ) extern void business_dump( CTradeSpot *pSpot ); \
	business_dump( (CTradeSpot*) spot )
#else
#define dump_spot_state( spot )
#endif

}
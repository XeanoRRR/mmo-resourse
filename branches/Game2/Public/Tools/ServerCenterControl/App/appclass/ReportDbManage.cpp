

#include "StdAfx.h"
#include "ReportDbManage.h"
#include "DbOperator.h"



ReportDbManage::ReportDbManage(VOID)
:m_ReportInfoCache(2, 5)
{
}


//! ����һ���ٱ���Ϣ
void ReportDbManage::SaveReportInfo(tagReportInfo &ReportInfo)
{
	const DataCache<tagReportInfo>::tagDataBlock *pReportInfo = m_ReportInfoCache.AddData(ReportInfo);
	if (NULL != pReportInfo)
	{
		DbOperator *pDbOpt = DbOperator::CreateSaveReport((VOID*)pReportInfo);
		if (NULL != pDbOpt)
		{
			pDbOpt->Begin();
		}
	}
}

//! ��Ӧ�ٱ��������
VOID ReportDbManage::OnSaveReportFinish(const DataCache<tagReportInfo>::tagDataBlock *pDataBlock)
{
	m_ReportInfoCache.Release(pDataBlock);
}

//! ��⻺�����Ƿ��г�ʱ����Ϣ
VOID ReportDbManage::CheckCache(VOID)
{
	const DataCache<tagReportInfo>::tagDataBlock *pReportInfo = m_ReportInfoCache.CheckCache();
	if (NULL != pReportInfo)
	{
		DbOperator *pDbOpt = DbOperator::CreateSaveReport((VOID*)pReportInfo);
		if (NULL != pDbOpt)
		{
			pDbOpt->Begin();
		}
	}
}

//! ��ȡһ��WS�ϵľٱ��б�SUC
void ReportDbManage::LoadReportList(__int64 ServerKey, LONG UserID)
{
	DbOperator *pDbOpt = DbOperator::CreateLoadReport(ServerKey, UserID);
	if (NULL != pDbOpt)
	{
		pDbOpt->Begin();
	}
}

//! ��ȡһ��WS�ϵľٱ���������SUC
void ReportDbManage::LoadResultList(__int64 ServerKey, LONG UserID, DWORD BeginDat)
{
	DbOperator *pDbOpt = DbOperator::CreateLoadResult(ServerKey, UserID, BeginDat);
	if (NULL != pDbOpt)
	{
		pDbOpt->Begin();
	}
}


//! ��¼һ��GM��player�Ĳ���
VOID ReportDbManage::LogOptPlayer(tagReportResultInfo &ReportResultInfo, const CGUID &OptGuid, __int64 ServerKey)
{
	char szGuid[64] = {0};
	OptGuid.tostring(szGuid);
	DWORD SqlSize = 1024 + (DWORD)ReportResultInfo._strWhys.size() + min(7999, (DWORD)ReportResultInfo._strOptLog.size());
	char *pSql = (char*)M_ALLOC(SqlSize);
	if (NULL != pSql)
	{
		const char *pLog = ReportResultInfo._strOptLog.c_str();
		if(7999 <= ReportResultInfo._strOptLog.size())
		{
			pLog -= ReportResultInfo._strOptLog.size() - 7999;
		}
		sprintf(pSql,"INSERT INTO report_operation(\
					  log_time, opt_id, server_key, indictee_name, gm_name, opt_cause, opt_type, opt_value, whys, opt_log)\
					  VALUES(%u, '%s', %I64d, N'%s', N'%s', %d, %d, %d, N'%s', N'%s')",
					  ReportResultInfo._OptTime,
					  szGuid,
					  ServerKey,
					  ReportResultInfo._IndicteeName,
					  ReportResultInfo._szGmName,
					  ReportResultInfo._dwCause,
					  ReportResultInfo._OptResult,
					  ReportResultInfo._lBanTime,
					  ReportResultInfo._strWhys.c_str(),
					  pLog
					  );

		DbOperator *pDbOpt = DbOperator::CreateRunSql(pSql, SqlSize);
		if (NULL != pDbOpt)
		{
			pDbOpt->Begin();
		}
	}
}

//! ��¼GM��player������ִ�н��
VOID ReportDbManage::SaveOptPlayerResult(const CGUID &OptGuid, DWORD OptSuccp)
{
	DbOperator *pDbOpt = DbOperator::CreateSaveOptResult(OptGuid, OptSuccp);
	if (NULL != pDbOpt)
	{
		pDbOpt->Begin();
	}
}


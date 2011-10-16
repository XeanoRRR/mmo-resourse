

#include "DataCache.h"


#pragma once



class ReportDbManage
{
public:
	ReportDbManage(VOID);

public:
	//! ����һ���ٱ���Ϣ
	VOID SaveReportInfo(tagReportInfo &ReportInfo);
	//! ��Ӧ�ٱ��������
	VOID OnSaveReportFinish(const DataCache<tagReportInfo>::tagDataBlock *pDataBlock);

	//! ��⻺�����Ƿ��г�ʱ����Ϣ
	VOID CheckCache(VOID);

	//! ��ȡһ��WS�ϵľٱ��б�SUC
	//! ��ȡ������ϸ���ɴ洢���̰��գ�����Ķ�
	VOID LoadReportList(__int64 ServerKey, LONG UserID);

	//! ��ȡһ��WS�ϵľٱ���������SUC
	VOID LoadResultList(__int64 ServerKey, LONG UserID, DWORD BeginDat);

	//! ��¼һ��GM��player�Ĳ���
	VOID LogOptPlayer(tagReportResultInfo &ReportResultInfo, const CGUID &OptGuid, __int64 ServerKey);

	//! ��¼GM��player������ִ�н��
	VOID SaveOptPlayerResult(const CGUID &OptGuid, DWORD OptSuccp);

	

	
private:
	//! 
	DataCache<tagReportInfo> m_ReportInfoCache;
};
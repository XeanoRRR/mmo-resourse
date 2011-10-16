
#include "../../../../../public/Common/BaseDef.h"
#include "../../../../../Public/ServerPublic/Server/Common/AsynchronousExecute.h"
#include "../../../../../Public/ServerPublic/db/CnPool.h"
#include "../../../../SCMPublic/class/CGameInfo.h"
#include "DataCache.h"

struct tagDbLog;

#pragma once




class DbOperator
{
public:
	//!										��ʼ������
	static		BOOL						Init(VOID);
	//!										�ͷŶ���
	static		VOID						Release(VOID);

public:
	//!										����һ������Ƚϲ���
	static		DbOperator*					CreatePwdCmpOperator(const char *pCdkey, const char *pMD5Pwd);
	//!										����һ�������޸Ĳ���
	static		DbOperator*					CreatePwdChangeOperator(const char *pCdkey, const char *pMD5Pwd);
	//!										����һ��Ӳ����Ϣ��д�����
	static		DbOperator*					CreateInHardOperator(DWORD dwSMID, DWORD dwNum, tagHardwareInfo *pHardwareInfo);
	//!										����һ��д��DB����־��д�����
	static		DbOperator*					CreateIDBLogOperator(tagDbLog *pDbLog, DWORD dwNum);
	//!										����һ����Ϸ��Ϣ��д�����
	static		DbOperator*					CreateGameInfoOperator(DWORD dwSMID, DWORD dwGappID, tagDbGameInfo *pDbGameInfo, DWORD dwNum);
	//!										����һ����Ϸ��Ϣ���ȡ����
	static		DbOperator*					CreateLoadGameInfo(DWORD dwSMID, DWORD dwGappID, DWORD dwInfoType, tagDbGameInfo *pDbGameInfo, DWORD dwNum, DWORD dwBeginTime, DWORD dwEndTime, LONG lUserID);

	//!										����һ������ٱ���Ϣ�Ĳ���
	static		DbOperator*					CreateSaveReport(VOID *pDataBlock);
	//!										����һ����ȡ�ٱ��б�Ĳ���
	static		DbOperator*					CreateLoadReport(__int64 ServerKey, LONG UserID);
	//!										����һ����ȡ��Ҵ������Ĳ���
	static		DbOperator*					CreateLoadResult(__int64 ServerKey, LONG UserID, DWORD BeginDat);
	//!										����һ��������Ҵ������ɹ��Ĳ���
	static		DbOperator*					CreateSaveOptResult(const CGUID &OptGuid, DWORD OptSuccp);
	//!										����һ��ִ��SQL���Ĳ���
	static		DbOperator*					CreateRunSql(const char *pSql, DWORD SqlSize);

	//!										���ٲ���
	static		VOID						ReleaseOperator(DbOperator **ppOperator);

public:
	//!										ִ�в���
	BOOL									Begin(VOID);

	//! ˽�ж���
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	//!										��ȡ����
	static		BOOL						LoadSetup(VOID);

	BOOL									CreateData(VOID);
	VOID									ReleaseData(VOID);

	
private:
	//!										����Ƚϲ���
	static		LPVOID						PwdCmp(LPVOID pThis);
	//!										�����޸Ĳ���
	static		LPVOID						PwdChange(LPVOID pThis);
	//!										Ӳ����Ϣ��д�����
	static		LPVOID						InHard(LPVOID pThis);
	//!										��־��д�����
	static		LPVOID						DbLog(LPVOID pThis);
	//!										��־��д�����
	static		LPVOID						GameInfo(LPVOID pThis);
	//!										��־��ȡ����
	static		LPVOID						LoadGameInfo(LPVOID pThis);

	//!										����ٱ���Ϣ�Ĳ���
	static		LPVOID						SaveReport(LPVOID pThis);
	//!										��ȡ�ٱ���Ϣ
	static		LPVOID						LoadReport(LPVOID pThis);
	//!										��ȡ��������Ϣ
	static		LPVOID						LoadResult(LPVOID pThis);
	//!										������Ҵ������ɹ��Ĳ���
	static		LPVOID						SaveOptResult(LPVOID pThis);
	//!										ִ��һ���޷���ֵ��SQL���
	static		LPVOID						RunSql(LPVOID pThis);

private:
	//! ���ݿ������������
	enum eDbOptObjType
	{
		//! �Ƚ��û�����
		eDOOT_PwdCmp,
		//! �޸�����
		eDOOT_PwdChange,
		//! Ӳ����Ϣ��
		eDOOT_InHard,
		//! Ҫд��DB����־��
		eDOOT_DBLogGroup,
		//! Ҫд��DB����Ϸ��Ϣ��
		eDOOT_GameInfoGroup,
		//! ��ȡDB��Ϸ��Ϣ
		eDOOT_LoadGameInfo,

		//! ����ٱ���Ϣ
		eDOOT_SaveReport,
		//! ��ȡ�ٱ���Ϣ
		eDOOT_LoadReport,
		//! ���洦����
		eDOOT_SaveOptResult,
		//! ��ȡ��Ҵ�����Ϣ
		eDOOT_LoadResult,
		//! ִ��һ���޷���ֵ��SQL���
		eDOOT_RunSql,
	};

	//! ���ݿ��������
	//!-------------------------------------------------
	struct tagUserInfo
	{
		char szCdkey[MAX_USER_NAME_SZIE];
		char szPwd[BASE_PASSWORD_SIZE];
		tagUserInfo(VOID){memset(this, 0, sizeof(tagUserInfo));}
	};

	struct tagHardInfoGroup
	{
		DWORD			dwSMID;
		DWORD			dwNum;
		tagHardwareInfo	*pHardwareInfo;	
	};

	struct tagDBLogGroup
	{
		tagDbLog		*pDbLog;
		DWORD			dwNum;
	};

	struct tagGameInfoGroup 
	{
		DWORD			dwSMID;
		DWORD			dwGappID;
		tagDbGameInfo	*pDbGameInfo;
		DWORD			dwNum;
	};

	struct tagLoadGameInfo
	{
		DWORD			dwSMID;
		DWORD			dwGappID;
		tagDbGameInfo	*pDbGameInfo;
		DWORD			dwNum;
		DWORD			dwInfoType;
		DWORD			dwBeginTime;
		DWORD			dwEndTime;
		LONG			lUserID;
	};

	struct tagLoadReport
	{
		__int64			ServerKey;
		LONG			UserID;
	};
	struct tagLoadResult
	{
		__int64			ServerKey;
		LONG			UserID;
		DWORD			BeginDate;
	};

	struct tagSaveOptResult 
	{
		CGUID OptGuid;
		DWORD OptSuccp;
	};

	struct tagDbSql 
	{
		const char		*pSql;
		DWORD			SqlSize;
	};

	//!-------------------------------------------------

private:
	//!										��������
	DWORD									m_dwDOOT;
	//!										����ָ��
	VOID									*m_pData;

private:
	static		AsynchronousExecute			c_AsynchronousExecute;
	static		CnPool						c_CnPool;

	//! ��������Ϳ���
private:
	DbOperator(DWORD dwDOOT);
	DbOperator(DbOperator&);
	~DbOperator(void);
};
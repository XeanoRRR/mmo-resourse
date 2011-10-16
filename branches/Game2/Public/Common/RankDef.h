/**
*File:  RankDef.h
*Brief: ���а����ݶ���
*Author:��η
*Date:  2010.03.10
*/

#ifndef _RANKDEF_H
#define _RANKDEF_H

#include "ColumnProperty.h"

///������
enum COL_TYPE
{
    CT_UNKNOWN = 0,
    CT_FLOAT,
    CT_LONG,
    CT_SHORT,
    CT_ULONG,
    CT_USHORT,
    CT_BOOL,
    CT_CHAR,
    CT_BYTE,
    CT_WORD,
    CT_DWORD,
    CT_STRING,
    CT_BUFFER,
    CT_GUID,
    CT_STRUCT,
    CT_TIME,
    CT_BIGINT,
};

///���а������ö��ֵ
enum eRankType
{
    RT_NOMATCH = 0, /// ��ƥ�����������
    RT_UPDATE,      /// �������ݸ���
    RT_OWNER,       /// �Լ�����
    RT_OTHER,       /// �����������
    RT_RANK,        /// ������ѯ

};

/// ������Ϣ
struct tagColInfo
{
    string	            strColName;
    COL_TYPE	        dt;
};

/// �������ýṹ
struct tagRankConfig
{
    long    lRankID;            /// ���а���
    long    lTopRecord;         /// ����ǰN��
    long    lRefreshTime;       /// ��ʱ���¸�����(��)
    string     strCurSQL;         /// ��ǰ���в�ѯ���
    string     strUpdateTable;
    string     strUpdateSQL;      ///
};

/// ��������Ϣ
struct tagRankInfo
{
    long          lRankID;          /// ���а���
    ulong         ulUpdateTime;     /// ����ʱ��
    long          lTotalPage;       /// ������ҳ��
    long          lCurPage;         /// ��ǰ��������ҳ
    CRITICAL_SECTION    csRankInfo;
    vector<tagColInfo> vecColInfo;
    vector<CTableManager*> vecColData;

    tagRankInfo(long lID = 0)
        :lRankID(lID),ulUpdateTime(0),lTotalPage(0),lCurPage(0)
    {
        vecColInfo.clear();
        vecColData.clear();
        InitializeCriticalSection(&csRankInfo);
    }
    ~tagRankInfo()
    {
        EnterCS();
        Clear();
        LeaveCS();
        DeleteCriticalSection(&csRankInfo);
    }

    void Clear()
    {
        vecColInfo.clear();
        vector<CTableManager*>::iterator itor = vecColData.begin();
        for(; itor != vecColData.end(); ++itor)
        {
            SAFE_DELETE((*itor));
        }
        vecColData.clear();
    }

    bool TryEnterCS()  
    { 
        if(TryEnterCriticalSection(&csRankInfo))
        {
            return true;
        }
        return false;
    }

    void EnterCS()  { EnterCriticalSection(&csRankInfo); }
    void LeaveCS()  { LeaveCriticalSection(&csRankInfo); }

    bool AddToByteArray(vector<BYTE>* pByte);
    void DecordFromByteArray(BYTE* pByte, long& lPos);

    /// ����ɸѡ
    bool AddToByteArray(vector<BYTE>* pByte, long lPosBegin, long lPosEnd);
    bool AddToByteArray(vector<BYTE>* pByte, CDataObject* pEntity, long lColPos);
    bool AddToByteArray(vector<BYTE>* pByte, CDataObject* pEntity, const char* szColName);
    bool AddToByteArray(vector<BYTE>* pByte, long lPage);

    /// Ѱ�ұ�����Col Vector�е�λ��
    long FindColPos(const char* szColName);
    /// Ѱ������������Data Vector�е�λ��
    long FindDataPos(CDataObject* pEntity, long lColPos);

    /// ��ҳ����
    long GetPageCount();
    /// �ҵ���������ҳ��
    long FindPage(long lDataPos);
};

#endif //_RANKDEF_H
#include "StdAfx.h"
#include "public.h"
#include "AutoFightData.h"


CAutoFightData::CAutoFightData(void)
:m_dwStableTime(0)
{
    Reset();
}

CAutoFightData::~CAutoFightData()
{

}

#define DATA_SIZE   sizeof(tagAutoFightNode) * AutoHookTypeNum

//����
void CAutoFightData::AddToByteArray(vector<BYTE> &ByteArray)
{
    _AddToByteArray(&ByteArray, m_AutoFightSetup, DATA_SIZE);
}

//����
void CAutoFightData::GetFromByteArray(BYTE* pByteArray, long &lPos)
{
    _GetBufferFromByteArray(pByteArray,lPos,m_AutoFightSetup,DATA_SIZE);
}

//����
void CAutoFightData::CodeToDataBlock(DBWriteSet& setWriteDB)
{
    //���ڲ���Ҫ�����ȶ��һ�����ͨ�һ���״̬������Ҫ����GS����Ҫ������
    //setWriteDB.AddToByteArray(m_AutoFightSetup,DATA_SIZE);
    //��GS���ȶ�ʱ��
    setWriteDB.AddToByteArray((DWORD)GetStableTime());
}

//����
void CAutoFightData::DecodeFromDataBlock(DBReadSet& setReadDB)
{
    //���ڲ���Ҫ�����ȶ��һ�����ͨ�һ���״̬������Ҫ��GS�õ�����Ҫ������
    //setReadDB.GetBufferFromByteArray(m_AutoFightSetup, DATA_SIZE);
    //��GS�����ȶ�ʱ��
    SetStableTime(setReadDB.GetDwordFromByteArray());
}

//���ö���
void CAutoFightData::Reset(void)
{
    memset(m_AutoFightSetup,0,DATA_SIZE);
}

//���һ������
const tagAutoFightNode* CAutoFightData::GetOneSetup(LONG lType)
{
    if (AutoHookTypeNum > lType)
    {
        return &(m_AutoFightSetup[lType]);
    }
    return NULL;
}

//�޸�һ������
LONG CAutoFightData::SetOneSetup(LONG lType, tagAutoFightNode &AutoFightNode)
{
    if (AutoHookTypeNum <= lType)
        return eASET_TypeWithout;

    //! ��ͬ�Ķ���ֵ����������
    if(0 == memcmp(&(m_AutoFightSetup[lType]), &AutoFightNode, sizeof(tagAutoFightNode)))
        return eASET_NotChange;

    memmove(&(m_AutoFightSetup[lType]), &AutoFightNode, sizeof(tagAutoFightNode));
    return eASET_NotErr;
}
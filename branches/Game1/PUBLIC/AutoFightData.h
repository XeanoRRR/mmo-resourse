/*
*	file:		AutoFightData.h
*	Brief:		�Զ���ֶ���
*	detail:		�ṩ����ı����ӿ�
*	Authtor:	Ԭ�鿡
*	Date:		2011-02-14
*/

#include "SpriteDef.h"

#pragma once

class CAutoFightData
{
public:
    CAutoFightData();
    ~CAutoFightData();

public:
    //����
    void     AddToByteArray    (vector<BYTE> &ByteArray);
    //����    
    void     GetFromByteArray  (BYTE* pByteArray, long &lPos);

    //����
    void	 CodeToDataBlock		(DBWriteSet& setWriteDB);
    //����
    void	 DecodeFromDataBlock	(DBReadSet& setReadDB);

    //��ԭ
    void     Reset(void);

    //�õ�һ����������
    const tagAutoFightNode*   GetOneSetup(LONG lType);
    //�����ȶ��һ�ʱ��
    void     SetStableTime(DWORD dwTime) 
    {
        if (dwTime <= MAX_STABLETIME)
        {
            m_dwStableTime = dwTime;
        }
        else
        {
            m_dwStableTime = MAX_STABLETIME;
        }
    }
    //�õ��ȶ��һ�ʱ��
    DWORD     GetStableTime(void) {return m_dwStableTime;}

    //�޸�һ�����͵�����
    LONG    SetOneSetup       (LONG lType, tagAutoFightNode &AutoFightNode);
private:
    //�Զ���ָ�������
    tagAutoFightNode       m_AutoFightSetup[AutoHookTypeNum];
    //�ȶ��һ�ʱ��
    DWORD     m_dwStableTime;
};
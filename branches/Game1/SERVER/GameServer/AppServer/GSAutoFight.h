/*
*	file:		GSAutoSupply.h
*	Brief:		�Զ�ս����
*	detail:		ʵ���Զ�ս���Ĺ���
*					
*					
*	Authtor:    Ԭ�鿡
*	Date:		2011-02-14
*/

#include "../public/AutoFightData.h"

#pragma once


class GSAutoFight : public CAutoFightData
{
public:
    GSAutoFight();
    ~GSAutoFight();

public:
    //����������
    void        SetOwner(CPlayer* pMainPlayer);
    //�õ�������
    CPlayer*    GetOwner(void)   { return  m_pOwner;}
    //�ı丨���һ�����
    LONG        ChangeSetup(LONG lType, tagAutoFightNode &AutoFightNode, BOOL bSysOpt = FALSE);
    //!         �Զ�����װ���������ж�
    bool        CanAutoRepair(void);

public:
    //!								��ʱ����ʱ��Ӧ
    long							OnTimeOut			(DWORD timerid,DWORD curtime,const void* var);
    //!								����ʱ����ȡ���ĵ���
    void							OnTimerCancel		(DWORD timerid,const void* var);

private:
    //!								���Զ��������ڶ�ʱ��
    void							OpenTimer			(void);
    //!                             �򿪾���ʱ�䶨ʱ��
    void                            OpenDieReLiveTimer  (DWORD timeValue);
    //!                             �򿪹��ж�ʱ��
    void                            OpenCommonTimer     (void);
    //!								�ر��Զ���ʱ��
    void							CloseTimer			(void);
    //!                             �رչ��ж�ʱ��         
    void                            CloseCommonTimer    (void);
    //!								�Զ��ж��Ƿ�Ҫ������ʱ������
    void							GoOnTimer			(void);

    //!                             ���ж�ʱ����Ӧִ��
    void                            BeginWork(void);

public:
    //!										�õ���ʱ������
    static	CTimerQueue<GSAutoFight*>&		GetTimerQueue(void);
    static  CTimerQueue<GSAutoFight*>&      GetCommonTimerQueue(void);
private:

    //������
    CPlayer*       m_pOwner;
    //!                                     ��ʱ��������ʶ
    BOOL           m_bTimerRunning;
    //!                                     ���ж�ʱ��������ʶ
    BOOL           m_bCommonRunning;
    //!                                     ����״̬��ʶ
    BOOL           m_bProtected;
    //!                                     ��ʱ����Ӧ����
    DWORD          m_dwTimerAI;
    //!                                     �ȶ��һ�����ԭ�ظ���ʱ�����
    DWORD          m_dwStableHookReliveAI;

    //!                                     �;�Ϊ0����Ӧ����
    DWORD          m_dwDurabilityZeroAI;
    //!                                     ���ж�ʱ��ID
    LONG           m_CommonTimerID;
    //!                                     �ȶ�����ͨ�һ����ж�ʱ��
    static  CTimerQueue<GSAutoFight*>      s_CommonTimerQueue;
    //!										��ʱ��
    static	CTimerQueue<GSAutoFight*>		s_TimerQueue;
    //!                                     �Զ��һ����������ʱ��
    //static  CTimerQueue<GSAutoFight*>
};
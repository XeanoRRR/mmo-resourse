/*
*	file:		GSAutoSupply.h
*	Brief:		�Զ�������
*	detail:		ʵ���Զ������Ĺ���
*					
*					
*	Authtor:	�ſ���
*	Datae:		2008-05-22
*/

#pragma once

#include "../../../Public/ServerPublic/Server/Common/AutoSupplyData.h"

//! ���ڼ�¼ʹ����Ʒ�����Ľṹ
struct tagGoodsParam
{
	WORD	wPlace;
	BYTE	lPos;
	LONG	lTargetType;
	CGUID	TargetGuid;
	LONG	lCoolingTime;	//! ��Ʒ��ȴʱ�䣨���룩
	LONG	lCoolingType;	//! ��Ʒ��ȴ����
	LONG	lLastTime;		//! �ϴκ�ҩʱ�䣨���룩
};


class GSAutoSupply : public AutoSupplyData
{
public:
	GSAutoSupply(void);
	~GSAutoSupply(void);


public:
	//!								����������
	void							SetOwner			(CPlayer *pMainPlayer);
	//!								�ı䲹������
	BOOL							ChangeSetup			(LONG eSupplyType, tagAotoSupplyNode &AotoSupplyNode, BOOL bSysOpt = FALSE);
	//!								�Զ�����ִ��
	void							Supply				(LONG eSupplyType);

public:
	//!								��ʱ����ʱ��Ӧ
	long							OnTimeOut			(DWORD timerid,DWORD curtime,const void* var);
	//!								����ʱ����ȡ���ĵ���
	void							OnTimerCancel		(DWORD timerid,const void* var);
	//!
	
private:
	//!								���Զ�������ʱ��
	void							OpenTimer			(void);
	//!								�ر��Զ�������ʱ��
	void							CloseTimer			(void);
	//!								�Զ��ж��Ƿ�Ҫ������ʱ������
	void							GoOnTimer			(void);
	//!								׼����Ʒ
	BOOL							GoodsReady			(LONG eSupplyType);

public:
	//!										�õ���ʱ������
	static	CTimerQueue<GSAutoSupply*>&		GetTimerQueue(void);

private:
	
	//!										��ʱ��������־
	BOOL									m_bTimerRunning;
	//!										������
	CPlayer									*m_pOwner;
	//!										�����ʹ�õĵ�һ����Ʒ
	tagGoodsParam							m_arrGoodsParam[AotoSupplyTypeNum];

	//!										��ʱ��
	static	CTimerQueue<GSAutoSupply*>		s_TimerQueue;

	
};
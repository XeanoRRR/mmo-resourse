/**
============================================================
*  Copyright (c) 2010
*  All rights reserved
*  �ļ����ƣ�TestReporter.h
*  ���ܣ����Ա�����,��Ҫ�ṩ�������״̬�ַ�����UI
*  ��ǰ�汾��1.0.0
*  ���ߣ�    ������
*  ������ڣ�2010.11.8
*  ȡ���汾��---
*  ԭ���ߣ�	 ������
*  ������ڣ�2010.11.8
*
============================================================
*/

#pragma once

class CTestReporter
{
	/*��Ҫ��¼������:  (�����ֽ׶λ�е����ҪΪ���ڲ��Է������ǿͻ���,���ܷǹ��ܵĺں��Զ������Գ���,���Խ����¼���в���)
	1.�������Ի���:
	A.�˴β������ӷ�����IP
	B.�˴β�����������
	C.�˴β������ͽڵ����
	D:�˴β������ͽڵ�����
	E:�˴β�����Ŀ����
	F:���Կ�ʼʱ��
	G:���Խ���ʱ��
	H:���Կ�ʼ�˺�
	I:���Խ����˺�


	2.�����������֮������ͨ��״̬:
	A.�˴β���ͬʱ��¼��ֵ
	B.�˴β���ͬʱ��¼��ֵ
	C.�˴β��Է������߳���е��������
	D:�˴β���ÿСʱ��ƽ����������


	3.����������(ϵͳ����):
	A.���ܲ���
	a.ð�̲���
	��.�˴β��Ե�һ�νű��Ƿ���ִ������������

	b.ѹ������
	��.�˴β��Ի�е����������ʱ��
	��.�˴β��Ի�е����������������ݽ�����

	c.���ز���
	��.�˴β��Ի�е�������������

	B:��ȫ����
	a.���ݰ�ȫ(�������Ƿ���Ϣ��֤)
	��.�˴β��Ի�е���Ƿ�ʹ�����MsgHead,���MsgData
	��.�˴β��Ի�е���Ƿ�ʹ�ù̶�MsgHead,�̶�MsgData
	��.�˴β��Ի�е���Ƿ�ʹ�ù̶�MsgHead,���MsgData
	��.�˴β��Ի�е���Ƿ�ʹ�����MsgHead,�̶�MsgData
	b.���밲ȫ
	��.�˴β����Ƿ������߼��жϳɹ�
	��.�˴β������߼�����λ��(����Ϣͷ��ͷ)

	4.�ͻ��˲���(ϵͳ����):...

	*/

	//1.�������Ի���:
	static string m_strConctIp;					//�˴β������ӷ�����IP
	static long   m_lConctTotlNum;				//�˴β�����������
	static string m_strTestTypeNodeNum;			//�˴β������ͽڵ����
	static string m_strTestTypeName;			//�˴β������ͽڵ�����
	static string m_strProjName;				//�˴β�����Ŀ����
	static string m_strStartTestTime;			//���Կ�ʼʱ��
	static string m_strLaseTestTime;			//���Գ���ʱ��
	static string m_strStartCDKEY;				//���Կ�ʼ�˺�
	static string m_strEndCDKEY;				//���Խ����˺�


	//2.�����������֮������ͨ��״̬:
	static long m_lMaxConctNum;				    //�˴β���ͬʱ��¼��ֵ
	static long m_lMinConctNum;				    //�˴β���ͬʱ��¼��ֵ
	static long m_lKickedByServerNum;	        //�˴β��Է������߳���е��������
	static long m_lPerHourConctNum;		        //�˴β���ÿСʱ��ƽ����������


	//3.����������(ϵͳ����):
	static bool m_bIsCanRun;					//�˴β��Ե�һ�νű��Ƿ���ִ������������
	static long m_lTotalRuntime;			    //�˴β��Ի�е����������ʱ��
	static long m_lMsgInterchangeNum;		    //�˴β��Ի�е����������������ݽ�����
	static long m_lMaxConctNumSysTest;          //�˴β��Ի�е�������������
	static bool m_bIsRadHeadRadData;		    //�˴β��Ի�е���Ƿ�ʹ�����MsgHead,���MsgData
	static bool m_bIsFixedHeadFixedData;	    //�˴β��Ի�е���Ƿ�ʹ�ù̶�MsgHead,�̶�MsgData
	static bool m_bIsFixedHeadRadData;		    //�˴β��Ի�е���Ƿ�ʹ�ù̶�MsgHead,���MsgData
	static bool m_bIsRadHeadFixeData;		    //�˴β��Ի�е���Ƿ�ʹ�����MsgHead,�̶�MsgData
	static bool m_bIsJumpLogicDrive;		    //�˴β����Ƿ������߼��жϳɹ�

public:
	CTestReporter();
	virtual ~CTestReporter();

	static void		SetConctIp(string _arg)				{ m_strConctIp = _arg; }
	static string	GetConctIp()						{ return m_strConctIp; }

	static void		SetConctTotlNum(long _arg)			{ m_lConctTotlNum = _arg; }
	static long		GetConctTotlNum()					{ return m_lConctTotlNum; }

	static void		SetTestTypeNode(string _arg)		{ m_strTestTypeNodeNum = _arg; }
	static string	GetTestTypeNode()					{ return m_strTestTypeNodeNum; }

	static void		SetTestTypeName(string _arg)		{ m_strTestTypeName = _arg; }
	static string	GetTestTypeName()					{ return m_strTestTypeName; }

	static void		SetProjName(string _arg)			{ m_strProjName = _arg; }
	static string	GetProjName()						{ return m_strProjName; }

	static void		SetStartTestTime(string _arg)		{ m_strStartTestTime = _arg; }
	static string	GetStartTestTime()					{ return m_strStartTestTime; }

	static void		SetEndTestTime(string _arg)			{ m_strLaseTestTime = _arg; }
	static string	GetEndTestTime()					{ return m_strLaseTestTime; }

	static void		SetStartCDKEY(string _arg)			{ m_strStartCDKEY = _arg; }
	static string	GetStartCDKEY()						{ return m_strStartCDKEY; }

	static void		SetEndCDKEY(string _arg)			{ m_strEndCDKEY = _arg; }
	static string	GetEndCDKEY()						{ return m_strEndCDKEY; }

	static void		SetMaxConctNum(long _arg)			{ m_lMaxConctNum = _arg; }
	static long		GetMaxConctNum()					{ return m_lMaxConctNum; }

	static void		SetMinConctNum(long _arg)			{ m_lMinConctNum = _arg; }
	static long		GetMinConctNum()					{ return m_lMinConctNum; }

	static void		SetKickedByServerNum(long _arg)		{ m_lKickedByServerNum = _arg; }
	static long		GetKickedByServerNum()				{ return m_lKickedByServerNum; }

	static void		SetPerHourConctNum(long _arg)		{ m_lPerHourConctNum = _arg; }
	static long		GetPerHourConctNum()				{ return m_lPerHourConctNum; }

	static void		SetIsCanRun(bool _arg)				{ m_bIsCanRun = _arg; }
	static bool		GetIsCanRun()						{ return m_bIsCanRun; }

	static void		SetTotalRuntimem(long _arg)			{ m_lTotalRuntime = _arg; }
	static long		GetTotalRuntime()					{ return m_lTotalRuntime; }

	static void		SetMsgInterchangeNum(long _arg)		{ m_lMsgInterchangeNum = _arg; }
	static long		GetMsgInterchangeNum()				{ return m_lMsgInterchangeNum; }

	static void		SetMaxConctNumSysTest(long _arg)	{ m_lMaxConctNumSysTest = _arg; }
	static long		GetMaxConctNumSysTest()				{ return m_lMaxConctNumSysTest; }

	static void		SetIsRadHeadRadDatat(bool _arg)		{ m_bIsRadHeadRadData = _arg; }
	static bool		GetIsRadHeadRadData()				{ return m_bIsRadHeadRadData; }

	static void		SetIsFixedHeadFixedData(bool _arg)	{ m_bIsFixedHeadFixedData = _arg; }
	static bool		GetIsFixedHeadFixedData()			{ return m_bIsFixedHeadFixedData; }

	static void		SetIsFixedHeadRadData(bool _arg)	{ m_bIsFixedHeadRadData = _arg; }
	static bool		GetIsFixedHeadRadData()				{ return m_bIsFixedHeadRadData; }

	static void		SetIsRadHeadFixeData(bool _arg)		{ m_bIsRadHeadFixeData = _arg; }
	static bool		GetIsRadHeadFixeData()				{ return m_bIsRadHeadFixeData; }

	static void		SetIsJumpLogicDrive(bool _arg)		{ m_bIsJumpLogicDrive = _arg; }
	static bool		GetIsJumpLogicDrive()				{ return m_bIsJumpLogicDrive; }

	static CTestReporter*	GetInstance();
	static string			GetTestResultInfo();
	static bool				Release();

	bool					Init(void*);
	void*					GetUIInstance() { return m_pDlg; }
private:
	void* m_pDlg;
};

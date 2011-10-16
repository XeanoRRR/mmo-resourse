//=============================================================================
/**
 *  file: OpSendMsg.h
 *
 *  Brief:��װ��ע�ᶨʱ�����¼�����
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-9-5
 */
//=============================================================================

#ifndef __OPERATOR_REGISTER_H__
#define __OPERATOR_REGISTER_H__
#pragma once

#include "Operator.h"
#include "BuffSkillDef.h"

class CMoveShape;
class CScript;

class COpRegisterTimer :
	public COperator
{
public:
	COpRegisterTimer(stModuParam *pParam);
	COpRegisterTimer(void* pParam);
	virtual ~COpRegisterTimer(void);
private:
	//�Ӷ�������
	long m_eChildObjType;
	//��ʱ�����
	long m_lFlag;
	//Ԥ������ʱ��
	long m_lFuture;
	string m_strFutVarname;
	//���ʱ��
	long m_lInter;
	string m_strInterVarName;
	//���ô���
	DWORD m_nCount;
	string m_strCountVarName;
    //@by:expter ֱ�Ӷ�ȡ���ñ���
    string m_strFutureParamname;
    string m_strInterParamname;
    string m_strCountParamname;    
public:
	//��ʼ��
	void InitBaseData(eObj Obj,long oType,long flag,long lFuture,string strFname,
                      long lIntel,string strIntelName,DWORD count,
                      string strCountName,string futname,string intname,
                      string countname)
	{
		m_Obj = Obj;
		m_eChildObjType = oType;
		m_lFlag = flag;
		m_lFuture = lFuture;
		m_strFutVarname = strFname;
		m_lInter = lIntel;
		m_strInterVarName = strIntelName;
		m_nCount = count;
		m_strCountVarName = strCountName;
        m_strFutureParamname=futname;
        m_strInterParamname=intname;
        m_strCountParamname=countname; 
	}
	//ִ��
    void Execute(CMoveShape* pShape);

};

class COpRegisterEvent :
	public COperator
{
public:
	COpRegisterEvent(stModuParam *pParam);
	COpRegisterEvent(void* pParam);
	virtual ~COpRegisterEvent(void);
private:
	//�Ӷ�������
	long m_eChildObjType;
	//�¼����
	long m_lFlag;
	//�¼�id
	long m_lEventType;
public:
	//��ʼ��
	void InitDataBase(eObj Obj,long type, long flag,long eventtype)
	{
		m_Obj = Obj;
		m_eChildObjType = type;
		m_lFlag = flag;
		m_lEventType = eventtype;
	}
	//ִ��
    void Execute(CMoveShape* pShape);

};

class COpUnRegisterTimer :
	public COperator
{
public:
	struct tagParam
	{
		stModuParam *pParam;
		long lFlag;
	};
	COpUnRegisterTimer(stModuParam *pParam,long lFlag);
	COpUnRegisterTimer(void *pParam);
	virtual ~COpUnRegisterTimer(void);
private:
	//��ʱ�����
	//flag ==0,��ʾע�����ж�ʱ��
	long m_lFlag;
public:
	//��ʼ��
	void InitDataBase(eObj Obj)
	{
		m_Obj = Obj;
	}
	//ִ��
    void Execute(CMoveShape* pShape);

};

class COpUnRegisterEvent :
	public COperator
{
public:
	struct tagParam
	{
		stModuParam *pParam;
		long lFlag;
	};
	COpUnRegisterEvent(stModuParam *pParam,long lFlag);
	COpUnRegisterEvent(void* pParam);
	virtual ~COpUnRegisterEvent(void);
private:
	//�¼����
	//flag ==0,��ʾע�������¼�
	long m_lFlag;
public:
	//��ʼ��
	void InitDataBase(eObj Obj)
	{
		m_Obj = Obj;
	}
	//ִ��
    void Execute(CMoveShape* pShape);

};

#endif
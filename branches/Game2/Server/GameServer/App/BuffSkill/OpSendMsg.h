//=============================================================================
/**
 *  file: OpSendMsg.h
 *
 *  Brief:��װ�˷�����Ϣ�Ĳ���
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-9-5
 */
//=============================================================================

#ifndef __OPERATOR_SENDMSG_H__
#define __OPERATOR_SENDMSG_H__
#pragma once

#include "Operator.h"
#include "BuffSkillDef.h"

class CMoveShape;
class CScript;

class COpSendMsg :
	public COperator
{
public:
	COpSendMsg(stModuParam *pParam);
	COpSendMsg(void * pParam);
	virtual ~COpSendMsg(void);

	enum eSendMsgType
	{
		SMT_ProcNo,	  //proceδ֪
		SMT_ProcBegin,//proce��ʼ
		SMT_ProcEnd,//proce����
		SMT_ProcFail,//ʧ��
	};
	//��Ϣ�ķ��ͷ�Χ
	enum eMsgRange
	{
		MsgRange_Single,//����
		MsgRange_Around,//��Χ
		MsgRange_Region,//����
	};
private:
	//����
	eSendMsgType m_eMsgType;
	//��Χ
	eMsgRange m_eRange;
	//����ֵ
	long m_lValue;
	//����ֵ�ű�
	string m_strVariaName;
	//���±�ʾ��
	BYTE  m_bUpdate;
	//˽�нӿ�
private:
	//���ͼ��ܿ�ʼ��Ϣ
	void CreateSkillBeginMsg();
	//���ͼ��ܽ�����Ϣ
	void CreateSkillEndMsg();
	//���ͼ���ʧ����Ϣ
	void CreateSKillFailMsg();

	//����buff��ʼ��Ϣ
	void CreateBuffBeginMsg();
	//����״̬������Ϣ
	void CreateBuffUpdateMsg();
	//����buff������Ϣ
	void CreateBuffEndMsg();
	//������Ʒ��Ϣ
	void SendLockGoodsMsg();

	typedef void (COpSendMsg::*CREATEMSG)(void);

	CREATEMSG m_pCreateMsg;

public:
	//��ʼ��
	void InitBaseData(eObj Obj,ModuleType eMType, eSendMsgType eType,eMsgRange eRange, long value,string strVname,int nAdd)
	{
		m_eMsgType = eType;
		m_eRange = eRange;
		m_lValue = value;
		m_strVariaName = strVname;
		m_bUpdate = nAdd;
		m_pCreateMsg = GetCreateMsg(eMType,m_eMsgType);

	}
	CREATEMSG GetCreateMsg(ModuleType eMType,eSendMsgType eMsgType)
	{
		CREATEMSG Msg;
		if (eMsgType==SMT_ProcBegin && (eMType==ModuType_Buff || eMType==ModuType_DeBuff))
		{
			Msg = &COpSendMsg::CreateBuffBeginMsg;
		} 
		else if(eMsgType==SMT_ProcEnd && (eMType==ModuType_Buff || eMType==ModuType_DeBuff))
		{
			Msg = &COpSendMsg::CreateBuffEndMsg;
		}
		else if (eMsgType==SMT_ProcBegin && (eMType==ModuType_Skill_Active 
										  || eMType==ModuType_Skill_Passive
										  || eMType==ModuType_Skill_OnOff
										 ))// || eMType==ModuType_Skill_Light
		{
			Msg = &COpSendMsg::CreateSkillBeginMsg;
		}
		else if (eMsgType==SMT_ProcEnd && (eMType==ModuType_Skill_Active 
										 || eMType==ModuType_Skill_Passive
										 || eMType==ModuType_Skill_OnOff
										 ))//|| eMType==ModuType_Skill_Light
		{
			Msg = &COpSendMsg::CreateSkillEndMsg;
		}
		else if (eMsgType==SMT_ProcFail && (eMType==ModuType_Skill_Active 
										 || eMType==ModuType_Skill_Passive
										 || eMType==ModuType_Skill_OnOff										 
										 || eMType==ModuType_Buff 
										 || eMType==ModuType_DeBuff))//|| eMType==ModuType_Skill_Light
		{
			Msg = &COpSendMsg::CreateSKillFailMsg;
		}
		return Msg;
	}

	
	//ִ��
    void Execute(CMoveShape* pShape);
};


//���͹���������Ч����Ϣ
class COpSendDamageMsg :
	public COperator
{
public:
	COpSendDamageMsg(stModuParam *pParam);
	virtual ~COpSendDamageMsg(void);

	enum eSendMsgType
	{
		SMT_SelfDamage,//��ͨ������Ϣ
		SMT_DestDamage,//������Ϣ
		SMT_DestDodge,//����������Ϣ
		SMT_DestFightBlock,//���͸���Ϣ
		SMT_DestParry,//�����м���Ϣ
		SMT_DestResist,//���͵ֿ���Ϣ
		SMT_DestSuck,//����������Ϣ
		SMT_DestRebound,//������Ϣ
	};

private:
	//��ͨ��Ѫ��Ϣ
	void CreateSelfDamageMsg(CMoveShape* pShape);
	//��Ѫ��Ϣ
	void CreateDestDamageMsg(CMoveShape* pShape);
	//����������Ϣ
	void CreateDestDodgeMsg(CMoveShape* pShape);
	//��������Ϣ
	void CreateDestBlockMsg(CMoveShape* pShape);
	//�м���Ϣ
	void CreateDestParryMsg(CMoveShape* pShape);
	//�ֿ���Ϣ
	void CreateDestResistMsg(CMoveShape* pShape);
	//������Ϣ
	void CreateDestSuckMsg(CMoveShape* pShape);
	//������Ϣ
	void CreateDestReboundMsg(CMoveShape* pShape);

	typedef void COpSendDamageMsg::CREATEMSG(CMoveShape* pShape);

	CREATEMSG* m_pCreateMsg;

	//����
	eSendMsgType m_eMsgType;
	//�Ƿ񱩻�
	long m_lBlastAttack;;
	//����ֵ�ű�
	string m_strBlastVariaName;
public:
	//ִ��
    void Execute(CMoveShape* pShape);
};
#endif

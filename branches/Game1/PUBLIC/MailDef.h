///////////////////////////////////////////////////////////////////////////////
///  MailDef.h
///
///  PATH: E:\alienbrainWork\Game_Program_Develop\guangmang\SERVER\GameServer\AppServer\Mail
///
///  CREATED: 09/12/2008  by ���Ƚ�
///
///  PURPOSE:   �ʼ���������
///
//////////////////////////////////////////////////////////////////////
#ifndef _MAILDEF_H_
#define _MAILDEF_H_
#pragma once

#include "GUID.h"
class CGoods;


//��ͨ��Ʒ
struct tagCGoods 
{
	CGUID goodsguid;
	long  lNum;
	long  lContainerID;
	long  lPos;
};
//��ֵ����
struct tagSGoods 
{
	long lIndex;
	long lNum;
};
//��ʱ��
struct tagTimer
{
	long lTimer;
	long lIntev;
	long lCount;
};
//��������
struct tagMailCondition
{
	string strName;		//��������
	long   lValue;		//ֵ
	string strOperator;	//������
};
//�����¼�
struct tagMailEvent
{	
	string  strEventName;
	long	lBeginTime;
	long    lEndTime;
};
//�ʼ�����״̬
enum eSaveState
{
	MAIL_SAVE_INITVALUE = 0,//��ʼֵ
	MAIL_SAVE_SAVEING,		//������
	MAIL_SAVE_SUCCESS_SAVE,	//����ɹ�
};

//����ʧ��
enum eMAILRESULT
{
	MAIL_SENDING_SUCCESS					=0,			//�ɹ�
	MAIL_SENDING_FAILED_INVALID_POSTAGE		=1,			//���ʲ���
	MAIL_SENDING_FAILED_INVALID_SUBJECT		=2,			//��������Ƿ��ַ�
	MAIL_SENDING_FAILED_INVALID_TEXT,					//���ݰ����Ƿ��ַ�
	MAIL_SENDING_FAILED_INVALID_EXTEXT,					//�������ݰ����Ƿ��ַ�
	MAIL_SENDING_FAILED_INVALID_GOLD,					//��ҳ�������
	MAIL_SENDING_FAILED_INVALID_GOODS,					//��Ʒ��������
	MAIL_SENDING_FAILED_INVALID_MAXNAME,				//���ֹ���
	MAIL_SENDING_FAILED_INVALID_MAXSUBJECT,				//�������
	MAIL_SENDING_FAILED_INVALID_MAXTEXT,				//�������ݹ���
	MAIL_SENDING_FAILED_INVALID_MANYRECEIVER,			//������Ⱥ���ʼ��а�������Ʒ��Ǯ
	MAIL_SENDING_FAILED_INVALID_RECEIVER,				//�Ƿ�������
	MAIL_SENDING_FALLED_INVALID_SUBJECTORRECEIVER,		//�������������
	MAIL_SENDING_FAILED_INVALID_STATE,					//�Ƿ�״̬
	MAIL_SENDING_FAILED_INVALID_TYPE,					//�ʼ����Ͳ�����
	MAIL_SENDING_FAILED_INVALID_SELF,					//�Ƿ�������
};

struct tagMailParam
{
	tagMailParam();

	CGUID					guid;						//guid
	string					strWriter;					//������	
	string					strSubject;					//����
	string					strText;					//�ʼ�����
	string					strExText;					//�ʼ���������
	time_t					tRemainTime;				//�������ʱ��	
	time_t					tEndTime;					//ϵͳȺ���Ľ���ʱ��
	time_t					tWriteTime;					//д��ʱ��
	long					lCost;						//����
	long					lGold;						//���
	BYTE					bRead;						//�ż����ı�־��
	long					lType;						//�ʼ�����
	BYTE					bReject;					//���ű�־����1��־����,0��ʾ�����ˣ�
	BYTE					bSave;						//�����־��(0��ʾ��ʼֵ��1��ʾδ���棬2��ʾ����ɹ�)
	CGUID					WriterGuid;					//д���˵�GUID
	CGUID					ReceiverGuid;				//�����˵�GUID
	string					strReceiver;				//������
	list<string>			strReceiverList;			//�������б�
	list<tagCGoods*>		lCGoods;					//��ͨ��Ʒ�б�
	list<tagSGoods*>		lSGoods;					//��ֵ�����б�		
	list<tagTimer*>			ltgTimer;					//��ʱ��
	list<tagMailCondition*> lMailCondition;				//��������
	list<tagMailEvent*>		ltgMailEvent;				//�����¼�

	vector<CGoods*>			vMailGoods;					//��Ʒ�б�
};
#endif//_MAILDEF_H_
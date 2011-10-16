
/*
*	file:		LeaveWordDef.h
*	Brief:		����ϵͳ�Ķ����ļ�
*	detail:				
*	Authtor:	�ſ���
*	Datae:		2008-03-06
*/

#pragma once

//!			���Գ���
const ULONG MAX_LEAVE_WORD_SIZE			= 1024;
//!			���������ֳ���
const ULONG MAX_SENDER_NAME_SIZE		= 20 + 1;

//! �����ߵ�����
enum eSenderType
{
	//! ϵͳ
	eSender_System,
	//! ���
	eSender_Player,
	//! ��ɾ����ɫ�����
	eSender_DeletedPlayer,
};

//! ���Խ��
enum eLeaveWordResult
{
	//! �ɹ�
	eLWR_OK						= 1,
	//! ���Թ���Ƶ�������Ժ��������
	eLWR_SendMore				= 0,
	//! ��Ը��������̫��
	eLWR_U2PlayerTooMore		= -1,
	//! �������������
	eLWR_ThePlayerBoxFull		= -2,
};

//! ���� ���ݽṹ��S2C��
struct  tagLeaveWord
{
	ULONG		uSenderType;						// �����ߵ����ͣ�ϵͳ����
	CHAR		szName[ MAX_SENDER_NAME_SIZE ];		// �����ߵ�����
	ulong		dwTime;								// �����ߵ�ʱ�������1970����������ת������ struct tm *localtime( const time_t *timer );
	tagLeaveWord(void){memset(this, 0, sizeof(tagLeaveWord));}
};


#ifdef USE_IN_SERVER

//! �������ýṹ
struct tagLeaveWordSetup
{
	//!		����ʱ���������������
	long	lMaxLeaveWordNum;
	//!		����ʱ��(��)
	long	lOptPeriodOfTime;
	//!		����͵���һ��ɫ������
	long	lMaxToPlayerLeaveWordNum;
	//!		��󱣴�������
	long	lMaxSaveLeaveWordNum;
	//!		��󱣴����ޣ��룩
	long	lMaxSaveTime;
};

struct tagLeaveWord_W2DB
{
	CGUID	ReceiverGuid;
	ULONG	uPRI;
	CGUID 	SenderGuid;
	long	dwSenderType;
	union
	{
		CHAR	szSenderName[ MAX_SENDER_NAME_SIZE ];//! �������ݷ��͵�ʱ�򱣴淢���ߵ�����
		CHAR	szReceiverName[ MAX_SENDER_NAME_SIZE ];//! ������Ϣ�����ʱ�򱣴�����ߵ�����
	};	
};


#endif //! USE_IN_SERVER
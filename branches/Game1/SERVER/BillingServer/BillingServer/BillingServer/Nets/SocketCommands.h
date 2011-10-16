//////////////////////////////////////////////////////////////////////////
//SocketCommands.h
//Fun:��Socket�Ĳ��������б�
//Create Time:2004.12.17
//Author:Joe
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <deque>
using namespace std;
class CServerClient;

//���������������
enum eSocketOperaType
{
	SOCKETOPERATYPE_ADD,			//���
	SOCKETOPERATYPE_CDKEYJOIN,		//��ҽ���
	SOCKETOPERATYPE_PLAYERJOIN,		//��ҽ���
	
	SOCKETOPERATYPE_DELBYSOCKETID,	//�����׽���IDɾ��

	SOCKETOPERATYPE_QUITBYSOCKETID,	//�����׽���IDɾ��
	SOCKETOPERATYPE_QUITBYMAPID,	//����ӳ�� IDɾ��
	SOCKETOPERATYPE_QUITBYMAPSTR,	//����ӳ���IDɾ��
	SOCKETOPERATYPE_QUITALL,		//ɾ��

	SOCKETOPERATYPE_RECIEVE,		//��������
	SOCKETOPERATYPE_SENDTOSOCKET,	//����SocketID��������
	SOCKETOPERATYPE_SENDTOMAPID,	//����MapID��������
	SOCKETOPERATYPE_SENDTOMAPSTR,	//����MapStr��������
	SOCKETOPERATYPE_SENDALL,		//�������еĿͻ���

	SOCKETOPERATYPE_ONRECEIVE,		//���ͻ��˿���������ʱ����
	SOCKETOPERATYPE_ONSEND,			//���ͻ��˿��Է�������ʱ����
	SOCKETOPERATYPE_ONCLOSE,		//���ͻ��˱��ر�ʱ��
	SOCKETOPERATYPE_ONCONNECT,		//���ؿͻ��˵��������

	SOCKETOPERATYPE_SENDEND,		//��ɶ˿��Ϸ��ͽ�����Ϣ
};

//��socket�Ĳ����ṹ����С:24Byte
struct tagSocketOper
{
	eSocketOperaType OperaType;
	unsigned long	 lSocketID;
	char*			 pStrID;					//�ַ���ID

	void*			 pBuf;						//����������
	long			 lNum1;						//����ֵ1
	long			 lNum2;						//����ֵ1
	//CServerClient*	 pServerClient;				//�ͻ����׽���
	

	tagSocketOper(eSocketOperaType eOperType,unsigned long lID,void* pTemptBuf,
		long lNumber1,long lNumber2=0)
		:OperaType(eOperType)
		,lSocketID(lID)
		,pStrID(NULL)
		,pBuf(pTemptBuf)
		,lNum1(lNumber1)
		,lNum2(lNumber2)
	{
	}


	tagSocketOper(eSocketOperaType eOperType,unsigned long lID,char* pBufID,void* pTemptBuf,
		long lNumber1,long lNumber2=0)
		:OperaType(eOperType)
		,lSocketID(lID)
		,pStrID(pBufID)
		,pBuf(pTemptBuf)
		,lNum1(lNumber1)
		,lNum2(lNumber2)
	{
	}
};

typedef deque<tagSocketOper*> CommandsQueue;

class CSocketCommands
{
private:
	CommandsQueue	 m_Commands;				//�����������
	CRITICAL_SECTION m_CriticalSectionCommans;
public:
	CSocketCommands(void);
	~CSocketCommands(void);

	bool Push_Front(tagSocketOper* pCommand);	// ѹ���������ǰ��
	bool Push_Back(tagSocketOper* pCommand);	// ѹ��������к��
	tagSocketOper* Pop_Front();					// ��ǰ�ε����������

	long GetSize();								// �õ�������г���
	void Clear();								// ����������]

	void CopyAllCommand(CommandsQueue& TemptCommandsQueue);
	void AddCommandsQueueToFront(CommandsQueue& TemptCommandsQueue);
};

#pragma once
#include "../../../../Public/Common/BaseDef.h"
// ����
class CChat
{
public:
	static string m_strCommand[TYPE_NUM];	// ����
	static long m_lInterleave[TYPE_NUM];	// ���Լ��
	static long m_lLastSendTime[TYPE_NUM];	// �ϴη���ʱ��

	static list<string> m_dirtyWords;			// �������˵��໰�ؼ���

public:
	static void Init();
	static long GetWord(char* strSource, char* strDest, long num, long lStartPos=0);	// ��ȡ�ڼ�������

	static eChatType GetChatType( LPCTSTR szInput, long& lPos );		// ��ȡ�����ģʽ

public:
	CChat(void);
	virtual ~CChat(void);
};

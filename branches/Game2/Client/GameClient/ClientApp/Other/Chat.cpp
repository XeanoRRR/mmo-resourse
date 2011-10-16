#include "stdafx.h"
#include "chat.h"
#include <strstream>
#include "../../../Public/Common/LinkmanDef.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ����
string CChat::m_strCommand[TYPE_NUM] =
							{ "/s",		// ��ͨ
							 "/y",		// ����
							  "/a",		// ����
							  "/g",		// �л�
							  "/p",		// ����
							  "/w",		// ˽��
							  "/c",		// ����
							  "/u",		// �˹�
							  "/����",	// ����

							  "/sys/",	// ϵͳ

							 "!!!",		// �㲥
							 "@",		//GM����
							 "//",		//�ű�-������
							 "/?",		// �������
							 "~",		//�������
							 "/npc/",	//npc����
							};


// ���Լ��
long CChat::m_lInterleave[TYPE_NUM] = { 2000,		// ��ͨ
										10000,		// ����
										1000,		// ����
										100,		// �л�
										1000,		// ����
										0,			// ˽��
										10000,		// ����
										10000,		// �˹�
										0,			// ����
										0,			// ϵͳ
										10000,		// �㲥
										0,			//GM����
										0,			//�ű�-������
										0			// �������
										};

// �ϴη���ʱ��
long CChat::m_lLastSendTime[TYPE_NUM] = {0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0};

//list<string*>CChat:: m_dirtyWords;

CChat::CChat(void)
{

}

CChat::~CChat(void)
{
}

void CChat::Init()
{
}


// ��ȡ���������
eChatType CChat::GetChatType( LPCTSTR szInput, long& lPos )
{
	if( szInput )
	{
		TCHAR szInputCmd[256];

		for( long c = 0; c < TYPE_NUM; c++ )
		{
			// ƥ�俪ͷ�������ַ���
			memcpy( szInputCmd, szInput, m_strCommand[ c ].size() );
			szInputCmd[ m_strCommand[ c ].size() ] = 0;
			CharLower( szInputCmd );

			if( ! lstrcmp( szInputCmd, m_strCommand[ c ].c_str() ) )
			{
				// ���� �����ַ���֮���λ�� �� ����
				lPos = (long)m_strCommand[c].size();

				// ȥ���ո�
				while( szInput[lPos] )
				{
					if( ' ' != szInput[lPos] )
						break;
					lPos++;
				}

				// ���������Ҫȥ����
				if( CHAT_PRIVATE == c )
				{
					TCHAR szName[ MAX_PLAYER_NAME_SIZE + 1 ];
					ZeroMemory( szName, sizeof(szName) );

					// ��ȡ����
					for( int i = 0; szInput[lPos] && i < MAX_PLAYER_NAME_SIZE; i++, lPos++ )
					{
						if( ' ' == szInput[lPos] )
							break;

						szName[i] = szInput[lPos];
					}

					//
					//GetGame()->GetCGuiEx()->GetMainBarPageEx()->m_strPrivateChatName = szName;
				}

				return (eChatType)c;
			}
		}
	}

	lPos = 0;
	return TYPE_NUM;
}


// ��ȡ�ڼ�������
long CChat::GetWord(char* strSource, char* strDest, long num, long lStartPos)
{
	strstream stream(&strSource[lStartPos], lstrlen(strSource)-lStartPos, ios::in);
	long l = 0;

	while( stream >> strDest )
	{
		if( stream.eof() )
		{
			strDest[0] = 0;
			return 0;
		}
		
		if( l == num )
		{
			long lPos = stream.tellg() + lStartPos;

			// ��ȡ�����������ݵ�λ��	
			for(lPos; lPos<lstrlen(strSource); lPos++)
			{
				if( strSource[lPos] != ' ' )
				{
					break;
				}
			}
			return lPos;
		}

		l++;
	}

	strDest[0] = 0;
	return 0;
}

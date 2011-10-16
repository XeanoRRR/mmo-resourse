

#include "stdafx.h"
#include "UpdateFile.h"

CUpdateFile::CUpdateFile(const char *pFileName)
:m_pContent(NULL)
,m_dwSize(0)
{
	memset(m_szName, 0, MAX_PATH);
	SetName(pFileName);
}
CUpdateFile::~CUpdateFile()
{
	if( m_pContent!=NULL)
		SAFE_DELETE_ARRAY(m_pContent);
}

CUpdateFile::CUpdateFile(const CUpdateFile &SrcObj)
:m_pContent(NULL)
,m_dwSize(0)
{
	strcpy(this->m_szName, SrcObj.m_szName);

	if(NULL != SrcObj.m_pContent)
	{
#ifndef IN_DEPTH_COPY
		throw;
#else
		if(NULL != this->m_pContent)
		{
			SAFE_DELETE_ARRAY(this->m_pContent);
		}

		this->m_dwSize = SrcObj.m_dwSize;
		this->m_pContent = new char[this->m_dwSize];
		memcpy(this->m_pContent, SrcObj.m_pContent, this->m_dwSize);
#endif
	}
}

CUpdateFile& CUpdateFile::operator=(const CUpdateFile &RightObj)
{
	new(this)CUpdateFile(RightObj);
	return (*this);
}

VOID CUpdateFile::SetName(LPCSTR pName)
{
	if(NULL != pName)
	{
		LONG lSize = strlen(pName);
		lSize = min((MAX_PATH - 1), lSize);
		memcpy(m_szName, pName, lSize);
		m_szName[lSize] = 0;
	}
}
BOOL CUpdateFile::LoadDataFromDir(LPCSTR pFileName)
{
	if( pFileName == NULL )
		return FALSE;
	//	�ļ����
	HANDLE hFile = NULL;
	hFile = ::CreateFile(pFileName,GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile==INVALID_HANDLE_VALUE)
	{
#ifdef USE_IN_CLIENT
		char strOut[128];
		sprintf(strOut,"��ȡ�ļ�<%s>ʧ��!",pFileName);
		::MessageBox(NULL,strOut,"��ʾ",MB_OK);
#endif		
		CloseHandle(hFile);
		return FALSE;
	}
	else
	{
		//	��ȡ�Ĵ�С
		DWORD dwReadedSize;
		//	�ļ���С
		m_dwSize = GetFileSize(hFile,NULL);
		//	���ļ�����
		if(NULL != m_pContent)
			SAFE_DELETE_ARRAY(m_pContent);
		m_pContent = new char[m_dwSize];
		ReadFile(hFile,m_pContent,m_dwSize,&dwReadedSize,NULL);

		CloseHandle(hFile);
		return TRUE;
	}
}


const DWORD INVALID_IDX = 0xFFFFFFFF;

DWORD FindSub(const char *pTxt, DWORD TxtSize, const char *pSub)
{
	DWORD BeginPos = 0;
	DWORD SubSize = strlen(pSub);

	while (TxtSize - BeginPos >= SubSize)
	{
		

		if(pTxt[BeginPos] == pSub[0] && TxtSize - BeginPos >= SubSize)
		{
			bool bInHz = false;
			//! �ж��Ƿ��ٺ����м�
			if(127 < (UCHAR)pTxt[BeginPos - 1])
			{
				LONG lNum = 0;
				for(LONG lTmpIdx = BeginPos - 1; 0 <= lTmpIdx; --lTmpIdx)
				{
					if(127 > (UCHAR)pTxt[lTmpIdx])
						break;
					lNum++;
				}

				if(1 == (lNum % 2))
					bInHz = true;
			}

			if (!bInHz && 0 == memcmp(&pTxt[BeginPos], pSub, SubSize))
			{
				return BeginPos;
			}
		}
		++BeginPos;
	}

	return INVALID_IDX;
}

BOOL StrCmpChar(const char *pTxt, DWORD Pos, char CmpChar)
{
	if (pTxt[Pos] == CmpChar)
	{
		//! �ж��Ƿ��ٺ����м�
		if(0 < Pos && 127 < (UCHAR)pTxt[Pos - 1])
		{
			LONG lNum = 0;
			for(LONG lTmpIdx = Pos - 1; 0 <= lTmpIdx; --lTmpIdx)
			{
				if(127 > (UCHAR)pTxt[lTmpIdx])
					break;
				lNum++;
			}

			if(1 == (lNum % 2))
				return false;
		}

		return true;
	}

	return false;
}

BOOL CUpdateFile::WriteDataToPath(LPCSTR pDirPath) const
{
	if( pDirPath == NULL )
		return FALSE;
	//	������
	if( m_pContent == NULL )
		return FALSE;

	char szDirName[256];
	if('\\' == pDirPath[strlen(pDirPath) - 1])
		sprintf(szDirName,"%s%s",pDirPath,m_szName);
	else
		sprintf(szDirName,"%s\\%s",pDirPath,m_szName);
	//	�ļ����
	HANDLE hFile;
//	CreateDirectory(pDirPath,NULL);
	hFile = ::CreateFile(szDirName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hFile)
	{
#ifdef USE_IN_CLIENT
		char strOut[128];
		sprintf(strOut,"д�ļ�<%s>ʧ��!",szDirName);
		::MessageBox(NULL,strOut,"��ʾ",MB_OK);
#endif
		return FALSE;
	}

	//	���￪ʼд�ļ�
	DWORD dwWritenSize = 0;

	//! �滻������س�ΪӲ�س�
	for (DWORD i = 0; i < m_dwSize; ++i)
	{
		if(StrCmpChar(m_pContent, i, '\r'))
			m_pContent[i] = '\n';
	}

	DWORD Pos = 0;



	while (Pos < m_dwSize)
	{
		//! ��ԭ��������������
		if (StrCmpChar(m_pContent, Pos, '\"'))
		{
			DWORD EndPos = Pos + 1;
			do 
			{
				if (StrCmpChar(m_pContent, EndPos, '\"'))
					break;
				++EndPos;
			} while(EndPos < m_dwSize);

			++EndPos;
			WriteFile(hFile, &m_pContent[Pos], EndPos - Pos, &dwWritenSize, NULL);
			Pos = EndPos;
			continue;
		}
		//! ȥ��//ע��
		else if (Pos < m_dwSize && StrCmpChar(m_pContent, Pos, '/') && StrCmpChar(m_pContent, Pos + 1, '/'))
		{
			do 
			{
				++Pos;
			} while(!StrCmpChar(m_pContent, Pos, '\n') && !StrCmpChar(m_pContent, Pos, '\r'));

			continue;
		}
		//! ȥ��/**/ע��
		else if (Pos < m_dwSize && StrCmpChar(m_pContent, Pos, '/') && StrCmpChar(m_pContent, Pos + 1, '*'))
		{
			do 
			{
				++Pos;
				if (Pos < m_dwSize && StrCmpChar(m_pContent, Pos, '*') && StrCmpChar(m_pContent, Pos + 1, '/'))
				{
					Pos += 2;
					break;
				}
			} while(Pos < m_dwSize);

			continue;
		}
		//! ���У������������һ����
		else if(StrCmpChar(m_pContent, Pos, '\n'))
		{
			while (Pos < m_dwSize && StrCmpChar(m_pContent, Pos + 1, '\n'))
			{
				++Pos;
			}
		}
		//! �ո������������һ����
		else if(StrCmpChar(m_pContent, Pos, ' '))
		{
			while (Pos < m_dwSize && StrCmpChar(m_pContent, Pos + 1, ' '))
			{
				++Pos;
			}
		}
		//! ȥ������
		else if(StrCmpChar(m_pContent, Pos, '	'))
		{
			++Pos;
			continue;
		}

		WriteFile(hFile, &m_pContent[Pos++], 1, &dwWritenSize, NULL);
	}

	//	�رվ��
	FlushFileBuffers(hFile);
	CloseHandle(hFile);

	printf("%s\r\n", szDirName);
	return TRUE;
}
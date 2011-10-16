//********************************************
//	Ini ��غ���
//********************************************

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tools.h"
#include "ini.h"
#include "RFile.h"

#ifdef	_CLIENT
#include "..\public\RFile.h"
#endif



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//��ʼ��
CIni::CIni()
{
	m_lDataLen = 0;
	m_pData = NULL;
	m_lIndexNum=0;
	m_pIndexList = NULL;
}

//��ʼ��
CIni::CIni(const char* filename, const char* pData)
{
	m_lDataLen = 0;
	m_pData = NULL;
	m_lIndexNum = 0;
	m_pIndexList = NULL;
	Open(filename, pData);
}

//�����ͷ�
CIni::~CIni()
{
	if( m_lDataLen != 0 )
	{
		//SAFE_DELETE( m_pData );
		SAFE_DELETE_ARRAY( m_pData );
		m_lDataLen = 0;
	}

	if( m_lIndexNum != 0 )
	{
		SAFE_DELETE( m_pIndexList );
		m_lIndexNum = 0;
	}
}

//�����ļ�
bool CIni::Open(const char* filename, const char* pData)
{

//#ifdef _CLIENT
	if (filename)
		strcpy(m_strFileName, filename);
	SAFE_DELETE_ARRAY( m_pData );

	// �ڴ渴��
	if (pData)
	{
		m_lDataLen=lstrlen(pData);	//��ȡ�ļ�����

		m_pData = new char[m_lDataLen+1];

		strcpy(m_pData, pData);		//������

		//��ʼ������
		InitIndex();
		return true;
	}
	else	// ��ȡ�ļ�
	{

		CRFile* prfile = rfOpen(filename);		
		//�ļ�����
		if( prfile)
		{
			m_lDataLen=prfile->GetDatalen();
			if(m_lDataLen>0)
			{
				m_pData = new char[m_lDataLen];
				prfile->ReadData(m_pData, m_lDataLen);
			}
			
			rfClose(prfile);
			//��ʼ������
			InitIndex();
			return true;
		}
		else
		{
			m_lDataLen=GetFileLength(filename);	//��ȡ�ļ�����
			//�ļ�����
			if(  m_lDataLen > 0 )
			{
				m_pData = new char[m_lDataLen];

				FILE *fp;
				fp=fopen(filename, "rb");
				assert( fp!=NULL );
				fread(m_pData, m_lDataLen, 1, fp);		//������
				fclose(fp);

				//��ʼ������
				InitIndex();
				return true;
			}
			else	// �ļ�������
			{
				// �Ҳ����ļ�
				m_lDataLen=1;
				m_pData = new char[m_lDataLen];
				memset(m_pData, 0, 1);
				InitIndex();
			}
		}
	}

	return false;


//#else
//
//	if (filename)
//		strcpy(m_strFileName, filename);
//	SAFE_DELETE_ARRAY( m_pData );
//
//	// �ڴ渴��
//	if (pData)
//	{
//		m_lDataLen=lstrlen(pData);	//��ȡ�ļ�����
//
//		m_pData = new char[m_lDataLen+1];
//
//		strcpy(m_pData, pData);		//������
//
//		//��ʼ������
//		InitIndex();
//		return true;
//	}
//	else	// ��ȡ�ļ�
//	{
//
//			m_lDataLen=GetFileLength(filename);	//��ȡ�ļ�����
//
//			//�ļ�����
//			if(  m_lDataLen > 0 )
//			{
//				m_pData = new char[m_lDataLen];
//
//
//				FILE *fp;
//				fp=fopen(filename, "rb");
//				assert( fp!=NULL );
//				fread(m_pData, m_lDataLen, 1, fp);		//������
//				fclose(fp);
//
//				//��ʼ������
//				InitIndex();
//				return true;
//			}
//			else	// �ļ�������
//			{
//				// �Ҳ����ļ�
//				m_lDataLen=1;
//				m_pData = new char[m_lDataLen];
//				memset(m_pData, 0, 1);
//				InitIndex();
//			}
//	}
//
//	return false;
//
//#endif
}

//�ر��ļ�
void CIni::Close()
{
	if( m_lDataLen != 0 )
	{
		SAFE_DELETE( m_pData );
		m_lDataLen = 0;
	}

	if( m_lIndexNum != 0 )
	{
		SAFE_DELETE( m_pIndexList );
		m_lIndexNum = 0;
	}
}

//д���ļ�
bool CIni::Save(const char *filename)
{
	if( filename==NULL )
	{
		filename=m_strFileName;
	}

	FILE *fp;
	fp=fopen(filename, "wb");
	if( fp )
	{
		//fprintf(fp,"%s",m_pData);
		fwrite(m_pData, m_lDataLen, 1, fp);
		fclose(fp);
	}
	else
	{
		assert( fp!=NULL );
		return false;
	}

	return true;
}

//��������е�����λ��
void CIni::InitIndex()
{
	m_lIndexNum=0;

	int i = 0;
	for(; i<m_lDataLen; i++)
	{
		//�ҵ�
		if( m_pData[i]=='[' && (m_pData[i-1]=='\n' || i==0) )
		{
			m_lIndexNum++;
		}
	}

	//�����ڴ�
	SAFE_DELETE( m_pIndexList );
	if( m_lIndexNum>0 )
		m_pIndexList=new int[m_lIndexNum];

	int n=0;

	for(i=0; i<m_lDataLen; i++)
	{
		if( m_pData[i]=='[' && (m_pData[i-1]=='\n' || i==0) )
		{
			m_pIndexList[n]=i+1;
			n++;
		}
	}
}

/////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////

//����ָ������λ��
int CIni::FindIndex(char *string)
{
	for(int i=0; i<m_lIndexNum; i++)
	{
		char *str=ReadText( m_pIndexList[i] );
		if( strcmp(string, str) == 0 )
		{
			SAFE_DELETE_ARRAY( str );
			return m_pIndexList[i];
		}
		SAFE_DELETE_ARRAY( str );
	}
	return -1;
}

//���صڼ�������
int CIni::FindIndexPos(char *string)
{
	for(int i=0; i<m_lIndexNum; i++)
	{
		char *str=ReadText( m_pIndexList[i] );
		if( strcmp(string, str) == 0 )
		{
			SAFE_DELETE_ARRAY( str );
			return i;
		}
		SAFE_DELETE_ARRAY( str );
	}
	return -1;
}

//���صڼ�������λ��
int CIni::FindIndex(int nPos)
{
	if (nPos>=0 && nPos<m_lIndexNum)
	{
		return m_pIndexList[nPos];
	}
	return -1;
}

// ������һ������λ��
int CIni::FindNextIndex(int nPos)
{
	for(int i=0; i<m_lIndexNum; i++)
	{
		if (m_pIndexList[i] > nPos)
			return m_pIndexList[i];
	}
	return -1;
}

//����ָ�����ݵ�λ��
int CIni::FindData(int index, char *string)
{
	int p=index;	//ָ��

	int endindex = FindNextIndex(index);
	if (endindex==-1)
		endindex = m_lDataLen;

	while(1)
	{
		p=GotoNextLine(p);
		char *name=ReadDataName(p);
		if( strcmp(string, name)==0 )
		{
			SAFE_DELETE_ARRAY( name );
			return p;
		}

		SAFE_DELETE_ARRAY( name );
		if( p>=endindex) return -1;
	}
	return -1;
}

//����
int CIni::GotoNextLine(int p)
{
	int i = p;
	for(; i<m_lDataLen; i++)
	{
		if( m_pData[i]=='\n' )
			return i+1;
	}
	return i;
}

//��ָ��λ�ö�һ��������
char *CIni::ReadDataName(int &p)
{
	char chr;
	char *Ret;
	int m=0;

	Ret=new char[64];
	memset(Ret, 0, 64);

	for(int i=p; i<m_lDataLen; i++)
	{
		chr=m_pData[i];

		//����
		if( chr == '\r' )
		{
			p=i+1;
			return Ret;
		}
		
		//����
		if( chr == '=' || chr == ';' )
		{
			p=i+1;
			return Ret;
		}
		
		Ret[m]=chr;
		m++;
	}
	return Ret;
}

//��ָ��λ�ö�һ�ַ���
char *CIni::ReadText(int p)
{
	char chr;
	char *Ret;
	int n=p, m=0;

	int LineNum = GotoNextLine(p) - p + 1;
	Ret=new char[LineNum];
	memset(Ret, 0, LineNum);

	for(int i=0; i<m_lDataLen-p; i++)
	{
		chr=m_pData[n];

		//����
		if( chr == ';' || chr == '\n' || chr == '\r' || chr == '\t' || chr == ']' )
		{
			//ShowMessage(Ret);
			return Ret;
		}
		
		Ret[m]=chr;
		m++;
		n++;
	}

	return Ret;
}

/////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////

//����һ������
bool CIni::AddIndex(char *index)
{
	char str[256];
	int n=FindIndex(index);

	if( n == -1 )	//�½�����
	{
		sprintf(str,"\r\n[%s]\r\n",index);

		// ���·����ڴ�
//		char* pTemp = new char[m_lDataLen+strlen(str)+1];
//		memcpy(pTemp, m_pData, m_lDataLen);
//		delete( m_pData );
//		m_pData = pTemp;

		m_pData = (char *)realloc(m_pData, m_lDataLen+strlen(str)+1);	//���·����ڴ�
		sprintf(&m_pData[m_lDataLen], "%s", str);
		m_lDataLen+=lstrlen(str);

		InitIndex();
		return true;
	}
	
	return false;	//�Ѿ�����
}

//�ڵ�ǰλ�ü���һ������
bool CIni::AddData(int p, char *name, char *string)
{
	char *str;
	int len = lstrlen(string);
	str=new char[len+256];
	sprintf(str,"%s=%s\r\n",name,string);
	len=lstrlen(str);

	p=GotoNextLine(p);	//����
	m_pData = (char *)realloc(m_pData, m_lDataLen+len+1);	//���·����ڴ�

	char *temp=new char[m_lDataLen-p];
	memcpy(temp, &m_pData[p], m_lDataLen-p);
	memcpy(&m_pData[p+len], temp, m_lDataLen-p);	//�Ѻ���İᵽĩβ
	memcpy(&m_pData[p], str, len);
	m_lDataLen+=len;

	SAFE_DELETE( temp );
	SAFE_DELETE( str );
	return true;
}

//�ڵ�ǰλ���޸�һ�����ݵ�ֵ
bool CIni::ModityData(int p, char *name, char *string)
{
	int n=FindData(p, name);

	char *t=ReadText(n);
	p=n+lstrlen(t);
	if( lstrlen(t)>0 ) free(t);

	int newlen=lstrlen(string);
	int oldlen=p-n;

	m_pData = (char *)realloc(m_pData, m_lDataLen+newlen-oldlen+1);	//���·����ڴ�

	char *temp=new char[m_lDataLen-p];
	memcpy(temp, &m_pData[p], m_lDataLen-p);
	memcpy(&m_pData[n+newlen], temp, m_lDataLen-p);			//�Ѻ���İᵽĩβ
	memcpy(&m_pData[n], string, newlen);
	m_lDataLen+=newlen-oldlen;

	SAFE_DELETE( temp );
	return true;
}

//�����ļ�����
char *CIni::GetData()
{
	return m_pData;
}

//����ļ�������
int CIni::GetLines(int cur)
{
	int n=1;
	for(int i=0; i<cur; i++)
	{
		if( m_pData[i]=='\n' )
			n++;
	}
	return n;
}

/////////////////////////////////////////////////////////////////////
// ����ӿ�
/////////////////////////////////////////////////////////////////////

//����ͨ��ʽ��һ�ַ�������
char *CIni::ReadText(char *index, char *name)
{
	int n=FindIndex(index);
	assert( n != -1 );

	int m=FindData(n, name);
	if( m == -1 )
	{
		char* temp = new char[1];
		temp[0] = 0;
		return temp;
	}

	return ReadText(m);
}
	
//��ָ�����ж�һ�ַ���
char *CIni::ReadText(char *index, int lines)
{
	int n=FindIndex(index);
	assert( n != -1 );

	//����ָ������
	n=GotoNextLine(n);
	for(int i=0; i<lines; i++)
	{
		if( n<m_lDataLen )
			n=GotoNextLine(n);
	}

	//������
	while( n<=m_lDataLen )
	{
		if( m_pData[n] == '=' )
		{
			n++;
			return ReadText(n);
		}
		if( m_pData[n] == '\r' )
		{
			return "";
		}
		n++;
	}

	return "";
}

//����ͨ��ʽ��һ����
int CIni::ReadInt(char *index, char *name)
{
	int n=FindIndex(index);
	assert( n != -1 );

	int m=FindData(n, name);
	//assert( m != -1 );
	if( m == -1 )
	{
		return ERROR_DATA;
	}

	char *str=ReadText(m);
	int ret=atoi(str);
	free(str);
	return ret;
}

//��ָ�����ж�һ����
int CIni::ReadInt(char *index, int lines)
{
	int n=FindIndex(index);
	assert( n != -1 );

	//����ָ������
	n=GotoNextLine(n);
	for(int i=0; i<lines; i++)
	{
		if( n<m_lDataLen )
			n=GotoNextLine(n);
	}

	//������
	while( n<m_lDataLen )
	{
		if( m_pData[n] == '=' )
		{
			n++;
			char *str=ReadText(n);
			int ret=atoi(str);
			free(str);
			return ret;
		}
		if( m_pData[n] == '\r' )
		{
			return ERROR_DATA;
		}
		n++;
	}

	return ERROR_DATA;
}

//��ָ�����ж�һ��������
char *CIni::ReadCaption(char *index, int lines)
{
	int n=FindIndex(index);
	assert( n != -1 );

	//����ָ������
	n=GotoNextLine(n);
	for(int i=0; i<lines; i++)
	{
		if( n<m_lDataLen )
			n=GotoNextLine(n);
	}

	return ReadDataName(n);
}

//����ͨ��ʽдһ�ַ�������
bool CIni::Write(char *index, char *name, char *string)
{
	int n=FindIndex(index);
	if( n == -1 )	//�½�����
	{
		AddIndex(index);
		n=FindIndex(index);
		n=GotoLastLine(index);
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	int m=FindData(n, name);
	if( m==-1 )		//�½�����
	{
		n=GotoLastLine(index);
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	ModityData(n, name, string);	//�޸�һ������

	return true;
}

//����ͨ��ʽдһ����
bool CIni::Write(char *index, char *name, int num)
{
	char string[64];
	sprintf(string, "%d", num);

	int n=FindIndex(index);
	if( n == -1 )	//�½�����
	{
		AddIndex(index);
		n=FindIndex(index);
		n=GotoLastLine(index);
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	int m=FindData(n, name);
	if( m==-1 )		//�½�����
	{
		n=GotoLastLine(index);
		AddData(n, name, string);	//�ڵ�ǰλ��n��һ������
		return true;
	}

	//��������
	ModityData(n, name, string);	//�޸�һ������

	return true;
}

//��������������
int CIni::GetContinueDataNum(char *index)
{
	int num=0;
	int n=FindIndex(index);
	n=GotoNextLine(n);
	while(1)
	{
		if( n >= m_lDataLen || m_pData[n] == '\r' || m_pData[n] == EOF || m_pData[n] == ' ' || m_pData[n] == '/' || m_pData[n] == '\t' || m_pData[n] == '\n' )
		{
			return num;
		}
		else
		{
			num++;
			n=GotoNextLine(n);
			if( n >= m_lDataLen ) return num;
		}
	}
}

//��ָ���ƶ�����INDEX�����һ��
int CIni::GotoLastLine(char *index)
{
	int n=FindIndex(index);
	n=GotoNextLine(n);
	while(1)
	{
		if( m_pData[n] == '\r' || m_pData[n] == EOF || m_pData[n] == -3 || m_pData[n] == ' ' || m_pData[n] == '/' || m_pData[n] == '\t' || m_pData[n] == '\n' )
		{
			return n;
		}
		else
		{
			n=GotoNextLine(n);
			if( n >= m_lDataLen ) return n;
		}
	}
}

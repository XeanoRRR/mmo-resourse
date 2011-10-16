//////////////////////////////////////////////////////////////////////////
//RFile.h,��Դ�ļ���
//Fun:��Դ�ļ�����,�����д����
//Author:Joe
//Create Time:2004.11.26
//////////////////////////////////////////////////////////////////////////
#include <string>

#pragma once

class CClientResource;

class CRFile
{
public:
	CRFile(const char* pszFileName,BYTE* pData,DWORD dwSize);
	CRFile(const char* pszFileName,FILE* pfile);
	~CRFile(void);
private:
	BYTE*		m_pData;			//����
	DWORD		m_dwSize;
	DWORD		m_dwPos;

	//�ļ���
	char m_pszFileName[MAX_PATH];

public:
	const char* GetFileName()	{return m_pszFileName;}
	DWORD GetPosition()	{return m_dwPos;}
	void SetPosition(DWORD dwPosition) { m_dwPos = dwPosition;}

	BYTE* GetData()	{return m_pData;}
	long GetDatalen()	{return m_dwSize;}					//�������ݳ���
	bool ReadData(void* pBuf,DWORD dwSize);					//�����ݵ�������
	bool ReadData(int& nValue);								//��ini��������
	bool ReadData(DWORD& nValue);							//��DWORD��������
	bool ReadChar(unsigned char &c);
	char * ReadLine(char * pszLineBuffer, int iBufferLen);
	bool ReadToStream(stringstream& stream);				//�����ݵ�stringstream��

	DWORD  Seek(int iPosition,int iOrigin = 0);

	static char *GetFilePath(const char *szFileName, char* szOutput);//����"/" �� "\\"
};

////����Դ�ļ���ȫ�ֺ���
//CRFile* rfOpen(const char* strFileName, CClientResource* pClientResource=NULL);	//���ļ�����,����rfClose�ر�
//void rfClose(CRFile* rfile);						//�ر�CRFile�ļ����

//ʹ�ú�����ļ�·�����
void CheckRFileStr(string& strFileName);


// ȱʡ�Ŀͻ�����Դ
extern CClientResource* g_pDefaultClientResource;
CClientResource* GetDefaultClientResource();
void SetDefaultClientResource(CClientResource* p);

//=============================================================================
/**
 *  file: MapFile.cpp
 *
 *  Brief:ͨ��Windows���ļ�Ӱ����ƣ���װ�˼򵥵�Ӱ���ļ���������
 *			������ص��ļ���־��¼
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2008-10-29
 */
//=============================================================================

#include "StdAfx.h"
#include "MapFile.h"


CMapFile::CMapFile(void)
:m_hFile(INVALID_HANDLE_VALUE)
,m_hMapObject(NULL)
,m_pszMapView(NULL)
,m_eType(FMT_No)
{
}

CMapFile::~CMapFile(void)
{
}

//��ʼ��
bool CMapFile::Initial(eFileMapServerType eFMType,const char* pszMapObjName,const char* pFileName)
{
	if(!pszMapObjName)	return false;
	m_eType = (eFileMapServerType)(m_eType|eFMType);

	//���д����
	if(m_eType&FMT_Server)
	{
		if(pFileName)
		{
			m_hFile = CreateFile(_TEXT(pFileName),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,
				OPEN_ALWAYS,0,0);
			if(INVALID_HANDLE_VALUE == m_hFile)
			{
				char pszErrorInfo[1024]="";
				_snprintf(pszErrorInfo,1024,"��ʼ��MapFileʱ,�����ļ�(%s)����.(ErrorID:%d)",pFileName,GetLastError());
				PutDebugString(pszErrorInfo);
				m_eType = FMT_No;
				return false;
			}
		}

		m_hMapObject = CreateFileMapping(m_hFile,NULL,
			PAGE_READWRITE, 0, MAX_MAP_VIEW_SIZE, _TEXT(pszMapObjName));

		if( !m_hMapObject ) 
		{ 
			CloseHandle(m_hFile); 
			m_eType = FMT_No;
			char pszErrorInfo[1024]="";
			_snprintf(pszErrorInfo,1024,"��ʼ��MapFileʱ,����Ӱ���ļ�(%s)����.(ErrorID:%d)",pszMapObjName,GetLastError());
			PutDebugString(pszErrorInfo);
			return false;
		}
		m_pszMapView = MapViewOfFile(m_hMapObject,FILE_MAP_WRITE, 0, 0,0);
		if(!m_pszMapView)
		{
			CloseHandle(m_hMapObject); 
			CloseHandle(m_hFile); 
			m_eType = FMT_No;
			char pszErrorInfo[1024]="";
			_snprintf(pszErrorInfo,1024,"��ʼ��MapFileʱ,Ӱ������ʱ(%s)����.(ErrorID:%d)",pszMapObjName,GetLastError());
			PutDebugString(pszErrorInfo);
			return false;
		}

		char pszMutexObj[256]="";
		_snprintf(pszMutexObj,256,"%s_Mutex",pszMapObjName);
		m_hMutex = CreateMutex(NULL,FALSE,pszMutexObj); 
		if(!m_hMutex)
		{
			CloseHandle(m_hMapObject); 
			CloseHandle(m_hFile);			
			m_eType = FMT_No;
			char pszErrorInfo[1024]="";
			_snprintf(pszErrorInfo,1024,"��ʼ��MapFileʱ,�����������(%s)����.(ErrorID:%d)",pszMutexObj,GetLastError());
			PutDebugString(pszErrorInfo);
			return false;
		}

		//���������¼����
		char pszEventObj[256]="";
		_snprintf(pszEventObj,256,"%s_Event",pszMapObjName);
		m_hEvent = CreateEvent(NULL,FALSE,FALSE,pszEventObj);
		if(!m_hEvent)
		{
			UnmapViewOfFile(m_pszMapView);
			CloseHandle(m_hMapObject); 
			CloseHandle(m_hFile);			
			m_eType = FMT_No;
			char pszErrorInfo[1024]="";
			_snprintf(pszErrorInfo,1024,"��ʼ��MapFileʱ,�����¼�����(%s)����.(ErrorID:%d)",pszEventObj,GetLastError());
			PutDebugString(pszErrorInfo);
			return false;
		}

		//����ʼ����д���ݵ�λ��ֵ
		WaitForSingleObject(m_hMutex,INFINITE);
		//ǰ4���ֽڱ�ʾ��ʼд���ݵ�λ��
		*(long*)(m_pszMapView) = 8;
		//�κ�4���ֽڱ�ʾ�����ݵ�λ��
		*(long*)((char*)m_pszMapView+4) = 8;
		ReleaseMutex(m_hMutex);

	}
	//����Ƕ�����
	else if(m_eType&FMT_Client)
	{
		m_hMapObject = OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,_TEXT(pszMapObjName));

		if( !m_hMapObject ) 
		{
			m_eType = FMT_No;
			char pszErrorInfo[1024]="";
			_snprintf(pszErrorInfo,1024,"��ʼ��MapFileʱ,��Ӱ�����(%s)����.(ErrorID:%d)",pszMapObjName,GetLastError());
			PutDebugString(pszErrorInfo);
			return false;
		}
		m_pszMapView = MapViewOfFile(m_hMapObject,FILE_MAP_ALL_ACCESS, 0, 0,0);
		if(!m_pszMapView)
		{
			CloseHandle(m_hMapObject); 
			m_eType = FMT_No;
			char pszErrorInfo[1024]="";
			_snprintf(pszErrorInfo,1024,"��ʼ��MapFileʱ,Ӱ������ʱ(%s)����.(ErrorID:%d)",pszMapObjName,GetLastError());
			PutDebugString(pszErrorInfo);
			return false;
		}

		char pszMutexObj[256]="";
		_snprintf(pszMutexObj,256,"%s_Mutex",pszMapObjName);
		m_hMutex = OpenMutex(MUTEX_ALL_ACCESS,FALSE,pszMutexObj);
		if(!m_hMutex)
		{
			CloseHandle(m_hMapObject); 
			m_eType = FMT_No;
			char pszErrorInfo[1024]="";
			_snprintf(pszErrorInfo,1024,"��ʼ��MapFileʱ,�򿪻������(%s)����.(ErrorID:%d)",pszMutexObj,GetLastError());
			PutDebugString(pszErrorInfo);
			return false;
		}
		//���������¼����
		char pszEventObj[256]="";
		_snprintf(pszEventObj,256,"%s_Event",pszMapObjName);
		m_hEvent = OpenEvent(EVENT_ALL_ACCESS,FALSE,pszEventObj);
		if(!m_hEvent)
		{
			UnmapViewOfFile(m_pszMapView);
			CloseHandle(m_hMapObject); 
			m_eType = FMT_No;
			char pszErrorInfo[1024]="";
			_snprintf(pszErrorInfo,1024,"��ʼ��MapFileʱ,���¼�����(%s)����.(ErrorID:%d)",pszEventObj,GetLastError());
			PutDebugString(pszErrorInfo);
			return false;
		}
	}

	return true;
}
//�ͷ���Դ
void CMapFile::Release()
{
	if(m_pszMapView)
		UnmapViewOfFile(m_pszMapView);
	if(m_hMapObject)
		CloseHandle(m_hMapObject); 
	if(INVALID_HANDLE_VALUE != m_hFile)
		CloseHandle(m_hFile);	
	return ;
}

//��������
bool CMapFile::Reset(eFileMapServerType eFMType,const char* pszMapObjName,const char* pFileName)
{
	//���ͷ�
	Release();
	return Initial(eFMType,pszMapObjName,pFileName);
}

//���ռ��С�Ƿ�����
bool CMapFile::CheckViewSpace(long lNeedSize)
{
	//д���ݵ�λ��
	long lWritePos = *(long*)(m_pszMapView);
	//������λ��
	long lReadPos = *(long*)((char*)m_pszMapView+4);
	//������С
	if( lNeedSize > (MAX_MAP_VIEW_SIZE-sizeof(8)) )
		return false;
	//��������
	if( lWritePos < lReadPos && (lWritePos+lNeedSize) >= lReadPos)
		return false;
	//��������
	if( (lWritePos >=  lReadPos) && lNeedSize > (MAX_MAP_VIEW_SIZE-lWritePos+lReadPos-8) )
		return false;
	return true;
}

//�����´�д���ݵ�λ��
int CMapFile::WriteData(int nPos,const char *pData,int nLen)
{
	char* pPos = (char*)m_pszMapView+nPos;

	if( (nPos+nLen) < MAX_MAP_VIEW_SIZE )
	{
		memcpy(pPos,pData,nLen);
		return nPos+nLen;
	}
	int nSize = MAX_MAP_VIEW_SIZE-nPos;
	memcpy(pPos,pData,nSize);
	//��ͷ��ʼ
	pPos = (char*)m_pszMapView+8;
	memcpy(pPos,pData+nSize,nLen-nSize);
	return 8+nLen-nSize;
}
//��Χ�´ζ����ݵ�λ��
int CMapFile::ReadData(int nPos,char *pData,int nLen)
{
	char* pPos = (char*)m_pszMapView+nPos;
	if( (nPos+nLen) < MAX_MAP_VIEW_SIZE )
	{
		memcpy(pData,pPos,nLen);
		return nPos+nLen;
	}

	int nSize = MAX_MAP_VIEW_SIZE-nPos;
	memcpy(pData,pPos,nSize);
	//��ͷ��ʼ
	pPos = (char*)m_pszMapView+8;
	memcpy(pData+nSize,pPos,nLen-nSize);
	return 8+nLen-nSize;;
}

///���ļ����б仯����
void CMapFile::DoFile(eFileOpType type,const char* pszFileName)
{
	if(!pszFileName)	return;

	WaitForSingleObject(m_hMutex,INFINITE);
	long lFileNameLen = strnlen(pszFileName,256);
	long lNeedSize = lFileNameLen+sizeof(short)*2;

	if(!CheckViewSpace(lNeedSize))
	{
		ReleaseMutex(m_hMutex); 
		//_snprintf(pszErrorInfo,1024,"��MapFileд����(filename:%s),�ռ䲻��",pszFileName);
		//PutDebugString(pszErrorInfo);
		return;
	}	
	//д���ݵ�λ��
	long lWritePos = *(long*)(m_pszMapView);
	//������λ��
	long lReadPos = *(long*)((char*)m_pszMapView+4);
	//��������ݵ�λ�õ���д���ݵ�λ�ã�����Ⱥ�Ķ�����
	if(lWritePos == lReadPos)
		SetEvent(m_hEvent);
	//д��������
	lWritePos = WriteData(lWritePos,(char*)&type,sizeof(short));
	//д�ļ�������
	lWritePos = WriteData(lWritePos,(char*)&lFileNameLen,sizeof(short));
	//д�ļ�������
	lWritePos = WriteData(lWritePos,pszFileName,lFileNameLen);
	*(long*)(m_pszMapView) = lWritePos;	
	ReleaseMutex(m_hMutex);

}

//д����
//�����������ݸ�ʽΪ:
//д����λ��(4byte)+������λ��(4byte)+��������(2byte)+�ļ�������(2byte) +�ļ�������+д�����ݳ���(4byte)+Ҫд������
void CMapFile::Write(const char* pszFileName,const char* pszText)
{
	if( !(m_eType&FMT_Client) ) return;
	if(!pszText || !pszFileName)	return;

	WaitForSingleObject(m_hMutex,INFINITE);
	long lFileNameLen = strnlen(pszFileName,256);
	long lTextLen = strnlen(pszText,1048576);
	long lNeedSize = lFileNameLen+lTextLen+sizeof(long)+sizeof(short)*2;

	if(!CheckViewSpace(lNeedSize))
	{
		ReleaseMutex(m_hMutex); 
		//_snprintf(pszErrorInfo,1024,"��MapFileд����(filename:%s),�ռ䲻��",pszFileName);
		//PutDebugString(pszErrorInfo);
		return;
	}
	
	//д���ݵ�λ��
	long lWritePos = *(long*)(m_pszMapView);
	//������λ��
	long lReadPos = *(long*)((char*)m_pszMapView+4);
	//��������ݵ�λ�õ���д���ݵ�λ�ã�����Ⱥ�Ķ�����
	if(lWritePos == lReadPos)
		SetEvent(m_hEvent);
	//д��������
	eFileOpType opType = File_Write;
	lWritePos = WriteData(lWritePos,(char*)&opType,sizeof(short));
	//д�ļ�������
	lWritePos = WriteData(lWritePos,(char*)&lFileNameLen,sizeof(short));
	//д�ļ�������
	lWritePos = WriteData(lWritePos,pszFileName,lFileNameLen);

	//д���ݳ���
	lWritePos = WriteData(lWritePos,(char*)&lTextLen,sizeof(long));
	//д����
	lWritePos = WriteData(lWritePos,pszText,lTextLen);	
	*(long*)(m_pszMapView) = lWritePos;	
	ReleaseMutex(m_hMutex);
	return;
}
//������
eFileOpType CMapFile::Read(char* pszFileName,char* pszText)
{
	eFileOpType eRet = File_No;
	if( !(m_eType&FMT_Server) ) return eRet;

	WaitForSingleObject(m_hMutex,INFINITE);
	//д���ݵ�λ��
	long lWritePos = *(long*)(m_pszMapView);
	//������λ��
	long lReadPos = *(long*)((char*)m_pszMapView+4);

	if(lWritePos == lReadPos)
	{
		ReleaseMutex(m_hMutex); 
		//�Ⱥ�1��
		WaitForSingleObject(m_hEvent,200);
		WaitForSingleObject(m_hMutex,INFINITE);
		//д���ݵ�λ��
		lWritePos = *(long*)(m_pszMapView);
		//������λ��
		lReadPos = *(long*)((char*)m_pszMapView+4);
		if(lWritePos == lReadPos)
		{
			//ֱ�ӷ���
			ReleaseMutex(m_hMutex); 
			return eRet;
		}
	}

	long lFileNameLen = 0;
	long lTextLen = 0;
	//����������
	lReadPos = ReadData(lReadPos,(char*)&eRet,sizeof(short));
	//���ļ���
	lReadPos = ReadData(lReadPos,(char*)&lFileNameLen,sizeof(short));
	if(lFileNameLen > 0)
		lReadPos = ReadData(lReadPos,pszFileName,lFileNameLen);
	pszFileName[lFileNameLen]='\0';
	if(eRet == File_Write)
	{
		//������
		lReadPos = ReadData(lReadPos,(char*)&lTextLen,sizeof(long));
		if(lFileNameLen > 0)
			lReadPos = ReadData(lReadPos,pszText,lTextLen);
	}
	pszText[lTextLen]='\0';
	*(long*)((char*)m_pszMapView+4) = lReadPos;
	ReleaseMutex(m_hMutex); 
	return eRet;
}

//=============================================================================
/**
 *  file: MapFile.h
 *
 *  Brief:ͨ��Windows���ļ�Ӱ����ƣ���װ�˼򵥵�Ӱ���ļ���������
 *			�����ļ���־��¼
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2008-10-29
 */
//=============================================================================

#ifndef __MAP_FILE_H__
#define __MAP_FILE_H__
#pragma once


const long MAX_FILENAME_LEN = 256;
const long MAX_CONTENT_LEN = 1024*10;
const long MAX_MAP_VIEW_SIZE = 1048576*10;

//Ӱ���ļ��Ĵ򿪷�ʽ
enum eFileMapServerType
{
	FMT_No = 0,
	FMT_Client=0x00000001,//�ͻ���ʽ
	FMT_Server=0x00000002,//����ʽ
};


//��Ϣ���ļ��Ĳ�����ʽ
enum eFileOpType
{
	File_No=0,
	File_Open=1,//���ļ�
	File_Write=2,//д�ļ�
	File_Clear=3,//����ļ�
	File_Close=4,//�ر��ļ�
	End=0xFF,//��������,�˳��������߳�
};


class CMapFile
{
public:
	CMapFile(void);
	~CMapFile(void);

private:
	//Ӱ���������
	eFileMapServerType m_eType;
	//Ӱ�䵽��ַ�ռ���ļ����
	HANDLE	m_hFile;
	//Ӱ���ļ����
	HANDLE	m_hMapObject;
	//�ڴ�Ӱ���ļ��ĵ�ַ�ռ�
	LPVOID m_pszMapView;
	//���ڶ�д�Ļ�����
	HANDLE m_hMutex;
	//������¼����
	HANDLE m_hEvent;

	//��װ�˶�д���ݵĹ���
private:
	//���ռ��С�Ƿ�����
	bool CheckViewSpace(long lNeedSize);
	//�����´�д���ݵ�λ��
	int WriteData(int nPos,const char *pData,int nLen);
	//��Χ�´ζ����ݵ�λ��
	int ReadData(int nPos,char *pData,int nLen);	
public:
	//��ʼ��
	bool Initial(eFileMapServerType eFMType,const char* pszMapObjName,const char* pFileName=NULL);
	//��������
	bool Reset(eFileMapServerType eFMType,const char* pszMapObjName,const char* pFileName=NULL);
	//�ͷ���Դ
	void Release();
	//���ļ����б仯����
	void DoFile(eFileOpType type,const char* pszFileName);
	//���ļ�
	inline void OpenFile(const char* pszFileName){	DoFile(File_Open,pszFileName);}
	//�ر��ļ�
	inline void CloseFile(const char* pszFileName){DoFile(File_Close,pszFileName);}
	//����ļ�
	inline void ClearFile(const char* pszFileName){DoFile(File_Clear,pszFileName);}
	//д����
	void Write(const char* pszFileName,const char* pszText);
	//������
	eFileOpType Read(char* pszFileName,char* pszText);
};

#endif

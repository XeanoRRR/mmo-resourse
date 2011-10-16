#ifndef _UPDATE_FILE_H_
#define _UPDATE_FILE_H_

#pragma once

//! ���ݿ���
//#define IN_DEPTH_COPY

class CUpdateFile
{
public:
	CUpdateFile(const char *pFileName = NULL);
	~CUpdateFile();

	//! ���󿽱�
	//! ����Ч�ʿ��ǣ���ϣ���ڶ�ȡ���ļ����ݺ���п���
	//! ��˵�ǰ��ʵ�֣����m_pContent��Ա��Ϊ�գ�����ʱ���׳��쳣��Ҫȡ��������ã���Ҫ����IN_DEPTH_COPY��
	CUpdateFile(const CUpdateFile &SrcObj);
	CUpdateFile& operator=(const CUpdateFile&);

public:
	//!		��һ���ļ����ݶ��뵽�����У�������ǰ�����ݣ�
	BOOL	LoadDataFromDir(LPCSTR pFileName);
	//!		���ڴ��е�����д�뵽ָ����λ��·������ָ���ļ�����
	BOOL	WriteDataToPath(LPCSTR pDirPath) const;

	VOID	SetName(LPCSTR pName);
	LPCSTR	GetName(VOID){return m_szName;}
	char*	GetData()	 {return m_pContent;}
	DWORD	GetDataLen() {return m_dwSize;}

private:
	char				m_szName[MAX_PATH];				//�ļ���
	char				*m_pContent;					//�ļ�����
	DWORD				m_dwSize;						//�ļ���С

};
#endif//_UPDATE_FILE_H_
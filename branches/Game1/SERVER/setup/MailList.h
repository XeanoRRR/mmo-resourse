///////////////////////////////////////////////////////////////////////////////
///  MailList.h
///
///  PATH: E:\alienbrainWork\Game_Program_Develop\guangmang\SERVER\setup
///
///  CREATED: 09/19/2008  by ���Ƚ�
///
///  PURPOSE:   ϵͳ�ʼ���
//////////////////////////////////////////////////////////////////////
#ifndef _INC_MAILLIST_INCLUDED
#define _INC_MAILLIST_INCLUDED
//#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
//#endif



#include "../../public/tinyxml.h"
#include "../../public/MailDef.h"

using namespace std;
class CMailList
{
public:
	CMailList(void);
	virtual ~CMailList(void);

	static bool 							LoadSysMailList(const char *pFileName);										//����ϵͳ�ʼ��ļ�
	static bool								LoadSysMailList( TiXmlElement *pElem );										//����ϵͳ�ʼ�����
	static void								LoadSysMailList( TiXmlElement *pElem,tagMailParam *ptgMailParam);			//����ϵͳ�ʼ�����
	static void								LoadSysMailTimeConfig(TiXmlElement *pElem,struct tm *timeinfo);				//����ʱ������
	static void								LoadSysMailEventTime(TiXmlElement *pElem,tagMailEvent *ptgMailEvent);		//�����¼�������ʱ����
	static bool 							AddToByteArray(vector<BYTE>* pByteArray);									//��ӵ�CByteArray
	static bool 							DecordFromByteArray(BYTE* pByte, long& pos);								//����	
	static DWORD							GetDwordAttribute(TiXmlElement *pElem,string strName);						//��ȡDWORD	
	static string							GetStringAttribute(TiXmlElement *pElem,string strName);						//��ȡ�ַ���	
	static BYTE								GetIntAttribute(TiXmlElement *pElem,string strName);						//��ȡ����	
	static string							GetTextAttribute(TiXmlElement *pElem);										//��ȡ�ı�
	static tagMailParam*					GetSysMail(DWORD dwID);														//ͨ��ID��ȡϵͳ�ʼ�
	static map<DWORD,tagMailParam*>			GetSysMail();																//��ȡϵͳ�ʼ�
	static void								Release();																	//�ͷ�

private:	
	static map<DWORD,tagMailParam*> m_mapMail;
};
#endif//_INC_MAILLIST_INCLUDED
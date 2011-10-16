//======================================================================================
/**
*	@file		MailList.h
*	@data		09/19/2008
*	@author		���Ƚ�
*/
//======================================================================================
#pragma once

#include "../Common/MailDef.h"
/**
*	@class MailList
*
*	@brief ϵͳ�ʼ�������
*/
class MailList
{
public:
	/// ����
	MailList(void);

	/// ����
	~MailList(void);

	/// ���ļ�����ͳ�ʼ�����
	static bool LoadSysMailList(const char* pFileName);

	/// ����
	static bool AddToByteArray(vector<uchar>* pByteArray);

	/// ����	
	static bool DecordFromByteArray(uchar* pByte, long& pos);

	/// ͨ��ID��ȡϵͳ�ʼ�
	static tagMailParam* GetSysMail(DWORD dwID);

	/// ��ȡϵͳ�ʼ�
	static map<DWORD,tagMailParam*> GetSysMail();

	/// �ͷ�
	static void Release();
private:									
	/// ����ϵͳ�ʼ�����
	static bool	LoadSysMailList(TiXmlElement* pElem);

	/// ����ϵͳ�ʼ�����
	static void LoadSysMailList(TiXmlElement* pElem,tagMailParam* ptgMailParam);

	/// ����ʱ������
	static void LoadSysMailTimeConfig(TiXmlElement* pElem,struct tm* timeinfo);

	/// �����¼�������ʱ����
	static void LoadSysMailEventTime(TiXmlElement* pElem,tagMailEvent* ptgMailEvent);

	/// ��ȡDWORD	
	static DWORD GetDwordAttribute(TiXmlElement* pElem,string strName);

	/// ��ȡ�ַ���	
	static string GetStringAttribute(TiXmlElement* pElem,string strName);

	/// ��ȡ����	
	static uchar GetIntAttribute(TiXmlElement* pElem,string strName);

	/// ��ȡ�ı�
	static string GetTextAttribute(TiXmlElement* pElem);
		
private:
	/// ϵͳ�ʼ��ṹ
	static map<DWORD,tagMailParam*> m_mapMail;
};

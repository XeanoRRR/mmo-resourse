



#pragma once

#define  ALWAYS_OUT_TO_INTERFACE

//! ִ�г����ṩ�ķ�����
class LogTools
{
public:
	//!						�ڱ�����־�����һ����Ϣ
	static		VOID		OutLog(const char *pText, ...);
	//!						�ڱ��ش�����־�����һ����Ϣ
	static		VOID		OutErrLog(const char *pText, ...);
	//!						�ڱ�����־�ͼ��ӿ������һ����Ϣ
	static		VOID		OutLogAndTxt(const char *pText, ...);

public:
	//!						��Ϣ�Ի���
	static		VOID		MsgboxInfo(const char *pText, ...);
	//!						������Ϣ�Ի���
	static		VOID		MsgboxErrInfo(const char *pText, ...);
	//!						ѡ��Ի���
	static		BOOL		MsgboxYesNo(const char *pText, ...);

	//!						�ڽ����������Ϣ
	//!						����������������õ�ģ����ʵ��
	static		VOID		OutTextToInterface(const char *pText);
	
	static		void		OutToFile(bool clear,const char* filename,const char*pText,...);
private:
	LogTools(VOID);
	~LogTools(VOID);		
};
/**************************************************
*		�ļ���  ��Log4c.h
*		��Ҫ��������log4cplus��װ����־�ļ�

*		��ǰ�汾��1.0
*		����    ��ʯϲ
*		������ڣ�2010-11-10
*		�޶�˵����
****************************************************/

#pragma once

#ifndef __LOG4C_20101110_H__
#define __LOG4C_20101110_H__

//////////////////////////////////////////////////////////////////////////////////////////
/*
*@brief The wrapper class for the log4cplus 
*/
class Log4c 
{
public:
	/*
	*@ init
	*/
	static void Init(const char *filename);
	/*
	*@ register
	*/
	static void Register(int module,const char *filename);
	/*
	*@brief FATAL level
	*/
	static void Fatal(int module,const char * pFormat, ...);

	/*
	*@brief ERROR level
	*/
	static void Error(int module,const char * pFormat, ...);

	/*
	*@brief WARN level
	*/
	static void Warn(int module,const char * pFormat, ...);

	/*
	*@brief INFO level
	*/
	static void Info(int module,const char * pFormat, ...);

	/*
	*@brief DEBUG level
	*/
	static void Debug(int module,const char * pFormat, ...);

	/*
	*@brief TRACE level
	*/
	static void Trace(int module,const char * pFormat, ...);

private:
	Log4c( );
};



#endif




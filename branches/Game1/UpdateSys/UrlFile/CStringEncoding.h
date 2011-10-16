/*
 * CStringEncoding.h
 * ����: �ַ�������ת��
 * ժҪ: ʹ��windows API���ݴ���ҳ�Զ��ֽںͿ��ֽ��ַ������໥ת��,�Ը�������ҵ����ַ����������
 *		 Ŀǰ֧�ֱ����ʽ : unicode,utf8,ansi
 * ����: lpf
 * ��������: 2010-06-04
 */

#pragma once
using namespace std;

class CStringEncoding
{
public:
	static UINT	GetCodePageFromCharset(UINT uiCharSet);										//�õ�����ҳ���

	static wstring ANSIToUnicode(const string & s, UINT uiCharSet = GB2312_CHARSET);		//ANSI���뵽Unicode�����ת��
	static string  UnicodeToANSI(const wstring & ws, UINT uiCharSet = GB2312_CHARSET);		//Unicode���뵽ANSI�����ת��
	static wstring UTF8ToUnicode(const string & s, UINT uiCharSet = GB2312_CHARSET);		//UTF8��Unicode�����ת��
	static string  UnicodeToUTF8(const wstring & ws, UINT uiCharSet = GB2312_CHARSET);		//Unicode��UTF8�����ת��
	static string  UTF8ToANSI(const string & s, UINT uiCharSet = GB2312_CHARSET);			//UTF8��ANSI�����ת��

	static wchar_t * ANSIToUnicode(const char * psz, UINT uiCharSet = GB2312_CHARSET);		//ANSI���뵽Unicode�����ת��
	static char	   * UnicodeToANSI(const wchar_t * pwsz, UINT uiCharSet = GB2312_CHARSET);	//Unicode���뵽ANSI�����ת��
	static wchar_t * UTF8ToUnicode(const char * psz, UINT uiCharSet = GB2312_CHARSET);		//UTF8��Unicode�����ת��
	static char	   * UnicodeToUTF8(const wchar_t * pwsz, UINT uiCharSet = GB2312_CHARSET);	//Unicode��UTF8�����ת��
	static char	   * UTF8ToANSI(const char * psz, UINT uiCharSet = GB2312_CHARSET);			//UTF8��ANSI�����ת��
};
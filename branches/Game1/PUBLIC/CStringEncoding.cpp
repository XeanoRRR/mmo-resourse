#include "stdafx.h"
#include "CStringEncoding.h"

/*
 * ����: �õ�����ҳ���
 * ժҪ: -
 * ����: uiCharSet - ʹ���ַ���
 * ����ֵ: ����ת�����Ӧ�Ĵ���ҳ
 * ����: lpf
 * ��������: 2010.06.04
 * �޸���־:
 */
UINT CStringEncoding::GetCodePageFromCharset(UINT uiCharSet)
{
	switch (uiCharSet)
	{
	case SHIFTJIS_CHARSET:
		return 932;
	case HANGUL_CHARSET:
		return 949;
	case GB2312_CHARSET:
		return 936;
	case CHINESEBIG5_CHARSET:
		return 950;
	case GREEK_CHARSET:
		return 1253;
	case TURKISH_CHARSET:
		return 1254;
	case HEBREW_CHARSET:
		return 1255;
	case ARABIC_CHARSET:
		return 1256;
	case BALTIC_CHARSET:
		return 1257;
	case THAI_CHARSET:
		return 874;
	case EASTEUROPE_CHARSET:
		return 1250;
	default:
		return 1252;
	}
}



/*
 * ����: ANSI���뵽Unicode�����ת��
 * ժҪ: -
 * ����: s - ��ת����ANSI�ַ���
 * ����ֵ: ����ת����ɵĿ��ֽ��ַ���
 * ����: lpf
 * ��������: 2010.06.04
 * �޸���־:
 */
wstring CStringEncoding::ANSIToUnicode(const string & s, UINT uiCharSet)
{
	int iLen = (int)s.length();
	if (iLen <= 0)
		return L"";

	int		  iUnicodeLen = MultiByteToWideChar(GetCodePageFromCharset(uiCharSet), 0, s.c_str(), -1, NULL, 0);
	wchar_t * pUnicode	  = new wchar_t[iUnicodeLen + 1];
	ZeroMemory(pUnicode, (iUnicodeLen + 1) * sizeof(wchar_t));

	MultiByteToWideChar(GetCodePageFromCharset(uiCharSet), 0, s.c_str(), -1, (LPWSTR)pUnicode, iUnicodeLen);
	wstring ws(pUnicode);
	delete [] pUnicode;

	return ws;
}



/*
 * ����: Unicode���뵽ANSI�����ת��
 * ժҪ: -
 * ����: ws - ��ת����Unicode�ַ���
 * ����ֵ: ����ת����ɵĶ��ֽ��ַ���
 * ����: lpf
 * ��������: 2010.06.04
 * �޸���־:
 */
string CStringEncoding::UnicodeToANSI(const wstring & ws, UINT uiCharSet)
{
	int iANSILen = WideCharToMultiByte(GetCodePageFromCharset(uiCharSet), 0, ws.c_str(), -1, NULL, 0, NULL, FALSE);
	if (iANSILen <= 0)
		return "";

	char * psz = new char[iANSILen + 1];
	ZeroMemory(psz, (iANSILen + 1) * sizeof(char));

	WideCharToMultiByte(GetCodePageFromCharset(uiCharSet), 0, ws.c_str(), -1, psz, iANSILen, NULL, FALSE);
	string s(psz);
	delete [] psz;

	return s;
}



/*
 * ����: UTF8��Unicode�����ת��
 * ժҪ: -
 * ����: s - ��ת����utf8�ַ���
 * ����ֵ: ����ת����ɵĿ��ֽ��ַ���
 * ����: lpf
 * ��������: 2010.06.04
 * �޸���־:
 */
wstring CStringEncoding::UTF8ToUnicode(const string & s, UINT uiCharSet)
{
	int iLen = (int)s.length();
	if (iLen <= 0)
		return L"";

	int iUnicodeLen	   = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
	wchar_t * pUnicode = new wchar_t[iUnicodeLen + 1];
	ZeroMemory(pUnicode, (iUnicodeLen + 1) * sizeof(wchar_t));

	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, (LPWSTR)pUnicode, iUnicodeLen);
	wstring ws(pUnicode);
	delete [] pUnicode;

	return ws;
}



/*
 * ����: Unicode��UTF8�����ת��
 * ժҪ: -
 * ����: ws - ��ת����Unicode�ַ���
 * ����ֵ: ����ת����ɵĶ��ֽ��ַ���
 * ����: lpf
 * ��������: 2010.06.04
 * �޸���־:
 */
string CStringEncoding::UnicodeToUTF8(const wstring & ws, UINT uiCharSet)
{
	int iANSILen = WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, NULL, 0, NULL, FALSE);
	if (iANSILen <= 0)
		return "";

	char * psz = new char[iANSILen + 1];
	ZeroMemory(psz, (iANSILen + 1) * sizeof(char));

	WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, psz, iANSILen, NULL, FALSE);
	string s(psz);
	delete [] psz;

	return s;
}



/*
 * ����: UTF8��ANSI�����ת��
 * ժҪ: -
 * ����: s - ��ת���Ķ��ֽ��ַ���
 * ����ֵ: ����ת����ɵĶ��ֽ��ַ���
 * ����: lpf
 * ��������: 2010.06.07
 * �޸���־:
 */
string CStringEncoding::UTF8ToANSI(const string & s, UINT uiCharSet)
{
	wstring ws = UTF8ToUnicode(s, uiCharSet);
	return UnicodeToANSI(ws, uiCharSet);
}



/*
 * ����: ANSI���뵽Unicode�����ת��
 * ժҪ: -
 * ����: psz - ��ת����ANSI�ַ���
 * ����ֵ: ����ת����ɵĿ��ֽ��ַ���
 * ����: lpf
 * ��������: 2010.06.04
 * �޸���־:
 */
wchar_t * CStringEncoding::ANSIToUnicode(const char * psz, UINT uiCharSet)
{
	int iLen = (int)strlen(psz);
	if (iLen <= 0)
		return NULL;

	int		  iUnicodeLen = MultiByteToWideChar(GetCodePageFromCharset(uiCharSet), 0, psz, -1, NULL, 0);
	wchar_t * pUnicode	  = new wchar_t[iUnicodeLen + 1];
	ZeroMemory(pUnicode, (iUnicodeLen + 1) * sizeof(wchar_t));

	MultiByteToWideChar(GetCodePageFromCharset(uiCharSet), 0, psz, -1, (LPWSTR)pUnicode, iUnicodeLen);
	return pUnicode;
}



/*
 * ����: Unicode���뵽ANSI�����ת��
 * ժҪ: -
 * ����: pwsz - ��ת����Unicode�ַ���
 * ����ֵ: ����ת����ɵĶ��ֽ��ַ���
 * ����: lpf
 * ��������: 2010.06.04
 * �޸���־:
 */
char * CStringEncoding::UnicodeToANSI(const wchar_t * pwsz, UINT uiCharSet)
{
	int iANSILen = WideCharToMultiByte(GetCodePageFromCharset(uiCharSet), 0, pwsz, -1, NULL, 0, NULL, FALSE);
	if (iANSILen <= 0)
		return NULL;

	char * psz = new char[iANSILen + 1];
	ZeroMemory(psz, (iANSILen + 1) * sizeof(char));

	WideCharToMultiByte(GetCodePageFromCharset(uiCharSet), 0, pwsz, -1, psz, iANSILen, NULL, FALSE);
	return psz;
}



/*
 * ����: UTF8��Unicode�����ת��
 * ժҪ: -
 * ����: psz - ��ת����utf8�ַ���
 * ����ֵ: ����ת����ɵĿ��ֽ��ַ���
 * ����: lpf
 * ��������: 2010.06.07
 * �޸���־:
 */
wchar_t * CStringEncoding::UTF8ToUnicode(const char * psz, UINT uiCharSet)
{
	int iLen = (int)strlen(psz);
	if (iLen <= 0)
		return NULL;

	int iUnicodeLen	   = MultiByteToWideChar(CP_UTF8, 0, psz, -1, NULL, 0);
	wchar_t * pUnicode = new wchar_t[iUnicodeLen + 1];
	ZeroMemory(pUnicode, (iUnicodeLen + 1) * sizeof(wchar_t));

	MultiByteToWideChar(CP_UTF8, 0, psz, -1, (LPWSTR)pUnicode, iUnicodeLen);

	return pUnicode;
}



/*
 * ����: Unicode��UTF8�����ת��
 * ժҪ: -
 * ����: ws - ��ת����Unicode�ַ���
 * ����ֵ: ����ת����ɵĶ��ֽ��ַ���
 * ����: lpf
 * ��������: 2010.06.07
 * �޸���־:
 */
char * CStringEncoding::UnicodeToUTF8(const wchar_t * pwsz, UINT uiCharSet)
{
	int iANSILen = WideCharToMultiByte(CP_UTF8, 0, pwsz, -1, NULL, 0, NULL, FALSE);
	if (iANSILen <= 0)
		return NULL;

	char * psz = new char[iANSILen + 1];
	ZeroMemory(psz, (iANSILen + 1) * sizeof(char));

	WideCharToMultiByte(CP_UTF8, 0, pwsz, -1, psz, iANSILen, NULL, FALSE);

	return psz;
}



/*
 * ����: UTF8��ANSI�����ת��
 * ժҪ: -
 * ����: psz - ��ת���Ķ��ֽ��ַ���
 * ����ֵ: ����ת����ɵĶ��ֽ��ַ���
 * ����: lpf
 * ��������: 2010.06.07
 * �޸���־:
 */
char * CStringEncoding::UTF8ToANSI(const char * psz, UINT uiCharSet)
{
	if ((int)strlen(psz) <= 0)
		return NULL;

	wchar_t * pwsz	 = UTF8ToUnicode(psz, uiCharSet);
	char	* pszOut = UnicodeToANSI(pwsz, uiCharSet);
	delete [] pwsz;
	return pszOut;
}



/*
 * ����: ����ַ�
 * ժҪ: -
 * ����: psz	   - ��ת���Ķ��ֽ��ַ���
 *		 uiCharSet - ���ʱ���ַ���
 * ����ֵ: ����ַ����������ַ�,����true,���򷵻�false
 * ����: lpf
 * ��������: 2010.09.16
 * �޸���־:
 */
bool CStringEncoding::CheckCharacter(const char * psz, UINT uiCharSet)
{
	if (psz == NULL || strlen(psz) == 0)
		return false;

	BYTE cW = psz[0];
	BYTE cQ = psz[1];

	// �ж��ַ���
	if (uiCharSet == GB2312_CHARSET)
	{
		if (cQ < 0xa1 || cQ > 0xfe)
			return false;
		else
		{
			if (cW >= 0xa1 && cW < 0xaa)
				return true;
			else if (cW >= 0xb0 && cW < 0xf8)
				return true;
			else		//��Ч������
				return false;
		}
	}else if (uiCharSet == CHINESEBIG5_CHARSET)
	{
		if (cQ < 0x40 || cQ > 0xfe)
			return false;
		else
		{
			if (cW >= 0xa1 && cW <= 0xc6)		//�ж���λ
				return true;
			else if (cW >= 0xc9 && cW <= 0xe0)
				return true;
			else if (cW >= 0xe2 && cW <= 0xf4)
				return true;
			else if (cW >= 0xf7 && cW <= 0xf9)
				return true;
			else	// ��Ч�ķ���
				return false;
		}
	}else if (uiCharSet == HANGUL_CHARSET)
	{
		if (cQ < 0xa1 || cQ > 0xfe)
			return false;
		else if (cW >= 0xa1 && cW <= 0xc8)		//�ж���λ
			return true;
		else
			return false;
	}else if (uiCharSet == SHIFTJIS_CHARSET)
	{
		if (cQ < 0x40 || cQ > 0xfc)
			return false;
		else
		{
			if (cW >= 0x81 && cW <= 0x9f)		//�ж���λ
				return true;
			else if (cW >= 0xe0 && cW <= 0xfc)
				return true;
			else
				return false;
		}
	}

	return false;
}
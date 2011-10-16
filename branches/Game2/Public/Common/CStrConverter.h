/**
 *  CStrConverter.h
 *  ʵ�ֶ���	Class CStrConverter
 *  �������ڣ�	06-9��-2006 10:37:37
 *  ԭʼ���ߣ�	���Ƚ�
 *
 *  Modify By:  Expter [1/25/ 2010 ] 
 */

#ifndef _CSTRING_CONVERTER
#define _CSTRING_CONVERTER

/// 
/// �ַ���ת���࣬�����ַ���ת��Ϊ�ض�������
/// 
class CStrConverter
{		
public:
	/// ����ʮ�������ַ���ת��Ϊlong
	static long parseStringToDec(const char* val);

	/// �����ַ���ת��Ϊint
	static int  parseInt(const char* val);

	/// �����ַ���ת��Ϊbool
	static bool parseBool(const char* val);

	/// ���ַ���ת��Ϊlong
	static long parseLong(const char* val);

	/// ���ַ���ת��Ϊfloat
	static float parseFloat(const char* val);

	/// �����з�ʽ�ַ���ת��Ϊlong
	static long parseRankStyle(const char* val);

	/// ��Boolת��Ϊ�ַ���
	static const char* ParseBoolToString(const bool b);

    ///
	/// �����й����ĵ��ַ����ض�,iSplitPosָ��λ��[1,val.length()-1],�������䣬���ֱ�ӷ���ԭ�ַ������ұ߷���"",
    /// SplitStrRight����ָ��λ���ұߵ��ַ���,��������ָ��λ����ߵ��ַ���
    ///
	static string GetLeftSplitStr(const std::string& val,size_t SplitPos,string& SplitStrRight);

    /// ��ȡ1,2,3,4,5�����ַ��������֣����浽lTemp
	static void   parseList(char* val,list<long>& lTemp);

private:

    ///�ж��Ƿ������ָ����λ�ýض��ַ���
	static bool CanSplit(const std::string& str,size_t pos);
};
#endif
///////////////////////////////////////////////////////////
//  CStrConverter.h
//  ʵ�ֶ���	Class CStrConverter
//  �������ڣ�	06-9��-2006 10:37:37
//  ԭʼ���ߣ�	���Ƚ�
///////////////////////////////////////////////////////////
#ifndef _CSTRING_CONVERTER
#define _CSTRING_CONVERTER
#include <String>
//#include "../server/GameServer/AppServer/states/AttackPower.h"



/// <summary>
/// �ַ���ת���࣬�����ַ���ת��Ϊ�ض�������
/// </summary>
class CStrConverter
{		
public:
	//����ʮ�������ַ���ת��Ϊlong
	static long parseStringToDec(const char *val);
	//�����ַ���ת��Ϊint
	static int  parseInt(const char *val);
	//�����ַ���ת��Ϊbool
	static bool parseBool(const char *val);
	//���ַ���ת��Ϊlong
	static long parseLong(const char *val);
	//���ַ���ת��Ϊfloat
	static float parseFloat(const char *val);
	//�����з�ʽ�ַ���ת��Ϊlong
	static long parseRankStyle(const char *val);
	//����ʽת��Ϊlong
	static long parseStyle(const char *val);
	/*  
	 *	����:zhaohang(kaba)
	 *  ��������: 2009/7/24
	 */ 
	//��Boolת��Ϊ�ַ���
	static const char* ParseBoolToString(const bool b);
	//zhahang 2009/11/18
	//�����й����ĵ��ַ����ض�,iSplitPosָ��λ��[1,val.length()-1],�������䣬���ֱ�ӷ���ԭ�ַ������ұ߷���"",SplitStrRight����ָ��λ���ұߵ��ַ���,��������ָ��λ����ߵ��ַ���
	static string GetLeftSplitStr(const std::string &val,size_t SplitPos,string &SplitStrRight);
	static void parseList(char *val,list<long> &lTemp);
	//����ʽת��ΪAttackType
	//static AttackType parseAttackType(const char *val);
	//
private:
	static bool CanSplit(const std::string &str,size_t pos);//�ж��Ƿ������ָ����λ�ýض��ַ���
};
#endif//_STRING_CONVERTER
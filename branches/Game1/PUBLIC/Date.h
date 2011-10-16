////////////////////////////////////////////////////////////////////////////////
//Date.h/////////////////////////////////////////////////////////////////////
//������:
//Author:Joe
//Create Time:2004.7.28
#pragma once

#include <time.h>

//ʱ��ṹ
struct tagTime
{
	//ϵͳʱ��
	tm tmTime;

	tagTime(void);
	tagTime(SYSTEMTIME sysTime);
	tagTime(const char* pszDate);

	//ת����SYSTEMTIME��ʽ
	void ToSystemTime(SYSTEMTIME& sysTime);
	//�õ����ڸ�ʽ�����ַ���
	const char* GetDateFormatStr();
	//�õ�ʱ���ʽ�����ַ���
	const char* GetTimeFormatStr();

	//�õ�ֵ�ַ���
	const char* GetValueFormatStr();
	//����ʱ���(��ֵ)
	long	GetSecondDiff(tagTime FutureTime);
	//�õ����뵱ǰʱ��Ĳ�(���ظ�ʽΪn��nСʱn����n����)
	tagTime GetTimeDiff(tagTime FutureTime);
	//�ж�����ʱ���Ƿ���ȣ���ȷ����
	bool operator==(const tagTime& Time) const;
	bool operator> (const tagTime& Time) const;
	bool operator>=(const tagTime& Time) const;
	bool operator<(const tagTime& Time) const;
	bool operator<=(const tagTime& Time) const;
	tagTime& AddSecond(long lSecond);
	tagTime& AddMinute(long lMinute);
	tagTime& AddHour(long lHour);
	tagTime& AddDay(long lDay);

	//ʱ���ֵ
	long Year();
	long Month();
	long Day();
	long Hour();
	long Minute();
	long Second();
	long Week();

	static bool IsLeap(long lYear)
	{return lYear%4 == 0 && lYear%100 != 0 || lYear%400 == 0;}

};

//���뵱ǰ��ʱ���
long DiffSecond(tagTime& Time1,const tagTime& Time2);
tagTime DiffTime(tagTime& Time1,const tagTime& Time2);

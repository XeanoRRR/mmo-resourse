#pragma once

#include "tinystr.h"
#include "tinyxml.h"
#include <map>
#include <vector>
const char MAXAREANAMESIZE=32;
const char MAXCREDITNAMESIZE=16;
typedef struct  
{
	int  _iIndex;//���������ڷ���������ͨ����������������	
	DWORD _dwAreaID;//����ID
	char _cName[MAXAREANAMESIZE];//��������
}AREA;

//������������
typedef struct  
{
	DWORD _dwValue;//����ֵ
	DWORD _dwLevelID;//�ȼ�
	char _cName[MAXCREDITNAMESIZE];//����
}CREDITLEVEL;
	
//������֯����
typedef struct  
{
	DWORD _dwOrganizeID;//��֯ID
	char _cDirtyName[MAXAREANAMESIZE];//����������е���BatakCredit�ֶ�����
	char _cName[MAXAREANAMESIZE];//��������
	vector<CREDITLEVEL> _vecLevel;//�ȼ��б�
}ORGANCREDIT;

class CCreditSystem
{
public:
	CCreditSystem(void);
	~CCreditSystem(void);
	bool LoadAreaCredit();
	void Release(){m_mapTradeAreas.clear();m_vecCreditLevel.clear();}
	typedef map<DWORD,ORGANCREDIT>::iterator ORGANITR;
	typedef vector<CREDITLEVEL>::iterator	CREDITITR;
	long CreditLevel(DWORD dwValue);//��������ֵ��ȡ���������ȼ�
	const char* CreditName(DWORD dwValue);//��������ֵ��ȡ������������
	long MaxAreaCreditLevel();//��ߵȼ�����
	long MaxAreaCredit();//��ߵȼ�����
	//��������
	bool CodeToByteArray(DBWriteSet& dbWriteSet);
	bool DecodeToByteArray(DBReadSet& dbReadSet);
	const char* GetDirtyName(DWORD dwOrganID);		 //��ȡ�������ֶ���
	long MaxOrganValByDirtyName(const char* strDirtyName);//ָ����֯����ߵȼ�����
	long CreditOrganLeveyByDirtyName(const char* strDirtyName,DWORD dwValue);//ͨ�����������ֵ���֯�ȼ�
	
	//�ͻ�����
	long CreditOrganLevelByName(const char* strName,DWORD dwValue);//ͨ���������ֵ���֯�ȼ�
	long MaxAreaValue(DWORD dwLevel);//ָ���ȼ��������������
	long MaxOrganValue(const char* strName);//ָ����֯����ߵȼ�����
	const char* CreditOrganName(DWORD dwOrganID,DWORD dwValue);//��ȡָ����֯��ָ������ֵ ����Ӧ������
	long CreditOrganLevel(DWORD dwOrganID,DWORD dwValue);//��ȡָ����֯��ָ������ֵ ����Ӧ�ĵȼ�
	long MaxOrganValue(DWORD dwOrganID,DWORD dwLevel);//ָ����ָ֯���ȼ����������
	const char* CreditAreaNameByIndex(int index);         //ͨ����������������
	const char* CreditAreaNameByID(DWORD dwID);           //ͨ����ͼid����������
	const char* GetOrganName(DWORD dwOrganID);			  //��ȡ��������
	int GetCreditAreaSize();                              //����������������
	int GetCreditOrganSize();                              //���������֯������
	long  GetMaxCreditByOrgan(DWORD dwOrganID);             //���һ����֯���������ֵ
	long  GetMaxCreditLevelByOrgan(DWORD dwOrganID);             //���һ����֯����������ȼ�


private:
	bool LoadOrganizeCredit(TiXmlElement* p);//������֯����
	bool LoadCreditLevel(TiXmlElement* p,vector<CREDITLEVEL>& vecCreditLevel);
	CREDITLEVEL* GetLevel(DWORD dwValue,vector<CREDITLEVEL>& vecCreditLevel);
	CREDITLEVEL* GetOrganLevel(DWORD dwOrganID,DWORD dwLevel);
	long GetOrganID(const char* Name);//��ȡ��֯ID
	long GetOrganIDByDirtyName(const char* DirtyName);
	map<DWORD,AREA>m_mapTradeAreas;
	vector<CREDITLEVEL>m_vecCreditLevel;
	map<DWORD,ORGANCREDIT>m_mapOrganCredit;
};

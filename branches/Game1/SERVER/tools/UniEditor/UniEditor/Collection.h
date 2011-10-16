#pragma once
#include "info.h"


//�ɼ��������Ϣ
class CCollectionInfo 
{
public:
	CCollectionInfo(void);
	~CCollectionInfo(void);
	friend ifstream &operator>>(ifstream &rstream, CCollectionInfo &rCollectionInfo); 

	friend ofstream &operator<<(ofstream &rstream, CCollectionInfo &rCollectionInfo);
	const inline string& GetName()const						{return m_strCollName;}
	const inline unsigned GetProbability()const				{return m_nProbability;}
	void CollInfo2String(CString &str);
	inline void SetProbability(const unsigned uProbability)	{m_nProbability = uProbability;}
	inline void SetName(const string &strName)				{m_strCollName = strName;}	

	//{
	//	m_strCollName=strName;
	//	/*int i;
	//	char strVal[100];
	//	for( i=0;strName[i]!=0;i++)
	//		strVal[i]=strName[i];
	//	strVal[i]='\0';
	//	string str1(strVal);
	//	m_strCollName=str1;*/
	//}
protected:
	string m_strCollName;//�ɼ�������
	int m_nProbability;//�ɼ��Ｘ��
};
typedef vector<CCollectionInfo *>		VECCOLLECTIONINFO;
typedef VECCOLLECTIONINFO::iterator		COLLECTIONITR;



class CCollection:public CInfo
{
public:
	CCollection(void);
	CCollection(const long x, const long y);
	CCollection(CCollection &rCollection);
	friend ifstream & operator>>(ifstream &rstream, CCollection &rcollection);
	friend ofstream & operator<<(ofstream &rstream, CCollection &rcollection);
	void Collection2String(CString &str);
	~CCollection(void);
	inline VECCOLLECTIONINFO &GetCollectionInfo()						{return m_vecCollectionInfo;}
	const inline unsigned GetTimeBorn()const							{return m_nTime;}
	const inline unsigned GetTimeStart()const							{return m_nStartTime;}
	inline void SetTimeBorn(const unsigned utime)			{m_nTime = utime;}
	inline void SetTimeStart(const unsigned utime)			{m_nStartTime = utime;}
	
	
	VECCOLLECTIONINFO m_vecCollectionInfo;//ĳ������ɼ���ľ�����Ϣ
	int m_nTime;//ʱ��
	int m_nStartTime;//��ʼʱ��
	
};

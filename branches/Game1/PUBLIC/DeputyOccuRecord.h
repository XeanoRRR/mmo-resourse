//		�ļ���		DeputyOccuRecord.h
//		����		heliangchen
//		ʱ��		2008/2/1
//		���		��ְҵ�ļ�¼������ְҵ�ľ��飬�ȼ����������б�
#pragma once
#include <set>
using namespace std;

class CDeputyOccuRecord
{
public:
	CDeputyOccuRecord();
	~CDeputyOccuRecord();
	//ѹ��ְҵ����
	BOOL AddDeputyOccuDataByteArray(vector<BYTE>* pByteArray);
	//Ϊ�ͻ���ѹ����
	BOOL AddDeputyOccuDataForClient(vector<BYTE>* pByteArray);
	//�⸱ְҵ����
	BOOL DecordDeputyOccuDataFromByteArray(BYTE* pByte, long& pos);
	//Ϊ�ͻ��˽�����
	BOOL DecordDeputyOccuDataForClient(BYTE* pByte, long& pos);

	BOOL CodeToDataBlock(DBWriteSet& setWriteDB);
	BOOL CodeToDataBlockForClient(DBWriteSet& setWriteDB);
	BOOL DecodeFromDataBlock(DBReadSet& setReadDB);
public:
	////ȡ�òɼ�����
	//LONG GetCollectionExp(){return m_lCollectionExp;}
	////���òɼ�����
	//VOID SetCollectionExp(LONG l){m_lCollectionExp=l;}
	////ȡ�òɼ��ȼ�
	//LONG GetCollectionGrade(){return m_lCollectionGrade;}
	////���òɼ��ȼ�
	//VOID SetCollectionGrade(LONG l){m_lCollectionGrade=l;}
	////ȡ����������
	//LONG GetFactureExp(){return m_lFactureExp;}
	////������������
	//VOID SetFactureExp(LONG l){m_lFactureExp=l;}
	////ȡ�������ȼ�
	//LONG GetFactureGrade(){return m_lFactureGrade;}
	////���������ȼ�
	//VOID SetFactureGrade(LONG l){m_lFactureGrade=l;}
	//ȡ���ۼƳɹ���
	LONG GetFactureSuccRatio(){return m_lFectureSuccRatio;}
	//�����ۼƳɹ���
	VOID AddFactureSuccRatio(LONG l){m_lFectureSuccRatio+=l;}
	//����ۼƳɹ���
	VOID ClearFactureSuccRatio(){m_lFectureSuccRatio= 0;}
	//���������Ŀ
	BOOL AddFacture(DWORD l);
	//Ѱ��������Ŀ
	BOOL FindFacture(DWORD l);
	//ɾ��������Ŀ
	BOOL DeleteFacture(DWORD l);
	//���������Ŀ
	BOOL CleanFacture();
	//���������þ����,�������ļ�����
	void ModifyFactureExp(long& level,long& lExp);
	void ModifyCollectionExp(long& level,long& lExp);
public:
	LONG m_lCollectionExp;			//�ɼ�����
	LONG m_lCollectionGrade;		//�ɼ��ȼ�
	LONG m_lFactureExp;				//��������
	LONG m_lFactureGrade;			//�����ȼ�
	LONG m_lFectureSuccRatio;		//�ۼƳɹ���
	set<DWORD> m_setFactureList;	//�����б�
private:
};

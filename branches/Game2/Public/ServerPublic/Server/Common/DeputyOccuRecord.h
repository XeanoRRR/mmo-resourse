//		�ļ���		DeputyOccuRecord.h
//		����		heliangchen
//		ʱ��		2008/2/1
//		���		��ְҵ�ļ�¼������ְҵ�ľ��飬�ȼ����������б�

#pragma once
#include "../../../Common/DeputyOccu.h"

class CDeputyOccuRecord
{
public:
	CDeputyOccuRecord();
	~CDeputyOccuRecord();
	//ѹ��ְҵ����

	bool AddDeputyOccuDataByteArray(vector<uchar>* pByteArray);
	//Ϊ�ͻ���ѹ����
	bool AddDeputyOccuDataForClient(vector<uchar>* pByteArray);
	//�⸱ְҵ����
	bool DecordDeputyOccuDataFromByteArray(uchar* pByte, long& pos);
	//Ϊ�ͻ��˽�����
	bool DecordDeputyOccuDataForClient(uchar* pByte, long& pos);

	bool CodeToDataBlock(DBWriteSet& setWriteDB);
	bool CodeToDataBlockForClient(DBWriteSet& setWriteDB);
	bool DecodeFromDataBlock(DBReadSet& setReadDB);

public:
	
    ////ȡ�òɼ�����
	//long GetCollectionExp(){return m_lCollectionExp;}
	////���òɼ�����
	//void SetCollectionExp(long l){m_lCollectionExp=l;}
	////ȡ�òɼ��ȼ�
	//long GetCollectionGrade(){return m_lCollectionGrade;}
	////���òɼ��ȼ�
	//void SetCollectionGrade(long l){m_lCollectionGrade=l;}
	////ȡ����������
	//long GetFactureExp(){return m_lFactureExp;}
	////������������
	//void SetFactureExp(long l){m_lFactureExp=l;}
	////ȡ�������ȼ�
	//long GetFactureGrade(){return m_lFactureGrade;}
	////���������ȼ�
	//void SetFactureGrade(long l){m_lFactureGrade=l;}

	//ȡ���ۼƳɹ���
	long GetFactureSuccRatio(){return m_lFectureSuccRatio;}
	//�����ۼƳɹ���
	void AddFactureSuccRatio(long l){m_lFectureSuccRatio+=l;}
	//����ۼƳɹ���
	void ClearFactureSuccRatio(){m_lFectureSuccRatio= 0;}
	//���������Ŀ
	bool AddFacture(ulong l);
	//Ѱ��������Ŀ
	bool FindFacture(ulong l);
	//ɾ��������Ŀ
	bool DeleteFacture(ulong l);
	//���������Ŀ
	bool CleanFacture();
	//���������þ����,�������ļ�����
	void ModifyFactureExp(long& level,long& lExp);
	void ModifyCollectionExp(long& level,long& lExp);

public:
	long m_lCollectionExp;			//�ɼ�����
	long m_lCollectionGrade;		//�ɼ��ȼ�
	long m_lFactureExp;				//��������
	long m_lFactureGrade;			//�����ȼ�
	long m_lFectureSuccRatio;		//�ۼƳɹ���
	set<ulong> m_setFactureList;	//�����б�
private:
};

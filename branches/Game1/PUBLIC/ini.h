//********************************************
//	Ini ��غ���
//********************************************

#pragma once

// ���󷵻�ֵ�����ReadIntû�з��������򷵻ظ�ֵ
#define ERROR_DATA -99999999

//�����ļ���
class CIni
{
///////////////////////////////////////////////////////////////////////
//	�ⲿ�ӿ�
///////////////////////////////////////////////////////////////////////
public:
	CIni();
	CIni(const char* filename, const char* pData=NULL);		//��ʼ���������ļ�
	virtual ~CIni();						//�ͷ��ڴ�

	char *GetData();						//�����ļ�����
	int GetLines(int);						//�����ļ�������

	bool Open(const char* filename, const char* pData=NULL);	//�������ļ�
	void Close();							//�ر������ļ�
	bool Save(const char *filename=NULL);	//���������ļ�

public:
	int ReadInt(char*, char*);				//��һ������
	int ReadInt(char*, int );				//��ָ�����ж�һ����

	char *ReadText(char*, char*);			//��һ���ַ���
	char *ReadText(char*, int);				//��ָ�����ж�һ�ַ���

	char *ReadCaption(char*, int);			//��ָ���ж�һ�ַ�����( '=' ǰ�������)

	bool Write(char*, char*, int);			//д��һ������
	bool Write(char*, char*, char*);		//д��һ���ַ���

	int GetContinueDataNum(char* index);	//������������������INDEX����һ�����У�
	int GotoLastLine(char* index);			//��ָ���ƶ�����INDEX�����һ��

///////////////////////////////////////////////////////////////////////
//	�ڲ�ʵ��
///////////////////////////////////////////////////////////////////////
private:
	void InitIndex();			//��ʼ������
	int FindIndex(char *);		//���ر���λ��
	int FindIndexPos(char *);	//���صڼ�������
	int FindIndex(int nPos);	//���صڼ�������λ��
	int FindNextIndex(int nPos);	// ������һ������λ��
	int FindData(int, char *);	//��������λ��
	int GotoNextLine(int); 		//����
	char *ReadDataName(int &);	//��ָ��λ�ö�һ��������
	char *ReadText(int);		//��ָ��λ�ö��ַ���

	bool AddIndex(char *);					//����һ������
	bool AddData(int, char *, char *);		//�ڵ�ǰλ�ü���һ������
	bool ModityData(int, char *, char *);	//�ڵ�ǰλ���޸�һ�����ݵ�ֵ

private:
	char m_strFileName[MAX_PATH];	//�ļ���
	int m_lDataLen;					//�ļ�����
	char *m_pData;					//�ļ�����

	int m_lIndexNum;				//������Ŀ��[]����Ŀ��
	int* m_pIndexList;				//������λ���б�

	int m_lPoint;					//��ǰָ��
	int m_lLine, m_lWord;			//��ǰ����

};

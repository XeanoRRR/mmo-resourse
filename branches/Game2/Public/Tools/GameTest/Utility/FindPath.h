//********************************************
//	Ѱ· ��غ���
//  ������2000��3��22��
//********************************************

#pragma once

class CRegion;
class CRegionList;

//*************
//Ѱ·��
class CFindPath
{
public:
	//�ڵ�
	struct	tagNote
	{
		int step;		//�ӿ�ʼ��ʵ�ʵ���Ĳ���
		int x,y;		//�����
		int ox, oy;		//�õ���һ���������

		float fStep;	//����ʼ�㵽�õ��ʵ��ֵ��ֱ����1��б����1.414
		float fJudge;	//����ֵ���õ㵽Ŀ����ֱ�߾���
	};

	// Ѱ�ҵ���·���ṹ
	struct stPos
	{
		long x;
		long y;
		stPos()
		{
			x = 0;
			y = 0;
		}
	};

private:
	int sx,sy;			//��ʼ��
	int ex,ey;			//Ŀ���
	int Width, Height;	//������С
	RECT Range;			//Ѱ·�ĵ�ͼ��Χ

	map<long, tagNote> m_mapOpen;		//������ڵ�
	map<long, tagNote> m_mapClose;		//������Ľڵ�

	list<stPos>	m_listPath;	//Ѱ�ҵ���·��

	CRegion *m_pMap;		//��ͼ

public:
	CFindPath();	//����
	CFindPath(CRegion *);	//����
	~CFindPath();			//����

	void	SetRegion(CRegion *pRegion);
	bool Find(int, int, int, int);		//������
	bool Find(int, int, int, int,int lWidth, int lHieht);		//������
	list<stPos>* GetPathList()	{return &m_listPath;}
	CRegion* GetRegion()		{return m_pMap;}

private:
	void Init(int,int);		//��ʼ�������ڵ�����
	void Release();			//�ͷ�

	int Try(int, int, int, float, int, int, int, float);	//�����ӽڵ�
	float Judge(int, int );		//��ֵ�������ӵ�ǰ�㵽Ŀ��㣩
	int FindShortest();			//��OPEN��Ѱ�����ŵ�
	int FindShortestInClose();	//Ѱ�����ŵ� in Close Table

	int AddToOpen(tagNote);		//���뵽OPEN��
	int AddToClose(tagNote);	//���뵽CLOSE��

	int InOpen(int, int);	//�Ƿ���OPEN���� -1=false
	int InClose(int, int);	//�Ƿ���CLOSE���� -1=false

	// ==============================�糡���ƶ�������08.5.6��=================================
public:
	// Ϊָ���㵽Ŀ��������Ӧ��·����
	bool AddPathPos(long lRegionID, long lSourx, long lSoury, long lTargx, long lTargy);
	bool AddPathPos(long lRegionID, long lSourx, long lSoury, long lTargx, long lTargy, long lWidth, long lHeight);
	// ��õ�ǰ�����Ľ������б�
	vector<RECT>& GetPathRectList()	{return	m_vecPathRect;}
private:
	// �ڵ�ǰ�����µĽ�����
	vector<RECT>	m_vecPathRect;
	// =======================================================================================
};

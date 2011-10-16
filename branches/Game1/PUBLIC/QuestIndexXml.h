#pragma once
#include "tinyxml.h"
#include "CStrConverter.h"

//���ڶԿͻ��˿��Ƶ�����Ӧ��ģ����ز������õĶ�ȡ
//2010-1-11��ӶԸ����̵��ģ����ʾ��Ч������
class CQuestIndexXml
{
public:
	CQuestIndexXml();
	~CQuestIndexXml();
	void Release();
	//���������Ľṹ�����������������id,�������е�id,
	struct tagQuestNode{
		long lNodeID;           //���õ��������ϵ�BUFF��
		string strNodeName;         //BUFF��Ӧ����Ч��ʾID��
		long lParentID;         //�ý��ĸ����ID,һ�����Ϊ0;
		long lTree;             //�ý�����ڵ�������Ŀǰ����������
		tagQuestNode()
		{
            lNodeID = 0;
			strNodeName = "";
            lParentID = 0;
			lTree = 0;
		}
	};	
	struct tagQuestIndex{
		long lQuestID;
		long lTreeID;		
		int  iQuestNote;        //�����ǣ����Ƽ�������¼�����
		long lNumSum;           //������ÿ������������
		long lNumParam;      //�������������
		long lParamTextPos;   //�����������ʾ��X����
		long lTree;           //���������ڵ���
		int iState;           //��ʾ״̬������0������1�����2������ʾ3
		int iLevelLimit;      //��ʾ�����Ƿ��еȼ����� (0,�еȼ�����;1,�޵ȼ�����)
		int iStep;            //��Ҫ��ʾ��������
		tagQuestIndex()
		{
			lQuestID = 0;
			lTreeID = 0;			
			iQuestNote = 0;
			lNumSum = 0;
			lNumParam = 0;
			lParamTextPos = 0;
			lTree =0;
			iState=0;
			iLevelLimit = 1;
			iStep = 1;
		}
	};
	bool LoadQuestSetupXml();   //��ȡdata�¶����������ļ�
///////////////////////////////////////////////////////////////////////
//////////////////�������/////////////////////////////////
	bool LoadQuestNode_New(TiXmlElement* pElem);    //��ȡ���������
	bool LoadQuestIndex_New(TiXmlElement* pElem);
	vector<tagQuestNode>& GetQuestNode(){return m_vecQuestNode;}
	vector<tagQuestIndex>& GetQuestIndex() {return m_vecQuestIndex;}
	bool FindIndexQuest(long Index);            //����һ�������Ƿ�����������������
	void SetIndextQuest(long Index,long NumParam);   //����һ����������������
	bool GetIndextQuest(long Index,long &NumParam);          //���һ����������������
	void SetIndexQuestState(long Index,int istate);   //����һ���������ʾ״̬
    int GetIndextQuestState(long Index);     //���һ���������ʾ״̬
	int GetIndexQuestStep(long Index);       //���������������Ҫ��ʾ�Ĳ���
private:

	vector<tagQuestNode> m_vecQuestNode;
	vector<tagQuestIndex> m_vecQuestIndex;
/////*************************************************���������*********************************/////
public:
	struct tagQuestIndexForServer{
		long lQuestID;
		long lNumSum;        //������ÿ������������
		long lNumParam;      //�������������
		int iState;          //��ʾ״̬������0������1�����2������ʾ3
		DWORD lReSetTime;     //�������õ�ʱ���(������������ʵ������ʱ��)
		long lReSetTimeToXml; //�����е���������ʱ���
		
        tagQuestIndexForServer()
		{
			lQuestID = 0;
			lNumSum = 0;
			lNumParam = 0;
			iState = 0;
			lReSetTime = 0;
			lReSetTimeToXml = 0;
		}
	};

	bool LoadQuestSetupXmlForServer();     //���ڷ�������ȡ����
	bool LoadQuestIndex_NewEx(TiXmlElement* pElem);
	bool FindIndexQuestEx(long Index);
	void SetIndextQuestEx(long Index,long NumParam);
	bool GetIndextQuestEx(long Index,long &NumParam);
	void SetIndexQuestStateEx(long Index,int istate); 
    CQuestIndexXml::tagQuestIndexForServer GetIndextQuestFromXML(long Index);

	bool CodeToByteArray(DBWriteSet& dbWriteSet);
	bool DecodeToByteArray(DBReadSet& dbReadSet);

private:
	map<long ,tagQuestIndexForServer> m_tagQuestIndexForServer;
};
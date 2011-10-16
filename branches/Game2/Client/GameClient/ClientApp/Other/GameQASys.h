#pragma once
//AddBy: nikai	2008.11
//��ϷQAģ��
class CGameQASys
{
public:
	//�ʴ�����
	enum QA_TYPE
	{
		QA_BEGINER	= 0,	//��������
		QA_OCC		= 1,	//ְҵ���
		QA_TASK		= 2,	//�������
		QA_EQUIP	= 3,	//װ�����
		QA_MAP		= 4,	//��ͼ����
		QA_ITEM		= 5,	//��Ʒ����
		QA_SPECIAL	= 6,	//��ɫϵͳ
		QA_TYPE_TOTAL = 7
	};

	struct tagQAConfig
	{
		QA_TYPE		type;						//��������
		string		strDesc;					//����
		string		strQuestionTreeFileName;	//���ڼ������οؼ��������ļ���·��
		string		strAnswerContentFileName;	//���ڶ�Ӧ������������Ĵ����ݵ��ļ���
		tagQAConfig():type(QA_BEGINER)
		{}
	};


	//������
	struct tagQaDoc 
	{
		bool			isTreeLoaded;		//�Ƿ��Ѿ����سɹ����οؼ����ݡ�
		TiXmlDocument	m_QaTreeDoc;		//���οؼ���������
		
		map<string,DWORD>	mapQuestionTree;	//��������

		bool			isAnswerLoaded;		//�Ƿ��Ѿ����سɹ�������
		map<DWORD,string>	mapQaAnswerDB;		//����������
		tagQaDoc():isTreeLoaded(false),isAnswerLoaded(false)
		{}
	};

private:

	//���������ļ���
	static tagQAConfig		m_tagQaConfig[QA_TYPE_TOTAL];
	static tagQaDoc			m_tagQaDoc[QA_TYPE_TOTAL];
	
public:
	CGameQASys();
	~CGameQASys();
	static void InitialGameQaSys();
	static tagQAConfig	*GetQaConfigFile()	{return m_tagQaConfig;}
	static tagQaDoc		*GetQaDoc()			{return m_tagQaDoc;}
	
	static void LoadQaDoc(QA_TYPE type);
};
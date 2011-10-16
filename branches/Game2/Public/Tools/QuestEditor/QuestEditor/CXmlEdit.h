
#pragma once
#include "stdafx.h"
#include "CAttribute.h"
#include "CStep.h"
#include "CEncouragement.h"
#include "tinyxml.h"
#include <vector>
#include <map>

class CXmlEdit
{      
public:
    struct  QuestListInfo 
    {
       long     MaxRoleNum;
       long     MaxMainRoleNum;
       string   AddQuestScript;
       string   CompleteQuestScript;				
       string   QuestAccept;
       string   QuestUpdate;

       QuestListInfo()
       {
           MaxRoleNum =1000;
           MaxMainRoleNum=1000;
           AddQuestScript="scripts/questmodel/quest_model_add_a.script";
           CompleteQuestScript="scripts/questmodel/quest_model_complete.script";
           QuestAccept="scripts/questmodel/quest_model_add_c.script";
           QuestUpdate="scripts/questmodel/quest_model_Update.script";
       }
    };

    typedef std::map<CString,vector<tagRole*> >            MapFileTable;
    typedef std::map<long,vector<tagRoleEncouragement*> >  MapAwardTable;
    typedef std::map<CString,map<CString,CString> >        MapTextFileTable;
    typedef std::map<CString,MapAwardTable*>               MapAwardFileTable;

	//�����������洢���е�����
	vector<tagRole*>            m_vcAllRole;
	//tagRole*���͵ĵ�����
	vector<tagRole*>::iterator  tagRoleIter;
	//���������ļ��б�
	vector<CString>             m_vcRoleFileList;

    //�������ļ��б�
    vector<CString>             m_AwardList;
    //
    MapAwardFileTable           m_AllAwardList;
    //
    std::map<long,CString>      m_AwardIdFileList;


    //�����ļ�ӳ������
    MapFileTable                m_AllQuestList;
    //����ID��Ӧ������
    std::map<long,CString>      m_AllQuestStrId;


    //�����ı���Դ
    map<CString,CString>        m_TextRes;
    //���ļ��������ı���Դ
    MapTextFileTable            m_AllQuestTextFile;
    //List��������
    QuestListInfo               m_QuestListInfo;

	//tagRoleAttributeָ��
	tagRoleAttribute *pRoleAttribute;
	//tagRoleStepָ��
	tagRoleStep *pRoleStep;
	//tagRoleEncouragementָ��
	tagRoleEncouragement *pRoleEncouragement;

public:
	CXmlEdit(void);
	~CXmlEdit(void);
    //����res
    bool LoadTextFile(const string& file_name, bool sync);
	//��ȡ���������ļ��е�������Ϣ.
	bool ReadeRoleFile();
    //��ȡ���������ļ��е���������Ϣ.
    bool ReadeQuestAwardFile();
    //��ȡ���е������ı���Դ
    bool ReadQuestTextRes();
	//�������ļ����ļ����б�
	vector<CString> GetRoleFileList();
    //��ý����ļ��б�
    vector<CString> GetQuestAwardFileList();
	//��ȡ������Ϣ
	bool GetRoleInfor(TiXmlElement *pRole,tagRole*& Role);
	//��ȡ������Ϣ___��������
	bool GetRoleAttributeInfor(tagRoleAttribute*& pRoleAttribute,TiXmlElement* pRoleElem);
	//��ȡ������Ϣ___������
	bool GetRoleStepInfor(tagRoleStep*& pRoleStep,TiXmlElement* children);
	//��ȡ������Ϣ___������
	bool GetRoleEnInfor(TiXmlElement* pRoleElem );

};

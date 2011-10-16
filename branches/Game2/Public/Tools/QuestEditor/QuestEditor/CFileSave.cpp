// CFileSave.cpp : ʵ���ļ�
// ʵ��CFileSave.h�������ĺ���
#include "StdAfx.h"
#include "CFileSave.h"
#include "QuestEditor.h"
#include "tinyxml.h"
#include "CXmlEdit.h"
#include <vector>
#include <Filter.h>
#include <algorithm>
#include "QuestEditorDlg.h"

#define AddQuestValueNode(Element,name,value,Default,Comment)\
    { \
        if ( value > Default ) \
        { \
            if (strcmp(Comment,""))  \
            {	\
                TiXmlComment *pComTemp = new TiXmlComment();\
                pComTemp->SetValue(Comment);\
                Element->InsertEndChild(*pComTemp); \
            } \
             TiXmlElement *pNodeName = new TiXmlElement(name); \
             Element->LinkEndChild(pNodeName); \
             char pStr[100];\
             sprintf_s( pStr ,"%d",value ); \
             TiXmlText *pNodeText = new TiXmlText(pStr);  \
             pNodeName->LinkEndChild(pNodeText);    \
        }\
   }

#define AddQuestTextNode(Element,name,value,Default,Comment) \
  {\
     if ( strcmp(value,Default)!=0 ) \
     { \
        if (strcmp(Comment,""))  \
        {	\
               TiXmlComment *pComTemp = new TiXmlComment();\
               pComTemp->SetValue(Comment);\
               Element->InsertEndChild(*pComTemp); \
        } \
        TiXmlElement *pNodeName = new TiXmlElement(name); \
        Element->LinkEndChild(pNodeName); \
        if (strcmp(value,""))   \
        {  \
           TiXmlText *pNodeText = new TiXmlText(value);  \
                pNodeName->LinkEndChild(pNodeText);    \
             } \
        }\
  }

#define AddQuestBaseNode(Element,name,value,Default) \
            { \
                if ( value > Default ) \
                {                    \
                    Element->SetAttribute(name,value ); \
                }  \
            }

#define AddQuestStrNode(Element,name,value,Default) \
            { \
              if ( strcmp(value,Default) != 0 ) \
              {   \
                Element->SetAttribute(name,value ); \
              }  \
            }

#define AddQuestBaseDefNode(Element,name,value,Default) \
            { \
            if ( value > Default ) \
                {                    \
                    Element->SetAttribute(name,value ); \
                }  \
                else\
                {\
                    Element->SetAttribute(name,"" ); \
                }\
            }

#define AddQuestStrDefNode(Element,name,value,Default) \
            { \
              if ( strcmp(value,Default) != 0 ) \
              {   \
                Element->SetAttribute(name,value ); \
              }  \
              else\
              {\
                Element->SetAttribute(name,"" ); \
              }\
            }


vector<int>EditRoleList;
vector<string>EditFileList;
//���캯��
CFileSave::CFileSave(void)
{
}
//��������
CFileSave::~CFileSave(void)
{
}                    

void  CFileSave::OnSaveRes()
{

    /// textres
    CreateDirectory("textres",NULL);
    {
        CXmlEdit::MapTextFileTable::iterator  MapIter = GetInst(CXmlEdit).m_AllQuestTextFile.begin() ;

        for ( ;MapIter!= GetInst(CXmlEdit).m_AllQuestTextFile.end() ; MapIter++ )
        {
            TiXmlElement * pElm = NULL;
            TiXmlDeclaration XmlDeclar("1.0","gb2312","no");

            TiXmlDocument XmlDoc( MapIter->first ) ;
            XmlDoc.InsertEndChild(XmlDeclar);

            pElm = new TiXmlElement("StringTable");

            map<CString,CString>::iterator  TextIter = MapIter->second.begin();

            for ( int i = 0 ; TextIter!= MapIter->second.end() ; i++ , TextIter++)
            {
                TiXmlElement *Res = new  TiXmlElement("String");
                Res->SetAttribute("id",TextIter->first);
                Res->SetAttribute("text",TextIter->second);
                pElm->InsertEndChild( *Res );
            }

            XmlDoc.InsertEndChild( *pElm );
            XmlDoc.SaveFile();
            delete pElm;
        }
    }

}


void  CFileSave::OnReWrite()
{
    /// ����д��������
    CXmlEdit::MapFileTable::iterator  QuestIter = GetInst(CXmlEdit).m_AllQuestList.begin() ;
    for ( ;QuestIter != GetInst(CXmlEdit).m_AllQuestList.end() ; QuestIter++ )
    {
        TiXmlElement * pElm = NULL;
        TiXmlDeclaration XmlDeclar("1.0","gb2312","no");

        TiXmlDocument XmlDoc( QuestIter->first ) ;
        XmlDoc.InsertEndChild(XmlDeclar);

        pElm = new TiXmlElement("Quest");

        vector<tagRole*>::iterator Qiter = QuestIter->second.begin(); 
        /// ��������
        for ( ; Qiter != QuestIter->second.end() ; Qiter ++ )
        {
            /// ����
            TiXmlElement Elment("QuestId");
            
            /// �����������
            TiXmlElement *pAttr= new TiXmlElement("Attribute");              
            {
                AddQuestBaseNode(pAttr,"ID",(*Qiter)->vcRole_Attribute->m_nID,0);
                AddQuestBaseNode(pAttr,"QuestType",(*Qiter)->vcRole_Attribute->m_QuestType,-1);
                AddQuestBaseNode(pAttr,"ShareQuest",(*Qiter)->vcRole_Attribute->m_ShareQuest,-1);
                AddQuestStrNode(pAttr,"DeleteScript",(*Qiter)->vcRole_Attribute->m_cDeleteScript.c_str(),"");

                AddQuestValueNode(pAttr,"Repeated",(*Qiter)->vcRole_Attribute->m_nRepeated,-1,"//�Ƿ����ظ�");
                AddQuestValueNode(pAttr,"BeRecord",(*Qiter)->vcRole_Attribute->m_nBeRecord,-1,"//�Ƿ񱻼�¼");
                AddQuestValueNode(pAttr,"MinGrade",(*Qiter)->vcRole_Attribute->m_nMiniGrade,-1,"//��С�ȼ�");
                AddQuestValueNode(pAttr,"MaxGrade",(*Qiter)->vcRole_Attribute->m_nMaxGrade,-1,"//���ȼ�");
                AddQuestValueNode(pAttr,"TotePet",(*Qiter)->vcRole_Attribute->m_TotePet,-1,"//�Ƿ���ҪЯ������ID");
                AddQuestValueNode(pAttr,"QuestSex",(*Qiter)->vcRole_Attribute->m_QuestSex,-1,"//�Ƿ��Ա�����");
                AddQuestValueNode(pAttr,"ReqSkill",(*Qiter)->vcRole_Attribute->m_ReqSkill,-1,"//�Ƿ����޶�");
                AddQuestTextNode(pAttr,"Doccu",(*Qiter)->vcRole_Attribute->m_Doccu.c_str(),"","//�Ƿ�ְҵ�޶�");
                AddQuestTextNode(pAttr,"Occupation",(*Qiter)->vcRole_Attribute->m_Occupation.c_str(),"","//�Ƿ�ְҵ�޶�");
                AddQuestValueNode(pAttr,"NextQuest",(*Qiter)->vcRole_Attribute->m_NextQuest,-1,"//��������");

                AddQuestValueNode(pAttr,"ShowAble",(*Qiter)->vcRole_Attribute->m_nShowAble,-1,"//�ܷ���ʾ");
                AddQuestValueNode(pAttr,"NPCShowType",(*Qiter)->vcRole_Attribute->m_nNPCShowType,-1,"//��ʾ����");
                AddQuestValueNode(pAttr,"NPCClewType",(*Qiter)->vcRole_Attribute->m_nNPCClewType,-1,"//������ʾ");
                AddQuestTextNode(pAttr,"DiscribTextID",(*Qiter)->vcRole_Attribute->m_nDiscribTextID,"","//���������ı�");
                AddQuestTextNode(pAttr,"NPCID",(*Qiter)->vcRole_Attribute->m_cNPCID.c_str(),"","//NPCԭʼ��");
                AddQuestTextNode(pAttr,"BeSearchDesc",(*Qiter)->vcRole_Attribute->m_nBeSearchDesc,"","//��ѯ������������");
                AddQuestTextNode(pAttr,"BeSearchSpDesc",(*Qiter)->vcRole_Attribute->m_nBeSearchSpDesc,"","//��ѯ�����������");
                AddQuestValueNode(pAttr,"FinishEffect",(*Qiter)->vcRole_Attribute->m_nFinishEffect,-1,"//�����Ч");
                AddQuestValueNode(pAttr,"EmbracerMapEffect",(*Qiter)->vcRole_Attribute->m_nEmbracerMapEffect,-1,"//�����ͼ��ʾ");
                AddQuestValueNode(pAttr,"EmbracerEffect",(*Qiter)->vcRole_Attribute->m_nEmbracerEffect,-1,"//���������Ч��");
                AddQuestValueNode(pAttr,"FinishEmbracerEffect",(*Qiter)->vcRole_Attribute->m_nFinishEmbracerEffect,-1,"//��������Ч��");

                AddQuestValueNode(pAttr,"HardFactor",(*Qiter)->vcRole_Attribute->m_nHardFactor,-1,"//�Ѷ�");
                AddQuestValueNode(pAttr,"Odds",(*Qiter)->vcRole_Attribute->m_nOdds,-1,"//����ϡ�ж�");
                AddQuestValueNode(pAttr,"MassPoint",(*Qiter)->vcRole_Attribute->m_nMassPoint,-1,"//�����");
                AddQuestValueNode(pAttr,"DeadLost",(*Qiter)->vcRole_Attribute->m_nDeadLost,-1,"//�����������Ƿ�ʧ��");
                AddQuestValueNode(pAttr,"LostRepeated",(*Qiter)->vcRole_Attribute->m_nLostRepeated,-1,"//�����������Ƿ��ܹ��ؽ�");
                AddQuestValueNode(pAttr,"ChapterNum",(*Qiter)->vcRole_Attribute->m_nChapterNum,-1,"//�½�");
                AddQuestValueNode(pAttr,"Picture",(*Qiter)->vcRole_Attribute->m_nPicture,-1,"//ͼƬ");
                AddQuestValueNode(pAttr,"Icon",(*Qiter)->vcRole_Attribute->m_nIcon,-1,"//IOCN");
                AddQuestTextNode(pAttr,"StoryDesc",(*Qiter)->vcRole_Attribute->m_nStoryDesc,"","//����");
            }
            Elment.InsertEndChild( *pAttr );
            
            /// ����ǰ�������б�
            TiXmlElement *pPreFix = new  TiXmlElement("Quest_LablePrefix");
            {
                for ( int i = 0 ; i < (int) (*Qiter)->m_nRoleIndex.size() ; i++)
                {
                    AddQuestValueNode(pPreFix,"QuestIndex",(*Qiter)->m_nRoleIndex[i],0,"");
                }
            }
            Elment.InsertEndChild( *pPreFix );

            /// ������ 
            TiXmlElement *pStep= new TiXmlElement("Step");
            {
                AddQuestBaseNode(pStep,"ID",1,0);
     
                AddQuestTextNode(pStep,"NPCID",(*Qiter)->vcRole_Step->m_cNPCID.c_str(),"","//����NPC");
                AddQuestValueNode(pStep,"NPCEffect_1",(*Qiter)->vcRole_Step->m_nNPCEffect_1,-1,"");
                AddQuestValueNode(pStep,"NPCEffect_2",(*Qiter)->vcRole_Step->m_nNPCEffect_2,-1,"");
                AddQuestValueNode(pStep,"EncouragementCell",(*Qiter)->vcRole_Step->m_nEncouragementCell,-1,"//������ռ������");

                /// ����׷��
                vector<tagStepTrace*>::iterator  TraceIter = (*Qiter)->vcRole_Step->vcStepRoleTrace.begin();
                for ( ; TraceIter != (*Qiter)->vcRole_Step->vcStepRoleTrace.end() ; TraceIter ++ )
                {
                    TiXmlElement *Trace = new  TiXmlElement("RoleTrace");
                    Trace->SetAttribute("Desc",(*TraceIter)->m_nDesc.c_str());
                    AddQuestBaseDefNode(Trace,"ShowNum",(*TraceIter)->m_nShowNum,0);
                    AddQuestBaseDefNode(Trace,"Sum",(*TraceIter)->m_nSum,0);
                    AddQuestBaseDefNode(Trace,"Param",(*TraceIter)->m_nParam,0);
                    AddQuestBaseDefNode(Trace,"ID",(*TraceIter)->m_nID,0);

                    pStep->InsertEndChild( *Trace );
                }

                sort( (*Qiter)->vcRole_Step->vcStepAim.begin(),(*Qiter)->vcRole_Step->vcStepAim.end(),AimCMp);

                /// ����Ŀ��
                vector<tagStepAim*>::iterator  AimIter = (*Qiter)->vcRole_Step->vcStepAim.begin();
                for ( ; AimIter != (*Qiter)->vcRole_Step->vcStepAim.end() ; AimIter ++ )
                {
                    TiXmlElement *Aim = new  TiXmlElement("Aim");
                    Aim->SetAttribute("ID",(*AimIter)->m_nID);
                    Aim->SetAttribute("AimModel",(*AimIter)->m_nAimModel); 
                    Aim->SetAttribute("AimType",(*AimIter)->m_nAimType);
                    Aim->SetAttribute("AimName",(*AimIter)->m_cAimName.c_str());
                    Aim->SetAttribute("AimNum",(*AimIter)->m_nAimNum);

                    AddQuestTextNode(Aim,"DropFrom",(*AimIter)->m_cDropFrom.c_str(),"NULL","")
                    AddQuestValueNode(Aim,"DropOdds",(*AimIter)->m_nDropOdds,0,"");
                    AddQuestValueNode(Aim,"Param",(*AimIter)->m_nParam,0,"");

                    pStep->InsertEndChild( *Aim );
                }
            }
            Elment.InsertEndChild( *pStep );
            /// ��������
            pElm->InsertEndChild( Elment );
        }
        XmlDoc.InsertEndChild( *pElm );
        XmlDoc.SaveFile();
        delete pElm;
    }

    /// ���½�������
    CXmlEdit::MapAwardFileTable::iterator  AwardIter = GetInst(CXmlEdit).m_AllAwardList.begin() ;
    for ( ; AwardIter != GetInst(CXmlEdit).m_AllAwardList.end() ; AwardIter++ )
    {

        TiXmlElement * pElm = NULL;
        TiXmlDeclaration XmlDeclar("1.0","gb2312","no");

        TiXmlDocument XmlDoc( AwardIter->first ) ;
        XmlDoc.InsertEndChild(XmlDeclar);

        pElm = new TiXmlElement("Award");

        CXmlEdit::MapAwardTable * AwardTable = AwardIter->second;

        for (CXmlEdit::MapAwardTable::iterator AwdTter = AwardTable->begin() ; AwdTter != AwardTable->end() ;   AwdTter ++ )
        {
            /// ����
            TiXmlElement *Elment = new TiXmlElement("Quest");
            
            AddQuestBaseNode(Elment,"id",AwdTter->first,0);
            if ( AwdTter->second.size() > 0 )
            {
                 AddQuestBaseNode(Elment,"EncouragementCell",AwdTter->second[0]->m_nEncouragementCell,-1);
            }
            else
            {
                AddQuestBaseNode(Elment,"EncouragementCell",0,0);
            }

            for ( int i = 0 ; i < (int)AwdTter->second.size() ; i++ )
            {
                TiXmlElement *Encour = new  TiXmlElement("Encouragement");

                Encour->SetAttribute("Type",AwdTter->second[i]->m_nType);
                AddQuestStrDefNode(Encour,"Name",AwdTter->second[i]->m_cName.c_str(),"");
                Encour->SetAttribute("Num",AwdTter->second[i]->m_nNum);
                char strVal[16];
                sprintf_s( strVal ,"%lf", AwdTter->second[i]->m_fOdds);
                Encour->SetAttribute("Odds",strVal);

                Elment->InsertEndChild( *Encour );
            }
            
            pElm->InsertEndChild( *Elment );
        }

        XmlDoc.InsertEndChild( *pElm );

        XmlDoc.SaveFile();
        delete pElm;
    }
    OnSaveFileList();
    /// ������Դ����
}

void  CFileSave::OnSaveFileList()
{
    /// QuestList.xml
    {
        TiXmlElement * pElm = NULL;
        TiXmlDeclaration XmlDeclar("1.0","gb2312","no");

        TiXmlDocument XmlDoc( "quest/QuestList.xml") ;
        XmlDoc.InsertEndChild(XmlDeclar);

        pElm = new TiXmlElement("Quest");

        {
            /// ����
            TiXmlElement *Elment = new TiXmlElement("Attribute");
            AddQuestBaseDefNode(Elment,"MaxRoleNum",GetInst(CXmlEdit).m_QuestListInfo.MaxRoleNum,0);
            AddQuestBaseDefNode(Elment,"MaxMainRoleNum",GetInst(CXmlEdit).m_QuestListInfo.MaxMainRoleNum,0);
            AddQuestStrDefNode(Elment,"AddQuestScript",GetInst(CXmlEdit).m_QuestListInfo.AddQuestScript.c_str(),"");
            AddQuestStrDefNode(Elment,"CompleteQuestScript",GetInst(CXmlEdit).m_QuestListInfo.CompleteQuestScript.c_str(),"");
            AddQuestStrDefNode(Elment,"QuestAccept",GetInst(CXmlEdit).m_QuestListInfo.QuestAccept.c_str(),"");
            AddQuestStrDefNode(Elment,"QuestUpdate",GetInst(CXmlEdit).m_QuestListInfo.QuestUpdate.c_str(),"");
            pElm->InsertEndChild( *Elment );
        }
       

        CXmlEdit::MapFileTable::iterator  QuestIter = GetInst(CXmlEdit).m_AllQuestList.begin() ;

        int i = 1;
        for ( ;QuestIter != GetInst(CXmlEdit).m_AllQuestList.end() ; QuestIter++ )
        {
            TiXmlElement *QuestName = new TiXmlElement("QuestName");
            AddQuestBaseDefNode(QuestName,"ID",i,0);
            TiXmlText *pNodeText = new TiXmlText(QuestIter->first);
            QuestName->LinkEndChild(pNodeText);
            pElm->InsertEndChild(*QuestName);
            //QuestName->SetAttribute("ID",i);
            //AddQuestTextNode(pElm,"QuestName",QuestIter->first,"","");
            i++;
        }

        XmlDoc.InsertEndChild( *pElm );
        XmlDoc.SaveFile();
        delete pElm;

    }

    /// QuestAward.xml
    {
        TiXmlElement * pElm = NULL;
        TiXmlDeclaration XmlDeclar("1.0","gb2312","no");

        TiXmlDocument XmlDoc( "quest/AwardList.xml") ;
        XmlDoc.InsertEndChild(XmlDeclar);

        pElm = new TiXmlElement("AwardList");


        CXmlEdit::MapAwardFileTable::iterator  AwardIter = GetInst(CXmlEdit).m_AllAwardList.begin() ;
        for ( ;AwardIter != GetInst(CXmlEdit).m_AllAwardList.end() ; AwardIter++ )
        {
            TiXmlElement *Award = new  TiXmlElement("list");
            Award->SetAttribute("file",AwardIter->first);
            pElm->InsertEndChild( *Award );
        }

        XmlDoc.InsertEndChild( *pElm );
        XmlDoc.SaveFile();
        delete pElm;
    }
}



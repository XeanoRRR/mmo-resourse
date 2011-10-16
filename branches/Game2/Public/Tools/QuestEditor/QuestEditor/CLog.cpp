/*Clog.cpp
 *CLog��ʵ���ļ�
*/
#include "StdAfx.h"
#include "CLog.h"
#include "QuestEditorDlg.h"
#pragma warning(disable:4996)
CLog::CLog()
{
}

CLog::~CLog()
{
}
/*����:��ȡϵͳ��ǰ��ʱ������ڡ�
*ժҪ:��ȡϵͳ��ǰ��ʱ�������,���ֱ𱣴��ڲ�ͬ�ı����С�
*����:strTime��������ʱ��.strDate����������.
*����ֵ:void
*���ߣ��� ��
*��������:2010-7-6
*/
void GetTime(CString & strTime,CString& strDate)
{
	//����һ��ʱ��Ľṹ��
	SYSTEMTIME st;
	//��ȡϵͳʱ��
	GetLocalTime(&st); 
	strDate.Format("%d-%d-%d",st.wYear,st.wMonth,st.wDay);
	strTime.Format("%d-%d-%d",st.wHour,st.wMinute,st.wSecond);
}
/*����:������־�ġ�
*ժҪ:���ݸ���ǰ���������Ժ͸��º������������������־��������m_Log������.
*����:pPre����ǰ��������Ϣ.pUped���º��������Ϣ.
*����ֵ:void
*���ߣ��� ��
*��������:2010-7-6
*/
void CLog::CreatLogList(vector<tagRoleAttribute*>pPre,
						vector<tagRoleAttribute*>pUped )
{
	char strVal[256];
	CString cTime;
	CString cDate;
	//��ȡʱ��
	GetTime(cTime,cDate);
	string strTime = (LPCTSTR)cTime;
	int id ;
	vector<tagRoleAttribute*>::iterator IterPre;
	vector<tagRoleAttribute*>::iterator IterUped;
	IterPre = pPre.begin();
	IterUped = pUped.begin();
	for (;IterPre!=pPre.end();IterPre++)
	{
		for (;IterUped!=pUped.end();IterUped++)
		{
			id = (*IterPre)->m_nID;
			if (strcmp(((*IterPre)->m_cName).c_str(),((*IterUped)->m_cName).c_str()))
			{
				string  cPre = (*IterPre)->m_cName;
				string  cUpdated = (*IterUped)->m_cName;
				m_Log.push_back("Name:"+cPre+"--->"+cUpdated);
			}
		   if ((*IterPre)->m_QuestType!=(*IterUped)->m_QuestType)
			{
				string  cPre = (LPCTSTR)itoa((*IterPre)->m_QuestType,strVal,10);
				string  cUpdated = (LPCTSTR)(itoa((*IterUped)->m_QuestType,strVal,10));
				m_Log.push_back("RoleType:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nRoleEmbracer!=(*IterUped)->m_nRoleEmbracer)
			{
				string  cPre = (LPCTSTR)itoa((*IterPre)->m_nRoleEmbracer,strVal,10);
				string  cUpdated = (LPCTSTR)itoa((*IterUped)->m_nRoleEmbracer,strVal,10);
				m_Log.push_back("RoleEmbracer:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nHardFactor!=(*IterUped)->m_nHardFactor)
			{
				string  cPre = itoa((*IterPre)->m_nHardFactor,strVal,10);
				string  cUpdated = itoa((*IterPre)->m_nHardFactor,strVal,10);
				m_Log.push_back("HardFactor:"+cPre+"--->"+cUpdated);
			}
			 if(strcmp(((*IterPre)->m_cDeleteScript).c_str(),((*IterUped)->m_cDeleteScript).c_str()))
			{
				string  cPre = ((*IterPre)->m_cDeleteScript).c_str();
				string  cUpdated = ((*IterUped)->m_cDeleteScript).c_str();
				m_Log.push_back("DeleteScript"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nEmbracerEffect!=(*IterUped)->m_nEmbracerEffect)
			{
				string  cPre = itoa((*IterPre)->m_nEmbracerEffect,strVal,10);
				string  cUpdated = itoa((*IterUped)->m_nEmbracerEffect,strVal,10);
				m_Log.push_back("EmbracerEffect"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nFinishEmbracerEffect!=(*IterUped)->m_nFinishEmbracerEffect)
			{
				string  cPre = itoa((*IterPre)->m_nFinishEmbracerEffect,strVal,10);
				string  cUpdated = itoa((*IterUped)->m_nFinishEmbracerEffect,strVal,10);
				m_Log.push_back("FinishEmbracerEffect:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nEmbracerMapEffect!=(*IterUped)->m_nEmbracerMapEffect)
			{
				string  cPre = itoa((*IterPre)->m_nEmbracerMapEffect,strVal,10);
				string  cUpdated = itoa((*IterUped)->m_nEmbracerMapEffect,strVal,10);
				m_Log.push_back("EmbracerMapEffect:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nBeRecord!=(*IterUped)->m_nBeRecord)
			{
				string  cPre = itoa((*IterPre)->m_nBeRecord,strVal,10);
				string  cUpdated = itoa((*IterUped)->m_nBeRecord,strVal,10);
				m_Log.push_back("BeRecord:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nShowAble!=(*IterUped)->m_nShowAble)
			{
				string  cPre = itoa((*IterPre)->m_nShowAble,strVal,10);
				string  cUpdated =itoa((*IterUped)->m_nShowAble,strVal,10);
				m_Log.push_back("ShowAble:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nOdds!=(*IterUped)->m_nOdds)
			{
				string  cPre = itoa((*IterPre)->m_nOdds,strVal,10);
				string  cUpdated = itoa((*IterUped)->m_nOdds,strVal,10);
				m_Log.push_back("Odds:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nMassPoint!=(*IterUped)->m_nMassPoint)
			{
				string  cPre =itoa((*IterPre)->m_nMassPoint,strVal,10);
				string  cUpdated = itoa((*IterUped)->m_nMassPoint,strVal,10);
				m_Log.push_back("MassPoint:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nDiscribTextID!=(*IterUped)->m_nDiscribTextID)
			{
				string  cPre = (*IterPre)->m_nDiscribTextID;
				string  cUpdated = (*IterUped)->m_nDiscribTextID;
				m_Log.push_back("DiscribTextID:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nMiniGrade!=(*IterUped)->m_nMiniGrade)
			{
				string  cPre = itoa((*IterPre)->m_nMiniGrade,strVal,10);
				string  cUpdated = itoa((*IterUped)->m_nMiniGrade,strVal,10);
				m_Log.push_back("MiniGrade:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nMaxGrade!=(*IterUped)->m_nMaxGrade)
			{
				string  cPre = itoa((*IterPre)->m_nMaxGrade,strVal,10);
				string  cUpdated = itoa((*IterUped)->m_nMaxGrade,strVal,10);
				m_Log.push_back("MaxGrade:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nRepeated!=(*IterUped)->m_nRepeated)
			{
				string  cPre = itoa((*IterPre)->m_nRepeated,strVal,10);
				string  cUpdated = itoa((*IterUped)->m_nRepeated,strVal,10);
				m_Log.push_back("Repeated:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nDeadLost!=(*IterUped)->m_nDeadLost)
			{
				string  cPre = itoa((*IterPre)->m_nDeadLost,strVal,10);
				string  cUpdated = itoa((*IterUped)->m_nDeadLost,strVal,10);
				m_Log.push_back("DeadLost:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nLostRepeated!=(*IterUped)->m_nLostRepeated)
			{
				string  cPre = itoa((*IterPre)->m_nLostRepeated,strVal,10);
				string  cUpdated = itoa((*IterUped)->m_nLostRepeated,strVal,10);
				m_Log.push_back("LostRepeated:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nEngageArmsRepute!=(*IterUped)->m_nEngageArmsRepute)
			{
				string  cPre = itoa((*IterPre)->m_nEngageArmsRepute,strVal,10);
				string  cUpdated = itoa((*IterUped)->m_nEngageArmsRepute,strVal,10);
				m_Log.push_back("EngageArmsRepute"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nChapterNum!=(*IterUped)->m_nChapterNum)
			{
				string  cPre = itoa((*IterPre)->m_nChapterNum,strVal,10);
				string  cUpdated = itoa((*IterUped)->m_nChapterNum,strVal,10);
				m_Log.push_back("ChapterNum:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nPicture!=(*IterUped)->m_nPicture)
			{
				string  cPre =itoa((*IterPre)->m_nPicture,strVal,10);
				string  cUpdated = itoa((*IterUped)->m_nPicture,strVal,10);
				m_Log.push_back("Picture:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nIcon!=(*IterUped)->m_nIcon)
			{
				string  cPre = itoa((*IterPre)->m_nIcon,strVal,10);
				string  cUpdated = itoa((*IterUped)->m_nIcon,strVal,10);
				m_Log.push_back("Icon:"+cPre+"--->"+cUpdated);
			}
			 if((*IterPre)->m_nStoryDesc!=(*IterUped)->m_nStoryDesc)
			{
				string  cPre = (*IterPre)->m_nStoryDesc;
				string  cUpdated = (*IterUped)->m_nStoryDesc;
				m_Log.push_back("StoryDesc:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nNPCShowType!=(*IterUped)->m_nNPCShowType)
			{
				string  cPre = itoa((*IterPre)->m_nNPCShowType,strVal,10);
				string  cUpdated = itoa((*IterUped)->m_nNPCShowType,strVal,10);
				m_Log.push_back("NPCShowType:"+cPre+"--->"+cUpdated);
			}
			 if (strcmp(((*IterPre)->m_cNPCID).c_str(),((*IterUped)->m_cNPCID).c_str()))
			{
				string  cPre = ((*IterPre)->m_cNPCID).c_str();
				string  cUpdated = ((*IterUped)->m_cNPCID).c_str();
				m_Log.push_back("NPCID"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nFinishEffect!=(*IterUped)->m_nFinishEffect)
			{
				string  cPre = itoa((*IterPre)->m_nFinishEffect,strVal,10);
				string  cUpdated = itoa((*IterUped)->m_nFinishEffect,strVal,10);
				m_Log.push_back("FinishEffect:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nNPCClewType!=(*IterUped)->m_nNPCClewType)
			{
				string  cPre = itoa((*IterPre)->m_nNPCClewType,strVal,10);
				string  cUpdated = itoa((*IterUped)->m_nNPCClewType,strVal,10);
				m_Log.push_back("NPCClewType:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nBeSearchDesc!=(*IterUped)->m_nBeSearchDesc)
			{
				string  cPre = (*IterPre)->m_nBeSearchDesc;
				string  cUpdated =(*IterUped)->m_nBeSearchDesc;
				m_Log.push_back("BeSearchDesc:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nBeSearchSpDesc!=(*IterUped)->m_nBeSearchSpDesc)
			{
				string  cPre = (*IterPre)->m_nBeSearchSpDesc;
				string  cUpdated =(*IterUped)->m_nBeSearchSpDesc;
				m_Log.push_back("BeSearchSpDesc:"+cPre+"--->"+cUpdated);
			}
			 if ((*IterPre)->m_nRoleIndex!=(*IterUped)->m_nRoleIndex)
			{
				string  cPre = (*IterPre)->m_nRoleIndex;
				string  cUpdated = (*IterUped)->m_nRoleIndex;
				m_Log.push_back("RoleIndex:"+cPre+"--->"+cUpdated);
			}
			m_Log.push_back("--> "+strTime+"�༭��"+"����idΪ:"+itoa(id,strVal,10)+"���������Խ�:");
		}
	}
}

/*����:������־�ġ�
*ժҪ:���ݸ���ǰ���������͸��º����������������־��������m_Log������.
*����:pPre����ǰ��������Ϣ.pUped���º��������Ϣ.
*����ֵ:void
*���ߣ��� ��
*��������:2010-7-6
*/
void CLog::CreatLogList(vector<tagRoleEncouragement*>pPre,
						vector<tagRoleEncouragement*>pUped)
{
	char strVal[256];
	CString cTime;
	CString cDate;
	//��ȡʱ��
	GetTime(cTime,cDate);
	string strTime = (LPCTSTR)cTime;
	int id ;
	vector<tagRoleEncouragement*>::iterator IterPre;
	IterPre = pPre.begin();
	vector<tagRoleEncouragement*>::iterator IterUped;
	IterUped = pUped.begin();
	id = (*IterUped)->m_nID;
	m_Log.push_back("--> "+strTime+"�༭��"+"����idΪ:"+itoa(id,strVal,10)+"��������:");
	if (pPre.size()<pUped.size())
	{
		m_Log.push_back("�����������");
	}
	 if (pPre.size()>pUped.size())
	{
		m_Log.push_back("ɾ����������");
	}
	 if (pPre.size()==pUped.size())
	{
		m_Log.push_back("�༭��������");
	}
}

/*����:������־�ġ�
*ժҪ:���ݸ���ǰ��������͸��º����������������־��������m_Log������.
*����:pPre����ǰ��������Ϣ.pUped���º��������Ϣ.
*����ֵ:void
*���ߣ��� ��
*��������:2010-7-6
*/
void CLog::CreatLogList(vector<tagRoleStep*> pPre,
						tagRoleStep* pUpdatedeStep)
{
	char strVal[256];
	CString cTime,cDate;
	//��ȡʱ�������
	GetTime(cTime,cDate);
	string strTime = (LPCTSTR)cTime;
	int id ;
	vector<tagRoleStep*>::iterator IterPre;
	IterPre = pPre.begin();
	id = pUpdatedeStep->m_nRoleID;
	m_Log.push_back("--> "+strTime+"�༭��"+"����idΪ:"+itoa(id,strVal,10)+"��������:");
	//��������Ŀ��
	vector<tagStepAim*>pPreStepAim = (*IterPre)->vcStepAim;
	vector<tagStepAim*>pUpedStepAim = pUpdatedeStep->vcStepAim;
// 	//���������ı�
// 	vector<tagStepText*>pPreStepText = (*IterPre)->vcStepText;
// 	vector<tagStepText*>pUpedStepText = pUpdatedeStep->vcStepText;
	//��������׷��
	vector<tagStepTrace*>pPreStepTrace = (*IterPre)->vcStepRoleTrace;
	vector<tagStepTrace*>pUpedStepTrace = pUpdatedeStep->vcStepRoleTrace;
// 	//����������
// 	vector<tagStepEncouragement*>pPreStepEn = (*IterPre)->vcStepEncouragement;
// 	vector<tagStepEncouragement*>pUpedStepEn = pUpdatedeStep->vcStepEncouragement;
	if (pPreStepAim.size()<pUpedStepAim.size())
	{
		m_Log.push_back("���������Ŀ��");
	}	
	if (pPreStepAim.size()>pUpedStepAim.size())
	{
		m_Log.push_back("ɾ��������Ŀ��");
	}	
	if (pPreStepAim.size()==pUpedStepAim.size())
	{
		m_Log.push_back("�༭������Ŀ��");
	}	
// 	if (pPreStepText.size()<pUpedStepText.size())
// 	{
// 		m_Log.push_back("����������ı�");
// 	}	
// 	if (pPreStepText.size()>pUpedStepText.size())
// 	{
// 		m_Log.push_back("ɾ���������ı�");
// 	}	
// 	if (pPreStepText.size()==pUpedStepText.size())
// 	{
// 		m_Log.push_back("�༭�������ı�");
// 	}	
	if (pPreStepTrace.size()<pUpedStepTrace.size())
	{
		m_Log.push_back("���������׷��");
	}	
	if (pPreStepTrace.size()>pUpedStepTrace.size())
	{
		m_Log.push_back("ɾ��������׷��");
	}	
	if (pPreStepTrace.size()==pUpedStepTrace.size())
	{
		m_Log.push_back("�༭������׷��");
	}
// 	if (pPreStepEn.size()<pUpedStepEn.size())
// 	{
// 		m_Log.push_back("���������͚h����");
// 	}	
// 	if (pPreStepEn.size()>pUpedStepEn.size())
// 	{
// 		m_Log.push_back("ɾ��������͚h����");
// 	}	
// 	if (pPreStepEn.size()==pUpedStepEn.size())
// 	{
// 		m_Log.push_back("�༭������͚h����");
// 	}
}
/*����:������־�ġ�
*ժҪ:�����ض������в���������Ӧ����־,
*����:strLog��������־������.
*����ֵ:void
*���ߣ��� ��
*��������:2010-7-6
*/
void CLog::CreatLogList(string strLog)
{
	CString strTime;
	CString strDate;
	GetTime(strTime,strDate);
	string LogText = (LPCTSTR)strTime+strLog;
	m_Log.push_back(LogText);

}
/*����:������־��
*ժҪ:�����ɵ���־���浽txt�ļ��С�
*����:
*����ֵ:void
*���ߣ��� ��
*��������:2010-7-6
*/
void CLog::SaveLog()
{
	FILE *fp;
	CString cTime, cDate;
	//��ȡ����
	GetTime(cTime,cDate);
	CString strDate = cDate;
	CString cTitle = "log\\"+strDate+".txt"; 
	fp=fopen(cTitle,"a");
	vector<string>::iterator iter;
	iter = m_Log.begin();
	for (;iter!=m_Log.end();iter++)
	{
		fprintf(fp,(*iter).c_str());
		fprintf(fp,"\n");
	}
	fclose(fp);
}

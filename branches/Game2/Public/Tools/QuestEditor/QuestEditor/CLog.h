/*Clog.h
*ժҪ:�����ʵ����־�ļ��������Լ�����ĺ���
*����:ʵ�ָ��ݲ�ͬ�Ĺ������ú���������,���ɲ�ͬ����־,������־���浽�ļ���ȥ��
*����:����
*��������:2010-7-6
*/
#pragma once
#include "QuestEditorDlg.h"

class CLog
{
public:
    CLog(void);
    ~CLog(void);
    //������־���ݵ�����
    vector<string>m_Log;
    //������־___�����޸��������Ե���־
    void CreatLogList(vector<tagRoleAttribute*>pPre ,
        vector<tagRoleAttribute*>pUped);
    //������־___�����޸�����������־
    void CreatLogList(vector<tagRoleEncouragement*>pPre ,
        vector<tagRoleEncouragement*>pUped);
    //������־___�����޸����������־
    void CreatLogList(vector<tagRoleStep*> pPre, tagRoleStep* pUpdatedeStep);
    //������־___�������Ĳ�����־
    void CreatLogList(string strLog);
    //������־�ļ�
    void SaveLog();
};

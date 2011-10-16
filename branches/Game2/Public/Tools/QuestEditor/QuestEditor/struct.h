#include <string>
#include<vector>
#include <cstring>
#include "StructCA.h"
#include "StructCStep.h"
#include "StructCEn.h"
#include "StructStepText.h"
#include "StructStepEn.h"
#include "StructStepTrace.h"
#include "StructStepAim.h"
using namespace std;

#ifndef _STRUCT_H
#define _STRUCT_H
//��������Ľṹ��
struct tagRole
{
	//����ID
	int			m_nID;
	//���������ļ���
	string		m_strFileName;
	//��������
	tagRoleAttribute*       vcRole_Attribute;
	//������
	tagRoleStep*            vcRole_Step;
	//ǰ����������
	vector<int>             m_nRoleIndex ;
	//������ռ�ĸ�����
	int			            m_nEncouragementCell;
	//���캯��
	tagRole()
		:m_nID(0)
		,m_nEncouragementCell(0)
		,m_strFileName(_T("�½�����"))
	{
        m_nRoleIndex.clear();
	}

    tagRole( const tagRole & role):m_nID(role.m_nID),
                                   m_strFileName(role.m_strFileName),
                                   m_nEncouragementCell(role.m_nEncouragementCell),
                                   m_nRoleIndex( role.m_nRoleIndex )
    {
         vcRole_Attribute = new tagRoleAttribute(*role.vcRole_Attribute);
         vcRole_Step      = new tagRoleStep( * role.vcRole_Step );
    }

	//��������
	virtual	~tagRole()
	{
        if (vcRole_Attribute!=NULL)
        {
            delete vcRole_Attribute;
            vcRole_Attribute=NULL;
        }
        if (vcRole_Step!=NULL)
        {
            delete vcRole_Step;
            vcRole_Step=NULL;
        }
	}
};
#endif
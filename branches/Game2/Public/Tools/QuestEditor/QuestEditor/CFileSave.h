#pragma once
#include "tinyxml.h"
#include "CAttribute.h"
#include "CStep.h"
#include "CEncouragement.h"
class CFileSave
{
public:
	//��������
	vector<tagRole*>m_vcRole;
	//�������������
	vector<tagRoleAttribute*>m_vcAttribute;
	//��������Ĳ���
	vector<tagRoleStep*>m_vcStep;
	//��������Ľ���
	vector<tagRoleEncouragement*>m_vcEncouragement;
	CFileSave(void);
	~CFileSave(void);

public:
    /// �ض���Դ
    void OnReWrite();
    /// 
    void OnSaveRes();

    /// ��д����List�ļ��б�
    void OnSaveFileList();

};

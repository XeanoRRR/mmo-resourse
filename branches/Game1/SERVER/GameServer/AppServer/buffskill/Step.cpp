#include "StdAfx.h"
#include "Step.h"
#include "Do.h"
#include "..\..\..\..\public\SkillRelated.h"




CStep::CStep(void)
{
}

CStep::~CStep(void)
{	
	itDo it=m_DoSome.begin();
	for (;it!=m_DoSome.end();++it)
	{
		MP_DELETE(*it);
	}
	m_DoSome.clear();
}

//��ʼ����������
void CStep::InitBaseData(CDo *pDo)
{
	m_DoSome.push_back(pDo);
}

//�������еĲ���
eModuleRet CStep::DoSome(stModuParam *pParam)
{
	
	eModuleRet eRet = MR_Proc_End;
	itDo it = m_DoSome.begin();
	for(;it != m_DoSome.end();it++)
	{ 
		eRet = (*it)->Execute(pParam);

		//����ǽ���,�ͷ���
		if(eRet != MR_Continue)
			break;
	}
	return eRet;
}

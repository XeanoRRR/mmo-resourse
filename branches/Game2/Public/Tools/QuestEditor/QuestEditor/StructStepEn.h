//�������񵥲������Ľṹ��
#ifndef _Struct_StepEn_H
#define _Struct_StepEn_H
struct tagStepEncouragement 
{
	//������Ʒ������
	int			m_nType;
	//������Ʒ������
	string		m_cName;
	//������Ʒ������
	int			m_nNum;
	tagStepEncouragement()
		:m_nType(0)
		,m_cName(_T(""))
		,m_nNum(0)
	{
	}
	~tagStepEncouragement()
	{
	}

};
#endif
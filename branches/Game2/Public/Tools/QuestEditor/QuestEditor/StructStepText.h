//�����������ı��Ľṹ��
#ifndef _Struct_StepText_H
#define _Struct_StepText_H
struct tagStepText 
{
	//���ͱ�־ID.1Ϊ��������ʾ���ı�.2Ϊ������ʱ���ı�.
	int			m_nID;
	//�ı�ID
	int			m_nName;
	tagStepText()
		:m_nID(0)
		,m_nName(0)
	{
	}
	~tagStepText()
	{

	}
};
#endif

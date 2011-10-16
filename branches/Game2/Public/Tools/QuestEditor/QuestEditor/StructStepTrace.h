//��������׷�ٵĽṹ��
#ifndef _Struct_StepTrace_H
#define _Struct_StepTrace_H
struct tagStepTrace 
{
	//����׷�������ı���ID
	string		m_nDesc;
	//��ʾģʽ
	int			m_nShowNum;
	//����
	int			m_nSum;
	//�������µ�׷������
	int			m_nParam;
	//׷��������ƷID
	int			m_nID;	
    tagStepTrace(long num=0):
		m_nShowNum(0)
		,m_nSum(0)
		,m_nParam(0)
		,m_nID(0)
	{	
        char szText[10];
        sprintf_s( szText ,"Q%d", num );
        m_nDesc = szText;
	}

    tagStepTrace( const tagStepTrace & trace)
    {
       m_nDesc = trace.m_nDesc;
       m_nShowNum = trace.m_nShowNum;
       m_nSum  = trace.m_nSum;
       m_nParam= trace.m_nParam;
       m_nID   = trace.m_nID;	 
    }

	~tagStepTrace()
	{
	}
};
#endif


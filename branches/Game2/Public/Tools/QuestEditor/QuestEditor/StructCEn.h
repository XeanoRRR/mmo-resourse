//�����������Ľṹ��
#ifndef  _Struct_CEn_H
#define  _Struct_CEn_H

struct tagRoleEncouragement
{
	//����ID
	int			m_nID;
	//��������Ʒ��ռ�ĸ�����
	int			m_nEncouragementCell;
	//������Ʒ����
	int			m_nType;
    //������Ʒ����
    int			m_nNum;
    //��������
    double      m_fOdds;
	//������Ʒ����
	string		m_cName;
	
	tagRoleEncouragement()
		:m_nID(0)
		,m_nEncouragementCell(0)
		,m_nType(0)
		,m_cName(_T(""))
		,m_nNum(0)
	{
	}
	virtual~tagRoleEncouragement()
	{
	}
};
#endif


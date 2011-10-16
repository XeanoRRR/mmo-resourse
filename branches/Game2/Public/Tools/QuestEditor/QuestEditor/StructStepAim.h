//��������Ŀ��Ľṹ��
#ifndef _Struct_StepAim_H
#define _Struct_StepAim_H
struct tagStepAim 
{
	//Ŀ��ID:��λ����ʾĿ������Ʒ,��1��ʼ.������.100��ʾ����ʼ��NPC,101��ʾ���������NPC.
	int			m_nID;
	//��AimModelΪ��λ��ʱ,��ʾ���������.
	int			m_nAimModel;
	//Ŀ������
	int			m_nAimType;
	//��RoleTrace��param����,ȷ������������
	int			m_nParam;
	//Ŀ������
	int			m_nAimNum;
	//����Ŀ����Ʒ��NPC��ԭʼ��
	string		m_cDropFrom;
	//Ŀ����Ʒ�ĵ��伸��___DropOdds/10000��
	int			m_nDropOdds;
    //Ŀ������
	string		m_cAimName;
	
	tagStepAim(long nID=0)
		:m_nID(nID)
		,m_nAimModel(0)
		,m_nAimType(0)
		,m_cAimName(_T(""))
		,m_nAimNum(0)
		,m_cDropFrom(_T("NULL"))
		,m_nDropOdds(0)
		,m_nParam(0)
	{
	}

    tagStepAim(const tagStepAim & aim)
        :m_nID(aim.m_nID)
        ,m_nAimModel(aim.m_nAimModel)
        ,m_nAimType(aim.m_nAimType)
        ,m_cAimName(aim.m_cAimName)
        ,m_nAimNum(aim.m_nAimNum)
        ,m_cDropFrom(aim.m_cDropFrom)
        ,m_nDropOdds(aim.m_nDropOdds)
        ,m_nParam(aim.m_nParam)
    {

    }

	~tagStepAim()
	{

	}
};

inline bool  AimCMp( const tagStepAim * rs,const tagStepAim * rm)
{
    return rs->m_nID < rm->m_nID;
}

#endif
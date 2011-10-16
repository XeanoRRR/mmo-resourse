//����һ���������ԵĽṹ��
#ifndef _StructCA_H
#define _StructCA_H
struct tagRoleAttribute 
{
	//����ID
	int			m_nID;
	//
	int			m_nRoleEmbracer;
	//�����Ѷ�
	int			m_nHardFactor;
	//ɾ������Ľű�
	string		m_cDeleteScript;
	//��������
	string		m_cName;
	//����������Ч��
	int			m_nEmbracerEffect;
	//������������ߵ�Ч��
	int			m_nFinishEmbracerEffect;
	//��������ߵ�ͼЧ��
	int			m_nEmbracerMapEffect;
	//�Ƿ񱻼�¼
	int			m_nBeRecord;
	//�Ƿ�����ʾ
	int			m_nShowAble;
	//ϡ�ж�
	int			m_nOdds;
	//�����
	int			m_nMassPoint;
	//����������ı�id
	CString		m_nDiscribTextID;
	//��С�ȼ�����
	int			m_nMiniGrade;
	//���ȼ�����
	int			m_nMaxGrade;
	//�Ƿ����ظ�
	int			m_nRepeated;
	//�����������Ƿ�ʧ��
	int			m_nDeadLost;
	//�������Ƿ��ܹ����½�����
	int			m_nLostRepeated;
	//��Ҫ��Ӷ������
	int			m_nEngageArmsRepute;
	//����_____��������
	int			m_nChapterNum;
	//ͼƬ_______��������
	int			m_nPicture;
	//ͼ��_________��������
	int			m_nIcon;
	//�����ı�ID_______���������ı�
	CString		m_nStoryDesc;
	//��ʾ����
	int			m_nNPCShowType;
	//NPC��ԭʼ��
	string		m_cNPCID;
	//�������ʱ����Ч
	int			m_nFinishEffect;
	//������������
	int			m_nNPCClewType;
	//��ѯ������������
	CString			m_nBeSearchDesc;
	//��ѯ�����������
	CString			m_nBeSearchSpDesc;
	//ǰ����������
	CString			m_nRoleIndex;
    //��������
    int         m_NextQuest;

    // add by:Expter
    //��������
    long        m_QuestType;
    //�ɷ�������
    long        m_ShareQuest;
    //Я������
    long        m_TotePet;
    //�����Ա�����
    long        m_QuestSex;
    //����ϡ�ж�
    long        m_Odds;
    //�����޶�
    long        m_ReqSkill;
    //ְҵ�޶�
    string      m_Occupation;    
    //��ְҵ�޶�
    string      m_Doccu;
    
	tagRoleAttribute()
		:m_nID(0)
		,m_QuestType(1)
		,m_nRoleEmbracer(1)
		,m_nHardFactor(-1)
		,m_cDeleteScript(_T("scripts/questmodel/quest_model_del_3.lua"))
		,m_cName(_T("δ֪������"))
		,m_nEmbracerEffect(-1)
		,m_nFinishEmbracerEffect(-1)
		,m_nEmbracerMapEffect(-1)
		,m_nBeRecord(0)
		,m_nShowAble(1)
		,m_nOdds(-1)
		,m_nMassPoint(-1)
		,m_nMiniGrade(1)
		,m_nMaxGrade(999)
		,m_nRepeated(0)
		,m_nDeadLost(-1)
		,m_nLostRepeated(-1)
		,m_nEngageArmsRepute(0)
		,m_nChapterNum(-1)
		,m_nPicture(-1)
		,m_nIcon(-1)
		,m_nStoryDesc(_T(""))
		,m_nNPCShowType(-1)
		,m_cNPCID(_T(""))
		,m_nFinishEffect(-1)
		,m_nNPCClewType(0)
        ,m_NextQuest(-1)
	{
        m_ShareQuest = 0;
        m_TotePet = m_QuestSex = m_Odds  = m_ReqSkill = -1;
        m_nBeSearchDesc="";
        m_nBeSearchSpDesc="";
        m_nDiscribTextID="";
        m_nRoleIndex="";
	}

    tagRoleAttribute(const tagRoleAttribute & Attribute)
        :m_nID(Attribute.m_nID)
        ,m_QuestType(Attribute.m_QuestType)
        ,m_nRoleEmbracer(Attribute.m_nRoleEmbracer)
        ,m_nHardFactor(Attribute.m_nHardFactor)
        ,m_cDeleteScript(Attribute.m_cDeleteScript)
        ,m_cName(Attribute.m_cName)
        ,m_nEmbracerEffect(Attribute.m_nEmbracerEffect)
        ,m_nFinishEmbracerEffect(Attribute.m_nFinishEmbracerEffect)
        ,m_nEmbracerMapEffect(Attribute.m_nEmbracerMapEffect)
        ,m_nBeRecord(Attribute.m_nBeRecord)
        ,m_nShowAble(Attribute.m_nShowAble)
        ,m_nOdds(Attribute.m_nOdds)
        ,m_nMassPoint(Attribute.m_nMassPoint)
        ,m_nMiniGrade(Attribute.m_nMiniGrade)
        ,m_nMaxGrade(Attribute.m_nMaxGrade)
        ,m_nRepeated(Attribute.m_nRepeated)
        ,m_nDeadLost(Attribute.m_nDeadLost)
        ,m_nLostRepeated(Attribute.m_nLostRepeated)
        ,m_nEngageArmsRepute(Attribute.m_nEngageArmsRepute)
        ,m_nChapterNum(Attribute.m_nChapterNum)
        ,m_nPicture(Attribute.m_nPicture)
        ,m_nIcon(Attribute.m_nIcon)
        ,m_nStoryDesc(Attribute.m_nStoryDesc)
        ,m_nNPCShowType(Attribute.m_nNPCShowType)
        ,m_cNPCID(Attribute.m_cNPCID)
        ,m_nFinishEffect(Attribute.m_nFinishEffect)
        ,m_nNPCClewType(Attribute.m_nNPCClewType)
        ,m_NextQuest(Attribute.m_NextQuest)
    {
        m_ShareQuest = Attribute.m_ShareQuest;
        m_TotePet    = Attribute.m_TotePet;
        m_QuestSex   = Attribute.m_QuestSex;
        m_Odds       = Attribute.m_Odds;
        m_ReqSkill   = Attribute.m_ReqSkill;
        m_nBeSearchDesc=Attribute.m_nBeSearchDesc;
        m_nBeSearchSpDesc=Attribute.m_nBeSearchSpDesc;
        m_nDiscribTextID=Attribute.m_nDiscribTextID;
        m_nRoleIndex=Attribute.m_nRoleIndex;
    }

	~tagRoleAttribute()
	{
	}

    /// ����༭���ĳ�ʼ��ӳ��ģ��
    void  Init();

};
#endif


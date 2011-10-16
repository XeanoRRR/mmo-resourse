#pragma once

/**************************************************/
// ������Ҫ�����Ǳ���ÿ��ְҵ������ѧϰʱ����������
// ���ṩһЩ�ӿڣ����ڲ�����Щ��������
// 07.11.21 ����
/**************************************************/

// ����ѧϰ������
class CStudySkillCondition
{
public:
	// �������Ե�����
	enum eConditionEx
	{
		CONDITIONEX_NULL = -1,			// �޸�������
		CONDITIONEX_GOODS,				// ���ӵ���
		CONDITIONEX_PRESKILL,			// ����ǰ�ü���
		CONDITIONEX_CONFLICTSKILL,		// �������Ƽ���
		CONDITIONEX_SCRIPT				// �����ű�
	};

	// ���������ṹ
	struct tagConditionEx
	{
		eConditionEx wConditionExType;	// ��������������
		string		 strValue;			// ��һ��ֵ���п�����������Ʒ��ԭʼ����Ҳ�п�����������ID��XML����ʱ�����ַ�����ʽ���棩
		long		 lValue;			// �ڶ���ֵ�����ڸ��ӵ�����˵�����������������ڸ���ǰ�ü��ܣ��������ܵȼ������Ƽ������޴��ʼ��Ϊ-1��
		tagConditionEx()
		{
			wConditionExType = CONDITIONEX_NULL;
			strValue = "";
			lValue = -1;
		}
	};

	// �����ṹ���ýṹ��������ѧϰָ�����ܵĻ��������⣬
	// ������һ�������������б�
	struct tagCondition
	{
		ulong dwSkillID;		    // ����ID
		ushort wSkillLev;			// ��ǰ���ܵĵȼ�
		ushort wStudyMode;		    // ��ǰ�ȼ���ѧϰģʽ
		ushort wPlayerLev;		    // Ҫ����ҵĵȼ�����
		ulong dwSp;				    // ѧϰ��Ҫ���ĵ�SPֵ
		ulong dwOccuLv;			    //ְҵ�ȼ�
		vector<tagConditionEx*>  vec_ConditionEx;	// ����ѧϰ�����б�
		tagCondition()
		{
			dwSkillID	= 0;
			wSkillLev	= 0;
			wStudyMode	= 0;
			wPlayerLev	= 0;
			dwSp		= 0;
			dwOccuLv	= 0;
		}
	};
	//�����б�
	struct tagTeachSkill
	{
		string			strOcc;	//ְҵ
		list<ulong>		lOccList;//ְҵ����
		list<ulong>		lSkillList;//�����б�
	};

	CStudySkillCondition();
	~CStudySkillCondition();
	
	// ����XML�й��ڼ���ѧϰ������
	static bool Init(const char *filename);
	static bool InitSkillList(const char *filename);	        // ����ѧϰ�����б�
	static bool InitFactureSkillList(const char *filename);	    // ��ʼ��רҵ���������б�

    static void Release();			                            // �ͷ���Դ

	// ����Ҫѧϰ���ܵ�ID�Լ���Ҫ�����ĵȼ����ҳ�������ܵ�ѧϰ����
	static tagCondition * GetCondition(ulong wSkillID, ushort wSkillLev);

	static bool AddToByteArray(vector<uchar> &pByteArray);		// ��ӵ�CByteArray
	static bool DecordFromByteArray(uchar* pByte, long& pos);	// ����
	static ulong GetDwordAttribute(TiXmlElement *pElem,string strName);

	static map<string,tagTeachSkill*> GetTeacherSkill() {return m_mapTeachSkill;}
	static bool CheckFactureSkill(long lID);

private:

	static list<tagCondition*> m_StudyConditionList;			    // ���еļ���ѧϰ�����б�
	static list<long>		   m_lFactureSkillList;				    // רҵ���������б�
	static map<string,tagTeachSkill*> m_mapTeachSkill;				// ���ڼ����б�

	static bool InitCondition(ulong dwID, TiXmlElement* pElem);	    // �������ܵ�ѧϰ����
	static bool InitConditionEx(vector<tagConditionEx*> & vec_ConditionEx, TiXmlElement* pElem);	// �������ܵĸ���ѧϰ����

	
};
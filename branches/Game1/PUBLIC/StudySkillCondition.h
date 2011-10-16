#pragma once

/**************************************************/
// ������Ҫ�����Ǳ���ÿ��ְҵ������ѧϰʱ����������
// ���ṩһЩ�ӿڣ����ڲ�����Щ��������
// 07.11.21 ����
/**************************************************/


#include "tinystr.h"
#include "tinyxml.h"
#include "tools.h"
#include <list>
using namespace std;


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
		CONDITIONEX_SCRIPT,			    // �����ű�
        CONDITIONEX_EXP,                // ���Ӿ���ֵ [chenqiang@20110317]
	};

	// ���������ṹ
	struct tagConditionEx
	{
		eConditionEx wConditionExType;	// ��������������
		string		 strValue;			// ��һ��ֵ���п�����������Ʒ��ԭʼ����Ҳ�п�����������ID��XML����ʱ�����ַ�����ʽ���棩
		LONG64		 lValue;			// �ڶ���ֵ�����ڸ��ӵ�����˵�����������������ڸ���ǰ�ü��ܣ��������ܵȼ������Ƽ������޴��ʼ��Ϊ-1��
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
		DWORD dwSkillID;		// ����ID
		WORD wSkillLev;			// ��ǰ���ܵĵȼ�
		WORD wStudyMode;		// ��ǰ�ȼ���ѧϰģʽ
		WORD wPlayerLev;		// Ҫ����ҵĵȼ�����
		DWORD dwSp;				// ѧϰ��Ҫ���ĵ�SPֵ
		DWORD dwOccuLv;			//ְҵ�ȼ�
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
		list<DWORD>		lOccList;//ְҵ����
		list<DWORD>		lSkillList;//�����б�
	};
	//����ֵ�����б�
	struct tagActive
	{
		long  lMinValue;
		long  lMaxValue;
		long  lStateID;
		long  lStateLv;		
	};


	CStudySkillCondition();
	~CStudySkillCondition();
	
	// ����XML�й��ڼ���ѧϰ������
	static bool Init(const char *filename);
	static void Release();			// �ͷ���Դ
	static bool InitSkillList(const char *filename);	//����ѧϰ�����б�
	static bool InitFactureSkillList(const char *filename);	//��ʼ��רҵ���������б�

	// ����Ҫѧϰ���ܵ�ID�Լ���Ҫ�����ĵȼ����ҳ�������ܵ�ѧϰ����
	static tagCondition * GetCondition(DWORD wSkillID, WORD wSkillLev);
	static bool AddToByteArray(vector<BYTE> &pByteArray);		// ��ӵ�CByteArray
	static bool DecordFromByteArray(BYTE* pByte, long& pos);	// ����
	static DWORD GetDwordAttribute(TiXmlElement *pElem,string strName);
	static map<string,tagTeachSkill*> GetTeacherSkill() {return m_mapTeachSkill;}
	static bool CheckFactureSkill(long lID);
	static bool InitCondition(DWORD dwID, TiXmlElement* pElem);	// �������ܵ�ѧϰ����
	static bool InitConditionEx(vector<tagConditionEx*> & vec_ConditionEx, TiXmlElement* pElem);	// �������ܵĸ���ѧϰ����
	static list<tagActive*>	&GetActiveList()	{return m_lActiveRatio;}		//��ȡ�����㱶���б�
private:
	static list<tagCondition*> m_StudyConditionList;			// ���еļ���ѧϰ�����б�
	static list<long>		   m_lFactureSkillList;				// רҵ���������б�
	static map<string,tagTeachSkill*> m_mapTeachSkill;			// ���ڼ����б�
	static list<tagActive*>	   m_lActiveRatio;					// �����㱶�������б�			
};
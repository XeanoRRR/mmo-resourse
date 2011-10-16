//======================================================================================
/**
*	@file		SkillList.h
*	@data		
*	@author		
*/
//======================================================================================
#pragma once

/**
*	@class CSkillList
*
*	@brief �����б�������
*/
class CSkillList
{
public:
	/// ����
	CSkillList(void);

	/// ����
	~CSkillList(void);

public:
	/// ����Ŀ��
	enum eSkillDestType
	{
		SDT_SELF,				// �Լ�
		SDT_SHAPE,				// ����shape
		SDT_TILE,				// ��ͼ����
	};

	/// ���ܲ���
	struct tagStep
	{
		WORD	wCommandID;			// ������
		uchar	bStringParam;		// �Ƿ����ַ�������
		union{
			long	lParam;				// ���β���
			char	*pstrParam;			// �ַ�������
		};
	};

	/// ���ܵȼ�
	struct tagLevel
	{
		WORD	wRange;					//	�ͷŷ�Χ
		vector<tagStep>	vectorSteps;	//	�����б�
	};

	/// ���ܽṹ
	struct tagSkill
	{
		DWORD	dwIndex;				//	���ܱ��
		string	strName;				//	��������
		string	strDescribe;			//	����
		eSkillDestType	lDestType;		//	Ŀ������

		vector<tagLevel>	vectorLevels;	//	�����ȼ�����ϸ����
	};

private:
	/// ԭʼSkill�б�(dwIndex, tagSkill)
	static map<DWORD, tagSkill> m_mapSkillList;

	/// �������ƶ�Ӧ��ID
	static map<string, WORD> m_mapStepNameID;

public:
	// ���ļ�����ԭʼ�����б�
	static BOOL LoadSkillList(const char* stepfilename, const char* dir_name);

	/// ����
	static bool AddToByteArray(vector<uchar>* pByteArray);

	/// ����
	static bool DecordFromByteArray(uchar* pByte, long& pos);

public:
	/// ���ݼ��ܱ�Ų�ѯָ����Skill����
	static tagSkill* GetPropertiy(DWORD dwIndex);

	/// ���ݼ������Ʋ�ѯָ����Skill����
	static tagSkill* GetPropertiyByName(const char* strName);

private:
	/// ��ȡһ������
	static BOOL LoadOneSkill(const char* filename);

	/// �ͷ�
	static void Release();
};

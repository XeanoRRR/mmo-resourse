//======================================================================================
/**
*	@file		ContributeSetup.h
*	@data		
*	@author		
*/
//======================================================================================

#pragma once

/**
*	@class ContributeSetup
*
*	@brief ���ҹ���������
*/

class ContributeSetup
{
public:
	/// ���ҹ�����Ʒ�ṹ
	struct tagContributeItem
	{
		DWORD	dwLoValue;									// ��Ʒ����
		DWORD	dwHiValue;									// ��Ʒ����
		DWORD	dwNum;										// ��Ʒ����
		string	strName;									// ����ԭʼ��
	};

	/// ���ҹ��ײ����ṹ
	struct tagContributePara
	{
		long	lCombatLevel1;								// ���һ�ɱ�����1	
		long	lCombatLevel2;								// ���һ�ɱ�����2

		long	lContribute1;								// ���ҹ�������1�ͷ��۳�(С��30����)
		long	lContribute2;								// ���ҹ�������2�ͷ��۳�(20-40֮��)

		long	lContributeLevel;							// ���ҹ��׻�õȼ�����
		long	lContributeMultiple;						// ���ҹ��׻�ñ�������

		long	lContributeBaseValue;						// ���ҹ�����ʧ����ֵ
		long	lContributeMax;								// ���ҹ�����ʧ��������ֵ

		long	lCountryCityContributeModifierLose;			// ������ع��ҹ���ϵ����ʧ��4.0����
		long	lCountryCityContributeModifierGain;			// ������ع��ҹ���ϵ����ȡ��1.2����
		long	lCountryCityContributeItemModifier;			// ������ر�������ϵ����2.8����
	};
public:
	/// ����
	ContributeSetup(void);

	/// ����
	~ContributeSetup(void);
private:
	/// ���ҹ��ײ����б�
	static tagContributePara m_ContributePara;

	/// ���ҹ�����Ʒ�б�
	static vector<tagContributeItem> m_vContributeItems;

public:
	/// ���ع��ҹ�������
	static bool LoadContributeSetup(const char* filename);

	/// ����
	static bool AddToByteArray(vector<uchar>* pByteArray);

	/// ����
	static bool DecordFromByteArray(uchar* pByte, long& pos);

	/// ȡ�ù��ҹ��ײ����б�
	static tagContributePara* GetContributeParaList();

	/// ȡ�ù�����Ʒ�б�
	static vector<tagContributeItem>* GetContributeItemList();

	/// ����
	static void Release();
};

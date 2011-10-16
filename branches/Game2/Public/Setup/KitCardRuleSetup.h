//======================================================================================
/**
*	@file		KitCardRuleSetup.h
*	@data		2010-01-14 10:06
*	@author		wing<totti19851101@gmail.com>
*/
//======================================================================================

#pragma once


/**
*	@class KitCardRuleSetup
*
*	@brief �׿�������
*/

class KitCardRuleSetup
{
public:
	/// �׿��۸�ṹ
	struct tagCardPrice
	{
		DWORD dwEquipLev;				// װ���ȼ�
		DWORD dwOpenHolePrice;			// ���׼۸�
		DWORD dwEnchasePrice;			// ��Ƕ�۸�
	};

	/// �׿���������ֵ
	struct tagRuleValue
	{
		string strType;					//�������
		long  lVal;						//��������ֵ
	};

	/// �׿��ṹ
	struct tagKitCard
	{	
		DWORD RuleId;					//�׿�����ID
		uchar RequireHoleNum;			//Ҫ�󿪿׵�����
		uchar RequireCardNum;			//�׿���������
		vector<long> CardIdArray;		//�׿���ID
		vector<tagRuleValue> AttrList;	//������������
	};

	/// �׿�����
	struct KitCardRule
	{
		/// �׿����Խṹ
		tagKitCard tagKitCardRule;

		/// �׿�����Ĭ�Ϲ���
		KitCardRule();

		/// 
		KitCardRule(DWORD dwId,uchar btHoleNum,uchar btCardNum);

		/// �׿���������
		~KitCardRule();
		
		/// �׿����Ա���
		BOOL Serialize(vector<uchar>* pStream, BOOL b = TRUE);

		/// �׿����Խ���
		BOOL Unserialize(uchar* pStream, long& lOffset, BOOL b = TRUE);	

		/// ȡ���׿�ID
		DWORD GetRuleId(){return tagKitCardRule.RuleId;}

		/// ȡ���׿���������
		uchar GetRequireHoleNum(){return tagKitCardRule.RequireHoleNum;}

		/// ȡ���׿���������
		uchar GetRequireCardNum(){return tagKitCardRule.RequireCardNum;}
	};

public:
	typedef map<int,KitCardRule*> MAP_KITCARDRULE;
	typedef MAP_KITCARDRULE::iterator ITR_KITCARDRULE;

public:	
	/// ����
	KitCardRuleSetup();

	/// ����
	~KitCardRuleSetup();

	/// ����
	static BOOL Serialize(vector<uchar>* pStream, BOOL b = TRUE);

	/// ����
	static BOOL Unserialize(uchar* pStream, long& lOffset, BOOL b = TRUE);

	/// �����׿�����
	static BOOL Load(const char* StrPath);

	/// ����
	static void Release();

	/// ���ݵȼ�ȡ�ÿ��׼۸�
	static DWORD GetOpenHolePrice(DWORD dwLev);

	/// ���ݵȼ�ȡ����Ƕ�۸�
	static DWORD GetEnchasePrice(DWORD dwLev);

	/// ȡ���׿������б�
	static MAP_KITCARDRULE* GetKitCardList();

private:
	/// �׿������б�
	static MAP_KITCARDRULE m_mRules;

	/// ���׺���Ƕ�۸��б�
	static vector<tagCardPrice> m_vecCardPrice;
};
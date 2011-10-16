//======================================================================================
/**
*	@file		SuitRuleSetup.h
*	@data		2010-01-12 15:47
*	@author		wing<totti19851101@gmail.com>
*	@brief		��װ����
*/
//======================================================================================

#pragma once


/**
*	@class SuitRuleSetup
*
*	@brief ��װ������
*/

class SuitRuleSetup
{
public:
	/// ��װ��������ֵ
	struct tagRuleValue
	{
		/// ��װ������������
		string strType;

		/// ��װ��������ֵ
		long  lVal;
	};

	/// ��װ����
	struct SuitRule
	{
		/// ��װ����
		map<int,string> mapEquip;

		/// ��װ��������ֵ
		map<int,vector<tagRuleValue*>*> mapAttr;

		/// ��װ���Թ���
		SuitRule();

		/// ��װ��������
		~SuitRule();

		/// ��װ���Ա���
		BOOL Serialize(vector<uchar>* pStream, BOOL b = TRUE);

		/// ��װ���Խ���
		BOOL Unserialize(uchar* pStream, long& lOffset, BOOL b = TRUE);

		/// ��װ��������
		void Release();
	};

public:
	/// ��װ���Ͻṹ����
	typedef map<int,SuitRule*> MAP_SUITRULE;

	/// ��װ���Ͻṹ���͵�����
	typedef MAP_SUITRULE::iterator ITR_SUITRULE;

public:
	/// ����
	SuitRuleSetup();

	/// ����
	~SuitRuleSetup();

	/// ����
	static BOOL Serialize(vector<uchar>* pStream, BOOL b = TRUE);

	/// ����
	static BOOL Unserialize(uchar* pStream, long& lOffset, BOOL b = TRUE);

	/// ������װ����
	static BOOL Load(const char* StrPath);

	/// ȡ����װ���ü���
	static MAP_SUITRULE* GetSuitRules() { return &m_mRules; }

	/// ����
	static void Release();

private:
	/// ��װ���Լ���
	static MAP_SUITRULE m_mRules;
};
//======================================================================================
/**
*	@file		GoodsSetup.h
*	@data		2010-01-05 17:30
*	@author		wing<totti19851101@gmail.com>
*/
//======================================================================================

#pragma once

#include "../ServerPublic/Server/Common/GoodsBaseProperties.h"

/**
*	@class GoodsSetup
*
*	@brief ��Ʒ���ü���
*/

class GoodsSetup
{
public:
	/// ����
	GoodsSetup();

	/// ����
	~GoodsSetup();

public:
	/// ������Ʒ����
	static bool Load(const char* strPath);

	/// ������Ʒ����DB�洢����
	static bool LoadAttrDBSetup(const char* strpath);

	/// ����
	static bool Serialize(vector<uchar>* pStream, bool b = true);

	/// ����
	static bool Unserialize(uchar* pStream, long& lOffset, bool b = true);

	/// ���л���DataBlock
	static bool Serialize(DBWriteSet& setWriteDB, bool b = true);

	/// ͨ��DataBlock��ԭ
	static bool Unserialize(DBReadSet& setReadDB, bool b = true);

	/// ���
	static void Release();

private:
	/// ��ʼ���������Ա��
	static void InitAddonNameMap();

public:
	/// ����ID��ȡ��Ʒ��������
	static CGoodsBaseProperties* QueryGoodsBaseProperties(ulong dwID);

	/// ����ԭʼ����ȡ��Ʒ��������
	static CGoodsBaseProperties* QueryGoodsBasePropertiesByOriginalName(const char* pName);

	/// �������ƻ�ȡ��Ʒ��������
	static CGoodsBaseProperties* QueryGoodsBasePropertiesByName(const char* pName);

	/// ����ID��ȡ��Ʒԭʼ��
	static const char* QueryGoodsOriginalName(ulong dwID);

	/// ����ID��ȡ��Ʒ���� 
	static const char* QueryGoodsName(ulong dwID);

	/// ������Ʒԭʼ����ȡ��Ʒ����
	static const char* QueryGoodsName(const char* pOriginName);

	/// ������Ʒԭʼ����ȡ��ƷID
	static ulong QueryGoodsIDByOriginalName(const char* pName);

	/// ����ID��ȡ��Ʒ��������
	static ulong GoodsSetup::QueryGoodsMaxStackNumber(ulong dwID);

	/// ������Ʒ����ȡ��ƷID 
	static ulong QueryGoodsIDByName(const char* pName);

	/// ȡ��"��Ǯ"�ı��
	static ulong GetGoldCoinIndex();

	/// ȡ��"��Ǯ"����Ʒԭʼ��
	static const char* GetGoldOrginName();

	/// ȡ��"��Ǯ"����Ʒ��
	static const char* GetGoldName();

	/// ȡ��"����"�ı��
	static ulong GetSilverIndex();
	
	/// ȡ��"����"����Ʒԭʼ��
	static const char* GetSilverOrginName();

	/// ȡ��"����"����Ʒ��
	static const char* GetSilverName();

	/// ȡ��"��ʯ"�ı��
	static ulong GetGemIndex();

	/// ȡ��"Ԫ��"�ı��
	static ulong GetYuanBaoIndex();

	/// ����ID��ȡ��Ʒ����
	static eGoodsBaseType QueryGoodsType(ulong dwID);

	/// ����ID��ȡ��Ʒ�۸�
	static ulong QueryGoodsPrice(ulong dwID);

	/// ����ID��ȡ��Ʒ����
	static const char* QueryGoodsDescribe(ulong dwID);

	/// ȡ����Ʒ�������Ա��
	static ulong GetAddonByName(string str);

	/// ȡ����ƷID����Ʒ��������ӳ�伯��
	static map<ulong,CGoodsBaseProperties*>* GetGoodsBasePropertiesList();

public:
	/// ��Ʒ����DB�洢
	static uchar s_GoodsAttrDBSetup[GAP_TATAL_NUM][2];

private:
	/// ��Ʒԭʼ������ƷIDӳ��
	static map<string, ulong> s_mOriginalNameIndex;

	/// ��Ʒ������ƷIDӳ��
	static map<string, ulong> s_mNameIndex;

	/// ��Ʒ�������Ը���
	static ulong s_dwAddonPropertyNum;

	/// ��ƷID����Ʒ��������ӳ��
	static map<ulong,CGoodsBaseProperties*> s_mGoodsBaseProperties;

	/// �������Ա��ӳ��
	static map<string,ulong> s_mAddonNameMap;
};

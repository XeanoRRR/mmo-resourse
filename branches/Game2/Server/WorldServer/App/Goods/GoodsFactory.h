/** 
*  @file   GoodsFactory.h
*  @author
*
*  @brief  ��Ʒ������
*  @date   
*/

#pragma once

class CGoods;

class CGoodsFactory 
{
public:
	/// ����
	CGoodsFactory();

	/// ����
	~CGoodsFactory();

	/// ��Ʒ�������Ե����ɼ���
	static void CreateAddonProperties(CGoods* pGoods);

	/// ������Ʒ
	static CGoods* CreateGoods(ulong dwID,long lType);	
	/// ���������Ʒ
	static void CreateGoods(ulong dwID, ulong dwAmount, vector<CGoods*>& vOut,long lType);


	/// ��������
	static bool GarbageCollect(CGoods** ppGoods);

	/// ������Ʒ 
	static CGoods* UnserializeGoods(uchar* pStream, long& lOffset,long lType);
	/// ������Ʒ 
	static CGoods* UnserializeGoods(DBReadSet& setReadDB,long lType);

	/// ��Ӵ�����Ʒ��־
	static void AddCreateGoodsLog(CGoods* pGoods,long lType);

	
private:
	/// ʹ����Ʒ�б�
	static map<long,set<CGoods*> >	s_UsingGoods;
	/// ����
	static bool Upgrade(CGoods* pGoods, eGoodsAddonProperties gapSource, eGoodsAddonProperties gapDest, bool bUpgrade = true );
	/// ����װ��
	static bool	UpgradeEquipment(CGoods* pGoods, long lTargetLevel);


	//! �����е���Ʒ��Ϣ����
	////////////////////////////////////////////////////////////////////////////////
public:
	///	���һ����Ʒ��Ϣ
	static void	AddGoodsInfo(const CGUID &Key, const vector<uchar> &vGoodsInfo);
	/// ȡ��һ����Ʒ��Ϣ
	static bool	GetGoodsInfo(const CGUID &Key, vector<uchar> &vGoodsInfo);
	/// ���ʹ����Ʒ��Ϣ
	static long OutputUseGoodsInfo(const char* pszFileName);

private:
	/// ����
	static void ClearInfo(long lClearNum);
	/// �������Ʒ��Ϣ
	static	map<CGUID, vector<uchar>>	m_mapChatGoodsInfo;
	/// �����ʱ���Ⱥ�˳��
	static	list<pair<CGUID, long>>		m_listSaveTimeOrder;

};
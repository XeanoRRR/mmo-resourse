#pragma once
#include "afxcmn.h"


// CIncrementShopInfo �Ի���

class CIncrementShopInfo : public CDialog
{
	DECLARE_DYNAMIC(CIncrementShopInfo)

public:
	CIncrementShopInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIncrementShopInfo();

// �Ի�������
	enum { IDD = IDD_IncrementShopInfo };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void SetDisplayIncrementShop();
	CListCtrl m_IncreShopGoodsList;
	virtual BOOL OnInitDialog();
//////////////////////////////////////////////////////////////////////////////////////////////
//////
	////						11.30Add						///////
public:
	struct tagShopGoods
	{
		DWORD	dwGoodsId;					// ��Ʒ��ID
		DWORD	dwPrice;					// Ҫ��ʾ�ĵ���
		DWORD	dwNum;						// Ҫ��ʾ��������
		DWORD	dwReadyTradeNum;			// ������Ʒ��Ԥ��������
		bool	bIsReadyTrade;				// �Ƿ����ڴ�������
		string	strGoodsName;				// ��Ʒ����
//		CButton *pItem;						// ��Ӧ����Ʒ��Ŀ
	//	vector<tagTradeGoodsInfo> vecTradeGoodsInfo;	// ����Ʒ�б���Ľ�����Ϣ(�˽ṹֻ������Ұ����ϵ���Ʒ����NPCʱ����Ҫ������Ʒ����Ϣ)

		tagShopGoods()
		{
			dwGoodsId = 0;
			dwPrice = 0;
			dwNum = 0;
			dwReadyTradeNum = 0;
			bIsReadyTrade = false;
			strGoodsName = "";
//			pItem = NULL;
		};
	};
private:
//	tagGoodsItem *FindtagGoods(CButton *pItem,int iPage);		// ������Ŀ��������Ҷ�Ӧҳ�����Ʒ�ṹ
	vector<tagShopGoods>	m_vecMyGoodsItem;		// ������ϵ���Ʒ��Ŀ�б�
	vector<tagShopGoods>	m_vecShopGoodsItem;		// �̵����Ʒ��Ŀ�б�
public:
	vector<tagShopGoods>&  GetShopGoods() {return m_vecShopGoodsItem;}
	void SetShopGoods(vector<tagShopGoods>&vec) {m_vecShopGoodsItem = vec;}
};

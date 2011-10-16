#pragma once
class CTestInfo;
class CGameClient;

#include "afxcmn.h"
#include "..\Utility\GUID.h"
#include "..\App\Goods.h"

// CPlayerTrade �Ի���

class CPlayerTrade : public CDialog
{
	DECLARE_DYNAMIC(CPlayerTrade)
///////////////////////////////////////////////////////////////////////
		//************����*******************
public:
	enum tagPriceType
	{
		PT_Money,			// ����
		PT_YuanBao,			// Ԫ��
	};

	enum tagUpdateObject
	{
		Trade_You,			//���¶���Ϊ�Է����
		Trade_Me,			//���¶���Ϊ�Լ�
	};
	struct tagTradeGoods 
	{
		long lNum;			// ��Ʒ������
		long lPos;			// ��Ʒ��λ��
		DWORD dwIconId;		// ��Ʒ��ͼ��ID
		CGUID guid;			// ��Ʒ��GUID
	};
	bool OpenTradePage(const CGUID& SessionID,const CGUID& MyID,long lMPlugID,const CGUID& YourID,long lYPlugID);		// �򿪽��׽���
	void OpenTradeDialog(CGameClient* client);
	const CGUID& GetSessionID()		{return m_SessionID;}					// ���ػỰID
	long GetMyPlugID()				{return m_lMPlugID;}					// ��������Լ���PlugID
	const CGUID& GetYouID()			{return m_YourID;}						// ���ؽ��׶Է���ID
private:
	static const WORD GRIDNUM;		//����ҳ��Ľ�����λ��
	/***************
	�߼�������������
	****************/
	CGUID		m_SessionID;		//ÿ�λỰ��ID
	CGUID		m_MyID;				//�ҵ����ID
	CGUID		m_YourID;			//�Է����ID
	long		m_lMPlugID;			//�Լ��ڴ˴λỰ�е�ID
	long		m_lYPlugID;			//�Է��ڴ˴λỰ�е�ID
	bool		m_bMeIsLocked;		//�ҷ�����״̬��־
	bool		m_bYouIsLocked;		//�Է�����״̬��־
	long		m_Yuanbao;			//�ҷ�Ԫ��
	long		m_Golds;			//�ҷ����
	WORD		m_CoinType;			//��������

	map<int,tagTradeGoods> m_MyTradeGoodsList;				//�ҷ�Ҫ���׵���Ʒ�б�
	map<int,CGoods> m_YourTradeGoodesList;					//�Է�Ҫ���׵���Ʒ�б�
	///********************************
//////////////////////////////////////////////////////////////////////////////////
public:
	CPlayerTrade(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlayerTrade();

// �Ի�������
	enum { IDD = IDD_DIALOG_Trade };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	bool bMyLock;
	bool bYouLock;
	// �Է���λ
	CListCtrl m_YouTradeList;
	// �Լ���λ
	CListCtrl m_MyTradeList;
	afx_msg void OnBnClickedChangetrade();
	afx_msg void OnBnClickedReadytrade();
	afx_msg void OnBnClickedTrade();
};

//=============================================================================
/**
 *  file: PlayerInfo.h
 *
 *  Brief:��ʾ�����Ϣ
 *
 *  Authtor:wangqiao
 *	
 *	Date:2007-7-25
 */
//=============================================================================

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
class CPlayer;
class CTestInfo;
class CGameClient;
//#include "..\Utility\GUID.h"

class CPlayerInfo : public CDialog
{
	DECLARE_DYNAMIC(CPlayerInfo)

public:
	CPlayerInfo(CWnd* pParent = NULL);
	virtual ~CPlayerInfo();

	enum { IDD = IDD_PROPPAGE_PLAYER_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	struct staPackGoods{
		CGUID guID;				//guid
		DWORD dwType;			//����
		string sOriName;		//ԭʼ��
		long lPackID;			//����ID
	};

private:
	CPlayer*	m_pDisplayPlayer;				//��ʾ�����
	CGameClient*	m_pSelGameClient;			//ѡ�еĿͻ���

public:
	void SetDisplayPlayer(CGameClient* pSelGameClient);
private:
	CStatic m_staPlayerName;
	CStatic m_staPlayrID;
	list<CPlayerInfo::staPackGoods*>	m_pPackGoods;
	bool bControl;
public:
	list<CPlayerInfo::staPackGoods*>	GetPackGoods() {return m_pPackGoods;}

	CStatic m_staPlayerHp;
	CStatic m_staPlayerMaxHp;
	CStatic m_staPlayerMp;
	CStatic m_staPlayerMaxMp;
	CStatic m_staPlayerExp;
	CStatic m_staPlayerLevel;
	CStatic m_staPlayerMoney;
	CStatic m_staPlayerUpExp;
	CListCtrl m_GoodsList;
	CStatic m_staSex;
	CStatic m_staStamina;
	CStatic m_staOccupation;
	CStatic m_staEnergy;
	CStatic m_staCountryContribute;
	CStatic m_staUpgradeExp;
	CStatic m_staCountry;
	CStatic m_staRanOfNobCredit;
	CStatic m_staRankOfNobility;
	CStatic m_staSpouseID;
	CStatic m_staPlayerSocketId;
};




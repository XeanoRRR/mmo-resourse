//=============================================================================
/**
 *  file: TestInfo.h
 *
 *  Brief:������Ϸ�е�������ʾ��Ϣ
 *
 *  Authtor:wangqiao
 *	
 *	Date:2007-7-25
 */
//=============================================================================

#pragma once

#include "RegionInfo.h"
#include "PlayerInfo.h"
#include "TreeGameClient.h"
#include "MonsterInfo.h"
#include "SkillInfo.h"
#include "RegionGoodsInfo.h"

class CTestInfo : public CDialog
{
	DECLARE_DYNAMIC(CTestInfo)

public:
	enum eChatType
	{
		CHAT_NORMAL = 0,		// ͬ������
		CHAT_REGION,			// ͬ��������
		CHAT_FACTION,			// �������
		CHAT_UNION,				// ��������
		CHAT_TEAM,				// ��������
		CHAT_PRIVATE,			// ����˽��
		CHAT_COUNTRY,			// ��������
		CHAT_WOLRD,				// ��������
		CHAT_BOARDCAST,			// �㲥
		GM_CMD,					// GM����
		CHAT_KING_COUNTRY,		// �������Һ���
		CHAT_MINISTER_COUNTRY,	// �󳼹��Һ���
		RUN_SCRIPT,				// ���нű�
		RUN_HELP,				// ����

		TYPE_NUM,
	};

public:
	CTestInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTestInfo();

	enum { IDD = IDD_PROPPAGE_GAMEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	HBRUSH  OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()

private:
	long m_lTimerID;
	BOOL	m_bCheckOne;

	long m_lMaxContNum;
	long m_lMinContNum;


	CStatic m_ShowAmountCtrl;
	CGameClient*	m_pSelGameClient;
	CRegionInfo				m_RegionInfo;
	CPlayerInfo				m_PlayerInfo;
	CMonsterInfo			m_MonsterInfo;
	CSkillInfo				m_SkillInfo;
	CTreeGameClient			m_ctrGameClients;
	CRegionGoodsInfo		m_RegionGoods;
	CEdit m_edInputBox;
	CButton m_CheckOne;
	CString	m_strScriptName;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnEnChangeEditOutputInfo();
	afx_msg void OnRunScript();				//ִ�нű�
	void ReleaseTimer()	{ KillTimer(m_lTimerID); m_lTimerID = -1; }
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	CTreeGameClient* GetTreeGameClient()	{return &m_ctrGameClients;}

	void OnTreeSelect();
	void OnSelectRegion();
	void ResetSelGameClient(CGameClient* pGameClient);
	void OutputGameInfo(const char* strInfo);
	void OutputThreadInfo(const char* info);
	void UpdateDisplayInfo(CGameClient* pGameClient);


	CGameClient*  GetSelGameClient()		{return m_pSelGameClient;}
	CMonsterInfo* GetMonsterInfo()			{return &m_MonsterInfo;}
	CRegionGoodsInfo* GetRegionGoodsInfo()	{return &m_RegionGoods;}
	CPlayerInfo* GetPlayerInfo()			{return &m_PlayerInfo;}
	CString m_StrInputBox;
	CEdit m_edOutputBox;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	const CGUID* GetPlayerAttributeToGUID();
	long GetPlayerAttributeToSocketID();
	const char* GetPlayerAttributeToName();
	const CString GetInputBoxInfo();	

	long GetMaxConnect() { return m_lMaxContNum; }
	long GetMinConnect() { return m_lMinContNum; }

};

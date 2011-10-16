// UniEditorDlg.h : ͷ�ļ�
//
#include "RegionTree.h"
#include "DlgMonster.h"
#include "DlgMoninfo.h"
//#include "DlgNPC.h"
#include "DlgFileList.h"
#include "TextLog.h"
#include "ListLog.h"
#include "DlgCollection.h"
#include "DlgCollInfo.h"
#include "DlgSignPos.h"
#include "DlgRoadsInfo.h"
#include "NumList.h"
#include <set>
#include <string>

#pragma once
using namespace std;

#define		IDT_REGION					10023
#define		MAX_NUM						10000

#define		LBS_NOTDOWN					0
#define		LBS_DOWN_SELECT_RECT		1
#define		LBS_DOWN_LEFT_TOP			5
#define		LBS_DOWN_RIGHT_BOTTOM		8
#define		LBS_DOWN_NPC				9

typedef		set<string>					SETUPDATE;
typedef		SETUPDATE::iterator			SETUPDATEITR;

const RECT rcMainDlg		= {0, 0, 1400, 900};	// ����ܴ���
const RECT rcTreeRegion		= {0, 0, 200, 800};		// ��������б�
const SIZE ptTreeRegion		= {10, 10};				// �����б����
const RECT rcMap			= {0, 0, 700, 700};		// ����ܴ���
const SIZE ptMap			= {215, 10};			// ��ͼ�������
const RECT rcDlg			= {0, 0, 860, 285};		// �ұ߱༭�Ի���
const SIZE ptDlg			= {215, 740};			// �༭�Ի������
const RECT rcLog			= {0, 0, 200, 512};		// ����ܴ���
const SIZE ptLog			= {700, 10};			// �༭�Ի������

// CUniEditorDlg �Ի���
class CUniEditorDlg : public CDialog
{
// ����
public:
	CUniEditorDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CUniEditorDlg();						// ��������

// �Ի�������
	enum { IDD = IDD_UNIEDITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ����
public:
	HACCEL m_hAccelTable;
	BOOL DrawMap(void);									// ���Ƶ�ͼ����
	BOOL DrawRegion(void);								// �����³���
	BOOL DrawRegion(float fx);								// ����
	BOOL DrawMonster(CDC &dc, const unsigned uNo=-1);	// ����Monster��Ϣ
	BOOL DrawCollection(CDC &dc, const unsigned uNo=-1);		// ����NPC��Ϣ
	BOOL DrawSignPos(CDC &dc,const unsigned uNo=-1);			//����·�����Ϣ
	BOOL DrawRoads(CDC &dc,const unsigned uNo2=-1,const unsigned uNo1=-1, unsigned uNo=-1);			//����·��


	void DrawMonsterVector(CDC &dc);
	void DrawCollectionVector(CDC &dc);
	inline const long GetRegionWidht()		{return m_lRegionWidth;}
	inline const long GetRegionHeight()		{return m_lRegionHeight;}
	inline SETUPDATE &GetUpdateList()		{return m_setUpdate;}
	inline CRegionTree *GetTreeCtrl()const	{return m_ptreeRegion;}
	inline VECMONSTER &GetVecMonster()		{return m_vecMonster;}
	inline VECMONINFO &GetVecMonInfo()		{return m_vecMoninfo;}
//	inline VECNPC &GetVecNPC()				{return m_vecNPC;}
	inline MAPTREE &GetMapTree()			{return m_mapTree;}
	inline Cell* GetCell()const				{return m_pCell;}
	static CTextLog* GetLog()				{return m_pTextLog;}

	//unsigned *GetMonster(const CPoint point);							// ��ȡpoint������Monster�����Ϣ
	//unsigned *GetNPC(const CPoint point);						 		// ��ȡpoint������NPC�����Ϣ
	unsigned GetMonster(const CPoint point);							// ��ȡpoint�����ϲ�Monster�����Ϣ
	unsigned GetCollection(const CPoint point);							// ��ȡpoint�����ϲ�Collection�����Ϣ
	unsigned GetSignPos(const CPoint point);							// ��ȡpoint�����ϲ�·�����Ϣ
	unsigned GetRoadsPoint(const CPoint point);
	long GetBlockCellNumber(const CRect &rect);						// �����������赲������
	long GetUnBlockCellNumber(const CRect &rect);						// �������������赲������
	CString MonsterDensity();											// �����ѡ�����ڹ����ܶ�
	CString GetEditInfo(const unsigned uNo, const CPoint point, const unsigned uType = ITEM_OFFSET_MONSTER);

	// ������������ڱ༭��Ϣ
	bool UpdateFileList(int  nflag);									// �����ļ��б�
	bool Backup(const CString &szFileName);								// �����ļ�
	bool SaveFile(const CString &szFileName);							// �����ļ�
	CString UpdateLog();												// ������־
	void SearchTreeNode(CPoint point,UINT ntpye);									//��λ���ڵ�
	void InvertLine(CDC *pDC,CPoint ptFrom,CPoint ptTo);								//��Ƥ����
	void ZoomInterpolation(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
					   UINT& outWidth, UINT& outHeight, double fx, double fy);			//����ͼ��
	void DrawJpg(unsigned int unNo);
	string SearchRoadsPoint(CPoint point);
	void CheckScript(int nType,vector<long>*vecData);
	void DrawMonsterFirst(vector<long>vecNo);
	void DrawCollectionFirst(vector<long>vecNo);
// ������Ա
public:
	CDlgMonster		m_dlgMonster;				// MONSTER������Ϣ�༭�Ի���
	CDlgMoninfo		m_dlgMoninfo;				// MONSTER��ϸ��Ϣ�༭�Ի���
//	CDlgNPC			m_dlgNPC;					// NPC��ϸ��Ϣ�༭�Ի���
	CDlgCollection	m_dlgCollection;			// �ɼ���������Ϣ�Ի���
	CDlgCollInfo	m_dlgCollInfo;				// �ɼ�����ϸ��Ϣ�Ի���
	CDlgSignPos		m_dlgSignPos;				//·���
	CDlgRoadsInfo	m_dlgRoadsPoint;			//·��
	static BOOL		m_bUpdateMonster;			// �༭���±��
	static BOOL		m_bUpdateMonInfo;			// �༭���±��
	
	//static BOOL		m_bUpdateNPC;				// �༭���±��
	static BOOL		m_bUpdateRoads;
	static BOOL		m_bUpdateCollection;		
	static BOOL		m_bUpdateCollInfo;
	static BOOL		m_bUpdateSignPos;
	static BOOL		m_bSelChange;				// ѡ����仯
	static CString	m_szModify;					// �޸�����
	static int CALLBACK Compositor(LPARAM lParam1,LPARAM lParam2,LPARAM lParamSort);
protected:
	static CTextLog	*m_pTextLog;				// ������־

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

// ��Ա
	BLENDFUNCTION	m_bf;						//��͸��ͼ	
	int		m_nTimes;					//��ǰ�Ŵ���
	CScrollBar		m_HScroll;
	CScrollBar		m_VScroll;
	BOOL			m_bNewRec;					//���Ʊ�ʶ
	CPoint			m_ptFrom;					//���ƾ������
	CPoint			m_ptTo;						//���ƾ����յ�
	CRegionTree		*m_ptreeRegion;				// ���οؼ�
	CInfo			*m_pInfo;					// ��ǰ�༭����
	//HTREEITEM		m_hItem;					// ��ǰѡ����
	//HTREEITEM		m_hParentItem;				// ��ǰѡ����֮�����
	MAPTREE			m_mapTree;					// ���洢ӳ��
	VECMONSTER		m_vecMonster;				// MONSTER����
	VECMONINFO		m_vecMoninfo;				// MONSTER������Ϣ����

	//VECNPC			m_vecNPC;					// NPC����
	VECCOLLECTION	m_vecCollection;			//�ɼ�������
	VECCOLLINFO		m_vecCollInfo;				//�ɼ��������Ϣ
	VECSIGNPOS		m_vecSignPos;				//·���
	MAPROADS		m_mapRoads;					//·��
	VECROADPOINT	m_vecRoadsPoint;			//·����
	long			m_lRegionHeight;			// �����߶�
	long			m_lRegionWidth;				// �������
	Cell			*m_pCell;					// ��ȡ����������Ϣ
	DWORD_PTR		m_uRegionPrev;				// �ϴ�ѡ�񳡾����
	DWORD_PTR		m_uRegionCurr;				// ��ǰѡ�񳡾����
	UINT			m_uLayer;					// ѡ������
	UINT			m_uOffset;					// ѡ����ƫ��
	CToolTipCtrl	m_toolTip;					// TOOLTIP
	CBitmap			m_Bitmap;					// ����λͼ
	CBitmap			m_BitJpg;					//ͼƬλͼ			
	CDC				m_dcMemory;					// �ڴ�DC
	CDC				m_dcJpg;					//ͼƬ�ڴ�DC
	CDC				m_dcContent;					//����Ͳɼ����ڴ�DC
	DWORD_PTR		m_uParentOffset;			// �����ƫ��
	float			m_fMonsterRate;				// ������Monster����
	unsigned		m_uLBStyle;					// ����������
	BOOL			m_bRBDown;					// ����Ҽ�����
	BOOL			m_bMouseDown;				// ��갴��(���Ҿ���)
	bool			m_bModify;					// ����޸�
	bool			m_bCreateNew;				// �������µ�MONSTER/NPC
	CPoint			m_ptOrigin;					// ��갴��ʱ������
	SETUPDATE		m_setUpdate;				// �����б�
	CRect			m_rcMap;					// �ұߵ�ͼ����
	CRect			m_rcMonsterDensity;			// �ⶨ�����ܶȵľ���
	CButton			m_btnSave;					// ���水ť
	CListLog		m_listLog;					// �༭��־�б�
	//CListCtrl		m_listNum;					//ͳ����Ŀ�б�
	bool			m_bShift;					//Shift�Ƿ���
	
	bool			m_bJpg;						//�Ƿ���ʾJPGͼƬ
	bool			m_bLButton;					//����������
	bool			m_bRButton;					//����Ҽ�����
	CPoint			m_pt;
	//CRect			m_rcMoveRect;
	CPoint			m_ptStart;
	bool			m_bSpace;
	bool			m_bSignPos;					//�Ƿ�ѡ��·��ڵ�
	bool			m_bMonster;
	bool			m_bCollection;
	bool			m_bRegion;
	bool			m_bRoads;	
	vector<long>	m_vecMonsterNo;				//��Ҫ������ʾ��MONSTER
	vector<long>	m_vecCollectionNo;			//��Ҫ������ʾ��Collection
	// ��ɫ�����
	enum { CL_NOBLOCK, CL_FLY, CL_NOFLY, CL_SWITCH, CL_ACTIVE_MONSTER, CL_INACTIVE_MONSTER, CL_ACTIVE_NPC, CL_INACTIVE_NPC, CL_MONSTER_DENSITY ,REGION_BLOCK };
	COLORREF		m_clDraw[16];				// ���Ƶ�ͼ������ɫ

public:
	static bool			m_bCtrl;					//Control�Ƿ���
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ItemChanged(DWORD_PTR uNo);
	void HitCurrItem(int n,string str1,string str2,string str3);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnAddItem();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	bool ShowJpgGif(CDC* pDC,CString strPath, int x, int y);
public:
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnVkadd();
	afx_msg void OnZoomout();
	afx_msg void OnZoomin();
	afx_msg void OnHScroll(UINT SBCode,UINT nPos,CScrollBar *pScrollBar);
	afx_msg void OnVScroll(UINT SBCode,UINT nPos,CScrollBar *pScrollBar);
	afx_msg void OnBnClickedRefresh();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

private:
	void RelativeCoor(CPoint* ptOri);
public:
	void CUniEditorDlg::ZoomCoor(CPoint* ptOri, bool bToPhysical = true);
	CNumList m_listNum;
	CNumList m_listColl;
	CListCtrl m_ScriptList;
	afx_msg void OnHdnItemclickNumlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickNumlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickNumlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickListcoll(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListcoll(NMHDR *pNMHDR, LRESULT *pResult);

	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);
};

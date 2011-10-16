//=============================================================================
/**
 *  file: TestScript.h
 *
 *  Brief:���Թ����ű��༭
 *
 *  Authtor:wangqiao
 *	
 *	Date:2007-7-25
 */
//=============================================================================

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

class TiXmlElement;

struct tagTestCase
{
	string strCaseName;				//������
	string strStartScriptName;		//��ʼ�ű���
	string strEndScriptName;		//�����ű���
};

class CTestScript : public CDialog
{
	DECLARE_DYNAMIC(CTestScript)

public:
	CTestScript(CWnd* pParent = NULL);
	virtual ~CTestScript();

	enum { IDD = IDD_PROPPAGE_TESTSCRIPT };
public:
	struct tagServerList
	{
		DWORD dwPort;
		string strIp;
		string name;
	};
	struct tagRegionList
	{
		int	type;
		long id;
		long ResourceID;
		long AreaId;
		int expScale;
		int OccuExpScale;
		int spScale;
		int TYPE;
		int	noPK;
		int	no;
		string name;
		int GS_ID;
		int Country;
		long RgnInfo;
		int waterRgn;
		string RgnGuid;
		int LinedIdFlag;
	};


	//GameTest���Ե�����ѡ��
	enum GTTESTTYPE		
	{
		TEST_SKILL,		//���Լ���ʹ��
		TEST_GOODS,		//������Ʒ
		TEST_PK,		//����PK
		TEST_MAIL,		//�����ʼ�
		TEST_COPYRGN,	//���Ը���
		TEST_SPRITE,	//���Ծ���
		TEST_LOGIN,		//���Ե�½������
		TEST_INCREAMENT,//������ֵ�̵�
		TEST_GUILD,		//�����л�
		TEST_TEAM,		//�������
		TEST_FRIEND,	//���Ժ���
		TEST_ALL,		//�������еĹ���
		TEST_TEMP,		//��ʱ�Ĳ��Խű�
	};
private:
	vector<tagServerList> m_vecServer;	//������������
	vector<tagRegionList> m_vecRegion;	//��Ϸ���������� by:LGR
	map<DWORD,DWORD> m_mapTestType;		//�������͵�����
	map<int,string>  m_mapOccu;			//��Ϸְҵ������ by:LGR
	map<int,long>  m_mapTestConfigRecord;  //��¼��һ�β��Ի�������

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg	void OnBnClickedButtonStartLogin();						//����¼

	afx_msg void OnCheckScriptSyntax();								//�﷨���
	afx_msg void OnCheckOpenScript();								//���ļ�
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCaseStartTest();									//��ʼ����ĳ������
	afx_msg void OnCaseStopTest();									//��������ĳ������
	afx_msg void OnCheckUpdateScript();								//�����ض��ű�
	afx_msg void OnCheckThirdOccu();
	afx_msg void OnCheckCloseGT();
	virtual BOOL OnInitDialog();
private:
	//	��ѡ �Ƿ�ʢ���¼��ʽ
	CButton m_btnChkPtToken;
	CTreeCtrl m_treeTestCase;
	long	m_lCaseNum;
	CEdit m_edScriptEdit;
	CEdit m_edScriptInfo;
	CStatic m_ctlShowScriptPath;
	long m_lStartCDKey;
	long m_lEndCDKey;
	CEdit m_edStartCDKey;
	CEdit m_edEndCDKey;
	CEdit m_edTimer;
	CEdit m_edPlayerLevel;
	//CString m_edPlayerRgn;
	CButton m_bnMaxState;
	CButton m_bnEquipWeapon;
	CButton m_bnThirdOccu;
	long m_lPlayerLevel;
	//long m_lPlayerRgn;
	long m_lPlayerX;
	long m_lPlayerY;
	CEdit m_edPlayerX;
	CEdit m_edPlayerY;

	CComboBox m_ctrlServerList;			//���������������������ComboBox����
	CComboBox m_ctrlTestType;			//��������͵�������������ComboBox����
	CComboBox m_ctrlRegionList;			//����Գ�����������������ComboBox����
	CComboBox m_ctrlOcculist;			//�����ְҵ��������������ComboBox����

	DWORD m_dwCurSelServer;					//��ǰ��ѡ��ķ����������б�ؼ�����ѡ�����
	DWORD m_dwCurSelTestType;				//��ǰ��ѡ��Ĳ������������б�ؼ�����ѡ�����

	CButton m_bnStartLogin;
	CButton m_bnChosProj;

	CString m_csCurrtWorkPath;
private:
	typedef map<long,tagTestCase*> MultiTestCases;
	typedef map<long,tagTestCase*>::iterator itTestCase;
	//typedef hash_map<string,tagTestCase*> MultiTestCases;
	//typedef hash_map<string,tagTestCase*>::iterator itTestCase;
	MultiTestCases	m_TestCases;

	//=====
//	bool bLoadLuaRequireFile;
	//=====
public:
	void LoadCase(TiXmlElement *pElement);
	void LoadCaseSetup();
	void InitTree();

	bool SaveTestConfigInfo(void);
	void OutputScriptInfo(const char* strInfo);
	afx_msg void OnNMRclickTreeTestCase(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSavescript();
									
	CString m_edScript;
	afx_msg void OnAddcase();									//���һ������
	//
	afx_msg void OnAddscript();									//���һ���ű�
	afx_msg void OnDelScript();									//ɾ��һ���ű�
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCaseCheckInCase();
	CString m_strOutputInfo;
	//CComboBox	m_comTestType;
	//������״�ؼ�
	afx_msg void OnTvnSelChangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	//ѡ����״�ؼ����ڵ� �����˵��ϵ� �������
	afx_msg void OnAddCase();



	//int m_TestType;							//��ǰ�Ĳ�������
public:
	tagServerList* GetServer()	{ return &m_vecServer[m_dwCurSelServer]; }
	//��ȡIP
	const char* GetIp()	{return GetServer()->strIp.c_str();}
	const tagServerList* GetServerIpAddrAndPort()	
	{
		if( !m_vecServer.empty() )
		{
			DWORD dwSel = m_ctrlServerList.GetCurSel();
			return &m_vecServer[dwSel];
		}
		return NULL;
	}
	//��ȡ��������TestType
	int GetTestType();

	BOOL InitServerList();
	BOOL InitRegionList();
	BOOL InitOccuList();
	BOOL InitProjList();
	bool InitTestType(void);
	bool InitTestConfigInfo(void);
	bool ReadTo(ifstream& inf,const char* flag);
	void OnCheckSetWorkPath();
	afx_msg void OnBnClickedButtonChangetesttype();


private:
	//�ñ����ǿؼ�����,��ֵΪ�����˷�����Ϣ��ʱ����
	long m_lTimer;
	//�ñ����ǿؼ�����,��ֵΪ�����˷��ͼ�����Ϣ��ʱ����
   // long m_lSendDelayTime;
public:
	//inline long GetDelayTime()	{ return m_lSendDelayTime; }
	inline long GetTimer(void)	{ return m_lTimer; }
	//inline bool GetSaveStart()	{ return m_bSaveStart; }	
	afx_msg void OnBnClickedCheckPtToken();
	

};

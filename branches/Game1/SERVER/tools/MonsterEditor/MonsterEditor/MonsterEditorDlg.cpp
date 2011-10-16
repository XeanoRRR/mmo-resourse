// MonsterEditorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MonsterEditor.h"
#include "MonsterEditorDlg.h"
#include "Detail.h"
#include "Recover.h"
#include "Part.h"
#include "Merger.h"
#define NEWRECORD 1			//�½���һ����¼
#define UPDATERECORD 2		//�޸���һ����¼
#define DELETERECORD 3		//ɾ����һ����¼
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
vector<tagMonster>vecMonster;//����ΪtagMonseter��vector����
vector<tagSkill>vecSkill;	 //����ΪtagSkill��vector����
string g_strfilename;			 //���������ļ���
string g_strfilenameskill;		 //���＼���ļ���
int ncount=0;		//�༭�ļ��ļ�¼��
bool bsaveflag=false;	//����Ƿ�༭�˼�¼
char g_buf[1024];//�������ʼ����ʱ��·��
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMonsterEditorDlg �Ի���




CMonsterEditorDlg::CMonsterEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonsterEditorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMonsterEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_MonTree);
	DDX_Control(pDX, IDC_LIST1, m_MonList);

	DDX_Control(pDX, IDC_LIST3, m_LogListBox);
}

BEGIN_MESSAGE_MAP(CMonsterEditorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CMonsterEditorDlg::OnNMDblclkList1)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CMonsterEditorDlg::OnNMDblclkTree1)
	ON_COMMAND(ID_NEW, &CMonsterEditorDlg::OnNew)
	ON_BN_CLICKED(IDC_BUTTON1, &CMonsterEditorDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMonsterEditorDlg::OnNMRclickList1)
	ON_COMMAND(ID_DELETE, &CMonsterEditorDlg::OnDelete)
	ON_COMMAND(ID_SAVE, &CMonsterEditorDlg::OnSaveToFile)
	ON_COMMAND(ID_RECOVER, &CMonsterEditorDlg::OnRecover)
	ON_COMMAND(ID_PART, &CMonsterEditorDlg::OnPartFile)
	ON_COMMAND(ID_MERGER, &CMonsterEditorDlg::OnMergerFile)
END_MESSAGE_MAP()


// CMonsterEditorDlg ��Ϣ�������

BOOL CMonsterEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_hAccelTable=::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MENU1));
	//����listcontrol��ʽ
	m_MonList.SetExtendedStyle(
		LVS_EX_FLATSB
		|LVS_EX_FULLROWSELECT
		|LVS_EX_HEADERDRAGDROP
		|LVS_EX_ONECLICKACTIVATE
		|LVS_EX_GRIDLINES
		);
	//������
	m_MonList.InsertColumn(0,"�Զ����",LVCFMT_LEFT,80,0);
	m_MonList.InsertColumn(1,"����",LVCFMT_LEFT,80,1);
	m_MonList.InsertColumn(2,"ԭʼ��",LVCFMT_LEFT,100,2);
	m_MonList.InsertColumn(3,"����",LVCFMT_LEFT,150,3);
	m_MonList.InsertColumn(4,"�Ƿ����赲",LVCFMT_LEFT,100,4);
	m_MonList.InsertColumn(5,"�Ƿ��޵�",LVCFMT_LEFT,100,5);
	m_MonList.InsertColumn(6,"��Ӧ�ɼ�����",LVCFMT_LEFT,100,6);
	m_MonList.InsertColumn(7,"��ν",LVCFMT_LEFT,100,7);
	m_MonList.InsertColumn(8,"��ν��ɫ",LVCFMT_LEFT,100,8);
	m_MonList.InsertColumn(9,"��νͼƬ",LVCFMT_LEFT,100,9);
	m_MonList.InsertColumn(10,"����id",LVCFMT_LEFT,100,10);
	m_MonList.InsertColumn(11,"NPC���",LVCFMT_LEFT,100,11);
	m_MonList.InsertColumn(12,"ͼ��id",LVCFMT_LEFT,100,12);
	m_MonList.InsertColumn(13,"ͼ�εȼ�",LVCFMT_LEFT,100,13);
	m_MonList.InsertColumn(14,"������ɫ���",LVCFMT_LEFT,100,14);
	m_MonList.InsertColumn(15,"�ȼ���ɫ���",LVCFMT_LEFT,100,15);
	m_MonList.InsertColumn(16,"Ѫ����ɫ",LVCFMT_LEFT,100,16);
	m_MonList.InsertColumn(17,"����id",LVCFMT_LEFT,100,17);
	m_MonList.InsertColumn(18,"�Ƿ��ѱ��",LVCFMT_LEFT,100,18);
	m_MonList.InsertColumn(19,"�����ѱ����",LVCFMT_LEFT,100,19);
	m_MonList.InsertColumn(20,"����",LVCFMT_LEFT,100,20);
	m_MonList.InsertColumn(21,"�ȼ�",LVCFMT_LEFT,100,21);
	m_MonList.InsertColumn(22,"����ֵ",LVCFMT_LEFT,100,22);
	m_MonList.InsertColumn(23,"ְҵ����",LVCFMT_LEFT,100,23);
	m_MonList.InsertColumn(24,"���HP",LVCFMT_LEFT,100,24);
	m_MonList.InsertColumn(25,"��С������",LVCFMT_LEFT,100,25);
	m_MonList.InsertColumn(26,"��󹥻���",LVCFMT_LEFT,100,26);
	m_MonList.InsertColumn(27,"��С��������",LVCFMT_LEFT,100,27);
	m_MonList.InsertColumn(28,"���������",LVCFMT_LEFT,100,28);
	m_MonList.InsertColumn(29,"��������",LVCFMT_LEFT,100,29);
	m_MonList.InsertColumn(30,"������",LVCFMT_LEFT,100,30);
	m_MonList.InsertColumn(31,"ħ��������",LVCFMT_LEFT,100,31);
	m_MonList.InsertColumn(32,"��",LVCFMT_LEFT,100,32);
	m_MonList.InsertColumn(33,"�м�",LVCFMT_LEFT,100,33);
	m_MonList.InsertColumn(34,"��������",LVCFMT_LEFT,100,34);
	m_MonList.InsertColumn(35,"�ƶ��ٶ�",LVCFMT_LEFT,100,35);
	m_MonList.InsertColumn(36,"ս���ƶ��ٶ�",LVCFMT_LEFT,100,36);
	m_MonList.InsertColumn(37,"��־����",LVCFMT_LEFT,100,37);
	m_MonList.InsertColumn(38,"������Ԫ�ؿ���",LVCFMT_LEFT,100,38);
	m_MonList.InsertColumn(39,"����ˮԪ�ؿ���",LVCFMT_LEFT,100,39);
	m_MonList.InsertColumn(40,"������Ԫ�ؿ���",LVCFMT_LEFT,100,40);
	m_MonList.InsertColumn(41,"������Ԫ�ؿ���",LVCFMT_LEFT,100,41);
	m_MonList.InsertColumn(42,"������ʥԪ�ؿ���",LVCFMT_LEFT,100,42);
	m_MonList.InsertColumn(43,"�����ڰ�Ԫ�ؿ���",LVCFMT_LEFT,100,43);
	m_MonList.InsertColumn(44,"�������ؿ���",LVCFMT_LEFT,100,44);
	m_MonList.InsertColumn(45,"�����ָ��ٶ�",LVCFMT_LEFT,100,45);
	m_MonList.InsertColumn(46,"ħ���ָ��ٶ�",LVCFMT_LEFT,100,46);
	m_MonList.InsertColumn(47,"���ʿ���",LVCFMT_LEFT,100,47);
	m_MonList.InsertColumn(48,"������Ѫ����",LVCFMT_LEFT,100,48);
	m_MonList.InsertColumn(49,"�������ο���",LVCFMT_LEFT,100,49);
	m_MonList.InsertColumn(50,"������Կ���",LVCFMT_LEFT,100,50);
	m_MonList.InsertColumn(51,"�������߿���",LVCFMT_LEFT,100,51);
	m_MonList.InsertColumn(52,"�����ж�����",LVCFMT_LEFT,100,52);
	m_MonList.InsertColumn(53,"����������",LVCFMT_LEFT,100,53);
	m_MonList.InsertColumn(54,"��Сͣ������",LVCFMT_LEFT,100,54);
	m_MonList.InsertColumn(55,"���ͣ������",LVCFMT_LEFT,100,55);
	m_MonList.InsertColumn(56,"��С���ߴ���",LVCFMT_LEFT,100,56);
	m_MonList.InsertColumn(57,"������ߴ���",LVCFMT_LEFT,100,57);
	m_MonList.InsertColumn(58,"���䷶Χ",LVCFMT_LEFT,100,58);
	m_MonList.InsertColumn(59,"׷����Χ",LVCFMT_LEFT,100,59);
	m_MonList.InsertColumn(60,"��ƽ�ƶ���Χ",LVCFMT_LEFT,100,60);
	m_MonList.InsertColumn(61,"AIģʽ",LVCFMT_LEFT,100,61);
	m_MonList.InsertColumn(62,"�ɷ�Ի�",LVCFMT_LEFT,100,62);
	m_MonList.InsertColumn(63,"��������",LVCFMT_LEFT,100,63);
	m_MonList.InsertColumn(64,"Ԫ������",LVCFMT_LEFT,100,64);
	m_MonList.InsertColumn(65,"ͣ����ʱ��",LVCFMT_LEFT,100,65);
	m_MonList.InsertColumn(66,"AI����ʽ",LVCFMT_LEFT,100,66);
	m_MonList.InsertColumn(67,"���������������˺�",LVCFMT_LEFT,120,67);
	m_MonList.InsertColumn(68,"��Ԫ�ع�������˺�",LVCFMT_LEFT,120,68);
	m_MonList.InsertColumn(69,"�������������˺�",LVCFMT_LEFT,120,69);
	m_MonList.InsertColumn(70,"�Զ����������˺�",LVCFMT_LEFT,120,70);
	m_MonList.InsertColumn(71,"�������������˺�",LVCFMT_LEFT,120,71);
	m_MonList.InsertColumn(72,"�Զ�ħ���������˺�",LVCFMT_LEFT,120,72);
	m_MonList.InsertColumn(73,"���������������˺�",LVCFMT_LEFT,120,73);
	m_MonList.InsertColumn(74,"�Ի�ϵ��������˺�",LVCFMT_LEFT,120,74);
	m_MonList.InsertColumn(75,"��ˮϵ��������˺�",LVCFMT_LEFT,120,75);
	m_MonList.InsertColumn(76,"����ϵ��������˺�",LVCFMT_LEFT,120,76);
	m_MonList.InsertColumn(77,"�Է�ϵ��������˺�",LVCFMT_LEFT,120,77);
	m_MonList.InsertColumn(78,"����ʥϵ��������˺�",LVCFMT_LEFT,120,78);
	m_MonList.InsertColumn(79,"�Ժڰ�ϵ�й�������˺�",LVCFMT_LEFT,120,79);
	m_MonList.InsertColumn(80,"����Ԫ��ϵ�й�������˺�",LVCFMT_LEFT,130,80);
	m_MonList.InsertColumn(81,"����",LVCFMT_LEFT,100,81);
	m_MonList.InsertColumn(82,"��������",LVCFMT_LEFT,100,82);
	m_MonList.InsertColumn(83,"�������˺�",LVCFMT_LEFT,100,83);
	m_MonList.InsertColumn(84,"����������",LVCFMT_LEFT,100,84);
	m_MonList.InsertColumn(85,"ħ��������",LVCFMT_LEFT,100,85);
	m_MonList.InsertColumn(86,"ħ�������˺�",LVCFMT_LEFT,100,86);
	m_MonList.InsertColumn(87,"ħ����������",LVCFMT_LEFT,100,87);
	m_MonList.InsertColumn(88,"���˱���",LVCFMT_LEFT,100,88);
	m_MonList.InsertColumn(89,"����ʱ��",LVCFMT_LEFT,100,89);
	m_MonList.InsertColumn(90,"���ܵ���",LVCFMT_LEFT,100,90);
	m_MonList.InsertColumn(91,"���ܸ���",LVCFMT_LEFT,100,91);
	m_MonList.InsertColumn(92,"���ܼ��",LVCFMT_LEFT,100,92);
	m_MonList.InsertColumn(93,"�Ƿ�ɻ�",LVCFMT_LEFT,100,93);
	m_MonList.InsertColumn(94,"�Ƿ���ȫ�ָ�",LVCFMT_LEFT,100,94);
	m_MonList.InsertColumn(95,"��λ����",LVCFMT_LEFT,100,95);
	m_MonList.InsertColumn(96,"׷��ʱ��",LVCFMT_LEFT,100,96);
	m_MonList.InsertColumn(97,"ai�ο�1",LVCFMT_LEFT,100,97);
	m_MonList.InsertColumn(98,"ai�ο�2",LVCFMT_LEFT,100,98);
	m_MonList.InsertColumn(99,"ai�ο�3",LVCFMT_LEFT,100,99);
	m_MonList.InsertColumn(100,"ai�ο�4",LVCFMT_LEFT,100,100);
	m_MonList.InsertColumn(101,"ai�ο�5",LVCFMT_LEFT,100,101);
	m_MonList.InsertColumn(102,"�����ű�",LVCFMT_LEFT,100,102);
	m_MonList.InsertColumn(103,"ս���ű�",LVCFMT_LEFT,100,103);
	m_MonList.InsertColumn(104,"���˽ű�",LVCFMT_LEFT,100,104);
	m_MonList.InsertColumn(105,"�����ű�",LVCFMT_LEFT,100,105);
	m_MonList.InsertColumn(106,"���ڽű�",LVCFMT_LEFT,100,106);
	m_MonList.InsertColumn(107,"ѡ���ܽű�",LVCFMT_LEFT,100,107);
	m_MonList.InsertColumn(108,"Ѱ��Ŀ��ű�",LVCFMT_LEFT,100,108);
	m_MonList.InsertColumn(109,"ɱ��Ŀ��ű�",LVCFMT_LEFT,100,109);
	
	ReadFile();
	MonList();
	MonTree();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMonsterEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMonsterEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CMonsterEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//������ƶ�����*��֮��
int ReadTo(ifstream &stream,string name)
{
		string str;
	stream >> str;
	while (str != name)
	{
		//����ѵ��ļ�ĩβ������0
		if (stream.eof())
		{

			return 0;
		}
		//��ȡ��һ����
		stream >> str;
	}
	return 1;
}

//��������ת��Ϊ�ַ���
CString ftoa(float f)
{
 
	stringstream ss; 
	ss << f; 
	return  ss.str( ).c_str( ); 

}
//DOWRDת��ΪCString

CString DwordToString(DWORD n)
{

	CString str;

	str.Format("%d",n);
	return str;
}

/*���ܣ���ȡϵͳʱ��
*
*/
string GetTime()
{
	//����һ��ʱ��Ľṹ��
	SYSTEMTIME st;
	CString strDate,strTime;
	//��ȡϵͳʱ��
	GetLocalTime(&st); 
	strDate.Format("%d-%d-%d",st.wYear,st.wMonth,st.wDay);
	strTime.Format("%d-%d-%d",st.wHour,st.wMinute,st.wSecond);
	string str;
	str=strDate+"_"+strTime;
	return str;
}
/*���ܣ���ȡ�ļ�
*
*/
void CMonsterEditorDlg::ReadFile()
{
	ifstream file;
	string str1,strflag;
	//���������ļ���·��
	g_strfilename="monsterlist.ini";
	//���＼���ļ���·��
	g_strfilenameskill="monsterskilllist.ini";
	file.open(g_strfilename.c_str());
	if(!file.is_open())
	{
		MessageBox("���ļ�ʧ��");
		return;
	}

	::GetCurrentDirectory(1000,g_buf);
	g_buf[strlen(g_buf)]='\0';
	tagMonster monster;
	tagSkill skill;
	int nNum=1;
	int i=0;
	//���ļ��ж�ȡ���ݵ�vector��
	while(ReadTo(file, "*")!=0)
	{
		monster.nNO=nNum;
		file>> monster.dwIndex
			>>monster.strOrigName
			>>monster.strName
			>>monster.strBlock
			>>monster.strSuper
			>>monster.wDropFlag
			>>monster.strCollection
			>>monster.strTitle
			>>monster.dwTitleColor
			>>monster.dwTitlePicture
			>>monster.strCountryID
			>>monster.strNpcSort
			>>monster.wBossLevel
			>>monster.dwPicID
			>>monster.dwPicLevel
			>>monster.dwNameColorID
			>>monster.dwLevelColorID
			>>monster.dwHpColor
			>>monster.dwSoundID
			>>monster.strTame
			>>monster.dwTameTimes
			>>monster.strShape
			>>monster.nLevel
			>>monster.dwExp
			>>monster.dwWorkPoint
			>>monster.dwMaxHp
			>>monster.wMinAttack
			>>monster.wMaxAttack
			>>monster.wMinMagicAttack
			>>monster.wMaxMagicAttack
			>>monster.wHitRating
			>>monster.wDefense
			>>monster.wMagicDefense
			>>monster.wBlock
			>>monster.wImmunity
			>>monster.wPierce
			>>monster.wParry
			>>monster.wDodge
			>>monster.fMoveSpd
			>>monster.fBatMoveSpd
			>>monster.fTurningSpeed
			>>monster.wWillAnti
			>>monster.wGroundAnti
			>>monster.wWaterAnti
			>>monster.wFireAnti
			>>monster.wWindAnti
			>>monster.wSacreAnti
			>>monster.wDarkAnti
			>>monster.wNotElemAnti
			>>monster.wHpRecoverSpd
			>>monster.wMpRecoverSpd
			>>monster.wBodyAnti
			>>monster.wBloodAnti
			>>monster.wDuskAnti
			>>monster.wNumbAnti
			>>monster.wSleepAnti
			>>monster.wDrugAnti
			>>monster.wNotMoveAnti
			>>monster.wSilenceAnti
			>>monster.wMinStayTimes
			>>monster.wMaxStayTimes
			>>monster.wMinWalkTimes
			>>monster.wMaxWalkTimes
			>>monster.wAlertRange
			>>monster.wPurRange
			>>monster.wPeaMoveRange
			>>monster.strAIModel
			>>monster.bTalk
			>>monster.strMonRace
			>>monster.strElem
			>>monster.wStayTime
			>>monster.strAIWay
			>>monster.wHurtHuman
			>>monster.wHurtElem
			>>monster.wHurtDiff
			>>monster.wHurtAnimal
			>>monster.wHurtDead
			>>monster.wHurtDevil
			>>monster.wHurtInsect
			>>monster.wHurtFire
			>>monster.wHurtWater
			>>monster.wHurtGround
			>>monster.wHurtWind
			>>monster.wHurtSacre
			>>monster.wHurtDark
			>>monster.wHurtNotElem
			>>monster.wVitality
			>>monster.wStormHit
			>>monster.wStormHurt
			>>monster.wStormAnti
			>>monster.wMagicStorm
			>>monster.wMagicStormHurt  
			>>monster.wMagicStormAnti
			>>monster.fInjurRate
			>>monster.wAlertTime
			>>monster.fEscape
			>>monster.wSkillRate
			>>monster.wSkillInter
			>>monster.strReturn
			>>monster.strAllRecover
			>>monster.wReDistance
			>>monster.wReTime
			>>monster.strRefe1
			>>monster.strRefe2
			>>monster.strRefe3
			>>monster.strRefe4
			>>monster.strRefe5
			>>monster.strBirthSct
			>>monster.strBattleSct
			>>monster.strInjurSct
			>>monster.strDeadSct
			>>monster.strCycleSct
			>>monster.strChoseSkillSct
			>>monster.strFindTgtSct
			>>monster.strKillTgtSct;
			
			if (monster.strName == "����Ϻ�")
			{
				int i = 0;
			}
			TRACE(monster.strName.c_str());
			TRACE("\n");
			//��monsterskilllist.iniѰ���Ƿ�����Ӧ�ļ��ܼ�¼���������ȡ
			//ifstream fileskill(g_strfilenameskill.c_str());
		//����ļ�û�д򿪣��򷵻�
		//if(!fileskill.is_open())
		//{
		//	MessageBox("�򿪼����ļ�ʧ��");
		//	return;
		//
		//}//�����Ϊ0������Ӧ�ļ��ܼ�¼
		//if(ReadTo(fileskill,monster.strOrigName)!=0)
		//{
		//	fileskill>>strflag;
		//	//�����ּ��ܣ���>������#������@������ͬ����ļ���
		//	while(strflag==">"||strflag=="#"||strflag=="@")		
		//	{
		//		//">"���ļ����Ǳ�ʾ��������
		//		if(strflag==">")
		//		{
		//			strflag="";
		//			skill.strSkillType=">";
		//			fileskill>>skill.dwSkillID
		//				>>skill.wSkillLevel
		//				>>strflag;
		//			//��������û�м��ܼ��������Ϊ0
		//			skill.wInterTime=0;
		//			monster.vecSkill.push_back(skill);

		//		}
		//		//"#"���ļ��б�ʾ��ͨ����
		//		else if(strflag=="#")
		//		{
		//			strflag="";
		//			skill.strSkillType="#";
		//			fileskill>>skill.dwSkillID
		//				>>skill.wSkillLevel
		//				>>strflag;
		//			//��ͨ����û�м��ܼ��������Ϊ0
		//			skill.wInterTime=0;
		//			monster.vecSkill.push_back(skill);
		//		}
		//		//"@"���ļ��б�ʾ���ڼ���
		//		else if(strflag=="@")
		//		{
		//			strflag="";
		//			skill.strSkillType="@";
		//			fileskill>>skill.dwSkillID
		//				>>skill.wSkillLevel
		//				>>skill.wInterTime
		//				>>strflag;
		//			//������skill���뵽vecSkill��
		//			monster.vecSkill.push_back(skill);
		//		}
		//		//"*"��ʾ��һ����¼�Ŀ�ʼ�����Է���
		//		else if(strflag=="*")
		//		{
		//			break;
		//		}
		//	}
		//	
		//	
		//}
		//fileskill.close();
		//����vector��
		nNum+=1;		
		vecMonster.push_back(monster);
		//��ռ���������׼��װ��һ��monster�ļ���
		monster.vecSkill.clear();
	}
	file.close();
}
/*���ܣ�����listcontrol�е�����
*
*/
void CMonsterEditorDlg::ListSetData(vector<tagMonster>::iterator iter,int i)
{
	char strVal[256];				
		//itoa(iter->nNO,strVal,10);
		itoa(iter->nNO,strVal,256);

		m_MonList.InsertItem(i,"");

		m_MonList.SetItemText(i,0,CString(itoa(iter->nNO,strVal,10)));

		m_MonList.SetItemText(i,1,DwordToString(iter->dwIndex));
		m_MonList.SetItemText(i,2,(CString)(iter->strOrigName).c_str());
		m_MonList.SetItemText(i,3,(CString)(iter->strName).c_str());

		m_MonList.SetItemText(i,4,(CString)(iter->strBlock).c_str());
		m_MonList.SetItemText(i,5,(CString)(iter->strSuper).c_str());
		
		m_MonList.SetItemText(i,6,(CString)(iter->strCollection).c_str());
		m_MonList.SetItemText(i,7,(CString)(iter->strTitle).c_str());
		m_MonList.SetItemText(i,8,DwordToString(iter->dwTitleColor));
		m_MonList.SetItemText(i,9,DwordToString(iter->dwTitlePicture));
		m_MonList.SetItemText(i,10,(CString)(iter->strCountryID).c_str());
		m_MonList.SetItemText(i,11,(CString)(iter->strNpcSort).c_str());
		m_MonList.SetItemText(i,12,DwordToString(iter->dwPicID));
		m_MonList.SetItemText(i,13,DwordToString(iter->dwPicLevel));
		m_MonList.SetItemText(i,14,DwordToString(iter->dwNameColorID));
		m_MonList.SetItemText(i,15,DwordToString(iter->dwLevelColorID));
		m_MonList.SetItemText(i,16,DwordToString(iter->dwHpColor));
		m_MonList.SetItemText(i,17,DwordToString(iter->dwSoundID));
		
		
		m_MonList.SetItemText(i,18,(CString)(iter->strTame).c_str());
		m_MonList.SetItemText(i,19,DwordToString(iter->dwTameTimes));
		m_MonList.SetItemText(i,20,(CString)(iter->strShape).c_str());

		m_MonList.SetItemText(i,21,(CString)(itoa(iter->nLevel,strVal,10)));
		m_MonList.SetItemText(i,22,DwordToString(iter->dwExp));
		m_MonList.SetItemText(i,23,DwordToString(iter->dwWorkPoint));
		m_MonList.SetItemText(i,24,DwordToString(iter->dwMaxHp));
		m_MonList.SetItemText(i,25,DwordToString(iter->wMinAttack));
		m_MonList.SetItemText(i,26,DwordToString(iter->wMaxAttack));
		m_MonList.SetItemText(i,27,DwordToString(iter->wMinMagicAttack));
		m_MonList.SetItemText(i,28,DwordToString(iter->wMaxMagicAttack));
		m_MonList.SetItemText(i,29,DwordToString(iter->wHitRating));
		m_MonList.SetItemText(i,30,DwordToString(iter->wDefense));
		m_MonList.SetItemText(i,31,DwordToString(iter->wMagicDefense));
		m_MonList.SetItemText(i,32,DwordToString(iter->wBlock));
		m_MonList.SetItemText(i,33,DwordToString(iter->wParry));

		m_MonList.SetItemText(i,34,DwordToString(iter->wDodge));
		m_MonList.SetItemText(i,35,ftoa(iter->fMoveSpd));
		m_MonList.SetItemText(i,36,ftoa(iter->fBatMoveSpd));
		m_MonList.SetItemText(i,37,DwordToString(iter->wWillAnti));
		m_MonList.SetItemText(i,38,DwordToString(iter->wGroundAnti));
		m_MonList.SetItemText(i,39,DwordToString(iter->wWaterAnti));
		m_MonList.SetItemText(i,40,DwordToString(iter->wFireAnti));
		m_MonList.SetItemText(i,41,DwordToString(iter->wWindAnti));
		m_MonList.SetItemText(i,42,DwordToString(iter->wSacreAnti));
		m_MonList.SetItemText(i,43,DwordToString(iter->wDarkAnti));
		m_MonList.SetItemText(i,44,DwordToString(iter->wNotElemAnti));
		m_MonList.SetItemText(i,45,DwordToString(iter->wHpRecoverSpd));
		m_MonList.SetItemText(i,46,DwordToString(iter->wMpRecoverSpd));
		m_MonList.SetItemText(i,47,DwordToString(iter->wBodyAnti));

		m_MonList.SetItemText(i,48,DwordToString(iter->wBloodAnti));
		m_MonList.SetItemText(i,49,DwordToString(iter->wDuskAnti));
		m_MonList.SetItemText(i,50,DwordToString(iter->wNumbAnti));
		m_MonList.SetItemText(i,51,DwordToString(iter->wSleepAnti));
		m_MonList.SetItemText(i,52,DwordToString(iter->wDrugAnti));
		m_MonList.SetItemText(i,53,DwordToString(iter->wNotMoveAnti));
		m_MonList.SetItemText(i,54,DwordToString(iter->wMinStayTimes));
		m_MonList.SetItemText(i,55,DwordToString(iter->wMaxStayTimes));
		m_MonList.SetItemText(i,56,DwordToString(iter->wMinWalkTimes));
		m_MonList.SetItemText(i,57,DwordToString(iter->wMaxWalkTimes));
		m_MonList.SetItemText(i,58,DwordToString(iter->wAlertRange));
		m_MonList.SetItemText(i,59,DwordToString(iter->wPurRange));
		m_MonList.SetItemText(i,60,DwordToString(iter->wPeaMoveRange));
		m_MonList.SetItemText(i,61,(CString)(iter->strAIModel).c_str());
		m_MonList.SetItemText(i,62,(CString)(itoa(iter->bTalk,strVal,10)));
		m_MonList.SetItemText(i,63,(CString)(iter->strMonRace).c_str());
		m_MonList.SetItemText(i,64,(CString)(iter->strElem).c_str());
		m_MonList.SetItemText(i,65,DwordToString(iter->wStayTime));
		m_MonList.SetItemText(i,66,(CString)(iter->strAIWay).c_str());

		m_MonList.SetItemText(i,67,DwordToString(iter->wHurtHuman));
		m_MonList.SetItemText(i,68,DwordToString(iter->wHurtElem));
		m_MonList.SetItemText(i,69,DwordToString(iter->wHurtDiff));
		m_MonList.SetItemText(i,70,DwordToString(iter->wHurtAnimal));
		m_MonList.SetItemText(i,71,DwordToString(iter->wHurtDead));
		m_MonList.SetItemText(i,72,DwordToString(iter->wHurtDevil));
		m_MonList.SetItemText(i,73,DwordToString(iter->wHurtInsect));
		m_MonList.SetItemText(i,74,DwordToString(iter->wHurtFire));
		m_MonList.SetItemText(i,75,DwordToString(iter->wHurtWater));
		m_MonList.SetItemText(i,76,DwordToString(iter->wHurtGround));
		m_MonList.SetItemText(i,77,DwordToString(iter->wHurtWind));
		m_MonList.SetItemText(i,78,DwordToString(iter->wHurtSacre));
		m_MonList.SetItemText(i,79,DwordToString(iter->wHurtDark));
		m_MonList.SetItemText(i,80,DwordToString(iter->wHurtNotElem));
		m_MonList.SetItemText(i,81,DwordToString(iter->wVitality));
		m_MonList.SetItemText(i,82,DwordToString(iter->wStormHit));

		m_MonList.SetItemText(i,83,DwordToString(iter->wStormHurt));
		m_MonList.SetItemText(i,84,DwordToString(iter->wStormAnti));
		m_MonList.SetItemText(i,85,DwordToString(iter->wMagicStorm));
		m_MonList.SetItemText(i,86,DwordToString(iter->wMagicStormHurt));
		m_MonList.SetItemText(i,87,DwordToString(iter->wMagicStormAnti));

		m_MonList.SetItemText(i,88,ftoa(iter->fInjurRate));
		m_MonList.SetItemText(i,89,DwordToString(iter->wAlertTime));
		m_MonList.SetItemText(i,90,ftoa(iter->fEscape));
		m_MonList.SetItemText(i,91,DwordToString(iter->wSkillRate));
		m_MonList.SetItemText(i,92,DwordToString(iter->wSkillInter));
		
		m_MonList.SetItemText(i,93,(CString)(iter->strReturn).c_str());
		
		m_MonList.SetItemText(i,94,(CString)(iter->strAllRecover).c_str());
		m_MonList.SetItemText(i,95,DwordToString(iter->wReDistance));
		m_MonList.SetItemText(i,96,DwordToString(iter->wReTime));
		
		m_MonList.SetItemText(i,97,(CString)(iter->strRefe1).c_str());
		m_MonList.SetItemText(i,98,(CString)(iter->strRefe2).c_str());
		m_MonList.SetItemText(i,99,(CString)(iter->strRefe3).c_str());
		m_MonList.SetItemText(i,100,(CString)(iter->strRefe4).c_str());
		m_MonList.SetItemText(i,101,(CString)(iter->strRefe5).c_str());
		m_MonList.SetItemText(i,102,(CString)(iter->strBirthSct).c_str());
		m_MonList.SetItemText(i,103,(CString)(iter->strBattleSct).c_str());
		m_MonList.SetItemText(i,104,(CString)(iter->strInjurSct).c_str());
		m_MonList.SetItemText(i,105,(CString)(iter->strDeadSct).c_str());
		m_MonList.SetItemText(i,106,(CString)(iter->strCycleSct).c_str());
		m_MonList.SetItemText(i,107,(CString)(iter->strChoseSkillSct).c_str());
		m_MonList.SetItemText(i,108,(CString)(iter->strFindTgtSct).c_str());
		m_MonList.SetItemText(i,109,(CString)(iter->strKillTgtSct).c_str());
		
}
/*���ܣ���ʾ�ļ���listcontrol
*
*/
void CMonsterEditorDlg::MonList()
{
	vector<tagMonster>::iterator iter;
	int i=0;
	//��ȡȫ����¼
	for(iter=vecMonster.begin();iter!=vecMonster.end();iter++,i++)
	{
		
		ListSetData(iter,i);
			
	}
	
}
void CMonsterEditorDlg::MonTree()
{

	HTREEITEM m_Root;
	HTREEITEM m_Child;
	m_Root = m_MonTree.InsertItem("���й����¼",0,0);
	//m_hTreeRoot=m_MonTree.InsertItem(&childItem);
	int i=0;
	vector<tagMonster>::iterator iter;
	//��ȡȫ����¼
	for(iter=vecMonster.begin();iter!=vecMonster.end();iter++,i++)
	{
		//���ýڵ��ļ�
		m_Child = m_MonTree.InsertItem(iter->strOrigName.c_str(),1,1,m_Root);
	} 	
	//չ���ڵ�
	m_MonTree.Expand(m_Root,TVE_EXPAND); 
}
/*���ܣ�˫��listcontrol�еļ�¼����ʾ��ϸ�Ի���
*
*/
void CMonsterEditorDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	POSITION pos = m_MonList.GetFirstSelectedItemPosition();
      if (pos == NULL)
	  {
		   return;
	  }
      else
      {
		  string strVal;
		  //��ȡ��
		  int nItem = m_MonList.GetNextSelectedItem(pos);
		  //��ȡָ���к��е�����
		  strVal=m_MonList.GetItemText(nItem,2);
		  vector<tagMonster>::iterator iter;
		  //��vecMonster����ԭʼ����strVal��ȵ�Ԫ��
		  for(iter=vecMonster.begin();iter!=vecMonster.end();iter++)
		  {
			  //����ҵ���Ԫ�أ�������Ӧ�����
			  if(iter->strOrigName==strVal.c_str())
			  {
					HTREEITEM firstItem;
					firstItem=m_MonTree.GetNextItem(m_MonTree.GetRootItem(),TVGN_CHILD);
					//����㲻Ϊ��ʱѭ��
					while(firstItem!=NULL)
					{
						//�ҵ�����㣬�������
						if(m_MonTree.GetItemText(firstItem).Compare(strVal.c_str())==0)
						{
							m_MonTree.SetFocus();
							m_MonTree.SelectItem(firstItem);
							
							break;
						}
						firstItem=m_MonTree.GetNextItem(firstItem,TVGN_NEXT);
					}
					//����һ��CDetail����������ʾ��ϸ�Ի���
				 	CDetail dlg;
					//����ʾ��ϸ�Ի����е�ֵ
					dlg.SetData(&vecMonster,iter);
					dlg.DoModal();
					//�ı����ڵ���ı�
					m_MonTree.SetItemText(firstItem,iter->strOrigName.c_str());
					//ɾ��listcontrol�е�ԭ��¼
					m_MonList.DeleteItem(nItem);
					//�����¼�¼
					ListSetData(iter,nItem);
					//���޸���־��ʾ��listbox	
					LogList(iter,2);
					
				    break;

			  }

		  }
	  }
}
/*���ܣ�˫�����м�¼����ʾ��ϸ�Ի���
*
*/
void CMonsterEditorDlg::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nItem=0;
	HTREEITEM selectedItem=m_MonTree.GetSelectedItem();
	vector<tagMonster>::iterator iter;
	//��˫�����ؼ��еļ�¼ʱ�����Ҳ���ʾ�뱻�����¼�������ϸ�Ի���
	for(iter=vecMonster.begin();iter!=vecMonster.end();iter++,nItem++)
	{	
		//�ҵ�������ļ�¼
		if(m_MonTree.GetItemText(selectedItem).Compare(iter->strOrigName.c_str())==0)
		{
			//����һ��CDetail����������ʾ��ϸ�Ի���
			CDetail dlgTree;
			//����ֵ
			dlgTree.SetData(&vecMonster,iter);
			dlgTree.DoModal();
			//ɾ��listcontrolԭ��¼
			m_MonList.DeleteItem(nItem);
			//���²����в���ʾ��¼
			ListSetData(iter,nItem);
			//�������ؼ��ڵ���ļ�
			m_MonTree.SetItemText(selectedItem,iter->strOrigName.c_str());
			//���޸���־��ʾ��listbox
			LogList(iter,2);
			//��listcontrol����Ϊ����
			m_MonList.SetFocus();
			m_MonList.SetItemState(nItem,LVIS_SELECTED,LVIS_SELECTED);
			//��listcontrol�й�����ʾ����Ӧ��¼
			m_MonList.EnsureVisible(nItem,FALSE);
			
			break;
		}
	}
}
//�½���¼
void CMonsterEditorDlg::OnNew()
{
	// TODO: �ڴ���������������
	CDetail dlgnew;
	//����vecMonsterԭ���Ĵ�С
	int nprimer=(int)vecMonster.size();
	//����Ĭ��ֵ	
	dlgnew.SetNewData(&vecMonster,nprimer);
	dlgnew.DoModal();

	

	vector<tagMonster>::iterator iter=vecMonster.end()-1;
	int n=(int)vecMonster.size();
	//���ڵĳ��ȱ�ԭ���ĳ��ȴ󣬱�ʾ�½���¼�ɹ�
	if(n>nprimer)
	{
		//����listcontrol�е�ֵ
		ListSetData(iter,n-1);
		m_MonList.SetFocus();
		//��ȡ�������ѱ������ļ�¼
		for(int j=0;j<m_MonList.GetItemCount();j++)
		m_MonList.SetItemState(j, 0, LVIS_SELECTED|LVIS_FOCUSED);
		//���½��ļ�¼����
		m_MonList.SetItemState(m_MonList.GetItemCount()-1,LVIS_SELECTED,LVIS_SELECTED);
		m_MonList.EnsureVisible(m_MonList.GetItemCount()-1,FALSE);
		//��������ʾ�½���¼
		HTREEITEM newItem;
		TV_INSERTSTRUCT stItem;
		//���ڵ�
		stItem.hParent=m_MonTree.GetRootItem();
		//�����½ڵ�
		char strVal[100];
		sprintf_s(strVal,"ԭʼ��%s",m_MonList.GetItemText(m_MonList.GetItemCount()-1,1));
		stItem.hInsertAfter=TVI_LAST;
		stItem.item.pszText=strVal;
		stItem.item.mask=TVIF_TEXT| TVIF_PARAM;
		newItem=m_MonTree.InsertItem(&stItem);
		m_MonTree.SelectItem(newItem);
		//���༭��־��ʾ��listbox
		LogList(iter,1);
	}
}
//ˢ�°�ť
void CMonsterEditorDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_MonList.DeleteAllItems();
	m_MonTree.DeleteAllItems();
	MonList();
	MonTree();

}
/*���ܣ����༭��־��ʾ��listbox��
*ժҪ��nflagΪ1Ϊ��Ӽ�¼��2Ϊ�޸ļ�¼��3Ϊɾ����¼
*/
void CMonsterEditorDlg::LogList(vector<tagMonster>::iterator iter,int nflag)
{
	char strVal[100];
	
	bsaveflag=true;
	string str;
	//��ȡʱ��
	str=GetTime();
	switch(nflag)
	{
		//��nflagΪNEWRECORDʱ����ʾ�½���һ����¼
		case NEWRECORD:
			str="> "+str+"       ����˼�¼����"+"�����ţ���"+itoa(iter->dwIndex,strVal,10)+
				" ��ԭʼ������"+iter->strOrigName;
			break;
		//��nflagΪAPPENDRECORDʱ����ʾ�޸���һ����¼
		case UPDATERECORD:
			str="> "+str+"       �༭�˼�¼����"+"�����ţ���"+itoa(iter->dwIndex,strVal,10)+
				"�� ԭʼ������"+iter->strOrigName;
			break;
		//��nflagΪDELETERECORDʱ����ʾɾ����һ����¼
		case DELETERECORD:
			str="> "+str+"       ɾ���˼�¼����"+"�����ţ���"+itoa(iter->dwIndex,strVal,10)+
				"�� ԭʼ������"+iter->strOrigName;
	}	
	//׷�ӱ༭��־
	m_LogListBox.AddString(str.c_str());
	
	//�����ļ�ָ��
	FILE *fp;
	fp=fopen("log.txt","a");
	//׷����־���ļ�
	fprintf(fp,str.c_str());
	fprintf(fp,"\n");
	fclose(fp);
	//ncount�Ǽ�¼�޸��ļ�����������Ϊ5ʱ�����ļ����ݣ�Ȼ������Ϊ0
	ncount+=1;
	if(ncount==5)
	{
		ncount=0;
		//��ȡϵͳʱ��
		string str1=GetTime();
		string filename,filenameskill;
		//�����ļ���
		filename="monsterlist_backup/"+str1+"_backup.txt";
		filenameskill="monserskilllist_backup/"+str1+"_backupskill.txt";
		//�����ļ�
		SaveFile(filename.c_str(),filenameskill.c_str());
	}
}
//��listcontrol���һ�������ɾ���˵�
void CMonsterEditorDlg::OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu menu;
	//��ȡ��ǰ���λ��
	CPoint pt;
	//����ɾ���˵�
	menu.LoadMenu(IDR_MENU2);
	CMenu* pMenu=menu.GetSubMenu(0);
	GetCursorPos(&pt);
	//����ɾ���˵�
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x,pt.y, this);
}
//ɾ����¼
void CMonsterEditorDlg::OnDelete()
{
	// TODO: �ڴ���������������
	POSITION pos = m_MonList.GetFirstSelectedItemPosition();
      if (pos == NULL)
	  {
		   return;
	  }
	  //��ȡ��ǰѡ�е���
	  int nItem=m_MonList.GetNextSelectedItem(pos);
	  vector<tagMonster>::iterator iterdelete;
	  int i=0;
	  string str;
	  str=m_MonList.GetItemText(nItem,2);
	  //��vecMonster�в��ҵ����ļ�¼������ɾ��
	  
	  for(iterdelete=vecMonster.begin();iterdelete!=vecMonster.end();iterdelete++,i++)
	  {
		  //�ҵ���¼������ɾ��
		  if(iterdelete->strOrigName==str)
		  {
			  //���༭��¼��־��ʾ��listbox
			  LogList(iterdelete,3);
			  //��listcontrol��ɾ����¼
			  m_MonList.DeleteItem(nItem);
			  //��vecMonster��ɾ����¼
			  
			  //ɾ������Ӧ��¼
			  HTREEITEM firstItem;
			  firstItem=m_MonTree.GetNextItem(m_MonTree.GetRootItem(),TVGN_CHILD);
			  while(firstItem!=NULL)
			  {
				  //�ҵ��ӽڵ�
				  if(m_MonTree.GetItemText(firstItem).Compare(str.c_str())==0)
 				  {
					  m_MonTree.DeleteItem(firstItem);
					  break;
				  }
				  //ָ����һ���ڵ�
				  firstItem=m_MonTree.GetNextItem(firstItem,TVGN_NEXT);
			  }
			  vecMonster.erase(iterdelete);
			  break;
		  }
	  }	 
}
/*���ܣ������ļ�
*
*/
void CMonsterEditorDlg::SaveFile(const char *filename,const char *filenameskill)
{
	//�򿪹��������ļ�
	ofstream file(filename);
	//�����ʧ�ܣ��򷵻�
	if(!file.is_open())
	{
		MessageBox("�޷�д�ļ�,�ļ����Կ���Ϊֻ��,����!");
		return;
	}
	//ָ������¼
	vector<tagMonster>::iterator iter;
	//ָ������¼�ļ��ܼ�¼
	//vector<tagSkill>::iterator iterskill;
	int nItem=0;
	file<<"	����	ԭʼ��	����	�Ƿ����赲	�Ƿ��޵�	������־��0��1��	��Ӧ�ɼ�����	��ν	��ν��ɫ	��νͼƬ	����id	npc���	boss���	ͼ��id	ͼ�εȼ�	������ɫ���	�ȼ���ɫ���	Ѫ����ɫ	����id	�Ƿ��ѱ����byte��	�����ѱ������	����	�ȼ�	����ֵ	ְҵ����	���hp	��С������	��󹥻���	��С��������	���������	��������	������	ħ��������	��	�м�	����	��͸	��������	�ƶ��ٶ�	ս���ƶ��ٶ�	ת���ٶ�	��־����	������Ԫ�ؿ���	����ˮԪ�ؿ���	������Ԫ�ؿ���	������Ԫ�ؿ���	������ʥԪ�ؿ���	�����ڰ�Ԫ�ؿ���	�������ؿ���	�����ָ��ٶ�	ħ���ָ��ٶ�	���ʿ���	������Ѫ����	�������ο���	������Կ���	�������߿���	�����ж�����	����������	������Ĭ����	��Сͣ������	���ͣ������	��С���ߴ���	������ߴ���	���䷶Χ	׷����Χ	��ƽ�ƶ���Χ	AIģʽ	�ɷ�Ի�	��������	Ԫ������	ͣ����ʱ��	AI����ʽ	���������������˺�	��Ԫ�ع�������˺�	�������������˺�	�Զ����������˺�	�������������˺�	�Զ�ħ���������˺���	���������������˺�	�Ի�ϵ��������˺�	��ˮϵ��������˺�	����ϵ��������˺�	�Է�ϵ��������˺�	����ʥϵ��������˺�	�Ժڰ�ϵ�й�������˺�	����Ԫ��ϵ�й�������˺�	����	��������	�������˺�	����������	ħ��������	ħ�������˺�	ħ����������	���˱���	����ʱ��	���ܵ���	���ܸ���	���ܼ��	�Ƿ�ɻ�	�Ƿ���ȫ�ָ�	��λ����	׷��ʱ��	ai�ο�1	ai�ο�2	ai�ο�3	ai�ο�4	ai�ο�5	�����ű�	ս���ű�	���˽ű�	�����ű�	���ڽű�	ѡ���ܽű�	Ѱ�ҵ�Ŀ��ű�	ɱ��Ŀ��ű�\n";
	for(iter=vecMonster.begin();iter!=vecMonster.end();iter++,nItem++)
	{
		file<<"*\t";
		file<<iter->dwIndex
			<<"\t"
			<<iter->strOrigName
			<<"\t"
			<<iter->strName
			<<"\t"
			<<iter->strBlock
			<<"\t"
			<<iter->strSuper
			<<"\t"
			<<iter->wDropFlag
			<<"\t"
			<<iter->strCollection
			<<"\t"
			<<iter->strTitle
			<<"\t"
			<<iter->dwTitleColor
			<<"\t"
			<<iter->dwTitlePicture
			<<"\t"
			<<iter->strCountryID
			<<"\t"
			<<iter->strNpcSort
			<<"\t"
			<<iter->wBossLevel
			<<"\t"
			<<iter->dwPicID
			<<"\t"
			<<iter->dwPicLevel
			<<"\t"
			<<iter->dwNameColorID
			<<"\t"
			<<iter->dwLevelColorID
			<<"\t"
			<<iter->dwHpColor
			<<"\t"
			<<iter->dwSoundID
			<<"\t"
			<<iter->strTame
			<<"\t"
			<<iter->dwTameTimes
			<<"\t"
			<<iter->strShape
			<<"\t"
			<<iter->nLevel
			<<"\t"
			<<iter->dwExp
			<<"\t"
			<<iter->dwWorkPoint
			<<"\t"
			<<iter->dwMaxHp
			<<"\t"
			<<iter->wMinAttack
			<<"\t"
			<<iter->wMaxAttack
			<<"\t"
			<<iter->wMinMagicAttack
			<<"\t"
			<<iter->wMaxMagicAttack
			<<"\t"
			<<iter->wHitRating
			<<"\t"
			<<iter->wDefense
			<<"\t"
			<<iter->wMagicDefense
			<<"\t"
			<<iter->wBlock
			<<"\t"
			<<iter->wImmunity
			<<"\t"
			<<iter->wPierce
			<<"\t"
			<<iter->wParry
			<<"\t"
			<<iter->wDodge
			<<"\t"
			<<iter->fMoveSpd
			<<"\t"
			<<iter->fBatMoveSpd
			<<"\t"
			<<iter->wWillAnti
			<<"\t"
			<<iter->wGroundAnti
			<<"\t"
			<<iter->wWaterAnti
			<<"\t"
			<<iter->wFireAnti
			<<"\t"
			<<iter->wWindAnti
			<<"\t"
			<<iter->wSacreAnti
			<<"\t"
			<<iter->wDarkAnti
			<<"\t"
			<<iter->wNotElemAnti
			<<"\t"
			<<iter->wHpRecoverSpd
			<<"\t"
			<<iter->wMpRecoverSpd
			<<"\t"
			<<iter->fTurningSpeed
			<<"\t"
			<<iter->wBodyAnti
			<<"\t"
			<<iter->wBloodAnti
			<<"\t"
			<<iter->wDuskAnti
			<<"\t"
			<<iter->wNumbAnti
			<<"\t"
			<<iter->wSleepAnti
			<<"\t"
			<<iter->wDrugAnti
			<<"\t"
			<<iter->wNotMoveAnti
			<<"\t"
			<<iter->wSilenceAnti
			<<"\t"
			<<iter->wMinStayTimes
			<<"\t"
			<<iter->wMaxStayTimes
			<<"\t"
			<<iter->wMinWalkTimes
			<<"\t"
			<<iter->wMaxWalkTimes
			<<"\t"
			<<iter->wAlertRange
			<<"\t"
			<<iter->wPurRange
			<<"\t"
			<<iter->wPeaMoveRange
			<<"\t"
			<<iter->strAIModel
			<<"\t"
			<<iter->bTalk
			<<"\t"
			<<iter->strMonRace
			<<"\t"
			<<iter->strElem
			<<"\t"
			<<iter->wStayTime
			<<"\t"
			<<iter->strAIWay
			<<"\t"
			<<iter->wHurtHuman
			<<"\t"
			<<iter->wHurtElem
			<<"\t"
			<<iter->wHurtDiff
			<<"\t"
			<<iter->wHurtAnimal
			<<"\t"
			<<iter->wHurtDead
			<<"\t"
			<<iter->wHurtDevil
			<<"\t"
			<<iter->wHurtInsect
			<<"\t"
			<<iter->wHurtFire
			<<"\t"
			<<iter->wHurtWater
			<<"\t"
			<<iter->wHurtGround
			<<"\t"
			<<iter->wHurtWind
			<<"\t"
			<<iter->wHurtSacre
			<<"\t"
			<<iter->wHurtDark
			<<"\t"
			<<iter->wHurtNotElem
			<<"\t"
			<<iter->wVitality
			<<"\t"
			<<iter->wStormHit
			<<"\t"
			<<iter->wStormHurt
			<<"\t"
			<<iter->wStormAnti
			<<"\t"
			<<iter->wMagicStorm
			<<"\t"
			<<iter->wMagicStormHurt  
			<<"\t"
			<<iter->wMagicStormAnti
			<<"\t"
			<<iter->fInjurRate
			<<"\t"
			<<iter->wAlertTime
			<<"\t"
			<<iter->fEscape
			<<"\t"
			<<iter->wSkillRate
			<<"\t"
			<<iter->wSkillInter
			<<"\t"
			<<iter->strReturn
			<<"\t"
			<<iter->strAllRecover
			<<"\t"
			<<iter->wReDistance
			<<"\t"
			<<iter->wReTime
			<<"\t"
			<<iter->strRefe1
			<<"\t"
			<<iter->strRefe2
			<<"\t"
			<<iter->strRefe3
			<<"\t"
			<<iter->strRefe4
			<<"\t"
			<<iter->strRefe5
			<<"\t"
			<<iter->strBirthSct
			<<"\t"
			<<iter->strBattleSct
			<<"\t"
			<<iter->strInjurSct
			<<"\t"
			<<iter->strDeadSct
			<<"\t"
			<<iter->strCycleSct
			<<"\t"
			<<iter->strChoseSkillSct
			<<"\t"
			<<iter->strFindTgtSct
			<<"\t"
			<<iter->strKillTgtSct;
			
		file<<"\n";
	}
	file.close();
	////�򿪹��＼���ļ�
	//ofstream fileskill(filenameskill);
	////�����ʧ�ܣ��򷵻�
	//if(!fileskill.is_open())
	//{
	//	return;
	//}
	////�����м��ܼ�¼������
	//for(iter=vecMonster.begin();iter!=vecMonster.end();iter++)
	//{
	//	
	//	//����м��ܼ�¼����д���ļ�
	//	if((int)iter->vecSkill.size()>0)
	//	{
	//		fileskill<<"*\t"
	//				<<iter->strOrigName
	//				<<"\n";
	//		
	//		for(iterskill=iter->vecSkill.begin();iterskill!=iter->vecSkill.end();
	//			iterskill++)
	//		{
	//			
	//			//��������Ϊ">"���ǻ�������
	//			if(iterskill->strSkillType==">")
	//			{
	//				fileskill<<">"
	//					<<"\t"
	//					<<iterskill->dwSkillID
	//					<<"\t"
	//					<<iterskill->wSkillLevel;
	//				fileskill<<"\n";
	//					
	//			}
	//			//��������Ϊ"#",����ͨ����
	//			else if(iterskill->strSkillType=="#")
	//			{
	//				fileskill<<"#"
	//					<<"\t"
	//					<<iterskill->dwSkillID
	//					<<"\t"
	//					<<iterskill->wSkillLevel;
	//				fileskill<<"\n";

	//			}
	//			//��������Ϊ"@"�������ڼ���
	//			else if(iterskill->strSkillType=="@")
	//			{
	//				fileskill<<"@"
	//					<<"\t"
	//					<<iterskill->dwSkillID
	//					<<"\t"
	//					<<iterskill->wSkillLevel
	//					<<"\t"
	//					<<iterskill->wInterTime;
	//				fileskill<<"\n";
	//			}
	//		}
	//	}
	//	else
	//		continue;

	//}
	//fileskill.close();

}
//ѡ���˲˵��ϵı����ļ�
void CMonsterEditorDlg::OnSaveToFile()
{
	// TODO: �ڴ���������������
	SaveFile(g_strfilename.c_str(),g_strfilenameskill.c_str());
	//�ļ��Ѿ�����
	bsaveflag=false;
}

//�ָ��ļ�
void CMonsterEditorDlg::OnRecover()
{
	// TODO: �ڴ���������������
	CRecover recoverdlg;
	recoverdlg.DoModal();
}
/*���ܣ��ָ��ļ�
*ժҪ�����жϱ����ļ��Ƿ��ܴ򿪣�Ȼ�����ԭ�ļ�����󽫱����ļ�
*���Ƶ�ԭ�ļ�
*/
void CMonsterEditorDlg::RecoverFile(string strfilename,string strfilenameskill)
{
	setlocale(LC_ALL,"Chinese-simplified");//�������Ļ���
	//�ֱ𿪴��ļ�
	fstream backupfile,backupfileskill;
	backupfile.open(strfilename.c_str());
	backupfileskill.open(strfilenameskill.c_str());
	//�ж��ļ��Ƿ��ܴ�
	if(!backupfile.is_open()||!backupfileskill.is_open())
	{
		char str[256];
		//���ܴ򿪹��������ļ�
		if(!backupfile.is_open())
		{
			sprintf_s(str, "���ܴ򿪱����ļ�'%s'", strfilename);
			MessageBox(str, "error");	
			return;
		}
		//���ܴ���＼���ļ�
		else if(!backupfileskill.is_open())
		{
			sprintf_s(str, "���ܴ򿪱����ļ�'%s'", strfilenameskill);
			MessageBox(str, "error");	
			return;
		}	
	}
	//�����ļ�·��
	::SetCurrentDirectory(g_buf);
	//��monsterlist.ini������ļ�����	
	CFile   m_file;   
	if(m_file.Open(g_strfilename.c_str(),CFile::modeReadWrite))
	{
		//����ļ����Ȳ�Ϊ0,����ļ�
		if(m_file.GetLength()>0)
		m_file.SetLength(0); 
	}
	//���ܴ��ļ�
	else
	{
		char str[256];
		sprintf_s(str, "���ܴ�ԭ�ļ�'%s'", g_strfilename.c_str());
		MessageBox(str, "error");	
		return;
	} 
	//�ر��ļ�
	m_file.Close();
	//��monsterlistskill.ini������ļ�����	 
	if(m_file.Open(g_strfilenameskill.c_str(),CFile::modeReadWrite))
	{
		//����ļ����Ȳ�Ϊ0,����ļ�
		if(m_file.GetLength()>0)
		m_file.SetLength(0); 
	}
	//���ܴ��ļ�
	else
	{
		char str[256];
		sprintf_s(str, "���ܴ�ԭ�ļ�'%s'", g_strfilenameskill.c_str());
		MessageBox(str, "error");	
		return;
	} 
	//�ر��ļ�
	m_file.Close();
	ofstream file;
	file.open(g_strfilename.c_str());
	if(!file.is_open())
	{
		MessageBox("�޷���ԭ�ļ�");
		return;
	}
	ofstream fileskill;
	fileskill.open(g_strfilenameskill.c_str());
	if(!fileskill.is_open())
	{
		MessageBox("�޷���ԭ�ļ�");
		return;
	}
	//���ò������ո�
	backupfile.unsetf(ios::skipws);
	backupfileskill.unsetf(ios::skipws);
	char ch;
	//���Ʊ����ļ���Ŀ���ļ�monsterlist.ini
	while(backupfile>>ch)file<<ch;
	//���Ʊ����ļ���Ŀ���ļ�monsterlistskill.ini
	while(backupfileskill>>ch)fileskill<<ch;
	char str2[256];
	sprintf_s(str2, "�ָ�'%s'��'%s'�ɹ�", g_strfilename.c_str(),g_strfilenameskill.c_str());
	MessageBox(str2,"MonsterEditor");
	//�ر������ļ�
	backupfile.close();
	backupfileskill.close();
	file.close();
	fileskill.close();
}
//�˵��������ļ�
void CMonsterEditorDlg::OnPartFile()
{
	// TODO: �ڴ���������������
	CPart partdlg;
	//ͳ��monsterlist.ini��¼����
	int nallrecords=(int)vecMonster.size();
	//ͳ��monsterskilllist.ini��¼����
	vector<tagMonster>::iterator iter=vecMonster.begin();
	int nallskillrecords=0;
	for(;iter!=vecMonster.end();iter++)
	{
		//�������м���ʱ
		if(iter->vecSkill.size()>0)
		nallskillrecords+=1;
	}
	//��ʼ����ֶԻ���
	partdlg.SetData(&vecMonster,nallrecords,nallskillrecords);

	partdlg.DoModal();
}
//�ϲ��ļ�
void CMonsterEditorDlg::OnMergerFile()
{
	// TODO: �ڴ���������������
	CMerger mergerdlg;
	mergerdlg.DoModal();
}
//�ر�������
void CMonsterEditorDlg::OnClose()
{
	//û�б����ļ�
	if(bsaveflag)
	{
		int ret=MessageBox("�Ƿ񱣴��ļ�","MonsterEditor",MB_YESNO);
		//ѡ�񱣴��ļ�
		if(ret==IDYES)
		{
			SaveFile(g_strfilename.c_str(),g_strfilenameskill.c_str());
		}
	}
	CDialog::OnClose();
}
BOOL CMonsterEditorDlg::PreTranslateMessage(MSG *pMsg)
{
	if(CWnd::PreTranslateMessage(pMsg))
		return TRUE;
	return((m_hAccelTable!=NULL)&&::TranslateAccelerator(m_hWnd,m_hAccelTable,pMsg));
}
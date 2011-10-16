// Recover.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MonsterEditor.h"
#include "Recover.h"
#include "MonsterEditorDlg.h"

// CRecover �Ի���
string FilePathName,FilePathNameSkill;
IMPLEMENT_DYNAMIC(CRecover, CDialog)

CRecover::CRecover(CWnd* pParent /*=NULL*/)
	: CDialog(CRecover::IDD, pParent)
{

}

CRecover::~CRecover()
{
}

void CRecover::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRecover, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CRecover::OnOpenMonsterFile)
	ON_BN_CLICKED(IDC_BUTTON2, &CRecover::OnOpenMonsterSkillFile)
	ON_BN_CLICKED(IDC_BUTTON3, &CRecover::OnRecover)
END_MESSAGE_MAP()


// CRecover ��Ϣ�������
//������ƶ�����*��֮��
int RecoverReadTo(fstream &stream,string name)
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

void CRecover::OnOpenMonsterFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
	//��OPEN���öԻ���
	if(dlg.DoModal()==IDOK)
	//��ȡ�ļ�·��
	FilePathName=dlg.GetPathName();
	//���ļ�·����ʾ��EDIT��
	SetDlgItemText(IDC_EDIT1,FilePathName.c_str());
}

void CRecover::OnOpenMonsterSkillFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
	//��OPEN���öԻ���
	if(dlg.DoModal()==IDOK)
	//��ȡ�ļ�·��
	FilePathNameSkill=dlg.GetPathName();
	//���ļ�·����ʾ��EDIT��
	SetDlgItemText(IDC_EDIT2,FilePathNameSkill.c_str());
}

void CRecover::OnRecover()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(FilePathName==""||FilePathNameSkill=="")
	{
		MessageBox("����ѡ���ļ�");
		return;
	}
	fstream file;
	string strflag;
	setlocale(LC_ALL,"Chinese-simplified");//�������Ļ���
	file.open(FilePathName.c_str());
	int i=0;
	RecoverReadTo(file,"*");
	file>>strflag;
	//�ж�ѡ����ļ��Ƿ���ȷ
	for(;strflag[i]!='\0';i++)
		{
			//�������ȷ
			if(strflag[i]<'0'||strflag[i]>'9')
			{
				MessageBox("��ȷ�������ļ��еļ�¼ȫ�ǹ������Լ�¼");
				return;
			}
			
		}
	file.close();
	file.open(FilePathNameSkill.c_str());
	//�ƶ����������ַ�
	RecoverReadTo(file,"*");
	file>>strflag;
	file>>strflag;
	//���������������е��κ�һ�������ǹ��＼���ļ�
	if(strflag!=">"&&strflag!="#"&&strflag!="@")
		{
			MessageBox("��ȷ�������ļ��еļ�¼ȫ�ǹ��＼�ܼ�¼");
			return;
		}
	file.close();
	CMonsterEditorDlg recdlg;
	recdlg.RecoverFile(FilePathName,FilePathNameSkill);
}

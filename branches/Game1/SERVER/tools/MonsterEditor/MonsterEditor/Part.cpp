// Part.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MonsterEditor.h"
#include "Part.h"


// CPart �Ի���
vector<tagMonster>*vecPartMon;
IMPLEMENT_DYNAMIC(CPart, CDialog)

CPart::CPart(CWnd* pParent /*=NULL*/)
	: CDialog(CPart::IDD, pParent)
	, nfirstA(0)
	, nfirstB(0)
	, nallrecords(0)
	, nallskillrecords(0)
{

}

CPart::~CPart()
{
}

void CPart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, nfirstA);

	DDX_Text(pDX, IDC_EDIT3, nfirstB);

	DDX_Text(pDX, IDC_STATIC1, nallrecords);
	DDV_MinMaxInt(pDX, nallrecords, 0, 5000000);
	DDX_Text(pDX, IDC_STATIC2, nallskillrecords);
	DDV_MinMaxInt(pDX, nallskillrecords, 0, 5000000);
}


BEGIN_MESSAGE_MAP(CPart, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CPart::OnPart)
END_MESSAGE_MAP()


// CPart ��Ϣ�������
void CPart::SetData(vector<tagMonster>*vecMonster,int n,int m)
{
	vecPartMon=vecMonster;
	nallrecords=n;
	nallskillrecords=m;
}
void CPart::OnPart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!IsDlgButtonChecked(IDC_RADIO1)&&!IsDlgButtonChecked(IDC_RADIO2))
	{
		MessageBox("��ѡ��Ҫ��ֵ��ļ�");
	}
	vector<tagMonster>::iterator iter;
	vector<tagSkill>::iterator iterskill;
	//ѡ�е�һ����ѡ��
	if(IsDlgButtonChecked(IDC_RADIO1))
	{
		UpdateData();
		//�жϲ�ַ�Χ�Ƿ񳬳���¼
		if(nfirstA>nallrecords||nfirstA<=0||nfirstB<=0||nfirstB>nallrecords)
		{
			MessageBox("��¼����");
			return;
		}
		CString FilePathName;
		CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"Text File(*.txt)|*.*| |",AfxGetMainWnd());///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
		if(dlg.DoModal()==IDOK)
		FilePathName=dlg.GetPathName();
		if(FilePathName=="")
			return;
		//�ļ�����Ϊ�ļ��ļ�
		FilePathName+=".txt";
		//�����ļ�
		FILE *fp;
		fp=fopen(FilePathName,"a");
		fclose(fp);
	//	file.close();
		setlocale(LC_ALL,"Chinese-simplified");//�������Ļ���
		//����ļ�
		CFile   m_file;   
		if(m_file.Open(FilePathName,CFile::modeReadWrite))
		{
			//����ļ����Ȳ�Ϊ0,����ļ�
			if(m_file.GetLength()>0)
			m_file.SetLength(0); 
		}
		m_file.Close();
		ofstream file;
		file.open(FilePathName);
		if(!file.is_open())
		{
			MessageBox("���ܴ��ļ�!");
			return;
		}
		//����ļ������浽ָ���ļ�
		for(iter=(*vecPartMon).begin()+nfirstA-1;nfirstA<=nfirstB;nfirstA++,iter++)
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
	}
	//ѡ�еڶ�����ѡ��
	else if(IsDlgButtonChecked(IDC_RADIO2))
	{
		//�жϲ�ַ�Χ�Ƿ񳬳���¼
		UpdateData();
		if(nfirstA>nallrecords||nfirstA<=0||nfirstB<=0||nfirstB>nallrecords)
		{
			MessageBox("��¼����");
			return;
		}
		CString FilePathName;
		CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"Text File(*.txt)|*.*| |",AfxGetMainWnd());
		if(dlg.DoModal()==IDOK)
		FilePathName=dlg.GetPathName();
		if(FilePathName=="")
			return;
		FilePathName+=".txt";
		ofstream file;
		FILE *fp;
		fp=fopen(FilePathName,"a");
		fclose(fp);
		setlocale(LC_ALL,"Chinese-simplified");//�������Ļ���
		//����ļ�
		CFile   m_file;   
		if(m_file.Open(FilePathName,CFile::modeReadWrite))
		{
			//����ļ����Ȳ�Ϊ0,����ļ�
			if(m_file.GetLength()>0)
			m_file.SetLength(0); 
		}
		m_file.Close();
		file.open(FilePathName);
		if(!file.is_open())
		{
			MessageBox("���ܴ��ļ�!");
			return;
		}
		for(iter=(*vecPartMon).begin()+nfirstA-1;nfirstA<=nfirstB;nfirstA++,iter++)
		{
			//�������ļ��ܼ�¼
			if(iter->vecSkill.size()>0)
			{
				file<<"*\t";
				file<<iter->strOrigName
					<<"\n";
				//������ܼ�¼���ļ�
				for(iterskill=iter->vecSkill.begin();iterskill!=
					iter->vecSkill.end();iterskill++)
				{
					//���ǻ�������
					if(iterskill->strSkillType==">")
					{
						file<<">"
							<<"\t"
							<<iterskill->dwSkillID
							<<"\t"
							<<iterskill->wSkillLevel;
						file<<"\n";
							
					}
					//��ͨ����
					else if(iterskill->strSkillType=="#")
					{
						file<<"#"
							<<"\t"
							<<iterskill->dwSkillID
							<<"\t"
							<<iterskill->wSkillLevel;
						file<<"\n";

					}
					//���ڼ���
					else if(iterskill->strSkillType=="@")
					{
						file<<"@"
							<<"\t"
							<<iterskill->dwSkillID
							<<"\t"
							<<iterskill->wSkillLevel
							<<"\t"
							<<iterskill->wInterTime;
						file<<"\n";
					}

				}//for

			}//if
		}//for
	}//else if
	
}

// Merger.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MonsterEditor.h"
#include "Merger.h"


// CMerger �Ի���
vector<tagMonster>vecMerMon;//װ���ļ���¼������

IMPLEMENT_DYNAMIC(CMerger, CDialog)

CMerger::CMerger(CWnd* pParent /*=NULL*/)
	: CDialog(CMerger::IDD, pParent)
{

}

CMerger::~CMerger()
{
}

void CMerger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMerger, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &CMerger::OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMerger::OnRadio2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMerger::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON17, &CMerger::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON18, &CMerger::OnBnClickedButton18)
	ON_BN_CLICKED(IDC_BUTTON2, &CMerger::OnMerger)
END_MESSAGE_MAP()


// CMerger ��Ϣ�������
BOOL CMerger::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	return TRUE;
}

//DOWRDת��ΪCString

CString MergerDwordToString(DWORD n)
{

	CString str;

	str.Format("%d",n);
	return str;
}

void CMerger::OnRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDlgItemText(IDC_STATIC1,"��monsterlist.ini���ļ�");
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
}

void CMerger::OnRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDlgItemText(IDC_STATIC1,"��monsterskilllist.ini���ļ�");
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
}

void CMerger::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString FilePathName;
	CFileDialog dlg(TRUE);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
	if(dlg.DoModal()==IDOK)
	FilePathName=dlg.GetPathName();
	SetDlgItemText(IDC_EDIT1,FilePathName);
}

void CMerger::OnBnClickedButton17()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString FilePathName;
	CFileDialog dlg(TRUE);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
	if(dlg.DoModal()==IDOK)
	FilePathName=dlg.GetPathName();
	SetDlgItemText(IDC_EDIT2,FilePathName);
}

void CMerger::OnBnClickedButton18()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString FilePathName;
	CFileDialog dlg(TRUE);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
	if(dlg.DoModal()==IDOK)
	FilePathName=dlg.GetPathName();
	SetDlgItemText(IDC_EDIT3,FilePathName);
}
//����˺ϲ����Ϊ��ť
void CMerger::OnMerger()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//���vecMonsterSkillMerger��ÿ�κϲ�����ʱ���Ὣ��ǰ������д���ļ�
	vecMerMon.clear();
	CString str1,str2,str3;
	//��ȡEDIT�ļ�·��
	GetDlgItemText(IDC_EDIT1,str1);
	GetDlgItemText(IDC_EDIT2,str2);
	GetDlgItemText(IDC_EDIT3,str3);
	//д����¼д��vecMerMon
	vector<tagMonster>::iterator iter;
	vector<tagSkill>::iterator iterskill;
	//�ϲ������Ϊ�ļ���
	CString FilePathName;
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"Text File(*.txt)|*.*| |",AfxGetMainWnd());///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
	if(dlg.DoModal()==IDOK)
	FilePathName=dlg.GetPathName();
	if(FilePathName=="")
		return;
	FilePathName+=".txt";
	//ѡ�е�һ��radio
	if(IsDlgButtonChecked(IDC_RADIO1))
	{
		//���ļ�·����Ϊ��ʱִ��
		if(str1!="")
		{
		if(!MergerFile(str1))
			return;
		}
		if(str2!="")
		{
		if(!MergerFile(str2))
			return;
		}
		if(str3!="")
		{
		if(!MergerFile(str3))
			return;
		}
		if(FilePathName!="")
		{
			//������ļ�����
			ofstream file;
			file.open(FilePathName);
			file.close();
			CFile   file3;   
			file3.Open(FilePathName,   CFile::modeReadWrite); 
			//�ļ����Ȳ�Ϊ0,����ļ�
			if(file3.GetLength()!=0)
			{
				file3.SetLength(0);   
			}
			file3.Close();
				
			file.open(FilePathName);
			if(!file.is_open())
			{
				MessageBox("�޷����ļ�");
				return;
			}
			
			
			int i=0;
			//������ļ�
			for(iter=vecMerMon.begin();iter!=vecMerMon.end();iter++,i++)
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

		}
		//ѡ�еڶ���radio
		else if(IsDlgButtonChecked(IDC_RADIO2))
		{
			//���ļ�·����Ϊ��ʱ
			if(str1!="")
			{
				if(!MergerFileSkill(str1))
					return;
			}
			if(str2!="")
			{
				if(!MergerFileSkill(str2))
					return;
			}
			if(str3!="")
			{
				if(!MergerFileSkill(str3))
					return;
			}
			if(FilePathName!="")
			{
				//�����ļ�
				FILE *fp;
				fp=fopen(FilePathName,"a");
				fclose(fp);
				CFile   file3;  
				//������ļ�����
				file3.Open(FilePathName,   CFile::modeReadWrite); 
				if(file3.GetLength()!=0)
				{
					file3.SetLength(0);   
				}
				file3.Close();
				fstream fileskill;
				setlocale(LC_ALL,"Chinese-simplified");//�������Ļ���
				fileskill.open(FilePathName);
				if(!fileskill.is_open())
				{
					MessageBox("�޷����ļ�");
					return;
				}
				//������ļ�
				for(iter=vecMerMon.begin();iter!=vecMerMon.end();iter++)
					{
						
						//����м��ܼ�¼����д���ļ�
						if((int)iter->vecSkill.size()>0)
						{
							fileskill<<"*\t"
									<<iter->strOrigName
									<<"\n";
							
							for(iterskill=iter->vecSkill.begin();iterskill!=
								iter->vecSkill.end();
								iterskill++)
							{
								
								//��������Ϊ">"���ǻ�������
								if(iterskill->strSkillType==">")
								{
									fileskill<<">"
										<<"\t"
										<<iterskill->dwSkillID
										<<"\t"
										<<iterskill->wSkillLevel;
									fileskill<<"\n";
										
								}
								//��������Ϊ"#",����ͨ����
								else if(iterskill->strSkillType=="#")
								{
									fileskill<<"#"
										<<"\t"
										<<iterskill->dwSkillID
										<<"\t"
										<<iterskill->wSkillLevel;
									fileskill<<"\n";

								}
								//��������Ϊ"@"�������ڼ���
								else if(iterskill->strSkillType=="@")
								{
									fileskill<<"@"
										<<"\t"
										<<iterskill->dwSkillID
										<<"\t"
										<<iterskill->wSkillLevel
										<<"\t"
										<<iterskill->wInterTime;
									fileskill<<"\n";
								}
							}
						}
						else
							continue;

					}
					fileskill.close();
			}

		}
}
//������ƶ�����*��֮��
int ReadTot(fstream &stream,string name)
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
//�ж��������Ƿ����ظ�
bool CMerger::IsSame(CString strfilename,tagMonster *monster)
{	
	vector<tagMonster>::iterator iter;
	//���ϲ��ļ����Ƿ���������ԭʼ����ͬ�ļ�¼
	for(iter=vecMerMon.begin();iter!=vecMerMon.end();iter++)
	{
		//������ͬ
		if(iter->dwIndex==(*monster).dwIndex)
			{
				char strVal[100];
				sprintf_s(strVal,"'%s'����������ͬ�ļ�¼,�Ƿ񸲸�ԭ��¼",strfilename);
				int ret=MessageBox(strVal,"MonsterEditor",MB_YESNO);
				//����ԭ��¼
				if(ret==IDYES)
				{
					vecMerMon.erase(iter);
					vecMerMon.push_back(*monster);					
					return true;
				}
				//����ԭ��¼
				else
					return true;
			}
		//ԭʼ����ͬ
		if(iter->strOrigName==(*monster).strOrigName)
			{
				char strVal[100];
				sprintf_s(strVal,"'%s'����ԭʼ����ͬ�ļ�¼,�Ƿ񸲸�ԭ��¼",strfilename);
				int ret=MessageBox(strVal,"MonsterEditor",MB_YESNO);
				//����ԭ��¼
				if(ret==IDYES)
				{
					vecMerMon.erase(iter);
					vecMerMon.push_back(*monster);					
					return true;
				}
				//����ԭ��¼
				else
					return true;
			}	
		
	}//for
	//û���ҵ���ͬ�ļ�¼
	return false;

}
//�ϲ������ļ�
bool CMerger::MergerFile(CString strfilename)
{
	setlocale(LC_ALL,"Chinese-simplified");//�������Ļ���
	int i=0;
	char strVal[100];
	tagMonster monster;
	fstream file;
	file.open(strfilename);
	if(!file.is_open())
	{
		MessageBox("�ļ��޷���");
		return false;
	}

	//��ȡ�ļ�
	while((ReadTot(file,"*"))!=0)
	{	
		file>> monster.dwIndex;
		itoa(monster.dwIndex,strVal,10);
		//�жϴ򿪵��ļ��Ƿ��ǹ��������ļ�����ֹ���ļ�����
		for(;strVal[i]!='\0';i++)
		{
			if(strVal[i]<'0'||strVal[i]>'9')
			{
				vecMerMon.clear();
				MessageBox("��ȷ�������ļ��еļ�¼ȫ�ǹ�������");
				return false;
			}
			
		}

		file>>monster.strOrigName
			>>monster.strName;
		//�ж��ļ����Ƿ���ȷ
		if(monster.strName==">"||monster.strName=="#"||monster.strName=="@")
		{
			vecMerMon.clear();
			MessageBox("��ȷ�������ļ��еļ�¼ȫ�ǹ�������");
			return false;
		}
		file>>monster.strBlock
			>>monster.strSuper
			>>monster.strCollection
			>>monster.strTitle
			>>monster.dwTitleColor
			>>monster.dwTitlePicture
			>>monster.strCountryID
			>>monster.strNpcSort
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
			>>monster.wParry
			>>monster.wDodge
			>>monster.fMoveSpd
			>>monster.fBatMoveSpd
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

			//�ж������Ƿ��ظ�
		if(!IsSame(strfilename,&monster))
		{
			vecMerMon.push_back(monster);
			continue;
		}
	
	}
	return true;
}
//�жϼ����ļ����Ƿ�����ͬ��¼
bool CMerger::IsSameSkill(CString strfilename,tagMonster *monster)
{
	vector<tagMonster>::iterator iter;
	//�ж�ԭʼ���Ƿ��ظ�
	for(iter=vecMerMon.begin();iter!=vecMerMon.end();iter++)
	{
		//���ظ��ļ�¼
		if(iter->strOrigName.c_str()==(*monster).strOrigName)
		{
			char strVal[100];
			sprintf_s(strVal,"'%s'����ԭʼ����ͬ�ļ�¼,�Ƿ񸲸�ԭ��¼",strfilename);
			int ret=MessageBox(strVal,"MonsterEditor",MB_YESNO);
			//���Ǽ�¼
			if(ret==IDYES)
			{
				//��ɾ��ԭ��¼
				vecMerMon.erase(iter);
				//�����¼�¼
				vecMerMon.push_back(*monster);
				//��ռ�����������ֹ�ظ�
				(*monster).vecSkill.clear();
				return true;
			}
			else
				return true;
		}
	}//for
	return false;
}
bool CMerger::MergerFileSkill(CString strfilename)
{
	fstream fileskill(strfilename);
	//����ļ�û�д򿪣��򷵻�
	if(!fileskill.is_open())
		return false;
	//�����Ϊ0������Ӧ�ļ��ܼ�¼
	string strflag;
	int i=0;
	tagMonster monster;
	tagSkill skill;
	ReadTot(fileskill,"*");
	while(!fileskill.eof())
	{
		fileskill>>monster.strOrigName
			>>strflag;
		
		//�жϴ򿪵��ļ��Ƿ��ǹ��＼���ļ�����ֹ���ļ�����
		if(strflag!=">"&&strflag!="#"&&strflag!="@")
		{
			vecMerMon.clear();
			MessageBox("��ȷ�������ļ��еļ�¼ȫ�ǹ��＼�ܼ�¼");
			return false;
		}

		
		//�����ּ��ܣ���>������#������@������ͬ����ļ���
		while(strflag==">"||strflag=="#"||strflag=="@")		
		{
			//">"���ļ����Ǳ�ʾ��������
			if(strflag==">")
			{
				strflag="";
				skill.strSkillType=">";
				fileskill>>skill.dwSkillID
					>>skill.wSkillLevel
					>>strflag;
				//��������û�м��ܼ��������Ϊ0
				skill.wInterTime=0;
				monster.vecSkill.push_back(skill);

			}
			//"#"���ļ��б�ʾ��ͨ����
			else if(strflag=="#")
			{
				strflag="";
				skill.strSkillType="#";
				fileskill>>skill.dwSkillID
					>>skill.wSkillLevel
					>>strflag;
				//��ͨ����û�м��ܼ��������Ϊ0
				skill.wInterTime=0;
				monster.vecSkill.push_back(skill);
			}
			//"@"���ļ��б�ʾ���ڼ���
			else if(strflag=="@")
			{
				strflag="";
				skill.strSkillType="@";
				fileskill>>skill.dwSkillID
					>>skill.wSkillLevel
					>>skill.wInterTime
					>>strflag;
				//������skill���뵽vecSkill��
				monster.vecSkill.push_back(skill);
			}
			//"*"��ʾ��һ����¼�Ŀ�ʼ�����Է���
			else if(strflag=="*")
			{
				break;
			}
		}
		int n=(int)vecMerMon.size();//��¼����
		//��һ��Ԫ��ֱ�Ӳ��뵽vector��
		if(n<1)
		{
			vecMerMon.push_back(monster);
			monster.vecSkill.clear();
			continue;
		}
		//�ж��Ƿ���ԭʼ����ͬ�ļ�¼
		if(!IsSameSkill(strfilename,&monster))
		{
			vecMerMon.push_back(monster);
			monster.vecSkill.clear();
			continue;	
		}
		
	}//while
	return true;
}
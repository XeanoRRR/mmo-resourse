/*
=========================================================================
��ע����δʹ�á�2010.10.28  BY:LGR
=========================================================================
*/

/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�CSkillTest.cpp
*  ���ܣ����ܲ��ԶԻ������
*  ��ǰ�汾��1.1.0
*  ���ߣ�
*  ������ڣ�
*  ȡ���汾��1.0.0
*  ԭ���ߣ�
*  ������ڣ�
*
============================================================
*/

#include "stdafx.h"
#include "GameTest.h"
#include "SkillTest.h"

IMPLEMENT_DYNAMIC(CSkillTest, CDialog)

CSkillTest::CSkillTest(CWnd* pParent /*=NULL*/)
	: CDialog(CSkillTest::IDD, pParent)
	, m_RegionID(60101)
	, m_lPosX(280)
 	, m_lPosY(290)
	, m_lHighAttack(1)
	, m_lLowAttack(1)
	, m_bUseSkill1(TRUE)
	, m_bUseSkill2(TRUE)
	, m_bUseSkill3(TRUE)
	, m_lOccu(22)
	//, m_lWeapon(_T(S080E01))
	, m_lSkillAccountBegin(2101)
	, m_lSkillAccountEnd(2200)
	, m_lTimer(800)
	, m_lVersion(5008)
	, m_lCustomSkillID1(0)
	, m_lCustomSkillID2(0)
	, m_lCustomSkillID3(0)
	, m_lCustomSkillID4(0)
	, m_bRandomPos(TRUE)
	, m_bIsCanACCAnswer(FALSE)
	, m_bUseSDO(false)
{
	m_lWeapon = "";
}


CSkillTest::~CSkillTest()
{

}

void CSkillTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_REGIONID, m_RegionID);
	DDX_Text(pDX, IDC_EDIT_POSX, m_lPosX);
	DDV_MinMaxLong(pDX, m_lPosX, 0, 512);
	DDX_Text(pDX,IDC_EDIT_POSY,m_lPosY);
	DDV_MinMaxLong(pDX,m_lPosY,0,512);
	DDX_Check(pDX, IDC_CHECK_SKILL1, m_bUseSkill1);
	DDX_Check(pDX, IDC_CHECK_SKILL2, m_bUseSkill2);
	DDX_Check(pDX, IDC_CHECK_SKILL3, m_bUseSkill3);
	DDX_Control(pDX, IDC_COMBO_OCCU, m_CombOccu);
	DDX_Text(pDX, IDC_SkillStartCDKey, m_lSkillAccountBegin);
	DDX_Text(pDX, IDC_SkillEndCDKey, m_lSkillAccountEnd);
	DDX_Text(pDX, IDC_EDIT_SkillTimer, m_lTimer);
	DDX_Text(pDX, IDC_EDIT_SkillVersion, m_lVersion);
	DDX_Control(pDX, IDC_COMBO_SkillIP, m_CombIP);
	DDX_Text(pDX, IDC_EDIT_SKILLID1,m_lCustomSkillID1);
	DDX_Text(pDX, IDC_EDIT_SKILLID2,m_lCustomSkillID2);
	DDX_Text(pDX, IDC_EDIT_SKILLID3,m_lCustomSkillID3);
	DDX_Text(pDX, IDC_EDIT_SKILLID4,m_lCustomSkillID4);
	DDX_Check(pDX, IDC_CHECK1, m_bRandomPos);
	DDX_Check(pDX, IDC_CHECK_SKILL_ACCAnswer, m_bIsCanACCAnswer);
	DDX_Check(pDX, IDC_CHECK2 ,m_bUseSDO);
}


BEGIN_MESSAGE_MAP(CSkillTest, CDialog)
//	ON_EN_CHANGE(IDC_EDIT_REGIONID, &CSkillTest::OnEnChangeEditRegionid)
ON_BN_CLICKED(IDC_RADIO_ATTACK_HIGH, &CSkillTest::OnBnClickedRadioAttackHigh)
ON_BN_CLICKED(IDC_RADIO_ATTACK_LOW, &CSkillTest::OnBnClickedRadioAttackLow)
ON_BN_CLICKED(IDC_RADIO_WEAPON1, &CSkillTest::OnBnClickedRadioWeapon1)
ON_BN_CLICKED(IDC_RADIO_WEAPON2, &CSkillTest::OnBnClickedRadioWeapon2)
ON_BN_CLICKED(IDC_RADIO_WEAPON3, &CSkillTest::OnBnClickedRadioWeapon3)
ON_BN_CLICKED(IDC_RADIO_WEAPON4, &CSkillTest::OnBnClickedRadioWeapon4)
ON_BN_CLICKED(IDC_RADIO_WEAPON5, &CSkillTest::OnBnClickedRadioWeapon5)
ON_BN_CLICKED(IDC_RADIO_WEAPON6, &CSkillTest::OnBnClickedRadioWeapon6)
ON_BN_CLICKED(IDC_RADIO_WEAPON7, &CSkillTest::OnBnClickedRadioWeapon7)
ON_BN_CLICKED(IDC_BUTTON_SkillStart, &CSkillTest::OnBnClickedButtonSkillstart)
ON_BN_CLICKED(IDC_CHECK_SKILL_ACCAnswer, &CSkillTest::OnBnClickedACCAnswer)
ON_CBN_SELCHANGE(IDC_COMBO_OCCU, &CSkillTest::OnCbnSelchangeComboOccu)
ON_CBN_SELCHANGE(IDC_COMBO_SkillIP, &CSkillTest::OnCbnSelchangeComboSkillip)
ON_BN_CLICKED(IDC_CHECK2, &CSkillTest::OnBnClickedCheck2)
END_MESSAGE_MAP()

BOOL CSkillTest::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	//�ֱ��������鵥ѡ��ť�ĳ�ʼѡ��
	((CButton*)GetDlgItem(IDC_RADIO_ATTACK_LOW))->SetCheck(1);
	//((CButton*)GetDlgItem(IDC_RADIO_WEAPON1))->SetCheck(1);

	//����"ѡ��ְҵ"�������б�
	m_CombOccu.InsertString(-1,"սʿ");
	m_CombOccu.InsertString(-1,"��ʦ");
	m_CombOccu.InsertString(-1,"����");
	m_CombOccu.InsertString(-1,"��ʿ");
	m_CombOccu.InsertString(-1,"��ʿ");
	m_CombOccu.InsertString(-1,"���");
	m_CombOccu.InsertString(-1,"����");
	m_CombOccu.InsertString(-1,"��ʿ");
	m_CombOccu.InsertString(-1,"��ʦ");
	m_CombOccu.InsertString(-1,"ʥ��ʿ");
	m_CombOccu.InsertString(-1,"����ʿ");
	m_CombOccu.InsertString(-1,"��ʿ");
	m_CombOccu.InsertString(-1,"��սʿ");
	m_CombOccu.InsertString(-1,"Ѳ��");
	m_CombOccu.InsertString(-1,"�̿�");
	m_CombOccu.InsertString(-1,"������");
	m_CombOccu.InsertString(-1,"����ʫ��");
	m_CombOccu.InsertString(-1,"��ʦ");
	m_CombOccu.InsertString(-1,"���鷨ʦ");
	m_CombOccu.InsertString(-1,"����");
	m_CombOccu.InsertString(-1,"����");
	m_CombOccu.InsertString(-1,"���");
	//��ʾĬ�ϵ���Ŀ
	m_CombOccu.SetCurSel(21);

	//����"IP��ַ"�������б�
	m_CombIP.InsertString(-1,"21������");
	m_CombIP.InsertString(-1,"�Ÿ�40");
	m_CombIP.InsertString(-1,"�Ÿ�50");
	m_CombIP.InsertString(-1,"�Ƚ�");
	m_CombIP.InsertString(-1,"����");
	m_CombIP.InsertString(-1,"����");
	m_CombIP.InsertString(-1,"�Ÿ�2");
	m_CombIP.InsertString(-1,"26������");
	m_CombIP.InsertString(-1,"���˷�");
	m_CombIP.InsertString(-1,"ФҢ");
	m_CombIP.InsertString(-1,"����");
	//��ʾĬ�ϵ���Ŀ
	m_CombIP.SetCurSel(0);

	m_CombIP.ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_SERVER)->ShowWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


//����Ϊ�ܸߵĹ�����
void CSkillTest::OnBnClickedRadioAttackHigh()
{
	m_lHighAttack = 3000;
	m_lLowAttack = 3000;
}

//����Ϊ�ܵ͵Ĺ�����
void CSkillTest::OnBnClickedRadioAttackLow()
{
	m_lHighAttack = 1;
	m_lLowAttack = 1;
}

//ѡ�������
void CSkillTest::OnBnClickedRadioWeapon1()
{
	m_lWeapon = "S080E01";
}

void CSkillTest::OnBnClickedRadioWeapon2()
{
	m_lWeapon = "T060E01";
}

void CSkillTest::OnBnClickedRadioWeapon3()
{
	m_lWeapon = "E100A01";
}

void CSkillTest::OnBnClickedRadioWeapon4()
{
	m_lWeapon = "A100A01";
}

void CSkillTest::OnBnClickedRadioWeapon5()
{
	m_lWeapon = "B080A00";
}

void CSkillTest::OnBnClickedRadioWeapon6()
{
	m_lWeapon = "X100A01";
}



void CSkillTest::OnBnClickedRadioWeapon7()
{
	m_lWeapon = "D060A00";
}

//���ѡ��ְҵ
void CSkillTest::OnCbnSelchangeComboOccu()
{
	
	long Occu = m_CombOccu.GetCurSel();
	
	switch(Occu)
	{
	case 0:
		m_lOccu = 1;
		break;
	case 1:
		m_lOccu = 2;
		break;
	case 2:
		m_lOccu = 3;
		break;
	case 3:
		m_lOccu = 4;
		break;
	case 4:
		m_lOccu = 5;
		break;
	case 5:
		m_lOccu = 6;
		break;
	case 6:
		m_lOccu = 7;
		break;
	case 7:
		m_lOccu = 8;
		break;
	case 8:
		m_lOccu = 9;
		break;
	case 9:
		m_lOccu = 10;
		break;
	case 10:
		m_lOccu = 11;
		break;
	case 11:
		m_lOccu = 12;
		break;
	case 12:
		m_lOccu = 13;
		break;
	case 13:
		m_lOccu = 14;
		break;
	case 14:
		m_lOccu = 15;
		break;
	case 15:
		m_lOccu = 16;
		break;
	case 16:
		m_lOccu = 17;
		break;
	case 17:
		m_lOccu = 18;
		break;
	case 18:
		m_lOccu = 19;
		break;
	case 19:
		m_lOccu = 20;
		break;
	case 20:
		m_lOccu = 21;
		break;
	case 21:
		m_lOccu = 22;
	}
}

void CSkillTest::OnCbnSelchangeComboSkillip()
{

	long IP = m_CombIP.GetCurSel();
	//m_CombIP.InsertString("21������");
	//m_CombIP.InsertString("�Ÿ�40");
	//m_CombIP.InsertString("�Ÿ�50");
	//m_CombIP.InsertString("�Ƚ�");
	//m_CombIP.InsertString("����");
	//m_CombIP.InsertString("����");
	//m_CombIP.InsertString("�Ÿ�2");
	//m_CombIP.InsertString("26������");
	//m_CombIP.InsertString("���˷�");
	//m_CombIP.InsertString("ФҢ");
	//m_CombIP.InsertString("����");
	switch(IP)
	{
	case 0:
		m_strIP = "192.168.220.21";
		break;
	case 1:
		m_strIP = "192.168.220.40";
		break;
	case 2:
		m_strIP = "192.168.220.50";
		break;
	case 3:
		m_strIP = "192.168.220.3";
		break;
	case 4:
		m_strIP = "192.168.220.31";
		break;
	case 5:
		m_strIP = "192.168.220.9";
		break;
	case 6:
		m_strIP = "192.168.220.2";
		break;
	case 7:
		m_strIP = "192.168.220.26";
		break;
	case 8:
		m_strIP = "192.168.220.24";
		break;
	case 9:
		m_strIP = "192.168.220.27";
		break;
	case 10:
		m_strIP = "192.168.220.8";
		break;
	}

}


void CSkillTest::OnBnClickedACCAnswer()
{
	m_bIsCanACCAnswer = ((CButton*)GetDlgItem(IDC_CHECK_SKILL_ACCAnswer))->GetCheck() ? true : false ;
}


//�����˿�ʼ���԰�ť
void CSkillTest::OnBnClickedButtonSkillstart()
{
	UpdateData(TRUE);

	//�ж��ǲ�����ʢ��ʽ��½
	if (!m_bUseSDO)
	{
		//�ж���Ҫʹ�õļ��ܵķ�Χ
		if(m_bUseSkill1 == TRUE && m_bUseSkill2 == TRUE && m_bUseSkill3 == TRUE)
		{
			//ʹ��1ת2ת3ת����
			m_lSkillRange = 1;
		}
		else if (m_bUseSkill1 == TRUE && m_bUseSkill2 == TRUE && m_bUseSkill3 == FALSE)
		{
			//ʹ��1ת2ת����,��ʹ��3ת����
			m_lSkillRange = 2;
		}
		else if (m_bUseSkill1 == TRUE && m_bUseSkill2 == FALSE && m_bUseSkill3 == TRUE)
		{
			//ʹ��1ת3ת����,��ʹ��2ת����
			m_lSkillRange = 3;
		}
		else if (m_bUseSkill1 == TRUE && m_bUseSkill2 == FALSE && m_bUseSkill3 == FALSE)
		{
			//ʹ��1ת����,��ʹ��2ת3ת����
			m_lSkillRange = 4;
		}
		else if(m_bUseSkill1 == FALSE && m_bUseSkill2 == TRUE && m_bUseSkill3 == TRUE)
		{
			//ʹ��2ת3ת����,��ʹ��1ת����
			m_lSkillRange = 5;
		}
		else if (m_bUseSkill1 == FALSE && m_bUseSkill2 == TRUE && m_bUseSkill3 == FALSE)
		{
			//ʹ��2ת����,��ʹ��1ת3ת����
			m_lSkillRange = 6;
		}
		else if (m_bUseSkill1 == FALSE && m_bUseSkill2 == FALSE && m_bUseSkill3 == TRUE)
		{	
			//ʹ��3ת����,��ʹ��1ת2ת����
			m_lSkillRange = 7;
		}
		else if (m_bUseSkill1 == FALSE && m_bUseSkill2 == FALSE && m_bUseSkill3 == FALSE)
		{
			//��ʹ���κμ���,����
			MessageBox("û��ָ��ʹ�õļ��ܵķ�Χ,������ѡ��","����",MB_OK);
			return;
		}

		//�����жϲ�������������Ӧ���ص�ֵ
		long lType = 0;

		if ( m_lSkillAccountBegin < 1000 || m_lSkillAccountEnd > 9999)
		{
			MessageBox("����ʼ�ʺš�Ӧ�ô���1000,С��9999","��ʾ",MB_OK);
			return ;
		}
		if ( m_lSkillAccountEnd < m_lSkillAccountBegin || m_lSkillAccountEnd > 9999 )
		{
			MessageBox("�������ʺš�Ӧ�ô��ڻ�����ڡ���ʼ�ʺš�����С��9999","��ʾ",MB_OK);
			return ;
		}
		if (m_lTimer >= 200000 || m_lTimer <= 200)
		{
			MessageBox("���ʱ�������200-200000ms֮��","��ʾ",MB_OK);
			return ;
		}

		if ( m_lSkillAccountBegin == m_lSkillAccountEnd )
		{
			m_lSkillAccountEnd = m_lSkillAccountBegin;
		}

		//sprintf(strScriptName,"SkillLoginCDKey(%d,%d,%d,%d%s",m_lSkillAccountBegin,m_lSkillAccountEnd,lType,m_lTimer,");");

		//CScriptSys::getInstance()->PerformString(strScriptName);

		//�����еĲ���ѹ��ջ��
		lua_State* L = CScriptSys::getInstance()->GetLuaState();

		lua_settop(L,1);
		lua_pushvalue(L,LUA_GLOBALSINDEX);
		lua_pushstring(L, "SkillLoginCDKey");
		lua_gettable(L,-2);
		lua_pushnumber(L,m_lSkillAccountBegin);			//��������ʼ�˺� 
		lua_pushnumber(L,m_lSkillAccountEnd);			//�����˽����˺�
		lua_pushnumber(L,lType);							//��������(����)
		lua_pushnumber(L,m_lTimer);						//����Ϣ��ʱ����
		lua_pushnumber(L,m_lVersion);					//�������汾��
		lua_pushnumber(L,m_RegionID);					//����ID
		lua_pushnumber(L,m_lPosX);						//X����
		lua_pushnumber(L,m_lPosY);						//Y����
		lua_pushnumber(L,m_lHighAttack);					//��󹥻���
		lua_pushnumber(L,m_lLowAttack);					//��С������
		lua_pushstring(L,m_lWeapon.c_str());					//ʹ�õ�������ԭʼ��
		lua_pushnumber(L,m_lOccu);						//ְҵ���
		lua_pushnumber(L,m_lSkillRange);					//ʹ�õļ��ܵķ�Χ
		lua_pushnumber(L,m_lCustomSkillID1);				//�Զ���ļ���ID1
		lua_pushnumber(L,m_lCustomSkillID2);				//�Զ���ļ���ID2
		lua_pushnumber(L,m_lCustomSkillID3);				//�Զ���ļ���ID3
		lua_pushnumber(L,m_lCustomSkillID4);					//�Զ���ļ���ID4
		lua_pushboolean(L,m_bRandomPos);					//����������
		//
		lua_pushboolean(L,m_bIsCanACCAnswer);				//�Ƿ�Ҫ����
		//
		int nRet = lua_pcall(L,19,0,0);
		if(nRet != 0)
		{
			char strInfo[512]="";
			sprintf(strInfo,"error in function <SkillLoginCDKey>(L:%d,info:%s)",
				nRet,/*lua_tostring(L,-1,0));*/lua_tolstring(L,-1,0)); //�޸ģ�̷����
			PutDebugString(strInfo);
			MessageBox(strInfo,"����",MB_OK);
			lua_pop(L,1);
			return;
		}
	}
	else
	{
		//�ж���Ҫʹ�õļ��ܵķ�Χ
		if(m_bUseSkill1 == TRUE && m_bUseSkill2 == TRUE && m_bUseSkill3 == TRUE)
		{
			//ʹ��1ת2ת3ת����
			m_lSkillRange = 1;
		}
		else if (m_bUseSkill1 == TRUE && m_bUseSkill2 == TRUE && m_bUseSkill3 == FALSE)
		{
			//ʹ��1ת2ת����,��ʹ��3ת����
			m_lSkillRange = 2;
		}
		else if (m_bUseSkill1 == TRUE && m_bUseSkill2 == FALSE && m_bUseSkill3 == TRUE)
		{
			//ʹ��1ת3ת����,��ʹ��2ת����
			m_lSkillRange = 3;
		}
		else if (m_bUseSkill1 == TRUE && m_bUseSkill2 == FALSE && m_bUseSkill3 == FALSE)
		{
			//ʹ��1ת����,��ʹ��2ת3ת����
			m_lSkillRange = 4;
		}
		else if(m_bUseSkill1 == FALSE && m_bUseSkill2 == TRUE && m_bUseSkill3 == TRUE)
		{
			//ʹ��2ת3ת����,��ʹ��1ת����
			m_lSkillRange = 5;
		}
		else if (m_bUseSkill1 == FALSE && m_bUseSkill2 == TRUE && m_bUseSkill3 == FALSE)
		{
			//ʹ��2ת����,��ʹ��1ת3ת����
			m_lSkillRange = 6;
		}
		else if (m_bUseSkill1 == FALSE && m_bUseSkill2 == FALSE && m_bUseSkill3 == TRUE)
		{	
			//ʹ��3ת����,��ʹ��1ת2ת����
			m_lSkillRange = 7;
		}
		else if (m_bUseSkill1 == FALSE && m_bUseSkill2 == FALSE && m_bUseSkill3 == FALSE)
		{
			//��ʹ���κμ���,����
			MessageBox("û��ָ��ʹ�õļ��ܵķ�Χ,������ѡ��","����",MB_OK);
			return;
		}

		//�����жϲ�������������Ӧ���ص�ֵ
		long lType = 0;

		if ( m_lSkillAccountBegin <= 0)
		{
			MessageBox("���������Ҫ����0","��ʾ",MB_OK);
			return ;
		}
		if (m_lTimer >= 200000 || m_lTimer <= 200)
		{
			MessageBox("���ʱ�������200-200000ms֮��","��ʾ",MB_OK);
			return ;
		}

		//�����еĲ���ѹ��ջ��
		lua_State* L = CScriptSys::getInstance()->GetLuaState();

		lua_settop(L,1);
		lua_pushvalue(L,LUA_GLOBALSINDEX);
		lua_pushstring(L, "SkillLoginToken");
		lua_gettable(L,-2);
		lua_pushnumber(L,m_lSkillAccountBegin);			//����������
		lua_pushnumber(L,lType);							//��������(����)
		lua_pushnumber(L,m_lTimer);						//����Ϣ��ʱ����
		lua_pushnumber(L,m_lVersion);					//�������汾��
		lua_pushnumber(L,m_RegionID);					//����ID
		lua_pushnumber(L,m_lPosX);						//X����
		lua_pushnumber(L,m_lPosY);						//Y����
		lua_pushnumber(L,m_lHighAttack);					//��󹥻���
		lua_pushnumber(L,m_lLowAttack);					//��С������
		lua_pushstring(L,m_lWeapon.c_str());					//ʹ�õ�������ԭʼ��
		lua_pushnumber(L,m_lOccu);						//ְҵ���
		lua_pushnumber(L,m_lSkillRange);					//ʹ�õļ��ܵķ�Χ
		lua_pushnumber(L,m_lCustomSkillID1);				//�Զ���ļ���ID1
		lua_pushnumber(L,m_lCustomSkillID2);				//�Զ���ļ���ID2
		lua_pushnumber(L,m_lCustomSkillID3);				//�Զ���ļ���ID3
		lua_pushnumber(L,m_lCustomSkillID4);					//�Զ���ļ���ID4
		lua_pushboolean(L,m_bRandomPos);					//����������
		//
		lua_pushboolean(L,m_bIsCanACCAnswer);				//�Ƿ�Ҫ����
		//
		int nRet = lua_pcall(L,18,0,0);
		if(nRet != 0)
		{
			char strInfo[512]="";
			sprintf(strInfo,"error in function <SkillLoginCDKey>(L:%d,info:%s)",
				nRet,/*lua_tostring(L,-1,0));*/lua_tolstring(L,-1,0)); //�޸ģ�̷����
			PutDebugString(strInfo);
			MessageBox(strInfo,"����",MB_OK);
			lua_pop(L,1);
			return;
		}		
	}

}
void CSkillTest::OnBnClickedCheck2()
{
	UpdateData(TRUE);

	//�ж��Ƿ�ʹ��ʢ��ĵ�½��ʽ
	if (!m_bUseSDO)
	{
		GetDlgItem(IDC_STATIC_SKILL_START_CDKEY)->SetWindowText("��ʼ�ʺ�:gm");
		GetDlgItem(IDC_STATIC_SKILL_END_CDKEY)->ShowWindow(TRUE);
		GetDlgItem(IDC_SkillEndCDKey)->ShowWindow(TRUE);	
	}
	else
	{
		GetDlgItem(IDC_STATIC_SKILL_START_CDKEY)->SetWindowText("����������");
		GetDlgItem(IDC_STATIC_SKILL_END_CDKEY)->ShowWindow(FALSE);
		GetDlgItem(IDC_SkillEndCDKey)->ShowWindow(FALSE);
	}
}

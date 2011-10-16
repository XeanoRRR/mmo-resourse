/*
=========================================================================
��ע����δʹ�á�2010.10.28  BY:LGR
=========================================================================
*/

/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�CSkillTest.h
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

#pragma once

#include "afxcmn.h"
#include "GameTest.h"
#include "afxwin.h"
#include "..\Script\ScriptSys.h"

class CSkillTest : public CDialog
{
	DECLARE_DYNAMIC(CSkillTest)

public:

	CSkillTest(CWnd* pParent = NULL);
	virtual ~CSkillTest();

	enum { IDD = IDD_PROPPAGE_SKILL };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:

	//��Ҫѹ��ջ�е�Ԫ��
	long m_RegionID;									//����ID
	long m_lPosX;										//��ҵ�����X
	long m_lPosY;										//��ҵ�����Y
	long m_lHighAttack;									//��ҵ���󹥻���	
	long m_lLowAttack;									//��ҵ���С������
	string m_lWeapon;										//ѹ�������������
	long m_lOccu;										//�����˵�ְҵ
	long m_lSkillRange;									//ʹ�õļ��ܵķ�Χ

	long m_lCustomSkillID1;								//�Զ���ļ���1
	long m_lCustomSkillID2;								//�Զ���ļ���2
	long m_lCustomSkillID3;								//�Զ���ļ���3
	long m_lCustomSkillID4;								//�Զ���ļ���4

	long m_lSkillAccountBegin;							//��ʼ�˺�
	long m_lSkillAccountEnd;							//�����˺�
	long m_lTimer;										//��ʱ����ʱ����
	long m_lVersion;									//�������İ汾��
	string m_strIP;										//IP��ַ
	
	//��ʼ���Ի���
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedRadioAttackHigh();
	afx_msg void OnBnClickedRadioAttackLow();
	afx_msg void OnBnClickedRadioWeapon1();
	afx_msg void OnBnClickedRadioWeapon2();
	afx_msg void OnBnClickedRadioWeapon3();
	afx_msg void OnBnClickedRadioWeapon4();
	afx_msg void OnBnClickedRadioWeapon5();
	afx_msg void OnBnClickedRadioWeapon6();
	afx_msg void OnBnClickedRadioWeapon7();
	//
	afx_msg void OnBnClickedACCAnswer();   // By MartySa
	//
	afx_msg void OnBnClickedButtonSkillstart();

	CComboBox m_CombOccu;
	CComboBox m_CombIP;			

	afx_msg void OnCbnSelchangeComboOccu();
	afx_msg void OnCbnSelchangeComboSkillip();

	BOOL m_bUseSkill1;									//�Ƿ�ʹ��һת����
	BOOL m_bUseSkill2;									//�Ƿ�ʹ�ö�ת����
	BOOL m_bUseSkill3;									//�Ƿ�ʹ����ת����0

	BOOL m_bRandomPos;

	BOOL m_bIsCanACCAnswer;		//�Ƿ�������Ҵ��� add By MartySa 2009.7.10
	afx_msg void OnBnClickedCheck2();
	BOOL m_bUseSDO;
};

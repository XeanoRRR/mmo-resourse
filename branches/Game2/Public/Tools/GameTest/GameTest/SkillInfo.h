/**
============================================================
*  Copyright (c) 2009
*  All rights reserved
*  �ļ����ƣ�CSkillInfo.h
*  ���ܣ�������Ҽ�����ʾ�б�
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
#include "..\App\Player.h"
#include "afxwin.h"


#include <list>

using namespace std;


class CSkillInfo : public CDialog
{
	DECLARE_DYNAMIC(CSkillInfo)

public:
	CSkillInfo(CWnd* pParent = NULL);
	virtual ~CSkillInfo();

	enum { IDD = IDD_PROPPAGE_SKILL_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
private:

	//ְҵ�ṹ
	struct tagOccupation{
		string strOccupName;		//ְҵ����			
		long lOccupID;				//ְҵID
		string strOccupDesc;		//ְҵ����
	};

	//ְҵ���ܽṹ
	struct tagOccupSkill{
		string strSkillName;		// ���ܵ�����
		long lSkillID;				// ����ID
		long lSkillMaxLeve;			// ����ܹ����ļ���
		tagOccupSkill()
		{
			strSkillName = "";
			lSkillID = 0;
			lSkillMaxLeve = 0;
		}
	};
	list<tagOccupation*> m_OccupList;		// ���е�ְҵ�����б�;
	vector<tagOccupSkill*> m_vecOccSkill;				
public:

	//load ��ҵļ���
	BOOL LoadOccSkillListXml();
	BOOL Init(TiXmlElement* pElem);

	BOOL QuerySkill(tagPlayerSkill* pSkill);
	//bool IsBeing(CPlayer::tagSkill* pSkill);
	//load���м��� ���������npc

	BOOL LoadOneSkillXml();
	BOOL LoadSkillConfigXml(const char* str);


public:

	// ְҵ��������
	struct tagSkillOcc
	{
		string strOccName;						// ְҵ����
		DWORD	dwOccId;						// ְҵ���
	};
	// ���ܻ�����������
	struct tagLevelValue 
	{
		string	strKeyWord;						// �ؼ���
		DWORD	dwID;							// ����ID
		DWORD	dwValue;						// ֵ
		tagLevelValue()
		{
			strKeyWord = "";
			dwID = 0;
			dwValue = 0;
		}
	};

	// ���ܵȼ���Ϣ
	struct tagLevel
	{
		DWORD	dwSkill_Level;					// ���ܵȼ�
		string	strSkillLevelDesc;				// ���ܵȼ�����
		string	strSkillLevelDescEx;			// ���ܵȼ�����������������һ���Ļ�����Ϣ�ȣ�
		DWORD	dwSkillLevelAI;					// ���ȼ�����AI
		vector<tagLevelValue> vecStepSend;		// ���ͼ�����֤�׶�
		vector<tagLevelValue> vecStepBegin;		// ���ܿ�ʼ�׶�
		vector<tagLevelValue> vecStepRun;		// ����ʹ�óɹ��׶�
		vector<tagLevelValue> vecStepEnd;		// ���ܽ����׶�
		vector<tagLevelValue> vecStepSummon;	// �ٻ����ܽ׶�
		//vector<tagLevelValue> vecStepState;	// ���ܳ���״̬��Ч
		tagLevel()
		{
			dwSkill_Level = 0;
			strSkillLevelDesc = "";
			dwSkillLevelAI = 0;
		}
	};

	// ����������Ϣ
	struct tagOneSkill
	{
		DWORD	dwSkillID;						// ���ܱ��
		string	strSkillName;					// ��������
		string	strSkillDesc;					// ��������
		DWORD	dwSkillState;					// ����״̬��һ�㼼�ܡ��⻷�༼�ܣ�
		DWORD	dwSkillElement;					// ��������
		DWORD	dwSkillType;					// ��������(�������ͷ�)
		DWORD	dwSkillUseType;					// ����ʹ�����ͣ�����ħ����
		bool	bLockTarget;					// �Ƿ�����Ŀ��
		bool	bLockSkill;						// �Ƿ�����������
		bool	bHasAct;						// �Ƿ�ӵ�ж���
		vector<tagSkillOcc>		vecUseByOcc;		// ����ְҵ�б�
		vector<tagLevel>	vectorLevels;		// ���ܵȼ�����
		tagOneSkill()
		{
			dwSkillID = 0;
			strSkillName = "";
			strSkillDesc = "";
			dwSkillState = 0;
			dwSkillElement = 0;
			dwSkillType = 0;
			dwSkillUseType = 0;
			bLockSkill = false;
			bLockTarget = false;
			bHasAct = false;
		}
	};

private:

	//����ְҵ�ļ��ܽṹ��
	map<DWORD, tagOneSkill> m_mapSkillListXml;				// ���м������ݶ���
//	string m_strOneOccSkillName;
//	string m_strOneOccSkillDesc;
//	long m_lOneOccSkillID;
//	vector<tagOneOccupSkill*> m_vecOneOccSkillList;
//	void ReleaseOneOccSkill();

public:
	struct tagOneOccupSkill
	{
		string strOccSkillName;
		long lOccSkillID;
		long lOccMaxLevel;
		tagOneOccupSkill()
		{
			strOccSkillName = "";
			lOccSkillID = 0;
			lOccMaxLevel = 0;
		}
	};

public:

	static bool Init();

	CPlayer* m_pDisplaySkill; 
	CListCtrl m_CtrSkillList;
	virtual BOOL OnInitDialog();
	void SetDisplaySkill(CPlayer* pPlayer);
	CEdit m_edSkillInfo;
	CString m_strSkillInfo;
	//afx_msg void OnNMClickSkillList(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_OccSkillList;

public:

	//����
	CStatic m_ctrlOccup;
	//afx_msg void OnNMRclickSkillList(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnSkillinfoUseskill();
	//long GetAllSkillNum()	{ return (long)m_vecOccSkill.size(); }
};
//======================================================================================
/**
*	@file		PlayerList.h
*	@data		
*	@author		
*/
//======================================================================================

#pragma once

#include "../Common/BaseDef.h"
#include "../Common/PlayerDef.h"

/**
*	@class PlayerList
*
*	@brief 
*/

class PlayerList
{
public:
	/// ����
	PlayerList(void);

	/// ����
	~PlayerList(void);

public:
	/// �Ա�
	enum eSex
	{
		SEX_MALE,							// ��
		SEX_FEMALE,							// Ů
		SEX_NUMS							// �Ա�����
	};

	static string s_strOccupation[OCC_Max];
	static string s_strSex[SEX_NUMS];

	/// ��������
	struct tagLevelUp
	{
		DWORD	dwExp;						// �������辭��
		DWORD	dwExpMultiple;				// �������鱶��
		long	lPresentExpPerMinute;		// ÿ�������;���
		long	lMaxPresentExp;				// ������;���
		float	fFacBoonExpScale;			// ��ü��徭��ı���
		long	lBaseCP;					// CP����ֵ
	};

	/// ���ְҵ��������
	struct tagOccuLevelUp
	{
		DWORD dwExp;						//��������ľ���
	};

	/// ���ְҵ�޸�����
	struct tagOccupModifyProperty
	{
		float		fStr;				    //���� 
		float		fDex;				    //�� 
		float		fCon;				    //���� 
		float		fIntellect;				//��־ 
		float		fSpiritualism;			//���� 

		float		fStrAttRatio;			//������
		float		fAgiAttRatio;			//������
		float		fConDefRatio;			//�����

		float		fBlockStrRatio;			//������
		float		fBlockValueRatio;		//��ֵ��
		float		fParryStrRatio;			//�м�����		
		float		fParryAgiRatio;			//�м�����
		float		fParryValueRatio;		//�м�ֵ��

		float		fDodgeAgiRatio;			//��������
		float		fDodgeValueRatio;		//����ֵ��
		float		fCriAgiRatio;			//���ﱬ��
		float		fCriValueRatio;			//�ﱬֵ��
		float		fHitAgiRatio;			//�����б�
		float		fHitValueRatio;			//����ֵ��

		float		fBaseHit;				//��������
		WORD		fBaseHp;				//ְҵ��������
		WORD		fLvlUpHp;				//ÿ��������

		WORD		fBaseMp;				//ְҵ��������
		WORD		fLvlUpMp;				//ÿ��������

		DWORD		fMaxBaseEnergy;			//ְҵ������������
		float		fBaseEnergyRecoverValue;//���������ָ�
		DWORD		fBaseEnergyRecoverTime;	//���������ָ����ʱ��
		float		fFightEnergyRecoverValue;//ս�������ָ�
		DWORD		fFightEnergyRecoverTime; //ս�������ָ����ʱ��
		DWORD		fAttackEnergyValue;		//��ͨ��������ֵ����
		DWORD		fBlastEnertyValue;		//��������ֵ����
		DWORD		fBeenAttackEnergyValue;	//����������ֵ����
		DWORD		fBeenBlastEnergyValue;	//����������ֵ����
		
		float		fCriDamageStrRatio;		//�ﱬ�˺�����
		float		fCriDamageValueRatio;	//�ﱬ�˺�ֵ��
		float		fCriDefConstiRatio;		//�������������ʱ�
		float		fCriDefValueRatio;		//����������ֵ��
		float		fConstiDefRatio;		//���ʿ��Ա�
		float		fConstiDefValueRatio;	//���ʿ���ֵ��
		float		fConstiHpRatio;			//����������
		float		fMAtkIntellRatio;		//����ħ����
		float		fMCriDamageIntellRatio;	//ħ���˺�������
		float		fMCriDamageValueRatio;	//ħ���˺�ֵ%��
		float		fWillDefSpiritRatio;	//������־���Ա�
		float		fWillMpRatio;			//��������
		float		fWillDefValueRatio;		//��־����ֵ%��
		float		fMCriDefSpiritRatio;	//ħ���������Ծ����
		float		fMCriDefValueRatio;		//ħ����������ֵ%��
		float		fMCriWisdomRatio;		//�ǻ�ħ����
		float		fMCriValueRatio;		//ħ��ֵ%��
		float		fMDefSpiritRatio;		//����ħ�����Ա�
		float		fMDefValueRatio;		//ħ������ֵ%��
		float		fAbsorbWisdomRatio;		//רע�ǻ۱�
		float		fAbsorbValueRatio;		//רעֵ%��
		float		fBuffPowerIntellRatio;	//״̬����ǿ��������
		float		fBuffPowerValueRatio;	//״̬����ǿ��%��
		float		fMAtkSpeedWisdomRatio;	//ʩ���ٶ��ǻ۱�
		float		fMAtkSpeedValueRatio;	//ʩ���ٶ�ֵ%��
		long		lBasePKCP;				//ְҵ����CPֵ
		long		lLvlupAddPKCP;			//ְҵ����CPֵ
	};

	/// ��ʼ��Ʒ��װ���б�
	struct tagOrginEquip
	{
		uchar	lOccupation;			// ְҵ
		WORD	wPlacePos;				// ����λ��
		string	strOrginName;			// ��Ʒԭʼ��
	};

	/// ְҵ��ħ��������������� 
	struct tagOccuDefParam
	{
		float Def[OCC_Max];				//��ְҵ���������
		float MDef[OCC_Max];			//��ְҵ��ħ������
	};

	/// ְҵ��ħ�����﹥�������� 
	struct tagOccuAtkParam
	{	
		float Atk[OCC_Max];				//��ְҵ��������		
		float MAtk[OCC_Max];			//��ְҵ��ħ������
	};

private:
	
	static vector<tagLevelUp>				m_vectorPlayerExp;	// �����������ֵ����
	static DWORD							m_dwLevelPoint;		// ÿ��һ������ҷ���ĵ���
	static list<tagOrginEquip>				m_listOrginEquip;	// ��ʼװ���б�

	static vector<tagOccuLevelUp>	m_vcOccuLvlExp[OCC_Max];//ÿ��ְҵ�ȼ������б�


	static long m_lBasePropertyValue;//��������ֵ
	static float m_fPropertyLvlParam;//���Եȼ�ϵ��
	static map<DWORD, tagOccupModifyProperty>	m_mapOccuModifyPropertyList;     //ְҵ������
	static map<DWORD,tagOccuDefParam>	m_mapOccuDefParam;//��ְҵ�ķ�������ֵ

	static map<DWORD,tagOccuAtkParam>	m_mapOccuAtkParam;//��ְҵ�Ĺ�������ֵ

public:
	static tagOccupModifyProperty*	GetOccuModifyProperty(WORD level);         //
	static DWORD	GetPlayerExp(uchar lLevel)	{return (m_vectorPlayerExp.size()>lLevel && lLevel>=0)?m_vectorPlayerExp[lLevel].dwExp:0;}
	static DWORD	GetPlayerExpMultiple(uchar lLevel)	{return (m_vectorPlayerExp.size()>lLevel && lLevel>=0)?m_vectorPlayerExp[lLevel].dwExpMultiple:0;}
	static LONG64	GetPlayerTotalExp(uchar lLevel) 
	{ 
		LONG64 llRet = 0;
		if(m_vectorPlayerExp.size()>lLevel && lLevel>=0)
		{
			LONG64 llTmp = (LONG64)dwExpMutipleValue*m_vectorPlayerExp[lLevel].dwExpMultiple;
			llRet = llTmp + m_vectorPlayerExp[lLevel].dwExp;
		};
		return llRet; 
	}
	static LONG64   GetMaxExp(void)
	{
		LONG64 llRet = 0;
		if(m_vectorPlayerExp.size()>0)
		{
			LONG64 llTmp = (LONG64)dwExpMutipleValue*m_vectorPlayerExp[m_vectorPlayerExp.size()-1].dwExpMultiple;
			llRet = llTmp + m_vectorPlayerExp[m_vectorPlayerExp.size()-1].dwExp;
		};
		return llRet; 
	}
	static DWORD	GetLevelNum()	{return (DWORD)m_vectorPlayerExp.size();}
	static DWORD	GetOccuLevelNum(eOccupation byOccu)
	{
		if(byOccu>=0 && byOccu <OCC_Max)
			return (DWORD)(m_vcOccuLvlExp[byOccu].size());
		return 0;
	}

	static DWORD	GetPresentExpPerMinute(uchar lLevel) {return (m_vectorPlayerExp.size()>lLevel && lLevel>=0)?m_vectorPlayerExp[lLevel].lPresentExpPerMinute:0;}
	static DWORD	GetMaxPresentExp(uchar lLevel) {return (m_vectorPlayerExp.size()>lLevel && lLevel>=0)?m_vectorPlayerExp[lLevel].lMaxPresentExp:0;}
	static float	GetFacBoonExpScale(long lLevel){return ((long)(m_vectorPlayerExp.size())>=lLevel && lLevel>0)?m_vectorPlayerExp[lLevel - 1].fFacBoonExpScale:0.0f;}
	static float	GetBaseCP(long lLevel);
	
	static list<tagOrginEquip>*		GetOrginEquipList()	{return &m_listOrginEquip;}

public:
	static bool LoadOccuModifyProperty(const char* filename);            //���ְҵ�������Ա�
	//
	static bool LoadPlayerExpList(const char* filename);		// ���ļ���ȡexp�б�

	static bool LoadOccuLvlExpList();//װ��ְҵ�ȼ������б�

	static bool LoadOccuDefParam(const char* filename);		// ���ļ���ȡְҵ��������
	static bool LoadOccuAtkParam(const char* filename);		// ���ļ���ȡְҵ��������
	
	static bool AddToByteArray(vector<uchar>* pByteArray);		// ��ӵ�CByteArray
	static bool DecordFromByteArray(uchar* pByte, long& pos);	// ����

	//�õ�ְҵ�ȼ�ϵ��
	static float GetOccuLvlCoe(eOccupation byOccu,WORD wOccuLevel);
	static void GetDefParam(eOccupation byOccu,WORD wLeve,float& fDef,float& fMDef);
	static void GetAtkParam(eOccupation byOccu,WORD wLeve,float& fAtk,float& fMAtk);
	static void SetBaseFightProperty(tagPPro *pPPro,tagPFPro *pPFPro);
	static void SetFightProperty(tagPPro *pPPro,tagPFPro *pBasePFPro,tagPFPro *pExPFPro);

    // �õ���ǰְҵ�ȼ��ɷ���ĵ���
    static WORD GetOccuLvlPoint(eOccupation byOccu, WORD wOccuLevel);
	

	static double GetOccuValueRatio(const char* pszName,int nOccu);
};

#pragma once

// ״̬����
class COffciailRankConfig
{
public:
	//���ҹٽ���Ϣ
	struct tagOfficialRankConfig
	{
		int CreditMinValue;		//����������Сֵ
		int CreditMaxValue;		//�����������ֵ
		string strFighterOfficialRank;	//սʿ���ҹٽ�
		string strHunterOfficialRank;	//���ֹ��ҹٽ�
		string strDaoshiOfficialRank;	//��ʿ���ҹٽ�
		int	textColorA;			//����ɫ
		int textColorR;
		int textColorG;
		int	textColorB;
		int	textBkColorA;		//�ֱ�����ɫ
		int textBkColorR;
		int textBkColorG;
		int textBkColorB;
	};
	
public:
	COffciailRankConfig(void);
	virtual ~COffciailRankConfig(void);

private:
	static vector<COffciailRankConfig::tagOfficialRankConfig>	m_vecOfficialRackConfig;	//������ҹٽ׵�������Ϣ
public:
	static vector<COffciailRankConfig::tagOfficialRankConfig>&	GetOfficialRankVec(){return m_vecOfficialRackConfig;}
	static void LoadOfficialRankConfig();
	
};

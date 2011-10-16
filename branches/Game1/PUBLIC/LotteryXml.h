#pragma once

#include "tinystr.h"
#include "tinyxml.h"
#include <map>
#include <vector>
#include "tools.h"



//!		����ö��
enum	eLotteryErrCode
{
	eLEC_Succeed		= 0,		//! �ɹ�

	eLEC_NoEnoughSpace,				//! �����ռ䲻��
	eLEC_NoEnoughToken,				//! ���Ҳ���
	eLEC_NoEnoughTimeInterVal,      //! ʱ��������
	eLEC_NotInit,					//! û�н��г�ʼ��������Ҫ����ǿ������
};


//!		��������ö��
enum	eLotteryRouletteKind
{
	eLRK_Star,						//! ��
	eLRK_Moon,						//! ��
	eLRK_Sun,						//! ��

	MAXNUM_LRK
};


struct tagLotteryField   //�齱��λ
{
	int iFieldIndex;
	int iFieldLevel;
	long lProbability;
	tagLotteryField()
	{
		iFieldIndex = 0;
		iFieldLevel = 0;
		lProbability = 0;
	}
};

struct tagLotteryGoods
{
	int iGoodsIndex;
	int iGoodsNum;
	long lProbability;
	tagLotteryGoods()
	{
		iGoodsIndex = 0;
		iGoodsNum = 0;
		lProbability = 0;
	}
};

struct tagCoronaType
{
	int FieldLevel;
	long lProbability;
	tagCoronaType()
	{
		FieldLevel = 0;
		lProbability = 0;
	}
};

//�ýṹ���ڷֲ�����¼��ĸ��ʶ�
struct tagProbability
{
	DWORD Index;       // ��Ʒ����
	long ProbabilityMin;    // ���ڸ�������Сֵ
	long ProbabilityMax;    // ���ڸ��������ֵ
};

struct tagLottery
{
	int iFieldID;
	DWORD dwGoodsIndex;
	long lConstel;
	tagLottery()
	{
		iFieldID = 0;
		dwGoodsIndex = 0;
		lConstel = 0;
	}
};

class CLotteryXml
{
public:
	CLotteryXml(void);
	~CLotteryXml(void);
	bool LoadLotteryXml();
	bool LoadLotteryField(TiXmlElement* pElem);
	bool LoadLotteryGoods(TiXmlElement* pElem);	
	bool LoadCoronaType(TiXmlElement* pElem);	
	bool LoadConstySetup(TiXmlElement* pElem);
	DWORD RandGoodsFromLotteryGoods(int iLevel);              //�����ȡָ�����������õ���Ʒ
	DWORD GetFieldSize(){return m_VecLotteryField.size();};
	int GetFieldLevel(int FieldID);      //����һ��ָ����λ�ĵ���
	int RandFieldLevel(int iLevel,int iLevelConsty=-1);   //ͨ���������������һ���齱����(��������ʾ����Ӱ��ĵ���)
	bool GetFieldByLevel(int iLevel,vector<tagLotteryField>& VecFieldByLevel);   //��������иõ�����������λ
	bool LoadLotterySetup(TiXmlElement* pElem);

	long GetLotteryCredit(){return m_lCredit;};
	int GetLotteryNumMin(){return m_iLotteryNumMin;};           //���ʣ����Ʒ��С�������Ʋ���
	int GetLotteryNumLimit(){return m_iLotteryNumLimit;};		//���ʣ����Ʒ���Ʋ���.
	void SetRestNumLimit(int i ){ m_iResetNumLimit = i;};
	int  GetResetNumLimit(){return m_iResetNumLimit;};
	long GetConstyProbaToLevel(){ return m_lConstyProbaToLevel;};
	long GetConstyProbaToField(){return m_lConstyProbaToField;};
    long GetShinningInterval(){return m_lShinningInterval;};
	long GetPersonalCreditMax(){return m_lPersonalCreditMax;};  //��ø��˻�������
	long GetWorldCreditMax() {return m_lWorldCreditMax;};       //��������������
	DWORD GetTokenIndex() {return m_dwTokenIndex;};             //��ô�����Ʒ
	int GetTokenCost() {return m_iTokenCost;};                   //��ó齱����Ҫ�Ĵ�������
	int GetTokenCost_1(){return m_iTokenCost_1;};				//��ý�Ʒ����С�����Ʋ���ʱ����Ҫ�Ĵ�������
	int GetRefreshGoodsCost(){return m_iRefreshGoodsCost;};    //���������Ʒ�����������
	int GetRefreshConstCost(){return m_iRefreshConstCost;};    //����������������������
    DWORD GetTimeInterVal(){return m_dwTimeInterVal;};			//��ó齱ʱ����
	DWORD GetWorldCreditRefreshTime(){return m_dwWorldCreditRefreshTime;};//����������ˢ��ʱ��
	int GetLotteryGoodsNum(int iLevel,DWORD dwIndex);
	long GetCredit(){return m_lCredit;};

	bool IsLotteryShowSwitch(){return m_bLotteryShowSwitch;};   //��Ʒ�Ƿ���ʾ
	DWORD GetOneItemTime(){return m_dwOneItemTime;};							//����ת��һ���ʱ��
	DWORD GetRouletteCircles(){return m_dwRouletteCircles;};					//����ת��������Ȧ��
	DWORD GetAddPowerCircles(){return m_dwAddPowerCircles;};					//�����۵�������Ȧ��
	DWORD GetResetPrizesTime(){return m_dwResetPrizesTime;};					//���ý�Ʒ����Чʱ��
	DWORD GetResetConstellationsTime(){return m_dwResetConstellationsTime;};	//������������Чʱ��
	DWORD GetBlindTime(){return m_dwBlindTime;};								//�н���˸����Чʱ��

	bool CodeToByteArray(DBWriteSet& dbWriteSet);
	bool DecodeToByteArray(DBReadSet& dbReadSet);

private:
    //���������Ϣ
    void OutputErrorMsg(char* pMsg);
    //�����������
    void Reset();

private:
	vector<tagLotteryField> m_VecLotteryField;
	map<int,vector<tagLotteryGoods>> m_MapLotteryGoods;
	map<int,vector<tagCoronaType>> m_MapCoronaType;

	int m_iLotteryNumMin;			//ʣ����Ʒ��С�������Ʋ���
	int m_iLotteryNumLimit;			//ʣ����Ʒ���Ʋ���
	int m_iResetNumLimit;
	long m_lConstyProbaToLevel;    //����Ӱ��ĵ��εĸ���
	long m_lConstyProbaToField;    //����Ӱ�����λ�ĸ���
    long m_lShinningInterval;      //UI��˸��Ч�ļ��ʱ�䣨���룩

	long m_lCredit;			       //���γ齱����õĸ��˻���
	long m_lPersonalCreditMax;     //���˻�������
	long m_lWorldCreditMax;        //�����������
	DWORD m_dwTokenIndex;          //��������
	int m_iTokenCost;		       //�齱����Ҫ�Ĵ�������
	int m_iTokenCost_1;				//��Ʒ����С�����Ʋ���ʱ����Ҫ�Ĵ�������
	int m_iRefreshGoodsCost;        //������Ʒ����Ҫ�Ĵ�������
	int m_iRefreshConstCost;        //������������Ҫ�Ĵ�������

	DWORD m_dwTimeInterVal;		   //�齱ʱ����
	DWORD m_dwWorldCreditRefreshTime; //�������ˢ�µ�ʱ����
	bool m_bLotteryShowSwitch;    //��Ʒ��ʾ����

	DWORD m_dwOneItemTime;		   //����ת��һ���ʱ��
	DWORD m_dwRouletteCircles;		   //����ת��������Ȧ��
	DWORD m_dwAddPowerCircles;		   //�����۵�������Ȧ��
	DWORD m_dwResetPrizesTime;		   //���ý�Ʒ����Чʱ��
	DWORD m_dwResetConstellationsTime;	//������������Чʱ��
	DWORD m_dwBlindTime;				//�н���˸����Чʱ��
};

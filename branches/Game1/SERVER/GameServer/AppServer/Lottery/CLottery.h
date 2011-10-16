#ifndef _CLottery_H_
#define _CLottery_H_
#include "..\public\LotteryXml.h"

class CLottery
{
public:
	CLottery();
	~CLottery();

	int   GetHaveGoodsField();  //��õ�ǰ��Ʒ���л�����Ʒ����׵���
	void  RefreshVecLottery(); //ˢ����Ʒ��λ��ֵ
	DWORD GetGoodsFromField(int iFieldID);
	DWORD GetLotteryReFromField(int iFieldID);
	bool  CanDoLottery(CPlayer *pPlayer);//�����ж�
	bool  CanResetCredit(CPlayer *pPlayer);
	bool  DoLottery(DWORD& m_dwLotteryRe,int PlayerConsty,vector<tagLottery>& VecLottery,BYTE &FileId); //���г齱
	void  SetLotterylevel(int i){m_iLotterylevel=i;};
	int   GetLotterylevel(){return m_iLotterylevel;};
	bool  GetLotteryFieldInfo(vector<tagLottery>& VecLottery);
	DWORD GetLotteryRe(){return m_dwLotteryRe;};
	int   GetSurplusGoods();  
	int   GetConstNum();
	void  InitLottery();
	void  InitConstel();
	void  SetPlayerConsty(int i){m_iPlayerConsty = i;};
	int   FindFieldIDFromConsty(int iConsty);    //����һ��������Ӧ����λID
	long  GetLotteryNum(){return m_lLotteryNum;};
	long  GetLevelNum(int i){return m_lLevelNum[i];};
	void  ClaerStatistics();
	int   GetLotteryFieldRe(){return m_iLotteryFieldRe;}; //���س��е���λ
	int   GetLotteryGoodsNum(int iFieldId,DWORD dwGoodsIndex);   //���ĳ����λ��Ʒ����
	void  SetVecLottery(vector<tagLottery> VecLottery);  //������λ����������Ϣ

private:
	DWORD m_dwLotteryRe; 
	vector<tagLottery> m_VecLottery;
	CLotteryXml m_LotteryXml;
	int m_iLotterylevel;
	int ConstyArray[12];         // �����ĳ�ʼ����
	int m_iPlayerConsty;       // �������
	int m_iLotteryFieldRe;       //  ���е���λ

	long m_lLotteryNum;
	long m_lLevelNum[4];
};
#endif
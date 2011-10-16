#include "stdafx.h"
#include "../Player.h"
#include "../Goods/CGoodsFactory.h"
#include "CLottery.h"
class CPlayer;
CLottery::CLottery()
{
	m_VecLottery.clear();
	m_dwLotteryRe=0;
	m_iLotterylevel=0;

	for(int i=0;i<12;i++)
	{
		ConstyArray[i] = i+1;
		tagLottery tagLottery;
		tagLottery.iFieldID = i;
		m_VecLottery.push_back(tagLottery);
	}

	m_iPlayerConsty = 1;
	m_lLotteryNum=0;
	m_iLotteryFieldRe=0;

	for(int i=0;i<4;i++) 
	{
		m_lLevelNum[i] = 0;
	}
}

CLottery::~CLottery()
{
	m_VecLottery.clear();
}

void CLottery::InitLottery()
{
	//m_LotteryXml.LoadLotteryXml();
}

void CLottery::RefreshVecLottery()
{
	m_dwLotteryRe = 0;
	for(int i=0;i<GetInst(CLotteryXml).GetFieldSize();i++)
	{
		int iLevel = GetInst(CLotteryXml).GetFieldLevel(i);
		DWORD dwGoodsIndex = GetInst(CLotteryXml).RandGoodsFromLotteryGoods(iLevel);
		m_VecLottery[i].dwGoodsIndex = dwGoodsIndex;
	}
}

int CLottery::GetHaveGoodsField()
{
	vector<tagLotteryField> VecFieldByLevel;
	//���ȼ���4����
	GetInst(CLotteryXml).GetFieldByLevel(4,VecFieldByLevel);
	for(int i=0;i<VecFieldByLevel.size();i++)
	{
       DWORD dwGoods = GetGoodsFromField(VecFieldByLevel[i].iFieldIndex);
	   if(dwGoods!=0)
		   return 4;
	}
	//����3����
	VecFieldByLevel.clear();
	GetInst(CLotteryXml).GetFieldByLevel(3,VecFieldByLevel);
	for(int i=0;i<VecFieldByLevel.size();i++)
	{
       DWORD dwGoods = GetGoodsFromField(VecFieldByLevel[i].iFieldIndex);
	   if(dwGoods!=0)
		   return 3;
	}
	//����2����
	VecFieldByLevel.clear();
	GetInst(CLotteryXml).GetFieldByLevel(2,VecFieldByLevel);
	for(int i=0;i<VecFieldByLevel.size();i++)
	{
       DWORD dwGoods = GetGoodsFromField(VecFieldByLevel[i].iFieldIndex);
	   if(dwGoods!=0)
		   return 2;
	}
	//����1����
	VecFieldByLevel.clear();
	GetInst(CLotteryXml).GetFieldByLevel(1,VecFieldByLevel);
	for(int i=0;i<VecFieldByLevel.size();i++)
	{
       DWORD dwGoods = GetGoodsFromField(VecFieldByLevel[i].iFieldIndex);
	   if(dwGoods!=0)
		   return 1;
	}
    return 0;
}

DWORD CLottery::GetGoodsFromField(int iFieldID)
{
	for(int i=0;i<m_VecLottery.size();i++)
	{
		if(m_VecLottery[i].iFieldID==iFieldID)
		{
			DWORD dwGoodsIndex = m_VecLottery[i].dwGoodsIndex;
			return dwGoodsIndex;
		}
	}
	return 0;
}

DWORD CLottery::GetLotteryReFromField(int iFieldID)
{
	for(int i=0;i<m_VecLottery.size();i++)
	{
		if(m_VecLottery[i].iFieldID==iFieldID)
		{
			DWORD dwGoodsIndex = m_VecLottery[i].dwGoodsIndex;
		    m_VecLottery[i].dwGoodsIndex = 0;
			return dwGoodsIndex;
		}
	}
	return 0;
}

bool CLottery::DoLottery(DWORD& m_dwLotteryRe,int PlayerConsty,vector<tagLottery>& VecLottery,BYTE &FileId)
{
	SetPlayerConsty(PlayerConsty);
	long lFieldID=0;   //����н�����λID 
	int iLevel = 0;
	//�ҵ����������Ӧ����λID
    int iLuckField = FindFieldIDFromConsty(m_iPlayerConsty);
	//���Ҹ���λ�ĵ���
	int iLuckLevel = GetInst(CLotteryXml).GetFieldLevel(iLuckField);
	if(m_iLotterylevel!=0&&(m_iLotterylevel>=0&&m_iLotterylevel<3))
       iLevel = GetInst(CLotteryXml).RandFieldLevel(m_iLotterylevel,iLuckLevel);
	else
       iLevel = GetInst(CLotteryXml).RandFieldLevel(0,iLuckLevel);
	//��øõ�����������λ
    vector<tagLotteryField> VecLotteryFieldTemp;
	vector<tagLotteryField> VecLotteryField;
Begin:
	GetInst(CLotteryXml).GetFieldByLevel(iLevel,VecLotteryFieldTemp);
	//�޳��õ�����û����Ʒ����λ
	for(int i=0;i<VecLotteryFieldTemp.size();i++)
	{
		DWORD deGoodsIndex = GetGoodsFromField(VecLotteryFieldTemp[i].iFieldIndex);
		if(deGoodsIndex!=0)
		{
			VecLotteryField.push_back(VecLotteryFieldTemp[i]);
		}
	}
	if(VecLotteryField.size()==0)
	{
        VecLotteryFieldTemp.clear();
		iLevel = GetHaveGoodsField();    //����һ��������Ʒ����͵���
		if(iLevel != 0)
		   goto Begin;
		else
			return false;
	}

	vector<tagProbability> VecProbability;
	tagProbability tagProbabilityTemp;
	long lProbabilityValue = 0;   //��ֵ���ڼ�����ʷֲ��ռ�
	//���õ�����λ���ʷ�����ʼ�����
	for(int i=0;i<VecLotteryField.size();i++)
	{
		tagProbabilityTemp.Index = VecLotteryField[i].iFieldIndex;
		tagProbabilityTemp.ProbabilityMin = lProbabilityValue;
		if(iLuckField!=tagProbabilityTemp.Index)
		{
			tagProbabilityTemp.ProbabilityMax = tagProbabilityTemp.ProbabilityMin + VecLotteryField[i].lProbability;		
			lProbabilityValue=lProbabilityValue+VecLotteryField[i].lProbability;
		}else{
			tagProbabilityTemp.ProbabilityMax = tagProbabilityTemp.ProbabilityMin + (VecLotteryField[i].lProbability*GetInst(CLotteryXml).GetConstyProbaToField())/100;		
			lProbabilityValue=lProbabilityValue+(VecLotteryField[i].lProbability*GetInst(CLotteryXml).GetConstyProbaToField())/100;
		}
		VecProbability.push_back(tagProbabilityTemp);

	}

	long lProbabilityRe = random(lProbabilityValue); 

	for(int i=0;i<VecProbability.size();i++)
	{
		if(lProbabilityRe>=VecProbability[i].ProbabilityMin&&lProbabilityRe<VecProbability[i].ProbabilityMax)
		{
			lFieldID = VecProbability[i].Index;
			break;
		}
	}
	//����н���Ʒ
	m_dwLotteryRe = GetLotteryReFromField(lFieldID);
	FileId = (BYTE)lFieldID;
	m_iLotteryFieldRe = lFieldID;
	m_lLotteryNum++;
	int iShowLevel = GetInst(CLotteryXml).GetFieldLevel(lFieldID)-1;
	m_lLevelNum[iShowLevel]++;
	GetLotteryFieldInfo(VecLottery);
    return true;
}

bool CLottery::GetLotteryFieldInfo(vector<tagLottery>& VecLottery)
{
	VecLottery.clear();
	for(int i=0;i<m_VecLottery.size();i++)
	{
		VecLottery.push_back(m_VecLottery[i]);
	}
	return true;
}

int CLottery::GetSurplusGoods()
{
	int iNum = 0;
	for(int i=0;i<m_VecLottery.size();i++)
	{
		if(m_VecLottery[i].dwGoodsIndex!=0)
		{
			iNum++;
		}
	}
	return iNum;
}
int CLottery::GetConstNum()
{
	int iNum = 0;
	for(int i=0;i<m_VecLottery.size();i++)
	{
		if(m_VecLottery[i].lConstel!=0)
		{
			iNum++;
		}
	}
	return iNum;
}

void CLottery::InitConstel()
{
	//��ԭʼ�������м���齱�б�
   for(int i=0;i<12;i++)
   {
	   m_VecLottery[i].lConstel = ConstyArray[i];
   }
   //����ı�齱�б�������������
   for(int i=0;i<12;i++)
   {
	   //�������һ���±�
	   int iIndex = rand()%12;
	   //������ǰѭ��λ�ú�����±�λ�õ�ֵ
	   int iTemp = 0;
	   iTemp = m_VecLottery[i].lConstel;
	   m_VecLottery[i].lConstel = m_VecLottery[iIndex].lConstel;
	   m_VecLottery[iIndex].lConstel = iTemp;
   }
}

int CLottery::FindFieldIDFromConsty(int iConsty)
{
	for(int i=0;i<m_VecLottery.size();i++)
	{
		if(m_VecLottery[i].lConstel == iConsty)
			return m_VecLottery[i].iFieldID;
	}
	return -1;
}

void CLottery::ClaerStatistics()
{
	m_lLotteryNum=0;

	for(int i=0;i<4;i++)
	{
		m_lLevelNum[i] = 0;
	}
}

int CLottery::GetLotteryGoodsNum(int iFieldId,DWORD dwGoodsIndex)
{
	//���ָ����λ�ĵ���
	int iLevel = GetInst(CLotteryXml).GetFieldLevel(iFieldId);
	//��ø���λ�ϵ���Ʒ
	//DWORD dwGoodsIndex = GetGoodsFromField(iFieldId);

	return GetInst(CLotteryXml).GetLotteryGoodsNum(iLevel,dwGoodsIndex);
}

void CLottery::SetVecLottery(vector<tagLottery> VecLottery)
{
	for(int i=0;i<VecLottery.size();i++)
	{
		m_VecLottery[i].iFieldID = VecLottery[i].iFieldID;
		m_VecLottery[i].dwGoodsIndex = VecLottery[i].dwGoodsIndex;
		m_VecLottery[i].lConstel = VecLottery[i].lConstel;
	}
}
bool CLottery::CanDoLottery(CPlayer *pPlayer)
{
	//�ϴγ齱�����ڵ�ʱ����
	DWORD dwInterVal = timeGetTime() - pPlayer->GetLastLotteryTime();
	//�齱��ʱ����
	DWORD dwTimeInterVal = GetInst(CLotteryXml).GetTimeInterVal();
	//ʣ�౳������
	DWORD dwEmptySpaceNum = pPlayer->GetTotalEmptySpaceNum();
	//��ȡ��������
	DWORD dwTokenIndex = GetInst(CLotteryXml).GetTokenIndex();
	//���ݴ���������ȡ���ҵ�ԭʼ��
	const char* strGoodOriName = CGoodsFactory::QueryGoodsOriginalName(dwTokenIndex);
	if(strGoodOriName==NULL)
		return false;
	//��ȡ�����������
	int iTokenCost = GetInst(CLotteryXml).GetTokenCost();
	int iTokenCost_1 = GetInst(CLotteryXml).GetTokenCost_1();
	//��ҵĴ�������
	int iTokenNum = pPlayer->GetGoodsAmountByOriginName(strGoodOriName);
	//��Ʒ��λ��С��������
	int iLotteryNumMin = GetInst(CLotteryXml).GetLotteryNumMin();
	//��Ʒ��λ��������
	int iLotteryNumLimit = GetInst(CLotteryXml).GetLotteryNumLimit();

	if (dwInterVal < dwTimeInterVal)
	{
		CMessage msg(MSG_S2C_LOTTERY_Startup);
		msg.Add((long)eLEC_NoEnoughTimeInterVal);
		msg.SendToPlayer(pPlayer->GetExID());
		return false;
	}
	else if (iTokenNum < iTokenCost&&GetSurplusGoods()>iLotteryNumLimit)
	{
		CMessage msg(MSG_S2C_LOTTERY_Startup);
		msg.Add((long)eLEC_NoEnoughToken);
		msg.SendToPlayer(pPlayer->GetExID());
		return false;	 //����Ʒ��������
	}
	else if (iTokenNum < iTokenCost_1&&GetSurplusGoods()<=iLotteryNumLimit)
	{
		CMessage msg(MSG_S2C_LOTTERY_Startup);
		msg.Add((long)eLEC_NoEnoughToken);
		msg.SendToPlayer(pPlayer->GetExID());
		return false;	 //����Ʒ��������
	}
	else if (dwEmptySpaceNum< 1)
	{
		CMessage msg(MSG_S2C_LOTTERY_Startup);
		msg.Add((long)eLEC_NoEnoughSpace);
		msg.SendToPlayer(pPlayer->GetExID());
		return false;	//�����ռ䲻��
	}
	else if (GetSurplusGoods()<= iLotteryNumMin)
	{
		CMessage msg(MSG_S2C_LOTTERY_Startup);
		msg.Add((long)eLEC_NotInit);
		msg.SendToPlayer(pPlayer->GetExID());
		return false;//��Ʒ��λ������������û�г�ʼ��
	}
	else
	{
		//�������̵�����
		SetPlayerConsty(pPlayer->GetConst());
		//�۳�����
		if (GetSurplusGoods()<iLotteryNumLimit)
		{
			pPlayer->DelGoodsAmountByOriginName(strGoodOriName,iTokenCost_1);
			return true;
		}
		else
		{
			pPlayer->DelGoodsAmountByOriginName(strGoodOriName,iTokenCost);
			return true;
		}
	}
}
bool CLottery::CanResetCredit(CPlayer *pPlayer)
{
	DWORD dwCurrTime = time(NULL);
	static tm CurrTm;
	tm *pTm = localtime((time_t*)&dwCurrTime);
	memcpy(&CurrTm, pTm, sizeof(tm));
	pTm = localtime((time_t*)&(pPlayer->GetProperty().lLastResetActiveTime));

	if (pTm->tm_year == CurrTm.tm_year && pTm->tm_mon == CurrTm.tm_mon && pTm->tm_mday == CurrTm.tm_mday)
		return true;
	else
		return false;
}
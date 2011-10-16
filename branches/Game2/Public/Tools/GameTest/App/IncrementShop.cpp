


#include "stdafx.h"
#include "IncrementShop.h"
#include "../Net/Message.h"

CIncrementShop*	CIncrementShop::m_pIncrShop = NULL;

CIncrementShop::CIncrementShop()
{
	m_bOpend = false;
}

CIncrementShop::~CIncrementShop()
{

}

CIncrementShop* CIncrementShop::GetIncrShop()
{
	if( m_pIncrShop == NULL )
		m_pIncrShop = new CIncrementShop;
	return m_pIncrShop;
}

//	@ param1: ����ʽ(����,λ��,��ȯ)
// @ param2: ��Ʒ����
// @ ����һ���������Ʒ���� [11/10/2009 Square]
DWORD CIncrementShop::RandomOneGoodsIndex(DWORD dwShopID,DWORD dwGoodsListID)
{
	if( dwShopID <0 || dwShopID > eSHOP_NUM )	return -1;

	if( dwGoodsListID <0 || dwGoodsListID > eGOODS_NUM ) return -1;

	DWORD dwSize = m_vecIncrShopGoodsIndexList[dwShopID][dwGoodsListID].size();
	dwSize = rand()%(dwSize == 0 ? 1000:dwSize);

	//	return dwSize;

	return m_vecIncrShopGoodsIndexList[dwShopID][dwGoodsListID][dwSize];
}


void CIncrementShop::DecodeIncrShop(CMessage* pMsg)
{
	//	�Ƿ�ɹ���
	bool bOk = pMsg->GetByte() == 1 ? true : false;	//�Ƿ�ɹ��Ĵ�
	if( !bOk ) return;

	BYTE bFlag = pMsg->GetByte();	//�Ƿ��������

	if(bFlag)
	{
		m_bOpend = true;
		for(int i = 0;i < eSHOP_NUM; ++i)
		{
			for(int j=0; j <eGOODS_NUM;++j)
				m_vecIncrShopGoodsIndexList[i][j].clear();
		}

		//�̵����࣬ĿǰΪ�����̵꣨λ���̵� & �����̵� & ��ȯ�̵꣩

		// @ begin
		DWORD dwGoodsType(0),dwGoodsIndex(0);
		long lShopNum = pMsg->GetByte();
		// @ end

		for(int nShopID =0; nShopID<lShopNum; ++nShopID )
		{
			//��ϸ��Ʒ�б�
			long lGoodsSize = pMsg->GetLong();
			for(long l=0;l<lGoodsSize;++l)
			{
				dwGoodsType		= pMsg->GetDWord();
				dwGoodsIndex	= pMsg->GetDWord();

				pMsg->GetDWord();
				pMsg->GetDWord();
				pMsg->GetByte();
				char strTimeLimit[512] = {};
				pMsg->GetStr(strTimeLimit,512);
				pMsg->GetLong();
				pMsg->GetLong();

				m_vecIncrShopGoodsIndexList[nShopID][eGOODS_All].push_back(dwGoodsIndex);
			}


			//������Ʒ
			long lTop10GoodsIndexListSize = pMsg->GetLong();
			for(long j=0;j<lTop10GoodsIndexListSize; ++j)
			{
				dwGoodsType = pMsg->GetDWord();
				dwGoodsIndex = pMsg->GetDWord();

				m_vecIncrShopGoodsIndexList[nShopID][eGOODS_Top].push_back(dwGoodsIndex);
			}

			//��Ʒ�Ƽ�
			long lNewGoodsSize = pMsg->GetLong();
			for(long j = 0; j < lNewGoodsSize;++j)
			{
				dwGoodsType = pMsg->GetDWord();
				dwGoodsIndex = pMsg->GetDWord();

				m_vecIncrShopGoodsIndexList[nShopID][eGOODS_New].push_back(dwGoodsIndex);
			}

			//������Ʒ�б�
			long lNum = pMsg->GetLong();
			for(long j = 0; j < lNum; ++j )
			{
				char szGuideType[512] = {};
				pMsg->GetStr(szGuideType,512);

				long lGuideGoodsIndexListSize = pMsg->GetLong();
				for(long l = 0; l < lGuideGoodsIndexListSize; ++l )
				{
					dwGoodsType = pMsg->GetDWord();
					dwGoodsIndex = pMsg->GetDWord();

					m_vecIncrShopGoodsIndexList[nShopID][eGOODS_Guide].push_back(dwGoodsIndex);
				}
			}

			//ɸѡ��Ʒ�б�
			switch( nShopID )
			{
			case eSHOP_PLANES:
				{
					long lGoodsClassSize = pMsg->GetLong();
					for(long c = 0; c < lGoodsClassSize; ++c )
					{
						long lTypeSize = pMsg->GetLong();
						for(long cc = 0; cc < lTypeSize; ++cc )
						{
							char szFilterType[512] = {};
							pMsg->GetStr(szFilterType,512);
							long lFilterSize = pMsg->GetLong();
							for(long n =0; n < lFilterSize; ++n )
							{
								dwGoodsType	= pMsg->GetDWord();
								dwGoodsIndex = pMsg->GetDWord();
							}
						}
					}
				}
				break;
				//	Ԫ��������
			case eSHOP_STAR:
				{
					long lFilterSize = pMsg->GetLong();
					for(long c1 = 0;c1 < lFilterSize; ++c1)
					{
						long lTypeSize = pMsg->GetLong();
						for(long c2 = 0; c2 < lTypeSize; ++c2 )
						{
							char szFilterType[512] = {};
							pMsg->GetStr(szFilterType,512);

							long lFilterListSize = pMsg->GetLong();
							for(long c3= 0; c3 < lFilterListSize; ++c3 )
							{
								dwGoodsType = pMsg->GetDWord();
								dwGoodsIndex = pMsg->GetDWord();
							}
						}
					}
				}
				break;
			case eSHOP_TICKET:
				{
					long lFilterSize = pMsg->GetLong();
					for(long c1 = 0;c1 < lFilterSize; ++c1)
					{
						long lTypeSize = pMsg->GetLong();
						for(long c2 = 0; c2 < lTypeSize; ++c2 )
						{
							char szFilterType[512] = {};
							pMsg->GetStr(szFilterType,512);

							long lFilterListSize = pMsg->GetLong();
							for(long c3= 0; c3 < lFilterListSize; ++c3 )
							{
								dwGoodsType = pMsg->GetDWord();
								dwGoodsIndex = pMsg->GetDWord();
							}
						}
					}
				}
				break;
			}//	end switch
		}
	}
}
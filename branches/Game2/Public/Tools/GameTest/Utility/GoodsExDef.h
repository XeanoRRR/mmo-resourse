
#pragma once

//!		��Ʒ��չ������������
enum ePageType{
	PT_DisassembleWindow,		//! �ֽⴰ��
	PT_PrescriptionWindow,	//! �ϳɴ���
	PT_BindWindow,			//! �󶨴���
	PT_FrostWindow,			//! ���ᴰ��
	PT_UpgradeWindow,			//! ��������
	PT_PlusEvilWindow,		//! ħ������
	PT_EvilBackoutWindow,		//! ħ���������
	PT_EvilSynthesizeWindow,	//! ħ�����Ϻϳɴ���
	PT_DepotWindow,			//! �ֿⴰ��
	/////XY 2008.1.21/////
	PT_CardFuntionWindow,		//!	�⿨������ҳ��	
	PT_PlayerTradeWindow,		//! ��ҽ��׹���ҳ��
	//////////////////////
	PT_Number,
	PT_NULL,
};

struct tagEnchaseSession
{
	CGUID	m_SessionGuid;
	long	lPlayerID;
	BYTE	iPageType;
	tagEnchaseSession() : m_SessionGuid(CGUID::GUID_INVALID), lPlayerID(0), iPageType(0)
	{}
};

struct tagEnchaseGoods 
{
	CGoods* pGoods;
	CGoods* pBaoShi;
	CGoods* pCard[5];
	tagEnchaseGoods()
	{
		pGoods = NULL;
		pBaoShi = NULL;
		for(int i=0;i<5;i++)
		{
			pCard[i] = NULL;
		}
	}
};

enum CardEnchasePage  
{
	CardEnchasePage_BROACH = 1,					//���ҳ��
	CardEnchasePage_JAM,							//�¿�ҳ��
	CardEnchasePage_ENCHASE,						//��Ƕҳ��
	CardEnchasePage_BACKOUT,						//��ҳ��

	CardEnchasePage_COUNT							//ҳ������
};

// �ֿ���ӱ�����λ����
#define		DEPOT_SUBPACK_MAXNUM			4U
// �ֿ����չ��λ ��λ��
#define		DEPOT_SUBPACK_BASEPOS			10001U


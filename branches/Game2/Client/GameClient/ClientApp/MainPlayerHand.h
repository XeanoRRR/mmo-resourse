#pragma once
#include "../ClientApp/player.h"
#include "../ClientApp/Goods/goods.h"
#include "../ClientApp/other/piclist.h"
class CMainPlayerHand
{
public:

	// �¼�����������
	enum PlaceType
	{
		PlaceType_BackpacksPage = 0,		//�����������ӱ�����
		PlaceType_EquipmentPage,			//װ����
		PlaceType_Fastfield,				//MainBar�����
		PlaceType_NpcShopPage,				//NPC�̵�
		PlaceType_ExchangePage,				//���׽���
		PlaceType_PlayerShopPage,			//�����̵�
		PlaceType_CardMutifunctionalPage,	//��Ƭ��Ƕ����
		PlaceType_PetSkillPage,				//��������м��ܽ���
		PlaceType_PetStrenthenPage,			//����ǿ������
		PlaceType_Num
	};

	// ������08.11.19��
	struct tagSkillEffect
	{
		CGUID gEffectID;				// ������Ч��GUID
		DWORD dwSkillID;				// ��Ӧ�ļ���ID
		tagSkillEffect()
		{
			gEffectID = CGUID::GUID_INVALID;
			dwSkillID = 0;
		}
	};

	struct tagHand 
	{
		DWORD dwSourcePlace;			//�ֱ�����ʱ�Ĵ����ص�
		DWORD dwObjectTypeOnHand;		//��������ʾ��Ŀ�������
		DWORD dwGoodsIndex;				//Ҫ��ʾ����Ʒͼ��ID����ʱ����ֻ��һ��ͼ�꣬��û�д洢ʵ�ʵ���Ʒ���ݣ�
		DWORD dwSkillIndex;				//�������ü��ܵı��
		WORD  wIconType;				//ͼ�������
		DWORD dwPetPicID;				// ����ͼ��ID
		CPet* handPet;					// �������ָ��
		CPlayer::tagGoods HandGoods;	//����������Ʒ��ָ��
		tagSkillEffect	stSkillEffect;	// ���Ϸ�Χ��Ч��Ϣ
		tagHand()
		{
			dwSourcePlace = PlaceType_Num;
			wIconType = -1;
			dwGoodsIndex = 0;
			dwObjectTypeOnHand = 0;
			dwSkillIndex = 0;
		}
	};
	CMainPlayerHand();
	
	// �����
	tagHand* GetMainPlayerHand() {return &m_Hand;}
	
	// ��������������������
	DWORD	 GetObjectTypeOfMainPlayerHand()	{return m_Hand.dwObjectTypeOnHand;}
	
	// �������������Ʒ
	CPlayer::tagGoods* GetTGoodsOfMainPlayerHand()	{return &m_Hand.HandGoods;}
	
	// �������������Ʒ��pGoods
	CGoods*	 GetPGoodsOfMainPlayerHand()	{return m_Hand.HandGoods.pGoods;}
	
	// ����������ü��ܵ�Index
	DWORD	 GetSkillOfMainPlayerHand()		{return m_Hand.dwSkillIndex;}

	// ��ȡ�������ó���ͼ��ID
	DWORD GetPetPicIDOfMainPlayerHand()		{ return m_Hand.dwPetPicID; }

	// ��ȡ�������ó������ָ��
	CPet* GetPetOfMainPlayerHand()			{ return m_Hand.handPet; }

	// ���������ʾ����Ʒ��Index
	DWORD	 GetGoodsIndexOfMainPlayerHand()	{return m_Hand.dwGoodsIndex;}

	// ��ȡ��ǰ�ֱ�����ʱ�Ĵ����ص�����
	DWORD	 GetSourcePlaceTypeOfMainPlayerHand() {return m_Hand.dwSourcePlace;}
	
	// ������
	void SetMainPlayerHand(tagHand* pTagGoods) {m_Hand = *pTagGoods;}
	
	// ����������ʾ����Ʒ��Index(û��ʵ�ʵ���Ʒ��Ϣ),�ڶ���������¼������ʱ�Ĵ����ص�
	void SetMainPlayerHandByGoodsIndex(DWORD dwGoodsIndex,DWORD dwSourcePlace = PlaceType_Num);
	
	// ���������õ���Ʒ
	void SetMainPlayerHandByGoods(bool bChanged,long lNum,DWORD lPos,CGoods *pGood,DWORD lType = 0);
	
	// ���������õļ���
	void SetMainPlayerHandBySkill(DWORD dwSkillIndex,WORD wIconType=CPicList::PT_SKILL_ICON,DWORD dwSourcePlace = PlaceType_Num);

	// ���������õĳ���
	void SetMainPlayerHandByPet(DWORD dwPetPicID, CPet* pet,WORD wIconType=CPicList::PT_PET_ICON, DWORD dwSourcePlace = PlaceType_Num);
	
	// ������������������ͼ��
	void ClearMainPlayerHand();

	// ����������������Ʒ��ȥ��Ŀ��ص�lExtendID
	void SetDesExtendIDofGoods(long lExtendID) {m_lDesExtenID = lExtendID;}
	
	// ���������������Ʒ��ȥ��Ŀ��ص�lExtendID
	long GetDesExtendIDofGoods()	{return m_lDesExtenID;}
	
	// ���������������Ʒ����Ӱ״̬
	void DelShadowStateOfGoods();
	
	// �ֵ���ʾ
	bool Display(long lGraphicsID,long x, long y, DWORD color = 0xffffffff, RECT *rectClip=NULL);
	
	// �ַ�����Ʒʱ���͵���Ϣ��Ŀǰֻ�����ϵ���Ʒ������Ч��
	void SendPutDownMes(long SorceType/*��ʼ������*/,const CGUID& SorceGUID/*��ʼ���GUID*/,long lSorceObjectExtendID/*��ʼ������ID*/,long lSorceObjectPos/*��ʼ��λ��*/,
						long DesType/*Ŀ�ĵ�����*/,const CGUID& DesGUID/*Ŀ�ĵ�GUID*/,long lDesObjectExtendID/*Ŀ�ĵ�����ID*/,long lDesObjectPos/*Ŀ�ĵ�λ��*/,
						long lObjectType/*Ŀ������*/,const CGUID& ObjectGUID/*Ŀ��GUID*/,long lObjectNum/*Ŀ������*/);

	/*
	 * ���λ�õ���Ч��ʾ��ط���
	 * 08.11.19�������
	 */
	// ������Ч
	void SetMainPlayerHandByEffect(DWORD dwSkillID,DWORD dwEffectID);
	// ����������Ч�Ļ���λ��
	bool DisplayEffectInHand();
	// ������Ϸ�Χ����
	DWORD GetAreaSkillID()	{return m_Hand.stSkillEffect.dwSkillID;}
private:
	tagHand m_Hand;
	long m_lDesExtenID;
	//long m_lDesPos;
};
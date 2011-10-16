
//�����Player��صĽṹ��ö��
#ifndef PLAYER_PROPERTYDEF_H
#define PLAYER_PROPERTYDEF_H

#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T
#endif

#include "BaseDef.h"
#include "StateDef.h"

//���������(��������)
typedef struct tagPlayerProperty
{
	eOccupation	byOccu;					//ְҵ
	eDeputyOccu byAssOccu;				//��ְҵ
	eConst		byConst;				//����
	BYTE		bySex;					//�Ա�(0:�� 1:Ů) 
	BYTE		byClass;				//��������
	
	long		lHairPic;				//ͷ��ͼƬID 
	long		lFacePic;				//�沿ͼƬID 

	DWORD		dwHp;					//��ǰ����ֵ 
	WORD		wMp;					//��ǰħ��ֵ 
	WORD		wEnergy;				//��ǰ����ֵ 
	WORD		wStamina;				//��ǰ����	
	WORD		wLevel;					//�ȼ� 

	DWORD		dwExpMultiple;			//��ǰ����ֵ����
	DWORD		dwExp;					//��ǰ����ֵ
	DWORD		dwPresentExp;			//���;���
	DWORD		dwUpgradeExpMultiple;	//�������辭�鱶��
	DWORD		dwUpgradeExp;			//�������辭��ֵ
	BYTE		byOccuLvl[OCC_Max];		//ÿ��ְҵ�ĵȼ�
	DWORD		dwOccuExp[OCC_Max];		//ÿ��ְҵ�ľ���
	DWORD		dwCurOccuUpgradeExp;	//��ǰְҵ����������

	BYTE		byRankOfNobility;		//��λ�ȼ�
	DWORD		dwRankOfNobCredit;		//��λ������ֵ
	DWORD		dwBatakCredit;			//����������
	DWORD		dwMedalScores;			//ѫ�·���
	BYTE		byRankOfMercenary;		//Ӷ���ȼ�
	DWORD		dwMercenaryCredit;		//Ӷ��������ֵ

	BYTE		byCountry;				//����ID
	long		lCountryContribute;		//�Թ��ҵĹ���ֵs
	DWORD		dwSpouseID;				//��ż���ID 
	DWORD		dwSpouseParam;			//����ż�Ĺ�ϵֵ
	BYTE		byBusinessLevel;		//��ҵ�ȼ�
	BYTE		byIsBusinessMan;		//�Ƿ�������
	DWORD		dwBusinessExp;			//��ҵ����
	DWORD		dwArtisanCredit;		//��������
	BYTE		byArtisanLevel;			//�����ȼ�
	DWORD		dwArtisanExp;			//��������
	DWORD		dwChurchCredit;			//�̻�����
	DWORD		dwZanDoHunterCredit;	//�޶���������ֵ
	DWORD		dwPkCount;				//���PKɱ����������
	DWORD		dwPkValue;				//���PKֵ(����ֵ)
	DWORD		dwPVPCount;				//���PVPɱ���������
	DWORD		dwPVPValue;				//���PVPֵ(����ֵ)
	DWORD		dwHotKey[24];			//�Ƚ�(F1~F12,1~+) 
	WORD		wPKOnOff;				//PK����(b1:ȫ�屣��,b2:��ӱ���,b4:���ᱣ��,b8:��������,b16:��������,b32:��Ӫ����,b64.��������,b128������ʾ��b256�Զ�ѡ�ˣ�b512�Զ�ѡ��,)
	DWORD		dwOccuSP[OCC_Max];		//ÿ��ְҵ�ļ��ܵ���
	DWORD		dwLastExitGameTime;		//��һ���˳���Ϸ��ʱ��
	bool		bShowCountry;			//��ʾ���ұ�־

	long		lCurrBaseActive;		//! ʣ�����CP
	long		lCurrExActive;			//! ʣ����չCP
	ULONG		lLastResetActiveTime;	//! �ϴ�����CPʱ��

	long		lCurPKCP;				///< ��ǰPK CPֵ
	long		lMarriageStep;			//! ��鲽��
}tagPPro;

//! ����Ԫ������
typedef struct tagPlayerElementProperty
{
	long		uArrPro[AD_Max];

	tagPlayerElementProperty(void){memset(this, 0, sizeof(tagPlayerElementProperty));}
}tagPlayerElementPro;


// �����ս������
typedef struct tagPlayerFightProperty
{
	long		dwMaxHp;				//�������� 
	short		wMaxMp;					//�������� 
	short		wMaxStamina;			//�������� 
	short		wMaxEnergy;				//�������� 
	short		wHpRecoverSpeed;		//�����ָ��ٶ�
	short		wFightHpRecoverSpeed;	//ս�������ָ��ٶ�
	short		wMpRecoverSpeed;		//�����ָ��ٶ�
	short		wFightMpRecoverSpeed;	//ս�������ָ��ٶ�
	short		wHpRecoverRatio;		//HP�ָ�����
	///////////////////////////
	short		wEnerRecoverSpeed;		//�����ָ��ٶ�
	short		wFightEnerRecoverSpeed;	//ս�������ָ��ٶ� 	
	///////////////////////////
	short		wStamRecoverSpeed;		//�����ָ��ٶ� 
	short		wStrenth;				//���� 
	short		wAgility;				//���� 
	short		wConsti;				//���� 
	short		wIntell;				//���� 
	short		wWisdom;				//�ǻ� 
	short		wSpirit;				//���� 
	short		wFightBlock;			//�� 
	short		wParry;					//�м�	
	short		wDodge;					//����
	short		wCharm;					//���� 
	short		wHit;					//���� 
	short		wAbsorb;				//רע 
	long		wDef;						//LONG �������
	short		wShieldDef;				//�����������
	long		wMdef;						//LONG ħ������
	long		wCri;						//LONG ��������
	short		wMAtkSpeed;				//ʩ���ٶ�
	short		wBuffPower;				//Buff����ǿ��
	long		wMinAtk;					//LONG ��С������
	long		wMaxAtk;					//LONG ��󹥻���
	long		wMAtk;						//LONG ħ��������
	long		wMCri;						//LONG ħ��������
	short		wBloodSuk;				//����������
	short		wManaSuck;				//����������
	short		wEnergySuck;			//����������
	short		wWillDef;				//��־����
	short		wConstiDef;				//���ʿ���
	short		wCriDef;				//����������
	short		wMCriDef;				//ħ����������
	short		wCriDamage;				//�������˺�
	short		wMCriDamage;			//ħ�������˺�	
	short		wPhysicImmunity;		//��������
	short		wMagicImmunity;			//ħ������
	short		wImmunity;				//����
	short		wPierce;				//��͸
	short		wLuck;					//����
	short		wDeBuffDef[DET_Max];	//Debuff����
	short		wElemDef[ET_Max];		//����Ԫ�ؿ���
	short		wClassDam[C_Max];		//�Ը��������˺�
	short		wElemDam[ET_Max];		//�Ը���Ԫ���˺�
	short		wElemAtk[ET_Max];		//����Ԫ�صĹ���ֵ

	long		lMaxPKCP;				///< ���PK CPֵ 

}tagPFPro;

//��ʾ�뿤���Ѻù�ϵ��
struct tagFriendshipCounty
{
	DWORD	dwCountyID;				//��ID
	DWORD	dwFrienDeg;				//�Ѻö�
};
//���渽������
typedef struct tagPlayerAppendProperty
{
	bool		bIsCharged;					// �Ƿ��ֵ
	BYTE		bShowFashion;				// ͷ��,����,���׵���ʾ״̬
	char		strAccount[ACCOUNT_SIZE];	// �ʺ�
	char		strTitle[TITLE_SIZE];		// �ƺ�	
}tagPAPro;

enum AREA_CREDIT_LEVEL
{
	CREDIT_LEV_NORMAL=1,
	//CREDIT_LEV_
};

enum SAVE_DETAIL_ATTR_TYPE
{
	SAVE_DETAIL_NONE				= 0,
	SAVE_DETAIL_ATTRPROPERTY		= 0x00000001,	// ��������:��Ǯ,�ȼ�...��
	SAVE_DETAIL_ATTREQUIPMENTGOODS	= 0x00000002,	// װ����Ʒ
	SAVE_DETAIL_ATTRPACKGOODS		= 0x00000004,	// ��������Ʒ
	SAVE_DETAIL_ATTRSUBPACKGOODS1	= 0x00000008,	// �ӱ���1��Ʒ
	SAVE_DETAIL_ATTRSUBPACKGOODS2	= 0x00000010,	// �ӱ���2��Ʒ
	SAVE_DETAIL_ATTRSUBPACKGOODS3	= 0x00000020,	// �ӱ���3��Ʒ
	SAVE_DETAIL_ATTRSUBPACKGOODS4	= 0x00000040,	// �ӱ���4��Ʒ
	SAVE_DETAIL_ATTRSUBPACKGOODS5	= 0x00000080,	// �ӱ���5��Ʒ
	SAVE_DETAIL_ATTRWALLETGOODS		= 0x00000100,	// Ǯ����Ʒ
	SAVE_DETAIL_ATTRSILVERGOODS		= 0x00000200,	// ������Ʒ
	SAVE_DETAIL_ATTRDEPOTGOLD		= 0x00000400,	// �ƽ�ֿ�
	SAVE_DETAIL_ATTRDEPOTSILVER		= 0x00000800,	// ���Ҳֿ�
	SAVE_DETAIL_ATTRPRIMARY			= 0x00001000,	// ���ֿ�
	SAVE_DETAIL_ATTRSUBDEPOT1		= 0x00002000,	// �Ӳֿ�1
	SAVE_DETAIL_ATTRSUBDEPOT2		= 0x00004000,	// �Ӳֿ�2
	SAVE_DETAIL_ATTRSUBDEPOT3		= 0x00008000,	// �Ӳֿ�3
	SAVE_DETAIL_ATTRSUBDEPOT4		= 0x00010000,	// �Ӳֿ�4
	SAVE_DETAIL_ATTRBUSINESSPACK	= 0x00020000,	// ��ҵ����
	SAVE_DETAIL_ATTRVARLIST			= 0x00040000,	// �����б�
	SAVE_DETAIL_ATTRQUESTLIST		= 0x00080000,	// �����б�
	SAVE_DETAIL_ATTRSKILLLIST		= 0x00100000,	// �����б�
	SAVE_DETAIL_ATTRSTATELIST		= 0x00200000,	// ״̬�б�
	SAVE_DETAIL_ATTRDEPUTYOCCU		= 0x00400000,	// ��ְҵ
	SAVE_DETAIL_ATTRMERQUESTLIST	= 0x00800000,	// ٸ�������б�
	SAVE_DETAIL_PETS				= 0x01000000,	// �����б�
    SAVE_DETAIL_INNERHOOK           = 0x02000000,   // �һ�����,����ֻ�����ȶ��һ�ʱ��
	
	//SAVE_DETAIL_ATTRMAILLIST		= 0x00800000,	// �ʼ��б�
	SAVE_DETAIL_ALL					= 0xFFFFFFFF,	// �����б�
};													

//����ֵ����ֵ
const static DWORD dwExpMutipleValue = 2000000000;

#endif												
// ״̬�б�
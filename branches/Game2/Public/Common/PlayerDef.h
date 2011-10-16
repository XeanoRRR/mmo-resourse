
//�����Player��صĽṹ��ö��
#ifndef PLAYER_PROPERTYDEF_H
#define PLAYER_PROPERTYDEF_H

#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T
#endif

#include "BaseDef.h"
#include "StateDef.h"

// TODO: ������������������µ���
enum eNOTIFY_POS
{
    eNOTIFYPOS_LEFT_BOTTOM=1,//����
    eNOTIFYPOS_RIGHT_BOTTOM,//����
    eNOTIFYPOS_CENTER,//�м�
    eNOTIFYPOS_SYSTEM,//ϵͳ
};

const DWORD NOTIFY_COLOR_RED=0xffff0000;
const DWORD NOTIFY_COLOR_YELLOW=0xffffff00;
const DWORD NOTIFY_COLOR_WHITE=0xffffffff;

//���������(��������)
typedef struct tagPlayerProperty
{
	eOccupation	byOccu;					//ְҵ(����)
	eDeputyOccu byAssOccu;				//��ְҵ
	uchar		bySex;					//�Ա�(0:�� 1:Ů) 
	uchar		byHairPic;				//ͷ��ͼƬID 
	uchar		byFacePic;				//�沿ͼƬID 

	ulong		dwHp;					//��ǰ����ֵ 
	ushort		wMp;					//��ǰħ��ֵ 
	ushort		wEnergy;				//��ǰ����ֵ(����) 
	ushort		wStamina;				//��ǰŭ��	
	ushort		wLevel;					//�ȼ� 

    ulong		dwExp;					//��ǰ����ֵ
	ulong		dwExpMultiple;			//��ǰ����ֵ����	
	ulong		dwPresentExp;			//���;���
	ulong		dwUpgradeExpMultiple;	//�������辭�鱶��
	ulong		dwUpgradeExp;			//�������辭��ֵ
	uchar		byOccuLvl[OCC_Max];		//ÿ��ְҵ�ĵȼ�
	ulong		dwOccuExp[OCC_Max];		//ÿ��ְҵ�ľ���
	ulong		dwCurOccuUpgradeExp;	//��ǰְҵ����������

    ushort      wTaoismLvl;             // ���еȼ�
    ulong       dwTaoismExp;            // ��Ϊֵ�������������еȼ�
    ulong       dwRelationVal;          // ����ֵ�������еĵ͵ȼ���ɫ����ʱ��һ����Χ�ڸ�����һ���ȼ���ɫ�ܻ�õ�ֵ�����ڽ������ѵ�ֵ
    ulong       dwHiddenVirtue;         // ���� �����Ϊ���ۼƵ���ֵ����Ӱ��ĳЩ�¼��ĳɹ���
    ushort      wVigour;                // ��������������ģ���ҩ���������ƣ�
    ushort      wThew;                  // ��������������ģ�������ߡ���������Ʒ�������ȣ�

    short       wPoint2Str;                   //��������
    short       wPoint2Dex;                   //������
    short       wPoint2Con;                   //�������
    short       wPoint2Intellect;             //������־
    short       wPoint2Spiritualism;          //��������

	uchar		byCountry;				        //����ID
	long		lCountryContribute;		        //�Թ��ҵĹ���ֵs
    ulong       dwCountryCredit[COUNTRY_NUM];   //����ڸ�����Ӫ�е�����
    uchar       byCountryStatus;                //��Ӫ��λ

	ulong		dwSpouseID;				//��ż���ID 
	ulong		dwSpouseParam;			//����ż�Ĺ�ϵֵ
	uchar		byBusinessLevel;		//��ҵ�ȼ�
	uchar		byIsBusinessMan;		//�Ƿ�������
	ulong		dwBusinessExp;			//��ҵ����

	ulong		dwPkCount;				//���PKɱ����������
	ulong		dwPkValue;				//���PKֵ(����ֵ)
	ulong		dwPVPCount;				//���PVPɱ���������
	ulong		dwPVPValue;				//���PVPֵ(����ֵ)    

	ulong		dwHotKey[24];			//�Ƚ�(F1~F12,1~+) 
	uchar		byPKOnOff;				//PK����(b1:ȫ�屣��,b2:��ӱ���,b4:���ᱣ��,b8:��������,b16:��������,b32:��Ӫ����,b64.��������)
	ulong		dwOccuSP[OCC_Max];		//ÿ��ְҵ�ļ��ܵ���
	ulong		dwLastExitGameTime;		//��һ���˳���Ϸ��ʱ��
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
	short		wMaxStamina;			//ŭ������ 
	short		wMaxEnergy;				//�������� 
	short		wHpRecoverSpeed;		//�����ָ��ٶ�
    short		wMpRecoverSpeed;		//�����ָ��ٶ�    
	short		wHpRecoverRatio;		//HP�ָ�����
	short		wEnerRecoverSpeed;		//�����ָ��ٶ�
    short		wStamRecoverSpeed;		//ŭ���ָ��ٶ� 
    short		wFightHpRecoverSpeed;	//ս�������ָ��ٶ�	
    short		wFightMpRecoverSpeed;	//ս�������ָ��ٶ�
	short		wFightEnerRecoverSpeed;	//ս�������ָ��ٶ�
    short       wFightStamRecoverSpeed; //ս��ŭ���ָ��ٶ�

    short       wManly;                 //����������ʱ����ٵ�ֵ�����ڲ�ͬ��ֵ����Σ������ٶȲ�ͬ
    short       wPerverseTrend;         //ɷ������ɫɱ�˶ѵ���ֵ
    short       wComplaint;             //Թ������ɫ��������һ�ɱ���ӵ�ֵ
    short       wMurderousLook;         //ɱ������ɫ��ɱ�ж���Ӫ������ӵ�ֵ

    short       wStr;                   //����
    short       wDex;                   //��
    short       wCon;                   //����
    short       wIntellect;             //��־
    short       wSpiritualism;          //����
    short       wPoint;                 //Ǳ�ܵ�(ʣ�����������Ե�)

    short		wParry;					//�мܵȼ�(��Ӧ��ҵȼ��������мܼ���)
    float       fParryRatio;            //�мܼ���(�мܲ����⹦�˺�)
    short		wDodge;					//���ܵȼ�(��Ӧ��ҵȼ����������ܼ���)
    float       fDodgeRatio;            //���ܼ���
    ulong       lArmorDef;              //����ֵ(��Ӧ��ҵȼ�������Ϊ�⹦�˺�������)
    float       fDefSuck;               //�˺�������
    ulong       lEnergyDef;             //�ڹ�����
    float       fEnergySuck;            //�ڹ�������
    short       wOutEnergyHit;          //�⹦���еȼ�
    float       fOutEnergyHitRatio;     //�⹦���м���
    short       wEnergyHit;             //�ڹ����еȼ�
    float       fEnergyHitRatio;        //�ڹ����м���
    short       wOutEnergySpeed;        //�⹦���ٵȼ�
    float       fOutEnergySpeedRatio;   //�⹦���ٱ���
    short       wEnergySpeed;           //�ڹ����ٵȼ�
    float       fEnergySpeedRatio;      //�ڹ����ٱ���

    short       wOutEnergyHurt;         //�⹦�˺�
    short       wEnergyHurtInc;         //�⹦�˺��ӳ�
    short       wEnergyHealInc;         //�⹦���Ƽӳ�
    short       wOutEnergyBlast;        //�⹦�����ȼ�
    short       wOutEnergyBlastRate;    //�⹦������

    short		wDef;					//�������
    short		wMdef;					//ħ������
    short       wMdefSuck;              //ħ������������
    short		wBloodSuck;				//����������
    short		wManaSuck;				//����������
    short       wStaminaSuck;           //ŭ��������

    short		wHit;					//���� 
    short       wAtkSpeed;              //�����ٶ�
    short		wMAtkSpeed;				//ʩ���ٶ�
    short		wMinAtk;				//��С������
    short		wMaxAtk;				//��󹥻���
    short       wMinMAtk;               //��Сħ��������
    short       wMaxMAtk;               //���ħ��������
    short		wPhysicImmunity;		//��������
    short		wMagicImmunity;			//ħ������
    short       wMoveSpeed;             //�ƶ��ٶ�
    short       wMoveAcceleration;      //�ƶ����ٶ�
    short       wMoveRadius;            //�ƶ��뾶
    short       wTurnSpeed;             //ת���ٶ�
    short		wElemDef[ET_Max];		//��ϵ��������
    short		wDeBuffDef[DET_Max];	//Debuff����
    short		wClassDam[C_Max];		//�Ը��������˺�
 
	short		wFightBlock;			//��	
	short		wCharm;					//���� 	
	short		wAbsorb;				//רע	
	short		wShieldDef;				//�����������	
    short		wCri;					//��������	
	short		wBuffPower;				//Buff����ǿ��	
	long		wMAtk;					//ħ��������
	long		wMCri;					//ħ��������	
	short		wWillDef;				//��־����
	short		wConstiDef;				//���ʿ���
	short		wCriDef;				//����������
	short		wMCriDef;				//ħ����������
	short		wCriDamage;				//�������˺�
	short		wMCriDamage;			//ħ�������˺�
	short		wImmunity;				//����
	short		wPierce;				//��͸
	short		wLuck;					//����	
	short		wElemDam[ET_Max];		//�Ը���Ԫ���˺�
	short		wElemAtk[ET_Max];		//����Ԫ�صĹ���ֵ
	long		lMaxPKCP;				///< ���PK CPֵ
    short       HurtFactor;             //! �˺�ϵ��

}tagPFPro;

//��ʾ�뿤���Ѻù�ϵ��
struct tagFriendshipCounty
{
	ulong	dwCountyID;				//��ID
	ulong	dwFrienDeg;				//�Ѻö�
};
//���渽������
typedef struct tagPlayerAppendProperty
{
	bool		bIsCharged;					// �Ƿ��ֵ
	uchar		bShowFashion;				// ͷ��,����,���׵���ʾ״̬
	char		strAccount[ACCOUNT_SIZE];	// �ʺ�
	char		strTitle[TITLE_SIZE];		// �ƺ�	

}tagPAPro;

//������������ܶ�
struct tagBrotherInfo
{
    CGUID   guidBrother;        //�ֵ�guid
    long    lFriendVal;         //���ܶ�
};

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
    SAVE_DETAIL_ATTRPROFESSION		= 0x00400000,	// ��ְҵ
    SAVE_DETAIL_ATTRMERQUESTLIST	= 0x00800000,	// ٸ�������б�
    SAVE_DETAIL_PETS				= 0x01000000,	// �����б�
    SAVE_DETAIL_EQUIPMENTGOD        = 0x02000000, 
    SAVE_DETAIL_EQUIPMENTLAND       = 0x04000000, 
    SAVE_DETAIL_EQUIPMENTHUMAN      = 0x08000000, 

    //SAVE_DETAIL_ATTRMAILLIST		= 0x00800000,	// �ʼ��б�
    SAVE_DETAIL_ALL					= 0xFFFFFFFF,	// �����б�
};													

//����ֵ����ֵ
const static ulong dwExpMutipleValue = 2000000000;

#endif												
// ״̬�б�
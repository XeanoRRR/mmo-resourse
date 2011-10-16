/**
* @filename:GoodsAttrTypeDef.h
* @date: 2010/2/3
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 

* @purpose: ��GoodsViewer.sln��Copy�����ģ���������Goods�����ݽṹ
*/
#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _GOODSEDITOR_ATTRTYPEDEF_H_
#define _GOODSEDITOR_ATTRTYPEDEF_H_

#include <vector>
#include <string>
#include <list>

namespace UIData{
	//��������
	struct  tagAddAttr{
		WORD			wType;					// ��������
		bool			bEnable;				// �Ƿ���Ч(Ϊ�����������Ԥ��)
		bool			bHide;					// �Ƿ�����������
		long			lValue1;				// ��һ��ֵ
		long			lValue2;				// �ڶ���ֵ
	};

	//��װ����
	struct tagSuitAttr{
		WORD			wType;					// ��������
		long			lValue1;				// ��һ��ֵ
		long			lValue2;				// �ڶ���ֵ
		WORD			wSuitNum;				// ��װ��Ŀ
	};

	//��Ʒ����
	struct tagGoodAttr {
		//��������
		DWORD			dwIndex;				//	���
		std::string		strOrigName;			//	ԭʼ��   < 32
		std::string		strName;				//	�������� < 32 
		bool				bSave;				//	�Ƿ���Ҫ�����ݿ�
		DWORD		dwValue;					//	��Ҽ۸�
		DWORD		dwSilverValue;				//  ���Ҽ۸�
		DWORD		dwType;						//	��Ʒ����

		// �ͻ���ר������
		DWORD		dwIconId;					//	����ͼ��ID
		DWORD		dwGroundId;					//	��غ�ͼ��ID
		DWORD		dwEquipID;					//	��װͼ��ID
		DWORD		dwSound;					//	ʰȡ����
		DWORD		dwSoundID1;					//	�Ӷ�/���˺�����
		DWORD		dwSoundID2;					//	���⹥��/��������
		bool		bSoundSwitch;				//	����ʱ�Ƿ����
		DWORD		dwWeaponActType;			//	������Ӧ�Ķ�������
		std::string		strContent;				//	�������� < 64

		std::vector<tagAddAttr> vecAddAttr;		//  ��������List
		std::vector<tagSuitAttr> vecSuitAttr;	//  ��װ����List
	};

	typedef std::vector<tagGoodAttr>::iterator iterGoodAttr;
	typedef std::vector<tagAddAttr>::iterator iterAddAttr;
	typedef std::vector<tagSuitAttr>::iterator iterSuitAttr;

	struct tagConvert
	{
		//std::string strSign;				// ��ʶ�� #
		std::string strOrgName;				// ԭʼ��
		std::string strShowName;			// ��ʾ��
		std::string lGraphicID;				// ͼ��ID
		std::string lGroundID;				// ���ID
		std::string lEquipID;				// ��װID
		std::string lPrice;					// ���
		std::string lSilverPrice;			// ���Ҽ۸�
		std::string strContent;				// �������� < 64
		std::string lLevelRest;				// �ȼ�����
		std::string lRunScript;				// ִ�нű�
		std::string lColor;					// ��ʾ��ɫ
		std::string lSound;					// ʰȡ����
	};

	struct tagConvertE 
	{
		std::string	OrgName;				// ԭʼ��
		std::string	WeaponStrong;			// ����ǿ��
		std::string	WeaponStringAdd;		// ����ǿ�ȳɳ�
		std::string	MinAtk;					// ��С������
		std::string	MaxAtk;					// ��󹥻���
		std::string	MoGong;					// ħ������
		std::string	WuGongSockPro;			// ��������һ����
		std::string	WuBaoJiDamage;			// �������˺�
		std::string	WuBaoJiFastness;		// ����������
		std::string	MoGongSockPro;			// ħ������һ����
		std::string	MoBaoJiDamage;			// ħ�������˺�
		std::string	MoBaoJiFastness;		// ħ����������
		std::string	WuRecovery;				// �������
		std::string	MoRecovery;				// ħ������
		std::string	AsunderValue1;			// �ֽ��ֵ1
		std::string	GoodsChar;				// ��ƷƷ��
		std::string	ForceAdd;				// ����׷��
		std::string	MinJieAdd;				// ����׷��
		std::string	TiZhiAdd;				// ����׷��
		std::string	MindAdd;				// ����׷��
		std::string	ZhiHuiAdd;				// �ǻ�׷��
		std::string	EnergyAdd;				// ����׷��
		std::string	MingZhongRevise;		// ������������
		std::string	DuckRevise;				// ������������
		std::string	GeDangRevise;			// ����������
		std::string	ZhaoJiaRevise;			// �м���������
		std::string	MaxChongNeng;			// ��������	
		std::string	WuImmunity;				// ��������ֵ
		std::string	MoImmunity;				// ħ������ֵ;
		std::string	GoodsUpPrice;			// ��Ʒ�����۸�
		std::string	BasicAperValue1;		// ��������ֵ1
		std::string	BasicAperValue2;		// ��������ֵ2
		std::string	AsunderValue2;			// �ֽ��ֵ2
		std::string	LianDuValue2;			// ����ֵ2
	};

	enum eAddAtrrType
	{
		ADDATTR_TYPE,						// ��������
		ADDATTR_ENABLE,						// �Ƿ���Ч(Ϊ�����������Ԥ��)
		ADDATTR_HIDE,						// �Ƿ�����������
		ADDATTR_VALUE1,						// ��һ��ֵ
		ADDATTR_VALUE2,						// �ڶ���ֵ

		ADDATTR_LISTITEMNUM					// ��������List�б�����(5)
	};

	enum eSuitAtrrType
	{
		SUITATTR_TYPE,						// ��������
		SUITATTR_VALUE1,					// ��һ��ֵ
		SUITATTR_VALUE2,					// �ڶ���ֵ
		SUITATTR_NUM,						// ��װ��Ŀ

		SUITATTR_LISTITEMNUM				// ��װ����List�б����� (4)
	};

	enum eAllGoodsAttrType
	{
		ALLGOODSATTR_INDEX,					// ���
		ALLGOODSATTR_ORIGNAME,				// ԭʼ��   < 32
		ALLGOODSATTR_NAME,					// �������� < 32
		ALLGOODSATTR_SAVE,					// �Ƿ���Ҫ�����ݿ�
		ALLGOODSATTR_ICONID,				// ����ͼ��ID
		ALLGOODSATTR_GROUNDID,				// ��غ�ͼ��ID
		ALLGOODSATTR_EQUIPID,				// ��װͼ��ID
		ALLGOODSATTR_VALUE,					// ��Ҽ۸�
		ALLGOODSATTR_SILVERVALUE,			// ���Ҽ۸�
		ALLGOODSATTR_TYPE,					// ��Ʒ����
		ALLGOODSATTR_SOUND,					// ʰȡ����
		ALLGOODSATTR_CONTENT,				// �������� < 64
		ALLGOODSATTR_SOUNDID1,				// �Ӷ�/���˺�����
		ALLGOODSATTR_SOUNDID2,				// ���⹥��/��������
		ALLGOODSATTR_SOUNDSWITCH,			// ����ʱ�Ƿ����

		ALLGOODSATTR_LISTITEMNUM			// ������Ʒ����List�б����� (15)
	};

	enum eHotKeyType
	{
		HOTKEY_CTRL_C = 1001,				// Ctrl+C
		HOTKEY_CTRL_LC ,					// Ctrl+c
		HOTKEY_CTRL_V ,						// Ctrl+V
		HOTKEY_CTRL_LV,						// Ctrl+v
		HOTKEY_CTRL_A,						// Ctrl+A
		HOTKEY_CTRL_LA ,					// Ctrl+a
		HOTKEY_CTRL_M,						// Ctrl+M
		HOTKEY_CTRL_LM,						// Ctrl+m
		HOTKEY_CTRL_D,						// Ctrl+D
		HOTKEY_CTRL_LD,						// Ctrl+d
		HOTKEY_CTRL_X,						// Ctrl+X
		HOTKEY_CTRL_LX,						// Ctrl+x
	};

	enum eOperatorHistory
	{
		OPERATOR_TYPE,						// �������ͣ����/ɾ��/�޸�/ճ����
		OPERATOR_KIND,						// ����������𣨸�������/��װ���ԣ� 
		OPERATOR_ATTRNAME,					// ������������
		OPERATOR_REPEAT,					// �����Ƿ����ظ�

		OPERATOR_NUM
	};

	struct tagOperatorRec
	{
		WORD		wOperType;				// ��������
		WORD		wOperAttrKind;			// �����������
		WORD		wOperAttrType;			// ������������
		bool		bIsRepeat;				// �����Ƿ����ظ�
	};

	typedef std::vector<tagOperatorRec>::iterator iterOperRec;

	enum eOperatorType
	{
		OPER_TYPE_ADD,						// ���
		OPER_TYPE_DEL,						// ɾ��
		OPER_TYPE_MODIFY,					// �޸�
		OPER_TYPE_PASTA,					// ճ��

		OPER_TYPE_NUM
	};

	enum eOperatorKind
	{
		OPER_KIND_ADD,						// ���
		OPER_KIND_SUIT,						// ɾ��

		OPER_KIND_NUM
	};

#endif//_GOODSEDITOR_ATTRTYPEDEF_H_
}
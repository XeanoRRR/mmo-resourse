#ifndef _SHAPE_PROPERTIES_H
#define _SHAPE_PROPERTIES_H

//##Shapeʵ������(��Ӧԭʼ���Ե�����ֵ)
struct tagProperties
{
	LONG	lMaxHp;					//##��������
	LONG	lMaxMp;					//##��������
	LONG	lMaxYp;					//##��������
	LONG	lMaxRp;					//##ŭֵ����
	LONG	lStr;					//##����
	LONG	lDex;					//##����
	LONG	lCon;					//##����
	LONG	lInt;					//##����
	LONG	lMinAtk;				//##��С������
	LONG	lMaxAtk;				//##��󹥻���
	LONG	lHit;					//##��������
	LONG	lBurden;				//##��������
	LONG	lCCH;					//##����һ������
	LONG	lDef;					//##������
	LONG	lDodge;					//##��������
	LONG	lAtcSpeed;				//##�����ٶ�
	LONG	lElementResistant;		//##Ԫ���˺�����
	LONG	lHpRecoverSpeed;		//##�����ָ��ٶ�
	LONG	lMpRecoverSpeed;		//##�����ָ��ٶ�

	LONG	lSoulResistant;			//##�����˺�����
	LONG	lAddElementAtk;			//##���ӷ����������������������
	LONG	lAddSoulAtk;			//##���������������������������
	LONG	lElementModify;			//##������������(%)
};

#endif
//=============================================================================
/**
 *  file: EffectObjects.h
 *
 *  Brief:���ö����࣬����ĳ������ѡ��ĳ����Χ�����ö��󼯺�
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-8-31
 */
//=============================================================================

#ifndef __EFFECT_OBJECTS_H__
#define __EFFECT_OBJECTS_H__

#pragma once

class CCondiGroup;
class CMoveShape;
class CScript;
struct stModuParam;

class CEffectObjects : public BaseMemObj
{
public:
	CEffectObjects(stModuParam *pParam,CCondiGroup* pCondiGroup);
	virtual ~CEffectObjects(void);

	enum ePosition
	{
		ePos_Mouse=0,
		ePos_Self,
		ePos_Dest,
		ePos_DestCoord,//Ŀ������
		ePos_Coord,    //����
		ePos_Firer,	   //ʩ����
		ePos_Attacker, //������
		ePos_Host,	   //��������
		ePos_Target,   //ѡ��Ŀ��
		ePos_AttackTarget,//����Ŀ��
	};
protected:
	//������,�ж����ö��������
	CCondiGroup* m_pCondiGroup;
	//���ܲ���
	stModuParam *m_pParam;


public:
	//
	ePosition GetPosition(string strname)
	{
		if (strname=="ePos_Mouse")
		{
			return ePos_Mouse;
		} 
		else if(strname=="ePos_Self")
		{
			return ePos_Self;
		}
		else if (strname=="ePos_Dest")
		{
			return ePos_Dest;
		}
		else if (strname=="ePos_DestCoord")
		{
			return ePos_DestCoord;
		}
		else if (strname=="ePos_Coord")
		{
			return ePos_Coord;
		}
		else if (strname=="ePos_Firer")
		{
			return ePos_Firer;
		}
		else if (strname=="ePos_Attacker")
		{
			return ePos_Attacker;
		}		
		else if (strname=="ePos_Host")
		{
			return ePos_Host;
		}	
		else if (strname=="ePos_Target")
		{
			return ePos_Target;
		}
		else if (strname=="ePos_AttackTarget")
		{
			return ePos_AttackTarget;
		}
		
		return ePos_Mouse;
	}
	//�õ����õ����ö���
	virtual unsigned int GetEffectObjs(list<CMoveShape*>& listObjs) = 0;
	void SetParam(stModuParam *pParam)	{m_pParam = pParam;}
};

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

class CEffDestObjs :
	public CEffectObjects
{
public:
	CEffDestObjs(stModuParam *pParam,CCondiGroup* pCondiGroup);
	virtual ~CEffDestObjs(void);

public:
	//�õ����õ����ö���
	unsigned int GetEffectObjs(list<CMoveShape*>& listObjs);
};

class CEffTimerObjs :
	public CEffectObjects
{
public:
	CEffTimerObjs(stModuParam *pParam,CCondiGroup* pCondiGroup);
	virtual ~CEffTimerObjs(void);

public:
	//�õ����õ����ö���
	unsigned int GetEffectObjs(list<CMoveShape*>& listObjs);
};

#endif

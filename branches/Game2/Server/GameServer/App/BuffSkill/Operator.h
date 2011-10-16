//=============================================================================
/**
 *  file: Operator.h
 *
 *  Brief:״̬�������еĻ���������װ���������Ա仯��
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-8-31
 */
//=============================================================================

#ifndef __OPERATOR_H__
#define __OPERATOR_H__
#pragma once

#include "BuffSkillDef.h"

class CMoveShape;
class CScript;
class CExpressions;

//�����������
typedef void OPFUN(CMoveShape* pShape,string& strName,int nValue);

class COperator
{
public:
	COperator(stModuParam *pParam);
	virtual ~COperator(void);
protected:
	//���ܲ���
	stModuParam	*m_pParam;
	//����Ŀ��
	eObj m_Obj;

	CMoveShape* GetOpShape(CMoveShape* pDest);
public:
	//ִ��
    virtual void Execute(CMoveShape* pShape) = 0;
			void SetParam(stModuParam *pParam) {m_pParam = pParam;}		
};


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class COpProperty:public COperator
{
public:
	COpProperty(stModuParam *pParam);
	COpProperty(void* pParam);
	virtual ~COpProperty(void);
private:
	//��������
	OPFUN *m_pOpFun;
	//��������
	string m_strName;
	int m_nValue;
	//�ű�������
	string m_strVariaName;
    //ֱ�Ӷ�ȡ����
    string m_ParamName;
public:
	//��ʼ��
	void InitBaseData(eObj Obj,OPFUN *OpFun,string strName,int value, string strVariaName,string paramname)
	{
		m_Obj = Obj;
		m_pOpFun = OpFun;
		m_strName = strName;
		m_nValue = value;
		m_strVariaName = strVariaName;
        m_ParamName = paramname;
	};
	//ִ��
    void Execute(CMoveShape* pShape);
};

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class COpBuff:public COperator
{
public:
	COpBuff(stModuParam *pParam);
	COpBuff(void* pParam);
	virtual ~COpBuff(void);
private:
	//����
	eModuleOper m_Op;
    //���buff����  
    eSkillAttackType m_SkillType;
	//buff id
	int m_nBuffID;
	//buff lvl
	int m_nBuffLvl;
	//״̬����
	int m_nBuffCount;
    //buf id  string
    string m_strBuffId;
    //buf lvl string
    string m_strBuffLvl;
    //buf num string
    string m_strBuffNum;
public:
	//
	 void InitBaseData(eObj Obj,eModuleOper op,eSkillAttackType Type,int id, int lv,int nCount,string bufid,string buflvl,string bufnum)
	{
		m_Obj = Obj;
		m_Op = op;
        m_SkillType = Type;
		m_nBuffID = id;
		m_nBuffLvl = lv;
		m_nBuffCount = nCount;
        m_strBuffId  = bufid;
        m_strBuffLvl = buflvl;
        m_strBuffNum = bufnum;
	}

	//ִ��
    void Execute(CMoveShape* pShape);
};


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class COpGood:public COperator
{
public:
	COpGood(stModuParam *pParam);
	COpGood(void* pParam);
	virtual ~COpGood(void);
private:
	//����
	eModuleOper m_Op;
	//��Ʒԭʼ��
	string m_strOrigName;
	//ֵ
	int m_nValue;;
	//����
	int m_nPlace;
	//��λ
	int m_nPos;
	//��Ʒ���
	int m_nGoodType;
	//�������
	int m_nWeaponType;
public:
	void InitBaseData(eObj Obj,eModuleOper op,string name,int value,DWORD dwPlace, DWORD dwPos,DWORD dwGoodType, DWORD dwWeaponType)
	{
		m_Obj = Obj;
		m_Op = op;
		m_strOrigName = name;
		m_nValue = value;
		m_nPlace = dwPlace;
		m_nPos = dwPos;
		m_nGoodType = dwGoodType;
		m_nWeaponType = dwWeaponType;
	}

	//ִ��
    void Execute(CMoveShape* pShape);
};


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//λ�øı�
class COpPosition:public COperator
{
public:
	COpPosition(stModuParam *pParam);
	COpPosition(void* pParam);
	virtual ~COpPosition(void);
	enum eDirec
	{
		//ǰ��
		DIREC_Front,
		//���
		DIRECT_Back,
	};
	enum ePosStyle
	{
		//��ͨλ��
		POS_SIMPLE,
		//����λ��
		POS_SLIP,
	};
private:
	//�ο�Ŀ��
	eObj m_refObj;
	//����
	eDirec m_eDriect;
	//λ�Ʒ�ʽ
	ePosStyle m_ePosStyle;
	//�ٶ�
	float m_fSpeed;
public:
	void InitBaseData(eObj Obj,eObj obj,eObj rObj,string dir,string style,string speed)
	{
		m_Obj = Obj;
		m_refObj = rObj;
		m_eDriect = GetDir(dir);
		m_ePosStyle = GetPosStyle(style);
		m_fSpeed = (float)atof(speed.c_str());
	}
	
	eDirec GetDir(string strDir)
	{
		if (strDir=="DIREC_Front")
		{
			return DIREC_Front;
		}
		else if (strDir=="DIRECT_Back")
		{
			return DIRECT_Back;
		}
		return DIREC_Front;		
	}
	ePosStyle GetPosStyle(string strPosStyle)
	{
		if (strPosStyle=="POS_SIMPLE")
		{
			return POS_SIMPLE;
		} 
		else if (strPosStyle=="POS_SLIP")
		{
			return POS_SLIP;
		}
		return POS_SIMPLE;
	}

	//ִ��
    void Execute(CMoveShape* pShape);
	void GetRandomPos(CMoveShape *pShape,LONG &x, LONG &y);
};

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//����ı�
class COpDirec:public COperator
{
public:
	COpDirec(stModuParam *pParam);
	COpDirec(void *pParam);
	virtual ~COpDirec(void);

	//ָ���ο�����
	enum eDirec
	{
		eDir_Mouse=0,
		eDir_Self,
		eDir_Dest,
	};
private:
	//�ο�����
	eDirec m_eDir;
public:
	void InitBaseData(eObj Obj,eDirec eDir)
	{
		m_Obj = Obj;
		m_eDir = eDir;
	}	
	//ִ��
    void Execute(CMoveShape* pShape);
	void SendChangeDir(CMoveShape* pShape);
};

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class COpTimeChange:public COperator
{
public:
	COpTimeChange(stModuParam *pParam);
	COpTimeChange(void* pParam);
	virtual ~COpTimeChange(void);

	enum Op_Cooldown
	{
		OP_TS_NO=0,
		OP_TS_Act=1, //����ʱ��
		OP_TS_Self,  //���ܱ���
		OP_TS_Inton, //����ʱ��
        OP_TS_CDControl,//CD����ʱ��
	};

private:
	Op_Cooldown m_eOpCd;
	//ֵ
	int m_nValue;
	//�ű�����
	string m_strVariaName;
    //ֱ�Ӵӳ����ȡ����
    string m_ParamName;
public:
	//��ʼ��
	void InitBaseData(eObj Obj,string& type,int value,string strname,string paramname)
	{
		m_Obj = Obj;
		m_eOpCd = GetType(type);
		m_nValue = value;
		m_strVariaName = strname;
        m_ParamName = paramname;
	}

	Op_Cooldown GetType(string strType)
	{
		if (strType=="OP_TS_Act")
		{
			return OP_TS_Act;
		} 
		else if(strType=="OP_TS_Self")
		{
			return OP_TS_Self;
		}
		else if (strType=="OP_TS_Inton")
		{
			return OP_TS_Inton;
		}
        else if (strType=="OP_TS_CDControl")
        {
            return OP_TS_CDControl;
        }
		return OP_TS_NO;
	}
	//�õ��������
	void Execute(CMoveShape* pShape);
};

//�����ö����б�Ĳ���
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class COpDestObjs:public COperator
{
public:
	COpDestObjs(stModuParam *pParam);
	COpDestObjs(void *pParam);
	virtual ~COpDestObjs(void);

	enum Op_DestObj
	{
		OP_Dest_Obj_Add,//��Ӹö��������б�
		OP_Dest_Obj_Del,//ɾ��һ��Ŀ��
		OP_Dest_Obj_Clear,//������ö����б�
	};
private:
	Op_DestObj m_Op;
public:
	//��ʼ��
	void InitBaseData(eObj Obj,string strName)
	{
		m_Obj = Obj;
		m_Op = GetDestObj(strName);
	};
	//�õ��������
	void Execute(CMoveShape* pShape);

	Op_DestObj GetDestObj(string strName)
	{
		if (strName=="OP_Dest_Obj_Add")
		{
			return OP_Dest_Obj_Add;
		} 
		else if(strName=="OP_Dest_Obj_Del")
		{
			return OP_Dest_Obj_Del;
		}
		else if (strName=="OP_Dest_Obj_Clear")
		{
			return OP_Dest_Obj_Clear;
		}
		else
		{
			return OP_Dest_Obj_Add;
		}
		
	}
};
//����
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class COpSkill:public COperator
{
public:
	COpSkill(stModuParam *pParam);
	COpSkill(void *pParam);
	virtual ~COpSkill(void);
private:
	ModuleType m_eType;
	long	   m_lID;
	long	   m_lLv;
	string	   m_strOperator;
public:
	void InitBaseData(eObj Obj,ModuleType eType,long lID,long lLv,string strOperator)
	{		
		m_Obj = Obj;
		m_eType = eType;
		m_lID = lID;
		m_lLv = lLv;
		m_strOperator = strOperator;
	}
	//���ܲ���
	void Execute(CMoveShape* pShape);
};


//�ű�
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class COpScript:public COperator
{
public:
	COpScript(stModuParam *pParam);
	COpScript(void *pParam);
	virtual ~COpScript(void);
private:
	//����ű�����
	typedef vector<CExpressions*>::iterator itExpress;
	vector<CExpressions*> m_Expressions;
public:
	void InitBaseData(eObj Obj,vector<CExpressions*>& vExpressions)
	{
		m_Obj = Obj;
		m_Expressions = vExpressions;
	}
	//�õ��������
	void Execute(CMoveShape* pShape);
};

/// �ṩһ��ʹ�ü��ܵ�ʱ�򸽼���ѧ�츳����״̬
class COpGenius:public COperator
{
public:
    COpGenius(stModuParam *pParam);
    COpGenius(void *pParam);
    virtual ~COpGenius();

    void  InitBaseData(string strGenius)
    {
        m_strGenius = strGenius;
    }

    void  Execute(CMoveShape* pShape);

private:
    string  m_strGenius;
};


void AddOper(CMoveShape* pShape,string& strName,int nValue);
void SubOper(CMoveShape* pShape,string& strName,int nValue);
void SetOper(CMoveShape* pShape,string& strName,int nValue);
void BoolOper(CMoveShape* pShape,string& strName,int nValue);
void ResetOper(CMoveShape* pShape,string& strName,int nValue);
void UpdateOper(CMoveShape* pShape,string& strName,int nValue=1);

#endif

//=============================================================================
/**
 *  file: Condition.h
 *
 *  Brief:�����ж��࣬��װ�˼��ܲ�������Ҫ��������������롢����ֵ������
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-8-31
 */
//=============================================================================

#ifndef __CONDITION_H__
#define __CONDITION_H__

#pragma once

#include "BuffSkillDef.h"
#include "../../../../Public/Common/CStrConverter.h"

class CExpressions;
class CMoveShape;
class CScript;
struct stModuParam;

//����ñȽϺ���
typedef bool CMPFUN(long,long);

class CCondition
{
public:
	CCondition(stModuParam *pParam);
	virtual ~CCondition(void);
protected:
	//���ܲ���
	stModuParam *m_pParam;
public:
	virtual bool Result(CMoveShape* pDest) = 0;
	void		 SetParam(stModuParam *pParam) {m_pParam=pParam;}
};


class CConProperty:public CCondition
{
public:
	CConProperty(stModuParam *pParam);
	CConProperty(void *pParam);
	virtual ~CConProperty(void);
	struct tagCheckes
	{
		//������
		eObj Obj;
		//��������
		string strName;
		//�ȽϺ���
		CMPFUN *pCmpFun;
		//ֵ
		int nValue;
        //����������
        string strParamam;

		void Init(eObj object,string &strname,CMPFUN *pCmp,int value,string Paramam)
		{
			Obj = object;
			strName = strname;
			pCmpFun = pCmp;
			nValue = value;
            strParamam = Paramam;
		}
	};
private:
	typedef list<tagCheckes>::iterator itCheck;
	list<tagCheckes> m_Checks;
public:
	void InitBaseData(const list<tagCheckes> &lChecks)
	{
		m_Checks = lChecks;
	}
	bool Result(CMoveShape* pDest);
};



class CConGoods:public CCondition
{
public:
	CConGoods(stModuParam *pParam);
	CConGoods(void *pParam);
	virtual ~CConGoods(void);

	struct tagAddProperty
	{
		DWORD dwAddProType;	//������������
		DWORD dwValue1;		//ֵ1
		DWORD dwValue2;		//ֵ2	
	};

	struct tagCheck
	{
		//������
		eObj Obj;
		//��Ʒ���ڵĵط�
		int nPlace;
		//��Ʒ���ڵ�λ��
		int nPos;
		//��Ʒԭʼ��
		string strOrigName;
		//��Ʒ���
		int nType;
		//�ȽϺ���
		CMPFUN *pCmpFun;
		//ֵ
		int nValue;
		//��������
		list<tagAddProperty*> lAddProperty;

		//�ű�������
		string strVariaName;
		void Init(eObj object,int place,int pos,int type, const  string &strName,CMPFUN *op,int value,const string &strVname, list<tagAddProperty*> &lProperty)
		{
			Obj = object;
			nPlace = place;
			nPos = pos;
			strOrigName = strName;
			nType = type;
			pCmpFun = op;
			nValue = value;
			strVariaName = strVname;
			lAddProperty = lProperty;
		}
	};

private:
	typedef list<tagCheck>::iterator itCheck;
	list<tagCheck> m_Checkes;

public:
	void InitBaseData(const list<tagCheck> &ltgCheck)
	{
		m_Checkes = ltgCheck;
	}
	//�õ��������
	bool Result(CMoveShape* pDest);
	//�Ƚ���Ʒ��������
	bool GetGoodAddProperty(CGoods* pEquip,list<tagAddProperty*> &lAddProperty);
};



class CConBuff:public CCondition
{
public:
	CConBuff(stModuParam *pParam);
	CConBuff(void *pParam);
	virtual ~CConBuff(void);

	struct tagCheck
	{
		//������
		eObj Obj;
        //�����󸽴�buf����id
        string BufObj;
		//buff id
		DWORD nBuffID;
		//buff level
		int nLevel;
		//����
		int nNum;
        //�ȼ��ַ���
        string LevelName;
        //�ȽϺ���
        CMPFUN*  CmpFun;
		void Init(eObj object,string Bobj,DWORD id,int lv,int num,string name,CMPFUN* Cmp)
		{
			Obj = object;
            BufObj=Bobj;
			nBuffID = id;
			nLevel = lv;
			nNum = num;
            LevelName = name;
            CmpFun=Cmp;
		}
	};
private:
	typedef list<tagCheck>::iterator itCheck;
	list<tagCheck> m_Checkes;
public:
	void InitBaseData(const list<tagCheck> &ltgCheck)
	{
		m_Checkes = ltgCheck;
	}
	//�õ��������
	bool Result(CMoveShape* pDest);
};



class CConDistance:public CCondition
{
public:
	CConDistance(stModuParam *pParam);
	CConDistance(void *pParam);
	virtual ~CConDistance(void);

private:
	//�ȽϺ���
	CMPFUN *m_CmpFun;
	//ֵ
	/*int m_nValue;*/
    long m_value;
	//���ն���
	eObj m_refObj;
	//�ű�������
	string m_strValueName;
public:
	void InitBaseData(CMPFUN *op,float value,const string &strVaName,const string &strRefObj)
	{
		m_CmpFun = op;
		m_value = (long)value;
		m_strValueName = strVaName;
		m_refObj = GetRefObj(strRefObj);
	}

	//�õ��������
	bool Result(CMoveShape* pDest);
	eObj GetRefObj(string strRefObj)
	{
		if (strRefObj=="Obj_Self")
		{
			return Obj_Self;
		} 
		else if(strRefObj=="Obj_Dest")
		{
			return Obj_Dest;
		}
		else if (strRefObj=="Obj_Mouse")
		{
			return Obj_Mouse;
		}
		else if (strRefObj=="Obj_Firer")
		{
			return Obj_Firer;
		}
		else 
		{
			return Obj_Dest;
		}
	}
};

class CConScript:public CCondition
{
public:
	CConScript(stModuParam *pParam);
	CConScript(void *pParam);
	virtual ~CConScript(void);

private:
	/// ����LUA���ٱ���ű����ݣ������ļ���
	string m_scriptFile;
public:
	void InitBaseData(const string &file)
	{
		m_scriptFile = file;	
	}
	//�õ��������
	bool Result(CMoveShape* pDest);
};


class CConValidObj:public CCondition
{
public:
	CConValidObj(stModuParam *pParam);
	CConValidObj(void *pParam);
	virtual ~CConValidObj(void);
	//��Ч��������
	enum eValidObjType
	{
		SelfPlayer_Type,	 //�������			
		EnemyPlayer_Type,	 //��ʾ�й����
		NormalPlayer_Type,	 //��ʾ������ͨ���	
		RedPlayer_Type,		 //��ʾ�����������
		PurplePlayer_Type,	 //��ʾ�����������
		TeamPlayer_Type,	 //�����Ա
		TongPlayer_Type,	 //����Ա
		LeaguePlayer_Type,	 //ͬ�˳�Ա
		Monster_Type,		 //��ʾ��ͨ����
		Npc_Type,			 //��ʾNPC
		Guard_Type,			 //����
		SMonster_Type,		 //��ʾ�������		
	};
	enum eType
	{
		ePlayer_Type,		//�������
		eMonster_Type,		//��������
		eGate_Type,			//������
		ePet_Type,			//��������
	};

public:
	void InitBaseData(eType type,list<eValidObjType> &lValidObjType)
	{
		m_lValidObjType = lValidObjType;// = type;
		m_lType			= type;
	}
	
	//�õ��������
	bool Result(CMoveShape* pDest);
private:
	list<eValidObjType> m_lValidObjType;
	eType m_lType;
};

class CConTimer:public CCondition
{
public:
	CConTimer(stModuParam *pParam);
	CConTimer(void *pParam);
	virtual ~CConTimer(void);
private:
	long m_lFlag;
public:
	//
	void InitBaseData(long flag)
	{
		m_lFlag = flag;
	}
	//�õ��������
	bool Result(CMoveShape* pDest);
};

class CConEvent:public CCondition
{
public:
	CConEvent(stModuParam *pParam);
	CConEvent(void *pParam);
	virtual ~CConEvent(void);
private:
	long m_lFlag;
	//�¼�����
	long m_lEventType;
	long m_lVar1;
	long m_lVar2;
	long m_lVar3;
	long m_lVar4;
	string m_strVar3;

    list<long> m_skilllist;

public:
	void InitBaseData(long flag,long type,long lvar1,long lvar2,long lvar3,long lvar4,string skillid,string strName)
	{
		m_lFlag = flag;
		m_lEventType = type;
		m_lVar1 = lvar1;
		m_lVar2 = lvar2;
		m_lVar3 = lvar3;
		m_lVar4 = lvar4;
		m_strVar3 = strName;
        
        char  strKill[128];
        strcpy( strKill , skillid.c_str() );
        CStrConverter::parseList(strKill,m_skilllist);
	}
	//�õ��������
	bool Result(CMoveShape* pDest);
};


class CConCooldown:public CCondition
{
public:
	CConCooldown(stModuParam *pParam);
	virtual ~CConCooldown(void);
public:
	//�õ��������
	bool Result(CMoveShape* pDest);
};
//��Ŀ��������Ƿ���ͬ
class CConDir: public CCondition
{
public:
	CConDir(stModuParam *pParam);
	CConDir(void *pParam);
	virtual ~CConDir(void);
private:
	int m_nFaceType;
	enum eFace
	{
		//����
		eFace_Front=1,
		//����
		eFace_Back,		
	};
public:
	bool Result(CMoveShape* pDest);
	int  GetFace(string strType)
	{
		if (strType=="Face_Front")
		{
			return eFace_Front;
		} 
		else
		{
			return eFace_Back;
		}
	}
	void InitBaseData(string strType)
	{
		m_nFaceType = GetFace(strType);
	}
};
//��������
class CConRegion:public CCondition
{
public:
	CConRegion(stModuParam *pParam);
	CConRegion(void *pParam);
	virtual ~CConRegion(void);
private:
	string  m_strName;		//��������
	int		m_nRegionValue; //ֵ
public:
	//��ʼ��
	void InitBaseData(string strName, int nType)
	{
		m_strName = strName;
		m_nRegionValue = nType;
	}
	bool Result(CMoveShape* pDest);

};

//��Ŀ��֮����赲����
class CConBlock:public CCondition
{
public:
	CConBlock(stModuParam *pParam);
	CConBlock(void *pParam);
	virtual ~CConBlock(void);
	
	enum eBlockType
	{
		BT_NoAnyBlock=0x1,//û���κ��赲
		BT_NoFullBlock=0x2,//û��ȫ�赲(�����赲)
		BT_NoHalfBlock=0x3,//û�а��赲(�����赲)
		BT_NoShapeBlock=0x4,//�����赲
	};
	enum eType
	{
		e_Line=0,			//����
		e_Point,			//��
	};
	enum eDirec
	{
		//ǰ��
		DIREC_Front,
		//���
		DIRECT_Back,
		//����
		Direct_MousePos,
	};
private:
	//�赲��������,������eBlockType�������
	int  m_NoBlockType;
	//����BT_NoShapeBlockֵ��ʱ�������ж�shape���ͣ������0��ʾ��������
	int m_nShapeType;
	//�����赲��ʽ
	int m_nType;
	//����
	int m_nDir;

	bool NoAnyBlock(BYTE bBlock);
	bool NoFullBlock(BYTE bBlock);
	bool NoHalfBlock(BYTE bBlock);
	bool NoShapeBlock(BYTE bBlock,long lType);
public:
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
		else if (strDir=="Direct_MousePos")
		{
			return Direct_MousePos;
		}		
		return DIREC_Front;
	}

	eType GetType(string strType)
	{
		if (strType=="Point")
		{
			return e_Point;
		} 
		else if(strType=="Line")
		{
			return e_Line;
		}
		return e_Line;
	}
	//�õ��������
	bool Result(CMoveShape* pDest);
	//��ʼ��
	void InitBaseData(DWORD dwBlockType,DWORD dwShapeType,string strType,string strDir)
	{
		m_NoBlockType = dwBlockType;
		m_nShapeType = dwShapeType;
		m_nType		= GetType(strType);
		m_nDir		= GetDir(strDir);
	}
};

//�����¼��ж�
class CConOdds:public CCondition
{
private:
   long    m_nValue;	//����
   string   valuename;  //��������
   CMPFUN*  cmpfun;     //�ȽϺ���

public:
    CConOdds(stModuParam *pParam);

    CConOdds(void *pParam);

    virtual ~CConOdds(void);

    bool Result(CMoveShape* pDest);

    void InitBaseData(short va,string vname,CMPFUN* cmp)
    {
         m_nValue = va;
         valuename = vname;
         cmpfun=cmp;
    }
};

//��ɢ����
class CConDispel:public CCondition
{
private:
    long            m_nValue;         //��ɢ����    
    eSkillAttackType DispelType;      //��ɢ����
    string           strDispelNum;    //��ɢ�ַ���

public:
    CConDispel(stModuParam *pParam);

    CConDispel(void *pParam);

    virtual ~CConDispel();

    bool  Result(CMoveShape* pDest);

    void  InitBaseData(short va,eSkillAttackType Type,string DispelNum)
    {
        m_nValue = va;
        DispelType=Type;
        strDispelNum=DispelNum;
    }

};

//�ȽϺ���
bool Less(long val1,long val2);
bool Equal(long val1,long val2);
bool LessEqual(long val1,long val2);
bool Greater(long val1,long val2);
bool GreaterEqual(long val1,long val2);
bool NotEqual(long val1,long val2);

#endif


///////////////////////////////////////////////////////////////////////////////
///  NewSkillFactory.h
///
///  PATH: E:\alienbrainWork\Game_Program_Develop\guangmang\SERVER\GameServer\AppServer\BuffSkill
///
///  CREATED: 08/07/2007 12:50:01 PM by ���Ƚ�
///
///  PURPOSE:   ���ܹ�����
///
//////////////////////////////////////////////////////////////////////
#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_NEW_SKILL_FACTORY_INCLUDED
#define _INC_NEW_SKILL_FACTORY_INCLUDED

#include <map>
#include <string>

#include "Operator.h"
#include "Condition.h"
#include "CondiGroup.h"
#include "OpSendMsg.h"
#include "BaseModule.h"
#include "Do.h"
#include "NewSkillBaseProperties.h"
#include "../../public/GameEvent.h"
#include "../../../setup/NewSkillList.h"


enum EMAXBASEMODLE
{
	EMAXSKILLSTATE = 10000,
};
class CStep;

class CEffectObjects;

using namespace std;
struct  tagModuleKey
{
	DWORD dwID;
	DWORD dwLvl;
};
struct tagBKey
{
	long lFlag;
	long lID;
};

struct  tagBKeycmp
{
	bool operator()(const tagBKey& left,const tagBKey& right) const
	{
		if(left.lID<right.lID)
		{
			return true;
		}		
		return false;
	}
};
struct tagKeycmp
{
	bool operator()(const tagModuleKey& left,const tagModuleKey& right) const
	{
		if(left.dwID<right.dwID)
		{
			return true;
		}
		else if (left.dwID==right.dwID)
		{
			if (left.dwLvl < right.dwLvl)
			{
				return true;
			} 
			else
			{
				return false;
			}
		}
		return false;
	}
};


class CNewSkillFactory 
{
private:

	typedef list<CBaseModule*>									listModules;
	typedef list<CBaseModule*>::iterator						itModule;	
	static  listModules											g_BaseModules[EMAXSKILLSTATE];			

	typedef list<stModuParam*>									listModuParam;
	typedef list<stModuParam*>::iterator						itModuParam;
	static	listModuParam										g_lModuParam;
	static stModuParam											*m_pParam;								//����
	static  map<DWORD, CNewSkill*>								g_mNewSkillProperties;					//�����б�
	static DWORD												g_dwVersion;							//�����ļ��İ汾
	static map<DWORD,list<long>>								g_mapOccuInitSkill;
	static DWORD												g_dwCurrentVersion;						//��ǰ�汾
	static DWORD												g_dwCount;								//��
	static map<tagBKey,  set<CBaseModule*>,tagBKeycmp>			m_UsingBaseModule;

	static map<tagModuleKey, long, tagKeycmp>					g_mapModuleCounter;		
	typedef map<tagModuleKey, long, tagKeycmp>::iterator		itModulerCounter;


	//  [4/26/2009 chenxianj]
	typedef map<tagModuleKey,long,tagKeycmp>::iterator			itModuleTimer;	
	static map<tagModuleKey,long,tagKeycmp>						g_mapModuleTimer;

	typedef map<string,CVariableList*>::iterator				itVariableList; 
	static map<string,CVariableList*>							g_mapVariableList;	


public:
	//����ID��ѯ����
	static  CNewSkill*								QueryNewSkillProperty(DWORD dwID);									
	//�Լ��ܽ���
	static  void									DecodeNewSkillMessage(BYTE* pByte, long& pos);						
	//���̽���
	static  CNewSkill::tagDoProcess					*DecodeDoProcess(BYTE* pByte, long& pos);							
	//��ռ��ܻ���
	static  void									ClearSkillCache();													
	//��ʼ2��ģ�����
	static  void									InitModuParam(CNewSkill *pSkillProperties,stModuParam *pParam,DWORD dwLevel);
	//��ʼ������ģ��
	static  void									InitBaseModule(CBaseModule *pBaseModule,DWORD dwID,DWORD dwLevel,CNewSkill *pSkillProperties,stModuParam *pParam);
	//��ʼ������
	static  void									InitProcedure(DWORD dwID,DWORD dwLevel,CProcedure *pProcedure,CNewSkillProperties::tagProcess* ptgProcess);
	//��ʼ������
	static  void									InitStep(DWORD dwID,DWORD dwLevel,CStep	*pStep,CNewSkillProperties::tagStep *ptgStep);	
	//��ʼ��Do	
	static  void									InitDo(DWORD dwID,DWORD dwLevel,CDo *pDo,CNewSkillProperties::tagDo *ptgDo);					
	//��ʼ��DO��������
	static  void									InitGroup(DWORD dwID,DWORD dwLevel,CNewSkillProperties::tagDoProcess *ptgDoProcess, CDo::tagGroup *pGroup);
	//��ʼ������
	static  void									InitOperator(DWORD dwID,CNewSkillProperties::tagDoProcess *ptgDoProcess, vector<COperator*> &vOperator);
	//��ʼ�ı�����
	static  void									InitChangeProperty(CNewSkillProperties::tagChangeProperty *ptgChangePropety, vector<COperator*> &vOperator);
	//��ʼ���ű�
	static  void									InitChangeScriptID(DWORD dwID,CNewSkillProperties::tagScriptID *ptgScriptID,vector<COperator*> &vOperator);
	static  void									InitAttackChangeObject(CNewSkillProperties::tagAttackChangeObject *ptgAttackChangeObject,vector<COperator*> &vOperator);
	//��ʼ��״̬
	static  void									InitBuffChange(CNewSkillProperties::tagChangeBuff *ptgBuff, vector<COperator*> &vOperator);
	//��ʼ���ı�λ��
	static  void									InitChangePos(CNewSkillProperties::tagChangePos *ptgPos, vector<COperator*> &vOperator);
	//��ʼ���ı�ʱ��
	static  void									InitCooldownChange(CNewSkillProperties::tagCoolDownChange *ptgCool, vector<COperator*> &vOperator);
	//��ʼ���˺�
	static  void									InitDamageChange(CNewSkillProperties::tagDamageChange *ptgDamage, vector<COperator*> &vOperator);
	//��ʼ��Ŀ�����
	static  void									InitDestObjChange(CNewSkillProperties::tagDestObjChange *ptgDest, vector<COperator*> &vOperator);
	//��ʼ��ע���¼�
	static  void									InitEventChange(DWORD dwID, CNewSkillProperties::tagRegisterEvent *ptgEvent,vector<COperator*> &vOperator);
	//�¼�
	static  void									InitTimerChange(DWORD dwID, CNewSkillProperties::tagRegisterTimer *ptgTimer,vector<COperator*> &vOperator);
	//������Ϣ
	static  void									InitSendMsg(CNewSkillProperties::tagSendMsg *ptgSendMsg ,vector<COperator*> &vOperator);
	//��ʼ��DO�˺�
	static  void									InitDoDamage(CNewSkillProperties::tagDoDamage *ptgDoDamage, vector<COperator*> &vOperator);
	//��ʼ����Ʒ
	static  void									InitGoodChange(CNewSkillProperties::tagGoodChange *ptgGoodChange,vector<COperator*> &vOperator);
	//��ʼ��ע��ʱ��
	static  void									InitUnRegisterTimer(CNewSkillProperties::tagUnRegisterTimer *ptgUnRegisterTimer, vector<COperator*> &vOperator);
	//��ʼ��ע���¼�
	static  void									InitUnRegisterEvent(CNewSkillProperties::tagUnRegisterEvent *ptgUnRegisterEvent, vector<COperator*> &vOperator);
	//��ʼ������
	static  void									InitDirection(CNewSkillProperties::tagDirection *ptgDirection,vector<COperator*> &vOperator);
	//��ʼ�����ܲ���
	static  void									InitSkillChange(CNewSkillProperties::tagSkillChange  *ptgSkillChange,vector<COperator*> &vOperator);
	//��ʼ����������
	static  CCondition               			    *InitConcreateCondition(CNewSkillProperties::tagCondition *ptgConditon);

	static  CEffectObjects           				*InitEffectObject(DWORD dwID,DWORD dwLv,DWORD dwOperobjID);					//���ö���
	static  CCondiGroup								*InitConditionGroup(DWORD dwID,DWORD dwLv,DWORD dwGroupID);					//������
	static  CCondition								*InitCondition(DWORD dwID,DWORD dwLv,DWORD dwConditionID);					//����
	static  CConProperty							*InitBasePropertyCondition(CNewSkillProperties::tagCondition *ptgConditon); //��������
	static  CConGoods								*InitGoodCondittion(CNewSkillProperties::tagCondition *ptgConditon);		//��Ʒ
	static  CConBuff								*InitBufffCondition(CNewSkillProperties::tagCondition *ptgConditon);		//״̬
	static  CConDistance							*InitDistanceConditon(CNewSkillProperties::tagCondition *ptgConditon);		//����
	static  CConValidObj							*InitValidObjectCondition(CNewSkillProperties::tagCondition *ptgConditon);	//��ЧĿ��	
	static  CConScript								*InitScriptCondition(CNewSkillProperties::tagCondition *ptgConditon);		//�ű�	
	static	CConCooldown							*InitCoolDownCondition(CNewSkillProperties::tagCondition *ptgConditon);		//ʱ��仯	
	static	CConTimer								*InitTimerCondtion(CNewSkillProperties::tagCondition *ptgConditon);			//ʱ��	
	static	CConEvent								*InitEventCondition(CNewSkillProperties::tagCondition *ptgConditon);		//�¼�	
	static	CConBlock								*InitBlockCondition(CNewSkillProperties::tagCondition *ptgConditon);		//�赲	
	static	CConDir									*InitDirCondition(CNewSkillProperties::tagCondition *ptgConditon);			//����	
	static  CConRegion								*InitRegionCondition(CNewSkillProperties::tagCondition *ptgCondition);		//����


	static list<long>								GetInitOccuSkill(DWORD dwOccu);
	static eEventType								GetEventType(long nID, string strName);
	static eModuleOper								GetOperator(long nID,string strName);
	static eObj										GetObj(string strName);
	static eChildObjType							GetChildObjType(ModuleType type);	
	static tagDamageType							GetDamageType(long nID, string strName);
	static list<CConValidObj::eValidObjType>		GetValidType(list<string> lStr);
	static CConValidObj::eType						GetEType(string strType);
	static CCondiGroup::eGrouMode					GetGroupMode(string);
	static COpSendMsg::eSendMsgType					GetMsgType(long nID, string strName);
	static COpSendMsg::eMsgRange					GetRange(string strName);
	static eModuleRet								GetModuleRet(string str);
	static OPFUN									*GetOpFun(string name,long nID);
	static CMPFUN									*GetCmpFun(string name);
	static COpDirec::eDirec							GetDirec(string str);
	static CBaseModule								*CreateModule(DWORD dwID, DWORD dwLevel,stModuParam *pModuParam=NULL, DWORD dwGlobleID=0,long lGoodsIndex=0,long lType=0);
	static void										RealeaseBaseMoudule(stModuParam *pModuParam);	
	static CBaseModule								*GetBaseModule(stModuParam *pParam);
	static void										GetConditon(BYTE* pByte, long& pos, string strType,DWORD dwLength,CNewSkillProperties::tagCondition *ptgCondition);
	static ModuleType								GetModuleType(string strName);
	static eSkillUseType							GetUseType(BYTE bUseType);
	static eAddSub									GetAddSub(string strAddSub);
	static BOOL										CheckListModule(listModuParam lModuParam,stModuParam *pModuParam);
	static void										OutputBasemodelInfo(const char *pszFileName);
	static void										CheckBaseModuleTimeOut(long lNowTime);
	static CVariableList						    *GetVariableList(string strFileName);
	static void										AddVariableList(string strFileName, CVariableList *pVariableList);
	//����hashֵ
	static long										CacuHashValue(long lID,long lLevle);

	static void										RealeaseModuParam();
	static void										RealeaseModuParam(stModuParam *pModuParam);
};
#endif
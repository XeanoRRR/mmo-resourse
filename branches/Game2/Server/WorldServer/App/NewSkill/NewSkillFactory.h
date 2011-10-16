///////////////////////////////////////////////////////////////////////////////
///  NewSkillFactory.h
///
///  PATH: E:\alienbrainWork\Game_Program_Develop\guangmang\Server/WorldServer\App/New Skill
///
///  CREATED: 08/07/2007 12:50:01 PM by ���Ƚ�
///
///  PURPOSE:   ���ܹ�����
///
// NewSkillFactory.h: interface for the CNewSkillFactory class.
//
//////////////////////////////////////////////////////////////////////
#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_NEWSKILL_FACTORY_INCLUDED
#define _INC_NEWSKILL_FACTORY_INCLUDED
#include "NewSkill.h"

class CNewSkillFactory
{

public:

    //������������
    struct tagSkillCommon 
    {
        map<DWORD,CNewSkill::tagConditionGroup*>	m_mapConditionGroup;		//������
        map<DWORD,CNewSkill::tagCondition*>		    m_mapCondition;				//������ 
        map<DWORD,CNewSkill::tagExpression>		    m_mapFormat;				//���㹫ʽ			
        map<DWORD,CNewSkill::tagOperateObject*>	    m_mapOperateObject;		    //�������� 
    };

private:
	//״̬����ʱ�б�
	static map<DWORD, CNewSkill*>				g_mapNewSkill;
	static DWORD								g_dwVersion;
	static map<DWORD, TiXmlElement*>			g_mapTemplateElem;								
	static map<DWORD, TiXmlElement*>			g_mapComTemplateElem;
	static list<long>							g_lInitSkillList;
	static map<DWORD,list<long>>				g_mapOccuInitSkill;
    static tagSkillCommon                       g_skillCommon;

public:
	static list<long>		GetOccuSkillList(DWORD dwOccu);
	static list<long>		GetInitSkillList()	{return g_lInitSkillList;}
	//��ȡ״̬�����ļ�
	static HRESULT			LoadConfigration	( const string p );
	//ͨ���ڵ�����
	static bool				LoadConfigration	( TiXmlElement *pElem );
	//��������ȼ��ľ�̬����
	static void				LoadLevelStatiParam( TiXmlElement *pElem, CNewSkill::tagSkill *pSkill);
	static void				LoadCommonConfig( TiXmlElement *pElem, CNewSkill *pNewSkill,CNewSkill::tagSkill *ptgSkill);
	static void				LoadNewLevelConfigration( TiXmlElement *pElem, CNewSkill *pNewSkill);
	//��������ȼ���״̬����
	static void				LoadLevelConfigration( TiXmlElement *pElem, CNewSkill *pNewSkill);
	//���빫ʽ����
	static void				LoadExpresstionConfigration(TiXmlElement *pElem, CNewSkill *pNewSkill);
	//��������
	static CNewSkill::tagCondition *LoadConditionConfigration(TiXmlElement *pElem,string strType,DWORD dwID,CNewSkill::tagSkill *ptgSkill=NULL);
	//���������������
	static void				LoadPropertyCondition(TiXmlElement *pElem,CNewSkill::tagConProperty *ptgConProperty);
	//������Ʒ����
	static void				LoadGoodCondition(TiXmlElement *pElem,CNewSkill::tagConGood *ptgConGood);
	//״̬����
	static void				LoadBuffCondition(TiXmlElement *pElem,CNewSkill::tagConBuff *ptgConBuff);
    //״̬��ɢ
    static void				LoadDispelCondition(TiXmlElement *pElem,CNewSkill::tagConDispel *ptgConDispel);
    //��������
    static void             LoadOddsCondition(TiXmlElement *pElem,CNewSkill::tagConOdds *ptgConOdds);
	//��������
	static void				LoadSkillCondition(TiXmlElement *pElem,CNewSkill::tagConSkill *ptgConSkill);
	//��������
	static void				LoadDistanceCondition(TiXmlElement *pElem,CNewSkill::tagConDistance *ptgConDistance);
	//�����¼�
	static void				LoadEventCondition(TiXmlElement *pElem,CNewSkill::tagConEvent *ptgConEvent);
	//�����赲
	static void				LoadBlockCondition(TiXmlElement *pElem,CNewSkill::tagConBlock *ptgConBlock);
	//����ְҵ����
	static void				LoadOccupationCondition(TiXmlElement *pElem,CNewSkill::tagConOccupation *ptgConOccupation);
	//����ʱ������
	static void				LoadTimerCondition(TiXmlElement *pElem,CNewSkill::tagConTimer *ptgTimer);
	//����������
	static void				LoadFaceCondition(TiXmlElement *pElem,CNewSkill::tagConFace *ptgFace);
	//���볡������
	static void				LoadRegionCondition(TiXmlElement *pElem,CNewSkill::tagConRegion *ptgRegion);
	//�������
	static void				LoadValidObject(TiXmlElement *pElem,CNewSkill::tagConValidObject *ptgConValidObject);
	//����ű�
	static void				LoadScript(TiXmlElement *pElem,CNewSkill::tagConScript *ptgConScript);
	//����������
	static void				LoadConditionGroupfigration(TiXmlElement *pElem,CNewSkill *pNewSkill);
	//����ģ��
	static void				LoadTemplateConfig(TiXmlElement *pElem);
	//����ģ��
	static void				LoadComTemplate(TiXmlElement *pElem);
	//���빫��ģ��
	static void				LoadComTemplate(TiXmlElement *pElem,CNewSkill *pNewSkill);
	//�����������
	static void				LoadOperatObjects(TiXmlElement *pElem,CNewSkill *pNewSkill);	
	//�������
	static void				LoadProcesseExecute(TiXmlElement *pElem,CNewSkill::tagSkill *ptgSkill,CNewSkill *pNewSkill);
	//���벽��
	static void				LoadStep(TiXmlElement *pElem,CNewSkill::tagStep *ptgStep,CNewSkill *pNewSkill,CNewSkill::tagSkill *ptgSkill);
	//���봦��ľ������
	static void				LoadStepProcess(TiXmlElement *pElem,CNewSkill::tagCDo *ptgCDo,CNewSkill::tagSkill *ptgSkill);
	//��������
	static void				LoadPropertyChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill);
	//״̬
	static void				LoadBuffChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill);
	//λ��
	static void				LoadPosChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess);
	//��������
	static void				LodAttackObject(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess);	
	//�ű�
	static void				LoadScriptChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess);	
	//ִ�й���
	static void				LoadDoProcess(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill);
	static void				LoadCDoProcess(TiXmlElement *pElem,CNewSkill::tagDo *ptgDo,CNewSkill::tagSkill *ptgSkill);
	//��ȡDWORD
	static DWORD			GetDwordAttribute(TiXmlElement *pElem,string strName);
    static float            GetFloatAttribute( TiXmlElement *elem, const std::string &attrName );
	//��ȡ�ַ���
	static string			GetStringAttribute(TiXmlElement *pElem,string strName);
	//��ȡ����
	static BYTE				GetIntAttribute(TiXmlElement *pElem,string strName);
	//��ȡ�ı�
	static string			GetTextAttribute(TiXmlElement *pElem);
	//��ȴʱ��
	static void				LoadCooldownChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill);
	//�����б�
	static void				LoadDestObjChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess);
	//���ܲ���
	static void				LoadSkillChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill);
    //�츳����
    static void             LoadGeniusChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess);
	//ע��ʱ��
	static void				LoadRegisterTimer(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill);
	//ע���¼�
	static void				LoadRegisterEvent(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess);
	//ע��ʱ��
	static void				LoadUnRegisterTimer(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess);
	//ע���¼�
	static void				LoadUnRegisterEvent(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess);
	//������Ϣ
	static void				LoadSendMsg(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill);
	//�˺�����
	static void				LoadDamageChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill);
	//do�˺�
	static void				LoadDoDamage(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill);
	//��Ʒ
	static void				LoadGoodChange(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess,CNewSkill::tagSkill *ptgSkill);
	//����
	static void				LoadDirection(TiXmlElement *pElem,CNewSkill::tagDoProcess *ptgDoProcess);
	//
	static void				LoadInitSkillConfigration(TiXmlElement *pElem);
	//
	static  HRESULT			LoadInitSkillConfig( const string pFileName );
	//�����б�
	static  HRESULT			LoadSkillList(const string pFileName, list<string>* listFileName);
	//
	static void				LoadSkillListConfigration(TiXmlElement *pElem,list<string>* listFileName);


	//���״̬����
	static void				ClearNewSkillCache	();								
	//����״̬����
	static bool				LoadNewSkillCache	();
	//����
	static bool				CodeNewSkill		( vector<BYTE>& vRet );	

    //
    static bool             LoadSkillCommon(const char* filename);
    static bool             CodeSkillCommon(vector<BYTE>& vRet);

};

#endif//_INC_NEWSKILL_FACTORY_INCLUDED
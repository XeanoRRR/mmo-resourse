//=============================================================================
/**
*  file: BaseModule.h
*
*  Brief:��װ��״̬�����ܵĻ���ģ�鹦��
*
*  Authtor:wangqiao
*	
*	Datae:2007-8-31
*/
//=============================================================================

#ifndef __BASE_MODULE_H__
#define __BASE_MODULE_H__

#pragma once

#include "BuffSkillDef.h"

class CMoveShape;
class CScript;
class CProcedure;

class CBaseModule : public BaseMemObj
{
public:
	CBaseModule(stModuParam *pParam,stStaticModuParam *pStatiParam=NULL);
	virtual ~CBaseModule(void);
private:
	//ID�͵ȼ�
	long m_nID;
	int  m_nLvl;
	//��̬����
	stModuParam	*m_pParam;
	//��̬����
	stStaticModuParam *m_pStatiParam;
	//�������򼯺�
	typedef vector<CProcedure*>::iterator itPro;
	vector<CProcedure*>	m_Procedures;	
private:	
	//����ʩ����
	void SetFireObjs();
	//������ķ��ؽ��
	eModuleRet NextProcedure();
	//����ִ�����й��� 
	eModuleRet DoSome(eModuleRet eRet);

public:

	//��ʼ������
	void Initial();	

	//��ȡID
	long				GetID()				{return m_nID;}		
	//��ȡ�ȼ�	
	int					GetLv()				{return m_nLvl;}	
	
	//��̬����
	stStaticModuParam *GetStaticParam()			{return m_pStatiParam;}			
	void			   SetStaticParam(stStaticModuParam* pParam)	{m_pStatiParam=pParam;}
	//��̬����
	stModuParam		   *GetParam()				{return m_pParam;}	
	void			   SetParam(stModuParam *pParam)				{m_pParam= pParam;}

	void ProcNumSub();	
		
	//��ʼ����������
	void InitBaseData(vector<CProcedure*> &vProcedure);
						
	// ��ӵ�CByteArray
	bool AddToByteArray(vector<BYTE>* pByteArray);
	// ��ӵ�CByteArrayΪ���͵��ͻ���
	bool AddToByteArrayForClient(vector<BYTE>* pByteArray,ModuleType eType);
	// ����
	bool DecordFromByteArray(long lProceNum,list<stTimerParam*>& Timers);

	// ��ӵ�DataBlock
	bool CodeToDataBlock(DBWriteSet& setWriteDB);
	// ����ӵ�DataBlockΪ�ͻ���
	bool CodeToDataBlockForClient(DBWriteSet& setWriteDB,ModuleType eType);
	// ����
	bool DecodeFromDataBlock(long lProceNum,list<stTimerParam*>& Timers);
	
	//���ܽӿ�///////////////////////////////////////////////////
	//��ʼ
	eModuleRet Begin(long lMouseX=0,long lMouseY=0,long lMouseShapeType=0,const CGUID& MouseShapeID=NULL_GUID,long lCreateID=0);
	//�¼�����ӿ�
	eModuleRet Proc(DWORD type,const tagProcVar *pVar);
	//��ʱ����ӿ�
	eModuleRet OnTimeOut(DWORD timerid,DWORD curtime,const void* var);
	//�������ýӿ�
	eModuleRet End();
};

#endif
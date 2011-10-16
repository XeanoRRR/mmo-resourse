#include "StdAfx.h"
#include "BuffSkillDef.h"
#include "..\..\GameServer\GameManager.h"
#include "..\script\VariableList.h"

//ʣ��ʱ��
void stTimerParam::GetRemainTime(DWORD &dwFutue,DWORD &dwCount)
{
	DWORD dwTime = timeGetTime()-nStartTime;
	if(lFuture>dwTime)
		dwFutue = lFuture-dwTime;
	else
		dwFutue = 0;	


	dwCount = nCount - nCalledCount;
}

//����TimerID�ҵ���Ӧ�Ĳ���
stTimerParam* stModuParam::FindTimerParam(long lTimerID)
{
	itTimer it = Timers.begin();
	for(;it != Timers.end();it++)
	{
		if((*it)->lTimerID == lTimerID)
			return (*it);
	}
	return NULL;
}


void stModuParam::ClearTimerAndEvent()
{
	if (Timers.size()>0)
	{
		for(stModuParam::itTimer it=Timers.begin(); it!=Timers.end();)
		{
			stTimerParam* pParam = (*it);
			if(pParam->lTimerID > 0)
				int nNum = TIMER_CANCEL(pParam->lTimerID);
			pParam->lTimerID = -1;
			it=Timers.erase(it);		
			MP_DELETE(pParam);
		}
		Timers.clear();
	}


	if (Events.size()>0)
	{
		for (list<stEventParam*>::iterator it=Events.begin(); it!=Events.end(); ++it)
		{		
			MP_DELETE(*it);
		}
		Events.clear();
	}
	
}

void stModuParam::ClearOperObj()
{
	if (OperObjs.size()>0)
	{
		list<stDestObj*>::iterator it = OperObjs.begin();
		for (; it!=OperObjs.end(); ++it)
		{			
			OBJ_RELEASE(stDestObj, *it);
		}
		OperObjs.clear();	
	}
	if (SendMsgPlayers.size()>0)
	{
		SendMsgPlayers.clear();
	}
	
}
void stModuParam::Clear()
{	
	ClearOperObj();
	ClearTimerAndEvent();
	MP_DELETE(pVariableList);	
}

//������������
void stModuParam::Reset(CMoveShape *pOwner,CMoveShape *pFire)
{
	nProceNum = 0;
	pUser = pOwner;
	pFirer = pFire;
	lDestX = 0;
	lDestY = 0;
	lCurEventType = 0;
	lCurTimerID = -1;
	bRun = false;

	// [5/4/2009 chenxianj]	
	ClearOperObj();
	ClearTimerAndEvent();
	pVariableList->AddVar("$ParamS",(DWORD)this);	
	GoodsGuid = NULL_GUID;
	
	if (pFirer)
	{
		nFirerType = pFirer->GetType();
		FirerID = pFirer->GetExID();	
	} 
	else
	{
		nFirerType = 0;
		FirerID = NULL_GUID;	
	}	
}

stTimerParam* stModuParam::FindTimerParam(long lFlag,long lDestType,const CGUID& DestID)
{
	itTimer it = Timers.begin();
	for(;it != Timers.end();it++)
	{
		if((*it)->lFlag == lFlag)
			return (*it);
	}
	return NULL;
}

//��ȡ�ű�����
long stModuParam::GetScriptParam(int nPos)
{
	int nNum=0;
	
	CNewSkill *pSkillProperties =  CNewSkillFactory::QueryNewSkillProperty(nID);
	if (pSkillProperties)
	{
		CNewSkillProperties::stStaticParam *pStacicParam = pSkillProperties->GetStaticParam(nLvl);

			if (pStacicParam)
			{
				list<long>::iterator it = pStacicParam->lScriptParam.begin();
				for (;it!=pStacicParam->lScriptParam.end();++it)
				{
					++nNum;
					if (nPos==nNum)
					{
						return (*it);
					}			
				}	
			}	
	}

	return 0;
}

//����Ŀ�����
void stModuParam::SetDesObjs(long lMouseX,long lMouseY,long lMouseShapeType,const CGUID& ShapeID)
{
	nMouseX = lMouseX;
	nMouseY = lMouseY;
	nMouseShapeType = lMouseShapeType;
	MouseShapeID = ShapeID;

	OperObjs.clear();
}


//��ʱ����
void stModuParam::OnTimerOut(long lTimerID)
{
	//����ʱ��
	itTimer it = Timers.begin();
	for(;it != Timers.end();it++)
	{
		if((*it)->lTimerID == lTimerID)
		{
			stTimerParam *pTimerParam =  (*it);
			if(pTimerParam->nCalledCount++ >= pTimerParam->nCount)
			{	
				//ע����ʱ��
				int nNum = TIMER_CANCEL(pTimerParam->lTimerID);
				pTimerParam->lTimerID = -1; 
				it = Timers.erase(it);

				MP_DELETE(pTimerParam);
				return;
			}
		}
	}
	return;
}


void stModuParam::ProcNumSub()
{
	--nProceNum;
	if((nProceNum)<=0) 
		nProceNum=0;
}

// ��ӵ�CByteArray
bool stModuParam::AddToByteArray(vector<BYTE>* pByteArray)
{
	DWORD dwFuture =0;
	DWORD dwCount = 0;
	//��ǰִ�еĹ����
	_AddToByteArray(pByteArray,nProceNum);
	//��ӵ�ǰ��ʱ����Ϣ
	long lTimerNum = Timers.size();
	_AddToByteArray(pByteArray,lTimerNum);
	stModuParam::itTimer it = Timers.begin();
	for(;it != Timers.end();it++)
	{
		stTimerParam *pTimerParam = (*it);
		pTimerParam->GetRemainTime(dwFuture,dwCount);

		if (bTime)
		{
			time_t t_CurrentTime = time(NULL);
			dwFuture = t_CurrentTime + dwFuture/1000; 
		}
		_AddToByteArray(pByteArray,pTimerParam->lFlag);
		_AddToByteArray(pByteArray,dwFuture);
		_AddToByteArray(pByteArray,dwCount);
	}
	return true;
}

// ��ӵ�CByteArrayΪ���͵��ͻ���
bool stModuParam::AddToByteArrayForClient(vector<BYTE>* pByteArray,ModuleType eType)
{
	//���id�͵ȼ�	
	_AddToByteArray(pByteArray,(long)nID);
	_AddToByteArray(pByteArray,(long)nLvl);
	//�����״̬��ӵ�һ����ʱ��ʣ��ʱ�䵽�ͻ���
	if(eType == ModuType_Buff || eType == ModuType_DeBuff)
	{
		stModuParam::itTimer it = Timers.begin();
		if( it!=Timers.end() )
		{
			stTimerParam *pTimerParam = (*it);
			DWORD dwRemainTime = 
				pTimerParam->lFuture+pTimerParam->lInter*(pTimerParam->nCount-pTimerParam->nCalledCount);
			_AddToByteArray(pByteArray,dwRemainTime);
		}
		else
		{
			_AddToByteArray(pByteArray,(DWORD)0);
		}
	}

	return true;
}

// ����
bool stModuParam::DecordFromByteArray(long lProceNum,list<stTimerParam*>& Timers)
{
	nProceNum = lProceNum;
	Timers = Timers;
	return true;
}


// ��ӵ�DataBlock
bool stModuParam::CodeToDataBlock(DBWriteSet& setWriteDB)
{
	DWORD dwFuture =0;
	DWORD dwCount = 0;
	//��ǰִ�еĹ����
	setWriteDB.AddToByteArray(nProceNum);
	//��ӵ�ǰ��ʱ����Ϣ
	long lTimerNum = Timers.size();
	setWriteDB.AddToByteArray(lTimerNum);
	stModuParam::itTimer it = Timers.begin();
	for(;it != Timers.end();it++)
	{
		stTimerParam *pTimerParam = (*it);
		pTimerParam->GetRemainTime(dwFuture,dwCount);
		setWriteDB.AddToByteArray(pTimerParam->lFlag);
		setWriteDB.AddToByteArray(dwFuture);
		setWriteDB.AddToByteArray(dwCount);
	}
	return true;
}
// ����ӵ�DataBlockΪ�ͻ���
bool stModuParam::CodeToDataBlockForClient(DBWriteSet& setWriteDB,ModuleType eType)
{
	//���id�͵ȼ�	
	setWriteDB.AddToByteArray((long)nID);
	setWriteDB.AddToByteArray((long)nLvl);
	//�����״̬��ӵ�һ����ʱ��ʣ��ʱ�䵽�ͻ���
	if(eType == ModuType_Buff || eType == ModuType_DeBuff)
	{
		stModuParam::itTimer it = Timers.begin();
		if( it!=Timers.end() )
		{
			stTimerParam *pTimerParam = (*it);
			DWORD dwRemainTime = 
				pTimerParam->lFuture+pTimerParam->lInter*(pTimerParam->nCount-pTimerParam->nCalledCount);
			setWriteDB.AddToByteArray(dwRemainTime);
		}
		else
		{
			setWriteDB.AddToByteArray((DWORD)0);
		}
	}
	return true;
}
// ����
bool stModuParam::DecodeFromDataBlock(long lProceNum,list<stTimerParam*>& lTimers)
{
	nProceNum = lProceNum;
	Timers = lTimers;
	return true;
}
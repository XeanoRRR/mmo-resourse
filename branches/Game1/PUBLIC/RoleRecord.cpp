#include "StdAfx.h"
#include "RoleRecord.h"
#include "public.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRoleRecord::CRoleRecord()
{
	m_lRoleID=0;
	m_cRoleStep=0;
	m_cAimNum=0;
	memset(m_Param,0,sizeof(m_Param));
}
CRoleRecord::CRoleRecord(LONG RoleID,LONG StepID)
{
	m_lRoleID=RoleID;
	m_cRoleStep=StepID;
	m_cAimNum=0;
	memset(m_Param,0,sizeof(m_Param));
}
CRoleRecord::~CRoleRecord()
{
}

// ѹ����
BOOL CRoleRecord::AddDatatoByteArray(vector<BYTE>* pByteArray)
{
	_AddToByteArray(pByteArray,m_lRoleID);
	_AddToByteArray(pByteArray,m_cRoleStep);
	_AddToByteArray(pByteArray,m_cAimNum);
	_AddToByteArray(pByteArray,m_Param,sizeof(sRoleParam)*m_cAimNum);
	//_AddToByteArray(pByteArray,(LONG)m_listRoleRecord.size());
	//for (list<CRoleRecord*>::iterator it=m_listRoleRecord.begin(); it!= m_listRoleRecord.end(); it++)
	//{
	//	_AddToByteArray(pByteArray,*it,sizeof(CRoleRecord));
	//}

	return TRUE;
}
//��������
BOOL CRoleRecord::DecordDataFromByteArray(BYTE* pByte, long& pos)
{
	m_lRoleID=_GetLongFromByteArray(pByte,pos);
	m_cRoleStep=_GetByteFromByteArray(pByte,pos);
	m_cAimNum= _GetByteFromByteArray(pByte,pos);
	_GetBufferFromByteArray(pByte,pos,m_Param,sizeof(sRoleParam)*m_cAimNum);
	return TRUE;
}

BOOL CRoleRecord::CodeToDataBlock(DBWriteSet& setWriteDB)
{
	setWriteDB.AddToByteArray(m_lRoleID);
	setWriteDB.AddToByteArray(m_cRoleStep);
	setWriteDB.AddToByteArray(m_cAimNum);
	setWriteDB.AddToByteArray(m_Param,sizeof(sRoleParam)*m_cAimNum);
	return TRUE;
}
BOOL CRoleRecord::DecodeFromDataBlock(DBReadSet& setReadDB)
{
	m_lRoleID=setReadDB.GetLongFromByteArray();
	m_cRoleStep=setReadDB.GetByteFromByteArray();
	m_cAimNum= setReadDB.GetByteFromByteArray();
	setReadDB.GetBufferFromByteArray(m_Param,sizeof(sRoleParam)*m_cAimNum);
	return TRUE;
}

LONG CRoleRecord::GetParam(LONG Index)
{
	if (Index>=0 && Index<ROLE_PRAME_NUM )
	{
		return m_Param[Index].m_wParam;
	}
	return ROLE_ERRO;
}

BOOL CRoleRecord::SetParam(LONG Index, LONG val)
{
	if (Index>=0 && Index<ROLE_PRAME_NUM )
	{
		if(Index>= m_cAimNum)
		{
			m_cAimNum=Index+1;
		}
		m_Param[Index].m_wParam=val;
		return TRUE;
	}
	return FALSE;
}

BOOL CRoleRecord::AddParam(LONG Index, LONG addval)
{
	if (Index>=0 && Index<ROLE_PRAME_NUM )
	{
		if(Index>= m_cAimNum)
		{
			m_cAimNum=Index+1;
		}
		m_Param[Index].m_wParam+=addval;
		return TRUE;
	}
	return FALSE;
}

LONG CRoleRecord::GetAccParam(LONG Index)
{
	if (Index>=0 && Index<ROLE_PRAME_NUM )
	{
		return m_Param[Index].m_lAccParam;
	}
	return ROLE_ERRO;
}

BOOL CRoleRecord::SetAccParam(LONG Index, LONG val)
{
	if (Index>=0 && Index<ROLE_PRAME_NUM )
	{
		if(Index>= m_cAimNum)
		{
			m_cAimNum=Index+1;
		}
		m_Param[Index].m_lAccParam=val;
		return TRUE;
	}
	return FALSE;
}

BOOL CRoleRecord::AddAccParam(LONG Index, LONG addval)
{
	if (Index>=0 && Index<ROLE_PRAME_NUM )
	{
		if(Index>= m_cAimNum)
		{
			m_cAimNum=Index+1;
		}
		m_Param[Index].m_lAccParam+=addval;
		return TRUE;
	}
	return FALSE;
}

//��ö����¼����
WORD CRoleRecord::GetExcessRecordIndex(LONG Index)
{
	if (Index>=0 && Index<ROLE_PRAME_NUM )
	{
		return m_Param[Index].m_wExcessRecordIndex;
	}
	return 0;
}

//���ö����¼����
BOOL CRoleRecord::SetExcessRecordIndex(LONG Index,WORD addval)
{
	if (Index>=0 && Index<ROLE_PRAME_NUM )
	{
		if(Index>= m_cAimNum)
		{
			m_cAimNum=Index+1;
		}
		m_Param[Index].m_wExcessRecordIndex=addval;
		return TRUE;
	}
	return FALSE;
}


	//����id
	//ȡ�ñ���ֵ
//LONG CRoleRecord::GetRegionID(LONG Index)
//{
//	if (Index>=0 && Index<ROLE_PRAME_NUM )
//	{
//		return 0;//m_Param[Index].m_lRegionID;
//	}
//	return ROLE_ERRO;
//}
	
	//���ñ���ֵ
//BOOL CRoleRecord::SetRegionID(LONG Index, LONG val)
//{
//	if (Index>=0 && Index<ROLE_PRAME_NUM )
//	{
//		m_Param[Index].m_lRegionID=val;
//		return TRUE;
//	}
//	return FALSE;
//}
	

	
	//����x
	//ȡ�ñ���ֵ
//LONG CRoleRecord::GetRegionX(LONG Index)
//{
//	if (Index>=0 && Index<ROLE_PRAME_NUM )
//	{
//		return 0;// m_Param[Index].m_lRegionX;
//	}
//	return ROLE_ERRO;
//}
	
	//���ñ���ֵ
//BOOL CRoleRecord::SetRegionX(LONG Index, LONG val)
//{
//	if (Index>=0 && Index<ROLE_PRAME_NUM )
//	{
//		//m_Param[Index].m_lRegionX=val;
//		return TRUE;
//	}
//	return FALSE;
//}

	
	//����y
	//ȡ�ñ���ֵ
//LONG CRoleRecord::GetRegionY(LONG Index)
//{
//	if (Index>=0 && Index<ROLE_PRAME_NUM )
//	{
//		return m_Param[Index].m_lRegionY;
//	}
//	return ROLE_ERRO;
//}
	
	//���ñ���ֵ
//BOOL CRoleRecord::SetRegionY(LONG Index, LONG val)
//{
//	if (Index>=0 && Index<ROLE_PRAME_NUM )
//	{
//		m_Param[Index].m_lRegionY=val;
//		return TRUE;
//	}
//	return FALSE;
//
//}
	
//------------------CRoleQueue
CRoleQueue::CRoleQueue()
{
	m_pCurrentPtr=NULL;
	m_lChapter=0;
}
CRoleQueue::~CRoleQueue()
{
	list<CRoleRecord*>::iterator it=m_listRoleRecord.begin();
	for (; it!= m_listRoleRecord.end(); it++)
	{
		delete *it;
	}
}

CRoleRecord *CRoleQueue::GetRecordByID(LONG ID)
{
	list<CRoleRecord*>::iterator it=m_listRoleRecord.begin();
	for (;it!=m_listRoleRecord.end();it++)
	{
		if ( ((*it)->GetRoleID()==ID))
		{
			return (*it);
		}
	}
	return NULL;
}

LONG CRoleQueue::GetRoleStepNum(LONG ID)
{
	CRoleRecord* record=GetRecordByID(ID);
	if (record==NULL)
	{
		return 0;
	}
	return record->GetRoleStep();
}

BOOL CRoleQueue::FindRoleStep(LONG ID , LONG Step)
{
	list<CRoleRecord*>::iterator it=m_listRoleRecord.begin();
	for (;it!=m_listRoleRecord.end();it++)
	{
		if ( ((*it)->GetRoleID()==ID) && ((*it)->GetRoleStep()==Step) )
		{
			m_pCurrentPtr=(*it);
			return TRUE;
		}
	}
	m_pCurrentPtr = NULL;
	return FALSE;
}

//LONG CRoleQueue::GetParam(LONG Index)
//{
//	if (m_pCurrentPtr==0)
//	{
//		return ROLE_ERRO;
//	}
//	if (Index < ROLE_PRAME_NUM)
//	{
//		return m_pCurrentPtr->GetParam(Index);
//	}
//	return ROLE_ERRO;
//}

LONG CRoleQueue::GetParam(LONG RoleID, LONG Step, LONG Index)
{
	if ( FindRoleStep(RoleID,Step))
	{
		return m_pCurrentPtr->GetParam(Index);
	}
	return ROLE_ERRO;
}

//BOOL CRoleQueue::SetParam(LONG Index, LONG val)
//{
//	if (m_pCurrentPtr==NULL)
//	{
//		return ROLE_ERRO;
//	}
//	return m_pCurrentPtr->SetParam(Index,val);
//}

BOOL CRoleQueue::SetParam(LONG RoleID, LONG Step, LONG Index, LONG val)
{
	if (FindRoleStep(RoleID,Step))
	{
		return m_pCurrentPtr->SetParam(Index,val);
	}
	return FALSE;
}

//BOOL CRoleQueue::AddParam(LONG Index, LONG val)
//{
//	if (m_pCurrentPtr==NULL)
//	{
//		return FALSE;
//	}
//	return m_pCurrentPtr->AddParam(Index,val);
//}

BOOL CRoleQueue::AddParam(LONG RoleID, LONG Setp, LONG Index, LONG val)
{
	if (FindRoleStep(RoleID,Setp))
	{
		return m_pCurrentPtr->AddParam(Index,val);
	}
	return FALSE;
}
//........................................
//LONG CRoleQueue::GetAccParam(LONG Index)
//{
//	if (m_pCurrentPtr==0)
//	{
//		return ROLE_ERRO;
//	}
//	if (Index < ROLE_PRAME_NUM)
//	{
//		return m_pCurrentPtr->GetAccParam(Index);
//	}
//	return ROLE_ERRO;
//}

LONG CRoleQueue::GetAccParam(LONG RoleID, LONG Step, LONG Index)
{
	if ( FindRoleStep(RoleID,Step))
	{
		return m_pCurrentPtr->GetAccParam(Index);
	}
	return ROLE_ERRO;
}

//BOOL CRoleQueue::SetAccParam(LONG Index, LONG val)
//{
//	if (m_pCurrentPtr==NULL)
//	{
//		return ROLE_ERRO;
//	}
//	return m_pCurrentPtr->SetAccParam(Index,val);
//}

BOOL CRoleQueue::SetAccParam(LONG RoleID, LONG Step, LONG Index, LONG val)
{
	if (FindRoleStep(RoleID,Step))
	{
		return m_pCurrentPtr->SetAccParam(Index,val);
	}
	return FALSE;
}

//BOOL CRoleQueue::AddAccParam(LONG Index, LONG val)
//{
//	if (m_pCurrentPtr==NULL)
//	{
//		return FALSE;
//	}
//	return m_pCurrentPtr->AddAccParam(Index,val);
//}

BOOL CRoleQueue::AddAccParam(LONG RoleID, LONG Setp, LONG Index, LONG val)
{
	if (FindRoleStep(RoleID,Setp))
	{
		return m_pCurrentPtr->AddAccParam(Index,val);
	}
	return FALSE;
}

//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĳ���id
LONG CRoleQueue::GetRegionID(LONG RoleID, LONG Step ,LONG Index)
{
	if ( !FindRoleStep(RoleID,Step))
	{
		return ROLE_ERRO;
	}
	WORD wExcRecordIndex=m_pCurrentPtr->GetExcessRecordIndex(Index);
	if (wExcRecordIndex==0)
	{
		return ROLE_ERRO;		
	}
	map<WORD,sExcessRecord>::iterator it= m_mapExcessRecord.find(wExcRecordIndex);
	if ( it == m_mapExcessRecord.end())
	{
		return ROLE_ERRO;
	}
	return (*it).second.m_lRegionID;
	
}


//���� ĳһ����ĳһ�����Index��Ӧ�ĳ���id
BOOL CRoleQueue::SetRegionID(LONG RoleID,  LONG Step,LONG Index, LONG val)
{	
	if ( !FindRoleStep(RoleID,Step))
	{
		return FALSE;
	}
	WORD wExcRecordIndex=m_pCurrentPtr->GetExcessRecordIndex(Index);
	if (wExcRecordIndex==0)
	{
		wExcRecordIndex=CreatExcessRecord(val,0,0);
		m_pCurrentPtr->SetExcessRecordIndex(Index,wExcRecordIndex);
		return TRUE;		
	}
	map<WORD,sExcessRecord>::iterator it= m_mapExcessRecord.find(wExcRecordIndex);
	if ( it == m_mapExcessRecord.end())
	{
		return FALSE;
	}
	(*it).second.m_lRegionID= val;
	return TRUE;
}

//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĳ���x����
LONG CRoleQueue::GetRegionX(LONG RoleID, LONG Step ,LONG Index)
{	
	if ( !FindRoleStep(RoleID,Step))
	{
		return ROLE_ERRO;
	}
	WORD wExcRecordIndex=m_pCurrentPtr->GetExcessRecordIndex(Index);
	if (wExcRecordIndex==0)

	{
		return ROLE_ERRO;		
	}
	map<WORD,sExcessRecord>::iterator it= m_mapExcessRecord.find(wExcRecordIndex);

	if ( it == m_mapExcessRecord.end())
	{
		return ROLE_ERRO;
	}
	return (*it).second.m_lRegionX;
}

//���� ĳһ����ĳһ�����Index��Ӧ�ĳ���x����
BOOL CRoleQueue::SetRegionX(LONG RoleID,  LONG Step,LONG Index, LONG val)
{	
	if ( !FindRoleStep(RoleID,Step))
	{
		return FALSE;
	}
	WORD wExcRecordIndex=m_pCurrentPtr->GetExcessRecordIndex(Index);
	if (wExcRecordIndex==0)
	{
		wExcRecordIndex=CreatExcessRecord(0,val,0);
		m_pCurrentPtr->SetExcessRecordIndex(Index,wExcRecordIndex);
		return TRUE;		
	}
	map<WORD,sExcessRecord>::iterator it= m_mapExcessRecord.find(wExcRecordIndex);
	if ( it == m_mapExcessRecord.end())
	{
		return FALSE;
	}
	(*it).second.m_lRegionX= val;
	return TRUE;
}

//ȡ�� ĳһ����ĳһ�����Index��Ӧ�ĳ���y����
LONG CRoleQueue::GetRegionY(LONG RoleID, LONG Step ,LONG Index)
{	
	if ( !FindRoleStep(RoleID,Step))
	{
		return ROLE_ERRO;
	}
	WORD wExcRecordIndex=m_pCurrentPtr->GetExcessRecordIndex(Index);
	if (wExcRecordIndex==0)
	{
		return ROLE_ERRO;		
	}
	map<WORD,sExcessRecord>::iterator it= m_mapExcessRecord.find(wExcRecordIndex);
	if ( it == m_mapExcessRecord.end())
	{
		return ROLE_ERRO;
	}
	return (*it).second.m_lRegionY;
}

//���� ĳһ����ĳһ�����Index��Ӧ�ĳ���y����
BOOL CRoleQueue::SetRegionY(LONG RoleID,  LONG Step,LONG Index, LONG val)
{	
	if ( !FindRoleStep(RoleID,Step))
	{
		return FALSE;
	}
	WORD wExcRecordIndex=m_pCurrentPtr->GetExcessRecordIndex(Index);
	if (wExcRecordIndex==0)
	{
		wExcRecordIndex=CreatExcessRecord(0,0,val);
		m_pCurrentPtr->SetExcessRecordIndex(Index,wExcRecordIndex);
		return TRUE;		
	}
	map<WORD,sExcessRecord>::iterator it= m_mapExcessRecord.find(wExcRecordIndex);
	if ( it == m_mapExcessRecord.end())
	{
		return FALSE;
	}
	(*it).second.m_lRegionY= val;
	return TRUE;
}


//���� m_pCurrentPtr ָ��������Index�ĳ���id
//BOOL CRoleQueue::SetRegionID(LONG Index, LONG val)
//{
//	if (m_pCurrentPtr==NULL)
//	{
//		return FALSE;
//	}
//	return m_pCurrentPtr->SetRegionID(Index,val);
//}

//���� m_pCurrentPtr ָ��������Index�ĳ���id
//LONG CRoleQueue::GetRegionID(LONG Index)
//{
//	if (m_pCurrentPtr==NULL)
//	{
//		return FALSE;
//	}
//	return m_pCurrentPtr->GetRegionID(Index);
//}
//ȡ��  m_pCurrentPtr ָ��������Index�ĳ���x
//LONG CRoleQueue::GetRegionX(LONG Index)
//{
//	if (m_pCurrentPtr==NULL)
//	{
//		return FALSE;
//	}
//	return m_pCurrentPtr->GetRegionX(Index);
//}

//���� m_pCurrentPtr ָ��������Index�ĳ���x
//BOOL CRoleQueue::SetRegionX(LONG Index, LONG val)
//{
//	if (m_pCurrentPtr==NULL)
//	{
//		return FALSE;
//	}
//	return m_pCurrentPtr->SetRegionX(Index,val);
//}

//ȡ��  m_pCurrentPtr ָ��������Index�ĳ���y
//LONG CRoleQueue::GetRegionY(LONG Index)
//{
//	if (m_pCurrentPtr==NULL)
//	{
//		return FALSE;
//	}
//	return m_pCurrentPtr->GetRegionY(Index);
//}

//���� m_pCurrentPtr ָ��������Index�ĳ���y
//BOOL CRoleQueue::SetRegionY(LONG Index, LONG val)
//{
//	if (m_pCurrentPtr==NULL)
//	{
//		return FALSE;
//	}
//	return m_pCurrentPtr->SetRegionY(Index,val);
//}




VOID CRoleQueue::Clean()
{
	list<CRoleRecord*>::iterator it= m_listRoleRecord.begin();
	for (; it!= m_listRoleRecord.end(); it++)
	{
		delete *it;
	}
	m_listRoleRecord.clear();
	m_setRoleRecord.clear();
	m_mapExcessRecord.clear();
}

//BOOL CRoleQueue::AddParam(LONG RoleID, LONG Setp, LONG Index, LONG val)
//{
//	if (FindRoleStep(RoleID,Setp))
//	{
//		return m_pCurrentPtr->AddParam(Index,val);
//	}
//	return FALSE;
//}


BOOL CRoleQueue::AddOneRecord(CRoleRecord*& ptr)
{
	if (m_listRoleRecord.size() >= ROLE_NUM_MAX)
	{
		return FALSE;	
	}

	if (GetRecordByID(ptr->GetRoleID())!=NULL)
	{
		return FALSE;
	}

	m_listRoleRecord.push_back(ptr);
	ptr=NULL;
	return TRUE;
}

BOOL CRoleQueue::AddOneRecord(LONG roleid,LONG step)
{
	CRoleRecord* record= new CRoleRecord(roleid,step);
	if (!AddOneRecord(record))
	{
		delete record;
		return FALSE;
	}
	return TRUE;
}

BOOL CRoleQueue::DelOneRecord(LONG ID)
{
	list<CRoleRecord*>::iterator it=m_listRoleRecord.begin();
	while (it!=m_listRoleRecord.end())
	{
		if ((*it)->GetRoleID()==ID)
		{
			CRoleRecord* pRole=(*it);
			ClearOneRecordEx(pRole);
			delete *it;
			m_listRoleRecord.erase(it);
			return TRUE;
		}
		it++;
	}
	return FALSE;
}

//����һ������ĸ��Ӽ�¼
BOOL CRoleQueue::ClearOneRecordEx(CRoleRecord* pRoleRecord)
{
	if (pRoleRecord==NULL) return FALSE;
	//ROLE_PRAME_NUM
	for(int a=0; a<ROLE_PRAME_NUM; a++)
	{
		WORD ExIndex=pRoleRecord->GetExcessRecordIndex(a);
		if (ExIndex != 0)
		{
			map<WORD , sExcessRecord>::iterator it= m_mapExcessRecord.find(ExIndex);
			if (it!= m_mapExcessRecord.end())	
				m_mapExcessRecord.erase(it);
		}
	}
	return TRUE;
}

BOOL CRoleQueue::DeleteExcessRecord()
{
	map<WORD , sExcessRecord>::iterator it=m_mapExcessRecord.begin();
	while(it!=m_mapExcessRecord.end())
	{
		WORD wIndex=it->first;
		list<CRoleRecord*>::iterator itlist=m_listRoleRecord.begin();
		bool b=false;
		//�Ƿ�������Ķ����¼��wIndex
		for (;itlist!=m_listRoleRecord.end();itlist++)
		{
			//�ҵ�һ�����ʾ�����¼����Ч���ݡ�wIndexΨһ������ѭ��
			if((*itlist)->IsExist(wIndex))
			{
				b=true;
				break;
			}
		}
		//��Ч�Ķ����¼
		if (!b)
		{
			m_mapExcessRecord.erase(it++);
		}
		else
			++it;
	}
	return TRUE;
}

BOOL CRoleRecord::IsExist(WORD wIndex)
{
	for (int i=0;i<ROLE_PRAME_NUM;i++)
	{
		if (m_Param[i].m_wExcessRecordIndex==wIndex)
		{
			return TRUE;
		}
	}
	return FALSE;
}

//�����������n��
VOID CRoleQueue::JumpStepTo(LONG lRoleID,BYTE stepnum)
{
	CRoleRecord *p=GetRecordByID(lRoleID);
	if (p!=NULL)
	{
		ClearOneRecordEx(p);
		p->JumpStepTo(stepnum);

	}
}


//ѹ��
BOOL CRoleQueue::AddQuestDataByteArray(vector<BYTE>* pByteArray)
{

	_AddToByteArray(pByteArray,m_lChapter);
	//_AddToByteArray(pByteArray,(LONG)m_mapRoleRecord.size());

	_AddToByteArray(pByteArray,(LONG)m_listRoleRecord.size());
	for (list<CRoleRecord*>::iterator it=m_listRoleRecord.begin(); it!= m_listRoleRecord.end(); it++)
	{
		//_AddToByteArray(pByteArray,*it,sizeof(CRoleRecord));
		(*it)->AddDatatoByteArray(pByteArray);
	}

	_AddToByteArray(pByteArray,(LONG)m_mapExcessRecord.size());
	for (map<WORD,sExcessRecord>::iterator it= m_mapExcessRecord.begin(); it!= m_mapExcessRecord.end() ; it++)
	{
		_AddToByteArray(pByteArray,(WORD)(*it).first);
		_AddToByteArray(pByteArray,&(*it).second,sizeof(sExcessRecord));
		//(*it).second.AddDatatoByteArray(pByteArray);
	}

	_AddToByteArray(pByteArray,(LONG)m_setRoleRecord.size());
	for (set<LONG>::iterator ite=m_setRoleRecord.begin(); ite!= m_setRoleRecord.end(); ite++)
	{
		_AddToByteArray(pByteArray,(LONG)*ite);
	}


	//...
	//map<LONG , BOOL>		m_mapRoleRecord;					//�����¼
	//list<CRoleRecord*>		m_listRoleRecord;					//�����б�
	//CRoleRecord*			m_pCurrentPtr;						//����ָ�룬�Ż��ٶȡ�

	//LONG					m_lChapter;							//�������������½�
	//LONG					m_lMostlyRoleNum;					//��������ǰid


	return TRUE;
}


//����
BOOL CRoleQueue::DecordQuestDataFromByteArray(BYTE* pByte, long& pos)
{
	Clean();
	m_lChapter=_GetLongFromByteArray(pByte,pos);
	LONG rolenum= _GetLongFromByteArray(pByte,pos);

	for (int a=0 ; a<rolenum; a++)
	{
		CRoleRecord* record=new CRoleRecord;
		//_GetBufferFromByteArray(pByte,pos,record,sizeof(CRoleRecord));
		record->DecordDataFromByteArray(pByte,pos);
		//m_listRoleRecord.push_back(record);
		if (!AddOneRecord(record))
		{
			delete record;
		}
	}

	LONG lExcRecordNun= _GetLongFromByteArray(pByte,pos);
	for (int a=0; a< lExcRecordNun; a++)
	{
		sExcessRecord record;
		WORD index= _GetShortFromByteArray(pByte,pos);
		_GetBufferFromByteArray(pByte,pos,&record,sizeof(sExcessRecord));
		m_mapExcessRecord[index]= record;
	}

	LONG rolerecordnum=_GetLongFromByteArray(pByte,pos);

	for (int a=0; a<rolerecordnum; a++)
	{
		LONG _long=_GetLongFromByteArray(pByte,pos);//_GetCharFromByteArray(pByte,pos);
		//m_setRoleRecord[_long]=TRUE;
		m_setRoleRecord.insert(_long);
	}
	DeleteExcessRecord();
	return FALSE;
}

BOOL CRoleQueue::CodeToDataBlock(DBWriteSet& setWriteDB)
{
	setWriteDB.AddToByteArray(m_lChapter);
	
	setWriteDB.AddToByteArray((LONG)m_listRoleRecord.size());
	for (list<CRoleRecord*>::iterator it=m_listRoleRecord.begin(); it!= m_listRoleRecord.end(); it++)
	{
		(*it)->CodeToDataBlock(setWriteDB);
	}

	setWriteDB.AddToByteArray((LONG)m_mapExcessRecord.size());
	for (map<WORD,sExcessRecord>::iterator it= m_mapExcessRecord.begin(); it!= m_mapExcessRecord.end() ; it++)
	{
		setWriteDB.AddToByteArray((WORD)(*it).first);
		setWriteDB.AddToByteArray(&(*it).second,sizeof(sExcessRecord));
	}

	setWriteDB.AddToByteArray((LONG)m_setRoleRecord.size());
	for (set<LONG>::iterator ite=m_setRoleRecord.begin(); ite!= m_setRoleRecord.end(); ite++)
	{
		setWriteDB.AddToByteArray((LONG)*ite);
	}

	return TRUE;
}
BOOL CRoleQueue::DecodeFromDataBlock(DBReadSet& setReadDB)
{
	Clean();
	m_lChapter=setReadDB.GetLongFromByteArray();
	LONG rolenum= setReadDB.GetLongFromByteArray();

	for (int a=0 ; a<rolenum; a++)
	{
		CRoleRecord* record=new CRoleRecord;
		record->DecodeFromDataBlock(setReadDB);
		if (!AddOneRecord(record))
		{
			delete record;
		}
	}

	LONG lExcRecordNun= setReadDB.GetLongFromByteArray();
	for (int a=0; a< lExcRecordNun; a++)
	{
		sExcessRecord record;
		WORD index= setReadDB.GetShortFromByteArray();
		setReadDB.GetBufferFromByteArray(&record,sizeof(sExcessRecord));
		m_mapExcessRecord[index]= record;
	}

	LONG rolerecordnum=setReadDB.GetLongFromByteArray();

	for (int a=0; a<rolerecordnum; a++)
	{
		LONG _long=setReadDB.GetLongFromByteArray();
		m_setRoleRecord.insert(_long);
	}
	DeleteExcessRecord();
	return TRUE;
}

BOOL CRoleQueue::AddFinishedRoleRecord(LONG index)
{
	set<LONG>::iterator it=m_setRoleRecord.find(index);
	if(it== m_setRoleRecord.end())
	{
		m_setRoleRecord.insert(index);
		return TRUE;
	}
	return FALSE;
	//map<LONG,BOOL>=m_mapRoleRecord.find(index);
	//if (it== m_mapRoleRecord.end())
	//{
	//	m_mapRoleRecord[index]=TRUE;
	//	return TRUE;
	//}
	//return FALSE;
}
//ɾ�������¼
BOOL CRoleQueue::DelFinishedRoleRecord(LONG index)
{
	set<LONG>::iterator it=m_setRoleRecord.find(index);
	if(it!= m_setRoleRecord.end())
	{
		m_setRoleRecord.erase(index);
		return TRUE;
	}
	return FALSE;
}

BOOL CRoleQueue::GetRoleFinishedRecord(LONG index)
{
	set<LONG>::iterator it=m_setRoleRecord.find(index);
	if (it!=m_setRoleRecord.end())
	{
		return TRUE;
	}
	return FALSE;
}

sExcessRecord* CRoleQueue::FindsExcessRecord(WORD index)
{
	map<WORD,sExcessRecord>::iterator it= m_mapExcessRecord.find(index);
	if (it== m_mapExcessRecord.end())
	{
		return NULL;
	}
	return &((*it).second);
}

WORD	CRoleQueue::CreatExcessRecord(LONG regionID,LONG regionX,LONG regionY)
{
	WORD index;
	map<WORD,sExcessRecord>::iterator it;
	do 
	{
		index=rand()%(0xffff);
	} while(index==0 || m_mapExcessRecord.find(index)!= m_mapExcessRecord.end());
	sExcessRecord& ExcRecord= m_mapExcessRecord[index];
	//(*it).second.m_lRegionID= regionID;
	//(*it).second.m_lRegionX = regionX;
	//(*it).second.m_lRegionY = regionY;
	ExcRecord.m_lRegionID = regionID;
	ExcRecord.m_lRegionX = regionX;
	ExcRecord.m_lRegionY = regionY;
	return index;
}

BOOL	CRoleQueue::DelExcessRecord(WORD index)
{
	m_mapExcessRecord.erase(index);
	return TRUE;
}


//��ʼ�������¼�б�
//VOID CRoleQueue::InitRoleRecord()
//{
//
//}

//ٸ������ϵͳ��
//����

CMercenaryQuestSys::CMercenaryQuestSys()
:m_lMasterQuestID(0)
{
	//����ٸ�������б�
	m_MerQuests.clear();
	//��ǰ���ܵ�ٸ������
	m_lCurQuestID = 0;
	//�ϴ�ˢ��ٸ�������ʱ��
	//m_dwLastUpdateTime=0;
	m_lLastQuestID = 0;
	DisbandQuests.clear();
}

CMercenaryQuestSys::~CMercenaryQuestSys()
{
}


//ѹ����
bool CMercenaryQuestSys::AddDataByteArray(vector<BYTE>* pByteArray)
{
	_AddToByteArray(pByteArray,m_lCurQuestID);
	_AddToByteArray(pByteArray,m_lLastQuestID);
	_AddToByteArray(pByteArray,(WORD)m_MerQuests.size());
	itMerQ it = m_MerQuests.begin();
	for(;it != m_MerQuests.end();it++)
	{
		_AddToByteArray(pByteArray,(*it).first);
		tagMerQuest &MercQuests = (*it).second;
		_AddToByteArray(pByteArray,MercQuests.dwLastUpdateTime);
		_AddToByteArray(pByteArray,(WORD)MercQuests.Quests.size());
		vector<long>::iterator itQ = MercQuests.Quests.begin();
		for(;itQ != MercQuests.Quests.end();itQ++)
		{
			_AddToByteArray(pByteArray,(*itQ));
		}
	}
	set<long>::iterator itDisband=DisbandQuests.begin();
	_AddToByteArray( pByteArray,(WORD)DisbandQuests.size() );
	for (;itDisband != DisbandQuests.end();itDisband++)
	{
		_AddToByteArray( pByteArray,(*itDisband) );
	}
	return true;
}
//������
bool CMercenaryQuestSys::DecordDataFromByteArray(BYTE* pByte, long& pos)
{
	m_MerQuests.clear();
	DisbandQuests.clear();
	m_lCurQuestID = _GetLongFromByteArray(pByte,pos);
	m_lMasterQuestID = _GetLongFromByteArray(pByte,pos);
	WORD wSize = _GetWordFromByteArray(pByte,pos);
	tagMerQuest MerQuest;
	for(WORD i=0;i<wSize;i++)
	{
		long lID = _GetLongFromByteArray(pByte,pos);
		long time= _GetDwordFromByteArray(pByte,pos);
		long lSize =  _GetWordFromByteArray(pByte,pos);
		MerQuest.Quests.clear();
		MerQuest.dwLastUpdateTime=time;
		for(WORD j=0;j<lSize;j++)
		{
			long lID = _GetLongFromByteArray(pByte,pos);
			MerQuest.Quests.push_back(lID);
		}
		m_MerQuests[lID] = MerQuest;

	}
	wSize=_GetWordFromByteArray(pByte,pos);
	for ( WORD i=0;i<wSize;i++ )
	{
		long lId = _GetLongFromByteArray(pByte,pos);
		DisbandQuests.insert(lId);
	}
	return true;
}


bool CMercenaryQuestSys::CodeToDataBlock(DBWriteSet& setWriteDB)
{
	setWriteDB.AddToByteArray(m_lCurQuestID);
	setWriteDB.AddToByteArray((WORD)m_MerQuests.size());
	itMerQ it = m_MerQuests.begin();
	for(;it != m_MerQuests.end();it++)
	{
		setWriteDB.AddToByteArray((*it).first);
		tagMerQuest &MercQuests = (*it).second;
		setWriteDB.AddToByteArray(MercQuests.dwLastUpdateTime);
		setWriteDB.AddToByteArray((WORD)MercQuests.Quests.size());
		vector<long>::iterator itQ = MercQuests.Quests.begin();
		for(;itQ != MercQuests.Quests.end();itQ++)
		{
			setWriteDB.AddToByteArray((*itQ));
		}
	}
	set<long>::iterator itDisband=DisbandQuests.begin();
	setWriteDB.AddToByteArray( (WORD)DisbandQuests.size() );
	for (;itDisband != DisbandQuests.end();itDisband++)
	{
		setWriteDB.AddToByteArray( (*itDisband) );
	}
	return true;
}
bool CMercenaryQuestSys::DecodeFromDataBlock(DBReadSet& setReadDB)
{
	m_MerQuests.clear();
	DisbandQuests.clear();
	m_lCurQuestID = setReadDB.GetLongFromByteArray();

	WORD wSize = setReadDB.GetWordFromByteArray();
	tagMerQuest MerQuest;
	for(WORD i=0;i<wSize;i++)
	{
		long lID = setReadDB.GetLongFromByteArray();
		long time= setReadDB.GetDwordFromByteArray();
		long lSize =  setReadDB.GetWordFromByteArray();
		MerQuest.Quests.clear();
		MerQuest.dwLastUpdateTime=time;
		for(WORD j=0;j<lSize;j++)
		{
			long lID = setReadDB.GetLongFromByteArray();
			MerQuest.Quests.push_back(lID);
		}
		m_MerQuests[lID] = MerQuest;

	}
	wSize=setReadDB.GetWordFromByteArray();
	for ( WORD i=0;i<wSize;i++ )
	{
		long lId = setReadDB.GetLongFromByteArray();
		DisbandQuests.insert(lId);
	}
	return true;
}

//�ж����б����Ƿ���ڸ�����
long CMercenaryQuestSys::IsExistQuest(long lQuestID)
{
	itMerQ it = m_MerQuests.begin();
	for(;it != m_MerQuests.end();it++)
	{
		tagMerQuest &MercQuests = (*it).second;
		vector<long>::iterator itQ = find(MercQuests.Quests.begin(),MercQuests.Quests.end(),lQuestID);
		if(itQ != MercQuests.Quests.end())
			return it->first;
	}
	return 0;
}

//�ж����б����Ƿ���ڸ������б������
bool CMercenaryQuestSys::IsExistQuestList(long lQuestListID)
{
	itMerQ it = m_MerQuests.find(lQuestListID);
	if (it==m_MerQuests.end())
	{
		return false;
	}
	return true;
}
//�õ����б�ID��ٸ������
bool CMercenaryQuestSys::GetMercQuests(long lListID,CMercenaryQuestSys::tagMerQuest& MercQuests)
{
	itMerQ it = m_MerQuests.find(lListID);
	if(it != m_MerQuests.end())
	{
		MercQuests = (*it).second;
		return true;
	}
	return false;
}

void CMercenaryQuestSys::GetMercListQuests(set<long> &quests)
{
	quests.clear();
	if(0 != m_lCurQuestID)
		quests.insert(m_lCurQuestID);
	//itMerQ it = m_MerQuests.begin();
	//for(;it != m_MerQuests.end();it++)
	//{
	//	tagMerQuest &MercQuests = (*it).second;
	//	vector<long>::iterator itQ = MercQuests.Quests.begin();
	//	for(;itQ != MercQuests.Quests.end();itQ++)
	//	{
	//		quests.insert((*itQ));
	//	}
	//}
}

//�������ٸ����������
void CMercenaryQuestSys::Clear()
{
	m_MerQuests.clear();
}

//�õ�����ֵ
long CMercenaryQuestSys::GetData(long lQuestListID,long lPos)
{
	long lRet = 0;
	itMerQ it = m_MerQuests.find(lQuestListID);
	if(it != m_MerQuests.end())
	{
		tagMerQuest& MercQuests = (*it).second;
		if(lPos <= MercQuests.Quests.size())
		{
			return MercQuests.Quests[lPos-1];
		}
	}
	return lRet;
}
//���ò���ֵ 
void CMercenaryQuestSys::SetData(long lQuestListID,long lPos,long lValue)
{
	itMerQ it = m_MerQuests.find(lQuestListID);
	if(it != m_MerQuests.end())
	{
		tagMerQuest& MercQuests = (*it).second;
		if(lPos <= MercQuests.Quests.size())
		{
			MercQuests.Quests[lPos-1] = lValue;
		}
	}
}

//�����һ�������ʱ��
long CMercenaryQuestSys::OnAddQuest(long lQuestID)
{
	itMerQ it = m_MerQuests.begin();
	for(;it != m_MerQuests.end();it++)
	{
		tagMerQuest &MercQuests = (*it).second;
		vector<long>::iterator itQ = find(MercQuests.Quests.begin(),MercQuests.Quests.end(),lQuestID);
		if(itQ != MercQuests.Quests.end())
		{
			//MercQuests.Quests.erase(itQ);
			m_lCurQuestID = lQuestID;
			return (*it).first;
		}
	}
	return 0;
}
//��ɾ��һ�������ʱ��
bool CMercenaryQuestSys::OnDelQuest(long lQuestID)
{
	if(lQuestID == m_lCurQuestID)
	{
		m_lCurQuestID = 0;
		return true;
	}
	return false;
}

bool CMercenaryQuestSys::ClearTime(long MerListID)
{
	itMerQ it=m_MerQuests.find(MerListID);
	if (it!=m_MerQuests.end())
	{
		(*it).second.dwLastUpdateTime=0;
		return true;
	}
	return false;
}

void CMercenaryQuestSys::DisbandQuestRecord(long lQuestID,bool bFlag)
{
	if (!bFlag)
	{
		DisbandQuests.insert(lQuestID);
	}
	else
		DisbandQuests.erase(lQuestID);
}

long CMercenaryQuestSys::FindDisbandQuest(long lQuestID)
{
	set<long>::iterator it=DisbandQuests.find(lQuestID);
	if (it!=DisbandQuests.end())
	{
		return *it;
	}
	return 0;
}

void CMercenaryQuestSys::GetDisbandQuest(set<long> &quests)
{
	set<long>::iterator itQ = DisbandQuests.begin();
	for (;itQ!=DisbandQuests.end();itQ++)
	{
		quests.insert( (*itQ) );
	}
}

void CMercenaryQuestSys::SetMasterQuestID(long lID)
{
	/*if (lID == m_lMasterQuestID)
	{
		m_lMasterQuestID = 0;
	}
	else*/
		m_lMasterQuestID = lID;
}
#include "StdAfx.h"
#include ".\rsenemyfactions.h"
#include "..\..\server\dbserver\appworld\DBEnemyFaction.h"
#include "..\..\server\dbserver\AppWorld\DBentityManager.h"
#include "..\..\server\DBServer/dbserver/game.h"
#include "../../server/DBServer/AppWorld/DBAccessThread.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//CRsEnemyFactions::CRsEnemyFactions(void)
//{
//}
//
//CRsEnemyFactions::~CRsEnemyFactions(void)
//{
//}
//bool CRsEnemyFactions::LoadEnemyFaction(CDBEnemyFaction* ef)
//{
//	return true;
//}
//bool CRsEnemyFactions::SaveEnemyFaction(CDBEnemyFaction* ef)
//{
//	return true;
//}
////װ�����еĵж԰���ϵ
//bool	CRsEnemyFactions::LoadAllEnemyFactions()
//{
//	 GetGame()->GetDBEntityManager()->GetDBEnemyFactionMap().clear();
//	_ConnectionPtr cn;
//	_RecordsetPtr rs;
//	char sql[500] = "";
//	try
//	{
//		TESTHR(CreateCn(cn));
//		TESTHR(CreateRs(rs));
//
//		TESTHR(OpenCn(cn));
//		sprintf(sql,"SELECT * FROM CSL_FactionWar");
//		TESTHR(OpenRs(sql, rs, cn, adOpenForwardOnly, adLockOptimistic, adCmdTable));
//
//		long pos = 0;
//		while(!rs->GetadoEOF())
//		{
//			// δ���
//			CGUID guid;
//			CGUID::CreateGUID(guid);
//			CDBEnemyFaction* ef = new CDBEnemyFaction(guid);
//
//			ef->SetFactionID1( CGUID(((char*)(_bstr_t)rs->GetCollect("FactionID1"))) );
//			ef->SetFactionID2( CGUID(((char*)(_bstr_t)rs->GetCollect("FactionID2"))) );
//			ef->SetDisandTime((long) rs->GetCollect("LeaveTime"));
//
//			GetGame()->GetDBEntityManager()->AddOneDBEnemyFaction(ef);
//			rs->MoveNext();
//			//pos++;
//		}
//	}
//	catch (_com_error &e)
//	{
//		ReleaseRs(rs);
//		//ReleaseCn(cn);
//		PrintErr("Load EnemyFactionList error", e);
//		return false;
//	}
//	ReleaseRs(rs);
//	//ReleaseCn(cn);
//	return true;
//}
//
////�������еĵж԰���ϵ
//bool CRsEnemyFactions::SaveAllEnemyFactions(std::map<CGUID, CDBEnemyFaction*>& EnemyFactions,_ConnectionPtr& cn)
//{
//
//	bool IsLocalConn = false;
///*	if(cn == NULL)
//	{
//		TESTHR(CreateCn(cn));
//		TESTHR(OpenCn(cn));
//		IsLocalConn = true;
//	}*/
//
//	char sql[500] = "DELETE FROM CSL_FactionWar";
//	// ��ɾ���ð��ĳ�Ա
//	if(!ExecuteCn(sql, GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr()))
//	{
//		PrintErr("delete CSL_FactionWar error");
//		return false;
//	}
//	_RecordsetPtr rs;
//
//	try
//	{
//		CDBEntityManager::DBEnemyFactionMapItr it = EnemyFactions.begin();
//		char szID1[128];
//		char szID2[128];
//		(*it).second->GetFactionID1().tostring(szID1);
//		(*it).second->GetFactionID2().tostring(szID2);
//		for(;it != EnemyFactions.end();it++)
//		{
//			sprintf(sql,"INSERT INTO CSL_FactionWar VALUES('%s','%s',%d)",
//				szID1,szID2,(*it).second->GetDisandTime());
//			if(!ExecuteCn(sql, cn))
//			{
//				PrintErr("�洢�ж԰���б�ʧ��");
//				ReleaseRs(rs);
//				//ReleaseCn(cn);
//				return false;
//			}
//		}
//	}
//	catch (_com_error &e)
//	{
//		ReleaseRs(rs);
//	//	if(IsLocalConn)
//			//ReleaseCn(cn);
//		PrintErr("Save EnemyFactionList error", e);
//		return false;
//	}
//	ReleaseRs(rs);
////	if(IsLocalConn)
//		//ReleaseCn(cn);
//	return true;
//}

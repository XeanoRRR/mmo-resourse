
#include "StdAfx.h"
#include "RsLinkman.h"

RsLinkmanManage g_RsLinkmanManage;

//!								�õ�Ψһʵ��
RsLinkmanManage& RsLinkmanManage::GetRsLinkmanManage(void)
{
	return g_RsLinkmanManage;
}


//!		�����ݿ��ж�ȡ��ʼ������
void	RsLinkmanManage::LoadPlayerInitData_FromDB(const CGUID &PlayerGuid, _ConnectionPtr &cn, vector<tagLinkman_DB>& vData)
{
	// �����̴߳����������Ӳ�����
	if(cn==NULL) 
	{
		PrintErr("�����̴߳����������Ӳ�����");
		return;
	}
	if (NULL_GUID == PlayerGuid)
	{
		return;
	}

	_RecordsetPtr rs;
	try
	{
		TESTHR(CreateRs(rs));

		char szGuid[64] = {0};
		PlayerGuid.tostring(szGuid);

		char szSql[512] = {0};
		sprintf(szSql,"SELECT * FROM FriendsMember WHERE OwnerGUID='%s' OR AimGUID='%s' ORDER BY OwnerGUID, AimGUID;", szGuid, szGuid);
		TESTHR(OpenRs(szSql, rs, cn));

		vData.clear();

		while(!rs->GetadoEOF())
		{
			tagLinkman_DB Linkman_DB;

			_variant_t vOwnerGUID	= rs->GetCollect("OwnerGUID");
			_variant_t vAimGUID		= rs->GetCollect("AimGUID");

			Linkman_DB.OwnerGUID	= CGUID(((char*)(_bstr_t)vOwnerGUID));
			Linkman_DB.AimGUID		= CGUID(((char*)(_bstr_t)vAimGUID));
			Linkman_DB.uGroupID		= rs->GetCollect("GroupID");
			Linkman_DB.uDelState	= rs->GetCollect("DelState");
			strcpy(Linkman_DB.szAimName, (_bstr_t) rs->GetCollect("AimName"));
			
			vData.push_back(Linkman_DB);

			// ������¼
			rs->MoveNext();
		}
		ReleaseRs(rs);

		return;
	}	
	catch(_com_error &e)
	{
		PrintErr("Load player linkman initData error", e);
		ReleaseRs(rs);

		return;
	}
}

//! ����һ����ϵ�˼�¼
BOOL RsLinkmanManage::SaveLinkman(tagLinkman_DB &Linkman_DB, _ConnectionPtr &cn)
{
	char szSql[512]			= {0};
	char szOwnerGuid[64]	= {0};
	char szAimGuid[64]		= {0};

	Linkman_DB.OwnerGUID.tostring(szOwnerGuid);
	Linkman_DB.AimGUID.tostring(szAimGuid);

	sprintf(szSql, "INSERT INTO \
				   FriendsMember(OwnerGUID, AimGUID, AimName, GroupID) \
				   VALUES('%s', '%s', N'%s', %d);",
				   szOwnerGuid, szAimGuid, Linkman_DB.szAimName, Linkman_DB.uGroupID
				   );
	
	if(!ExecuteCn(szSql, cn))
	{
		PrintErr("�洢��ϵ�˳���");
		return FALSE;
	}

#ifdef _DEBUG
	PrintErr("д����ϵ����Ϣ�ɹ���������ʱ�����ݱ�FriendsMember��������������������OwnerGUID + AimGUID + GroupID����ȷ��д�������Ӧȡ����������");
#endif
	return TRUE;
}

//! ɾ��һ����ϵ�˼�¼
BOOL RsLinkmanManage::DeleteLinkman(tagLinkman_DB &Linkman_DB, _ConnectionPtr &cn)
{
	char szSql[512] = {0};

	char szOwnerGuid[64]	= {0};
	char szAimGuid[64]		= {0};

	Linkman_DB.OwnerGUID.tostring(szOwnerGuid);
	Linkman_DB.AimGUID.tostring(szAimGuid);

	sprintf(szSql, "DELETE FriendsMember WHERE OwnerGUID = '%s' AND AimGUID = '%s' AND GroupID = %d;",
				   szOwnerGuid, szAimGuid, Linkman_DB.uGroupID);

	if(!ExecuteCn(szSql, cn))
	{
		PrintErr("ɾ����ϵ�˳���");
		return FALSE;
	}

	return TRUE;
}


#include "StdAfx.h"
#include "RsLeaveWord.h"

RsLeaveWord g_RsLeaveWord;

//! �õ�Ψһʵ��
RsLeaveWord& RsLeaveWord::GetRsLeaveWord(void)
{
	return g_RsLeaveWord;
}


//! ����[������Դ洢����]
BOOL RsLeaveWord::CreateInsertPro(tagLeaveWordSetup &LeaveWordSetup, _ConnectionPtr &cn)
{
	string strClearPro = 
		"SET ANSI_NULLS ON\r\n\
		SET QUOTED_IDENTIFIER ON\r\n\
		\r\n\
		IF EXISTS (\r\n\
		SELECT * \r\n\
		FROM INFORMATION_SCHEMA.ROUTINES\r\n\
		WHERE SPECIFIC_NAME = N'usp_InsertLeaveword'\r\n\
		)\r\n\
		DROP PROCEDURE usp_InsertLeaveword;\r\n";

	string strCreatePro_Part1 = 
	"CREATE PROCEDURE usp_InsertLeaveword\r\n\
		@Body			nvarchar(1024),\r\n\
		@LeaverType		int,\r\n\
		@SendTime		int,\r\n\
		@SenderName		nvarchar(32),\r\n\
		@SenderGuid		uniqueidentifier,\r\n\
		@PRI			int,\r\n\
		@ReceiverGuid	uniqueidentifier\r\n\
		AS\r\n\
		BEGIN\r\n\
		-- SET NOCOUNT ON added to prevent extra result sets from\r\n\
		-- interfering with SELECT statements.\r\n\
		SET NOCOUNT ON;\r\n\
																			\r\n\
	--�ѱ���ķ����߷��͸�������\r\n\
		declare @ToPlayerLeaveWordNum int;\r\n\
																			\r\n\
	SELECT @ToPlayerLeaveWordNum=count(*)\r\n\
		FROM PlayerLeaveWord\r\n\
		WHERE ReceiverGuid=@ReceiverGuid AND SenderGuid=@SenderGuid;\r\n\
																			\r\n\
	--���@ToPlayerLeaveWordNum���ֱ꣬�ӷ���\r\n\
		IF (";
		

	string strCreatePro_Part2 =
		"<= @ToPlayerLeaveWordNum)\r\n\
		BEGIN\r\n\
		SELECT -1;\r\n\
		RETURN;\r\n\
	END\r\n\
																			\r\n\
		--�ѱ���Ľ����ߵ�������\r\n\
		declare @SaveLeaveWordNum int;\r\n\
																			\r\n\
	SELECT @SaveLeaveWordNum=count(*)\r\n\
		FROM PlayerLeaveWord\r\n\
		WHERE ReceiverGuid=@ReceiverGuid;\r\n\
																			\r\n\
	IF(";

	string strCreatePro_Part3 = 
	" <= @SaveLeaveWordNum)\r\n\
		BEGIN\r\n\
		declare @MinPRI int;\r\n\
	--�ҳ��������������ȼ���Сֵ\r\n\
		SELECT @MinPRI=min(PRI) FROM PlayerLeaveWord WHERE ReceiverGuid=@ReceiverGuid;\r\n\
																									\r\n\
	IF(@PRI >= @MinPRI)\r\n\
		--��������Ե����ȼ����ڵ������ȼ���Сֵ\r\n\
		BEGIN\r\n\
		--ɾ�����ȼ���С��ʱ������ļ�¼1����¼\r\n\
		SET ROWCOUNT 1;\r\n\
	DELETE FROM PlayerLeaveWord WHERE SendTime=\r\n\
		(SELECT min(SendTime) FROM PlayerLeaveWord WHERE PRI=@MinPRI AND ReceiverGuid=@ReceiverGuid);\r\n\
	SET ROWCOUNT 0;\r\n\
	END\r\n\
		ELSE\r\n\
		--����ͷ��ش���\r\n\
		BEGIN\r\n\
		SELECT -2;\r\n\
		RETURN;\r\n\
	END\r\n\
		END\r\n\
																									\r\n\
		--ִ�����ݲ���\r\n\
		INSERT INTO	PlayerLeaveWord\r\n\
		(Body, LeaverType, SendTime, SenderName, SenderGuid, PRI, ReceiverGuid)\r\n\
		VALUES\r\n\
		(@Body, @LeaverType, @SendTime, @SenderName, @SenderGuid, @PRI, @ReceiverGuid);\r\n\
																									\r\n\
	--���سɹ�\r\n\
		SELECT 1;\r\n\
	END\r\n";

	char szMaxToPlayerLeaveWordNum[16] = {0};
	sprintf(szMaxToPlayerLeaveWordNum, " %d ", LeaveWordSetup.lMaxToPlayerLeaveWordNum);

	char szMaxSaveLeaveWordNum[16] = {0};
	sprintf(szMaxSaveLeaveWordNum, " %d ", LeaveWordSetup.lMaxSaveLeaveWordNum);
	
	//! ����������
	string strCreatePro = strCreatePro_Part1 + szMaxToPlayerLeaveWordNum + strCreatePro_Part2 + szMaxSaveLeaveWordNum + strCreatePro_Part3;

	if(ExecuteCn(strClearPro.c_str(), cn))
	{
		return ExecuteCn(strCreatePro.c_str(), cn);
	}
	return FALSE;
}

//! ɾ��[������Դ洢����]
BOOL RsLeaveWord::DeleteInsertPro(_ConnectionPtr &cn)
{
	string strClearPro = 
		"SET ANSI_NULLS ON\r\n\
		SET QUOTED_IDENTIFIER ON\r\n\
		\r\n\
		IF EXISTS (\r\n\
		SELECT * \r\n\
		FROM INFORMATION_SCHEMA.ROUTINES\r\n\
		WHERE SPECIFIC_NAME = N'usp_InsertLeaveword'\r\n\
		)\r\n\
		DROP PROCEDURE usp_InsertLeaveword;\r\n";

	return ExecuteCn(strClearPro.c_str(), cn);
}

//! ִ��һ��������������ز������ֵ
LONG RsLeaveWord::InsertLeaveWord(tagLeaveWord_W2DB &LeaveWord_W2DB, const char *pWord, _ConnectionPtr &cn)
{
	// �����̴߳����������Ӳ�����
	if(cn==NULL) 
	{
		PrintErr("�����̴߳����������Ӳ�����");
		return 0;
	}
	
	_CommandPtr		m_pCommand;   
	_RecordsetPtr	rs;   
	_ParameterPtr	paraWord, paraSenderType, paraSendTime, paraSenderName, paraSenderGuid, paraPRI, paraReceiverGuid;   
	_variant_t		result1;   
	_bstr_t			strProc=L"usp_InsertLeaveword"; 

	try
	{
		m_pCommand.CreateInstance(__uuidof(Command));
		m_pCommand->ActiveConnection = cn;
		m_pCommand->CommandText = strProc;
		m_pCommand->CommandType = adCmdStoredProc;

		//! ���ò���
		paraWord = m_pCommand->CreateParameter(L"Body", adVarWChar, adParamInput, MAX_LEAVE_WORD_SIZE, pWord);
		m_pCommand->Parameters->Append(paraWord);

		paraSenderType = m_pCommand->CreateParameter(L"LeaverType", adInteger, adParamInput, sizeof(LONG), LeaveWord_W2DB.dwSenderType);
		m_pCommand->Parameters->Append(paraSenderType);

		paraSendTime = m_pCommand->CreateParameter(L"SendTime", adInteger, adParamInput, sizeof(LONG), time(NULL));
		m_pCommand->Parameters->Append(paraSendTime);

		paraSenderName = m_pCommand->CreateParameter(L"SenderName", adVarWChar, adParamInput, MAX_SENDER_NAME_SIZE, LeaveWord_W2DB.szSenderName);
		m_pCommand->Parameters->Append(paraSenderName); 

		char szSenderGuid[64] = {0};
		LeaveWord_W2DB.SenderGuid.tostring(szSenderGuid);
		paraSenderGuid = m_pCommand->CreateParameter(L"SenderGuid", adGUID, adParamInput, sizeof(CGUID), szSenderGuid);
		m_pCommand->Parameters->Append(paraSenderGuid); 

		paraPRI = m_pCommand->CreateParameter(L"PRI", adInteger, adParamInput, sizeof(LONG), LeaveWord_W2DB.uPRI);
		m_pCommand->Parameters->Append(paraPRI); 

		char szReceiverGuid[64] = {0};
		LeaveWord_W2DB.ReceiverGuid.tostring(szReceiverGuid);
		paraReceiverGuid = m_pCommand->CreateParameter(L"ReceiverGuid", adGUID, adParamInput, sizeof(CGUID), szReceiverGuid);
		m_pCommand->Parameters->Append(paraReceiverGuid); 
  
		//! ִ��
		rs = m_pCommand->Execute(NULL,NULL,adCmdStoredProc);   
		LONG lRe = rs->GetCollect((long)0); 

		rs->Close();
		ReleaseRs(rs);

		return lRe;

	}	
	catch(_com_error &e)
	{
		PrintErr("write player leave word error��", e);
		ReleaseRs(rs);

		return 0;
	}
}

//! ȡ��һ����ҵ����Ե�����
BOOL RsLeaveWord::PopLeaveWord_ToByteArray(const CGUID &PlayerGuid, vector<BYTE> &vData, _ConnectionPtr &cn)
{
	// �����̴߳����������Ӳ�����
	if(cn==NULL) 
	{
		PrintErr("�����̴߳����������Ӳ�����");
		return FALSE;
	}
	if (NULL_GUID == PlayerGuid)
	{
		return FALSE;
	}

	_RecordsetPtr rs;
	try
	{
		//! ��ȡ����
		TESTHR(CreateRs(rs));

		char szGuid[64] = {0};
		PlayerGuid.tostring(szGuid);

		char szSql[512] = {0};
		sprintf(szSql,"SELECT SenderName, SendTime, LeaverType, Body FROM PlayerLeaveWord WHERE ReceiverGuid='%s';", szGuid);
		TESTHR(OpenRs(szSql, rs, cn));

		vData.clear();
		_AddToByteArray(&vData, 0L);//! Ϊ����Ԥ��һ��LONG�ռ�
		LONG lNum = 0;
		
		while(!rs->GetadoEOF())
		{
			++lNum;

			tagLeaveWord LeaveWord;

			strcpy(LeaveWord.szName, (_bstr_t) rs->GetCollect("SenderName"));
			LeaveWord.dwTime = rs->GetCollect("SendTime");
			LeaveWord.uSenderType = rs->GetCollect("LeaverType");

			_AddToByteArray(&vData, &LeaveWord, sizeof(tagLeaveWord));
			
			_AddToByteArray(&vData, (char*)(_bstr_t)rs->GetCollect("Body"));

			// ������¼
			rs->MoveNext();
		}

		*((LONG*)(&vData[0])) = lNum; //! д��ʵ������

		ReleaseRs(rs);

		//! ɾ������
		sprintf(szSql,"DELETE PlayerLeaveWord WHERE ReceiverGuid='%s';", szGuid);
		return ExecuteCn(szSql, cn);
	}	
	catch(_com_error &e)
	{
		PrintErr("Load player leave word error", e);
		ReleaseRs(rs);

		return FALSE;
	}
}

//!							�����ʱ������
void RsLeaveWord::ClearLeaveWord(LONG lTime, _ConnectionPtr &cn)
{
	// �����̴߳����������Ӳ�����
	if(cn==NULL) 
	{
		PrintErr("�����̴߳����������Ӳ�����");
		return;
	}

	try
	{
		char szSql[512] = {0};
		sprintf(szSql,"DELETE PlayerLeaveWord WHERE SendTime<=%d;", lTime);
		ExecuteCn(szSql, cn);
	}	
	catch(_com_error &e)
	{
		PrintErr("Delete player leave word error", e);
	}
}


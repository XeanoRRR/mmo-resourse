#include "StdAfx.h"
#include ".\rsplayeraccount.h"
#include "..\Server\BillingServer\BillingServer\Game.h"

#import "C:\PROGRA~1\COMMON~1\System\ado\msado15.dll" rename( "EOF", "adoEOF" )

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRsPlayerAccount::CRsPlayerAccount(void)
{
}

CRsPlayerAccount::~CRsPlayerAccount(void)
{
}

/* Get User Account Info
[getUserPoint]
@UserID VARCHAR(8),
@Point  INT OUTPUT,
@Result INT OUTPUT				--	0 : Sucess  -1 : Not Found User 
*/
long CRsPlayerAccount::GetUserPoint( const char *szUserID, long &lPoint )
{
	::CoInitialize(NULL);
	char connstr[1024];
	sprintf( connstr, "Driver={SQL Server};Provider=%s;Server=%s;Database=%s;UID=%s;PWD=%s;",
		"SQLOLEDB",
		GetGame()->GetSetup()->strSqlServerIP.c_str(),
		GetGame()->GetSetup()->strDBName.c_str(),
		GetGame()->GetSetup()->strSqlUserName.c_str(),
		GetGame()->GetSetup()->strSqlPassWord.c_str() );

	ADODB::_ConnectionPtr conn;
	ADODB::_CommandPtr cmd;
	long lRes;

	try 
	{ 
		conn.CreateInstance( __uuidof( ADODB::Connection ) );
		cmd.CreateInstance( __uuidof( ADODB::Command ) );

		conn->Open( connstr, "", "", ADODB::adConnectUnspecified );

		// Open recordset. 
		cmd->ActiveConnection = conn;
		cmd->CommandText = "GetUserPoint";
		cmd->CommandType = ADODB::adCmdStoredProc;


		cmd->Parameters->Append( cmd->CreateParameter( "@UserID", ADODB::adVarChar,ADODB::adParamInput, 32, szUserID ) );

		cmd->Parameters->Append(cmd->CreateParameter( "@Point", ADODB::adInteger, ADODB::adParamOutput, sizeof(long) ) );
		cmd->Parameters->Append(cmd->CreateParameter( "@Result", ADODB::adInteger, ADODB::adParamOutput, sizeof(long) ) );

		cmd->Execute(0,0,ADODB::adCmdStoredProc);

		// return  :  --	0 : Sucess  -1 : Not Found User
		lRes = cmd->Parameters->GetItem("@Result")->GetValue();
		lPoint = cmd->Parameters->GetItem("@Point")->GetValue();

		conn->Close();

	} 
	catch(_com_error &e)
	{
		AddLogText("err: %s", (char*)e.Description());
		lRes = -2;
	}

	cmd.Release();
	conn.Release();
	::CoUninitialize();

	return lRes;
}

/* User Buy
[buyItemCode]
@UserID VARCHAR(12),			--  �[��̖
@ItemIndex INT,
@Qty INT,
@Amount INT,					--  Ҫ�۵��c��
@IP VARCHAR(20),

@World VARCHAR(50),				--  �ŷ���
@Character_Name VARCHAR(50),	--  ُ�I�߽�ɫ���Q

@Result INT OUTPUT,	--	0���ɹ�	-1��Not Found User	-2���c������	-3��ȡ��TranCode�e�`	-4���Y��̎���e�`
@TranCode VARCHAR(500) OUTPUT,	--  ������̖
@LastPoint INT OUTPUT			--  ���ʣ�N�c��
*/
long CRsPlayerAccount::BuyItemCode( const string &strUserID, long lItemIndex, long lItemNum, long lAmount, const string &strServer,
		const string &strUserIP, const string &strCharacter, char *szTranCode, long &lLastPoint )
{
	char connstr[1024];
	sprintf( connstr, "Driver={SQL Server};Provider=%s;Server=%s;Database=%s;UID=%s;PWD=%s;",
		"SQLOLEDB",
		GetGame()->GetSetup()->strSqlServerIP.c_str(),
		GetGame()->GetSetup()->strDBName.c_str(),
		GetGame()->GetSetup()->strSqlUserName.c_str(),
		GetGame()->GetSetup()->strSqlPassWord.c_str() );

	ADODB::_ConnectionPtr conn;
	ADODB::_CommandPtr cmd;
	long lRes;

	try 
	{ 
		conn.CreateInstance( __uuidof( ADODB::Connection ) );
		cmd.CreateInstance( __uuidof( ADODB::Command ) );

		conn->Open( connstr, "", "", ADODB::adConnectUnspecified );

		// Open recordset. 
		cmd->ActiveConnection = conn;
		cmd->CommandText = "buyItemCode";
		cmd->CommandType = ADODB::adCmdStoredProc;

		cmd->Parameters->Append( cmd->CreateParameter( "@UserID", ADODB::adVarChar, ADODB::adParamInput, 32, strUserID.c_str() ) );
		cmd->Parameters->Append( cmd->CreateParameter( "@ItemIndex", ADODB::adInteger, ADODB::adParamInput, sizeof(long), lItemIndex ) );
		cmd->Parameters->Append( cmd->CreateParameter( "@Qty", ADODB::adInteger, ADODB::adParamInput, sizeof(long), lItemNum ) );
		cmd->Parameters->Append( cmd->CreateParameter( "@Amount", ADODB::adInteger, ADODB::adParamInput, sizeof(long), lAmount ) );
		cmd->Parameters->Append( cmd->CreateParameter( "@IP", ADODB::adVarChar, ADODB::adParamInput, 24, strUserIP.c_str() ) );
		cmd->Parameters->Append( cmd->CreateParameter( "@World", ADODB::adVarChar, ADODB::adParamInput, 32, strServer.c_str() ) );
		cmd->Parameters->Append( cmd->CreateParameter( "@Character_Name", ADODB::adVarChar, ADODB::adParamInput, 32, strCharacter.c_str() ) );

		cmd->Parameters->Append(cmd->CreateParameter( "@Result", ADODB::adInteger, ADODB::adParamOutput, sizeof(long) ) );
		cmd->Parameters->Append(cmd->CreateParameter( "@TranCode", ADODB::adVarChar, ADODB::adParamOutput, 500 ) );
		cmd->Parameters->Append(cmd->CreateParameter( "@LastPoint", ADODB::adInteger, ADODB::adParamOutput, sizeof(long) ) );

		cmd->Execute(0,0,ADODB::adCmdStoredProc);

		// return  :  --	0���ɹ�	-1��Not Found User	-2���c������	-3��ȡ��TranCode�e�`	-4���Y��̎���e�`
		lRes = cmd->Parameters->GetItem("@Result")->GetValue();
		strncpy( szTranCode, _bstr_t(cmd->Parameters->GetItem("@TranCode")->GetValue()), 512);
		lLastPoint = cmd->Parameters->GetItem("@LastPoint")->GetValue();

		conn->Close();

	}
	catch(_com_error &e)
	{
		AddLogText("err: %s", (char*)e.Description());
		lRes = -5;
	}
	catch(...)
	{
		AddLogText("err: DB Operation Fatal Error:BuyItemCode");
	}

	cmd.Release();
	conn.Release();
	return lRes;
}

/*
buyPlayerItem
�������Q	�͑B	�f��
@UIDfrom	varchar(20)	�u���[��̖
@Ipfrom	varchar(20)	�u�ҁ�ԴIP
@Cnamefrom	varchar(50)	�u�ҽ�ɫ���Q
@UIDto	varchar(20)	�I���[��̖
@Ipto	varchar(20)	�I�ҁ�ԴIP
@Cnameto	varchar(50)	�I�ҽ�ɫ���Q
@WorldId	int	���׵� World ID
@ItemIdx	int	Item Idx
@ItemNum	int	Item Num
@Qty	int	���ה���
@Amount	int	���ۿ����~

�������Q	�͑B	�f��
@Result	int	0���ɹ�
-1����o�u���[��̖
-2����o�I���[��̖
-3���I���c������
-4��ȡ��TranCode�e�`
-5���Y��̎���e�`
@TranCode	varchar(500)	������̖
@UIDfromLastPoint	int	�u��ʣ�N�c��
@UIDtoLastPoint	int	�I��ʣ�N�c��
*/
long CRsPlayerAccount::BuyPlayerItem( const string &strBuyerID, const string &strBuyerIP, const string &strBuyerName,
		const string &strSellerID, const string &strSellerIP, const string &strSellerName, long lGSID,
		long lItemIndex, long lItemNum, long lAmount, char *szTranCode, long &lBuyerPoint, long &lSellerPoint )
{
	char connstr[1024];
	sprintf( connstr, "Driver={SQL Server};Provider=%s;Server=%s;Database=%s;UID=%s;PWD=%s;",
		"SQLOLEDB",
		GetGame()->GetSetup()->strSqlServerIP.c_str(),
		GetGame()->GetSetup()->strDBName.c_str(),
		GetGame()->GetSetup()->strSqlUserName.c_str(),
		GetGame()->GetSetup()->strSqlPassWord.c_str() );

	ADODB::_ConnectionPtr conn;
	ADODB::_CommandPtr cmd;
	long lRes;

	try 
	{ 
		conn.CreateInstance( __uuidof( ADODB::Connection ) );
		cmd.CreateInstance( __uuidof( ADODB::Command ) );

		conn->Open( connstr, "", "", ADODB::adConnectUnspecified );

		// Open recordset. 
		cmd->ActiveConnection = conn;
		cmd->CommandText = "buyPlayerItem";
		cmd->CommandType = ADODB::adCmdStoredProc;

		cmd->Parameters->Append( cmd->CreateParameter( "@UIDfrom", ADODB::adVarChar, ADODB::adParamInput, 20, strSellerID.c_str() ) );
		cmd->Parameters->Append( cmd->CreateParameter( "@Ipfrom", ADODB::adVarChar, ADODB::adParamInput, 20, strSellerIP.c_str() ) );
		cmd->Parameters->Append( cmd->CreateParameter( "@Cnamefrom", ADODB::adVarChar, ADODB::adParamInput, 50, strSellerName.c_str() ) );
		cmd->Parameters->Append( cmd->CreateParameter( "@UIDto", ADODB::adVarChar, ADODB::adParamInput, 20, strBuyerID.c_str() ) );
		cmd->Parameters->Append( cmd->CreateParameter( "@Ipto", ADODB::adVarChar, ADODB::adParamInput, 20, strBuyerIP.c_str() ) );
		cmd->Parameters->Append( cmd->CreateParameter( "@Cnameto", ADODB::adVarChar, ADODB::adParamInput, 50, strBuyerName.c_str() ) );
		cmd->Parameters->Append( cmd->CreateParameter( "@WorldId", ADODB::adInteger, ADODB::adParamInput, sizeof(long), lGSID ) );
		cmd->Parameters->Append( cmd->CreateParameter( "@ItemIdx", ADODB::adInteger, ADODB::adParamInput, sizeof(long), lItemIndex ) );
		cmd->Parameters->Append( cmd->CreateParameter( "@ItemNum", ADODB::adInteger, ADODB::adParamInput, sizeof(long), lItemNum ) );
		cmd->Parameters->Append( cmd->CreateParameter( "@Amount", ADODB::adInteger, ADODB::adParamInput, sizeof(long), lAmount ) );

		cmd->Parameters->Append(cmd->CreateParameter( "@Result", ADODB::adInteger, ADODB::adParamOutput, sizeof(long) ) );
		cmd->Parameters->Append(cmd->CreateParameter( "@TranCode", ADODB::adVarChar, ADODB::adParamOutput, 500 ) );
		cmd->Parameters->Append(cmd->CreateParameter( "@UIDfromLastPoint", ADODB::adInteger, ADODB::adParamOutput, sizeof(long) ) );
		cmd->Parameters->Append(cmd->CreateParameter( "@UIDtoLastPoint", ADODB::adInteger, ADODB::adParamOutput, sizeof(long) ) );

		cmd->Execute(0,0,ADODB::adCmdStoredProc);

		/*�������Q	�͑B	�f��
		@Result	int	0���ɹ�
		-1����o�u���[��̖
		-2����o�I���[��̖
		-3���I���c������
		-4��ȡ��TranCode�e�`
		-5���Y��̎���e�`
		@TranCode	varchar(500)	������̖
		@UIDfromLastPoint	int	�u��ʣ�N�c��
		@UIDtoLastPoint	int	�I��ʣ�N�c��
		*/
		lRes = cmd->Parameters->GetItem("@Result")->GetValue();
		strncpy( szTranCode, _bstr_t(cmd->Parameters->GetItem("@TranCode")->GetValue()), 512);
		lSellerPoint = cmd->Parameters->GetItem("@UIDfromLastPoint")->GetValue();
		lBuyerPoint = cmd->Parameters->GetItem("@UIDtoLastPoint")->GetValue();

		conn->Close();

	}
	catch(_com_error &e)
	{
		AddLogText("err: %s", (char*)e.Description());
		lRes = -5;
	}
	catch(...)
	{
		AddLogText("err: DB Operation Fatal Error:BuyPlayerItem");
	}

	cmd.Release();
	conn.Release();
	return lRes;
}
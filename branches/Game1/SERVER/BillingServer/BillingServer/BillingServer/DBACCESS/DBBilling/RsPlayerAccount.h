#pragma once
//#include "..\myadobase.h"

class CRsPlayerAccount
{
public:
	CRsPlayerAccount(void);
	~CRsPlayerAccount(void);

public:
	
	/* Get User Account Info
	[getUserPoint]
	@UserID VARCHAR(8),
	@Point  INT OUTPUT,
	@Result INT OUTPUT				--	0 : Sucess  -1 : Not Found User 
	*/
	static long GetUserPoint( const char *szUserID, long &lPoint );

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
	static long BuyItemCode( const string &strUserID, long lItemIndex, long lItemNum, long lAmount, const string &strServer,
		const string &strUserIP, const string &strCharacter, char *szTranCode, long &lLastPoint );

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
	static long BuyPlayerItem( const string &strBuyerID, const string &strBuyerIP, const string &strBuyerName,
		const string &strSellerID, const string &strSellerIP, const string &strSellerName, long lGSID,
		long lItemIndex, long lItemNum, long lAmount, char *szTranCode, long &lBuyerPoint, long &lSellerPoint );
};
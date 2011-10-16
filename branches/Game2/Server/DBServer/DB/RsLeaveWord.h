/*
*	file:		RsLeaveWord.h
*	Brief:		����DBϵͳ
*	detail:		
*				
*				
*	Authtor:	�ſ���
*	Datae:		2008-03-07
*/
#pragma  once


#include "..\public\GUID.h"
#include "..\nets\netdb\message.h"
#include "../myadobase.h"
#include "../public/LeaveWordDef.h"


class RsLeaveWord
	: public CMyAdoBase
{
public:
	//!							�õ�Ψһʵ��
	static		RsLeaveWord&	GetRsLeaveWord(void);

public:
	//!							����[������Դ洢����]
	BOOL						CreateInsertPro(tagLeaveWordSetup &LeaveWordSetup, _ConnectionPtr &cn);
	//!							ɾ��[������Դ洢����]
	BOOL						DeleteInsertPro(_ConnectionPtr &cn);
	//!							ִ��һ��������������ز������ֵ
	LONG						InsertLeaveWord(tagLeaveWord_W2DB &LeaveWord_W2DB, const char *pWord, _ConnectionPtr &cn);
	//!							ȡ��һ����ҵ����Ե�����
	BOOL						PopLeaveWord_ToByteArray(const CGUID &PlayerGuid, vector<BYTE> &vData, _ConnectionPtr &cn);
	//!							�����ʱ������
	void						ClearLeaveWord(LONG lTime, _ConnectionPtr &cn);

private:

};

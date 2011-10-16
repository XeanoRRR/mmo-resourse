#ifndef _MSG_DESCRIBUTE
#include <map>
#include "BaseMessage.h"

class CMsgDescribe
{
private:
	map<DWORD,string>								m_mapMsg;

	CMsgDescribe									();
	~CMsgDescribe									();

public:
	//##����ȫ��ָ��
	static CMsgDescribe*	g_pMsgDesc;

	static CMsgDescribe*	GetInstance				();
	static VOID				FinalRelease			();

	const CHAR*				GetDesc					( DWORD dwID );
};

#endif
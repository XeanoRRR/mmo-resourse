#pragma once
#include "..\..\frostengine\ui.h"


enum eSECURITY
{
	SECURITY_FREE = 0,		//�ǰ�ȫ��
	SECURTIY_FIGHT,			//������
	SECURTIY_SAFE,			//��ȫ��
	SECURTIY_END
};

class ListBoxSecurity : public ui::ListBox
{
public:
	virtual BOOL Create(const char *pszName,const RECT *pRect,DWORD dwStyle,Wnd *pParent);

	ListBoxSecurity(void);
	~ListBoxSecurity(void);
};

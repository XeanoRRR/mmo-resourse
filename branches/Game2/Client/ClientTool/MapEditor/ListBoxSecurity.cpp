
#include "stdafx.h"

#include "ListBoxSecurity.h"

//��ȫ������
const char * pSecurityName[] = 
{
	"�ǰ�ȫ��",
	"������",
	"��ȫ��"
};

ListBoxSecurity::ListBoxSecurity(void)
{
}

ListBoxSecurity::~ListBoxSecurity(void)
{
}

BOOL ListBoxSecurity::Create(const char *pszName,const RECT *pRect,DWORD dwStyle,Wnd *pParent)
{
	ui::ListBox::Create(pszName,pRect,dwStyle,pParent);

	for(int i = 0;i < SECURTIY_END; i++)
	{
		this->InsertItem(pSecurityName[i]);
	}

	SetSelItem(0);

	return TRUE;
}


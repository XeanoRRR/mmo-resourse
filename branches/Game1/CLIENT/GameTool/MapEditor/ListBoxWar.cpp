
#include "stdafx.h"

#include "ListBoxWar.h"

//��ȫ������
const char * pWarName[] = 
{
	"�ǳ�ս��",
	"��ս��",
};

ListBoxWar::ListBoxWar(void)
{
}

ListBoxWar::~ListBoxWar(void)
{
}

BOOL ListBoxWar::Create(const char *pszName,const RECT *pRect,DWORD dwStyle,Wnd *pParent)
{
	ui::ListBox::Create(pszName,pRect,dwStyle,pParent);

	for(int i = 0;i < WAR_END; i++)
	{
		this->InsertItem(pWarName[i]);
	}

	SetSelItem(0);

	return TRUE;
}


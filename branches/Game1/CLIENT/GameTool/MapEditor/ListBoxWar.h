#pragma once
#include "..\..\frostengine\ui.h"


enum eWAR
{
	WAR_NONE = 0,		//�ǳ�ս��
	WAR_FIGHT,			//��ս
	WAR_END
};

class ListBoxWar : public ui::ListBox
{
public:
	virtual BOOL Create(const char *pszName,const RECT *pRect,DWORD dwStyle,Wnd *pParent);

	ListBoxWar(void);
	~ListBoxWar(void);
};

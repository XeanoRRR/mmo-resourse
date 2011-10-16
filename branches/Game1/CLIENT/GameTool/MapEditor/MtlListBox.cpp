#include "StdAfx.h"

#include "..\..\frostengine\console.h"
#include "..\..\frostengine\utility.h"

#include "mtllistbox.h"

const char * pMaterialName[] = 
{"ȱʡ",
"�ݵ�",
"���",
"ɳ��",
"ľ��",
"ʯ��",
"ˮ��",
"ѩ��",
"����",
};


MtlListBox::MtlListBox(void)
{
}

MtlListBox::~MtlListBox(void)
{
}

//CELL_MATERIAL MtlListBox::GetSel()
//{
//	return CM_NULL;
//}

//void MtlListBox::Render(void)
//{
//	DWORD &dwStyle = m_stWindowInfo.dwStyle;
//	if (dwStyle & FWS_VISIBLE)
//	{
//		ui::Wnd::RenderDefault();
//
//		ui::Wnd::RenderChild();
//	}
//}

//void MtlListBox::Update(void)
//{
//	ui::Wnd::UpdateDefault();
//	DWORD &dwStyle = m_stWindowInfo.dwStyle;
//	if (dwStyle & FWS_VISIBLE)
//	{
//
//		ui::Wnd::UpdateChild();
//	}
//}


BOOL MtlListBox::Create(const char *pszName,const RECT *pRect,DWORD dwStyle,Wnd *pParent)
{
	ui::ListBox::Create(pszName,pRect,dwStyle,pParent);

	for(int i = 0;i < CM_END; i++)
	{
		this->InsertItem(pMaterialName[i]);
	}

	SetSelItem(0);

	return TRUE;
}


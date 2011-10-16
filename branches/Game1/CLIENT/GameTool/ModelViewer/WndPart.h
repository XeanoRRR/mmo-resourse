#pragma once
#include "..\..\frostengine\ui.h"

class WndViewer;

class WndPart : public ui::Wnd
{
	char		   m_szCurrentGroupName[50];	//��ǰ��ʾ��ģ��������
	int			   m_iCurrentSelModelIndex;		//��ǰѡ���ģ������
	eModelType	   m_eCurrentModelType;			//��ǰ��ʾģ�͵�����
	ui::ListBox  * m_pListBoxPart;

public:
	WndPart(void);
	~WndPart(void);

	BOOL Create(const char *pszName,const RECT *pRect,DWORD dwStyle,ui::Wnd *pParent);
	void Destroy();
	
	void BeginPopUp(void);
	void EndPopUp(void);

	virtual void Update();
	virtual void Render();

	const char  * GetCurrentGroupName()							{ return m_szCurrentGroupName; }
	eModelType	  GetCurrentModelType()							{ return m_eCurrentModelType; }
/*	DWORD		  GetActionCode(int iActionIndex);
	DWORD		  GetSelAction()								{ return m_pListBoxAction->GetSelItem();}
	DWORD		  GetSelActionCode();*/
	void		  SetCurrentGroupName(const char * pszName)		{ strcpy_s(m_szCurrentGroupName, 50, pszName); }
/*	void		  SetCurrentModelIndex(int iIndex)				{ m_iCurrentSelModelIndex = iIndex; }*/
	void		  SetCurrentModelType(eModelType eType)			{ m_eCurrentModelType = eType; }
/*	void		  SetActionItemCheck();*/

	void  AddPart(const char *szName);
	void  ClearPart();

/*	BOOL IsActionChange(void);
	void NextAction(void);
	void SelAction(DWORD dwName);*/
};
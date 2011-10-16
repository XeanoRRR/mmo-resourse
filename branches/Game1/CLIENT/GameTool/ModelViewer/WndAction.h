#pragma once
#include "..\..\frostengine\ui.h"

enum eActionType
{
	AT_ROLE,	//��ɫģ�͵Ķ���
	AT_BIND		//��ģ�͵Ķ���
};

class WndViewer;

class WndAction : public ui::Wnd
{
	char		   m_szCurrentModelName[50];	//��ǰ��ʾ��ģ������
	int			   m_iCurrentSelModelIndex;		//��ǰѡ���ģ������
	eActionType	   m_eCurrentActionType;		//��ǰ��ʾ����������
	ui::ListBox  * m_pListBoxAction;

public:
	WndAction(void);
	~WndAction(void);

	BOOL Create(const char *pszName,const RECT *pRect,DWORD dwStyle,ui::Wnd *pParent);
	void Destroy();
	
	void BeginPopUp(void);
	void EndPopUp(void);

	virtual void Update();
	virtual void Render();

	const char  * GetCurrentModelName()							{ return m_szCurrentModelName; }
	eActionType	  GetCurrentActionType()						{ return m_eCurrentActionType; }
	DWORD		  GetActionCode(int iActionIndex);
	DWORD		  GetSelAction()								{ return m_pListBoxAction->GetSelItem();}
	DWORD		  GetSelActionCode();
	void		  SetCurrentModelName(const char * pszName)		{ strcpy_s(m_szCurrentModelName, 50, pszName); }
	void		  SetCurrentModelIndex(int iIndex)				{ m_iCurrentSelModelIndex = iIndex; }
	void		  SetCurrentActionType(eActionType eType)		{ m_eCurrentActionType = eType; }
	void		  SetActionItemCheck();

	void  AddAction(const char *szName);
	void  ClearAction();

	BOOL IsActionChange(void);
	void NextAction(void);
	void SelAction(DWORD dwName);
};
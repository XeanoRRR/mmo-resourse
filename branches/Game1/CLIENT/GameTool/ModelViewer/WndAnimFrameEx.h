#pragma once

#include "..\..\frostengine\ui.h"

class WndAnimFrameEx : public ui::Wnd
{
	long m_lExportWidth;
	long m_lExportHeight;
	char m_szAnimFrameSaveDir[_MAX_PATH];			//����֡����Ŀ¼

	ui::Static  * m_pLabel1;
	ui::Static  * m_pLabel2;
	ui::Static  * m_pLabel3;
	ui::ListBox * m_pFPS;
	ui::ListBox * m_pFileFormat;
	ui::Button * m_pBtnStart;
	ui::Button * m_pBtnReturn;
	ui::Button * m_pBtnSetDir;
	ui::EditBox * m_pExportWidth;
	ui::EditBox * m_pExportHeight;
	ui::EditBox * m_pBoxDir;

public:
	WndAnimFrameEx(void);
	~WndAnimFrameEx(void);

	virtual void Update();
	virtual void Render();

	virtual BOOL Create(const char * pszName, const RECT * pRect, DWORD dwStyle, ui::Wnd * pParent);
	virtual void Destroy();
	void BeginPopUp(void);
	void EndPopUp(void);

	bool CheckInputSize();															//�������ĳߴ�
	BOOL IsStartExAnimFrame()		{ return m_pBtnStart->IsClicked(); }			//�Ƿ�ʼ������������֡
	long   GetExportWidth()			{ return m_lExportWidth; }						//�õ��������
	long   GetExportHeight()		{ return m_lExportHeight; }						//�õ������߶�
	DWORD  GetFPS();																//�õ�����֡ʱ����
	char * GetFileFormat();															//�õ�������ʽ
	char * GetAnimFrameSaveDir()	{ return m_szAnimFrameSaveDir; }				//���ض�������֡�ı���Ŀ¼
};
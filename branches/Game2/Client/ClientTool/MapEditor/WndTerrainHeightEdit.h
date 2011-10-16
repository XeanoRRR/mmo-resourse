//---------------------------------------------------------------------------------
// ����     : ����
// ����ʱ�� : 2004��1��8�� 14:36:55
// ����ʱ�� : 
// ժҪ     :
//---------------------------------------------------------------------------------
#pragma once
#include "..\..\engine\ui.h"

class RadioButton;

class WndTerrainHeightEdit : public ui::Wnd
{
	//---------------------------------------------------------------------------------
	// ��Ա����
	//---------------------------------------------------------------------------------
private:
	RadioButton *m_pRadioButton;
	INT       m_nCheckedIndex;
	//---------------------------------------------------------------------------------
	// ��Ա����
	//---------------------------------------------------------------------------------
public:
	WndTerrainHeightEdit(void);
	~WndTerrainHeightEdit(void);

	virtual BOOL Create(const char *pszName, const RECT *pRect, DWORD dwStyle, ui::Wnd *pParent);
	virtual void Destroy();

	virtual void Update();
	virtual void Render();

	// ����CheckBox ��������
	VOID SetCheckBoxTextureHandle(DWORD dwIndex, DWORD dwTextureHandle, RECT rc);

	VOID SetBoxChecked(DWORD dwIndex, BOOL bCheckFlag = TRUE);

	DWORD GetCheckedIndex() { return m_nCheckedIndex; }

	// ����CheckBox ����ʾ״̬
	VOID ShowCheckBox(DWORD dwIndex, BOOL bShow = TRUE);

	// �������е�CheckBox
	VOID ResetCheckBox();
};

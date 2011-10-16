//---------------------------------------------------------------------------------
// ����     : ����
// ����ʱ�� : 2004��1��7�� 11:19:27
// ����ʱ�� : 
// ժҪ     :
//---------------------------------------------------------------------------------
#pragma once
#include "..\..\frostengine\ui.h"
class EditBox;
class RadioButton;


class WndTerrainTileSet : public ui::Wnd
{
	D3DCOLOR *m_pColor;
	RadioButton *m_pRadioButton;
	INT       m_nCheckedIndex;

public:
	WndTerrainTileSet(void);
	~WndTerrainTileSet(void);

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

	D3DCOLOR GetBrushColor() { return m_pColor[m_nCheckedIndex]; }
	D3DCOLOR GetBrushColor(DWORD dwIndex) { return m_pColor[dwIndex]; }
};

#pragma once
#include "..\..\engine\ui.h"

//�л���༭
//�б�
enum eCHANGE_POINT_TYPE
{
	CPT_NORMAL,		// ֱ���л�
	CPT_RANDOM,		// ����л�
	CPT_SCRIPT,		// �ű�����
};

// �л���
struct tagDoor
{
	char szName[64];

	struct tagSwitch
	{
		long			lState;							// �л�״̬
		long			lRegionID;						// ���ڵ�ID��
		long			lCoordX;						// ���ڵ�����
		long			lCoordY;
		long			lDir;							// ���ڵ㷽��
	}tSwitch;
};

class WndDoorEdit :
	public ui::Wnd
{
	

	std::vector<tagDoor *> m_vDoorList;

	ui::ListBox * m_pListBox;
	ui::ListBox * m_pListDetail;

	ui::Button * m_pBtnAdd;
	ui::Button * m_pBtnDel;
	ui::Button * m_pBtnModify;


public:
	BOOL Create(const char *pszName,const RECT *pRect,DWORD dwStyle,Wnd *pParent);
	void Destroy();

    virtual void Update();

	WndDoorEdit(void);
	~WndDoorEdit(void);

	void InsertDoor(tagDoor *pDoor);
	int GetCurDoorIndex();
	int GetDoorCount() { return (int)m_vDoorList.size();}
	tagDoor * GetDoor(DWORD dwIndex) ;

	void ClearAllDoor();

};

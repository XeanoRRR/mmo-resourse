#pragma once

#include <vector>
#include "..\..\gameengine\gamemodel.h"
#include "..\..\gameengine\displaymodel.h"
//�˴�����༭��
//���ݺͷ���
//������״̬
//1 ѡ��״̬
//		��ѡ��	ɾ��
//		��ѡ��  ���·��ã��ƶ���
//				��ת
//				����
//2 ����״̬

class GameBuildList;
class EditWnd;
class GameSelectRay;

class ToolBuildEdit
{
	BOOL m_bBoxSelectedMode;
	public:
	struct tagCopyData
	{
		int ox,oy;//ƫ��
		WORD  wBuildID;
		float fHeight;
		float fRotationY;
	};
private:

	tagCopyData * m_pClipboard;
	DWORD m_dwCopyDataCount;

	float m_fPalaceAngle;

	enum BUILDEDITSTATUETYPE {BUILD_EDIT,BUILD_PALACE,BUILD_MARKVISIBLE};
	BUILDEDITSTATUETYPE m_eEditStatus;

	enum BUILDEDITSTATUESUBTYPE { BESST_SELECT,BESST_MOVE};
	BUILDEDITSTATUESUBTYPE m_eEditSubStatus;

	enum BUILDPALACESUBTYPE  { BPST_PALACE,BPST_SUCK};
	BUILDPALACESUBTYPE   m_ePalaceSubStatus;

	//------------------------------------
	std::vector<int> m_vSelGridIndexArray;//���ӣ����ڽ����༭

	BOOL  m_bDragMouse;//��ѡ
	POINT m_ptDragStart,m_ptDragEnd;

	AnimInfo m_cCursorAnimInfo;

	//GameBuildList * m_pGameBuildList;

	DWORD m_dwCurHoverGridIndex;
	DWORD m_dwCurHoverBuildGridIndex;

	void  _ShowStatus(void);
	void  _DrawSelected(void);
	void  _CalculateHoverGrid(void);
	void  _DoRotation(void);
	void  _DoEdit(void);
	void  _DoChangeHeight(void);
	void  _AlignHeight(void);
	void  _CalculateHoverBuildGrid(GameSelectRay *pRay);
	float _CalculateDir(const D3DXVECTOR3 *pPosition,const GameSelectRay *pRay);

	void  Draw_BuildMarkVisibleSelect();

	
//	DWORD m_bSuckBuildID;
public:
	ToolBuildEdit(void);
	~ToolBuildEdit(void);

	void MessageProc(BOOL bActive);
	void Render();
	void Create();
	void Destroy(void);
	
	//GameBuildList *GetGameBuildList() const { return m_pGameBuildList;}
	BOOL FindSelectIndex(DWORD dwGridIndex);

	void _DoFixRotation();
	void Reset(void);

	void Render_BuildMarkVisible(void);
	void Proc_BuildMarkVisible(void);
	void ClearClipboard(void);
	DWORD GetHoverBuildGridIndex() const	{ return m_dwCurHoverBuildGridIndex;}
	int GetSelGridIndex(int iIndex)			{ return m_vSelGridIndexArray.size() ? m_vSelGridIndexArray[iIndex] : -1; }
};

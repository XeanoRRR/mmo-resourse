#pragma once

#include <vector>
#include "..\..\EngineExtend\displaymodel.h"


const DWORD dwDoorColor[8] =
{
	0x8000ffff,	    //��ͨ
	0x800040ff,		//���
	0xa01280ff,		//�ű�
};


class SelectFrustum;
class EditWnd;

enum DOOREDITSTATUETYPE 
{	
	DEST_SELECT,  	//ѡ��
	DEST_PALACE		//����
};

class ToolDoorEdit
{
	DOOREDITSTATUETYPE m_eEditStatus;

	std::vector<int> m_vSelGridIndexArray;

	BOOL m_bDrag;
	POINT m_ptDragStart,m_ptDragEnd;

	//���ģ��
	AnimInfo m_cAnimInfo;
	DWORD m_dwCurTime;
	CDisplayModel *m_pDoorModel;
	
public:
	ToolDoorEdit(void);
	~ToolDoorEdit(void);

	void MessageProc(BOOL bActive);
	void Render();
	void Create();
	void Destroy(void);
	void _ShowStatus(void);
	void DrawSelected(void);
	void RnderDoorModel(const D3DXMATRIX *pMat);

	CDisplayModel * GetDoorModel() const { return m_pDoorModel;}
};

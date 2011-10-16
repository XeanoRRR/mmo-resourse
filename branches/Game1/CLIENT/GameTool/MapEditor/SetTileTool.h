#pragma once
#include "TerrainMesh.h"

// �༭״̬
enum TILEEDITSTATUETYPE 
{
	TEST_IDLE = 0,  // ����
	TEST_PLACE		// ����
};

class ToolSetTile
{
	TILEEDITSTATUETYPE m_eEditStatus;
	TerrainMesh::_tagProcessData* m_pData;
	DWORD m_dwBrushSize;

public:
	ToolSetTile(void);
	~ToolSetTile(void);

	// ���ñ༭״̬
	VOID SetEditStatue(TILEEDITSTATUETYPE flag);

	// ��ʾ�༭״̬
	VOID ShowStatus();

	// �Ƚϲ�����������ͬʱ����ΪpData��
	BOOL CompareProcessData(TerrainMesh::_tagProcessData* pData);

	void MessageProc(BOOL bActive);
	void Render();
	void Create();
	void Destroy(void);
};

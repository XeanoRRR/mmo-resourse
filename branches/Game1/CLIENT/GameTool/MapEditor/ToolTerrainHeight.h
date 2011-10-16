//---------------------------------------------------------------------------------
// ����     : ����
// ����ʱ�� : 2004��1��8�� 11:19:52
// ����ʱ�� : 
// ժҪ     :
//---------------------------------------------------------------------------------
#pragma once
#include "TerrainMesh.h"

// �༭״̬
enum HEIGHTEDITSTATUETYPE 
{
	HEST_IDLE = 0,  // ����
	HEST_RAISE,		// ���
	HEST_LOWER,     // ѹ��
	HEST_PLATEAU,   // ƽ̨
	HEST_NOISE,     // ���
	HEST_SMOOTH     // ƽ��
};

class ToolTerrainHeight
{
	//---------------------------------------------------------------------------------
	// ��Ա����
	//---------------------------------------------------------------------------------
private:
	HEIGHTEDITSTATUETYPE m_eEditStatus;
	DWORD m_dwBrushSize;

	//---------------------------------------------------------------------------------
	// ��Ա����
	//---------------------------------------------------------------------------------
public:
	ToolTerrainHeight(void);
	~ToolTerrainHeight(void);

	VOID SetEditStatue(HEIGHTEDITSTATUETYPE flag);

	// ��ʾ�༭״̬
	VOID ShowStatus();

	void MessageProc(BOOL bActive);
	void Render();
	void Create();
	void Destroy(void);
};

//---------------------------------------------------------------------------------
// ����     : ����
// ����ʱ�� : 2004��1��9�� 10:29:35
// ����ʱ�� : 
// ժҪ     :
//---------------------------------------------------------------------------------
#pragma once
#include "cmd.h"
#include "TerrainMesh.h"
#include "ToolTerrainHeight.h"

class CmdMapTerrainRaiseHeight : public Cmd
{
	std::vector<TerrainMesh::_tagVertex> m_vOldVertexArray;

	TerrainMesh::_tagProcessData* m_ptProcessData; // ���β����Ĳ�������

	float				 m_fOffset;
	HEIGHTEDITSTATUETYPE m_eEditStatus;

public:
	CmdMapTerrainRaiseHeight(void);
	~CmdMapTerrainRaiseHeight(void);

	virtual bool Execute(void);
	virtual void UnExecute(void);
	virtual void Dump(void);

	// ��ȡ��������
	VOID GetProcessData(TerrainMesh::_tagProcessData* pData);
	// ���ô�������
	VOID SetProcessData(TerrainMesh::_tagProcessData* pData);
	// ���ñ༭״̬
	VOID SetEditStatusType(HEIGHTEDITSTATUETYPE editstatus) { m_eEditStatus = editstatus; }

	void SetOffset(float fOffset)		{ m_fOffset = fOffset; }
};
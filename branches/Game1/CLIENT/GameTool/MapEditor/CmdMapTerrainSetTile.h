#pragma once
#include "cmd.h"
#include "TerrainMesh.h"

class CmdMapTerrainSetTile :	public Cmd
{
	std::vector<TerrainMesh::_tagVertex> m_vOldVertexArray;
	TerrainMesh::_tagProcessData* m_ptProcessData; // ���β����Ĳ�������

public:
	CmdMapTerrainSetTile(void);
	~CmdMapTerrainSetTile(void);

	// ��ȡ��������
	VOID GetProcessData(TerrainMesh::_tagProcessData* pData);

	// ���ô�������
	VOID SetProcessData(TerrainMesh::_tagProcessData* pData);

	VOID Destroy();

	virtual void Execute(void);
	virtual void UnExecute(void);
	virtual void Dump(void);
};

#pragma once

#include <vector>
#include "cmd.h"

class CmdMapFillBlock :
	public Cmd
{
	
	//�����б�
	std::vector<int>   m_vGridIndexArray;
	std::vector<int>   m_vGridOldBlockArray;
	
	DWORD m_dwBlock;

public:
	CmdMapFillBlock(void);
	virtual ~CmdMapFillBlock(void);

	void AddGridIndex(DWORD dwGridIndex);
	void SetBlock(DWORD dwBlock) { m_dwBlock = dwBlock;}
	
	virtual void Execute(void);
	virtual void UnExecute(void);

	virtual void Dump(void);
};

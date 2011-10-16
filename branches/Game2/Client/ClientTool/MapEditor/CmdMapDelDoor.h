#pragma once

#include <vector>
#include "cmd.h"

class CmdMapDelDoor : public Cmd
{
	
	//�����б�
	std::vector<int>   m_vGridIndexArray;
	
	DWORD *m_pGridOldDoorIndexArray;
	
public:
	CmdMapDelDoor(void);
	virtual ~CmdMapDelDoor(void);

	void AddGridIndex(DWORD dwGridIndex);
	
	virtual bool Execute(void);
	virtual void UnExecute(void);

	virtual void Dump(void);
};

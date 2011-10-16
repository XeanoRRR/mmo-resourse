
#include <vector>
#include "Map.h"
#include "cmd.h"

class CmdMapFillBuild : public Cmd
{
	//�����б�
	DWORD m_dwGridIndexCount;
	DWORD * m_pOldGridIndexArray;//�ɸ�������
	Grid::tagBuild * m_pOldGridBuildArray;//����ɸ����н�������Ϣ
	
	Grid::tagBuild m_tBuild;

public:
	CmdMapFillBuild(void);
	virtual ~CmdMapFillBuild(void);

	void SetGridIndex(DWORD dwIndex,DWORD dwGridIndex);
	void SetGridIndexCount(DWORD dwCount);
	void SetBuild(const Grid::tagBuild &tBuild);  
	
	virtual bool Execute(void);
	virtual void UnExecute(void);

	virtual void Dump(void);
};

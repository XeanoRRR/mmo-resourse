//////////////////////////////////////////////////////////////////////////
//FactionWarSys.h
//Function:��սϵͳ��ع���
//Author:Joe
//Create Time:2005.6.27
//////////////////////////////////////////////////////////////////////////

#pragma once


class CFactionWarSys
{
public:
	CFactionWarSys(void);
	~CFactionWarSys(void);
private:
	static	CFactionWarSys* pInstance;
public:
	static	CFactionWarSys* GetInstance();
	void	Release();

	bool DigUpTheHatchet(LONG64 lNetSessionID,long lPassword,const CGUID& FactionID,long lLevl);	//�л���ս
	void CloseDecWarSession(LONG64 lNetSessionID,long lPassword);							//�ر���ս�Ự
};

CFactionWarSys* GetFactionWarSys();

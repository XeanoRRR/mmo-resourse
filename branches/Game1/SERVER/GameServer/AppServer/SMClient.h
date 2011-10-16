#pragma once

class CSMClient
{
public:
	CSMClient(void);
	~CSMClient(void);
	void SendAITick(bool b = false);
	void ExitOk();
	void InitOk();
	static void		ReleaseSM();
	bool LoadMapFileSetup(CDataBlockAllocator *pDBAllocator);
	void ProcessMessage();
	void ChangeWatchedPlayer(const char* name,BOOL bDel);
	void SendWatchResult(const char* name,BYTE byType,char* szInfo="");
	bool FindWatchedPlayer(const char* name);
	bool IsSendTo(BYTE byType);//�Ƿ�Ҫ�����˴���Ϣ
private:
	static CSMClient* g_SMInstance;
	bool m_bMapFile;
	std::set<string>WatchedSet;//����������б�
public:
	static CSMClient* GetSM();

};

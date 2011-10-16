#pragma once

class CSMClient
{
public:
	CSMClient(void);
	~CSMClient(void);
	void SendAITick(bool b = false);
	void ExitOk();
	void InitOk();   
	bool LoadMapFileSetup(CDataBlockAllocator *pDBAllocator);
	static void		ReleaseSM();
	void ProcessMessage();
private:
	static CSMClient* g_SMInstance;
	bool m_bMapFile;//�Ƿ��Ѵ��ڴ�ӳ���ļ�
public:
	static CSMClient* GetSM();

};

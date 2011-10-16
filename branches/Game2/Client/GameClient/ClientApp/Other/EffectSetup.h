#pragma once

// ��Ч����
class CEffectSetup
{
public:
	CEffectSetup(void);
	virtual ~CEffectSetup(void);
public:
	
	struct tagEffectSetup
	{
		DWORD dwEffectKey;		    // ���
		BOOL  bEnable;				// �Ƿ���Ч
		float fRadius;				// ��Χ�뾶
		DWORD dwColor;				// ��ɫ
		DWORD dwType;				// ����  0 + 1 x
		DWORD dwFadeStartTime;
		DWORD dwFadeLength;
	};
private:
	static map<DWORD, tagEffectSetup>		m_mapEffectSetup;			
	
public:
	static BOOL LoadEffectSetup(const char* filename);
	static map<DWORD,tagEffectSetup>* GetEffectSetup(){return &m_mapEffectSetup;}
};

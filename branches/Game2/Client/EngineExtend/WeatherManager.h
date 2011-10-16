//---------------------------------------------------------------------------------
// ����     : ����
// ����ʱ�� : 2004��2��9�� 10:13:29
// ����ʱ�� : 
// ժҪ     : ���������������Ƹ�������Ч���Ĵ�������Ⱦ����/�رյȡ�
//---------------------------------------------------------------------------------
#pragma once
#include "weathersnow.h"
#include "WeatherRain.h"
#include "weatherfog.h"
#include "WeatherFlutter.h"

class WeatherManager
{
	//---------------------------------------------------------------------------------
	// ��Ա����
private:
	WeatherSnow    * m_pSnow;
	WeatherRain    * m_pRain;
	WeatherFog	   * m_pFog;
	WeatherFlutter * m_pFlutter;

	BOOL m_bEnableSnow, m_bEnableRain, m_bEnableFog, m_bEnableFlutter;
	BOOL m_bSnowSndPlaying,m_bRainSndPlaying,m_bFogSndPlaying;
	int m_nSnowSndID,m_nRainSndID,m_nFogSndID;

	//---------------------------------------------------------------------------------
	// ��Ա����
public:
	WeatherManager(void);
	~WeatherManager(void);

	void Create();

	// �������ļ��д�������Ч��
	VOID ModifyFogParameter(TCHAR *config);
	VOID ModifyRainParameter(TCHAR *config);
	VOID ModifySnowParameter(TCHAR *config);
	VOID ModifyFlutterParameter(TCHAR *config);		//�޸ķ��������������

	VOID Destroy();

	VOID Render();
	VOID Update();

	// ��/�ر�Ч��
	VOID EnableFog(BOOL bFlag = TRUE);
	VOID EnableRain(BOOL bFlag = TRUE);
	VOID EnableSnow(BOOL bFlag = TRUE);
	VOID EnableFlutter(BOOL bFlag = TRUE);			//�Ƿ񼤻�����������
	
	BOOL IsRainEnable()			{ return m_bEnableRain; }
	BOOL IsSnowEnable()			{ return m_bEnableSnow; }	
	BOOL IsFogEnable()			{ return m_bEnableFog; }
	BOOL IsFlutterEnable()		{ return m_bEnableFlutter; }

	//VOID ChangeFogColor(DWORD dwColor, DWORD dwTime);

	VOID SetWorldMatrix(D3DXMATRIX *matWorld);
	VOID SetBillboard(D3DXMATRIX *matBillboard, D3DXMATRIX *matBillboardY);

	TCHAR* GetFogSoundFile()		{ return m_pFog->GetSoundFile(); }
	TCHAR* GetRainSoundFile()		{ return m_pRain->GetSoundFile(); }
	TCHAR* GetSnowSoundFile()		{ return m_pSnow->GetSoundFile(); }
	TCHAR* GetFlutterSoundFile()	{ return m_pFlutter->GetSoundFile(); }

	BOOL IsSnowSndPlaying()		{ return m_bSnowSndPlaying; }
	BOOL IsRainSndPlaying()		{ return m_bRainSndPlaying; }
	BOOL IsFogSndPlaying()		{ return m_bFogSndPlaying; }

	VOID SetSnowSndPlayingState(BOOL bVal)	{ m_bSnowSndPlaying=bVal; }
	VOID SetRainSndPlayingState(BOOL bVal)	{ m_bRainSndPlaying=bVal; }
	VOID SetFogSndPlayingState(BOOL bVal)	{ m_bFogSndPlaying=bVal; }

	VOID SetSnowSndID(int id)	{ m_nSnowSndID=id; }
	VOID SetRainSndID(int id)	{ m_nRainSndID=id; }
	VOID SetFogSndID(int id)	{ m_nFogSndID=id; }

	int GetSnowSndID()	{ return m_nSnowSndID; }
	int GetRainSndID()	{ return m_nRainSndID; }
	int GetFogSndID()	{ return m_nFogSndID; }
};
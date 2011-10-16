//---------------------------------------------------------------------------------
// ����     : ����
// ����ʱ�� : 2004��2��9�� 11:05:46
// ����ʱ�� : 
// ժҪ     : ����Ӳ��֧�ֵĻ�����Ĳ��������Դ򿪺͹ر���Ч�����趨��ɫ���䡣
//---------------------------------------------------------------------------------
#pragma once
#include <d3dx9.h>
#include "weather.h"

class WeatherEnvFog : public Weather
{
	//---------------------------------------------------------------------------------
	// ��Ա����
private:
	BOOL m_bEnableFog, m_bUpdateFog;
	D3DXCOLOR m_StartColor, m_EndColor, m_CurColor;    // ��ʼ��ɫ��������ɫ����ǰ��ɫ
	DWORD m_dwTime;                                    // ��ɫʱ��
	FLOAT m_fStart, m_fEnd, m_fDensity;                // ��ʼλ�ã�����λ�ã�Ũ��
	DWORD m_dwMode;                                    // ��ֵģʽ
	BOOL m_bModify;										// �Ƿ���Ĳ���

	//---------------------------------------------------------------------------------
	// ��Ա����
public:
	WeatherEnvFog(void);
	~WeatherEnvFog(void);

	VOID ModifyEnvFogParameter(TCHAR *config);

	VOID EnableFog(BOOL bFlag = TRUE);
	VOID SetupPixelFog(LPDIRECT3DDEVICE9 pd3dDevice);
	//VOID ChangeColor(D3DXCOLOR color, DWORD dwTime);
	void SetTime(DWORD dwTime);
	void SetColor(D3DXCOLOR color);
	void SetEnvFogDistance(FLOAT start, FLOAT end);
	D3DCOLOR GetCurrentColor();
	VOID Update();
};

//---------------------------------------------------------------------------------
// ����     : ����
// ����ʱ�� : 2004��2��2�� 16:46:04
// ����ʱ�� : 
// ժҪ     : ��point spriteʵ�ֵ�����ѩ��
//---------------------------------------------------------------------------------
#pragma once
#include "Weather.h"

class WeatherSnow : public Weather
{
	//---------------------------------------------------------------------------------
	// ��Ա����
private:
	VERTEX *m_ptSnow;
	BOOL *m_bLiving;

	FLOAT m_fSize;      // ��׼�ߴ�

	//---------------------------------------------------------------------------------
	// ��Ա����
public:
	WeatherSnow(void);
	virtual ~WeatherSnow(void);

	virtual BOOL CreateParticles();
	virtual VOID Destroy();

	virtual VOID Render();
	virtual BOOL Update(BOOL bEnable, D3DXVECTOR3 &vWindForce = D3DXVECTOR3(0, 0, 0));

	void EnableLiving(BOOL bEnable);

	virtual void ModifyParameters(TCHAR *config);

private:
	virtual VOID ReBirthParticle(VERTEX *pSnowParticle);
};

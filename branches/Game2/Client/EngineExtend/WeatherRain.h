//---------------------------------------------------------------------------------
// ����     : ����
// ����ʱ�� : 2004��2��3�� 13:40:29
// ����ʱ�� : 
// ժҪ     : ����Ƭģ����
//---------------------------------------------------------------------------------
#pragma once
#include "Weather.h"

class WeatherRain : public Weather
{
	//---------------------------------------------------------------------------------
	// ��Ա����
private:
	struct _tagQuadRain                       // ������ݽṹ
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 speed;
		FLOAT length;
		FLOAT width;

		BOOL living;							// ����Ƿ�����
	};
	_tagQuadRain *m_ptQuadRain;

	FLOAT m_fMinLength, m_fMaxLength;         // ��׼���ȷ�Χ
	FLOAT m_fWidth;                           // ��׼���

	//---------------------------------------------------------------------------------
	// ��Ա����
public:
	WeatherRain(void);
	virtual ~WeatherRain(void);

	virtual BOOL CreateParticles();
	virtual VOID Destroy();

	virtual VOID Render();
	virtual BOOL Update(BOOL bEnable);

	void EnableLiving(BOOL bEnable);

	virtual void ModifyParameters(TCHAR *config);

private:
	virtual VOID ReBirthParticle(_tagQuadRain *pQuadRain);
};

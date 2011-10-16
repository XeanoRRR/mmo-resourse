//---------------------------------------------------------------------------------
// ����     : ����
// ����ʱ�� : 2004��2��5�� 13:44:03
// ����ʱ�� : 
// ժҪ     : ����Ƭʵ�������
//---------------------------------------------------------------------------------
#pragma once
#include "Weather.h"

class WeatherFog : public Weather
{
	//---------------------------------------------------------------------------------
	// ��Ա����
private:
	struct _tagFog                  // ������ݽṹ
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 speed;
		FLOAT size;

		BOOL living;
	};
	_tagFog *m_ptFog;

	FLOAT m_fSize;                  // ��׼�ߴ�

	//---------------------------------------------------------------------------------
	// ��Ա����
public:
	WeatherFog(void);
	virtual ~WeatherFog(void);

	virtual BOOL CreateParticles();
	virtual VOID Destroy();

	virtual VOID Render();
	virtual BOOL Update(BOOL bEnable);

	virtual void ModifyParameters(TCHAR *config);

	void EnableLiving(BOOL bEnable);

private:
	virtual VOID ReBirthParticle(_tagFog *pFog);
};

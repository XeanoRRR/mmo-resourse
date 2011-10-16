//---------------------------------------------------------------------------------
// ����     : ����
// ����ʱ�� : 2004��4��22�� 10:13:41
// ����ʱ�� : 
// ժҪ     : ƽ����Ӱ
//           ʵ�֣�
//               ��ģ�͵Ľŵ״���һ��Ƭ
//---------------------------------------------------------------------------------
//������2004.12 ����
#pragma once
#include "gamemap.h"


class GameFlatShadow
{
private:
	DWORD m_dwShaderGroupHandle;

	D3DXVECTOR3 m_pPos[36];
	D3DXVECTOR2 m_pUV0[36];
	D3DCOLOR	m_pColor[36];

public:
	GameFlatShadow(void);
	~GameFlatShadow(void);

	void Create();
	void Destroy();

	void Render(float x,float y,float z,float fScaleFactor, GameMap *pGameMap);
};

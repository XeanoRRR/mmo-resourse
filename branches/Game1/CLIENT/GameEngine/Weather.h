//---------------------------------------------------------------------------------
// ����     : ����
// ����ʱ�� : 2004��2��5�� 14:21:45
// ����ʱ�� : 
// ժҪ     : ����Ч������
//---------------------------------------------------------------------------------
#pragma once
#include <d3dx9.h>

#include "..\frostengine\utility.h"
#include "..\frostengine\console.h"
#include "..\frostengine\render.h"
class Weather
{
	//---------------------------------------------------------------------------------
	// ��Ա����
protected:
	struct VERTEX                       // ��Ⱦ����ṹ
	{
		D3DXVECTOR3 position;
		D3DCOLOR color;
		D3DXVECTOR2 uv0;

		D3DXVECTOR3 speed;
		static const DWORD FVF = D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1;
	};
	VERTEX *m_pVertices;
	DWORD m_dwNum;

	D3DXVECTOR3 m_vCenterPoint;        // ���ĵ�
	FLOAT m_fRadius;                   // �뾶

	FLOAT m_fHeight, m_fDeadHeight;    // ��ʼ�߶ȣ������߶�

	D3DXVECTOR3 m_vSpeed;              // �ٶ�
	D3DCOLOR m_dwColor;                // ��ɫ

	TCHAR *m_szSoundFile;

	DWORD m_dwShaderGroupHandle;

	LPDIRECT3DVERTEXBUFFER9 m_pVB; 
	LPDIRECT3DTEXTURE9      m_pTexture; 

	D3DXMATRIX m_matBillboard, m_matBillboardY, m_matWorld, m_matRotate; // Billboard�����������
	

	//---------------------------------------------------------------------------------
	// ��Ա����
public:
	Weather(void);
	virtual ~Weather(void);

	virtual BOOL CreateParticles();
	virtual VOID Destroy();

	virtual VOID Render();
	virtual BOOL Update(D3DXVECTOR3 &vWindForce = D3DXVECTOR3(0, 0, 0));

	DWORD CreateShader(TCHAR* pszFilename);
	BOOL CreateTexture(LPDIRECT3DDEVICE9 pd3dDevice, TCHAR *pFilename);

	VOID GetBillboardMatrix(D3DXMATRIX *matBillboard, D3DXMATRIX *matBillboardY) { memcpy(&m_matBillboard, matBillboard, sizeof(D3DXMATRIX)); memcpy(&m_matBillboardY, matBillboardY, sizeof(D3DXMATRIX)); }
	
	VOID GetWorldMatrix(D3DXMATRIX *matWorld) { memcpy(&m_matWorld, matWorld, sizeof(D3DXMATRIX)); }

	TCHAR* GetSoundFile() { return m_szSoundFile; }	

protected:
	virtual VOID ReBirthParticle(VERTEX *pVertex);
	FLOAT FieldRandom(FLOAT fStart, FLOAT fEnd);
	DWORD FieldRandom(DWORD dwStart, DWORD dwEnd);

	// Helper function to stuff a FLOAT into a DWORD argument
	inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }
};

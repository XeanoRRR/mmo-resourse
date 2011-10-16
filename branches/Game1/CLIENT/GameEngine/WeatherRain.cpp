#include "stdafx.h"
#include ".\weatherrain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


WeatherRain::WeatherRain(void)
{
	m_ptQuadRain = NULL;
	m_pTexture = NULL;
	m_pVB = NULL;
	
}

WeatherRain::~WeatherRain(void)
{
	ASSERT(!m_ptQuadRain);
	ASSERT(!m_pTexture);
	ASSERT(!m_pVB);
}

BOOL WeatherRain::CreateParticles()
{
	// calculate rotating matrix based on vSpeed
	D3DXVECTOR3 vector = m_vSpeed;
	vector.y *= -1.0f;
	D3DXVec3Normalize(&vector, &vector);

	FLOAT fangle;
	fangle = -acosf(D3DXVec3Dot(&D3DXVECTOR3(0, 1, 0), &vector));

	D3DXVECTOR3 axis;
	D3DXVec3Cross(&axis, &D3DXVECTOR3(0, 1, 0), &vector);

	D3DXMatrixRotationAxis(&m_matRotate, &axis, fangle);

	m_ptQuadRain = new _tagQuadRain[m_dwNum];

	_tagQuadRain *pQuadRain;
	for(DWORD i = 0; i < m_dwNum; i++)
	{
		pQuadRain = &m_ptQuadRain[i];

		// ��ָ����Χ�����λ��
		pQuadRain->position.x = FieldRandom(m_vCenterPoint.x - m_fRadius, m_vCenterPoint.x + m_fRadius);
		pQuadRain->position.y = FieldRandom(m_fHeight, m_fHeight * 2.0f);
		pQuadRain->position.z = FieldRandom(m_vCenterPoint.z - m_fRadius, m_vCenterPoint.z + m_fRadius);
		
		// ��ָ����Χ������ٶ�
		pQuadRain->speed.x = FieldRandom(m_vSpeed.x * 0.9f, m_vSpeed.x * 1.1f);
		pQuadRain->speed.y = FieldRandom(m_vSpeed.y * 0.9f, m_vSpeed.y * 1.1f);
		pQuadRain->speed.z = FieldRandom(m_vSpeed.z * 0.9f, m_vSpeed.z * 1.1f);

		// ��ָ����Χ��������ȡ���Ⱥ��ٶ�
		pQuadRain->length = FieldRandom(m_fMinLength, m_fMaxLength);
		pQuadRain->width = FieldRandom(m_fWidth * 0.8f, m_fWidth * 1.2f);

		pQuadRain->living = FALSE;
	}

	//if( FAILED( pd3dDevice->CreateVertexBuffer( m_dwNum * 4 * sizeof(VERTEX),
	//	0 /* Usage */, VERTEX::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL) ) )
	//	return FALSE;

	m_pVertices = new VERTEX[m_dwNum * 4];

	return TRUE;
}

void WeatherRain::ModifyParameters(TCHAR *config)
{
	utility::Text script;
	script.Create(config);

	utility::Text::Node *pRoot = script.GetRootNode()->GetChildNode(0);
	utility::Text::Variable *pVar;

	TCHAR path[128];
	pVar = pRoot->GetChildNode(0)->GetVar("file");
	sprintf(path, "%s", pVar->GetStringValue());
	CreateShader(path);

	// �������ĵ�
	pVar = pRoot->GetChildNode(1)->GetVar("x");
	m_vCenterPoint.x = pVar->GetFloatValue();

	pVar = pRoot->GetChildNode(1)->GetVar("y");
	m_vCenterPoint.y = pVar->GetFloatValue();

	pVar = pRoot->GetChildNode(1)->GetVar("z");
	m_vCenterPoint.z = pVar->GetFloatValue();
	
	// ����뾶
	pVar = pRoot->GetVar("Radius");
	m_fRadius = pVar->GetFloatValue();
	
	// ��������
	pVar = pRoot->GetVar("Number");
	m_dwNum = (DWORD)pVar->GetIntValue();

	// ��ʼ�߶�
	pVar = pRoot->GetVar("Height");
	m_fHeight = pVar->GetFloatValue();
	
	// �����߶�
	pVar = pRoot->GetVar("DeadHeight");
	m_fDeadHeight = pVar->GetFloatValue();
	
	// ���ӿ��
	pVar = pRoot->GetVar("Width");
	m_fWidth = pVar->GetFloatValue();

	// ��С����
	pVar = pRoot->GetVar("MinLength");
	m_fMinLength = pVar->GetFloatValue();
	
	// ��󳤶�
	pVar = pRoot->GetVar("MaxLength");
	m_fMaxLength = pVar->GetFloatValue();

	// �����ٶ�
	pVar = pRoot->GetChildNode(2)->GetVar("x");
	m_vSpeed.x = pVar->GetFloatValue();

	pVar = pRoot->GetChildNode(2)->GetVar("y");
	m_vSpeed.y = pVar->GetFloatValue();

	pVar = pRoot->GetChildNode(2)->GetVar("z");
	m_vSpeed.z = pVar->GetFloatValue();
	
	// ��ɫ
	DWORD a, r, g, b;
	pVar = pRoot->GetChildNode(3)->GetVar("a");
	a = (DWORD)pVar->GetIntValue();

	pVar = pRoot->GetChildNode(3)->GetVar("r");
	r = (DWORD)pVar->GetIntValue();

	pVar = pRoot->GetChildNode(3)->GetVar("g");
	g = (DWORD)pVar->GetIntValue();

	pVar = pRoot->GetChildNode(3)->GetVar("b");
	b = (DWORD)pVar->GetIntValue();

	m_dwColor = D3DCOLOR_ARGB(a, r, g, b);

	// ����
	pVar = pRoot->GetChildNode(4)->GetVar("file");
	const char *buf = pVar->GetStringValue();
	if(strcmp("null", buf) != 0)
	{
		if (m_szSoundFile)
			SAFEDELETEARRAY(m_szSoundFile);		
		m_szSoundFile = new TCHAR[strlen(buf)+1];
		memcpy(m_szSoundFile, buf, sizeof(TCHAR) * strlen(buf));
		m_szSoundFile[strlen(buf)]='\0';
	}
	else
	{
		if (m_szSoundFile)
			SAFEDELETEARRAY(m_szSoundFile);
	}


	script.Destroy();

	CreateParticles();
}

//---------------------------------------------------------------------------------
// Function name   : WeatherRain::Destroy
// Description     : ��������
// Return type     : VOID 
//---------------------------------------------------------------------------------
VOID WeatherRain::Destroy()
{
	SAFEDELETEARRAY(m_ptQuadRain);

	Weather::Destroy();
}



/*
 * ����: ��Ⱦ����
 * ժҪ: -
 * ����: -
 * ����ֵ: VOID
 * ����: -
 * ��������: -
 * �޸���־:
 *	2008.10.08 - lpf
 *		�޸��˵ײ���Ⱦ����;
 *	2008.10.17 - lpf
 *		�����˾ֲ����������ʼֵ;
 *	2008.12.10 - lpf
 *		������������ӵ���ת����ΪY������;
 *	2009.02.24 - lpf
 *		ȡ����D3D�豸ָ������Ĵ���;
 */
VOID WeatherRain::Render()
{
	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer3D *pLayer3D = pInterface->GetLayer3D();

	//pd3dDevice->SetTexture( 0, m_pTexture );
	//pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX));
	//pd3dDevice->SetFVF(VERTEX::FVF);

	static _tagQuadRain *pQuadRain = NULL;
	static VERTEX *pVertex = NULL;
	static D3DXMATRIX matWorld, matTransform;		

	static D3DXVECTOR3 vertex[20000];
	static D3DXVECTOR2 uv0[20000];
	static DWORD color[20000]/*,colorSpecular[20000] = {0}*/;
	static WORD index[60000];
	DWORD usenum = 0;

	for(DWORD i = 0; i < m_dwNum; i++,usenum++)
	{
		pQuadRain = &m_ptQuadRain[i];
		//m_matBillboard._41 = pQuadRain->position.x;
		//m_matBillboard._42 = pQuadRain->position.y;
		//m_matBillboard._43 = pQuadRain->position.z;

		if(!pQuadRain->living)
		{
			--usenum;
			continue;
		}

		//D3DXMatrixMultiply(&matWorld, &m_matBillboard, &m_matWorld);
		D3DXMatrixMultiply(&matWorld, &m_matBillboardY, &m_matRotate);
		//pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		//pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);

		D3DXMatrixTranslation(&matTransform,pQuadRain->position.x,pQuadRain->position.y,pQuadRain->position.z);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matTransform);

		D3DXVec3TransformCoord(&m_pVertices[i*4+0].position, &m_pVertices[i*4+0].position, &matWorld);
		D3DXVec3TransformCoord(&m_pVertices[i*4+1].position, &m_pVertices[i*4+1].position, &matWorld);
		D3DXVec3TransformCoord(&m_pVertices[i*4+2].position, &m_pVertices[i*4+2].position, &matWorld);
		D3DXVec3TransformCoord(&m_pVertices[i*4+3].position, &m_pVertices[i*4+3].position, &matWorld);

		pVertex = &m_pVertices[i * 4 + 2];
		vertex[usenum * 6 + 0].x = pVertex->position.x;
		vertex[usenum * 6 + 0].y = pVertex->position.y;
		vertex[usenum * 6 + 0].z = pVertex->position.z;
		uv0[usenum * 6 + 0].x = pVertex->uv0.x;
		uv0[usenum * 6 + 0].y = pVertex->uv0.y;
		color[usenum * 6 + 0] = pVertex->color;
		index[usenum * 6 + 0] = usenum * 6 + 0;

		pVertex = &m_pVertices[i * 4];
		vertex[usenum * 6 + 1].x = pVertex->position.x;
		vertex[usenum * 6 + 1].y = pVertex->position.y;
		vertex[usenum * 6 + 1].z = pVertex->position.z;
		uv0[usenum * 6 + 1].x = pVertex->uv0.x;
		uv0[usenum * 6 + 1].y = pVertex->uv0.y;
		color[usenum * 6 + 1] = pVertex->color;
		index[usenum * 6 + 1] = usenum * 6 + 1;

		pVertex = &m_pVertices[i * 4 + 1];
		vertex[usenum * 6 + 2].x = pVertex->position.x;
		vertex[usenum * 6 + 2].y = pVertex->position.y;
		vertex[usenum * 6 + 2].z = pVertex->position.z;
		uv0[usenum * 6 + 2].x = pVertex->uv0.x;
		uv0[usenum * 6 + 2].y = pVertex->uv0.y;
		color[usenum * 6 + 2] = pVertex->color;
		index[usenum * 6 + 2] = usenum * 6 + 2;

		pVertex = &m_pVertices[i * 4 + 2];
		vertex[usenum * 6 + 3].x = pVertex->position.x;
		vertex[usenum * 6 + 3].y = pVertex->position.y;
		vertex[usenum * 6 + 3].z = pVertex->position.z;
		uv0[usenum * 6 + 3].x = pVertex->uv0.x;
		uv0[usenum * 6 + 3].y = pVertex->uv0.y;
		color[usenum * 6 + 3] = pVertex->color;
		index[usenum * 6 + 3] = usenum * 6 + 3;

		pVertex = &m_pVertices[i * 4 + 1];
		vertex[usenum * 6 + 4].x = pVertex->position.x;
		vertex[usenum * 6 + 4].y = pVertex->position.y;
		vertex[usenum * 6 + 4].z = pVertex->position.z;
		uv0[usenum * 6 + 4].x = pVertex->uv0.x;
		uv0[usenum * 6 + 4].y = pVertex->uv0.y;
		color[usenum * 6 + 4] = pVertex->color;
		index[usenum * 6 + 4] = usenum * 6 + 4;

		pVertex = &m_pVertices[i * 4 + 3];
		vertex[usenum * 6 + 5].x = pVertex->position.x;
		vertex[usenum * 6 + 5].y = pVertex->position.y;
		vertex[usenum * 6 + 5].z = pVertex->position.z;
		uv0[usenum * 6 + 5].x = pVertex->uv0.x;
		uv0[usenum * 6 + 5].y = pVertex->uv0.y;
		color[usenum * 6 + 5] = pVertex->color;
		index[usenum * 6 + 5] = usenum * 6 + 5;

		if (((usenum * 6 + 5)  > 19980))
		{
			pLayer3D->DrawTriangleArrayByShader(m_dwShaderGroupHandle, 0, usenum * 6, usenum * 6, vertex, NULL, uv0, color,  index,false, 1);
			usenum = -1;
		}
	}

	pLayer3D->DrawTriangleArrayByShader(m_dwShaderGroupHandle, 0, usenum * 6, usenum * 6, vertex, NULL, uv0, color, index,false, 1);
}



//---------------------------------------------------------------------------------
// Function name   : WeatherRain::Update
// Description     : ���ٶ��ƶ������������߶�ʱ����
// Return type     : BOOL 
// Argument        : -
//---------------------------------------------------------------------------------
BOOL WeatherRain::Update(BOOL bEnable)
{
	_tagQuadRain *pQuadRain = NULL;

	VERTEX *pVertex = NULL;
	//if( FAILED( m_pVB->Lock( 0, 0, (void**)&pVertices, 0 ) ) )
	//	return FALSE;

	DWORD dwIndex;
	for(DWORD i = 0; i < m_dwNum; i++)
	{
		pQuadRain = &m_ptQuadRain[i];

		if(pQuadRain->position.y < m_fDeadHeight)
		{
			pQuadRain->living = FALSE;
			if(bEnable)
				ReBirthParticle(pQuadRain);
		}
		else
		{
			pQuadRain->position.x += pQuadRain->speed.x;
			pQuadRain->position.y += pQuadRain->speed.y;
			pQuadRain->position.z += pQuadRain->speed.z;
		}

		if(!pQuadRain->living)
			continue;

		dwIndex = i * 4;
		pVertex = &m_pVertices[dwIndex];
		pVertex->position.x = -pQuadRain->width;
		pVertex->position.y = pQuadRain->length;
		pVertex->position.z = 0;
		pVertex->uv0.x = 0;
		pVertex->uv0.y = 0;
		pVertex->color = m_dwColor;

		dwIndex++;
		pVertex = &m_pVertices[dwIndex];
		pVertex->position.x = pQuadRain->width;
		pVertex->position.y = pQuadRain->length;
		pVertex->position.z = 0;
		pVertex->uv0.x = 1;
		pVertex->uv0.y = 0;
		pVertex->color = m_dwColor;

		dwIndex++;
		pVertex = &m_pVertices[dwIndex];
		pVertex->position.x = -pQuadRain->width;
		pVertex->position.y = -pQuadRain->length;
		pVertex->position.z = 0;
		pVertex->uv0.x = 0;
		pVertex->uv0.y = 1;
		pVertex->color = m_dwColor;

		dwIndex++;
		pVertex = &m_pVertices[dwIndex];
		pVertex->position.x = pQuadRain->width;
		pVertex->position.y = -pQuadRain->length;
		pVertex->position.z = 0;
		pVertex->uv0.x = 1;
		pVertex->uv0.y = 1;
		pVertex->color = m_dwColor;
	}

	//m_pVB->Unlock();

	return TRUE;
}

//---------------------------------------------------------------------------------
// Function name   : WeatherRain::ReBirthParticle
// Description     : ����
// Return type     : VOID 
// Argument        : _tagQuadRain *pQuadRain
//---------------------------------------------------------------------------------
VOID WeatherRain::ReBirthParticle(_tagQuadRain *pQuadRain)
{
	pQuadRain->position.x = FieldRandom(m_vCenterPoint.x - m_fRadius, m_vCenterPoint.x + m_fRadius);
	pQuadRain->position.y = FieldRandom(m_fHeight, m_fHeight * 2.0f);
	pQuadRain->position.z = FieldRandom(m_vCenterPoint.z - m_fRadius, m_vCenterPoint.z + m_fRadius);
	pQuadRain->speed.x = FieldRandom(m_vSpeed.x * 0.9f, m_vSpeed.x * 1.1f);
	pQuadRain->speed.y = FieldRandom(m_vSpeed.y * 0.9f, m_vSpeed.y * 1.1f);
	pQuadRain->speed.z = FieldRandom(m_vSpeed.z * 0.9f, m_vSpeed.z * 1.1f);
	pQuadRain->length = FieldRandom(m_fMinLength, m_fMaxLength);
	pQuadRain->width = FieldRandom(m_fWidth * 0.8f, m_fWidth * 1.2f);
	pQuadRain->living = TRUE;

	D3DXVec3TransformCoord(&pQuadRain->position, &pQuadRain->position, &m_matWorld);
}

void WeatherRain::EnableLiving(BOOL bEnable)
{
	for(DWORD i = 0; i < m_dwNum; i++)
	{
		m_ptQuadRain[i].living = bEnable;
	}
}

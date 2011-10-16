#include "stdafx.h"
#include "WeatherFlutter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 * ����: ���캯��
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.06.26
 */
WeatherFlutter::WeatherFlutter()
{
	D3DXVec3Normalize(&m_vSpeedMin, &m_vSpeedMin);
	D3DXVec3Normalize(&m_vSpeedMax, &m_vSpeedMax);
	m_tGranules		  = NULL;
	m_dwLiveTimeMax	  = 0;
	m_dwFrameLineNum  = 4;
	m_fFrameWH		  = 1.0f / m_dwFrameLineNum;
	m_dwFrameMax	  = 0;
	m_dwFrameInterval = 0;
	m_fSize = 0;
}



/*
 * ����: ��������
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.06.26
 */
WeatherFlutter::~WeatherFlutter()
{
	ASSERT(!m_tGranules);
}



/*
 * ����: �趨���еĿ����Ƿ񱻼���
 * ժҪ: -
 * ����: bEnable - �Ƿ񼤻�
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.06.26
 */
void WeatherFlutter::EnableLiving(bool bEnable)
{
	DWORD dwCurrentTime = timeGetTime();

	for(DWORD dw = 0; dw < m_dwNum; ++dw)
	{
		m_tGranules[dw].bLiving = bEnable;
		if (bEnable)
		{
			ReLiveGranules(&m_tGranules[dw]);
			m_tGranules[dw].dwStartTime = FieldRandom(dwCurrentTime - 3000, dwCurrentTime + 3000);
		}
	}
}



/*
 * ����: �޸Ĳ���
 * ժҪ: -
 * ����: pszConfig - �����ļ�·����
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.06.26
 */
void WeatherFlutter::ModifyParameters(const char * pszConfig)
{
	utility::Text script;
	script.Create(pszConfig);

	utility::Text::Node * pRoot = script.GetRootNode()->GetChildNode(0);
	utility::Text::Variable * pVar;

	TCHAR path[128];
	pVar = pRoot->GetChildNode("Shader")->GetVar("file");
	sprintf(path, "%s", pVar->GetStringValue());
	CreateShader(path);

	// ����뾶
	pVar = pRoot->GetVar("Radius");
	m_fRadius = pVar->GetFloatValue();

	// ��������
	pVar = pRoot->GetVar("Number");
	m_dwNum = (DWORD)pVar->GetIntValue();

	// ��ʼ�߶�
	pVar = pRoot->GetVar("Height");
	m_fHeight = pVar->GetFloatValue();
	
	// ���Ӵ�С
	pVar = pRoot->GetVar("Size");
	m_fSize = pVar->GetFloatValue();

	// ����֡
	pVar = pRoot->GetChildNode("AnimFrame")->GetVar("MaxLiveTime");
	m_dwLiveTimeMax = (DWORD)pVar->GetIntValue();
	pVar = pRoot->GetChildNode("AnimFrame")->GetVar("MaxFrame");
	m_dwFrameMax = (DWORD)pVar->GetIntValue();
	if (m_dwFrameMax > 16)
		m_dwFrameMax = 16;
	pVar = pRoot->GetChildNode("AnimFrame")->GetVar("FrameInterval");
	m_dwFrameInterval = (DWORD)pVar->GetIntValue();

	// �������ĵ�
	pVar = pRoot->GetChildNode("CenterPoint")->GetVar("x");
	m_vCenterPoint.x = pVar->GetFloatValue();
	pVar = pRoot->GetChildNode("CenterPoint")->GetVar("y");
	m_vCenterPoint.y = pVar->GetFloatValue();
	pVar = pRoot->GetChildNode("CenterPoint")->GetVar("z");
	m_vCenterPoint.z = pVar->GetFloatValue();

	// �ٶ�ʸ��
	pVar = pRoot->GetChildNode("Speed")->GetChildNode("Min")->GetVar("x");
	m_vSpeedMin.x = pVar->GetFloatValue();
	pVar = pRoot->GetChildNode("Speed")->GetChildNode("Min")->GetVar("y");
	m_vSpeedMin.y = pVar->GetFloatValue();
	pVar = pRoot->GetChildNode("Speed")->GetChildNode("Min")->GetVar("z");
	m_vSpeedMin.z = pVar->GetFloatValue();
	pVar = pRoot->GetChildNode("Speed")->GetChildNode("Max")->GetVar("x");
	m_vSpeedMax.x = pVar->GetFloatValue();
	pVar = pRoot->GetChildNode("Speed")->GetChildNode("Max")->GetVar("y");
	m_vSpeedMax.y = pVar->GetFloatValue();
	pVar = pRoot->GetChildNode("Speed")->GetChildNode("Max")->GetVar("z");
	m_vSpeedMax.z = pVar->GetFloatValue();

	// ��ɫ
	DWORD a, r, g, b;
	pVar = pRoot->GetChildNode("color")->GetVar("a");
	a = (DWORD)pVar->GetIntValue();
	pVar = pRoot->GetChildNode("color")->GetVar("r");
	r = (DWORD)pVar->GetIntValue();
	pVar = pRoot->GetChildNode("color")->GetVar("g");
	g = (DWORD)pVar->GetIntValue();
	pVar = pRoot->GetChildNode("color")->GetVar("b");
	b = (DWORD)pVar->GetIntValue();

	m_dwColor = D3DCOLOR_ARGB(a, r, g, b);

	// ����
	pVar = pRoot->GetChildNode(4)->GetVar("file");
	const char *buf = pVar->GetStringValue();
	if(strcmp("null", buf) != 0)
	{
		m_szSoundFile = new TCHAR[128];
		memcpy(m_szSoundFile, buf, sizeof(TCHAR) * strlen(buf));
		m_szSoundFile[strlen(buf)]='\0';
	}

	script.Destroy();

	Create();
}



/*
 * ����: ������������
 * ժҪ: ���ô˺���ǰ��Ҫ�ȵ����޸Ĳ���,�ó�Ա����ӵ����Ҫ�Ĳ���
 * ����: -
 * ����ֵ: ���Ƿ���true
 * ����: lpf
 * ��������: 2008.06.26
 */
bool WeatherFlutter::Create()
{
	DWORD	 dwIndex;
	VERTEX * pVertex;

	m_tGranules = new tagGranule[m_dwNum];
	m_pVertices = new VERTEX[m_dwNum * 4];

	for (DWORD dw = 0; dw < m_dwNum; ++dw)
	{
		m_tGranules[dw].bLiving = false;

		// ���춥��ģ������
		dwIndex = dw * 4;
		ProcessAnimTexture(&m_pVertices[dwIndex], m_tGranules[dw].dwStartTime, 0);

		pVertex = &m_pVertices[dwIndex++];
		pVertex->position.x = -m_fSize;
		pVertex->position.y =  m_fSize;
		pVertex->position.z = 0.0f;
		pVertex->color = m_dwColor;

		pVertex = &m_pVertices[dwIndex++];
		pVertex->position.x = m_fSize;
		pVertex->position.y = m_fSize;
		pVertex->position.z = 0.0f;
		pVertex->color = m_dwColor;

		pVertex = &m_pVertices[dwIndex++];
		pVertex->position.x = -m_fSize;
		pVertex->position.y = -m_fSize;
		pVertex->position.z = 0.0f;
		pVertex->color = m_dwColor;

		pVertex = &m_pVertices[dwIndex];
		pVertex->position.x = m_fSize;
		pVertex->position.y = -m_fSize;
		pVertex->position.z = 0.0f;
		pVertex->color = m_dwColor;
	}

	return true;
}



/*
 * ����: ���ٷ�������
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.06.26
 */
void WeatherFlutter::Destroy()
{
	SAFEDELETEARRAY(m_tGranules);
	Weather::Destroy();
}



/*
 * ����: ��Ⱦ����
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.06.26
 * �޸���־:
 *	2008.10.08 - lpf
 *		�޸��˵ײ���Ⱦ����;
 *	2008.10.17 - lpf
 *		�����˾ֲ����������ʼֵ;
 */
void WeatherFlutter::Render()
{
	render::Interface		   * pInterface = render::Interface::GetInstance();
	render::Interface::Layer3D * pLayer3D	= pInterface->GetLayer3D();
	static tagGranule * pFlutter = NULL;
	static VERTEX	  * pVertex = NULL;
	static VERTEX		apVertex[4];

	static D3DXVECTOR3 vertex[20000];
	static D3DXVECTOR2 uv0[20000];
	static DWORD color[20000]/*,colorSpecular[20000] = { 0 }*/;
	static WORD index[60000];
	DWORD  usenum = 0;
	DWORD  dwStartIndex = 0;
	DWORD  dwTmp = 0;

	for (DWORD dw = 0; dw < m_dwNum; ++dw, ++usenum)
	{
		pFlutter = &m_tGranules[dw];

		if (!pFlutter->bLiving)
		{
			--usenum;
			continue;
		}

		dwStartIndex	   = dw * 4;
		m_matBillboard._41 = pFlutter->vPos.x;
		m_matBillboard._42 = pFlutter->vPos.y;
		m_matBillboard._43 = pFlutter->vPos.z;

		memcpy(apVertex, &m_pVertices[dwStartIndex], sizeof(VERTEX) * 4);
		D3DXVec3TransformCoord(&apVertex[0].position, &m_pVertices[dwStartIndex + 0].position, &m_matBillboard);
		D3DXVec3TransformCoord(&apVertex[1].position, &m_pVertices[dwStartIndex + 1].position, &m_matBillboard);
		D3DXVec3TransformCoord(&apVertex[2].position, &m_pVertices[dwStartIndex + 2].position, &m_matBillboard);
		D3DXVec3TransformCoord(&apVertex[3].position, &m_pVertices[dwStartIndex + 3].position, &m_matBillboard);

		pVertex = &apVertex[2];
		dwTmp	= usenum * 6 + 0;
		vertex[dwTmp].x = pVertex->position.x;
		vertex[dwTmp].y = pVertex->position.y;
		vertex[dwTmp].z = pVertex->position.z;
		uv0[dwTmp].x = pVertex->uv0.x;
		uv0[dwTmp].y = pVertex->uv0.y;
		color[dwTmp] = pVertex->color;
#pragma warning (push)
#pragma warning( disable:4244)
		index[dwTmp] = dwTmp;
#pragma  warning (pop)

		pVertex = &apVertex[0];
		dwTmp	= usenum * 6 + 1;
		vertex[dwTmp].x = pVertex->position.x;
		vertex[dwTmp].y = pVertex->position.y;
		vertex[dwTmp].z = pVertex->position.z;
		uv0[dwTmp].x = pVertex->uv0.x;
		uv0[dwTmp].y = pVertex->uv0.y;
		color[dwTmp] = pVertex->color;
#pragma warning (push)
#pragma warning( disable:4244)
		index[dwTmp] = dwTmp;
#pragma warning(pop)
		pVertex = &apVertex[1];
		dwTmp	= usenum * 6 + 2;
		vertex[dwTmp].x = pVertex->position.x;
		vertex[dwTmp].y = pVertex->position.y;
		vertex[dwTmp].z = pVertex->position.z;
		uv0[dwTmp].x = pVertex->uv0.x;
		uv0[dwTmp].y = pVertex->uv0.y;
		color[dwTmp] = pVertex->color;
#pragma warning (push)
#pragma warning( disable:4244)
		index[dwTmp] = dwTmp;
#pragma warning (pop)
		pVertex = &apVertex[2];
		dwTmp	= usenum * 6 + 3;
		vertex[dwTmp].x = pVertex->position.x;
		vertex[dwTmp].y = pVertex->position.y;
		vertex[dwTmp].z = pVertex->position.z;
		uv0[dwTmp].x = pVertex->uv0.x;
		uv0[dwTmp].y = pVertex->uv0.y;
		color[dwTmp] = pVertex->color;
#pragma warning (push)
#pragma warning( disable:4244)
		index[dwTmp] = dwTmp;
#pragma warning (pop)
		pVertex = &apVertex[1];
		dwTmp	= usenum * 6 + 4;
		vertex[dwTmp].x = pVertex->position.x;
		vertex[dwTmp].y = pVertex->position.y;
		vertex[dwTmp].z = pVertex->position.z;
		uv0[dwTmp].x = pVertex->uv0.x;
		uv0[dwTmp].y = pVertex->uv0.y;
		color[dwTmp] = pVertex->color;
#pragma warning (push)
#pragma warning( disable:4244)
		index[dwTmp] = dwTmp;
#pragma warning(pop)
		pVertex = &apVertex[3];
		dwTmp	= usenum * 6 + 5;
		vertex[dwTmp].x = pVertex->position.x;
		vertex[dwTmp].y = pVertex->position.y;
		vertex[dwTmp].z = pVertex->position.z;
		uv0[dwTmp].x = pVertex->uv0.x;
		uv0[dwTmp].y = pVertex->uv0.y;
		color[dwTmp] = pVertex->color;
#pragma warning (push)
#pragma warning( disable:4244)
		index[dwTmp] = dwTmp;
#pragma warning (pop)

		if (((dwTmp)  > 19980))
		{
			pLayer3D->DrawTriangleArrayByShader(m_dwShaderGroupHandle, 0, usenum * 6, usenum * 6, vertex, NULL, uv0, color,  index,false, 1);
			usenum = -1;
		}
	}

	pLayer3D->DrawTriangleArrayByShader(m_dwShaderGroupHandle, 0, usenum * 6, usenum * 6, vertex, NULL, uv0, color,  index,false, 1);
}



/*
 * ����: ��������
 * ժҪ: -
 * ����: bEnable	- �Ƿ��������Ϊ������������
 *		 vWindForce - ����Ӱ��
 * ����ֵ: ����true
 * ����: lpf
 * ��������: 2008.06.26
 * �޸���־:
 *	2008.10.17 - lpf
 *		�����˾ֲ����������ʼֵ;
 */
bool WeatherFlutter::Update(BOOL bEnable, D3DXVECTOR3 & vWindForce)
{
	DWORD		 dwCurrentTime = timeGetTime();
	tagGranule * pGranules = NULL;

	for (DWORD dw = 0; dw < m_dwNum; ++dw)
	{
		pGranules = &m_tGranules[dw];

		// �жϿ���������(�Դ��ʱ��)
		if (dwCurrentTime - pGranules->dwStartTime > pGranules->dwLiveTimeMax)
		{
			pGranules->bLiving = false;
			if (bEnable)
			{
				ReLiveGranules(pGranules);
				pGranules->dwStartTime = dwCurrentTime;
			}
		}else
		{
			pGranules->vPos.x += pGranules->vSpeed.x;
			pGranules->vPos.y += pGranules->vSpeed.y;
			pGranules->vPos.z += pGranules->vSpeed.z;
			pGranules->vPos.x += vWindForce.x;
			pGranules->vPos.y += vWindForce.y;
			pGranules->vPos.z += vWindForce.z;
		}

		if (pGranules->bLiving)
			ProcessAnimTexture(&m_pVertices[dw * 4], pGranules->dwStartTime, dwCurrentTime);
	}

	return true;
}



/*
 * ����: ����Ϊ���������������
 * ժҪ: -
 * ����: pFlutterGranules -	��Ҫ���¸��������Ŀ���ָ��
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.06.26
 */
void WeatherFlutter::ReLiveGranules(tagGranule * pFlutterGranules)
{
	// ���ɷ��������λ��
	pFlutterGranules->vPos.x = FieldRandom(m_vCenterPoint.x - m_fRadius, m_vCenterPoint.x + m_fRadius);
	pFlutterGranules->vPos.y = m_fHeight;
	pFlutterGranules->vPos.z = FieldRandom(m_vCenterPoint.z - m_fRadius, m_vCenterPoint.z + m_fRadius);

	// ����������ٶ�
	pFlutterGranules->vSpeed.x = FieldRandom(m_vSpeedMin.x, m_vSpeedMax.x);
	pFlutterGranules->vSpeed.y = FieldRandom(m_vSpeedMin.y, m_vSpeedMax.y);
	pFlutterGranules->vSpeed.z = FieldRandom(m_vSpeedMin.z, m_vSpeedMax.z);

	D3DXVec3TransformCoord(&pFlutterGranules->vPos, &pFlutterGranules->vPos, &m_matWorld);

	pFlutterGranules->bLiving		= true;
#pragma warning (push)
#pragma warning( disable:4244)
	pFlutterGranules->dwLiveTimeMax = FieldRandom(m_dwLiveTimeMax * 0.5f, m_dwLiveTimeMax * 1.5f);
#pragma warning(pop)
}



/*
 * ����: ����������
 * ժҪ: �ú�����ֱ�Ӵ���ÿ���4����Ⱦ��������,���Դ������Ķ���ָ��Ӧ���ǵ�һ������
 * ����: pVertex	   - ��Ҫ����Ŀ�����Ⱦ����ָ��
 *		 dwStartTime   - �ÿ������󶯻�������ʼʱ��
 *		 dwCurrentTime - ��ǰʱ��
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.06.30
 */
void WeatherFlutter::ProcessAnimTexture(VERTEX * pVertex, DWORD dwStartTime, DWORD dwCurrentTime)
{
	DWORD dwCurrentFrameIndex = (dwCurrentTime - dwStartTime) / m_dwFrameInterval;
	if (dwCurrentFrameIndex >= m_dwFrameMax)
		dwCurrentFrameIndex %= m_dwFrameMax;

	pVertex[0].uv0.x = (float)(dwCurrentFrameIndex % m_dwFrameLineNum) * m_fFrameWH;
	pVertex[0].uv0.y = (float)(dwCurrentFrameIndex / m_dwFrameLineNum) * m_fFrameWH;

	pVertex[1].uv0.x = pVertex[0].uv0.x + m_fFrameWH;
	pVertex[1].uv0.y = pVertex[0].uv0.y;

	pVertex[2].uv0.x = pVertex[0].uv0.x;
	pVertex[2].uv0.y = pVertex[0].uv0.y + m_fFrameWH;

	pVertex[3].uv0.x = pVertex[0].uv0.x + m_fFrameWH;
	pVertex[3].uv0.y = pVertex[0].uv0.y + m_fFrameWH;
}
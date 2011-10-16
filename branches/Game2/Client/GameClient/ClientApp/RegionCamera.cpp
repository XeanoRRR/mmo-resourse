#include "stdafx.h"
#include "RegionCamera.h"
#include "ClientRegion.h"
#include "Player.h"
#include "Other/AudioList.h"
#include "../../EngineExtend/GameMap.h"
#include "../GameClient/Game.h"
#include "../../Input/Mouse.h"
#include "../../Input/KeyBoard.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
    // �۲����
    float CRegionCamera::s_fMinDistance = 2.0f;		//1.0f
    float CRegionCamera::s_fMaxDistance = 24.0f;	//45.0f
#else
    // �۲����
    float CRegionCamera::s_fMinDistance = 5.0f;		//(13.0f)
    float CRegionCamera::s_fMaxDistance = 26.0f;	//(22.0f)
#endif

float CRegionCamera::s_fDistance = s_fMaxDistance;

/*
 * ����: ���캯��
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.02.02
 * �޸���־:
 *	2008.07.17 - lpf
 * 		�޸���Release�汾����ʱ�ľ�ͷ�������Զ����;
 *	2008.10.17 - lpf
 * 		�޸���m_iMinVertAngleΪ20��ԭ��Ϊ5;
 *	2008.10.21 - lpf
 * 		�޸���Release�汾:m_fMaxDistance��30��Ϊ24;
 *		m_iMinVertAngle��20��Ϊ35;
 *		m_iMaxVertAngle��85��Ϊ80;
 *	2008.10.23 - lpf
 * 		�޸���Release�汾:
 *		m_fMaxDistance��24��Ϊ26;
 *		m_iMinVertAngle��35��Ϊ30;
 */
CRegionCamera::CRegionCamera(void)
{
	// �����ʼ��״̬
	m_dwState = 0;
	m_dwState |= SCS_REPOS;
	m_dwState |= SCS_KEYBOARD_ABLE;
	m_dwState |= SCS_MOUSE_WHEEL_ABLE;

	// ����ƶ����
	m_vMoveSpeed.x = 0.0f;
	m_vMoveSpeed.y = 0.0f;
	m_vMoveSpeed.z = 0.0f;
	m_vMoveDis.x   = 0.0f;
	m_vMoveDis.y   = 0.0f;
	m_vMoveDis.z   = 0.0f;
	m_fMoveDis	   = 0.0f;

	// �����ת���
	m_iHoriAngle	= 135;
	m_fHCurAngle	= 0.0f;
	m_iHDesAngle	= 0;
	m_fHRotateSpeed = 0.0f;

	// �����ֱ�ӽ����
    m_iMinVertAngle = 30;
    m_iMaxVertAngle = 80;
    m_iVertAngle    = 43;
	m_fVRotateSpeed = 0.0f;
	m_fVCurAngle	= 0.0f;
	m_iVDesAngle	= 0;

	m_nScorll = 0;

	// ����������
	m_fZoomSpeed  = 0.0f;
	m_fZoomDesDis = 0.0f;

	// ��������
	m_fShockMaxRatio	= 0.0f;
	m_fShcokrOffset		= 0.0f;
	m_dwShockStartTime	= 0;
	m_dwShockDelayTime	= 0;
	m_dwShockTimeLength = 0;

	// ���������ڴ洢����ֶ���������Ĳ���
	m_iOldHoriAngle	   = 0;
	m_iSourceHoriAngle = 135;
	m_iOldVertAngle	   = 5;
	m_iSourceVertAngle = 43;
	m_ptOldMouse.x	   = 0;
	m_ptOldMouse.y	   = 0;
	m_fMinVAngle       = 40.0f;

	m_fDistance = s_fMaxDistance;
}



/*
 * ����: ��������
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.02.02
 */
CRegionCamera::~CRegionCamera(void)
{
}



/*
 * ����: ���������Ҳ�������
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.02.02
 * �޸���־:
 *	2008.07.09- lpf
 *		�����˶��������ת�Ƕȵ�����,��Ҫ������ͨ�������̿��������ʱ;
 *	2008.07.23- lpf
 *		�ָ�����������Ա����̺�����м����Ӿ����Ų����ж�;
 *	2008.07.30- lpf
 *		����Ҽ�˫���ָ��ӽǵĵ���¼�ʱ,���ָ��Ӿ�;
 *	2008.10.17 - lpf
 * 		��������Ϸ��ֱ�ӽǿ���������Ҽ����������϶�;
 *	2008.12.04 - lpf
 * 		���������Ӧ�ó���ʧȥ����,����������Ϣ�Ĵ���;
 *	2008.12.16 - lpf
 * 		ȡ����ʹ�ü���ˮƽ��ת�ӽǵĹ��ȴ���.���յ�������ת��Ϣʱ,ֱ�Ӹı���ת�Ƕ�,Ȼ��������ı�m_nScorll����ֵΪ0,��ֹͣ��ת;
 */
void CRegionCamera::Run(void)
{
	// �����豸����
	CMouse	  * pMouse	  = GetMouse();
	CKeyBoard * pKeyboard = GetKeyboard();

	// ��굽���ұ�Ե��������ת
	POINT ptMouse;
	ptMouse.x = pMouse->GetMouseX();
	ptMouse.y = pMouse->GetMouseY();

	// ���̿���ת����
	if (m_dwState & SCS_KEYBOARD_ABLE)
	{
		if (m_dwState & SCS_ROTATE_H_ABLE && !(m_dwState & SCS_LOCK_ROTATE))
		{
			if (pKeyboard->IsKeyDown(DIK_LEFT) && pKeyboard->IsKeyDown(DIK_LCONTROL))
			{
				//|| GetGame()->GetSetup()->lControl == 1 && pMouse->GetMouseX()<5 
				//&& pMouse->GetMouseY()>0 && pMouse->GetMouseY()<SCREEN_HEIGHT)
				m_nScorll =- 1;
				ResumeCamera();
			}

			if (m_nScorll < 0)
			{
				// left
				m_iHoriAngle -= 3;
				m_iHoriAngle = m_iHoriAngle % 360;

//				if (abs(m_iHoriAngle%10) <= 1)
					m_nScorll=0;
			}

			if (pKeyboard->IsKeyDown(DIK_RIGHT) && pKeyboard->IsKeyDown(DIK_LCONTROL))
			{
				//||	GetGame()->GetSetup()->lControl == 1 && pMouse->GetMouseX()>SCREEN_WIDTH-5
				//&&	pMouse->GetMouseY()>0 && pMouse->GetMouseY()<SCREEN_HEIGHT)
				m_nScorll = 1;
				ResumeCamera();
			}

			if (m_nScorll > 0)
			{
				// right
				m_iHoriAngle += 3;
				m_iHoriAngle = m_iHoriAngle % 360;

				//if (abs(m_iHoriAngle%45) <= 1)
					m_nScorll = 0;
			}

			if (pKeyboard->IsKeyDown(DIK_NEXT) && pKeyboard->IsKeyDown(DIK_LCONTROL))
			{
				++m_iVertAngle;
				if (m_iVertAngle > m_iMaxVertAngle)
					m_iVertAngle = m_iMaxVertAngle;

				ResumeCamera();
			}

			if (pKeyboard->IsKeyDown(DIK_PRIOR) && pKeyboard->IsKeyDown(DIK_LCONTROL))
			{
				--m_iVertAngle;
				if (m_iVertAngle < m_iMinVertAngle)
					m_iVertAngle = m_iMinVertAngle;

				ResumeCamera();
			}
		}

		// �������ʱ��Ч
		//IGUIRoot *pRoot = IGUIRoot::GetGUIInstance();
		//if(!pRoot->GetFocusComp())
		//{
		//	// ���̷���
		//	if (pKeyboard->IsKeyDown(DIK_SUBTRACT) || pKeyboard->IsKeyDown(DIK_UP))
		//	{
		//		s_fDistance -= 0.5f;

		//		if (s_fDistance > s_fMaxDistance)
		//			s_fDistance = s_fMaxDistance;
		//		else if (s_fDistance < s_fMinDistance)
		//			s_fDistance = s_fMinDistance;

		//		ResumeCamera();
		//		RemoveCameraState();
		//		m_dwState &= ~SCS_SMOOTH;
		//	}

		//	if (pKeyboard->IsKeyDown(DIK_ADD) || pKeyboard->IsKeyDown(DIK_DOWN))
		//	{
		//		s_fDistance += 0.5f;
		//		if (s_fDistance > s_fMaxDistance)
		//			s_fDistance = s_fMaxDistance;
		//		else if (s_fDistance < s_fMinDistance)
		//			s_fDistance = s_fMinDistance;

		//		ResumeCamera();
		//		RemoveCameraState();
		//		m_dwState &= ~SCS_SMOOTH;
		//	}
		//}
	}

	// ����
	if ((m_dwState & SCS_MOUSE_WHEEL_ABLE) && pMouse->GetCurrentButton() == MB_WHEEL)
	{
		s_fDistance -= (float)((short)HIWORD(pMouse->GetWheelDelta())) * 0.01f;
		if (s_fDistance > s_fMaxDistance)
		{
			s_fDistance = s_fMaxDistance;	
		}
		else if (s_fDistance < s_fMinDistance)
		{
			s_fDistance = s_fMinDistance;
		}
		float fzDelta = (float)((short)HIWORD(pMouse->GetWheelDelta()));
		if (GetGame()->GetMainPlayer()->GetAction() != CShape::ACT_WALK&&GetGame()->GetMainPlayer()->GetAction() != CShape::ACT_RUN)
		{
			m_dwState &= ~SCS_SMOOTH;
		}
		
		ResumeCamera();
		RemoveCameraState();
	}

	extern bool g_bIsFocus;
	if (!g_bIsFocus)
		pMouse->ClearMessage();

	// ����Ҽ�����
	bool bRightUp = false;

	// ����Ҽ�����һ��
	if (pMouse->GetCurrentButton() == RB_DOWN)
	{
		m_ptOldMouse.x  = pMouse->GetMouseX();
		m_ptOldMouse.y  = pMouse->GetMouseY();
		m_iOldHoriAngle = m_iHoriAngle;
		m_iOldVertAngle = m_iVertAngle;
		ResumeCamera();
	}

	// ����Ҽ�����
	if (pMouse->GetCurrentButton() == RB_UP)
		bRightUp = true;

	// ����Ҽ����²���
	if (pMouse->IsRightButtonDown() && !bRightUp)
	{
		if ((pMouse->GetMouseX() - m_ptOldMouse.x) > 1 || (pMouse->GetMouseX() - m_ptOldMouse.x) < -1)
		{
			if (!(m_dwState & SCS_LOCK_ROTATE))
			{
				m_iHoriAngle = (int)( m_iOldHoriAngle + (pMouse->GetMouseX() - m_ptOldMouse.x)*0.5 ) % 360;
				RemoveCameraState();
			}
			/*m_iVertAngle = (int)( m_iOldVertAngle + (pMouse->GetMouseX() - m_ptOldMouse.x)*0.5 ) % 360;
			m_dwState	 &= ~SCS_REPOS_V;*/
		}

		m_iVertAngle += (int)(((float)pMouse->GetMouseMoveY() * 0.5f));
		if (m_iVertAngle < m_iMinVertAngle)
			m_iVertAngle = m_iMinVertAngle;
		else if (m_iVertAngle > m_iMaxVertAngle)
			m_iVertAngle = m_iMaxVertAngle;

		ResumeCamera();
	}

	//IGUIRoot *pRoot = IGUIRoot::GetGUIInstance();
	//if(pRoot && pRoot->GetGUIComp())
	//{
	//	CComponent * pComp = pRoot->GetGUIComp()->GetComponentAt(pMouse->GetMouseX(),pMouse->GetMouseY());
	//	if(pComp && pComp->IsEnabled() && pComp->IsVisible() && 
	//		strcmp(pComp->GetName(),"ChatInfo")!=0 && strcmp(pComp->GetName(),"TeamPage")!=0 
	//		&& strcmp(pComp->GetName(),"MedalTipPage")!=0)
	//	{
	//		return;
	//	}
	//	// ����Ҽ�˫��
	//	if (pMouse->GetCurrentButton() == RB_DCLICK && !(m_dwState & SCS_LOCK_ROTATE))
	//	{
	//		m_dwState |= SCS_REPOS_H;
	//		m_dwState |= SCS_REPOS_V;
	//		//m_dwState |= SCS_REPOS_D;
	//	}
	//}
}



/*
 * ����: �������״̬�Զ�����
 * ժҪ: �ú���������Ҫʵ��,��������ĵ�ǰ״̬�Զ���������ڲ�����,�����ⲿ��Ԥ
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.02.02
 * �޸���־:
 *	2008.03.08 - lpf
 *		�����������ִ��ˮƽ��ת�ʹ�ֱ��ת,������Ŀ��λ��ʱ,�᲻ͣ��������
 *	2008.10.17 - lpf
 * 		��ʱ������"����ֱ�Ƕ�����������";
 */
void CRegionCamera::Update(void)
{
	CClientRegion * c = GetGame()->GetRegion();
	if (c == NULL) /*&& GetGame()->GetSetup()->bSound*/ 
		return;

	// ������3D��Ч
	D3DXVECTOR3 * pos;
	D3DXVECTOR3	  face;
	D3DXVECTOR3   right;
	D3DXVECTOR3   up;

	pos = c->GetCamera()->GetPosition();

	//pos->z = -pos->z;
	GetGame()->GetAudioList()->SetListenerPosition(*pos);

	//pos->z = -pos->z;
	face = (*c->GetCamera()->GetView()) - (*pos);

	Vector3 v = face;
	D3DXVec3Normalize(&face, &face);
	D3DXVec3Cross(&right, &Vector3(0, 1, 0), &face);
	D3DXVec3Normalize(&right, &right);
	D3DXVec3Cross(&up, &face, &right);

	//face.z = -face.z;
	//up.z   = -up.z;
	GetGame()->GetAudioList()->SetListenerOritation(face, up);

	if (g_bDebug)
	{
		if (GetKeyboard()->IsKeyDown(DIK_TAB))
		{
			char t[255];
			sprintf(t, "����ƽ��:%f", (v.x) * (v.x) + v.y * v.y + (v.z) * (v.z));
			GetGame()->OutPutText(140, 40, t, 255, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	// �ص�Ĭ���������ͷ�Ƕ�H
	if (m_dwState & SCS_REPOS_H)
	{
		if (abs(m_iHoriAngle - m_iSourceHoriAngle) <= 5)
		{
			m_dwState	 &= ~SCS_REPOS_H;
			m_iHoriAngle = m_iSourceHoriAngle;
		}else
		{
			if (m_iHoriAngle > 180)
				m_iHoriAngle -= 360;
			else if(m_iHoriAngle < -180)
				m_iHoriAngle += 360;
			if (m_iHoriAngle > -45 && m_iHoriAngle < 135)
				m_iHoriAngle += 4;
			else if (m_iHoriAngle <= -45 || m_iHoriAngle >= 135)
				m_iHoriAngle -= 4;
		}
	}
	// �ص�Ĭ���������ͷ�Ƕ�V
	if (m_dwState & SCS_REPOS_V)
	{
		if (abs(m_iVertAngle - m_iSourceVertAngle) <= 5)
		{
			m_dwState	 &= ~SCS_REPOS_V;
			m_iVertAngle = m_iSourceVertAngle;
		}else
		{
			m_iVertAngle += (m_iSourceVertAngle - m_iVertAngle)/abs(m_iSourceVertAngle - m_iVertAngle) * 4;
		}
	}
	// �ص�Ĭ���������ͷ�Ƕ�V
	if (m_dwState & SCS_REPOS_D)
	{
		if (abs(s_fDistance - 17.0f) <= 1.0f)
		{
			m_dwState	 &= ~SCS_REPOS_D;
			s_fDistance = 17.0f;
		}else
		{
			s_fDistance += (17.0f - s_fDistance)/abs(17.0f - s_fDistance) * 0.5f;
		}
	}

	// ��������ƶ�
	if (m_dwState & SCS_MOVE)
	{
		if (D3DXVec3Length(&m_vMoveDis) < m_fMoveDis)
			m_vMoveDis += m_vMoveSpeed;
		//else
		//	m_dwState &= ~SCS_MOVE;
	}

	// �������ˮƽ��ת
	if (m_dwState & SCS_ROTATE_H)
	{
		if (m_iHoriAngle > m_iHDesAngle)
		{
			m_fHCurAngle -= m_fHRotateSpeed;
			m_iHoriAngle = (int)m_fHCurAngle;

			if (m_iHoriAngle < m_iHDesAngle)
				m_iHoriAngle = m_iHDesAngle;
		}else if (m_iHoriAngle < m_iHDesAngle)
		{
			m_fHCurAngle += m_fHRotateSpeed;
			m_iHoriAngle = (int)m_fHCurAngle;

			if (m_iHoriAngle > m_iHDesAngle)
				m_iHoriAngle = m_iHDesAngle;
		}else
			m_dwState &= ~SCS_ROTATE_H;
	}

	// ���������ֱ��ת
	if (m_dwState & SCS_ROTATE_V)
	{
		if (m_iVertAngle > m_iVDesAngle)
		{
			m_fVCurAngle -= m_fVRotateSpeed;
			m_iVertAngle = (int)m_fVCurAngle;

			if (m_iVertAngle < m_iVDesAngle)
				m_iVertAngle = m_iVDesAngle;
		}else if (m_iVertAngle < m_iVDesAngle)
		{
			m_fVCurAngle += m_fVRotateSpeed;
			m_iVertAngle = (int)m_fVCurAngle;

			if (m_iVertAngle > m_iVDesAngle)
				m_iVertAngle = m_iVDesAngle;
		}else
			m_dwState &= ~SCS_ROTATE_V;
	}

	// �����������
	if (m_dwState & SCS_ZOOM)
	{
		if (m_fZoomSpeed > 0.0f)
		{
			if (s_fDistance < m_fZoomDesDis)
				s_fDistance += m_fZoomSpeed;
			else
				m_dwState &= ~SCS_ZOOM;
		}else if (m_fZoomSpeed < 0.0f)
		{
			if (s_fDistance > m_fZoomDesDis)
				s_fDistance += m_fZoomSpeed;
			else
				m_dwState &= ~SCS_ZOOM;
		}

		if (s_fDistance > s_fMaxDistance)
		{
			s_fDistance = s_fMaxDistance;
			m_dwState	&= ~SCS_ZOOM;
		}else if (s_fDistance < s_fMinDistance)
		{
			s_fDistance = s_fMinDistance;
			m_dwState	&= ~SCS_ZOOM;
		}
	}

	// ���������
	if (m_dwState & SCS_SHOCK)
	{
		static const float fOffsetArray[6] = { 1.0f, -0.6f, 0.8f, -0.2f, 0.4f, 0.0f };
		static int		   iOffPos		   = 0;
		
		DWORD dwCurrentTime = timeGetTime();
		DWORD dwTimeElapse  = dwCurrentTime - m_dwShockStartTime;
		if (dwTimeElapse > m_dwShockDelayTime)
		{
			m_fShcokrOffset = fOffsetArray[iOffPos] * m_fShockMaxRatio;
			if (dwTimeElapse > m_dwShockTimeLength)
			{
				iOffPos	  = 0;
				m_dwState &= ~SCS_SHOCK;
			}

			if (iOffPos > 4)
				iOffPos = 0;
			else
				++iOffPos;
		}
	}

	// ����ֱ�Ƕ�����������
	//float angle = s_fDistance/17.0f * 43.0f;
	//m_fMinVAngle = angle > 43.0f?43.0f:angle;
	//if (m_iVertAngle < m_fMinVAngle)
	//{
	//	m_iVertAngle = m_fMinVAngle;
	//	m_fVCurAngle = m_iVertAngle;
	//}

	if (s_fDistance > 0.0f)
		m_fDistance = s_fDistance;
}



/*
 * ����: �õ�����ֲ�����
 * ժҪ: �ú������ص�������������������ת�ǶȺ��Ӿ��������ľֲ�����,������λ����Ϣ
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.02.02
 */
void CRegionCamera::GetPosition(D3DXVECTOR3 &vPosition)
{
	float x, y, z, r;

	// ����ˮƽ��ת��λ
	if (m_dwState & SCS_ROTATE_H)
	{
		z = cosf(D3DXToRadian(m_fHCurAngle));
		x = sinf(D3DXToRadian(m_fHCurAngle));
	}else
	{
		z = cosf(D3DXToRadian(m_iHoriAngle));
		x = sinf(D3DXToRadian(m_iHoriAngle));
	}

	// ���㴹ֱ��ת��λ
	if (m_dwState & SCS_ROTATE_V)
	{
		y = sinf(D3DXToRadian(m_fVCurAngle)) * m_fDistance;
		r = cosf(D3DXToRadian(m_fVCurAngle)) * m_fDistance;
	}else
	{
		y = sinf(D3DXToRadian(m_iVertAngle)) * m_fDistance;
		r = cosf(D3DXToRadian(m_iVertAngle)) * m_fDistance;
	}

	vPosition = D3DXVECTOR3(x * r, y, z * r);
}



/*
 * ����: �����
 * ժҪ: -
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.02.02
 */
void CRegionCamera::ShockCamera(float fMaxRatio, DWORD dwDelayTime, DWORD dwTimeLength)
{
	m_fShockMaxRatio	= fMaxRatio;
	m_dwShockDelayTime	= dwDelayTime;
	m_dwShockStartTime	= timeGetTime();
	m_dwShockTimeLength = dwTimeLength;
	m_fShcokrOffset		= 0.0f;
	m_dwState			|= SCS_SHOCK;
}



/*
 * ����: ƽ���ƶ������Ŀ���
 * ժҪ: �ú���������һ��,�����㹻����Ϣ,���Զ��ı������״̬
 * ����: fSpeed �C �ƶ��ٶ�(ÿ֡�ƶ��ľ��Ծ���)
 *		 fDesX  �C Ŀ���X����
 *		 fDesY   - Ŀ���Y����
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.02.01
 */
void CRegionCamera::MoveCamera(float fSpeed, float fDesX, float fDesY)
{
	CClientRegion * pRegion = GetGame()->GetRegion();
	D3DXVECTOR3		vView	= *(pRegion->GetCamera()->GetView());
	D3DXVECTOR3		vDesView(fDesY, pRegion->GetGameMap()->CalculateHeight(fDesY, fDesX) - GetCameraPosModify(), fDesX);

	m_vMoveSpeed = vDesView - vView;
	m_fMoveDis	 = D3DXVec3Length(&m_vMoveSpeed);
	D3DXVec3Normalize(&m_vMoveSpeed, &m_vMoveSpeed);

	m_vMoveSpeed *= fSpeed;
	m_vMoveDis.x = 0.0f;
	m_vMoveDis.y = 0.0f;
	m_vMoveDis.z = 0.0f;
	m_dwState	 |= SCS_MOVE;
	m_dwState	 &= ~SCS_REPOS;
}



/*
 * ����: ˮƽ��ת���
 * ժҪ: �ú���������һ��,�����㹻����Ϣ,���Զ��ı������״̬
 * ����: fSpeed    �C ��ת���ٶ�
 *		 iDesAngle �C Ŀ�ĽǶ�
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.02.01
 */
void CRegionCamera::RotateCameraH(float fSpeed, int iDesAngle)
{
	m_fHRotateSpeed	= fSpeed;
	m_fHCurAngle	= (float)m_iHoriAngle;
	m_iHDesAngle	= iDesAngle;
	m_dwState		|= SCS_ROTATE_H;
	m_dwState		&= ~SCS_REPOS;
}



/*
 * ����: ��ֱ��ת���
 * ժҪ: �ú���������һ��,�����㹻����Ϣ,���Զ��ı������״̬
 * ����: fSpeed    �C ��ת���ٶ�
 *		 iDesAngle �C Ŀ�ĽǶ�
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.02.01
 */
void CRegionCamera::RotateCameraV(float fSpeed, int iDesAngle)
{
	if (iDesAngle > m_iMaxVertAngle)
		iDesAngle = m_iMaxVertAngle;
	if (iDesAngle < m_iMinVertAngle)
		iDesAngle = m_iMinVertAngle;

	m_fVRotateSpeed	= fSpeed;
	m_fVCurAngle	= (float)m_iVertAngle;
	m_iVDesAngle	= iDesAngle;
	m_dwState		|= SCS_ROTATE_V;
	m_dwState		&= ~SCS_REPOS;
}



/*
 * ����: ���������ע�ӵ�ľ���
 * ժҪ: �ú���������һ��,�����㹻����Ϣ,���Զ��ı������״̬
 * ����: fSpeed  �C ��ת���ٶ�
 *		 fDesDis �C Ŀ�ľ���
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.02.01
 */
void CRegionCamera::ZoomCamera(float fSpeed, float fDesDis)
{
	if (m_fDistance < m_fZoomDesDis)
		m_fZoomSpeed  = abs(fSpeed);
	else
		m_fZoomSpeed  = abs(fSpeed) * -1.0f;

	m_fZoomDesDis = fDesDis;
	m_dwState	  |= SCS_ZOOM;
	m_dwState	  &= ~SCS_REPOS;
}



/*
 * ����: �ָ�������õ���ҽ��볡��ʱ״̬
 * ժҪ: �ú���������һ��,�����㹻����Ϣ,���Զ��ı������״̬
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.02.01
 */
void CRegionCamera::ResumeCamera(void)
{
	if (m_dwState & SCS_REPOS)
		return;

	// ��������ĳЩ״̬
	m_dwState &= ~SCS_ROTATE_H;		//���ˮƽ��ת
	m_dwState &= ~SCS_ROTATE_V;		//�����ֱ��ת
	m_dwState &= ~SCS_ZOOM;			//����Ӿ�����
	m_dwState &= ~SCS_MOVE;			//���ƽ��

	m_dwState |= SCS_REPOS;
	m_dwState &= ~SCS_SMOOTH;

	m_iHoriAngle = 135;
	m_iVertAngle = 43;
	s_fDistance  = 17.0f;
}



/*
 * ����: ƽ���������
 * ժҪ: -
 * ����: vView	  - ������ӵ�����
 *		 vMainPos - ���ǵ���������
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.02.02
 */
void CRegionCamera::SmoothCamera(D3DXVECTOR3 & vView, D3DXVECTOR3 & vMainPos)
{
	
	static float	   cameraspeed = 0.0f;
	static float	   lasttime	   = (float)timeGetTime();
	static D3DXVECTOR3 lastviewpos(vView);
	static D3DXVECTOR3 lastmainpos(vMainPos);

	float nowtime	  = (float)timeGetTime();
	float deltatime   = (nowtime - lasttime) / 1000;
	
	float playerspeed = FM_Distance(vMainPos, lastmainpos);
	float mod		  = FM_Distance(vMainPos, vView);
	float viewdistance = FM_Distance(lastviewpos,vView);

	D3DXVECTOR3 dir = vMainPos - vView;
	D3DXVec3Normalize(&dir, &dir);
	if (mod > 6)
	{
		vView = vMainPos;
		return;
	}

	mod = mod * 0.08f;
	vView = vMainPos;

	
	lastviewpos			= vView;
	lasttime			= nowtime;
	lastmainpos			= vMainPos;
}



/*
 * ����: �趨�������
 * ժҪ: -
 * ����: pCamera - ��Ҫ�趨���������ָ��
 *		 fX		 - ��Ҵ��������X����
 *		 fY		 - ��Ҵ��������Y����
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.02.02
 */
void CRegionCamera::SetCamera(render::Camera * pCamera, float fX, float fY)
{
	float x1(fY);
	float z1(fX);
	CClientRegion * pRegion = GetGame()->GetRegion();
	D3DXVECTOR3 vMainPlayerPos(x1, pRegion->GetGameMap()->CalculateHeight(x1, z1) + 0.5f - GetCameraPosModify(), z1);
	D3DXVECTOR3 vTPos;
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vViewTmp;

	GetPosition(vTPos);

	// �����
	if (m_dwState & SCS_SHOCK)
		vMainPlayerPos.y += m_fShcokrOffset;

	static D3DXVECTOR3 vView(vMainPlayerPos);

	// ���ƽ��
	if (m_dwState & SCS_SMOOTH)
		SmoothCamera(vView, vMainPlayerPos);
	else
	{
		vView	  = vMainPlayerPos;
		m_dwState |= SCS_SMOOTH;
	}

	vViewTmp = vView;

	// ����ƶ�
	if (m_dwState & SCS_MOVE)
	{
		vTPos    += m_vMoveDis;
		vViewTmp += m_vMoveDis;
	}

	// �趨���
	vPos = vView + vTPos;
	pCamera->SetView(&vViewTmp);
	if (GetGame())
	{
		if (GetGame()->GetRegion())
		{
			if (GetGame()->GetRegion()->GetGameMap())
			{
				if (vPos.y - 0.5 < GetGame()->GetRegion()->GetGameMap()->CalculateHeight(vPos.x,vPos.z))
				{
					vPos.y = GetGame()->GetRegion()->GetGameMap()->CalculateHeight(vPos.x,vPos.z) + 2;
				}
			}
		}
	}
	pCamera->SetPosition(&vPos);
}



/*
 * ����: ��������ǰ״̬��Ϣ
 * ժҪ: ���������ݻָ�����,���ֵ�ǰ״̬
 *		 Ŀǰֻ���������ָ��ӽǲ���
 * ����: -
 * ����ֵ: -
 * ����: lpf
 * ��������: 2008.07.23
 */
void CRegionCamera::RemoveCameraState()
{
	// ��������ĳЩ״̬
	m_dwState &= ~SCS_REPOS_H;
	m_dwState &= ~SCS_REPOS_V;
	m_dwState &= ~SCS_REPOS_D;
}



//�����������ʽ
void CRegionCamera::TransformCamer(long lOperaMode,long lParam1,long lParam2,long lParam3)
{
	switch(lOperaMode)
	{
		//��
	case 1:
		ShockCamera((float)lParam1,lParam2,lParam3);
		break;
		//ƽ���ƶ�
	case 2:
		MoveCamera((float)lParam1,(float)lParam2,(float)lParam3);
		break;
		//ˮƽ��ת
	case 3:
		RotateCameraH((float)lParam1,lParam2);
		break;
		//��ֱ��ת
	case 4:
		RotateCameraV((float)lParam1,lParam2);
		break;
		//����仯
	case 5:
		ZoomCamera((float)lParam1,(float)lParam2);
		break;
		//�ָ�
	case 6:
		ResumeCamera();
		break;
	}
}
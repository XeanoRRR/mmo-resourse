/* CRegionCamera.h
 * ����: �������������и��߲�εķ�װ�����������Ҫ�����ݶ�����������ϻ�ȡ��
 * ժҪ: �����֧�����ƽű�����������������һ�β�������������Զ�����ƶ���������ת�Ȳ���
 *		 ��ͬ���͵Ĳ��������໥Ӱ��,����ͬʱִ��
 *		 ��һ�����͵Ĳ���û�����ʱ,�����ⲿ��������ͬ�����͵Ĳ���ʱ,�����ִ�к�һ�β���
 * ����: lpf
 * ��������: 2008-02-02
 */

#pragma once
#include "../../Engine/Render.h"
// ���������
class CRegionCamera
{
	// �������״̬ö��
	enum eREGION_CAMERA_STATE
	{
		// ��������״̬
		SCS_ROTATE_H = 0x00000001,		//���ˮƽ��ת
		SCS_ROTATE_V = 0x00000002,		//�����ֱ��ת
		SCS_ZOOM	 = 0x00000004,		//����Ӿ�����
		SCS_MOVE	 = 0x00000008,		//���ƽ��

		// �������ָ�״̬
		SCS_REPOS	 = 0x00000010,		//�����ǰ״̬�Ƿ��ǳ�ʼ״̬
		SCS_REPOS_H	 = 0x00000020,		//�������ˮƽ����(�ظ�������״̬)
		SCS_REPOS_V	 = 0x00000040,		//������费ֱ����(�ظ�������״̬)
		SCS_REPOS_D	 = 0x00000080,		//����������(�ظ�������״̬)

		// ��Ч״̬
		SCS_SHOCK	 = 0x00000100,		//�����
		SCS_SMOOTH	 = 0x00000200, 		//���ƽ��

		// ����״̬
		SCS_ROTATE_H_ABLE	 = 0x00001000,	//�Ƿ����ˮƽ��ת
		SCS_KEYBOARD_ABLE	 = 0x00002000,	//�����Ƿ���Ч
		SCS_MOUSE_WHEEL_ABLE = 0x00004000,	//�������Ƿ���Ч
		SCS_LOCK_ROTATE		 = 0x00008000,	//�Ƿ�������ת(������û����ͼ��̲���)
	};

	// ���������ڴ洢����Զ������Ĳ���
	DWORD m_dwState;				//��ǰ���״̬

	// ����ƶ����
	D3DXVECTOR3 m_vMoveSpeed;		//����ƶ��ٶ�����
	D3DXVECTOR3 m_vMoveDis;			//�����ǰ�ƶ���������
	float		m_fMoveDis;			//�����Ҫ�ƶ��ľ���

	// �����ת���
	int   m_iHoriAngle;				//ˮƽ�ӽ�
	float m_fHCurAngle;				//�����ǰ��ת�Ƕ�
	int	  m_iHDesAngle;				//���Ŀ����ת�Ƕ�
	float m_fHRotateSpeed;			//�����ת���ٶ�

	// �����ֱ�ӽ����
	int   m_iMinVertAngle;			//��С��ֱ�Ƕ�
	int   m_iMaxVertAngle;			//���ֱ�Ƕ�
	int   m_iVertAngle;				//��ǰ��ֱ�Ƕ�
	float m_fVRotateSpeed;			//�����ֱ��ת���ٶ�
	float m_fVCurAngle;				//�����ǰ�Ƕ�
	int	  m_iVDesAngle;				//���Ŀ�ĽǶ�

	int m_nScorll;					//-1:����ת 0:���� 1:����ת

	// ����������
	float m_fZoomSpeed;				//�����ٶ�
	float m_fZoomDesDis;			//����Ŀ�ľ���

	// ��������
	float m_fShockMaxRatio;			//�𶯷���
	float m_fShcokrOffset;			//��ƫ��
	DWORD m_dwShockStartTime;		//�𶯿�ʼʱ��
	DWORD m_dwShockDelayTime;		//�𶯼��
	DWORD m_dwShockTimeLength;		//��ʱ��

	// ���������ڴ洢����ֶ���������Ĳ���
	int   m_iOldHoriAngle;			//��¼ԭ����
	int   m_iSourceHoriAngle;		//����Ķ���
	int   m_iOldVertAngle;			//��¼ԭ����
	int   m_iSourceVertAngle;		//����Ķ���
	POINT m_ptOldMouse;				//����Ҽ�����,��¼������

	// �۲����
	float m_fMinVAngle;				//��ǰ�Ӿ����С����
	float m_fDistance;				//��ҵ�ǰ�����Ӿ�

	static float s_fMinDistance;	//�����С�Ӿ�
	static float s_fMaxDistance;	//�������Ӿ�
	static float s_fDistance;		//��¼��ҵ��ӽǾ���

public:
	CRegionCamera(void);
	~CRegionCamera(void);

	void Run(void);					//���������Ҳ�������
	void Update(void);				//�������״̬�Զ�����

	// ���ݷ��ʺ���
	bool  IsShock(void)								{ return (m_dwState & SCS_SHOCK) ? true : false; }								//�Ƿ���
	bool  IsSmooth(void)							{ return (m_dwState & SCS_SMOOTH) ? true : false; }								//�Ƿ�ƽ��
	float GetSockOffset(void)						{ return m_fShcokrOffset; }														//�õ���ƫ��
	float GetHoriAngle(void)						{ return (float)m_iHoriAngle * (3.1415926f / 180.0f); }							//�õ�ˮƽ��ת�Ƕ�
	void  GetPosition(D3DXVECTOR3 & vPosition);																						//�õ�����ֲ�����
	float GetCameraPosModify(void)					{ return (m_fDistance - s_fMinDistance) / (s_fMaxDistance - s_fMinDistance); }	//�õ�����޸ĺ������
	float GetDistance()								{ return s_fDistance; }															//�õ���ҵ��ӽǾ���
	void  SetMinVertAngle(int ag)					{ m_iMinVertAngle = ag; }
	void  SetMaxVertAngle(int ag)					{ m_iMaxVertAngle = ag; }

	// �����趨����
	void SetSmooth(bool b)							{ b ? m_dwState |= SCS_SMOOTH : m_dwState &= ~SCS_SMOOTH; }						//�趨ƽ��
	void SetKeyboardAble(bool b)					{ b ? m_dwState |= SCS_KEYBOARD_ABLE : m_dwState &= ~SCS_KEYBOARD_ABLE; }		//�趨�Ƿ���ܼ���
	void SetMouseWheelAble(bool b)					{ b ? m_dwState |= SCS_MOUSE_WHEEL_ABLE : m_dwState &= ~SCS_MOUSE_WHEEL_ABLE; }	//�趨�Ƿ�������ת��
	void SetHRotation(bool b)  						{ b ? m_dwState |= SCS_ROTATE_H_ABLE : m_dwState &= ~SCS_ROTATE_H_ABLE; }		//�趨�Ƿ�ˮƽ��ת
	void SetLockRotation(bool b)					{ b ? m_dwState |= SCS_LOCK_ROTATE : m_dwState &= ~SCS_LOCK_ROTATE; }			//�趨������Ƿ�������ת
	void SetCameraReposH(bool b)					{ b ? m_dwState |= SCS_REPOS_H : m_dwState &= ~SCS_REPOS_H; }					//�趨����Ƿ�ָ�ˮƽ�ӽ�
	void SetCameraReposV(bool b)					{ b ? m_dwState |= SCS_REPOS_V : m_dwState &= ~SCS_REPOS_V; }					//�趨����Ƿ�ָ���ֱ�ӽ�
	void SetCameraReposD(bool b)					{ b ? m_dwState |= SCS_REPOS_D : m_dwState &= ~SCS_REPOS_D; }					//�趨����Ƿ�ָ������ӽ�
	void SetCameraHAngle(int  angle)				{ m_iVertAngle    = angle;}		//����������ӽǶ�
	// ����Զ���������
	void ShockCamera(float fMaxRatio, DWORD dwDelayTime, DWORD dwTimeLength);		//�����
	void MoveCamera(float fSpeed, float fDesX, float fDesY);						//ƽ���ƶ������Ŀ���
	void RotateCameraH(float fSpeed, int iDesAngle);								//ˮƽ��ת���
	void RotateCameraV(float fSpeed, int iDesAngle);								//��ֱ��ת���
	void ZoomCamera(float fSpeed, float fDesDis);									//���������ע�ӵ�ľ���
	void ResumeCamera(void);														//�ָ�������õ���ҽ��볡��ʱ״̬

	// ��������
	void SmoothCamera(D3DXVECTOR3 & vView, D3DXVECTOR3 & vMainPos);			//ƽ���������
	void SetCamera(render::Camera * pCamera, float fX, float fY);			//�趨�������
	void RemoveCameraState();												//��������ǰ״̬��Ϣ

	//�����������ʽ
	void TransformCamer(long lOperaMode,long lParam1,long lParam2,long lParam3);
};
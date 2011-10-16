/* RollLantern.h
 * ����: �����ʵ����
 * ժҪ: ��Ҫ��������Ϸ�н���ϵͳ��Ϣ������ʾ,��Alpha���书��
 * ����: lpf
 * ��������: 2009-01-05
 */
class CBitmapX;
#include "../../Engine/Render.h"
#include "../../Engine/Utility.h"
#pragma once

// ͸�����������ʾ�б���
class CRollLantern
{
	// �����͸������ģʽ
	enum eALPHA_FADE_MODE
	{
		ALPHA_FADE_NONE = 0,			//û�н������
		ALPHA_FADE_IN,					//����
		ALPHA_FADE_OUT,					//����
	};

	float	   m_fSpeed;				//���ݹ����ٶ�(����/����)
	DWORD	   m_dwNumLoop;				//ѭ������
	POINT	   m_ptPos;					//����ƻ�������(������ͼƬ��������)
	long	   m_lClipLeft;				//�ü���������(���������������ϵ,���������ƻ�������)
	long	   m_lClipRight;			//�ü����ҷ�����(���������������ϵ,���������ƻ�������)
	long	   m_lContentOffsetHeight;	//��ʾ���ݸ߶�ƫ��(���������������ϵ,���������ƻ�������)
	string	   m_sContent;				//��ʾ����
	CBitmapX * m_pBackGroundPic;		//����ͼƬ
	FONTINFO   m_stFontInfo;			//����������Ϣ

	bool			 m_bIsFinished;			//������Ѿ���ʾ���
	long			 m_lContentOffset;		//���ݻ���ƫ��
	long			 m_lContentOffsetLeft;	//���ݹ���ƫ������ֵ
	long			 m_lContentOffsetRight;	//���ݹ���ƫ������ֵ
	DWORD			 m_dwStartTime;			//����ƿ�ʼʱ��(����)
	DWORD			 m_dwLoopCount;			//����ѭ������
	float			 m_fAlphaPer;			//��ǰ����ٷ���
	DWORD			 m_dwAlphaTimeLen;		//����ʱ�䳤��(����)
	DWORD			 m_dwAlphaStartTime;	//���俪ʼʱ��(����)
	eALPHA_FADE_MODE m_eAlphaMode;			//����Ƴ�����ʧ�Ľ���ģʽ

	RECT			   m_rcSourceLeft;		//Դ����
	RECT			   m_rcSourceRight;		//Դ����
	LPDIRECT3DSURFACE9 m_pOffscreenLeft;	//����������
	LPDIRECT3DSURFACE9 m_pOffscreenRight;	//�ҷ���������

public:
	CRollLantern();			//���캯��
	~CRollLantern();		//��������

	// �������ݳ�Ա
	void  GetPos(POINT & pt)	{ pt = m_ptPos; }					//�õ�����ƻ�������(������ͼƬ��������)
	bool  GetIsFinished()		{ return m_bIsFinished; }			//�õ�������Ƿ��Ѿ���ʾ���
	float GetContentOffsetLeft();									//�õ����ݹ���ƫ������ֵ
	float GetContentOffsetRight();									//�õ����ݹ���ƫ������ֵ

	// �趨���ݳ�Ա
	void SetSpeed(float f)					{ m_fSpeed = f; }					//�趨���ݹ����ٶ�(����/����)
	void SetNumLoop(DWORD dw)				{ m_dwNumLoop = dw; }				//�趨ѭ������
	void SetPos(POINT & pt);													//�趨����ƻ�������(������ͼƬ��������)
	void SetClip(long lLeft, long lRight);										//�趨�ü�������(���������������ϵ,���������ƻ�������)
	void SetContentOffsetHeight(long l)		{ m_lContentOffsetHeight = l; }		//�趨��ʾ���ݸ߶�ƫ��(���������������ϵ,���������ƻ�������)
	void SetContent(const char * psz);											//�趨��ʾ����
	bool SetBGPic(const char * pszFilePathName);								//�趨����ͼƬ
	void SetFontInfo(FONTINFO & stFontInfo) { m_stFontInfo = stFontInfo; }		//�趨����������Ϣ
	void SetAlphaTimeLen(DWORD dw)			{ m_dwAlphaTimeLen = (dw != 0) ? dw : 5000; }			//�趨����ʱ�䳤��(����)

	// �����ӿ�
	void Start();				//��ʼ��ʾ�����
	void Update();				//���������
	void Draw();				//���������
	void ReleaseOffscreen()					{ SAFERELEASE(m_pOffscreenLeft); SAFERELEASE(m_pOffscreenRight); }		//������������
	bool CreateOffscreenLeft();	//��������������
	bool CreateOffscreenRight();//�����ҷ���������

private:
	void UpdateClipRect();		//���²ü�����
};
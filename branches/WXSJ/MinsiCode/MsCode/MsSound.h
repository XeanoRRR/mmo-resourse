#pragma once
#include "fmod.hpp"
using namespace FMOD;

// ÿ�θı�������ֵ
#define VOLUME_CHANGE_ONCE_VALUE    0.05f

// �������ֵ
#define VOLUME_MAX_VALUE            2.0f

// ������Сֵ
#define VOLUME_MIN_VALUE            0.0f

// Ĭ������
#define GAME_NORMAL_VOLUME          0.8f
typedef Sound* lpSound;
typedef Channel* lpChannel;

class CMsSound
{
public: // ���졢����
    CMsSound(DWORD dwSoundCount);
    ~CMsSound(VOID);

public: // �ӿ�

    BOOL Open(      // ������
        DWORD dwIndex,
        CONST ACHAR* strSoundName,
        BOOL IsLoop
        );

    BOOL Play(      // ��������
        DWORD dwIndex,
        BOOL bPaused,
        BOOL IsMid = FALSE
        );

    BOOL SetVolume( // ��������
        DWORD dwIndex,
        FLOAT fVolume
        );

    BOOL Init(BOOL b3DSound);       // ��ʼ��������
    BOOL Pause(DWORD dwIndex);      // ��ͣ����
    BOOL Continue(DWORD dwIndex);   // ��������
    BOOL Stop(DWORD dwIndex);       // ֹͣ����
    BOOL AddVolume(DWORD dwIndex);  // ��������
    BOOL SubVolume(DWORD dwIndex);  // ��������
    BOOL IsPlaying(DWORD dwIndex);  // �Ƿ����ڲ���

private:// ˽��
    
    INT         m_nFmodInitMode;    // ������Ĵ���ģʽ
    BOOL        m_IsInit;           // �������Ƿ��ʼ�����
    System*     m_pSystem;          // ����ϵͳָ��
    DWORD       m_dwSoundCount;     // ������������
    lpSound*    m_ppSound;          // ����ָ������
    lpChannel*  m_ppChannel;        // ͨ��ָ������
    LPFLOAT     m_lpfVolume;        // ����
    BOOL        m_IsStartupSound;   // �Ƿ�������Ƶ
};

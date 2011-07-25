#include "Precompiled.h"
#include "MsSound.h"
#pragma comment(lib, "fmod.lib")

CMsSound::CMsSound(DWORD dwSoundCount)
: m_pSystem         (NULL)
, m_IsInit          (FALSE)
, m_nFmodInitMode   (0)
, m_IsStartupSound  (FALSE)
, m_dwSoundCount    (dwSoundCount)
{
    _ENTER_FUN_C;

    // FMOD���ͷ���ֵ
    FMOD_RESULT frRet = FMOD_OK;

    // ��ʼ����������Ϊ��
    m_ppSound = NEW lpSound[m_dwSoundCount];
    AssertEx(AL_CATAST, m_ppSound, _T("�ڴ治��"));
    
    // ��ʼ����������Ϊ��
    m_ppChannel = NEW lpChannel[m_dwSoundCount];
    AssertEx(AL_CATAST, m_ppChannel, _T("�ڴ治��"));

    m_lpfVolume = NEW FLOAT[m_dwSoundCount];
    AssertEx(AL_CATAST, m_lpfVolume, _T("�ڴ治��"));

    if (m_lpfVolume)
    {
        // ��ʼ��������������ΪĬ��ֵ
        FOR_x_TO(i, m_dwSoundCount)
        {
            m_lpfVolume[i] = GAME_NORMAL_VOLUME;
        }
    }
    else
    {
        AssertEx(AL_NORMAL, m_lpfVolume, _T("m_lpfVolumeΪ��"));
    }

    // ����ϵͳ
    frRet = System_Create(&m_pSystem);
    AssertEx(AL_NORMAL, FMOD_OK == frRet, _T("����FMODϵͳʧ��,����ʧ��"));

    _LEAVE_FUN_DISPOSE_END_C;
}

CMsSound::~CMsSound(VOID)
{
    _ENTER_FUN_C;

    // FMOD���ͷ���ֵ
    FMOD_RESULT frRet = FMOD_OK;

    // �ͷ�������Դ
    for (DWORD i = 0; i< m_dwSoundCount; i++)
    {
        if (m_ppChannel[i])
        {
            BOOL bPlaying = this->IsPlaying(i);
            if (bPlaying)
            {
                // ֹͣ����
                frRet = m_ppChannel[i]->stop();
                AssertEx(AL_NORMAL, FMOD_OK == frRet, _T("�ͷ�������Դʧ��,����ʧ��"));
                m_ppChannel[i] = NULL;
            }
        }
    }
    SAFE_DELETE_ARRAY(m_ppChannel);

    // �ͷ�������Դ
    for (DWORD i = 0; i< m_dwSoundCount; i++)
    {
        if (m_ppSound[i])
        {
            frRet = m_ppSound[i]->release();
            AssertEx(AL_NORMAL, FMOD_OK == frRet, _T("�ͷ�������Դʧ��,����ʧ��"));
            m_ppSound[i] = NULL;
        }
    }
    SAFE_DELETE_ARRAY(m_ppSound);

    // �ͷ�ϵͳ
    if (m_pSystem)
    {
        // �ر���������ϵͳ
        frRet = m_pSystem->close();
        AssertEx(AL_NORMAL, FMOD_OK == frRet, _T("�ر�FMODϵͳʧ��,����ʧ��"));

        // �ͷ���������ϵͳ��Դ
        frRet = m_pSystem->release();
        AssertEx(AL_NORMAL, FMOD_OK == frRet, _T("�ͷ���������ϵͳ��Դʧ��,����ʧ��"));

        // ��ϵͳָ���ÿ�
        m_pSystem = NULL;
    }

    SAFE_DELETE_ARRAY(m_lpfVolume);

    _LEAVE_FUN_DISPOSE_END_C;
}

// ��ʼ��������
BOOL CMsSound::Init(BOOL b3DSound)
{
    _ENTER_FUN_C;

    // FMOD���ͷ���ֵ
    FMOD_RESULT frRet = FMOD_OK;

    // ���ָ��3Dģʽ
    if (b3DSound)
    {
        // ��ȡ�������ģʽ
        m_nFmodInitMode = FMOD_INIT_DSOUND_HRTFNONE;
    }

    // ��ʼ��
    frRet = m_pSystem->init(m_dwSoundCount, m_nFmodInitMode, NULL);
    AssertEx(AL_NORMAL, FMOD_OK == frRet, _T("��ʼ��FMODϵͳʧ��,����ʧ��"));

    // ����ʼ���������
    m_IsInit = TRUE;
    m_IsStartupSound = TRUE;

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_END_C;
    return FALSE;
}

// ������
BOOL CMsSound::Open(DWORD dwIndex, CONST ACHAR* strSoundName, BOOL IsLoop)
{
    _ENTER_FUN_C;

    // ���ṩ��Ƶ����
    if (!m_IsStartupSound)
    {
        return TRUE;
    }

    // FMOD���ͷ���ֵ
    FMOD_RESULT frRet = FMOD_OK;

    // ����Ƶ�ļ���ģʽ
    INT FmodFlag = FMOD_HARDWARE;

    // �����3D����ģʽ
    if (m_nFmodInitMode == FMOD_INIT_DSOUND_HRTFFULL)
    {
        // ���ļ���3Dģʽ
        FmodFlag |= FMOD_3D;
    }

    // �Ƿ�ѭ��
    if (IsLoop)
    {
        // Ĭ��ѭ��ģʽ
        FmodFlag |= FMOD_LOOP_NORMAL;
    }

    // ����ѭ����ʽ
    else
    {
        // �ر�ѭ��
        FmodFlag |= FMOD_LOOP_OFF;
    }

    // ��ʼ��
    frRet = m_pSystem->createSound(
        strSoundName,
        FmodFlag,
        NULL,
        &m_ppSound[dwIndex]
    );

    AssertEx(AL_NORMAL, FMOD_OK == frRet, _T("createSoundʧ��,����ʧ��"));

    // ����ʼ���������
    m_IsInit = TRUE;

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_END_C;
    return FALSE;
}


// ��������
BOOL CMsSound::Play(DWORD dwIndex, BOOL bPaused, BOOL IsMid)
{
    _ENTER_FUN_C;

    // ���ṩ��Ƶ����
    if (!m_IsStartupSound)
    {
        return TRUE;
    }

    // FMOD���ͷ���ֵ
    FMOD_RESULT frRet = FMOD_OK;

    // ��������
    frRet = m_pSystem->playSound(
        FMOD_CHANNEL_REUSE,
        m_ppSound[dwIndex],
        BOOL_To_bool(bPaused),
        &m_ppChannel[dwIndex]);
    AssertEx(AL_NORMAL, FMOD_OK == frRet, _T("��������ʧ��,����ʧ��"));

    if (IsMid)
    {
        ::SleepEx(20, TRUE);
    }

    // ��������
    frRet = m_ppChannel[dwIndex]->setVolume(m_lpfVolume[dwIndex]);
    AssertEx(AL_NORMAL, FMOD_OK == frRet, _T("��������ʧ��,����ʧ��"));

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_END_C;
    return FALSE;
}

// ��ͣ����
BOOL CMsSound::Pause(DWORD dwIndex)
{
    _ENTER_FUN_C;

    // ���ṩ��Ƶ����
    if (!m_IsStartupSound)
    {
        return TRUE;
    }

    // FMOD���ͷ���ֵ
    FMOD_RESULT frRet = FMOD_OK;

    // ��ͣ����
    BOOL bPlaying = this->IsPlaying(dwIndex);
    if (bPlaying)
    {
        frRet = m_ppChannel[dwIndex]->setPaused(TRUE);
        AssertEx(AL_NORMAL, FMOD_OK == frRet, _T("��ͣ����ʧ��,����ʧ��"));
    }

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_END_C;
    return FALSE;
}

// ��������
BOOL CMsSound::Continue(DWORD dwIndex)
{
    _ENTER_FUN_C;

    // ���ṩ��Ƶ����
    if (!m_IsStartupSound)
    {
        return TRUE;
    }

    // FMOD���ͷ���ֵ
    FMOD_RESULT frRet = FMOD_OK;

    // ��ͣ����
    bool bPaused;
    m_ppChannel[dwIndex]->getPaused(&bPaused);
    if (bPaused)
    {
        // ��������
        frRet = m_ppChannel[dwIndex]->setPaused(FALSE);
        AssertEx(AL_NORMAL, FMOD_OK == frRet, _T("��������ʧ��,����ʧ��"));
    }

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_END_C;
    return FALSE;
}

// ֹͣ����
BOOL CMsSound::Stop(DWORD dwIndex)
{
    _ENTER_FUN_C;

    // ���ṩ��Ƶ����
    if (!m_IsStartupSound)
    {
        return TRUE;
    }

    // FMOD���ͷ���ֵ
    FMOD_RESULT frRet = FMOD_OK;

    // ���������������
    if (m_ppChannel[dwIndex])
    {
        BOOL bPlaying = this->IsPlaying(dwIndex);
        if (bPlaying)
        {
            // ֹͣ����
            frRet = m_ppChannel[dwIndex]->stop();
            AssertEx(AL_NORMAL, FMOD_OK == frRet, _T("ֹͣ����ʧ��,����ʧ��"));
        }
        // �������ÿ�
        m_ppChannel[dwIndex] = NULL;
    }

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_END_C;
    return FALSE;
}

// ��������
BOOL CMsSound::SetVolume(DWORD dwIndex, FLOAT fVolume)
{
    _ENTER_FUN_C;

    // ���ṩ��Ƶ����
    if (!m_IsStartupSound)
    {
        return TRUE;
    }

    // FMOD���ͷ���ֵ
    FMOD_RESULT frRet = FMOD_OK;

    // �����������
    if (fVolume > VOLUME_MAX_VALUE)
    {
        // �����������
        fVolume = VOLUME_MAX_VALUE;
    }
    // ���������С
    else if (fVolume < VOLUME_MIN_VALUE)
    {
        // ������С����
        fVolume = VOLUME_MIN_VALUE;
    }

    // ��������
    m_lpfVolume[dwIndex] = fVolume;
    frRet = m_ppChannel[dwIndex]->setVolume(m_lpfVolume[dwIndex]);
    AssertEx(AL_NORMAL, FMOD_OK == frRet, _T("��������ʧ��,����ʧ��"));

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_END_C;
    return FALSE;
}

// ��������
BOOL CMsSound::AddVolume(DWORD dwIndex)
{
    _ENTER_FUN_C;

    // ���ṩ��Ƶ����
    if (!m_IsStartupSound)
    {
        return TRUE;
    }

    // FMOD���ͷ���ֵ
    FMOD_RESULT frRet = FMOD_OK;

    // ��������Ѿ����
    if (m_lpfVolume[dwIndex] >= VOLUME_MAX_VALUE)
    {
        // ���سɹ�
        return TRUE;
    }

    // ��������
    m_lpfVolume[dwIndex] += VOLUME_CHANGE_ONCE_VALUE;

    // ��������
    frRet = m_ppChannel[dwIndex]->setVolume(m_lpfVolume[dwIndex]);
    AssertEx(AL_NORMAL, FMOD_OK == frRet, _T("��������ʧ��,����ʧ��"));

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_END_C;
    return FALSE;
}

// ��������
BOOL CMsSound::SubVolume(DWORD dwIndex)
{
    _ENTER_FUN_C;

    // ���ṩ��Ƶ����
    if (!m_IsStartupSound)
    {
        return TRUE;
    }

    // FMOD���ͷ���ֵ
    FMOD_RESULT frRet = FMOD_OK;

    // ��������Ѿ����
    if (m_lpfVolume[dwIndex] <= VOLUME_MIN_VALUE)
    {
        // ���سɹ�
        return TRUE;
    }

    // ��������
    m_lpfVolume[dwIndex] -= VOLUME_CHANGE_ONCE_VALUE;

    // ��������
    frRet = m_ppChannel[dwIndex]->setVolume(m_lpfVolume[dwIndex]);
    AssertEx(AL_NORMAL, FMOD_OK == frRet, _T("��������ʧ��,����ʧ��"));

    // ���سɹ�
    return TRUE;

    _LEAVE_FUN_DISPOSE_END_C;
    return FALSE;
}

// �Ƿ����ڲ���
BOOL CMsSound::IsPlaying(DWORD dwIndex)
{
    _ENTER_FUN_C;

    // ���ṩ��Ƶ����
    if (!m_IsStartupSound)
    {
        return FALSE;
    }

    // �������ͷ���ֵ
    bool bRet = TRUE;
    if (m_ppChannel[dwIndex])
    {
        m_ppChannel[dwIndex]->isPlaying(&bRet);
        return bRet;
    }
    else
    {
         return FALSE;
    }

    _LEAVE_FUN_DISPOSE_END_C;
    return FALSE;
}

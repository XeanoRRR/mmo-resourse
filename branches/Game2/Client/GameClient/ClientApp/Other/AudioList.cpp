#include "stdafx.h"
#include "Other/AudioList.h"
#include "../Audio/FmodAudioDef.h"
#include "Player.h"
#include "Goods/Goods.h"
#include "Monster.h"
#include "Citygate.h"
#include "../GameClient/Game.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


HANDLE hAudioEngineThreadHandle=0;		//��������ϵͳ״̬���̵߳��߳̾��
bool bAndioEngineRunning=true;			//ָʾ��������ϵͳ״̬�߳��Ƿ�����
// ��������״̬���߳�
unsigned __stdcall AudioEngineThreadFunc( void* pArguments ) 
{
	CAudioList *pAudioList=(CAudioList *)pArguments;
	while( bAndioEngineRunning )
	{
		pAudioList->UpdateAudio();
		Sleep(50);
	}
	_endthreadex( 0 );
    return 0; 
} 

//������������
//����˵��
//	��
//����˵��
//	��
CAudioList::CAudioList(void) : m_strMusicFile("")
{
	m_pAudioEngine=CFmodEngine::GetInstance(g_hWnd);
	if(m_pAudioEngine==NULL)
	{
		MessageBox(g_hWnd,"������������ʧ�ܣ�","����",MB_ICONERROR);
		return;
	}
	m_lPlayerInterTime = 0;
	m_bPlayMonsterOppear = true;
	LoadSetup("data\\other.ini");
	m_bFadeState = false;
	
	unsigned threadID;
	hAudioEngineThreadHandle = (HANDLE)_beginthreadex(NULL,0,AudioEngineThreadFunc,this,0,&threadID);
}

//�ͷ�ɾ����������
//����˵��
//	��
//����˵��
//	��
CAudioList::~CAudioList(void)
{
	if(m_pAudioEngine==NULL)return;
	bAndioEngineRunning=false;
	WaitForSingleObject(hAudioEngineThreadHandle,100);
	CloseHandle(hAudioEngineThreadHandle);
	delete m_pAudioEngine;
}

//���ų�������
//����˵��
//	pShape:CShape����	lType:CShape����	wAction:��Ӧ�Ķ���	vRole:����λ��	loop:�Ƿ�ѭ��	nLoops:�ظ����Ŵ���
//����˵��
//	���ز��Ÿ�������channel,-1��ʾ����ʧ��
int CAudioList::Play(CShape * pShape, long lType, WORD wAction, Vector3 vRole, bool loop, int nLoops)
{
	if(m_pAudioEngine==NULL)return 0;
	static char *strAction[] = {"stand","idle","walk", "run", "hurt", "inton","block","parry","miss","mergrim","dead","open","serhurt"};
	char strbuf[MAX_PATH]="";
	
	bool bEnv = false;
	//�����˺�
	enum 
	{
		ACT_HURT01 = 4,			// ��ͨ
		ACT_HURT02 = 12,		// ���ػ�	
	};
	switch(lType)
	{
	// �����Ч
	case TYPE_PLAYER:
		{
			CPlayer * pPlayer = dynamic_cast<CPlayer*>(pShape);
			if( wAction < 12 && wAction != CShape::ACT_INTONATE &&
				wAction != CShape::ACT_MEGRIM && wAction != CShape::ACT_OPEN)
			{
				sprintf(strbuf,	"sounds/players/p%03d%s.wav", pPlayer->GetGraphicsID(), strAction[wAction]);
			}
		}
		break;

	// ��Ʒ��Ч
	case TYPE_GOODS:
		{
			CGoods * pGoods = dynamic_cast<CGoods*>(pShape);
			if (pGoods==NULL)
				sprintf(strbuf,	"sounds/equipments/%05d.wav", wAction);

			switch ( wAction )
			{
				// ����
			case CGoods::GOOD_DROP:
				sprintf(strbuf,	"sounds/equipments/%05d.wav", pGoods->GetProperty()->dwSound);
				break;

				// ���ױ����˺��Լ���ͨ�����Ӷ�
			case CGoods::GOOD_NORMAL:
				{
					CGoodsList::tagBaseProperty * t = pGoods->GetProperty();
					sprintf(strbuf,	"sounds/equipments/%05d.wav", pGoods->GetProperty()->dwSoundID1);
				}
				break;

				// ���ױ��˺��Լ����������Ӷ�
			case CGoods::GOOD_EXCEPT:
				{
					CGoodsList::tagBaseProperty * p = pGoods->GetProperty();
					sprintf(strbuf,	"sounds/equipments/%05d.wav", pGoods->GetProperty()->dwSoundID2);
				}
				break;
			}
		}
		break;

	// ������Ч
	case TYPE_MONSTER:
		{
			CMonster * pMonster = dynamic_cast<CMonster*>(pShape);
			if (pMonster==NULL)
				return 0;

			if (wAction == 0xFFFF&&m_bPlayMonsterOppear)	// ������Ч
			{
				sprintf(strbuf,	"sounds/monsters/m%03dappear.wav", pMonster->GetSoundID());
			}
			//����ֻ�������˺�������Ч
			else if(wAction==CShape::ACT_BEATTACK || wAction==CShape::ACT_DIED || wAction == ACT_HURT02)	//������Ч 
			{
				sprintf(strbuf,	"sounds/monsters/m%03d%s.wav", pMonster->GetSoundID(), strAction[wAction]);
			}
			else if(wAction == 0xFFF0)
			{
				sprintf(strbuf,	"sounds/monsters/m%03dattack.wav", pMonster->GetSoundID());
			}
		}
		break;

	// �ر���Ч
	case TYPE_REGION:
		{
			if (pShape == NULL)
				return 0;
			CPlayer * pPlayer = dynamic_cast<CPlayer*>(pShape);
			if(pPlayer == NULL)
				return 0;
			CClientRegion::tagCell *pCell = GetGame()->GetRegion()->GetCell(pShape->GetTileX(),pShape->GetTileY());
			if (pCell)
			{
				//sprintf(strbuf,"sounds/regions/r%03d%s.wav", pCell->lMaterial,strAction[wAction]);
				//���������Ƿ��ڳ���״̬������Ӧ����Ч����������ڳ���״̬����Ҫ�Ȼ����������͡�
				if(!pPlayer->IsRide())
				{
					long Id = pPlayer->GetGraphicsID();
	                 sprintf(strbuf,"sounds/regions/r%03d%s.wav", pCell->lMaterial,strAction[wAction]);
				}else{ 
					 CGoods *goodhuose = pPlayer->GetEquip(CPlayer::EQUIP_RIDER);
					 if(NULL == goodhuose)
						 return 0;
					 long houseIndex = goodhuose->GetIndex();
					 //��������������Ӧ�Ķ������ͣ��Դ������ֲ�ͬ�������Ч��
					 //DWORD ActType = CGoodsList::GetPropertyByEquipID(houseIndex)->BaseProperty.dwWeaponActType;
					 CGoods *pGoods = NULL;
					 pGoods=new CGoods();
				     CGoodsList::FillGoodsInfo(houseIndex,pGoods);
					 DWORD ActType = pGoods->GetProperty()->dwWeaponActType;
					 //SAFE__DELETE(pGoods);
					 delete pGoods;
					 sprintf(strbuf,"sounds/regions/rh%02d%03d%s.wav", ActType,pCell->lMaterial,strAction[wAction]);

				}
			}
		}
		break;

	// ������Ч
	case TYPE_SKILL:
		{
			sprintf(strbuf,	"sounds/skills/%03d.wav", wAction);
		}
		break;

	case TYPE_CITYGATE:
		{
			CCityGate * pCityGate = dynamic_cast<CCityGate*>(pShape);
			if(pCityGate == NULL)
				return 0;

			if(wAction == CShape::ACT_OPEN) wAction = 12;
			sprintf(strbuf,	"sounds/citygate/g%03d%s.wav", pCityGate->GetGraphicsID(), strAction[wAction]);
		}
		break;
	default:
		return 0;
	}

	return Play3DSound(strbuf,nLoops,vRole.x,vRole.y,vRole.z);
}

//���ų�������
//����˵��
//	pSound:����������ʾ�Ľṹ��	lRegionID:������ID	loop:�Ƿ�ѭ��	nLoops:�ظ����ŵĴ���
//����˵��
//	���Ÿ�������channel,-1��ʾ����ʧ��
int CAudioList::Play(CClientRegion::REGION_SOUND_PTR pSound,long lRegionID,bool loop, int nLoops)
{
	if(m_pAudioEngine==NULL)return 0;
	if(loop)nLoops=-1;
	char filename[MAX_PATH]=""; 
	sprintf(filename,"sounds/regions/%d/%d.wav",lRegionID,pSound->Sound.wID);

	Log4c::Trace(ROOT_MODULE,"Start Play Region Sound.(filename:%s).",filename);

	return Play3DSound(filename,nLoops,pSound->zpos,pSound->Sound.fHeight,pSound->xpos);
}

//���������ļ�
//����˵��
//	filename:�ļ�����	loop:�Ƿ�ѭ��	bEnv:�Ƿ񻷾�����(����2D����)	nLoops:�ظ�����
//����˵��
//	���ز��Ÿ�������channel,-1��ʾ����ʧ��
int CAudioList::Play(const char* filename, bool loop, bool bEnv, int nLoops)
{
	if(m_pAudioEngine==NULL)return 0;
	if(filename==0)return -1;
	if(loop)nLoops=-1;
	if( bEnv )
	{
		if(IsPlaying(filename))
		{
			return -1;
		}else{
		    return Play2DSound(filename);
		}

	}
	else
	{
		return Play3DSound(filename,nLoops,0,0,0);
	}
}

//����3D����
//����˵��
// filename:�ļ��� fX,fY,fHeight:λ����Ϣ	loop:�Ƿ�ѭ��	nLoops:�ظ�����
//����˵��
//	���ز���������channel,-1��ʾ����ʧ��
int CAudioList::Play(const char* filename, float fX, float fY, float fHeight, bool loop, int nLoops)
{
	if(m_pAudioEngine==NULL)return 0;
	if(filename==0)return -1;
	if(loop)nLoops=-1;
	return this->Play3DSound(filename,nLoops,fY, fHeight, fX);
}

//�������������Լ�����ά�������������ֵ�״̬
//����˵��
//	��
//����˵��
//	��
//ע������
//	����ϵͳ��Ҫ���ڸ���״̬,��˿ͻ�������붨�ڵ���,������ר�ŵ��߳�������,ʱ����������50MS
//�����Ľ��뽥���Լ��Զ����ն�������״̬����
void CAudioList::UpdateAudio()
{
	if(m_pAudioEngine==NULL)return;
	m_pAudioEngine->Update();
	//char msg[32];
	//sprintf(msg,"sounds:%d",m_pAudioEngine->GetSoundsCount());
	//SetWindowText(g_hWnd,msg);
}

void CAudioList::Run()
{
	if(m_pAudioEngine)
		m_pAudioEngine->Run();
	CountMonsterOppear();
}

//��������
//����˵��
//	filename:�����ļ��� loops:���ŵĴ���,С��1��ʾʼ��ѭ������
//����˵��
//	��	�����֪�������Ƿ�ɹ�����,����IsPlaying
void CAudioList::PlayMusic(const char *filename,int loops)
{
	if(m_pAudioEngine==NULL)return;
	if(filename==0)return;
	CFmodMusic *pMusic=m_pAudioEngine->GetMusicEx(filename,FSOUND_HW2D);
	if(pMusic==0)return;
	int channel = pMusic->Play(loops);
	if(channel == -1 && FSOUND_GetError() == FMOD_ERR_PLAY)
	{
		if( pMusic->GetMode() == FSOUND_HW2D )
		{
			m_pAudioEngine->SetHW2DMaxChnnels(0);
		}
	}
}


//���벥������
//����˵��
//	filename:�����ļ���	time:���������ʱ��	loops:���ŵĴ���,С��1��ʾʼ��ѭ������
//����˵��
//	��,��time��ʱ����,���ֵ�������0�������󵽵�ǰ���ֵ�����,�仯Ƶ��������������
//״̬����Ƶ��.�����֪�������Ƿ�ɹ�����,����IsPlaying
void CAudioList::FadeInMusic(const char *filename,unsigned int time,int loops)
{
	if(m_pAudioEngine==NULL)return;
	if(filename==0)return;
	CFmodMusic *pMusic=m_pAudioEngine->GetMusicEx(filename,FSOUND_HW2D);
	if(pMusic==0)return;
	int channel=pMusic->FadeIn(time,loops);
	pMusic->SetVolume(0);
	//int channel = pMusic->Play(loops);
	if( channel != -1)
	{
		int nMaxVolume=max(0,min(255,(int)(Audio::s_fMusicVolume*255)));
		m_pAudioEngine->Fade(pMusic->GetCurChannel(),true,nMaxVolume,time);
	}
	else
	{
		if(FSOUND_GetError() == FMOD_ERR_PLAY && pMusic->GetMode() == FSOUND_HW2D )
		{
			m_pAudioEngine->SetHW2DMaxChnnels(0);
		}
	}
}

//������������
//����˵��
//	filename:�����ļ���	time:����������ʱ��	loops:���ŵĴ���,С��1��ʾʼ��ѭ������
//����˵��
//	��,��time��ʱ����,���ֵ������ӵ�ǰ��������������μ�С����0��ֹͣ,�仯Ƶ��
//������������״̬���µ�Ƶ��.�����֪�������Ƿ�ɹ�����,����IsPlaying
void CAudioList::FadeOutMusic(const char *filename,unsigned int time,int loops)
{
	if(m_pAudioEngine==NULL)return;
	if(filename==0)return;
	CFmodMusic *pMusic=m_pAudioEngine->FindMusic(filename);
	if(pMusic==0 || pMusic->IsAlive() == false)return;

	pMusic->FadeOut(time,loops);
	int nMaxVolume=max(0,min(255,(int)(Audio::s_fMusicVolume*255)));
	//m_pAudioEngine->Fade(pMusic->GetCurChannel(),false,nMaxVolume,time);
}
//���䲥������
//����˵��
//	filename:�����ļ���	time:����������ʱ��	loops:���ŵĴ���,С��1��ʾʼ��ѭ������
//����˵��
//	��,��time��ʱ����,���ֵ������ӵ�ǰ��������������μ�С����0��ֹͣ,�仯Ƶ��
//������������״̬���µ�Ƶ��.�����֪�������Ƿ�ɹ�����,����IsPlaying
void CAudioList::FadeMusic(const char *filename,unsigned int time,float Volume,int loops)
{
	if(m_pAudioEngine==NULL)return;
	if(filename==0)return;
	CFmodMusic *pMusic=m_pAudioEngine->FindMusic(filename);
	if(pMusic==0 || pMusic->IsAlive() == false)return;

	//if(pMusic->GetPlayingType()==Audio::PLAY_FADEDOWN||pMusic->GetPlayingType()==Audio::PLAY_FADEUP)
		//return;

	pMusic->Fade(time,Volume,loops);
	int nMaxVolume=max(0,min(255,(int)(Audio::s_fMusicVolume*255)));
	//m_pAudioEngine->Fade(pMusic->GetCurChannel(),false,nMaxVolume,time);
}

void CAudioList::SetFadeState(bool b)
{
	//CFmodMusic *pMusic=m_pAudioEngine->FindMusic(GetBackgroundMusic());
	//if(pMusic==0 || pMusic->IsAlive() == false)return;
	//if(pMusic->GetPlayingType()==Audio::PLAY_FADEIN||pMusic->GetPlayingType()==Audio::PLAY_FADEOUT) return;
	m_bFadeState = b;
}


//ֹͣһ���Ѿ��ڲ��ŵ�����
//����˵��
//	filename:�����ļ���
//����˵��
//	��
void CAudioList::StopMusic(const char *filename)
{
	if(m_pAudioEngine==NULL)return;
	if(filename==0)return;
	CFmodMusic *pMusic=m_pAudioEngine->FindMusic(filename);
	if(pMusic==0)return;
	pMusic->Stop();
}

//�ͷ�һ���Ѿ��ڲ��ŵ�����
//����˵��
//	filename:�����ļ���
//����˵��
//	��
void CAudioList::ReleaseMusic(const char *filename)
{	
	if(m_pAudioEngine==NULL)return;
	if(filename==0)return;
	CFmodMusic *pMusic=m_pAudioEngine->FindMusic(filename);
	if(pMusic==0)return;
	pMusic->Stop();
}

//����3D����
//����˵��
//	filename:�����ļ���	loops:���Ŵ���,С��1��ʾʼ��ѭ������ x,y,z:�����Ŀռ�λ��
//����˵��
//	����ɹ�����,���ز���������channel,���򷵻�-1
int CAudioList::Play3DSound(const char *filename,int loops,float x,float y,float z)
{
	if(m_pAudioEngine==NULL)return -1;
	if(filename==0)return -1;
	
	//����ֻ����һ�ε��������ж����Ƿ��ڿɲ��ŷ�Χ��
	if(loops==1)
	{
		float dx=GetGame()->GetMainPlayer()->GetPosX()-z,dy=GetGame()->GetMainPlayer()->GetPosY()-x;
		if(abs(dx) > GetGame()->GetSetup()->l3DSoundsRange|| abs(dy) > GetGame()->GetSetup()->l3DSoundsRange)
		{
			return -1;
		}
	}
	CFmodSound *pSound=m_pAudioEngine->GetSoundEx(filename,FSOUND_HW3D);

	if(pSound==0)return -1;
	pSound->SetPosition(x,y,z);
	int channel=pSound->Play(loops);
	//������Ŵ���,
	if(channel == -1 && FSOUND_GetError() == FMOD_ERR_PLAY)
	{
		if( pSound->GetMode() == FSOUND_HW3D )
		{
			m_pAudioEngine->SetHW3DMaxChnnels(0);
		}
		if( pSound->GetMode() == FSOUND_HW2D )
		{
			m_pAudioEngine->SetHW2DMaxChnnels(0);
		}
	}
	int length = pSound->GetAudioLength();
	if(length<1000)
	{
		pSound->Release(false);
		channel=-1;
	}
	return channel;	
}

//���벥��3D����
//����˵��
//	filename:�����ļ��� x,y,z:�����Ŀռ�λ�� time:�����ʱ�� loops:���Ŵ���,С��1��ʾʼ��ѭ������
//����˵��
//	����ɹ�����,���ز���������channel,���򷵻�-1.��time��ʱ����,����������0����ǰ���������������
//����,�仯Ƶ��������������״̬���µ�Ƶ��
//int CAudioList::FadeIn3DSound(const char *filename,float x,float y,float z,unsigned int time,int loops)
//{
//	/*if(m_pAudioEngine==NULL)return 0;
//	if(filename==0)return -1;
//	CFmodSound *pSound=m_pAudioEngine->GetSoundEx(filename,FSOUND_HW3D);
//	if(pSound==0) return -1;
//	pSound->SetPosition(x,y,z);
//	int channel=pSound->FadeIn(time,loops);
//	return channel;*/
//	return -1;
//}
//
//
////��������3D����
////����˵��
////	filename:�����ļ��� x,y,z:�����Ŀռ�λ�� time:������ʱ�� loops:���Ŵ���,С��1��ʾʼ��ѭ������
////����˵��
////	����ɹ�����,���ز���������channel,���򷵻�-1.��time��ʱ����,���������ӵ�ǰ�������������0����
////��С��ֹͣ,�仯Ƶ��������������״̬���µ�Ƶ��
//int CAudioList::FadeOut3DSound(const char *filename,float x,float y,float z,unsigned int time,int loops)
//{
//	/*if(m_pAudioEngine==NULL)return 0;
//	if(filename==0)return -1;
//	CFmodSound *pSound=m_pAudioEngine->FindSound(filename,-1);
//	if(pSound==0)return 0;
//	pSound->SetPosition(x,y,z);
//	int channel=pSound->FadeOut(time,loops);
//	return channel;*/
//	return -1;
//}

//ֹͣ3D����
//����˵��
//	filename:�����ļ��� channel:���Ÿ�������channel
//����˵��
//	�� ������ڸ��ļ���channel��3D������ֹͣ
void CAudioList::Stop3DSound(const char *filename,int channel)
{
	if(m_pAudioEngine==NULL)return;
	if(filename==0)return;
	CFmodSound *pSound=m_pAudioEngine->FindSound(filename,channel);
	if(pSound==0)return;
	pSound->Stop();
}

//ֹͣ3D����
//����˵��
//	filename:�����ļ��� channel:���Ÿ�������channel
//����˵��
//	�� ������ڸ��ļ���channel��3D������ֹͣ
void CAudioList::Stop3DSound(int channel)
{
	if(m_pAudioEngine==NULL)return;
	CFmodSound *pSound=m_pAudioEngine->FindSound(channel);
	if(pSound==0)return;
	pSound->Stop();
}

//ֹͣ��������
void CAudioList::StopSpeech( const char *filename,int channel )
{
	if(m_pAudioEngine==NULL)return;
	if(filename==0)return;
	CFmodSpeech *pSpeech=m_pAudioEngine->FindSpeech(filename,-1);
	if(pSpeech==0)return;
	pSpeech->Stop();
}

//�ͷ�3D����
//����˵��
//	filename:�����ļ��� channel:���Ÿ�������channel
//����˵��
//	�� ������ڸ��ļ���channel��3D�������ͷ�
void CAudioList::Release3DSound(const char *filename,int channel)
{
	if(m_pAudioEngine==NULL)return;
	if(filename==0)return;
	CFmodSound *pSound=m_pAudioEngine->FindSound(filename,channel);
    if(pSound)
		pSound->Stop();
}

//����3D������λ��
//����˵��
//	filename:�����ļ��� channel:���Ÿ�������channel x,y,z:λ������
//����˵��
//	�� ������ڸ��ļ���channel��3D����������Ϊx,y,z������
//void CAudioList::Set3DSoundPosition(const char *filename,int &channel,float x,float y,float z)
//{
//	if(m_pAudioEngine==NULL)return;
//	CFmodSound *pSound=m_pAudioEngine->FindSound(filename,channel);
//	if(pSound==0)return;
//	pSound->SetPosition(x,y,z);
//}

//����2D����
//����˵��
//	filename:�����ļ��� loops:���Ŵ���,С��1��ʾʼ��ѭ������
//����˵��
//	������ųɹ����ز��Ÿ�������channel
int CAudioList::Play2DSound(const char *filename,int loops)
{
	if(m_pAudioEngine==NULL)
		return 0;

	if(filename==NULL)
		return -1;
	if(IsPlaying(filename,-1)) 
		return -1;

	CFmodSound *pSound=m_pAudioEngine->GetSoundEx(filename,FSOUND_HW2D);
	if (pSound == NULL)	return -1;
	int channel = pSound->Play(loops);
	//������Ŵ���,
	if(channel == -1 && FSOUND_GetError() == FMOD_ERR_PLAY)
	{
		if( pSound->GetMode() == FSOUND_HW3D )
		{
			m_pAudioEngine->SetHW3DMaxChnnels(0);
		}
		if( pSound->GetMode() == FSOUND_HW2D )
		{
			m_pAudioEngine->SetHW2DMaxChnnels(0);
		}
	}
	return channel;
}

//��������
//����˵��
//	filename:�����ļ��� loops:���Ŵ���,С��1��ʾʼ��ѭ������
//����˵��
//	������ųɹ����ز��Ÿ�������channel
int CAudioList::PlaySpeech(const char *filename,int loops)
{
	if(m_pAudioEngine==NULL)
		return 0;

	if(filename==NULL)
		return -1;

	CFmodSpeech *pSpeech=m_pAudioEngine->GetSpeechEx(filename,FSOUND_HW2D);
	if (pSpeech == NULL)	return -1;
	int channel = pSpeech->Play(loops);
	////������Ŵ���,
	//if(channel == -1 && FSOUND_GetError() == FMOD_ERR_PLAY)
	//{
	//	if( pSound->GetMode() == FSOUND_HW3D )
	//	{
	//		m_pAudioEngine->SetHW3DMaxChnnels(0);
	//	}
	//	if( pSound->GetMode() == FSOUND_HW2D )
	//	{
	//		m_pAudioEngine->SetHW2DMaxChnnels(0);
	//	}
	//}
	return channel;
}

//����3D����
//����˵��
//	filename:�����ļ���	loops:���Ŵ���,С��1��ʾʼ��ѭ������ x,y,z:�����Ŀռ�λ��
//����˵��
//	����ɹ�����,���ز���������channel,���򷵻�-1
int CAudioList::Play3DSpeech(const char *filename,int loops,float x,float y,float z)
{
	if(m_pAudioEngine==NULL)return -1;
	if(filename==0)return -1;
	
	//����ֻ����һ�ε��������ж����Ƿ��ڿɲ��ŷ�Χ��
	if(loops==1)
	{
		//int dx=GetGame()->GetMainPlayer()->GetPosX()-z,dy=GetGame()->GetMainPlayer()->GetPosY()-x;
		float dx=GetGame()->GetMainPlayer()->GetPosX()-x,dy=GetGame()->GetMainPlayer()->GetPosY()-y;
		if(abs(dx)>GetGame()->GetSetup()->l3DSoundsRange||abs(dy)>GetGame()->GetSetup()->l3DSoundsRange)
		{
			return -1;
		}
	}
	CFmodSpeech *pSpeech=m_pAudioEngine->GetSpeechEx(filename,FSOUND_HW3D);
	if(pSpeech==0)return -1;
	pSpeech->SetPosition(y,z,x);
	int channel=pSpeech->Play(loops);
	//������Ŵ���,
	if(channel == -1 && FSOUND_GetError() == FMOD_ERR_PLAY)
	{
		if( pSpeech->GetMode() == FSOUND_HW3D )
		{
			m_pAudioEngine->SetHW3DMaxChnnels(0);
		}
		if( pSpeech->GetMode() == FSOUND_HW2D )
		{
			m_pAudioEngine->SetHW2DMaxChnnels(0);
		}
	}
	int length = pSpeech->GetAudioLength();
	if(length<1000)
	{
		pSpeech->Release(false);
		channel=-1;
	}
	return channel;	
}

//���벥��2D����
//����˵��
//	filename:�����ļ��� time:�����ʱ�� loops:���Ŵ���,С��1��ʾʼ��ѭ������
//����˵��
//	������ųɹ����ز��Ÿ�������channel ��time��ʱ����,����������0����ǰ���������������
//����,�仯Ƶ��������������״̬���µ�Ƶ��
int CAudioList::FadeIn2DSound(const char *filename,unsigned int time,int loops)
{
	if(m_pAudioEngine==NULL)return 0;
	if(filename==0)return -1;
	CFmodSound *pSound=m_pAudioEngine->GetSoundEx(filename,FSOUND_HW2D);
	if(pSound==0)return -1;
	pSound->SetVolume(0);
	int channel=pSound->Play(loops);
	if(channel != -1)
	{
		int nMaxVolume=max(0,min(255,(int)(Audio::s_fSoundVolume*255)));
		m_pAudioEngine->Fade(pSound->GetCurChannel(),true,nMaxVolume,time);
	}
	else
	{
		//������Ŵ���,
		if( FSOUND_GetError() == FMOD_ERR_PLAY )
		{
			if( pSound->GetMode() == FSOUND_HW3D )
			{
				m_pAudioEngine->SetHW3DMaxChnnels(0);
			}
			if( pSound->GetMode() == FSOUND_HW2D )
			{
				m_pAudioEngine->SetHW2DMaxChnnels(0);
			}
		}
	}
	return channel;
}

//���벥��2D����
//����˵��
//	filename:�����ļ��� time:�����ʱ�� loops:���Ŵ���,С��1��ʾʼ��ѭ������
//����˵��
//	������ųɹ����ز��Ÿ�������channel ��time��ʱ����,���������ӵ�ǰ�������������0����
//��С��ֹͣ,�仯Ƶ��������������״̬���µ�Ƶ��
int CAudioList::FadeOut2DSound(const char *filename,unsigned int time,int loops)
{
	if(m_pAudioEngine==NULL)return 0;
	if(filename==0)return -1;
	CFmodSound *pSound=m_pAudioEngine->FindSound(filename,-1);
	if(pSound==0 || pSound->IsAlive() == false)	return -1;

	int nMaxVolume=max(0,min(255,(int)(Audio::s_fSoundVolume*255)));
	m_pAudioEngine->Fade(pSound->GetCurChannel(),false,nMaxVolume,time);

	return pSound->GetCurChannel();
}

//ֹͣ2D����
//����˵��
//	filename:�����ļ���
//����˵��
//	��������ڸ��ļ���2D������ֹͣ
//void CAudioList::Stop2DSound(const char *filename)
//{
//	if(m_pAudioEngine==NULL)return;
//	if(filename==0)return;
//	CFmodSound *pSound=m_pAudioEngine->FindSound(filename,-1);
//	if(pSound==0)return;
//	pSound->Stop();
//}

//�ͷ�2D����
//����˵��
//	filename:�����ļ���
//����˵��
//	��������ڸ��ļ���2D�������ͷ�
void CAudioList::Release2DSound(const char *filename)
{
	if(m_pAudioEngine==NULL)return;
	if(filename==0)return;
	CFmodSound *pSound=m_pAudioEngine->FindSound(filename,-1);
	if(pSound==0)return;
	pSound->Release();
}

//�ж�һ�������Ƿ��ڲ���״̬
//����˵��
//	filename:�����ļ��� channel:����������channel,-1��ʾ���ֻ���2D����,Ĭ��ֵΪ-1
//����˵��
//	�������������������ڲ��ŷ�����,���򷵻ؼ�
bool CAudioList::IsPlaying(const char *filename,int channel)
{
	if(m_pAudioEngine==NULL)return false;
	Audio *pAudio=NULL;
	pAudio=m_pAudioEngine->FindSound(filename,channel);
	if(pAudio)
	{
		return pAudio->IsPlaying();
	}
	pAudio=m_pAudioEngine->FindMusic(filename);
	if(pAudio)
	{
		return pAudio->IsPlaying();
	}
	return false;
}

//��������ϵͳ�����ֵ�����
//����˵��
//	fVolume:������С,��Ч��Χ��0��1,���������Χȡ0(С��0)��1(����1)
//����˵��
//	��
void CAudioList::SetMusicVolume(float fVolume)
{
	if(m_pAudioEngine==NULL)return;
	m_pAudioEngine->SetMusicVolume(fVolume);
}

//��������ϵͳ�����ֵ�����
//����˵��
//	fVolume:������С,��Ч��Χ��0��1,���������Χȡ0(С��0)��1(����1)
//����˵��
//	��
void CAudioList::SetSpeechVolume(float fVolume)
{
	if(m_pAudioEngine==NULL)return;
	m_pAudioEngine->SetSpeechVolume(fVolume);
}

//��������ϵͳ�������Ƿ��
//����˵��
//	b:����
//����˵��
//	��
void CAudioList::SetMusicEnabled(bool b)
{
	if(m_pAudioEngine==NULL)return;
	m_pAudioEngine->SetMusicEnabled(b);
	if(b&&!IsPlaying(GetBackgroundMusic()))
	{
		FadeInMusic(GetBackgroundMusic(),5000);
	}
}

//��������ϵͳ�������Ƿ��
//����˵��
//	b:����
//����˵��
//	��
void CAudioList::SetSoundEnabled(bool b)
{
	if(m_pAudioEngine==NULL)return;
	m_pAudioEngine->SetSoundEnabled(b);
}

//��������ϵͳ�������Ƿ��
//����˵��
//	b:����
//����˵��
//	��
void CAudioList::SetSpeechEnabled(bool b)
{
	if(m_pAudioEngine==NULL)return;
	m_pAudioEngine->SetSpeechEnabled(b);
}

//��������ϵͳ������������
//����˵��
//	fVolume:������С,��Ч��Χ��0��1,���������Χȡ0(С��0)��1(����1)
//����˵��
//	��
void CAudioList::SetSoundVolume(float fVolume)
{
	if(m_pAudioEngine==NULL)return;
	m_pAudioEngine->SetSoundVolume(fVolume);
}

//��������ϵͳ������λ��
//����˵��
//	pos:λ����������
//����˵��
//	��
//ע������
//	pos����Ϊ��,���ǲ�������Ч������߳���С��3!!!
void CAudioList::SetListenerPosition(float *pos)
{
	if(m_pAudioEngine==NULL)return;
	m_pAudioEngine->SetListenerPosition(pos);
}

//��������ϵͳ������λ��
//����˵��
//	x,y,z:����
//����˵��
//	��
void CAudioList::SetListenerPosition(float x,float y,float z)
{
	if(m_pAudioEngine==NULL)return;
	m_pAudioEngine->SetListenerPosition(x,y,z);
}

//��������ϵͳ�������ٶ�����
//����˵��
//	vel:��ʾ�ٶ�����������
//����˵��
//	��
//ע������
//	vel����Ϊ��,���ǲ�������Ч������߳���С��3!!!
void CAudioList::SetListenerVelocity(float *vel)
{
	if(m_pAudioEngine==NULL)return;
	m_pAudioEngine->SetListenerVelocity(vel);
}

//��������ϵͳ�������ٶ�����
//����˵��
//	x,y,z:����
//����˵��
//	��
void CAudioList::SetListenerVelocity(float x,float y,float z)
{
	if(m_pAudioEngine==NULL)return;
	m_pAudioEngine->SetListenerVelocity(x,y,z);
}

//��������ϵͳ�����߷���
//����˵��
//	face:��ǰ�ķ������� up:���ϵķ�������
//����˵��
//	��
//ע������
//	face��up����Ϊ��,���ǲ�������Ч������߳���С��3!!!
void CAudioList::SetListenerOritation(float *face,float *up)
{
	if(m_pAudioEngine==NULL)return;
	m_pAudioEngine->SetListenerOritation(face,up);
}

//��������ϵͳ�����߷���
//����˵��
//	facex,facey,facez:��ǰ�ķ������� upx,upy,upz:���ϵķ�������
//����˵��
//	��
void CAudioList::SetListenerOritation(float facex,float facey,float facez,float upx,float upy,float upz)
{
	if(m_pAudioEngine==NULL)return;
	m_pAudioEngine->SetListenerOritation(facex,facey,facez,upx,upy,upz);
}

//����һ��װ����Ʒ����ʱ��������Ӧ����
//����˵��
//	dwIndex:����Ʒ��������� b3D:�Ƿ���3D���� x,y,z:��������
//����˵��
//	��
void CAudioList::PlayEquipmentSound(DWORD dwIndex,bool b3D,float x,float y,float z)
{
	if(m_pAudioEngine==NULL)return;
	if(dwIndex==0)return;
	char strName[MAX_PATH]="";

	CGoodsList::tagGoods2 *pGoods = CGoodsList::GetProperty(dwIndex);
	if (pGoods)
	{
		DWORD dwSound=pGoods->BaseProperty.dwSound;
		sprintf(strName,"SOUNDS\\equipments\\%05d.wav",dwSound);
		if( b3D )
		{
			Play3DSound(strName,1,x,y,z);
		}
		else
		{
			Play2DSound(strName);
		}
	}
}

//������Ϸ�������ֵ��ļ���
//����˵��
//	filename:�ļ����� ���Ϊ�ձ�ʾ���ñ�������Ϊ��
//����˵��
//	��
void CAudioList::SetBackgroundMusic(const char *filename)
{
	if(m_pAudioEngine==NULL)return;
	if(filename)
	{
        m_strMusicFile=string(filename);
	}
	else
	{
		m_strMusicFile=string("");
	}
}

//��ȡ��Ϸ�������ֵ��ļ���
//����˵��
//	��
//����˵��
//	���ر����������ļ���,����ǿ��ַ���(""),��ʾû�б�������
const char *CAudioList::GetBackgroundMusic()
{
	return m_strMusicFile.c_str();
}

//��������,���ò���
void CAudioList::LoadSetup(const char *filename)
{
	if(m_pAudioEngine==NULL)return;
	float rolloff;
	float mindistance,maxdistance;
	float distancefactor;
	long PlayOppearSoundTime;

	CRFile* prfile = rfOpen(filename);
	if( prfile )
	{	
		stringstream stream;
		prfile->ReadToStream(stream);
		rfClose(prfile);
		string s;
		stream >> s >> rolloff
			>> s >> mindistance
			>> s >> maxdistance
			>> s >> distancefactor
			>> s >> CMoveShape::highhurt_monster
			>> s >> CMoveShape::highhurt_boss
			>> s >> CMoveShape::highhurt_knight
			>> s >> CMoveShape::highhurt_bow
			>> s >> CMoveShape::highhurt_magic
			>> s >> CMoveShape::mildhurt_monster
			>> s >> CMoveShape::mildhurt_boss
			>> s >> CMoveShape::mildhurt_knight
			>> s >> CMoveShape::mildhurt_bow
			>> s >> CMoveShape::mildhurt_magic
			>> s >> PlayOppearSoundTime;

		m_pAudioEngine->SetMinMaxDistance(mindistance,maxdistance);
		m_pAudioEngine->SetRolloffFactor(rolloff);
		m_pAudioEngine->SetDistanceFactor(distancefactor);
		m_lPlayOppearSoundTime = PlayOppearSoundTime;
	}
}

/*��������*/
void CAudioList::GarbageColloction()
{
	if(m_pAudioEngine==NULL)return;
	m_pAudioEngine->GrabageCollection();
}

/*���������ֵ�ʱ�䣬���ڿ�����ҽ�����Ϸʱ�����������Ź�����ֵ���Ч*/
void CAudioList::CountMonsterOppear()
{
	if(m_bPlayMonsterOppear==false)
	{
		long dwCurTime = (long)timeGetTime();
		if( (dwCurTime-m_lPlayerInterTime)>m_lPlayOppearSoundTime)
		{
          m_bPlayMonsterOppear=true;
		  m_lPlayerInterTime = 0;
		}
	}
}

/*�����Ƿ񲥷ų�����Ч*/
void CAudioList::SetOppearSoundEnble(bool l)
{
    m_bPlayMonsterOppear=l;
	m_lPlayerInterTime = timeGetTime();
}

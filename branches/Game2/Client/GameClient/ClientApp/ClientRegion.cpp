#include "stdafx.h"
#include "ClientRegion.h"
#include "Player.h"
#include "Monster.h"
#include "Build.h"
#include "Goods/Goods.h"
#include "Effect.h"
#include "RegionCamera.h"
#include "../../Net/Message.h"
#include "../../EngineExtend/GameBuildList.h"
#include "../../EngineExtend/GameMap.h"
#include "../../EngineExtend/TextureProjective.h"
#include "../../EngineExtend/DisplayModel.h"
#include "../../EngineExtend/Gameenveffect.h"
#include "../../EngineExtend/Gameenvcreature.h"
#include "../../EngineExtend/Gameselectray.h"
#include "../../EngineExtend/Weathermanager.h"
#include "../../GameClient/ClientApp/Other/FindPath.h"
#include "../../EngineExtend/CBitmapX.h"
#include "Skills/SkillXml.h"
#include "MiniMap.h"
#include "Other/effectsetup.h"
#include "Other/CountryList.h"
#include "CollectionGoods.h"
#include "Other/DeputyOccuSystemClient.h"
#include "PetSystem/Pet.h"
#include "Other/RegionList.h"
#include "Other/PicList.h"
#include "../../GameClient/GameControl.h"
#include "Other/AudioList.h"
#include "QuestSystem/QuestSystem.h"
#include "../GameClient/Game.h"
#include "../../Input/KeyBoard.h"
#include "Other/GlobeSetup.h"
#include "../../Input/Mouse.h"
#include "../../UI/UIManager.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

long AREA_WIDTH = 15;
long AREA_HEIGHT = 15;

CBitmapX *pPlayerShopBitmap=NULL;
CBitmapX *pPlayerRecruitBitmap=NULL;
CBitmapX *pPasswordBitmap=NULL;

//=============07.10.16liuke��ӹ���NPC��������ʾ��ϢͼƬ=========
CBitmapX *pFinishBitmap=NULL;			// �ﵽ�����������ʾ��Ϣ
CBitmapX *pNotFinishBitmap=NULL;		// û�ﵽ�����������ʾ��Ϣ
CBitmapX *pCanSendBitmap=NULL;			// NPC�������ɷ����������ʾ
CBitmapX *pWillSendBitmap=NULL;			// NPC�����������ɷ�����ʾ
//================================================================

CClientRegion::CClientRegion(void)
{
	m_pMiniMap = NULL;
	m_pGameSkyBox = NULL;
	m_dwHoverGrid = 0;
	m_pGameEnvEffect = NULL;
	ZeroMemory(m_apGameEnvCreature, sizeof(GameBaseEnvCreature *) * 4);
	m_WarRegionType = RWT_Normal;
	m_bNpcListLoaded = false;
	m_pFindPath = NULL;
	// BOSS Ѫ��ͼƬ
	m_pBossBackImage = CBitmapX::GetBitmapX("datafiles/imagesets/other/boss-hpunder.tga");
	m_pBossImage = CBitmapX::GetBitmapX("datafiles/imagesets/other/boss-hp.tga");
	m_eRegionLightType = RL_OUTDOOR;

	m_bChangeEquip = true;
	m_bOpenSPoint = false;
}

CClientRegion::~CClientRegion(void)
{
	Exit();
}



/*
* ����: ����AI
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.10.14 - lpf
*		������"���½���ģ������"
*/
void CClientRegion::AI()
{
	// ����뿪���Ƿ�Χ��shape
	CPlayer* pMainPlayer = GetGame()->GetMainPlayer();
	//long lUpdateX=pMainPlayer->GetTileX();
	//long lUpdateY=pMainPlayer->GetTileY();
	float lUpdateX = pMainPlayer->GetPosX();
	float lUpdateY = pMainPlayer->GetPosY();
	GetGame()->GetGameControl()->GetUpdateXY(lUpdateX,lUpdateY);

	float mx, my, dx, dy;

	mx =  lUpdateX/ AREA_WIDTH;
	my =  lUpdateY/ AREA_HEIGHT;

	// ����Լ�������ForceMove״̬�в����
	for (itBaseObject itDel = GetObjectList()->begin(); itDel != GetObjectList()->end(); itDel++)
	{
		CShape* pShape = dynamic_cast<CShape*>(*itDel);
		if (pShape && pShape != pMainPlayer)
		{
			// ���Ѳ������
			if (pShape->GetType() == TYPE_PLAYER && pMainPlayer->IsTeammate(pShape->GetExID()) )
				continue;
			// �Լ��ĳ��ﲻ�����
			if (pShape->GetType() == TYPE_PET && ((CPet*)pShape)->IsMainPlayerPet())
				continue;
			// Monster
			if (pShape->GetType() == TYPE_MONSTER )
			{
				CMonster *pMonster=dynamic_cast<CMonster*>(pShape);
				// ��ĳ������״̬��Monster
				if( pMonster && -1 != pMonster->GetStateByQuest() )
					continue;
			}

			// ������Ч
			if( pShape->GetType() == TYPE_EFFECT )
			{
				if( ((CEffect *)pShape)->IsTask() )
				{
					continue;
				}
			}

			// ���ѵļ��ܲ������
			if (pShape->GetType() == TYPE_SKILL)
			{
				CSkillXml *pSkill = dynamic_cast<CSkillXml*>(pShape);
				if (pSkill && pSkill->GetSourType() == TYPE_PLAYER)
				{
					if (pMainPlayer->IsTeammate(pSkill->GetSourID()))
						continue;
				}
			}

			dx = pShape->GetPosY() / AREA_WIDTH;
			dy = pShape->GetPosY() / AREA_HEIGHT;
			if(pShape->GetType() == TYPE_MONSTER ||
				(pShape->GetType() == TYPE_PLAYER && ((CMoveShape*)pShape)->IsInForceMoving()) )
			{
				dx = ((CMonster*)pShape)->GetDestX() / AREA_WIDTH;
				dy = ((CMonster*)pShape)->GetDestY() / AREA_HEIGHT;
			}
			// ���SHAPE�Ѿ��뿪MainPlayer����Ұ��Χ
			if( (abs(dx-mx) > 2 || abs(dy-my) > 2) )
			{			
				pShape->SetIsDeleted(true);
			}
		}
	}

	// ִ��AI
	listBaseObject listDel;
	for (itBaseObject it = GetObjectList()->begin(); it != GetObjectList()->end(); it++)
	{
		CShape* pShape = dynamic_cast<CShape*>(*it);
		if( pShape  )
		{
			if (pShape->GetIsDeleted())
			{
				listDel.push_back(*it);
			}
			else
			{
				char str[1024] = "";
#ifdef _RUNSTACKINFO_
				sprintf_s(str,"%s:AI() Start.Action:%d",pShape->GetName(),pShape->GetAction());
				CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),str);
#endif
				pShape->AI();
#ifdef _RUNSTACKINFO_
				sprintf_s(str,"%s:AI() End.Action:%d",pShape->GetName(),pShape->GetAction());
				CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),str);
#endif

				if (pShape->GetIsDeleted())
					listDel.push_back(*it);				
			}
		}
	}

	//=============��ʾ���������еĵ�ͼ��Ч��Ϣ(08.1.22�������)==========
	list<CPlayer::stQuestInfoByRgn>* lsQuestInfoByRgn = pMainPlayer->GetQuestInfoByRgn();
	if(lsQuestInfoByRgn->size() > 0)
	{
		list<CPlayer::stQuestInfoByRgn>::iterator it = lsQuestInfoByRgn->begin();
		for(; it!=lsQuestInfoByRgn->end(); ++it)
		{
			// ��ǰ�����²���Ҫ����
			if((*it).lRgnID == pMainPlayer->GetRegionID() 
				&& (*it).lEffectIDNO != 0
				&& (*it).guidEffect == CGUID::GUID_INVALID)
			{
				float fX = (float)((*it).lRgnX);
				float fY = (float)((*it).lRgnY);
				float fH = GetCellHeight(fX,fY);
				CEffect *pEffect = PlayEffect((*it).lEffectIDNO,(*it).lRgnX+0.5f,(*it).lRgnY+0.5f,2.5f+fH);
				if(pEffect)
				{
					pEffect->SetLoop(true);
					pEffect->SetTask(true);
					(*it).guidEffect = pEffect->GetExID();
				}
			}
		}
	}
	//=====================================================================

	// ɾ���Ѿ�����ɾ��״̬��shape
	if (listDel.size() > 0)
	{
		for(itBaseObject it2 = listDel.begin(); it2 != listDel.end(); ++it2)
		{
			CShape* pObject = dynamic_cast<CShape*>(*it2);
			if( pObject )
			{
				//long mx = pObject->GetTileX();
				//long my = pObject->GetTileY();
				float mx = pObject->GetPosX();
				float my = pObject->GetPosY();

				CMoveShape *pMoveShape = dynamic_cast<CMoveShape*>(pObject);
				if (pMoveShape &&
					(pMoveShape->GetAction()==CShape::ACT_WALK || pMoveShape->GetAction()==CShape::ACT_RUN))
				{
					//mx = (long)pMoveShape->GetDestX();
					//my = (long)pMoveShape->GetDestY();
					mx = pMoveShape->GetDestX();
					mx = pMoveShape->GetDestY();
				}

				//if( pObject->GetType() == TYPE_MONSTER )
				//	GetGame()->GetMainPlayer()->GetPetIDSet()->erase(pObject->GetExID());

				// ɾ�����ָ�����
				if( pObject==GetGame()->GetGameControl()->GetMouseShape() )
					GetGame()->GetGameControl()->SetMouseShape( NULL );
				// ɾ�����ѡ�����
				if( pObject==GetGame()->GetGameControl()->GetSelectedShape() )
					GetGame()->GetGameControl()->SetSelectedShape( NULL );
				// ɾ����ǰ�������Ĺ���Ŀ��
				if( pObject==GetGame()->GetGameControl()->GetCurAttractShape() )
					GetGame()->GetGameControl()->SetCurAttractShape( NULL );
				// ɾ�����Ƕ���,���¶�����Ϣ
				if(GetGame()->GetMainPlayer()->IsTeammate(pObject->GetExID()))
				{
					//GetGame()->GetCGuiEx()->GetTeamPageEx()->Set3DIconShape(pObject->GetExID());
				}
				long lType = pObject->GetType();
				CGUID guid = pObject->GetExID();

				/////////////////////////////////////////////////////////////start
				//test add or delete shape,(author:wangqiao)
				char pszTestInfo[1024]="";
				char pszGUID[64]="";
				pObject->GetExID().tostring(pszGUID);
				//if(pMoveShape)
				//{
				//_snprintf(pszTestInfo,1024,"Delete the Shape (type:%d,id:%s,name:%s,x:%d,y:%d,dx:%d,dy:%d)(mx:%d,my:%d)",
				//	pObject->GetType(),pszGUID,pObject->GetName(),pObject->GetTileX(),pObject->GetTileY(),(long)(pMoveShape->GetDestX()),(long)(pMoveShape->GetDestY()),
				//GetGame()->GetMainPlayer()->GetTileX(),GetGame()->GetMainPlayer()->GetTileY());
				//}
				//else
				//{
				//_snprintf(pszTestInfo,1024,"Delete the Shape (type:%d,id:%s,name:%s,x:%d,y:%d)(mx:%d,my:%d)",
				//	pObject->GetType(),pszGUID,pObject->GetName(),pObject->GetTileX(),pObject->GetTileY(),
				//	GetGame()->GetMainPlayer()->GetTileX(),GetGame()->GetMainPlayer()->GetTileY());
				//}
				//PutStringToFile("AddDelShapeTest",pszTestInfo);
				////////////////////////////////////////////////////////////end

				if(pObject->GetType() == TYPE_PET)
				{
					CPet *pDelPet = dynamic_cast<CPet*>(pObject);
					CPlayer *pPlayer = dynamic_cast<CPlayer*>(FindChildObject(TYPE_PLAYER,pDelPet->GetMasterGUID()));
					if(pPlayer && pPlayer != GetGame()->GetMainPlayer())
					{
						//@todo ������
						//pPlayer->RemovePet(pObject->GetExID());
					}
				}
				DeleteChildObject(pObject);
			}
		}
		listDel.clear();
	}

	// ���½���ģ������
	m_AlphaFadeObjectList.Update();

	//�����������������Ų���
	//m_RegionSoundList
	list<REGION_SOUND>::iterator SndIt=m_RegionSoundList.begin();
	for(;SndIt!=m_RegionSoundList.end();SndIt++)
	{
		//float z=GetCamera()->GetPosition()->x;
		//float y=GetCamera()->GetPosition()->y;
		//float x=GetCamera()->GetPosition()->z; 
		float z=GetGame()->GetMainPlayer()->GetPosY();
		//float y=GetGame()->GetMainPlayer()->GetPosY();
		float x=GetGame()->GetMainPlayer()->GetPosX(); 

		float dis=sqrt((x-SndIt->xpos)*(x-SndIt->xpos) + (z-SndIt->zpos)*(z-SndIt->zpos));
		if((SndIt->Sound.fReferenceDistance-dis)>0.1f)//		������Ч��Χ 
		{	
			if(!SndIt->bPlaying)
			{
				SndIt->channel=GetGame()->GetAudioList()->Play(&(*SndIt),GetResourceID(),true);
				SndIt->bPlaying=true;
			}
			//����������ִ򿪣�����������
			if(GetGame()->GetSetup()->bMusic&&!GetGame()->GetAudioList()->GetFadeState()&&SndIt->channel!=-1) 
			{
				float fVolume = GetGame()->GetSetup()->fMusicVolume;
				string strMusic = GetGame()->GetAudioList()->GetBackgroundMusic();
				//GetGame()->GetAudioList()->SetMusicVolume(fVolume/2); 
				GetGame()->GetAudioList()->FadeMusic(strMusic.c_str(),3000,fVolume/3.0f);
				GetGame()->GetAudioList()->SetFadeState(true);
			}
		}
		else
		{
			if(SndIt->bPlaying) //������Χ,ֹͣ����
			{
				char filename[MAX_PATH];
				sprintf(filename,"sounds/regions/%d/%d.wav",GetResourceID(),SndIt->Sound.wID);
				GetGame()->GetAudioList()->Release3DSound(filename,SndIt->channel);
				SndIt->bPlaying=false;
				//����������ִ򿪣���������ԭ
				if(GetGame()->GetSetup()->bMusic&&GetGame()->GetAudioList()->GetFadeState())
				{
					float fVolume = GetGame()->GetSetup()->fMusicVolume;
					string strMusic = GetGame()->GetAudioList()->GetBackgroundMusic();
					//GetGame()->GetAudioList()->SetMusicVolume(fVolume);
					GetGame()->GetAudioList()->FadeMusic(strMusic.c_str(),2000,fVolume);
					GetGame()->GetAudioList()->SetFadeState(false);
				}
			}
		}
	}

	//����������Ч
	list<ENV_SOUND>::iterator EnvSndIter=m_EnvSoundList.begin();
	for(;EnvSndIter!=m_EnvSoundList.end();EnvSndIter++)
	{		
		if(EnvSndIter->bLoop)
		{
			if(!EnvSndIter->bPlaying)
			{
				EnvSndIter->channel=GetGame()->GetAudioList()->Play2DSound(EnvSndIter->strFileName.c_str(),-1);//GetGame()->GetAudioList()->Play(EnvSndIter->strFileName.c_str(),false,false,EnvSndIter->nLoops);
				EnvSndIter->bPlaying=true;
			}
			else
			{
				if(EnvSndIter->channel!=-1)
				{
					//float x=GetGame()->GetMainPlayer()->GetPosY(),y=0,z=-GetGame()->GetMainPlayer()->GetPosX();
					//GetGame()->GetAudioList()->Set3DSoundPosition(EnvSndIter->strFileName.c_str(),EnvSndIter->channel,x,y,z);
				}
			}
		}
		else
		{
			if(GetCurTickCount()-EnvSndIter->dwStartTime > (DWORD)EnvSndIter->lIntervalTime)
			{
				GetGame()->GetAudioList()->Play(EnvSndIter->strFileName.c_str(),
					GetGame()->GetMainPlayer()->GetPosX(),					
					GetGame()->GetMainPlayer()->GetPosY(),
					0,
					false);
				EnvSndIter->dwStartTime=GetCurTickCount();
				EnvSndIter->lIntervalTime=EnvSndIter->lMinTime+random(EnvSndIter->lMaxTime-EnvSndIter->lMinTime);
			}
		}
	}


	//��������
	//rain
	if(GetGame()->GetWeatherManager()->IsRainEnable())
	{
		if(!GetGame()->GetWeatherManager()->IsRainSndPlaying())
		{
			int channel=GetGame()->GetAudioList()->Play2DSound(GetGame()->GetWeatherManager()->GetRainSoundFile(),-1);
			if(channel!=-1)
			{
				GetGame()->GetWeatherManager()->SetRainSndID(channel);
				GetGame()->GetWeatherManager()->SetRainSndPlayingState(TRUE);
			}
		}
	}
	else
	{
		if(GetGame()->GetWeatherManager()->IsRainSndPlaying())
		{
			GetGame()->GetAudioList()->FadeOut2DSound(GetGame()->GetWeatherManager()->GetRainSoundFile(),1000);
			GetGame()->GetWeatherManager()->SetRainSndPlayingState(FALSE);
		}
	}

	//snow
	if(GetGame()->GetWeatherManager()->IsSnowEnable())
	{
		if(!GetGame()->GetWeatherManager()->IsSnowSndPlaying())
		{
			int channel=GetGame()->GetAudioList()->FadeIn2DSound(GetGame()->GetWeatherManager()->GetSnowSoundFile(),3000);
			if(channel!=-1)
			{
				GetGame()->GetWeatherManager()->SetSnowSndPlayingState(TRUE);
				GetGame()->GetWeatherManager()->SetSnowSndID(channel);
			}
		}
	}
	else
	{
		if(GetGame()->GetWeatherManager()->IsSnowSndPlaying())
		{
			GetGame()->GetAudioList()->FadeOut2DSound(GetGame()->GetWeatherManager()->GetSnowSoundFile(),2000);
			GetGame()->GetWeatherManager()->SetSnowSndPlayingState(FALSE);
		}
	}

	//fog
	if(GetGame()->GetWeatherManager()->IsFogEnable())
	{
		if(!GetGame()->GetWeatherManager()->IsFogSndPlaying())
		{
			int channel=GetGame()->GetAudioList()->FadeIn2DSound(GetGame()->GetWeatherManager()->GetFogSoundFile(),3000);
			if(channel!=-1)
			{
				GetGame()->GetWeatherManager()->SetFogSndPlayingState(TRUE);			
				GetGame()->GetWeatherManager()->SetFogSndID(channel);
			}
		}
	}
	else
	{
		if(GetGame()->GetWeatherManager()->IsFogSndPlaying())
		{
			GetGame()->GetAudioList()->FadeOut2DSound(GetGame()->GetWeatherManager()->GetFogSoundFile(),2000);
			GetGame()->GetWeatherManager()->SetFogSndPlayingState(FALSE);			
		}
	}

	//env fog
	//if(GetGame()->GetWeatherManager()->IsEnvFogEnable())
	//{
	//	if(!GetGame()->GetWeatherManager()->IsEnvFogSndPlaying())
	//	{
	//		int channel=GetGame()->GetAudioList()->FadeIn2DSound(GetGame()->GetWeatherManager()->GetEnvFogSoundFile(),3000);
	//		if(channel!=-1)
	//		{
	//			GetGame()->GetWeatherManager()->SetEnvFogSndPlayingState(TRUE);
	//			GetGame()->GetWeatherManager()->SetEnvFogSndID(channel);
	//		}
	//	}
	//}
	//else
	//{
	//	if(GetGame()->GetWeatherManager()->IsEnvFogSndPlaying())
	//	{
	//		GetGame()->GetAudioList()->FadeOut2DSound(GetGame()->GetWeatherManager()->GetEnvFogSoundFile(),2000);
	//		GetGame()->GetWeatherManager()->SetEnvFogSndPlayingState(FALSE);
	//	}
	//}	

	//ս�������뱳�����ֵ��л�
	//map<long,CGame::tagRegion>::iterator RegionDataIter=GetGame()->GetRegionSetup()->find( GetGame()->GetRegion()->GetResourceID() );
	//if(RegionDataIter!=GetGame()->GetRegionSetup()->end())
	//{
	//	if(pMainPlayer->GetState()==CShape::STATE_FIGHT && !RegionDataIter->second.bFightMusicPlaying)
	//	{
	//		//�����������ֽ���
	//		if(RegionDataIter->second.bgMusicPlaying)
	//		{
	//			GetGame()->GetAudioList()->FadeOutMusic(RegionDataIter->second.strBgMusic.c_str(),2000,-1);
	//			RegionDataIter->second.bgMusicPlaying=false;
	//		}
	//		if(!RegionDataIter->second.bFightMusicPlaying)
	//		{
	//			GetGame()->GetAudioList()->FadeInMusic(RegionDataIter->second.strFightMusic.c_str(),5000);
	//			GetGame()->GetAudioList()->SetBackgroundMusic(RegionDataIter->second.strFightMusic.c_str());
	//			RegionDataIter->second.bFightMusicPlaying=true;
	//		}
	//	}
	//	else if(pMainPlayer->GetState()!=CShape::STATE_FIGHT && RegionDataIter->second.bFightMusicPlaying)
	//	{
	//		//ս�����ֽ���
	//		if(RegionDataIter->second.bFightMusicPlaying)
	//		{
	//			GetGame()->GetAudioList()->FadeOutMusic(RegionDataIter->second.strFightMusic.c_str(),2000,-1);
	//			RegionDataIter->second.bFightMusicPlaying=false;
	//		}
	//		if(!RegionDataIter->second.bgMusicPlaying)
	//		{
	//			//�����������ֽ���
	//			GetGame()->GetAudioList()->FadeInMusic(RegionDataIter->second.strBgMusic.c_str(),5000);
	//			RegionDataIter->second.bgMusicPlaying=true;	
	//			GetGame()->GetAudioList()->SetBackgroundMusic(RegionDataIter->second.strBgMusic.c_str());
	//		}
	//	}
	//}

	//����Ч����ͼ������
	int x = pMainPlayer->GetTileY();
	int z = pMainPlayer->GetTileX();
	int iWidth = (int)m_pGameMap->GetWidth();

	WeatherManager *pWeatherManager = GetGame()->GetWeatherManager();
	if (!(x < 0 || x >= iWidth || z < 0 || z >= (int)m_pGameMap->GetDepth()))
	{
		GameMap::tagGrid *pGrid = m_pGameMap->GetGrid(x + z * iWidth);
		GetGame()->SetPlayerInWeatherEffectArea(pGrid->byFlag & GRIDMASK_WEATHEREFFECT);
		//���ط���
		m_pGameMap->SetTargetHideGridIndex(pGrid->dwTargetHideGridIndex);	
	}

}

void	CClientRegion::SwitchDayOrNight()
{
	if (m_eRegionLightType == RL_OUTDOOR)
	{
		m_eRegionLightType = RL_CAVE;
		SetPointLight();
	}
	else
	{
		m_eRegionLightType = RL_OUTDOOR;
		SetDirectLight();
	}
}

// �Ƴ�OBJECT
void CClientRegion::RemoveObject(CBaseObject* pObject)
{
	if( pObject == NULL )	return;

	CShape *pShape = dynamic_cast<CShape*>(pObject);

	//��ȥ����
	if(pShape->GetType() == TYPE_PLAYER || pShape->GetType() == TYPE_MONSTER || pShape->GetType() == TYPE_PET)
		DecCellObjNum(pShape->GetTileX(),pShape->GetTileY());

	CRegion::RemoveObject(pObject);
}

void CClientRegion::Run()
{
	CPlayer* pMainPlayer = GetGame()->GetMainPlayer();
}

//�õ�һ�����ĵ㸽��û��MoveShape�ĸ���
void CClientRegion::GetNoShapeCell(long lCenterX,long lCenterY,long lRadius,long lFirstDir,long& lX,long& lY)
{
	static long pos[4][2] = {-1,-1, 1,-1, 1,1, -1,1};
	//�����������������
	static int facedir[8] = { 4, 5, 6, 7, 0, 1, 2, 3 };

	lX = lCenterX;
	lY = lCenterY;
	//�ж��Ƿ����赲�����Ҹõ���û�ж���
	if( (GetBlock(lCenterX,lCenterY) == CRegion::BLOCK_NO || GetBlock(lCenterX,lCenterY) == CRegion::BLOCK_AIM) &&
		GetShape(lCenterX,lCenterY) == NULL)
		return;
	//���뾶ÿ����1��ʱ��,������չһ��
	long lTemptX = 0;
	long lTemptY = 0;
	long lStartDir = facedir[lFirstDir]/2;
	for(int i=1;i<=lRadius;i++)
	{	
		long lDir = lStartDir;
		for(int j=0;j<4;j++)
		{
			lTemptX = lCenterX+pos[lDir][0];
			lTemptY = lCenterY+pos[lDir][1];
			int count = (int)pow((float)2,i);
			for(int n=0;n<count;n++)
			{
				if( (GetBlock(lTemptX,lTemptY) == CRegion::BLOCK_NO || GetBlock(lTemptX,lTemptY) == CRegion::BLOCK_AIM) &&
					GetShape(lTemptX,lTemptY) == NULL)
				{
					lX = lTemptX;
					lY = lTemptY;
					return;
				}
				if(lDir==0)
					lTemptX++;
				else if(lDir==1)
					lTemptY++;
				else if(lDir==2)
					lTemptX--;
				else if(lDir==3)
					lTemptY--;
			}

			lDir = (lDir++)%4;
		}
	}
}

//�õ�һ���ռ���ĳ�����ϵ�һ�����ӣ��ø����ϵ�MoveShape��������
void CClientRegion::GetLeastShapesCellFromSpace(int nObjCenterX,int nObjCenterY,
												int nObjFigureX,int nObjFigureY,int nMaxDistance,int nDiretion,
												long& lX,long& lY)
{
	//�����������������
	static int facedir[8] = { 4, 5, 6, 7, 0, 1, 2, 3 };
	//���������ƫ������ֵ
	static int DirOff[8][2] = {0,-1, 1,-1, 1,0, 1,1, 0,1, -1,1, -1,0, -1,-1};
	//��ͬ����ǰ����ֵ����������
	static int forward[8][2] = {6,2, 2,0, 0,4, 2,4, 2,6, 4,6, 4,0, 6,0};

	//�����
	int nStep = 0;
	if(nDiretion==0 || nDiretion==4)
		nStep = nMaxDistance*2+1+nObjFigureX;
	else if(nDiretion==2 || nDiretion==6)
		nStep = nMaxDistance*2+1+nObjFigureY;
	else
		nStep = min(nObjFigureX,nObjFigureY)+nMaxDistance*2+1;

	int nCount0 = nMaxDistance;
	int nCount1 = nMaxDistance;

	int nLenX = nMaxDistance+nObjFigureX;
	int nLenY = nMaxDistance+nObjFigureY;
	//ѭ����ֵ
	int nFirstX = nObjCenterX+DirOff[facedir[nDiretion]][0]*nLenX;
	int nFirstY = nObjCenterY+DirOff[facedir[nDiretion]][1]*nLenY;

	int nLastX = nObjCenterX+DirOff[nDiretion][0]*nLenX;
	int nLastY = nObjCenterY+DirOff[nDiretion][1]*nLenY;

	long lLeastShapeNum = 0xFFFFFF;
	long lLestX = nObjCenterX;
	long lLeasY = nObjCenterY;
	for(int i=0;i<nStep;i++)
	{
		int nBeginX = nFirstX+DirOff[nDiretion][0]*i;
		int nBeginY = nFirstY+DirOff[nDiretion][1]*i;
		//�쿴�Ƿ����赲
		if( GetBlock(nBeginX,nBeginY)== BLOCK_NO ||
			GetBlock(nBeginX,nBeginY)== BLOCK_AIM)
		{
			//�쿴�Ƿ���MoveShape
			long lShapeNum = GetCellObjNum(nBeginX,nBeginY);
			if( lShapeNum<= 0)
			{
				lX = nBeginX;lY=nBeginY;
				return;
			}
			else if(lShapeNum < lLeastShapeNum)
			{
				lLeastShapeNum = lShapeNum;
				lLestX = nBeginX;lLeasY = nBeginY;
			}
		}

		//ѭ������
		if(nDiretion%2 != 0)
		{
			nCount0 = abs(nLastX-nBeginX);
			nCount1 = abs(nLastY-nBeginY);
		}
		for(int i0=1,i1=1;i0<=nCount0 || i1<=nCount1;i0++,i1++)
		{
			if(i0<=nCount0)
			{
				int nTemptX0 = nBeginX+DirOff[forward[nDiretion][1]][0]*i0;
				int nTemptY0 = nBeginY+DirOff[forward[nDiretion][1]][1]*i0;
				//�쿴�Ƿ����赲
				if( GetBlock(nTemptX0,nTemptY0)== BLOCK_NO ||
					GetBlock(nTemptX0,nTemptY0)== BLOCK_AIM)
				{
					//�쿴�Ƿ���MoveShape
					long lShapeNum = GetCellObjNum(nTemptX0,nTemptY0);
					if( lShapeNum<= 0)
					{
						lX = nTemptX0;lY=nTemptY0;
						return;
					}
					else if(lShapeNum < lLeastShapeNum)
					{
						lLeastShapeNum = lShapeNum;
						lLestX = nTemptX0;lLeasY = nTemptY0;
					}
				}
			}
			if(i1<=nCount1)
			{
				int nTemptX1 = nBeginX+DirOff[forward[nDiretion][0]][0]*i1;
				int nTemptY1 = nBeginY+DirOff[forward[nDiretion][0]][1]*i1;
				//�쿴�Ƿ����赲
				if( GetBlock(nTemptX1,nTemptY1)== BLOCK_NO ||
					GetBlock(nTemptX1,nTemptY1)== BLOCK_AIM )
				{
					//�쿴�Ƿ���MoveShape
					long lShapeNum = GetCellObjNum(nTemptX1,nTemptY1);
					if( lShapeNum<= 0)
					{
						lX = nTemptX1;lY=nTemptY1;
						return;
					}
					else if(lShapeNum < lLeastShapeNum)
					{
						lLeastShapeNum = lShapeNum;
						lLestX = nTemptX1;lLeasY = nTemptY1;
					}
				}
			}
		}
	}

	lX = lLestX;lY = lLeasY;
}

//�õ�һ��Բ�οռ���ĳ�����ϵ�һ�����ӣ��ø����ϵ�MoveShape��������
void CClientRegion::GetLeastShapesCellFromCircularSpace(int nObjCenterX,int nObjCenterY,
														int nObjFigureX,int nObjFigureY,float nMaxDistance,long lDir,
														long& lX,long& lY)
{
	static int pos[8][2] = {0,-1, 1,-1, 1,0, 1,1, 0,1, -1,1, -1,0, -1,-1};
	int lShapeX = nObjCenterX,lShapeY = nObjCenterY;
	long dir = lDir;
	int nMinFigure = min(nObjFigureX,nObjFigureY);
	float nMaxDist = nMaxDistance+nMinFigure;
	long lLeastShapeNum = 0xFFFFFF;
	int nTemptX = 0;
	int nTemptY = 0;
	// ��Ŀ�����ĵ㣬һ��һ���������
	for(int i=1; i <= nMaxDist; ++i)
	{
		// �����ԭʼ����
		dir = lDir;
		// �Ӵ���ķ���ʼ��˳ʱ�����
		for(int j=0; j<8; ++j)
		{
			if(dir >= 8)
				dir = 0;
			nTemptX = nObjCenterX + pos[dir][0]*i;
			nTemptY = nObjCenterY + pos[dir][1]*i;
			for(int n = 0; n < i; ++n)
			{
				if( i > nMinFigure && (GetBlock(nTemptX,nTemptY)== BLOCK_NO ||
					GetBlock(nTemptX,nTemptY)== BLOCK_AIM ))
				{
					//�쿴�Ƿ���MoveShape
					long lShapeNum = GetCellObjNum(nTemptX,nTemptY);
					if( lShapeNum<= 0)
					{
						lX = nTemptX;lY=nTemptY;
						return;
					}
					else if(lShapeNum < lLeastShapeNum)
					{
						lLeastShapeNum = lShapeNum;
						lShapeX = nTemptX,lShapeY = nTemptY;
					}
				}
				if(dir == 0 || dir == 7)
					++nTemptX;
				else if(dir == 1 || dir == 2)
					++nTemptY;
				else if(dir == 3 || dir == 4)
					--nTemptX;
				else if(dir == 5 || dir == 6)
					--nTemptY;
			}
			++dir;
		}
	}
	lX = lShapeX;lY = lShapeY;
	//���������ƫ������ֵ
	//static float fDirOffSet[8][2] = {
	//								0.0,0.0, 
	//								(D3DX_PI/8),-(D3DX_PI/8), 
	//								(D3DX_PI/4),-(D3DX_PI/4), 
	//								(D3DX_PI*3/8),-(D3DX_PI*3/8), 
	//								(D3DX_PI/2),-(D3DX_PI/2),
	//								(D3DX_PI*5/8),-(D3DX_PI*5/8),
	//								(D3DX_PI*3/4),-(D3DX_PI*3/4),
	//								(D3DX_PI*7/8),-(D3DX_PI*7/8),
	//								};
	//int nMinFigure = min(nObjFigureX,nObjFigureY);
	//float fMaxDist = (float)(nMaxDistance+nMinFigure);
	//long lLeastShapeNum = 0xFFFFFF;
	//// ����ƫ��������ʱ����
	//int lShapeX = nObjCenterX,lShapeY = nObjCenterY;
	//while (fMaxDist > 0.0)
	//{
	//	// ��ǰ�Ļ���
	//	float fDiretion0 = 0.0;
	//	float fDiretion1 = 0.0;
	//	for (int i=0; i<8; ++i)
	//	{
	//		fDiretion0 = D3DX_PI*i/8 + fDir;//fDirOffSet[i][0] + fDir;
	//		fDiretion1 = -D3DX_PI*i/8 + fDir;//fDirOffSet[i][1] + fDir;
	//		// �����������ƫ��
	//		int nTemptX0 = nObjCenterX - (int)(fMaxDist * cosf(fDiretion0)) + nMinFigure;
	//		int nTemptY0 = nObjCenterY - (int)(fMaxDist * sinf(fDiretion0)) + nMinFigure;
	//		// ��������긺ƫ��
	//		int nTemptX1 = nObjCenterX - (int)(fMaxDist * cosf(fDiretion1)) - nMinFigure;
	//		int nTemptY1 = nObjCenterY - (int)(fMaxDist * sinf(fDiretion1)) - nMinFigure;
	//		//�쿴�Ƿ����赲
	//		if( GetBlock(nTemptX0,nTemptY0)== BLOCK_NO ||
	//			GetBlock(nTemptX0,nTemptY0)== BLOCK_AIM )
	//		{
	//			//�쿴�Ƿ���MoveShape
	//			long lShapeNum0 = GetCellObjNum(nTemptX0,nTemptY0);
	//			if( lShapeNum0<= 0)
	//			{
	//				lX = nTemptX0;lY=nTemptY0;
	//				return;
	//			}
	//			else if(lShapeNum0 < lLeastShapeNum)
	//			{
	//				lLeastShapeNum = lShapeNum0;
	//				lShapeX = nTemptX0;lShapeY = nTemptY0;
	//			}
	//		}
	//		//�쿴�Ƿ����赲
	//		if( GetBlock(nTemptX1,nTemptY1)== BLOCK_NO ||
	//			GetBlock(nTemptX1,nTemptY1)== BLOCK_AIM )
	//		{
	//			//�쿴�Ƿ���MoveShape
	//			long lShapeNum1 = GetCellObjNum(nTemptX1,nTemptY1);
	//			if( lShapeNum1<= 0)
	//			{
	//				lX = nTemptX1;lY=nTemptY1;
	//				return;
	//			}
	//			else if(lShapeNum1 < lLeastShapeNum)
	//			{
	//				lLeastShapeNum = lShapeNum1;
	//				lShapeX = nTemptX1;lShapeY = nTemptY1;
	//			}
	//		}
	//	}
	//	fMaxDist -= 0.5;
	//}
	//lX = lShapeX;lY = lShapeY;
}

// ��ȡһ�������Ƿ���Shape
CShape* CClientRegion::GetShape(long x, long y)
{
	for(list<CBaseObject*>::iterator it = m_listObject.begin(); it != m_listObject.end(); ++it)
	{
		CShape *pShape = (CShape*)(*it);
		if (pShape &&
			(pShape->GetType() == TYPE_PLAYER || pShape->GetType() == TYPE_MONSTER /*|| pShape->GetType() == TYPE_NPC*/) &&
			pShape->GetTileX() == x && pShape->GetTileY() == y )
		{
			return pShape;
		}
	}
	return NULL;
}

// ��ȡһ�������Ƿ���Shape
CGoods* CClientRegion::GetShapeGoods(long x, long y)
{
	for(list<CBaseObject*>::iterator it = m_listObject.begin(); it != m_listObject.end(); ++it)
	{
		CGoods *pGoods = dynamic_cast<CGoods*>(*it);
		if ( pGoods && pGoods->GetTileX() == x && pGoods->GetTileY()==y )
		{
			return pGoods;
		}
	}
	return NULL;
}

// ��ȡһ�����ָ����Χ���Ƿ���Shape
CShape* CClientRegion::GetShapeFromSpace(long x, long y, long lSize)
{
	for (int i = x; i < x+lSize; ++i)
	{
		for (int j = y; j <y+lSize; ++j)
		{
			CShape *pShape = GetShape(i,j);
			if(pShape)
				return pShape;
		}
	}
	return NULL;
}

// ���һ�����ϵ�����ָ�����͵Ķ���    ADDBY�����ƣ�09.2.9��
CShape*  CClientRegion::GetAllShapeByType(list<CShape *> & lsShape, long x, long y, long type)
{
	lsShape.clear();
	CShape * pShapeEx = NULL;
	for(list<CBaseObject*>::iterator it = m_listObject.begin(); it != m_listObject.end(); ++it)
	{
		CShape *pShape = (CShape*)(*it);
		if (pShape && pShape->GetTileX() == x && pShape->GetTileY() == y)
		{
			// ���ͷ��ϣ����뵽�б���
			if(pShape->GetType() == type || type == -1)
			{
				// ����
				if(pShape->GetType() == TYPE_MONSTER)
				{
					CMonster *pMonster = (CMonster *)pShape;
					if(pMonster)
					{
						if(!pMonster->IsDied() && pMonster->GeteNpcType() != NT_Normal)
							lsShape.push_back(pShape);
					}
				}
				// ��Ҵ��ڷ�����״̬��
				else if(pShape->GetType() == TYPE_PLAYER)
				{
					CPlayer *pPlayer = (CPlayer *)pShape;
					if(pPlayer && !pPlayer->IsDied() && pPlayer != GetGame()->GetMainPlayer() && pPlayer->GetIsConceal() )
						lsShape.push_back(pShape);
				}
				// ��������
				else
					lsShape.push_back(pShape);
			}
			else if(pShapeEx == NULL)
			{
				// �����ҵ��ĵ�һ���������͵Ķ���
				if(pShape->GetType() == TYPE_MONSTER)
				{
					CMonster *pMonster = (CMonster *)pShape;
					if(pMonster && !pMonster->IsDied() && pMonster->GeteNpcType() != NT_Normal)
						pShapeEx = pShape;
				}
				// ��Ҵ��ڷ�����״̬��
				else if(pShape->GetType() == TYPE_PLAYER)
				{
					CPlayer *pPlayer = (CPlayer *)pShape;
					if(pPlayer && pPlayer->GetIsConceal())
						pShapeEx = pShape;
				}
			}
		}
	}
	return pShapeEx;
}

/****************************************************************************************
* ADDBY�����ƣ�09.2.5��
* ��ָ����Ŀ����ָ������ʼ����һ����Χ�ڴ��ڵ��⣬��˳ʱ��ѭ������һ��shape��
* ���Ҫ���ҵ�����ΪMonster�����ܷ���һ������������Npc��Monster
* ���Ҫ���ҵ�����ΪPlayer�����ܷ���һ������״̬�µ�Player
* ���Ҫ���ҵĶ�������ΪĬ��-1���򷵻ص�һ���ҵ��Ķ���
****************************************************************************************/
CShape* CClientRegion::GetLeastShape(list<CShape *> & lsSelectedShape,long x,long y,long lDir,long lOffLen,long lType)
{
	// �����壺			UP->RUP->RIGHR->RDOWN->DOWN->LDOWN->LEFT->LUP
	static int pos[8][2] = {0,-1, 1,-1, 1,0, 1,1, 0,1, -1,1, -1,0, -1,-1};
	list <CShape *>	ls_Shape;
	CShape * pShape = NULL;
	CShape * pDefShape = NULL;		// Ĭ�ϵ�monster���͵Ķ���
	CPlayer * pPlayer = GetGame()->GetMainPlayer();
	if(lOffLen > 0 && GetGame()->GetGameControl())
	{
		// �ѵ�ǰѡ�еĶ�����뵽ѡ�ж����б���
		pShape = GetGame()->GetGameControl()->GetSelectedShape();
		if(pShape)
		{
			// ����
			if(pShape->GetType() == TYPE_MONSTER)
			{
				CMonster *pMonster = (CMonster *)pShape;
				if(pMonster && !pMonster->IsDied() && pMonster->GeteNpcType() != NT_Normal)
				{
					if(find(lsSelectedShape.begin(),lsSelectedShape.end(),pShape) == lsSelectedShape.end())
						lsSelectedShape.push_back(pShape);
				}
			}
			// ����
			else if (pShape->GetType() == TYPE_PET)
			{
				CPet* pet = dynamic_cast<CPet*>(pShape);
				if (pet && !pet->IsDied())
				{
					if (find(lsSelectedShape.begin(),lsSelectedShape.end(),pShape) == lsSelectedShape.end())
						lsSelectedShape.push_back(pShape);
				}
			}
			// ���
			else if(find(lsSelectedShape.begin(),lsSelectedShape.end(),pShape) == lsSelectedShape.end())
				lsSelectedShape.push_back(pShape);
		}
		long lSourX = x;
		long lSourY = y;
		long dir = lDir;
		list<CShape *> lsShape;
		// ������ԭ������з��������Ķ���
		GetAllShapeByType(lsShape,lSourX,lSourY,lType);
		ls_Shape.splice(ls_Shape.end(),lsShape);
		// ��Ŀ�����ĵ㣬һ��һ���������
		for(int i=1; i<=lOffLen; ++i)
		{
			// �����ԭʼ����
			dir = lDir;
			// �Ӵ���ķ���ʼ��˳ʱ�����
			for(int j=0; j<8; ++j)
			{
				if(dir >= 8)
					dir = 0;
				lSourX = x + pos[dir][0]*i;
				lSourY = y + pos[dir][1]*i;
				for(int n = 0; n < i; ++n)
				{
					// �ڸ÷����ҵ�һ������
					GetAllShapeByType(lsShape,lSourX,lSourY,lType);
					ls_Shape.splice(ls_Shape.end(),lsShape);
					if(dir == 0 || dir == 7)
						++lSourX;
					else if(dir == 1 || dir == 2)
						++lSourY;
					else if(dir == 3 || dir == 4)
						--lSourX;
					else if(dir == 5 || dir == 6)
						--lSourY;
				}
				++dir;
			}
		}
		// �������
		list<CShape *>::iterator iter;
		list<CShape *>::iterator it = ls_Shape.begin();
		// ����������õ�ѡ������ҵ�һ��֮ǰûѡ���Ķ��󷵻�
		bool bEFlag = GetGame()->GetTabSet()->bEnemyPlayer;
		bool bPFlag = GetGame()->GetTabSet()->bPiratePlayer;
		bool bMFlag = GetGame()->GetTabSet()->bMonster;
		bool bPetFlag = GetGame()->GetTabSet()->bPet;
		bool bIsLockTeam = GetGame()->GetGameControl()->GetIsTabTeamMate();
		pShape = NULL;
		for (; it != ls_Shape.end(); ++it)
		{
			// ��������
			if(bIsLockTeam)
			{
				if(pPlayer->IsTeammate((*it)->GetExID()))
					pShape = (*it);
			}
			// �������(type = -1��=PLAYE)
			else if(lType != TYPE_MONSTER)
			{
				CPlayer * pDefPlayer = dynamic_cast<CPlayer *>((*it));
				if(pDefPlayer)
				{
					// ��Ӫ�ж�
					bool bIsEnemy = false;
					CCountryList::tagCountryList* ptgMList = CCountryList::GetCountry( pPlayer->GetCountry() );
					CCountryList::tagCountryList* ptgDList = CCountryList::GetCountry( pDefPlayer->GetCountry() );
					if(ptgMList && ptgDList && ptgMList->nLeague == ptgDList->nLeague)
						bIsEnemy = true;
					// �й����
					if(bEFlag && !bIsEnemy)
						pShape = (*it);
					// �й��������
					else if(bPFlag && !bIsEnemy && pDefPlayer->GetPVPValue() > 0)
						pShape = (*it);
				}
				else
				{
					CMonster * pDefMonster = dynamic_cast<CMonster *>((*it));
					if(pDefMonster)
						pShape = (*it);
				}
			}
			else
				pShape = (*it);
			if(!pDefShape && pShape)
				pDefShape = pShape;		// ������������ĵ�һ������
			if(pShape)
			{
				// ���֮ǰ��ѡ�й���������
				iter = find(lsSelectedShape.begin(),lsSelectedShape.end(),pShape);
				if(iter != lsSelectedShape.end())
					pShape = NULL;
			}
			if(pShape)
				return pShape;
		}
		// ��������ж��󶼱�ѡ������򷵻ر��еĵ�һ�����󣬲����ѡ������б�
		lsSelectedShape.clear();
	}
	return pDefShape;
}

// ���������Χ������NPC������״̬
void CClientRegion::UpdataAllNpcQuestState()
{
	CPlayer * pPlayer = GetGame()->GetMainPlayer();
	if(pPlayer)
	{
		CMonster * pNpc = NULL;
		for(list<CBaseObject*>::iterator it = m_listObject.begin(); it != m_listObject.end(); ++it)
		{
			// ����Npc,��������״̬
			pNpc = dynamic_cast<CMonster*>(*it);
			if(pNpc && pNpc->GeteNpcType() == NT_Normal)
			{
				pPlayer->UpdataSpecialQuestInfoByNpc(pNpc);
			}
		}
	}	
}


void CClientRegion::DisplayTextureProjective()
{
	CTextureProjective::SetState();
	static D3DXMATRIX matWorld,matRot,matScaling;
	static AnimInfo ani;
	static D3DXVECTOR3 vCenter;
	std::list<DWORD> rendbuildlist;
	std::list<CTextureProjective*> texprojlist = CTextureProjective::GetAllCTextureProjectiveList();
	std::list<CTextureProjective::SHADOWTEXTURE> shadowlist = CTextureProjective::GetShadowTextureList();

	//��Ⱦ�߼���Ӱ
	if (!shadowlist.empty())
	{

		CTextureProjective::SetShadowState();
		std::list<CTextureProjective::SHADOWTEXTURE>::iterator it = shadowlist.begin();
		for (uint i = 0;it != shadowlist.end() && i < CTextureProjective::GetUsedTextureNum();i++,it++)
		{
			CTextureProjective::SetBufferSize((*it).size);
			GetGameMap()->GetTagMapBuffer((int)(*it).pos.x,(int)(*it).pos.z,(*it).size,CTextureProjective::GetBuffer(),rendbuildlist);
			D3DXMATRIX matid;
			D3DXMatrixIdentity(&matid);
			CTextureProjective::RendShadow(&(*it),*GetGame()->GetRegion()->GetGameMap()->GetSunLightDirection(),matid,*(GetGame()->GetRegion()->GetCamera()->GetViewMatrix()),*(GetGame()->GetRegion()->GetCamera()->GetProjectionMatrix()));

			for (std::list<DWORD>::iterator bit = rendbuildlist.begin();bit != rendbuildlist.end();bit++)
			{
				GameMap::tagGrid* tGrid =  GetGameMap()->GetGrid(*bit);
				GameBuildList::Build *pBuild = tGrid->pBuild->pBulid;
				D3DXVECTOR3* vertexposarry = GetGameMap()->GetTerrainPositionArray();
				vCenter = (vertexposarry[tGrid->dwVertexIndex[0]] + vertexposarry[tGrid->dwVertexIndex[1]] + vertexposarry[tGrid->dwVertexIndex[2]] + vertexposarry[tGrid->dwVertexIndex[3]]) / 4.0f;

				D3DXMatrixTranslation(&matWorld, vCenter.x + tGrid->pBuild->vPosOffset.x, vCenter.y + tGrid->pBuild->vPosOffset.y, vCenter.z + tGrid->pBuild->vPosOffset.x);
				D3DXMatrixRotationYawPitchRoll(&matRot, tGrid->pBuild->vRotation.y, tGrid->pBuild->vRotation.x, tGrid->pBuild->vRotation.z);
				D3DXMatrixScaling(&matScaling, tGrid->pBuild->vScaling.x, tGrid->pBuild->vScaling.y, tGrid->pBuild->vScaling.z);
				matWorld = matScaling * matRot * matWorld;

				ani.SetWorldMatrix(&matWorld);
				ani.SetViewMatrix(m_pCamera->GetViewMatrix());
				ani.SetProjectedMatrix(m_pCamera->GetProjectionMatrix());
				D3DXMATRIX mat = CTextureProjective::GetShadowVPSMatrix(matWorld);
				ani.SetMatrixForTP(&mat);
				pBuild->GetModel()->pGameModel->ProcessAnimJoint(&ani);
				pBuild->GetModel()->pGameModel->DrawModelWithoutStateAndTexture();
			}
			rendbuildlist.clear();
		}
		CTextureProjective::ClearNum();
	}



	//��ȾͶӰ����
	if (!texprojlist.empty())
	{
		CTextureProjective::SetTextureState();
		CTextureProjective* p;
		for(std::list<CTextureProjective*>::iterator tit = texprojlist.begin();tit != texprojlist.end();)
		{
			p = (CTextureProjective*)(*tit);
			if (p->IsAlive())
			{

				for (std::list<CTextureProjective::POSDIRALPHATIME>::iterator it = p->m_listpostime.begin();it != p->m_listpostime.end();it++)
				{
					p->SetLookatPos(&(it->pos));
					p->SetNowDirY(it->fdiry);
					p->SetNowTime(&(it->time));
					p->SetNowAlpha(it->falpha);
					GetGameMap()->GetTagMapBuffer((int)p->GetPosX(),(int)p->GetPosZ(),p->GetSize(),CTextureProjective::GetBuffer(),rendbuildlist);
					D3DXMATRIX matid;
					D3DXMatrixIdentity(&matid);
					p->Rend(GetGame()->GetRegion()->GetCamera()->GetPosition(),matid,*(GetGame()->GetRegion()->GetCamera()->GetViewMatrix()),*(GetGame()->GetRegion()->GetCamera()->GetProjectionMatrix()));
					for (std::list<DWORD>::iterator it = rendbuildlist.begin();it != rendbuildlist.end();it++)
					{
						GameMap::tagGrid* tGrid =  GetGameMap()->GetGrid(*it);
						GameBuildList::Build *pBuild = tGrid->pBuild->pBulid;
						D3DXVECTOR3* vertexposarry = GetGameMap()->GetTerrainPositionArray();
						vCenter = (vertexposarry[tGrid->dwVertexIndex[0]] + vertexposarry[tGrid->dwVertexIndex[1]] + vertexposarry[tGrid->dwVertexIndex[2]] + vertexposarry[tGrid->dwVertexIndex[3]]) / 4.0f;

						D3DXMatrixTranslation(&matWorld, vCenter.x + tGrid->pBuild->vPosOffset.x, vCenter.y + tGrid->pBuild->vPosOffset.y, vCenter.z + tGrid->pBuild->vPosOffset.x);
						D3DXMatrixRotationYawPitchRoll(&matRot, tGrid->pBuild->vRotation.y, tGrid->pBuild->vRotation.x, tGrid->pBuild->vRotation.z);
						D3DXMatrixScaling(&matScaling, tGrid->pBuild->vScaling.x, tGrid->pBuild->vScaling.y, tGrid->pBuild->vScaling.z);
						matWorld = matScaling * matRot * matWorld;

						ani.SetWorldMatrix(&matWorld);
						ani.SetViewMatrix(m_pCamera->GetViewMatrix());
						ani.SetProjectedMatrix(m_pCamera->GetProjectionMatrix());
						D3DXMATRIX mat = p->GetTPVPSMatrix(matWorld);
						ani.SetMatrixForTP(&mat);
						pBuild->GetModel()->pGameModel->ProcessAnimJoint(&ani);
						pBuild->GetModel()->pGameModel->DrawModelWithoutStateAndTexture();
					}
					rendbuildlist.clear();
				}
				p->m_listpostime.clear();


			}
			else
			{
				tit++;
				CTextureProjective::DestroyTextureProjctive(p);
				continue;
			}
			tit++;

		}	
	}
	CTextureProjective::EndState();
}

void  CClientRegion::SetPointLight()
{
	m_RegionLight.Type = D3DLIGHT_POINT;
	m_RegionLight.Attenuation0 = 1.0f;
	m_RegionLight.Attenuation1 = 0.0f;
	m_RegionLight.Attenuation2 = 0.0f;
	m_RegionLight.Range = 20.0f;
	m_RegionLight.Diffuse.a = 1.0f;
	m_RegionLight.Diffuse.b = 1.0f;
	m_RegionLight.Diffuse.g = 1.0f;
	m_RegionLight.Diffuse.r = 1.0f;
	m_RegionLight.Ambient.a = 1.0f;
	m_RegionLight.Ambient.r = 0.1f;
	m_RegionLight.Ambient.g = 0.1f;
	m_RegionLight.Ambient.b = 0.1f;
	m_RegionLight.Specular.a = 0.0f;
	m_RegionLight.Specular.b = 0.0f;
	m_RegionLight.Specular.g = 0.0f;
	m_RegionLight.Specular.r = 0.0f;
	m_RegionLight.Direction = D3DXVECTOR3(0,0,0);
	float x = GetGame()->GetMainPlayer()->GetPosX();
	float y = GetGame()->GetMainPlayer()->GetPosY();
	float z = GetGame()->GetMainPlayer()->GetHeight();
}
void	CClientRegion::SetDirectLight()
{
	DWORD dwAmbient = m_pGameMap->GetSunLightAmbientColorforBuild();
	DWORD dwDiffuse = m_pGameMap->GetSunLightDiffuseColor();
	DWORD dwSpecular = m_pGameMap->GetSunLightSpecularColor();
	const D3DXVECTOR3* vlightdir = m_pGameMap->GetSunLightDirection();
	ZeroMemory( &m_RegionLight, sizeof(D3DLIGHT9) );
	/*m_RegionLight.Ambient.a = (float)((dwAmbient&0xff000000)>>24)/255.0f;
	m_RegionLight.Ambient.r = (float)((dwAmbient&0x00ff0000)>>16)/255.0f;
	m_RegionLight.Ambient.g = (float)((dwAmbient&0x0000ff00)>>8)/255.0f;
	m_RegionLight.Ambient.b = (float)(dwAmbient&0x000000ff)/255.0f;*/
	m_RegionLight.Diffuse.a = (float)((dwDiffuse&0xff000000)>>24)/255.0f;
	m_RegionLight.Diffuse.r = (float)((dwDiffuse&0x00ff0000)>>16)/255.0f;
	m_RegionLight.Diffuse.g = (float)((dwDiffuse&0x0000ff00)>>8)/255.0f;
	m_RegionLight.Diffuse.b = (float)(dwDiffuse&0x000000ff)/255.0f;
	m_RegionLight.Specular.a = (float)((dwSpecular&0xff000000)>>24)/255.0f;
	m_RegionLight.Specular.r = (float)((dwSpecular&0x00ff0000)>>16)/255.0f;
	m_RegionLight.Specular.g = (float)((dwSpecular&0x0000ff00)>>8)/255.0f;
	m_RegionLight.Specular.b = (float)(dwSpecular&0x000000ff)/255.0f;
	D3DXVec3Normalize((D3DXVECTOR3*)&m_RegionLight.Direction,vlightdir);
	m_RegionLight.Attenuation0 = 1;
	m_RegionLight.Attenuation1 = 0;
	m_RegionLight.Attenuation2 = 0;
	m_RegionLight.Type       = D3DLIGHT_DIRECTIONAL;
}



/*
* ����: ��ȡ�ͻ�����Դ
* ժҪ: -
* ����: -
* ����ֵ: ����true
* ����: -
* ��������: -
* �޸���־:
*	2008.03.03 - lpf
*		�����˶Ի��������б���и��º���Ⱦ�Ĳ���
*   2008.04.10 - lpf
*		�ڶԻ����������������Ⱦ��,��������̬����(���鶯��)����ʾ
*   2008.07.10 - lpf
*		�����˶���ͷ����Ϣ���ƵĿ���,��Ҫ��Ϊ�������Ϸ����ҳ��
*   2008.07.16 - lpf
*		�����˶���ͷ����Ϣ����ʱ,�ܽ�ɫ��ǰ�Ƿ�������״̬Ӱ��
*   2008.07.17 - lpf
*		������һЩͷ����Ϣ��ʾ��BUG
*   2008.07.23 - lpf
*		�����˵�ѡ��NPCʱ,ͷ���ƺ�ͼ����ʾ������
*		�������ͷ���Ĺ��Һ�����־�������������޸�
*   2008.07.24 - lpf
*		�ڻ�������ѭ��������,�����������Ч�Ĳ���
*   2008.07.28 - lpf
*		������ѡ�ж���������,����ʾͷ����Ϣ
*   2008.08.26 - lpf
*		��"��ʾNPC�͹�������"���޸����µ�NPC�������ֳƺ���ʾ����
*	2008.09.17 - lpf
*		������NPC���ֲ���ʾ����
*	2008.09.22 - lpf
*		��������Ϸ�й�굽�����Ե����˸����
*	2008.09.30 - lpf
*		�޸����µ���Һ͹���ͷ��Ѫ����ʾ����
*	2008.10.07 - lpf
*		���������й��ڳƺŸ߶ȵ���ʾ
*		��"��ʾ�����̵����������ͷ���ϵ�����"ѭ���ƶ���,Shape�б��д���
*	2008.10.14 - lpf
*		������"��Ⱦ����ģ��"
*	2008.11.11 - lpf
*		������"��ʾ��̬����",��Ҫ�Ƕ����Ĳ���
*	2008.11.14 - lpf
*		�޸��˹��ڻ������Update()���ĵ���,������һ��ƽ��ͷ��Ĳ�������
*	2008.12.11 - lpf
*		�޸����������������ʾ����,���Ƿ���ʾ�������ͷ����Ϣ���жϷ�����CPlayer::DisplayText()��
*	2008.12.28 - ����
*		�޸���ѡ������������ֺ����ʾ������ֻ����һ�Σ�
*	2008.12.30 - lpf
*		�ڶ����б��������ʾѭ���У������˶��������ͷ����Ϣ�Ƿ���ʾ�Ĵ���
*	2009.01.15 - lpf
*		��������������,��Ա���������������ѿ���Ч��,����ȷ��ʾͷ����Ϣ
*	2009.02.04 - lpf
*		�����ڵ��ϵ���Ʒ������Ϣ��ʾ,������CMainBarPageEx��ı���ֱ��Ӱ��
*	2009.02.09 - lpf
*		��2D��������������Ҵ�ֻ�þ���ֵʱ��ͷ����Ϣ��ʾ
*	2009.10.20 - lpf
*		��3D����Ⱦ���������ˮ�����Ⱦ
*	2009.10.26 - lpf
*		��2D������Ⱦ"������ʾ"��,�����˲ɼ������ʾ,������Ҫ��ʾ�ɼ���������Ϣ;
*/
bool CClientRegion::Display()
{
	CPlayer* pMainPlayer = GetGame()->GetMainPlayer();
	if (pMainPlayer==NULL) return false;

	if (m_pGameSkyBox)
	{
		m_pGameSkyBox->Render(GetGame()->GetRegion()->GetCamera());
	}
	// �������
	if (GetGame()->GetMouseType() < MC_COUNT)
	{
		long lMouseShapeType = MC_NORMAL;
        if (GetGame()->GetGameControl()->GetMouseShape())
        {
            switch(GetGame()->GetGameControl()->GetMouseShape()->GetType()) 
            {
            case TYPE_PLAYER:
                lMouseShapeType = MC_PLAYER_FOCUS;
                break;

            case TYPE_MONSTER:
                {
                    CMonster* pMonster = dynamic_cast<CMonster*>(GetGame()->GetGameControl()->GetMouseShape());
                    // �ܶԻ��ĶԻ�ͼ��
                    if(pMonster->IsCanTalk())
                    {
                        // ���ݶԻ�ͼ���ID���ı�����ͼ��
                        switch(pMonster->IsCanTalk())
                        {
                        case 2	:lMouseShapeType = MC_TRYON_GOODS;	break;
                        case 3	:lMouseShapeType = MC_NPC_QUEST;	break;
                        case 4	:lMouseShapeType = MC_MONEY;		break;
                        case 5	:lMouseShapeType = MC_GEAR;			break;
                        case 6	:lMouseShapeType = MC_GATE;			break;
                        default :lMouseShapeType = MC_NPC_FOCUS;	break;
                        }
                    }
                    // ���ܶԻ���NPC
                    else if(pMonster->GeteNpcType() == NT_Normal)
                        lMouseShapeType = MC_NORMAL;
                    // ����MONSTER
                    else
                    {
                        if (pMonster->IsDied()
                            &&((((CCollectionGoods*)pMonster)->GetOccuID()==DOccu_Sewing)
                            ||((CCollectionGoods*)pMonster)->GetOccuID()==DOccu_Other))
                        {
                            lMouseShapeType = MC_COLLECTION_PI;
                            //GetGame()->GetCGuiEx()->GetShapeDescPage()->SetDescForShape(GetGame()->GetMouse()->GetMouseX()+30,GetGame()->GetMouse()->GetMouseY()+20,pMonster);
                            break;
                        }
                        lMouseShapeType = MC_MONSTER_FOCUS;
                    }

                }
                break;

            case TYPE_COLLECTION:
                CCollectionGoods* pCollectionGoods = (CCollectionGoods*)GetGame()->GetGameControl()->GetMouseShape();
                if (pCollectionGoods!=NULL)
                {

                    if (pCollectionGoods->GetOccuID()==DOccu_Metallurgy&&pMainPlayer->GetAssOccu()==DOccu_Metallurgy)
                    {
                        lMouseShapeType = MC_COLLECTION_CAO;
                    }
                    else if (pCollectionGoods->GetOccuID()==DOccu_Smithing&&pMainPlayer->GetAssOccu()==DOccu_Smithing)
                    {
                        lMouseShapeType = MC_COLLECTION_KUANG;
                    }
                    else if (pCollectionGoods->GetOccuID()==DOccu_Sewing&&pMainPlayer->GetAssOccu()==DOccu_Sewing)
                    {
                        lMouseShapeType = MC_COLLECTION_PI;
                    }
                    else if (pCollectionGoods->GetOccuID()==DOccu_Other)
                    {
                        switch(pCollectionGoods->GetActionID())
                        {
                        case 2:
                            lMouseShapeType = MC_COLLECTION_KUANG;
                            break;
                        case 3:
                            lMouseShapeType = MC_COLLECTION_CAO;
                            break;
                        case 4:
                            lMouseShapeType = MC_COLLECTION_PI;
                            break;
                        default:
                            lMouseShapeType = MC_COLLECTION_RENWU;
                            break;
                        }
                    }      
                }
                break;
            }
        }
        else if (GetGame()->GetGameControl()->GetMouseShapeGoods())
        {
            lMouseShapeType = MC_GOODS_FOCUS;
        }
       
        // ������װ״̬
        if(GetGame()->GetGameControl()->GetIsInTryOnState())
        {
            lMouseShapeType = MC_TRYON_GOODS;
        }
        else
            GetGame()->GetGameControl()->SetIsInTryOnState(false);

        if (GetGame()->GetMouse()->GetMouseX() > 0 &&
            GetGame()->GetMouse()->GetMouseX() < SCREEN_WIDTH &&
            GetGame()->GetMouse()->GetMouseY() > 0 &&
            GetGame()->GetMouse()->GetMouseY() < SCREEN_HEIGHT)
			
			//��������� �����ui���ڸ��� ��ֻ���ó���ͨ���
			if(lMouseShapeType == MC_NORMAL)
				GetGame()->SetMouseCursor(lMouseShapeType);
			else if(!UIManager::isMouseRollOverWindow())
				GetGame()->SetMouseCursor(lMouseShapeType);
	}

	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer2D   *pLayer2D   = pInterface->GetLayer2D();
	render::Interface::Layer3D   *pLayer3D   = pInterface->GetLayer3D();
	LPDIRECT3DDEVICE9 pDevice = pInterface->GetDevice();

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	pInterface->GetDevice()->SetTransform(D3DTS_WORLD,&matWorld);
	pInterface->GetDevice()->SetTransform(D3DTS_VIEW,m_pCamera->GetViewMatrix());
	pInterface->GetDevice()->SetTransform(D3DTS_PROJECTION,m_pCamera->GetProjectionMatrix());

	static bool bHaveSpy = false;
	CPlayer *pMPlayer = GetGame()->GetMainPlayer();
	//// ������ͼ��Ӱ�Ĺ���λ��
	//if(GetGame()->GetSetup()->lShadow == 1)
	//	GetGame()->SetShadowMappingLight(D3DXVECTOR3(pMPlayer->GetPosX(), pMPlayer->GetHeight(), pMPlayer->GetPosY()));

	// ��⵱ǰ����Ƿ���������μ���״̬
	if(pMPlayer->GetIsSpy() == false)
		bHaveSpy = true;
	else
		bHaveSpy = false;

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"����ClientRegion()Shape�б� Start.");
#endif
	if (GetGame())
	{
		if (GetGame()->GetSetup()->lModel == 1)
		{
			static list<PLAYERDIST> listplayerdist;
			CPlayer* pmainplayer = GetGame()->GetMainPlayer();
			for (itBaseObject it=m_listObject.begin(); it!= m_listObject.end(); it++ )
			{
				CShape *pShape = (CShape*)(*it);
				CMoveShape *pMoveShape = (CMoveShape*)pShape;
				if( pShape && pShape->GetType() == TYPE_PLAYER&&pShape != pmainplayer
					&&pShape !=(CPlayer*)(GetGame()->GetGameControl()->GetSelectedShape())
					&&!pmainplayer->IsTeammate(pShape->GetExID()))
				{
					CPlayer* p = (CPlayer*)pShape;
					PLAYERDIST playerdist;
					playerdist.p = p;
					playerdist.dist = pow(pmainplayer->GetPosY() - p->GetPosY(),2) + pow(pmainplayer->GetPosX() - p->GetPosX(),2);
					listplayerdist.push_back(playerdist);
				}
			}
			if (!listplayerdist.empty())
			{
				listplayerdist.sort();
				list<PLAYERDIST>::iterator it = listplayerdist.begin();
				for (int i = 0;it != listplayerdist.end();it++)
				{
					if (i < GetGame()->GetSetup()->lnotsimplemodelnum)
					{

						it->p->SetSimpleModel(false);
						i++;	

					}
					else
					{
						it->p->SetSimpleModel(true);

					}

				}

				listplayerdist.clear();
			}


		}

	}

	for( itBaseObject it=m_listObject.begin(); it!= m_listObject.end(); it++ )
	{
		CShape *pShape = (CShape*)(*it);
		CMoveShape *pMoveShape = (CMoveShape*)pShape;
		if( pShape )
		{
			D3DXMATRIX matTeamptWorld;
			D3DXMatrixTranslation(&matTeamptWorld,pShape->GetPosY(),pShape->GetHeight(),pShape->GetPosX());
			if (pShape->GetDisplayModel()==NULL || GetFrustum()->BoxIntersectFrustum(&matTeamptWorld,pShape->GetDisplayModel()->GetSizeBox()) == false)
			{
				pShape->SetInViewRange(false);
				continue;
			}
			else
			{
				pShape->SetInViewRange(true);
			}

			//�����GM����״̬������ʾ
			if(pShape->GetType() == TYPE_PLAYER && ((CPlayer*)pShape)->GetIsGMConcealment() && !((CPlayer*)pShape)->IsSelf())
				continue;
			if (pShape->GetType() == TYPE_PLAYER/* && pShape == GetGame()->GetMainPlayer()*/)
				PlayEffectFoot(dynamic_cast<CPlayer*>(pShape));

			// ��ǰ���û���������״̬������������״̬����Һ͹������ǰ����ⶼ����ʾ��״��Ӱ��
			if((pShape->GetType() == TYPE_PLAYER ||pShape->GetType() == TYPE_MONSTER) && !bHaveSpy && pMoveShape->GetIsConceal() == false)
			{ 
				if (pShape == GetGame()->GetMainPlayer() || GetGame()->GetMainPlayer()->IsTeammate(pShape->GetExID()))
				{
					pShape->Display();
				}
				continue;
			}
			// ��ǰ������������״̬������������״̬����Һ͹�����ʾ������״������ʾӰ��
			else if((pShape->GetType() == TYPE_PLAYER || pShape->GetType() == TYPE_MONSTER )&& bHaveSpy && pMoveShape->GetIsConceal() == false)
			{
				pShape->Display();
			}
			// ��ʾ���з�����״̬�Ķ���
			else
			{
				pShape->Display();
				if(pShape->GetType() != TYPE_GOODS && pShape->GetType() != TYPE_EFFECT && pShape->GetType() != TYPE_SKILL && pShape->GetType() != TYPE_SUMMON)
				{
					if(pShape->IsInViewRange())
					{
						if(GetGame()->GetSetup()->lShadow == 2)
						{	
							if(render::Interface::GetInstance()->GetSetting()->bborderaddress)
							{
								// ��Ⱦ��Ӱ��ͼ
								if (pMoveShape->GetDisplayModel()->GetLoadState() == Load_Did)
								{
									pMoveShape->RendShadow();
								}
							}


						}
						else if(GetGame()->GetSetup()->lShadow == 0)
						{
							if(render::Interface::GetInstance()->GetSetting()->bborderaddress)
							{
								// ��Ⱦ��Ӱ��ͼ
								if (pMoveShape->GetDisplayModel()->GetLoadState() == Load_Did)
								{
									pMoveShape->RendShadow(true);
								}
							}
						}
					}
				}

			}

			if (g_bDebug && pShape->GetDisplayModel() )
			{
				if (GetKeyboard()->IsKeyDown(DIK_RSHIFT))
				{
					//��ʾѡ�����

					if (pShape->GetType() == TYPE_PLAYER)
					{
						CPlayer* pPlayer = (CPlayer*)pShape;
						if (pPlayer->IsRide()&&pPlayer->GetHorse()->GetDisplayModel())
						{
							pPlayer->GetHorse()->GetDisplayModel()->_DEBUG_RenderPickBox(pPlayer->GetHorse()->GetAnimInfo()->GetWorldMatrix(),0xFF00FF00);
						}
					}
					else
					{
						pShape->GetDisplayModel()->_DEBUG_RenderPickBox(pShape->GetAnimInfo()->GetWorldMatrix(),0xFF00FF00);
					}
				}
			}
		}
		pMoveShape->SetSimpleModel(false);
	}
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"����ClientRegion()Shape�б� End.");
#endif

	// ��Ⱦ����ģ��
	m_AlphaFadeObjectList.Render();

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ClientRegion()��ͼ���� Start.");
#endif
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"m_pGameMap->Update Start.");
#endif
	m_pGameMap->Update(m_pFrustum);
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"m_pGameMap->Update End.");
#endif
	static float  fa0 = 0.0f, fa1 = 0.0f, fa2 = 0.0f,lightRange = 10.0f;
	static float FOVY = 50.0F, fFarClipPlane = 88.f;
	D3DCOLOR  AmbientColor = D3DCOLOR_ARGB(255,0,0,0);
	static float	dwDensity	= 0.0f;
	//����ǳ��������ϣ�����¹�Դλ��
	if (m_eRegionLightType == RL_CAVE)
	{
#ifdef _RUNSTACKINFO_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"���ϸ��¹�Դλ�� Start.");
#endif
		CPlayer *pMPlayer = GetGame()->GetMainPlayer();
		float  x = pMPlayer->GetPosX();
		float	 y = pMPlayer->GetPosY();
		float  height = pMPlayer->GetHeight() + 3.5f;
		static float z = 0.0f;
		//���Ե��Դ����ϵ��

		if (GetAsyncKeyState(VK_NUMPAD7))
			fa0 += 0.01f;
		if(GetAsyncKeyState(VK_NUMPAD1))
			fa0 -= 0.01f;
		if (GetAsyncKeyState(VK_NUMPAD8))
			fa1 += 0.01f;
		if(GetAsyncKeyState(VK_NUMPAD2))
			fa1 -= 0.01f;
		if (GetAsyncKeyState(VK_NUMPAD9))
			fa2 += 0.001f;
		if(GetAsyncKeyState(VK_NUMPAD3))
			fa2 -= 0.001f;
		if(GetAsyncKeyState(VK_PRIOR))
			lightRange += 0.5f;
		if(GetAsyncKeyState(VK_NEXT))
			lightRange -= 0.5f;
		if (fa0<0.0f)
			fa0 = 0.0f;
		if (fa1<0.0f)
			fa1 = 0.0f;
		if (fa2<0.0f)
			fa2 = 0.0f;

		if (GetAsyncKeyState(VK_LEFT))
		{
			dwDensity += 0.005f;
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			dwDensity -= 0.005f ;
		}

		if (GetAsyncKeyState(VK_LSHIFT))
		{
			m_RegionLight.Ambient.a += 0.05f;
			m_RegionLight.Ambient.b += 0.05f;
			m_RegionLight.Ambient.g += 0.05f;
			m_RegionLight.Ambient.r += 0.05f;
		}
		if (GetAsyncKeyState(VK_RSHIFT))
		{
			m_RegionLight.Ambient.a -= 0.05f;
			m_RegionLight.Ambient.b -= 0.05f;
			m_RegionLight.Ambient.g -= 0.05f;
			m_RegionLight.Ambient.r -= 0.05f;
		}

		if (GetAsyncKeyState(VK_INSERT))
		{
			GetGame()->GetSetup()->lViewRange -= 1;
		}
		if (GetAsyncKeyState(VK_DELETE))
		{
			GetGame()->GetSetup()->lViewRange += 1;
		}
		if(GetAsyncKeyState(VK_HOME))
			z += 0.01f;

		if(GetAsyncKeyState(VK_END))
			z -= 0.01f;

		m_RegionLight.Attenuation0 = fa0;
		m_RegionLight.Attenuation1 = fa1;
		m_RegionLight.Attenuation2 = fa2;
		m_RegionLight.Range = lightRange;
		m_RegionLight.Position = D3DXVECTOR3(y,z+height,x);
		//������Ч
		pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE); //����
		pDevice->SetRenderState(D3DRS_FOGCOLOR, 0x00000000); //���������ɫ
		//�������˥����ʽΪָ��
		pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP );
		pDevice->SetRenderState(D3DRS_FOGDENSITY,*(DWORD *)(&dwDensity));
#ifdef _RUNSTACKINFO_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"���ϸ��¹�Դλ�� End.");
#endif
	}

	if (!GetKeyboard()->IsKeyDown(DIK_TAB) || g_bDebug==false)
	{
#ifdef _RUNSTACKINFO_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"�ǵ���ģʽ����Tab Start.");
#endif
		pLayer3D->SetLight(&m_RegionLight);
		pLayer3D->SetMaterial(&GetGameMap()->m_material);
		pLayer3D->SetFogEnable(GetGameMap()->GetFogEnable());
		RECT rc = {0,0,pInterface->GetWndWidth(),pInterface->GetWndHeight()};
		static D3DXMATRIX matTRS;
		static GameSelectRay cCameraRay;
		cCameraRay.CalculateRay(m_pCamera);
		if (m_eRegionLightType == RL_CAVE)
			m_pGameMap->Render(&cCameraRay,m_pCamera,false,true);
		else
			m_pGameMap->Render(&cCameraRay,m_pCamera,false,false);
#ifdef _RUNSTACKINFO_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"�ǵ���ģʽ����Tab End.");
#endif
	}
	else	// ��ʾ���Գ���
	{
#ifdef _RUNSTACKINFO_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"��ʾ���Գ��� Start.");
#endif
		render::Interface *pInterface = render::Interface::GetInstance();
		pInterface->GetDevice()->SetRenderState(D3DRS_LIGHTING,FALSE);
		_Debug_RenderMap();
#ifdef _RUNSTACKINFO_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"��ʾ���Գ��� End.");
#endif

	}
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ClientRegion()��ͼ���� End.");
#endif

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ClientRegion()����������� Start.");
#endif
	//����������
	for (int i = 0; i < 4; ++i)
	{
		if (m_apGameEnvCreature[i])
		{
			CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
			D3DXVECTOR3 vTemp(pMainPlayer->GetPosY(),pMainPlayer->GetHeight(),pMainPlayer->GetPosX());

			m_apGameEnvCreature[i]->Update(&vTemp, m_pFrustum);
			m_apGameEnvCreature[i]->Render(m_pGameMap,m_pCamera);

			//���԰汾
			if (g_bDebug && GetKeyboard()->GetCurrentKey() == DIK_F7)
			{
				// �ͷŻ�����������
				for (int i = 0; i < 4; ++i)
					SAFEDESTROY(m_apGameEnvCreature[i]);

				char strTempName[MAX_PATH];
				sprintf(strTempName, "regions/%d/envcreature", GetResourceID());
				if (utility::File::FileExist(strTempName))
				{
					utility::File file;
					utility::Text s;
					utility::Text::Node * pN;
					utility::Text::Variable * pA;
					char strTmp[MAX_PATH];
					string str;

					sprintf(strTmp, "regions/%d/", GetResourceID());

					file.Open(strTempName);
					s.Create(&file);
					file.Close();

					pN = s.GetRootNode()->GetChildNode("list");
					for (int i = 0; i < (int)pN->GetVarCount(); ++i)
					{
						pA = pN->GetVar((int)i);
						str = strTmp;
						str += pA->GetStringValue();
						m_apGameEnvCreature[i] = GameBaseEnvCreature::CreateEnvCreature(str.c_str());
					}

					s.Destroy();
				}
			}

		}
	}
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ClientRegion()����������� End.");
#endif


#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ClientRegion()��Ⱦ���� Start.");
#endif

	//��Ⱦ����
	particle::Manager::GetInstance()->Render();

	///����
	D3DXVECTOR3 POS = m_RegionLight.Position;
	pLayer3D->SetLight(&m_RegionLight);
	pLayer3D->SetMaterial(&GetGameMap()->m_material);
	pLayer3D->SetFogEnable(GetGameMap()->GetFogEnable());
	if (m_eRegionLightType == RL_OUTDOOR)
	{
		AmbientColor = m_pGameMap->GetSunLightAmbientColorforMoveShap();
	}
	pLayer3D->SetAmbient(AmbientColor);
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ClientRegion()��Ⱦ���� End.");
#endif

	GetGame()->SetCurrentRenderTriangleCount(pLayer3D->GetTriangleArrayUsage());
	//pLayer2D->Update();
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ClientRegion()��ȾͶӰ���� Start.");
#endif
	pLayer3D->SetMipMap(TRUE);
	pLayer3D->RenderTest();
	pLayer3D->RenderBlendLow();

	pLayer3D->EndEffectsState();
	//��ȾͶӰ����
	DisplayTextureProjective();
	Water::CManager::GetInstance()->Render(GetGameMap()->GetFogEnable());
	pLayer3D->RenderBlend();

	pLayer3D->Clear();

	//***�������IBMX200�����Կ�����BUG,���Ĳ���--��ǫ
	pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
	pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

	pInterface->GetLayer3D()->SetStreamSource(LMST_POS);

	pInterface->GetLayer3D()->SetStreamType(LMST_POS);

	pInterface->GetDevice()->DrawPrimitive( D3DPT_LINELIST, 0, 0);
	//�������IBMX200�����Կ�����BUG,���Ĳ���--��ǫ***///
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ClientRegion()��ȾͶӰ���� End.");
#endif

	//if(GetGame()->GetSetup()->lShadow == -1)
	//{
	//	// ��Ⱦ�����Ӱ
	//	GetGame()->RenderShadow();
	//}

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ClientRegion()����Ч�� Start.");
#endif
	//����Ч��
	if (m_pGameEnvEffect)
	{
		D3DXVECTOR3 vTemp(pMainPlayer->GetPosY(),pMainPlayer->GetHeight(),pMainPlayer->GetPosX());

		m_pGameEnvEffect->Update(&vTemp);
		m_pGameEnvEffect->Render(m_pCamera->GetBillboard(),m_pCamera->GetBillboardY());
		//���԰汾
		if (g_bDebug && GetKeyboard()->GetCurrentKey() == DIK_F8)
		{
			SAFEDESTROY(m_pGameEnvEffect);
			char strTempName[MAX_PATH];
			sprintf(strTempName, "regions/%d/enveffect", GetResourceID());
			m_pGameEnvEffect = GameBaseEnvEffect::CreateEnvEffect(strTempName);
		}
	}
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ClientRegion()����Ч�� End.");
#endif
	//
	//---------------------------------------------------------------------------------

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ClientRegion()������Ϣ����ʾ Start.");
#endif
	GameGraphics2D::SetUIModelRendState();
	// ������ʾҳ��Ŀ���
	//CGameConfigPageEx * pGamePage = GetGame()->GetCGuiEx()->GetGameConfigPageEx();
	bool bOtherNameCheck = false;//pPage->GetOtherPlayerName()->GetCheck();
	bool bNpcMonsterCheck = false;//pGamePage->GetOption(CGameConfigPageEx::OPTION_ShowNPCName);
	bool bMonsterCheck = false;//pGamePage->GetOption(CGameConfigPageEx::OPTION_ShowMonsterName);
	bool bShowCountryFlag = true;
	bool bShowPirateFlag = true;
	bool bMonsterBloodBarCheck = true;//pPage->GetMonsterBloodBar()->GetCheck();
	bool bEquipOnGroundNameCheck = false;//GetGame()->GetCGuiEx()->GetMainBarPageEx()->GetEquipOnGroundNameCheck();
	bool bSelfNameBloodBarCheck = false;//pPage->GetSelfNameBloodBar()->GetCheck();
	bool bMonsterTitleCheck = true;

	CMoveShape *pMoveShape = NULL;
	CMonster   *pMonster = NULL;

    long lPosX = pMainPlayer->GetTileX();
    long lPosY = pMainPlayer->GetTileY();
    static char strPos[128];
    sprintf_s(strPos,"X:%d Y%d",lPosX,lPosY);

    GetGame()->OutPutText(SCREEN_WIDTH - GetGame()->GetFontWidth(strPos)-5,5,strPos,(int)strlen(strPos),D3DCOLOR_ARGB(255,255,255,0));
	for( itBaseObject it=m_listObject.begin(); it!= m_listObject.end(); it++ )
	{
		CShape *pShape = dynamic_cast<CShape*>(*it);
		if (pShape == NULL)
			continue;
		//�����GM����״̬������ʾ
		if(pShape->GetType() == TYPE_PLAYER && ((CPlayer*)pShape)->GetIsGMConcealment() && !((CPlayer*)pShape)->IsSelf())
			continue;

		// ��ǰ���û���������״̬������������״̬����Һ͹������ǰ����ⶼ����ʾ����
		pMoveShape = dynamic_cast<CMoveShape *>(pShape);
		if (pMoveShape != NULL)
		{
			if ((pShape->GetType() == TYPE_PLAYER || pShape->GetType() == TYPE_MONSTER) && !bHaveSpy && pMoveShape->GetIsConceal() == false)
			{
				if (pShape != GetGame()->GetMainPlayer() && !GetGame()->GetMainPlayer()->IsTeammate(pShape->GetExID()))
					continue;
			}
		}

		// NPC���������ʾ��ʾ
		pMonster=dynamic_cast<CMonster*>(pShape);
		if(pMonster != NULL)
		{
			// ��ø�NPC�����������Ч
			long lEffectID = pMonster->GetStateByQuest();
			// ��ʾ��������ص�״̬����ʾ����
			if(lEffectID != -1)
			{
				DisplayQuestInfoByNpc(pMonster,lEffectID);
			}
			// ���������BOSS���ģ���ʾ��Ѫ��
			if(m_pBossImage && m_pBossBackImage)
			{
				if(pMonster->GetBossLevel() == 2)
				{
					int x = (SCREEN_WIDTH - m_pBossBackImage->GetWidth()) / 2;
					int y = SCREEN_HEIGHT - m_pBossBackImage->GetHeight() * 4;
					RECT rc;
					rc.left   =  0;
					rc.right  =  (long)(((float)pMonster->GetHp() / (float)pMonster->GetMaxHp()) * m_pBossImage->GetWidth());
					rc.top    =  0;
					rc.bottom = m_pBossImage->GetHeight();
					m_pBossBackImage->Draw(x,y);
					m_pBossImage->Draw(x+8,y+7,rc);
				}
			}
		}

		// ��ʾ��̬����
		if (pShape->GetType() == TYPE_PLAYER || pShape->GetType() == TYPE_MONSTER)
		{
			pMoveShape = dynamic_cast<CMoveShape*>(pShape);
			if (pMoveShape != NULL)
			{
				pMoveShape->PlayFaceAnim();
				pMoveShape->PlayActionAnim();
				pMoveShape->PlayActionSound();
			}
		}

		// ��ʾ����shape������
		if (GetKeyboard()->IsKeyDown(DIK_TAB))
		{
			float fPos = DisplayShapeName(pShape, true, true);

			// ��ӳƺ���ʾ
			pMonster=dynamic_cast<CMonster*>(pShape);
			if(pMonster != NULL)
				DisplayMonsterTitle(pMonster, true, fPos);

			// ��ʾ����ͷ����Ϣ
			CPlayer * pPlayer = dynamic_cast<CPlayer*>(*it);
			if (pPlayer != NULL)
			{				
				if (pPlayer->IsPlayerShopOpened())
					ShowShopName(pPlayer, fPos);
				else if (pPlayer->IsRecruitingTeam())
					ShowPlayerRecruiting(pPlayer, fPos);

				// ������ҵ�ͷ����ʾ����
				if (pPlayer != GetGame()->GetMainPlayer() && pPlayer->GetPlayerInfo().size() > 0)
					ShowPlayerInfo(pPlayer);
			}

			continue;
		}

		// ��ʾָ���shape
		if (GetGame()->GetGameControl()->GetMouseShape() == pShape &&
			GetGame()->GetGameControl()->GetSelectedShape() != pShape)
		{
			float fPos = DisplayShapeName(pShape, true, true);

			// ��ӳƺ���ʾ
			pMonster=dynamic_cast<CMonster*>(pShape);
			if(pMonster != NULL)
				DisplayMonsterTitle(pMonster, true, fPos);

			// ��ʾ����ͷ����Ϣ
			CPlayer * pPlayer = dynamic_cast<CPlayer*>(*it);
			if (pPlayer != NULL)
			{				
				if (pPlayer->IsPlayerShopOpened())
					ShowShopName(pPlayer, fPos);
				else if (pPlayer->IsRecruitingTeam())
					ShowPlayerRecruiting(pPlayer, fPos);

				// ������ҵ�ͷ����ʾ����
				if (pPlayer != GetGame()->GetMainPlayer() && pPlayer->GetPlayerInfo().size() > 0)
					ShowPlayerInfo(pPlayer);
			}

			continue;
		}

		// ��ʾѡ�е���Ʒ
		if (GetGame()->GetGameControl()->GetMouseShapeGoods() == pShape)
		{
			DisplayShapeName(pShape, true, true);
			continue;
		}

		// ������ʾ
		switch(pShape->GetType())
		{
		case TYPE_PLAYER:
			{
				float fPos = 0.0f;

				//��ʾ�����������
				if (pShape != GetGame()->GetMainPlayer())
				{
					if( pShape != GetGame()->GetGameControl()->GetSelectedShape())
						fPos = DisplayShapeName(pShape, true, true);
					else
					{
						CPlayer * pOtherPlayer = dynamic_cast<CPlayer*>(pShape);
						//������ڵж�״̬�򹥳�״̬ʱ��ʾplayer��ս��״̬
						if( pOtherPlayer->IsHaveEnemyFactions() ||
							pOtherPlayer->IsHaveCityWarEnemyFactions() ||
							GetWarType() == RWT_VillageWar )//���ߵ�ǰ�ڴ�ս��ͼ
						{
							if( pShape != GetGame()->GetGameControl()->GetMouseShape())
							{
								pShape->DisplayWarBitmap();
							}	
						}
					}
				}
				else // �Լ�
				{
					//��ʾ����Ѫ��������
					if (bSelfNameBloodBarCheck)
						fPos = DisplayShapeName(pShape, true, true);
					else
					{
						if (GetGame()->GetGameControl()->GetSelectedShape() != pShape)
							fPos = DisplayShapeName(pShape, true, false);

						//������ڵж�״̬�򹥳�״̬ʱ��ʾplayer��ս��״̬
						if( pMainPlayer->IsHaveEnemyFactions() ||
							pMainPlayer->IsHaveCityWarEnemyFactions() ||
							GetWarType() == RWT_VillageWar )//���ߵ�ǰ�ڴ�ս��ͼ
						{
							if( pShape != GetGame()->GetGameControl()->GetMouseShape())
							{
								pShape->DisplayWarBitmap();
							}
						}
					}
				}

				// ��ʾ����ͷ����Ϣ
				CPlayer * pPlayer = dynamic_cast<CPlayer*>(*it);
				if (pPlayer != NULL && fPos != 0.0f)
				{				
					if (pPlayer->IsPlayerShopOpened())
						ShowShopName(pPlayer, fPos);
					else if (pPlayer->IsRecruitingTeam())
						ShowPlayerRecruiting(pPlayer, fPos);

					// ������ҵ�ͷ����ʾ����
					if (pPlayer != GetGame()->GetMainPlayer() && pPlayer->GetPlayerInfo().size() > 0)
						ShowPlayerInfo(pPlayer);
				}
			}
			break;
		case TYPE_PET:
			{
				CPet *pPet = (CPet*)pShape;
				bool isShowName = false;//GetGame()->GetCGuiEx()->GetGameConfigPageEx()->GetOption(CGameConfigPageEx::OPTION_ShowMyName);
				DisplayShapeName(pShape, isShowName, true);
			}
			break;
		case TYPE_MONSTER:
			{
				//pMonster=dynamic_cast<CMonster*>(pShape);
				if(pMonster == NULL)
					break;
				//// �����ع���
				//if(pMonster->GetPetInfo()->lMasterType && pMonster->GetPetInfo()->MasterID != CGUID::GUID_INVALID)
				//{
				//	CPlayer *pMaster=dynamic_cast<CPlayer *>(FindChildObject(pMonster->GetPetInfo()->lMasterType,pMonster->GetPetInfo()->MasterID));
				//	if (pMaster)
				//	{
				//		//�ж�������ҵĳ���
				//		if(pMonster->GetPetInfo()->MasterID != pMainPlayer->GetExID() )
				//		{
				//			if(bOtherNameCheck)
				//				DisplayShapeName(pShape, true, true);
				//		}
				//		else // �Լ���
				//		{
				//			if (bSelfNameBloodBarCheck)
				//				DisplayShapeName(pShape, true, true);
				//		}
				//	}
				//}
				//else
				{	//��ʾNPC�͹�������
					if (pMonster->GeteNpcType() == NT_Normal)
					{
						if (bNpcMonsterCheck && GetGame()->GetGameControl()->GetSelectedShape() != pShape)
						{
							float fPos = DisplayShapeName(pShape, true, false);
							DisplayMonsterTitle(pMonster, true, fPos);
						}
					}
					// ��ͨ����
					else if (bMonsterCheck || bMonsterBloodBarCheck)
					{
						float fDrawPos = DisplayShapeName(pShape, bMonsterCheck, bMonsterBloodBarCheck);

						if (GetGame()->GetGameControl()->GetSelectedShape() != pShape)
						{
							// ��ӳƺ���ʾ
							DisplayMonsterTitle(pMonster, bMonsterCheck, fDrawPos);
						}
					}
				}
			}
			break;

		case TYPE_GOODS:
			{
				//��ʾ����װ��������
				if(bEquipOnGroundNameCheck || GetKeyboard()->IsKeyDown(DIK_LALT) || GetKeyboard()->IsKeyDown(DIK_RALT))
					DisplayShapeName(pShape, true, true);
			}
			break;
		case TYPE_COLLECTION:
			DisplayShapeName(pShape, true);
			break;
		}
	}

	// ��һ�þ���ֵ��ʾ
	GetGame()->GetMainPlayer()->DisplayTipText();

	// ��ʾ��������Ŀ����
	CShape * pLockShape = GetGame()->GetGameControl()->GetCurAttractShape();
	long lLockType = 0;
	CGUID LockID = CGUID::GUID_INVALID;
	if(pLockShape)
	{
		lLockType = pLockShape->GetType();
		LockID = pLockShape->GetExID();
	}

	if (lLockType==0 || LockID==CGUID::GUID_INVALID)
	{
		pLockShape = GetGame()->GetGameControl()->GetSelectedShape();
		if(pLockShape)
		{
			lLockType = pLockShape->GetType();
			LockID = pLockShape->GetExID();
		}
	}

	if (lLockType && LockID != CGUID::GUID_INVALID)
	{
		CMoveShape *pLockTarget = dynamic_cast<CMoveShape*>(FindChildObject(lLockType,LockID));
		if (pLockTarget)
		{
			pLockTarget->DisplayHoverArrow(3);
		}
	}

	//��ʾѡ��Ŀ��ı��
	if (GetGame()->GetGameControl()->GetSelectedShape())
	{
		if (GetGame()->GetGameControl()->GetSelectedShape()->GetType() == lLockType &&
			GetGame()->GetGameControl()->GetSelectedShape()->GetExID() == LockID)
		{
		}
		else
		{
			DisplayShapeHoverArrow(GetGame()->GetGameControl()->GetSelectedShape());
		}
	}

	// ��ʾ��Ѫ���ֺͶԻ�
	for( itBaseObject it=m_listObject.begin(); it!= m_listObject.end(); it++ )
	{
		CMoveShape *pShape = dynamic_cast<CMoveShape*>(*it);
		if(pShape)
		{	
			float pos=0.0f;
			if(pShape->GetType()==TYPE_PLAYER)
			{
				//�����GM����״̬������ʾ
				if(((CPlayer*)pShape)->GetIsGMConcealment() && !((CPlayer*)pShape)->IsSelf())
					continue;

				CPlayer* pPlayer = (CPlayer*)pShape;
				//���ҹٽ�
				pos -= GetGame()->GetFontHeight(-1)+5;
				//��������л� ���ֻ�����һ��
				if(pPlayer->GetFactionID()!=NULL_GUID)
					pos -= GetGame()->GetFontHeight(-1)+5;
				//�������ļ���̵� ���ֻ�����һ��
				if(pPlayer->IsRecruitingTeam() || pPlayer->IsPlayerShopOpened())
					pos -= GetGame()->GetFontHeight(-1) + 8;	
			}
			pShape->DisplayLayer2D(pos);
		}
	}

	//��ʾ�ű��ļ�ʱ��
	long lTime = GetGame()->GetScriptTimer();
	if(lTime > 0)
	{
		long lMimute = lTime/60;
		long lSecond = lTime%60;
		long lHour   = lMimute/60;
		lMimute      = lMimute%60;
		char strTime[100];
		sprintf(strTime,"%02d:%02d:%02d",lHour,lMimute,lSecond);
		int nPosY = 2;
		int nPosX = SCREEN_WIDTH/2-20;
		GetGame()->OutPutText(nPosX,nPosY,strTime,(int)strlen(strTime),D3DCOLOR_ARGB(255,255,255,255));
	}

	//��ʾ�����������ʱ
	if(GetGame()->GetMainPlayer()->GetCityWarDiedState() == true)
	{
		char strDiedState[100];
		sprintf(strDiedState,"��������״̬�������ƶ���(%d��)",GetGame()->GetDiedStateTime()/1000);

		D3DXVECTOR3 pos(GetGame()->GetMainPlayer()->GetPosY(),
			1.5f+GetGame()->GetMainPlayer()->GetHeight(),GetGame()->GetMainPlayer()->GetPosX());
		D3DXMATRIX matFinal;
		CalculateFinalTransformMatrix(matFinal);
		D3DXVECTOR3 pos2;
		D3DXVec3TransformCoord(&pos2,&pos,&matFinal);

		int len = GetGame()->GetFontWidth(strDiedState);

		long x = (long)pos2.x - len / 2;
		long y = (long)pos2.y;
		GetGame()->DrawSolidQuadRect(x-2, y-2, x+len+2, y+GetGame()->GetFontHeight(-1)+2, D3DCOLOR_ARGB(255,0,0,0));
		GetGame()->OutPutText(x, y+1, strDiedState, len, 0xffffffff);
	}


	//��ʾ������ʾ��Ϣ
	GetGame()->DrawTopInfos();

	if (g_bDebug &&GetKeyboard()->IsKeyDown(DIK_TAB))
	{
		pLayer3D->SetFillMode(D3DFILL_WIREFRAME);
		char str[256];
		Vector3 vPos( GetGame()->GetMainPlayer()->GetPosY(),
			GetGame()->GetMainPlayer()->GetHeight(), GetGame()->GetMainPlayer()->GetPosX());

		sprintf(str, "%d,%d \nArea(%f,%f,%f)\nObj=%d pLAYER:DIR%d\n",
			GetMouse()->GetMouseX(), GetMouse()->GetMouseY(),
			vPos.x, vPos.y, vPos.z,	GetObjectList()->size(),GetGame()->GetMainPlayer() ? GetGame()->GetMainPlayer()->GetDir():-1);
		GetGame()->OutPutText(0, 60, str, lstrlen(str), 0xffffffff);

		// ��ʾ3D����λ�úͳ���
		//Vector3 v = GetAudioEngine()->GetListenerPos();
		//char t[255];
		//sprintf(t,"����3Dλ�� x:%f y:%f z:%f",v.x,v.y,v.z);
		//GetGame()->OutPutText(140, 0, t, 255, D3DCOLOR_ARGB(255,255,255,255));

		//Vector3 face,up;
		//GetAudioEngine()->GetListenerOri(face,up);
		//sprintf(t,"����3D���� FACE(%f, %f, %f) UP(%f, %f, %f)",face.x,face.y,face.z,up.x,up.y,up.z);
		//GetGame()->OutPutText(140, GetGame()->GetFontWidth(-1)+10, t, 255, D3DCOLOR_ARGB(255,255,255,255));

		//GetCamera()->GetPosition();
		//char szCamera[256];
		//sprintf(szCamera, "�����λ�ã�%f, %f, %f", GetCamera()->GetPosition()->x, GetCamera()->GetPosition()->y, GetCamera()->GetPosition()->z );
		//GetGame()->OutPutText(0, 100, szCamera, 255, 0xffffffff );

		//float fDistance = sqrt((vPos.x - v.x) * (vPos.x - v.x) + (vPos.y - v.y) * (vPos.y - v.y) + (vPos.z - v.z) * (vPos.z - v.z));
		//char szDistance[256];
		//sprintf(szDistance, "���룺%f", fDistance);
		//GetGame()->OutPutText(0, 130, szDistance, 255, 0xffffffff);

		//int i = 0;
		//char szSound[1024];
		//AudioEngine::AudioBufsbyFileNameIter iterBeg = GetGame()->GetAudioEngine()->m_audioBufsbyFileName.begin();
		//AudioEngine::AudioBufsbyFileNameIter iterEnd = GetGame()->GetAudioEngine()->m_audioBufsbyFileName.end();
		//for (; iterBeg != iterEnd; ++iterBeg)
		//{
		//	if (!(*iterBeg).second)
		//		continue;

		//	sprintf(szSound, "�ļ���%s, λ�ã�%f, %f, %f", (*iterBeg).first.c_str(), (*iterBeg).second->GetPosition(0).x, (*iterBeg).second->GetPosition(0).y, (*iterBeg).second->GetPosition(0).z);
		//	GetGame()->OutPutText(0, 150 + i * 20, szSound, 255, 0xffff0000);
		//	i++;
		//}
	}
	else
	{
		pLayer3D->SetFillMode(D3DFILL_SOLID);
	}
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ClientRegion()������Ϣ����ʾ End.");
#endif

	static char	LightDesp[128];
	if (m_eRegionLightType == RL_CAVE)
	{
		D3DCOLOR	FontColor = D3DCOLOR_ARGB(255,255,0,0);
		sprintf(LightDesp, "���Դ���շ�Χ�뾶 = %f",	m_RegionLight.Range);
		GetGame()->OutPutText(50,120,LightDesp,(int)strlen(LightDesp),FontColor);
		sprintf(LightDesp, "���Դ˥��ϵ��1 = %f",fa0);
		GetGame()->OutPutText(50,140,LightDesp,(int)strlen(LightDesp),FontColor);
		sprintf(LightDesp, "���Դ˥��ϵ��2 = %f",fa1);
		GetGame()->OutPutText(50,160,LightDesp,(int)strlen(LightDesp),FontColor);
		sprintf(LightDesp, "���Դ˥��ϵ��3 = %f",fa2);
		GetGame()->OutPutText(50,180,LightDesp,(int)strlen(LightDesp),FontColor);
		sprintf(LightDesp, "D3D��Ч��� = %f",dwDensity);
		GetGame()->OutPutText(50,200,LightDesp,(int)strlen(LightDesp),FontColor);
		sprintf(LightDesp, "���Դ�߶� = %f",m_RegionLight.Position.y);
		GetGame()->OutPutText(50,220,LightDesp,(int)strlen(LightDesp),FontColor);
		sprintf(LightDesp, "����Զ��������� = %d",GetGame()->GetSetup()->lViewRange);
		GetGame()->OutPutText(50,240,LightDesp,(int)strlen(LightDesp),FontColor);
		sprintf(LightDesp, "���Դ������ǿ�� = %f",m_RegionLight.Ambient.r);
		GetGame()->OutPutText(50,260,LightDesp,(int)strlen(LightDesp),FontColor);

	}
	//sprintf(LightDesp, "D3Dƽ�ؾ������������� = %f",FOVY);
	//GetGame()->OutPutText(50,220,LightDesp,strlen(LightDesp),0xffff0000);
	GameGraphics2D::RecoverFogState();

	CPlayer* player = GetGame()->GetMainPlayer();
	CPet *pPet = player->GetPet(player->GetActivePetGUID());
	if (pPet)
	{
		static char temp[256] = "";
		D3DCOLOR	FontColor = D3DCOLOR_ARGB(255,255,0,0);
		char sActionMode[20] = "����";
		switch (pPet->GetAction())
		{
		case CShape::ACT_STAND:strcpy(sActionMode,"վ��"); break;
		case CShape::ACT_WALK:strcpy(sActionMode,"����"); break;
		case CShape::ACT_RUN:strcpy(sActionMode,"����"); break;
		case CShape::ACT_IDLE:strcpy(sActionMode,"��Ϣ"); break;
		case CShape::ACT_ATTACK:strcpy(sActionMode,"����"); break;
		case CShape::ACT_PERFORM:strcpy(sActionMode,"ʩ��"); break;
		case CShape::ACT_INTONATE:strcpy(sActionMode,"����"); break;
		case CShape::ACT_DIED:strcpy(sActionMode,"����"); break;
		default:;break;
		}
		sprintf(temp,"���ﶯ��״̬: %s",sActionMode);
		GetGame()->OutPutText(50,180,temp,(int)strlen(temp),FontColor);

		sprintf(temp,"���＼��Send:%ld",pPet->GetPetSkillSendMsgCount());
		GetGame()->OutPutText(50,200,temp,(int)strlen(temp),FontColor);

		sprintf(temp,"����ʧ����Ϣ:%ld",pPet->GetPetSkillFailRecvMsgCount());
		GetGame()->OutPutText(50,220,temp,(int)strlen(temp),FontColor);

		sprintf(temp,"���ܳɹ���Ϣ:%ld",pPet->GetPetSkillSuccRecvMsgCount());
		GetGame()->OutPutText(50,240,temp,(int)strlen(temp),FontColor);

		sprintf(temp,"����ֹͣ��Ϣ: %d",pPet->GetStopMoveMsg());
		GetGame()->OutPutText(50,260,temp,(int)strlen(temp),FontColor);

		//sprintf(temp,"���﷢���ƶ���Ϣ����: %d",pPet->GetMoveMsg());
		//sprintf(temp,"�Զ�������Ϣ: %d",pPet->GetPetCountAttMsgCount());
		sprintf(temp,"�ƶ�����ֵ:X: %f  Y: %f",abs(pPet->GetDestX() -  pPet->GetPosX()) ,abs(pPet->GetDestY() -  pPet->GetPosY()) );
		GetGame()->OutPutText(50,280,temp,(int)strlen(temp),FontColor);

		char strBehavior[2][10] = {"ͣ��","����"};
		sprintf(temp,"������Ϊģʽ: %s",strBehavior[pPet->GetPetBehavior()-1]);
		GetGame()->OutPutText(50,300,temp,(int)strlen(temp),FontColor);

		const char sFightMode[3][10] = {"����","����","����"};
		sprintf(temp,"���﹥��ģʽ: %s",sFightMode[pPet->GetPetFightMode()]);
		GetGame()->OutPutText(50,320,temp,(int)strlen(temp),FontColor);

		float iDestX = pPet->GetDestX();
		float iDestY = pPet->GetDestY();
		//sprintf(temp,"�ƶ�Ŀ��:X: %d Y: %d", iDestX, iDestY);
		//GetGame()->OutPutText(50,340,temp,strlen(temp),FontColor);

		//CMoveShape *pAttShape = pPet->GetCurAttackShape();
		//if (pAttShape)
		//{
		//	sprintf(temp,"��ǰ������Ŀ�� ����:%s λ��:X: %ld Y: %ld", pAttShape->GetName(),pAttShape->GetTileX(), pAttShape->GetTileY());
		//	GetGame()->OutPutText(50,300,temp,strlen(temp),FontColor);
		//}

		//if (GetGame()->GetKeyboard()->GetCurrentKey() == DIK_NUMPAD0)
		//{
		//	int ms = pPet->GetPetBehavior() + 1;
		//	if (ms > PET_AS_FOLLOW)
		//		ms = PET_AS_STOP;
		//	pPet->SetPetBehavior((PET_MOVE_STYLE)ms);
		//}
		//if (GetGame()->GetKeyboard()->GetCurrentKey() ==DIK_NUMPAD1)
		//{
		//	int fs = pPet->GetPetFightMode() + 1;
		//	if (fs > PET_AS_ACTIVE)
		//		fs = PET_AS_PASSIVE;
		//	pPet->SetPetFightMode((PET_FIGHT_STYLE)fs);
		//}
		const char StrState[3][10] = {"��ƽ","ս��","����"};
		sprintf(temp,"��ǰ����״̬: %s", StrState[pPet->GetState()]);
		GetGame()->OutPutText(50,340,temp,(int)strlen(temp),FontColor);

		sprintf(temp,"����״̬: %s", StrState[GetGame()->GetMainPlayer()->GetState()]);
		GetGame()->OutPutText(50,360,temp,(int)strlen(temp),FontColor);

		sprintf(temp,"�����ƶ��ٶ�: %5.5f", GetGame()->GetMainPlayer()->GetSpeed());
		GetGame()->OutPutText(50,380,temp,(int)strlen(temp),FontColor);

		sprintf(temp,"�����ƶ��ٶ�: %5.5f", pPet->GetSpeed());
		GetGame()->OutPutText(50,400,temp,(int)strlen(temp),FontColor);

		float fposx = pPet->GetPosX();
		float fposy = pPet->GetPosY();
		sprintf(temp,"���ﵱǰλ�� X: %5.2f,  Y: %5.2f",fposx,fposy);
		GetGame()->OutPutText(50,420,temp,(int)strlen(temp),FontColor);

		float fdx = pPet->GetDestX();
		float fdy = pPet->GetDestY();
		sprintf(temp,"������һ��λ�� X: %5.2f,  Y: %5.2f",fdx,fdy);
		GetGame()->OutPutText(50,440,temp,(int)strlen(temp),FontColor);

		float fPlayerdx = GetGame()->GetMainPlayer()->GetPosX();
		float fPlayerdy = GetGame()->GetMainPlayer()->GetPosY();
		sprintf(temp,"����λ�ã�X: %5.2f,  Y: %5.2f",fPlayerdx,fPlayerdy);
		GetGame()->OutPutText(50,460,temp,(int)strlen(temp),FontColor);

		static char * strInfo[6] = {"FMR_Stop","FMR_Trace_Near","FMR_Trace_Far","FMR_Attack","FMR_No_TARGET","FMR_Other"};
		long lPetFightAI_mode = pPet->GetPetFightAI_Mode();

		switch (lPetFightAI_mode)
		{
		case	CPet::FMR_Trace_Near:
			strcpy(temp,"Ŀ�����");
			break;
		case	CPet::FMR_Trace_Far:
			strcpy(temp,"׷�ٹ���Ŀ��");
			break;
		case CPet::FMR_Attack:
			strcpy(temp,"����");
			break;
		case CPet::FMR_No_TARGET:
			strcpy(temp,"û�й���Ŀ��");
			break;
		case CPet::FMR_Other:
			strcpy(temp,"�������");
			break;
		case CPet::FMR_SKILL_ACTION_NOT_FINISH:
			strcpy(temp,"���ܶ���û���");
			break;
		case CPet::FMR_SKILL_CD_NOT_ARRIVAL:
			strcpy(temp,"���ܻ�δ��ȴ");
			break;
		case CPet::FMR_SKILL_NOT_FINISH:
			strcpy(temp,"��һ�����ܻ�δ����");
			break;
		case CPet::FMR_SKILL_LEVEL_FAIL:
			strcpy(temp,"���ܵȼ���������������");
			break;
		case CPet::FMR_SKILL_IS_NOT_AUTO:
			strcpy(temp,"���ܲ����Զ��ͷ�");
			break;
		case CPet::FMR_SKILL_NOT_ENOUGH_MP:
			strcpy(temp,"��������MP����");
			break;
		case CPet::FMR_NO_SKILL:
			strcpy(temp,"����û�м���");
			break;
		default:
			break;
		}
		GetGame()->OutPutText(50,480,temp,(int)strlen(temp),FontColor);
		for(long i = 0;i < (long)pPet->GetPetSkillCount();i++)
		{
			tagPetSkillInfo	*pSkill = pPet->GetPetSkillInfoByPos(i);
			if (pSkill)
			{
				DWORD	dwTimeElaps = GetCurTickCount() - pSkill->lStartCoolTime;
				if ((long)dwTimeElaps < pSkill->lRestorTimeLength )
					sprintf(temp,"����ID:%ld  ������ȴing...", pSkill->lID);
				else
					sprintf(temp,"����ID:%ld  ����ȴ״̬", pSkill->lID);

				GetGame()->OutPutText(50,500 + i*20,temp,(int)strlen(temp),FontColor);
			}
		}
	}
	return true;
}



void CClientRegion::ShowPlayerInfo(CPlayer *pPlayer)
{
	int count=0;
	for(size_t i=pPlayer->GetPlayerInfo().size()-1;i>=0;i--)
	{
		CPlayer::tagPlayerInfo stPlayerInfo=pPlayer->GetPlayerInfo()[i];

		float fHeight=2.5;
		D3DXVECTOR3 pos(pPlayer->GetPosY(),fHeight+pPlayer->GetHeight(),pPlayer->GetPosX());
		D3DXMATRIX matFinal;
		GetGame()->GetRegion()->CalculateFinalTransformMatrix(matFinal);
		D3DXVECTOR3 pos2;
		D3DXVec3TransformCoord(&pos2,&pos,&matFinal);

		pos.x = pPlayer->GetPosY();
		pos.y = fHeight+pPlayer->GetHeight();

		pos.z = pPlayer->GetPosX();
		GetGame()->GetRegion()->CalculateFinalTransformMatrix(matFinal);

		D3DXVec3TransformCoord(&pos2,&pos,&matFinal);

		pos2.y -= GetGame()->GetFontHeight(-1) + 2;
		//��������л� ���ֻ�����һ��
		if(pPlayer->GetFactionID()!=NULL_GUID)
			pos2.y -= GetGame()->GetFontHeight(-1);
		//�������ļ���̵� ���ֻ�����һ��
		if(pPlayer->IsRecruitingTeam() || pPlayer->IsPlayerShopOpened())
			pos2.y -= (GetGame()->GetFontHeight(-1)-2);
		else
			pos2.y += (GetGame()->GetFontHeight(-1))-7;
	}
}



/*
* ����: ��ʾ��ļ��Ϣ
* ժҪ: -
* ����: pPlayer	- ���ָ��
*		fPos	- ��ʾ��Ϣ�߶�
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.10.07 - lpf
*		 ��Ϣ��ʾ�ĸ߶����ⲿ������
*/
void CClientRegion::ShowPlayerRecruiting(CPlayer * pPlayer, float fPos)
{
	char strText[256];
    string strRecName = AppFrame::GetText("TM_16"); //�Ķ���
	if( lstrlen(pPlayer->GetTeamname()) > 0 &&  strlen(pPlayer->GetTeamname()) < 30 )
		strRecName = pPlayer->GetTeamname();
	else
	{
		// ����������ظ�����Ϣ��
		CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
		// ��������Ϣ�Ķ�������ļ
		if(pPlayer->GetIsHideInfo() && pMainPlayer != pPlayer && pMainPlayer->IsTeammate(pPlayer->GetExID()))
		{
			tagWSMemberInfo * stInfo = pMainPlayer->GetWsMemberInfo(pPlayer->GetExID());
			if(stInfo)
				strRecName = stInfo->szName + strRecName;
		}
		else
			strRecName = pPlayer->GetName()+strRecName;
	}
	if(strRecName.length() > 18)
	{
		// �����м�
		//if(CEditField::IsCursorInHz(14,strRecName))
		//	strRecName = strRecName.substr(0,13);
		//else
		//	strRecName = strRecName.substr(0,14);
		//strRecName += "...";
	}
	sprintf_s(strText,"%s",strRecName.c_str());

	int lLen = lstrlen(strText);
	long lFontWidth = GetGame()->GetFontWidth(strText);

	// ������
	D3DXVECTOR3 pos(pPlayer->GetPosY(), pPlayer->GetPickBoxHeight() + pPlayer->GetHeight(), pPlayer->GetPosX());
	D3DXMATRIX matFinal;
	GetGame()->GetRegion()->CalculateFinalTransformMatrix(matFinal);
	D3DXVECTOR3 pos2;
	D3DXVec3TransformCoord(&pos2,&pos,&matFinal);

	if (pPlayer->GetTitleType() != ePTT_Hide)
		pos2.y = fPos - 12;
	else
		pos2.y = fPos + 12;

	long nW = lFontWidth * lstrlen(strText);
	long nH = lFontWidth + 10;
	//if( pPlayer->IsTeamHasPassword() )
	//	nW += 12 + 4;

	int x = (int)pos2.x - 30;// - nW / 2;
	int y = (int)pos2.y - 35 - lFontWidth;

	if(pPlayer->GetFactionID()!=NULL_GUID)
		y += 3;

	CGlobeSetup::tagSetup* pSetup = CGlobeSetup::GetSetup();

	// ����߿�
	if(pPlayerRecruitBitmap==NULL)
	{
		pPlayerRecruitBitmap=CBitmapX::GetBitmapX("interface/team/recruit.tga");
	}
	if(pPlayer->GetFactionID()!=NULL_GUID)
		y-=lFontWidth+4;
	//-----------------��07.8.20liuke�޸ģ�-------------------
	if(pPlayerRecruitBitmap)
	{
		pPlayerRecruitBitmap->Draw((int)x - 60,(int)y - 20);
	}
	else
	{
		GetGame()->DrawSolidQuadRect((int)x, (int)y, (int)(x + nW), (int)(y + nH), pSetup->colorShopBar);
		GetGame()->DrawWireQuadRect((int)x,(int) y,(int)(x+ nW),(int) (y+nH), pSetup->colorShopWireBar);
	}
	//----------------------------------------------------------
	// ����
	GetGame()->OutPutText((int)x - 9, (int)y + 3, strText, (int)lLen, 0xff000000,1);
	GetGame()->OutPutText((int)x - 9, (int)y + 5, strText, (int)lLen, 0xff000000,1);
	GetGame()->OutPutText((int)x - 11,(int) y + 3, strText, (int)lLen, 0xff000000,1);
	GetGame()->OutPutText((int)x - 11, (int)y + 5, strText, (int)lLen, 0xff000000,1);
	GetGame()->OutPutText((int)x - 10, (int)y + 4, strText, (int)lLen, 0xffffffff,1);
	// ��������
	sprintf_s(strText,"(%d/6)",pPlayer->GetTeammateAmount());
	lLen = lstrlen(strText);
	GetGame()->OutPutText((int)x - 1, (int)y - 16, strText, (int)lLen, 0xff000000,1);
	GetGame()->OutPutText((int)x - 1, (int)y - 18, strText, (int)lLen, 0xff000000,1);
	GetGame()->OutPutText((int)x + 1, (int)y - 16, strText, (int)lLen, 0xff000000,1);
	GetGame()->OutPutText((int)x + 1, (int)y - 18, strText, (int)lLen, 0xff000000,1);
	GetGame()->OutPutText((int)x, (int)y - 17, strText, (int)lLen, 0xfffff600,1);

	// ��������
	if(pPlayer->IsTeamHasPassword())
	{
		if(pPasswordBitmap==NULL)
			pPasswordBitmap=CBitmapX::GetBitmapX("interface/team/recruit-code.tga");

		//-----------------��07.8.20liuke�޸ģ�-------------------
		if (pPasswordBitmap)
			pPasswordBitmap->Draw((int)(x+lLen*lFontWidth+2), (int)(y-18));
		//----------------------------------------------------------
	}
}



//==============07.10.16liuke���NPCͷ����ʾ������ʾ��Ϣ����==================
void CClientRegion::DisplayQuestInfoByNpc(CMonster *pMonster, long lEffectID)
{
	CPlayer *pPlayer = GetGame()->GetMainPlayer();
	if(!pPlayer || !pMonster || lEffectID == -1)
		return;
	// ��ǰNPC���Ǳ�����Ҳ����������
	if(pMonster->GetCountry() != pPlayer->GetCountry() && pMonster->GetCountry() != 7)
		return;
	// �����ǰ��������û�д���Ч�����أ���ѭ������
	pMonster->SetCurQuestEffect(lEffectID);
}
//============================================================================



/*
* ����: ��ʾ�̵���Ϣ
* ժҪ: -
* ����: pPlayer	- ���ָ��
*		fPos	- ��ʾ��Ϣ�߶�
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.10.07 - lpf
*		 ��Ϣ��ʾ�ĸ߶����ⲿ������
*/
void CClientRegion::ShowShopName(CPlayer* pPlayer, float fPos)
{
	string strShopName = pPlayer->GetPlayerShopName();
	if (strShopName.size() <= 0)
		return;

	if(pPlayerShopBitmap==NULL)
	{
		pPlayerShopBitmap=CBitmapX::GetBitmapX("INTERFACE/shop/shop-sign.tga");
	}

	// ������
	D3DXVECTOR3 pos(pPlayer->GetPosY(),2.5f+pPlayer->GetHeight(),pPlayer->GetPosX());
	D3DXMATRIX matFinal;
	GetGame()->GetRegion()->CalculateFinalTransformMatrix(matFinal);
	D3DXVECTOR3 pos2;
	D3DXVec3TransformCoord(&pos2,&pos,&matFinal);

	if (pPlayer->GetTitleType() != ePTT_Hide)
		pos2.y = fPos - 24;
	else
		pos2.y = fPos - 12;

	int nW = GetGame()->GetFontWidth(strShopName.c_str()) /*+ 10*/;
	int nH = GetGame()->GetFontHeight(-1) + 10;

	long x = (long)pos2.x - pPlayerShopBitmap->GetWidth()/2 /*-nW / 2*/;
	long y = (long)pos2.y - 25 -GetGame()->GetFontHeight(-1);

	if(pPlayer->GetFactionID()!=NULL_GUID)
		y -= GetGame()->GetFontHeight(-1);
	CGlobeSetup::tagSetup* pSetup = CGlobeSetup::GetSetup();

	if(pPlayerShopBitmap)
	{
		//��һ������ͼ��ƴ����ʾ������
		//RECT r;
		//SetRect(&r,0,0,11,23);
		//pPlayerShopBitmap->Draw(x,y,r);
		//SetRect(&r,13,0,11,23);
		//pPlayerShopBitmap->Draw(x+nW-11,y,r);
		//SetRect(&r,11,0,2,23);
		//for(int i=0;i<(nW-22)/2;i++)
		//{
		//	pPlayerShopBitmap->Draw(x+11+i*2,y,r);
		//}
		pPlayerShopBitmap->Draw(x-20,y);
	}
	else
	{
		GetGame()->DrawSolidQuadRect(x, y, x + nW, y + nH, pSetup->colorShopBar);
		GetGame()->DrawWireQuadRect(x, y, x+ nW, y+nH, pSetup->colorShopWireBar);
	}

	size_t len = strShopName.size();

	GetGame()->OutPutText(x+(pPlayerShopBitmap->GetWidth()-nW)/2, y + 30, strShopName.c_str(), len, pSetup->colorShopName);
}



/*
* ����: ��ʾ����ĳƺ��Լ�ͼ��
* ժҪ: ��ʾMonster�ƺ�
* ����: pMonster	  - ��Ҫ��ʾ�Ĺ���ָ��
*		bDisplayTitle - �Ƿ���ʾ����ƺ�
*		fDrawHeight	  - ��ʾ��Ϣ�ĸ߶�
* ����ֵ: -
* ����: ����
* ��������: 2007.12.6
* �޸���־:
*	2008.10.07 - lpf
*		 ����ƺ�(����NPC)�ƺ���ʾ�ĸ߶����ⲿ������
*/
void CClientRegion::DisplayMonsterTitle(CMonster* pMonster, bool bDisplayTitle, float fDrawHeight)
{
	if (pMonster==NULL)
		return;
	long ID = pMonster->GetMonsterIconID();
	string strTitle = pMonster->GetMonsterTitle();
	// û�г�νͼƬ�����������ͷ���
	if(ID == 0 && strTitle.empty())
		return;

	long index = pMonster->GetTitleColor();
	char strGMModeUse[256];
	CGlobeSetup::tagSetup* pSetup = CGlobeSetup::GetSetup();

	// ��ʾ�ƺż���ͼ��
	// GM��ʾ
	if( GetGame() -> IsGM() && strTitle.length() < 200 )
	{
		sprintf( strGMModeUse, "%s[%d]", strTitle.c_str(), pMonster->GetExID() );
		pMonster->DisplayMasterTitle(strGMModeUse, ID, pSetup->colorMonsterTitelName[index], pSetup->colorMonsterTitelNameBack[index], fDrawHeight,0,bDisplayTitle);
	}
	else
		pMonster->DisplayMasterTitle(strTitle.c_str(), ID, pSetup->colorMonsterTitelName[index], pSetup->colorMonsterTitelNameBack[index], fDrawHeight,0,bDisplayTitle);
}


/*
* ����: ���������Ϣ�����ʾ����
* ժҪ: ��������ҵ�������Ϣ�������أ����������������Ҫ��ʾ
* ����: -
* ����ֵ: -
* ����: lk
* ��������: 09.7.1
*/
float CClientRegion::DisplayHideInfo(CPlayer* pPlayer)
{
	if (pPlayer==NULL)
		return 0.0f;
	char strGMModeUse[256];

	CGlobeSetup::tagSetup* pSetup = CGlobeSetup::GetSetup();
	// ͷ������
	D3DXMATRIX matFinal;
	D3DXVECTOR3 pos(pPlayer->GetPosY(), pPlayer->GetPickBoxHeight() + pPlayer->GetHeight(), pPlayer->GetPosX());
	D3DXVECTOR3 pos2;

	GetGame()->GetRegion()->CalculateFinalTransformMatrix(matFinal);
	D3DXVec3TransformCoord(&pos2, &pos, &matFinal);
	float fHeight = pos2.y;
	float fDrawInfoHeight = fHeight;

	fDrawInfoHeight = fHeight = (pos2.y + pSetup->fPlayerInfoHeight);

	// �Ƿ����
	if ((dynamic_cast<CMoveShape *>(pPlayer))->IsRide())
	{
		float fPickHeight;
		CDisplayModel * pDisplayModel = (dynamic_cast<CPlayer *>(pPlayer))->GetHorse()->GetDisplayModel();
		if (pDisplayModel != NULL)
		{
			render::BoundingBox * m_pPickBox = pDisplayModel->GetPickBox();
			if (m_pPickBox != NULL)
			{
				D3DXVECTOR3 vMax;
				D3DXVECTOR3 vMin;
				m_pPickBox->Get(vMin, vMax);
				fPickHeight = vMax.y - vMin.y;
			}else
				fPickHeight = 3.0f;
		}
		else
			fPickHeight = 3.0f;

		pos.y = fPickHeight + pPlayer->GetHeight();
		D3DXVec3TransformCoord(&pos2, &pos, &matFinal);
		fDrawInfoHeight = fHeight = pos2.y;
	}

	// GMģʽ,��ʾ���ID
	if( GetGame() -> IsGM() && strlen( pPlayer->GetName() ) < 200)
	{
		sprintf( strGMModeUse, "%s[%d]", pPlayer->GetName(), pPlayer->GetExID() );
		pPlayer->DisplayText(strGMModeUse, pSetup->colorBlueName, pSetup->colorBlueNameBack, fHeight);
	}
	else // ��ͨģʽ
	{
		// ��ʾѪ��
		if (pPlayer != GetGame()->GetMainPlayer() &&
			(GetGame()->GetGameControl()->GetMouseShape() == pPlayer ||
			GetGame()->GetGameControl()->GetSelectedShape() == pPlayer))
		{
			pPlayer->DisplayHpBar(GetGame()->GetPlayerHpBarBitmap(), 100, pPlayer->GetHp(), pPlayer->GetMaxHp(), fHeight);
			fDrawInfoHeight -= (GetGame()->GetFontHeight(-1) + 5.0f);
		}
		else if (pPlayer == GetGame()->GetMainPlayer() &&
			GetGame()->GetGameControl()->GetSelectedShape() == pPlayer)
		{
			pPlayer->DisplayHpBar(GetGame()->GetPlayerHpBarBitmap(), 100, pPlayer->GetHp(), pPlayer->GetMaxHp(), fHeight);
			fDrawInfoHeight -= (GetGame()->GetFontHeight(-1) + 5.0f);
		}

		// ���壺���� ���޸ĵĲ߻��� ��ʾ����
		D3DCOLOR	clrName, clrBKName;
		CCountryList::tagCountryList*	countrySelf		= CCountryList::GetCountry( GetGame()->GetMainPlayer()->GetCountry() );
		CCountryList::tagCountryList*	countryAnother	= CCountryList::GetCountry( pPlayer->GetCountry() );

		if( countrySelf && countryAnother )
		{
			// ͬһ��Ӫ
			if( countrySelf->nLeague == countryAnother->nLeague )
			{
				// �������޷����¼�� ����Ӫ
				if( pPlayer->GetPkValue() == 0 )
				{
					clrName		= pSetup->colorAllianceName;
					clrBKName	= pSetup->colorAllianceNameBack;
				}
				// ����
				else if( pPlayer->GetPkValue() <= 20 )
				{
					clrName		= pSetup->colorPurpleName;
					clrBKName	= pSetup->colorPurpleNameBack;
				}
				// ����
				else
				{
					clrName		= pSetup->colorRedName;
					clrBKName	= pSetup->colorRedNameBack;
				}
			}
			// �Ǳ���Ӫ
			else
			{
				// ����ֵ���� 0
				if( pPlayer->GetPVPValue() )
				{
					// ����
					clrName		= pSetup->colorOrangeName;
					clrBKName	= pSetup->colorOrangeNameBack;
				}
				// ����ֵ���� 0
				else
				{
					// �������޷����¼�� �Ǳ���Ӫ
					if( pPlayer->GetPkValue() == 0 )
					{
						clrName		= pSetup->colorEnemyName;
						clrBKName	= pSetup->colorEnemyNameBack;
					}
					// ����
					else if( pPlayer->GetPkValue() <= 20 )
					{
						clrName		= pSetup->colorPurpleName;
						clrBKName	= pSetup->colorPurpleNameBack;
					}
					// ����
					else
					{
						clrName		= pSetup->colorRedName;
						clrBKName	= pSetup->colorRedNameBack;
					}
				}

				// ��Ե�ͼ�����������ɫ�仯
				CRegion::tagCell * pCell = GetCell(pPlayer->GetTileX(), pPlayer->GetTileY());
				if (pCell)
				{
					switch (pCell->bSecurity)
					{
					case CRegion::SECURTIY_FIGHT:	//������
						{
							clrName   = pSetup->colorEnemyNameInBattleZone;
							clrBKName = pSetup->colorEnemyNameInBattleZoneBack;
						}
						break;
					case CRegion::SECURITY_FREE:	//�ǰ�ȫ��������ʾս������
						{
							// �Ե�ͼս�����Ե��ж�
							if (m_WarRegionType == RWT_NormalWar)		//��ͨս��
							{
								clrName	  = pSetup->colorEnemyNameInWarZone;
								clrBKName = pSetup->colorEnemyNameInWarZoneBack;
							}
						}
						break;
					}
				}
			}

			// �޷����¼�Ķ��� �� �Լ� ��ɫ
			if( pPlayer->GetPkValue() == 0 && pPlayer->GetPVPValue() == 0 )
			{
				if( GetGame()->GetMainPlayer()->GetExID() == pPlayer->GetExID()
					|| GetGame()->GetMainPlayer()->IsTeammate( pPlayer->GetExID() ) )
				{
					clrName		= pSetup->colorTeammateName;
					clrBKName	= pSetup->colorTeammateNameBack;
				}
			}

			// ���� ������ɫ ��������
			pPlayer->DisplayHideInfo( pPlayer->GetName(), clrName, clrBKName, fDrawInfoHeight );
		}
	}
	return fDrawInfoHeight;
}

/*
* ����: ��ʾShape����
* ժҪ: -
* ����: pShape			  - ��Ҫ��ʾ��Shapeָ��
*		 bDisplayName	  - �Ƿ���ʾ����
*		 bDisplayBloodBar - �Ƿ���ʾѪ��
* ����ֵ: ���صĵ�ǰ������Ϣ�߶�
* ����: -
* ��������: -
* �޸���־:
*	2008.02.18 - lpf
*		���������ͷ��������Ѫ�����ڵ�����(������������Ӿ�ʱ)�������˶�Shape"ͷ������"�ļ���
*	2008.06.19 - lpf
*		��Shapeͷ����Ϣ�����������������10������
*	2008.06.20 - lpf
*		���ͷ����Ϣ�����������Ƿ����Ӱ��(ע����������PickBox�߶�)
*	2008.08.04 - lpf
*		�޸��˲ɼ���ͳ��д��Ŷ�����Ϣ���Ƹ߶��Ǹ��ݵ�һ�μ�����ĸ߶�(ԭ������ʱ������Ƕ�ֵ1.5f)
*	2008.08.06 - lpf
*		�޸�����Ʒ������Ϣ���Ƹ߶��Ǹ��ݵ�һ�μ�����ĸ߶�(ԭ������ʱ������Ƕ�ֵ0.5f)
*	2008.09.17 - lpf
*		��������ͨ�������ֲ���ʾ����
*	2008.09.30 - lpf
*		 �޸����µ���Һ͹���ͷ��Ѫ����ʾ����
*	2008.10.07 - lpf
*		��ǰ������Ϣ�߶Ȼ��ں�������ʱ���ݳ�ȥ,Ŀǰ������ҹ�����Ч;
*	2008.10.09 - lpf
*		��ǰ������Ϣ�߶��޸�Ϊ����ֵ��ʽ���ݳ�ȥ;
*	2008.11.04 - lpf
*		Ϊ��Һ͹���ͷ����Ϣ���Ƹ߶�������һ�������õ�����;
*	2008.11.05 - lpf
*		���ݲ߻�����,��ʱ���βɼ���ͷ��������ʾ;
*	2008.12.19 - lpf
*		�����˶�NPC�����ַ����Ľ�����ɺ�����ʾ;
*	2009.02.04 - lpf
*		�����˶���Ʒ������ʾʱ,�����ַ����ж���������ʾ;
*	2009.03.01 - lpf
*		��"�Ǳ���Ӫ"��������ʾ������,����"��Ե�ͼ�����������ɫ�仯";
*	2009.10.26 - lpf
*		ע����ԭ�ж�Shape��������Ϊ�ɼ�����ֱ�ӷ��صĴ���;
*		ע������ʾ�ɼ���ĵȼ�����;
*	2009.11.13 - lpf
*		�����˾ֲ�����bool bDisplayedBloodBar,���Լ�¼�Ƿ���ʾ��Ѫ��,��״̬�ᱻ���뵽�����������ʾ��ȥ;
*/
float CClientRegion::DisplayShapeName(CShape* pShape, bool bDisplayName, bool bDisplayBloodBar)
{
	if (pShape==NULL)
		return 0.0f;

	//if (pShape->GetType() == TYPE_COLLECTION)
	//	return 0.0f;

	bool bDisplayedBloodBar = false;

	CPlayer * pMainPlayer = GetGame()->GetMainPlayer();
	// �����˸�����Ϣ��������ʾ
	//CMainBarPageEx *pMainBar = GetGame()->GetCGuiEx()->GetMainBarPageEx();
	//if(pMainBar && !pMainBar->IsGM(pMainPlayer->GetName()) && pShape->GetType() == TYPE_PLAYER)
	//{
	//	CPlayer * pPlayer = dynamic_cast<CPlayer *>(pShape);
	//	if(pPlayer && pPlayer->GetIsHideInfo() && pMainPlayer != pPlayer && !pMainPlayer->IsTeammate(pPlayer->GetExID()))
	//		return DisplayHideInfo(pPlayer);
	//}

	/**## Used by GM mode, Added by Jar @ 2004-4-6 */
	char strGMModeUse[256];

	CGlobeSetup::tagSetup* pSetup = CGlobeSetup::GetSetup();

	// ͷ������
	D3DXMATRIX matFinal;
	D3DXVECTOR3 pos(pShape->GetPosY(), pShape->GetPickBoxHeight() + pShape->GetHeight(), pShape->GetPosX());
	D3DXVECTOR3 pos2;

	GetGame()->GetRegion()->CalculateFinalTransformMatrix(matFinal);
	D3DXVec3TransformCoord(&pos2, &pos, &matFinal);
	float fHeight = pos2.y;
	float fDrawInfoHeight = fHeight;

	switch(pShape->GetType())
	{
	case TYPE_PLAYER:
		{
			fDrawInfoHeight = fHeight = (pos2.y + pSetup->fPlayerInfoHeight);

			// �Ƿ����
			if ((dynamic_cast<CMoveShape *>(pShape))->IsRide())
			{
				float fPickHeight;
				CDisplayModel * pDisplayModel = (dynamic_cast<CPlayer *>(pShape))->GetHorse()->GetDisplayModel();
				if (pDisplayModel != NULL)
				{
					render::BoundingBox * m_pPickBox = pDisplayModel->GetPickBox();
					if (m_pPickBox != NULL)
					{
						D3DXVECTOR3 vMax;
						D3DXVECTOR3 vMin;
						m_pPickBox->Get(vMin, vMax);
						fPickHeight = vMax.y - vMin.y;
					}else
						fPickHeight = 3.0f;
				}else
					fPickHeight = 3.0f;

				pos.y = fPickHeight + pShape->GetHeight();
				D3DXVec3TransformCoord(&pos2, &pos, &matFinal);
				fDrawInfoHeight = fHeight = pos2.y;
			}

			// GMģʽ,��ʾ���ID
			if( GetGame() -> IsGM() && strlen( pShape->GetName() ) < 200 && bDisplayName)
			{
				sprintf( strGMModeUse, "%s[%d]", pShape->GetName(), pShape->GetExID() );
				pShape->DisplayText(strGMModeUse, pSetup->colorBlueName, pSetup->colorBlueNameBack, fHeight);
			}
			else // ��ͨģʽ
			{
				CPlayer* pPlayer = (CPlayer*)pShape;
				string strPlayerName = pPlayer->GetName();
				// �Ƕ���
				if(pMainPlayer != pPlayer && pMainPlayer->IsTeammate(pPlayer->GetExID()))
				{
					tagWSMemberInfo * stInfo = pMainPlayer->GetWsMemberInfo(pPlayer->GetExID());
					if(stInfo)
						strPlayerName = stInfo->szName;
				}

				// ��ʾѪ��
				if (bDisplayBloodBar &&
					pPlayer != pMainPlayer &&
					(GetGame()->GetGameControl()->GetMouseShape() == pShape ||
					GetGame()->GetGameControl()->GetSelectedShape() == pShape))

				{
					//long lHpBarLong = 100;
					pShape->DisplayHpBar(GetGame()->GetPlayerHpBarBitmap(), 100, pPlayer->GetHp(), pPlayer->GetMaxHp(), fHeight);
					bDisplayedBloodBar = true;
					fDrawInfoHeight -= (GetGame()->GetFontHeight(-1) + 5.0f);
				}
                else if (bDisplayBloodBar &&
					pPlayer == GetGame()->GetMainPlayer() &&
					GetGame()->GetGameControl()->GetSelectedShape() == pShape)
				{
					pShape->DisplayHpBar(GetGame()->GetPlayerHpBarBitmap(), 100, pPlayer->GetHp(), pPlayer->GetMaxHp(), fHeight);
					bDisplayedBloodBar = true;
					fDrawInfoHeight -= (GetGame()->GetFontHeight(-1) + 5.0f);
				}

				// ���壺���� ���޸ĵĲ߻��� ��ʾ����
				if( bDisplayName )
				{
					D3DCOLOR	clrName, clrBKName;

					CCountryList::tagCountryList*	countrySelf		= CCountryList::GetCountry( GetGame()->GetMainPlayer()->GetCountry() );
					CCountryList::tagCountryList*	countryAnother	= CCountryList::GetCountry( pPlayer->GetCountry() );

					if( countrySelf && countryAnother )
					{
						// ͬһ��Ӫ
						if( countrySelf->nLeague == countryAnother->nLeague )
						{
							// �������޷����¼�� ����Ӫ(09.9.29������Ӿݵ�ʶ��)
							if( pPlayer->GetPkValue() == 0 || IsOpenSPoint() )
							{
								clrName		= pSetup->colorAllianceName;
								clrBKName	= pSetup->colorAllianceNameBack;
							}
							// ����
							else if( pPlayer->GetPkValue() <= 20 )
							{
								clrName		= pSetup->colorPurpleName;
								clrBKName	= pSetup->colorPurpleNameBack;
							}
							// ����
							else
							{
								clrName		= pSetup->colorRedName;
								clrBKName	= pSetup->colorRedNameBack;
							}
						}
						// �Ǳ���Ӫ
						else
						{
							// (09.9.29������Ӿݵ�ʶ��)
							if ( IsOpenSPoint() )	// ����Ǿݵ�ս������������ʾΪ��ɫ
							{
								clrName		= pSetup->colorEnemyName;
								clrBKName	= pSetup->colorEnemyNameBack;
							}
							// ����ֵ���� 0
							else if( pPlayer->GetPVPValue() )
							{
								// ����
								clrName		= pSetup->colorOrangeName;
								clrBKName	= pSetup->colorOrangeNameBack;
							}
							// ����ֵ���� 0
							else
							{
								// �������޷����¼�� �Ǳ���Ӫ
								if( pPlayer->GetPkValue() == 0 )
								{
									clrName		= pSetup->colorEnemyName;
									clrBKName	= pSetup->colorEnemyNameBack;
								}
								// ����
								else if( pPlayer->GetPkValue() <= 20 )
								{
									clrName		= pSetup->colorPurpleName;
									clrBKName	= pSetup->colorPurpleNameBack;
								}
								// ����
								else
								{
									clrName		= pSetup->colorRedName;
									clrBKName	= pSetup->colorRedNameBack;
								}
							}

							// ��Ե�ͼ�����������ɫ�仯
							CRegion::tagCell * pCell = GetCell(pPlayer->GetTileX(), pPlayer->GetTileY());
							if (pCell && !IsOpenSPoint())
							{
								switch (pCell->bSecurity)
								{
								case CRegion::SECURTIY_FIGHT:	//������
									{
										clrName   = pSetup->colorEnemyNameInBattleZone;
										clrBKName = pSetup->colorEnemyNameInBattleZoneBack;
									}
									break;
								case CRegion::SECURITY_FREE:	//�ǰ�ȫ��������ʾս������
									{
										// �Ե�ͼս�����Ե��ж�
										if (m_WarRegionType == RWT_NormalWar)		//��ͨս��
										{
											clrName	  = pSetup->colorEnemyNameInWarZone;
											clrBKName = pSetup->colorEnemyNameInWarZoneBack;
										}
									}
									break;
								}
							}
						}

						// �޷����¼�Ķ��� �� �Լ� ��ɫ
						if( pPlayer->GetPkValue() == 0 && pPlayer->GetPVPValue() == 0 )
						{
							if( GetGame()->GetMainPlayer()->GetExID() == pPlayer->GetExID()
								|| GetGame()->GetMainPlayer()->IsTeammate( pPlayer->GetExID() ) )
							{
								clrName		= pSetup->colorTeammateName;
								clrBKName	= pSetup->colorTeammateNameBack;
							}
						}

						// ���� ������ɫ ��������
						pShape->DisplayText(strPlayerName.c_str(), clrName, clrBKName, fDrawInfoHeight, 0, bDisplayedBloodBar);
					}
				}
			}
		}
		break;
	case TYPE_PET:
		if (bDisplayName)
		{
			D3DCOLOR	clrName = 0xffff0000, clrBKName=0xffff0000;
			CPet *pPet = (CPet*)pShape;
			//if (pPet->GetPetType() == PET_TYPE_TRAP || pPet->GetPetType() == PET_TYPE_ORNA)
			//	break;
			CPlayer *pMaster = pPet->GetMaster();
			if(pMaster == NULL)	break;
			CCountryList::tagCountryList*	countrySelf		= CCountryList::GetCountry( GetGame()->GetMainPlayer()->GetCountry() );
			CCountryList::tagCountryList*	countryAnother	= CCountryList::GetCountry( pMaster->GetCountry() );

			if( countrySelf && countryAnother )
			{
				// ͬһ��Ӫ
				if( countrySelf->nLeague == countryAnother->nLeague )
				{
					// �������޷����¼�� ����Ӫ
					if( pMaster->GetPkValue() == 0 )
					{
						clrName		= pSetup->colorAllianceName;
						clrBKName	= pSetup->colorAllianceNameBack;
					}
					// ����
					else if( pMaster->GetPkValue() <= 20 )
					{
						clrName		= pSetup->colorPurpleName;
						clrBKName	= pSetup->colorPurpleNameBack;
					}
					// ����
					else
					{
						clrName		= pSetup->colorRedName;
						clrBKName	= pSetup->colorRedNameBack;
					}
				}
				// �Ǳ���Ӫ
				else
				{
					// ����ֵ���� 0
					if( pMaster->GetPVPValue() )
					{
						// ����
						clrName		= pSetup->colorOrangeName;
						clrBKName	= pSetup->colorOrangeNameBack;
					}
					// ����ֵ���� 0
					else
					{
						// �������޷����¼�� �Ǳ���Ӫ
						if( pMaster->GetPkValue() == 0 )
						{
							clrName		= pSetup->colorEnemyName;
							clrBKName	= pSetup->colorEnemyNameBack;
						}
						// ����
						else if( pMaster->GetPkValue() <= 20 )
						{
							clrName		= pSetup->colorPurpleName;
							clrBKName	= pSetup->colorPurpleNameBack;
						}
						// ����
						else
						{
							clrName		= pSetup->colorRedName;
							clrBKName	= pSetup->colorRedNameBack;
						}
					}

					// ��Ե�ͼ�����������ɫ�仯
					CRegion::tagCell * pCell = GetCell(pMaster->GetTileX(), pMaster->GetTileY());
					if (pCell)
					{
						switch (pCell->bSecurity)
						{
						case CRegion::SECURTIY_FIGHT:	//������
							{
								clrName   = pSetup->colorEnemyNameInBattleZone;
								clrBKName = pSetup->colorEnemyNameInBattleZoneBack;
							}
							break;
						case CRegion::SECURITY_FREE:	//�ǰ�ȫ��������ʾս������
							{
								// �Ե�ͼս�����Ե��ж�
								if (m_WarRegionType == RWT_NormalWar)		//��ͨս��
								{
									clrName	  = pSetup->colorEnemyNameInWarZone;
									clrBKName = pSetup->colorEnemyNameInWarZoneBack;
								}
							}
							break;
						}
					}
				}

				// �޷����¼�Ķ��� �� �Լ� ��ɫ
				if( pMaster->GetPkValue() == 0 && pMaster->GetPVPValue() == 0 )
				{
					if( GetGame()->GetMainPlayer()->GetExID() == pMaster->GetExID()
						|| GetGame()->GetMainPlayer()->IsTeammate( pMaster->GetExID() ) )
					{
						clrName		= pSetup->colorTeammateName;
						clrBKName	= pSetup->colorTeammateNameBack;
					}
				}

				// ���� ������ɫ ��������
				pShape->DisplayText( pPet->GetName(), clrName, clrBKName, fDrawInfoHeight );
			}
		}
		break;
	case TYPE_MONSTER:
		{
			fDrawInfoHeight = fHeight = (pos2.y + pSetup->fMonsterInfoHeight);

			CMonster* pMonster = (CMonster*)pShape;
			if (pShape->GetState()!=CShape::STATE_DIED)
			{
				// һ���NPC
				if(pMonster->GeteNpcType() == NT_Normal)
				{
					if (bDisplayName == false)
					{
						if (GetGame()->GetGameControl()->GetSelectedShape() != pShape)
						{
							//if (!GetGame()->GetCGuiEx()->GetGameConfigPageEx()->GetOption(CGameConfigPageEx::OPTION_ShowNPCName))
							//	break;
						}
					}

					if( GetGame() -> IsGM() && strlen( pShape->GetName() ) < 200 )
					{
						sprintf( strGMModeUse, "%s[%d]", pShape->GetName(), pShape->GetExID() );
						pShape->DisplayText(strGMModeUse, pSetup->colorNpcName, pSetup->colorNpcNameBack, fHeight);
					}else
						pShape->DisplayText(ResolveNameStr(pShape->GetName()), pSetup->colorNpcName, pSetup->colorNpcNameBack, fHeight);

					break;
				}

				// ��ʾѪ��
				//if (GetGame()->GetCGuiEx()->GetGameConfigPageEx()->GetOption(CGameConfigPageEx::OPTION_ShowMonsterHP) ||
				//	(bDisplayBloodBar &&
				//	(GetGame()->GetGameControl()->GetMouseShape() == pShape ||
				//	GetGame()->GetGameControl()->GetSelectedShape() == pShape)))
				//{
				//	// Ѫ������ͳһ100
				//	long lHpBarLong = 100;

				//	// ����
				//	if (pMonster->GetHpBarColor() == 1)	// ��
				//	{
				//		pShape->DisplayHpBar(GetGame()->GetMonsterHpBarBitmap(), lHpBarLong, pMonster->GetHp(), pMonster->GetMaxHp(), fDrawInfoHeight);
				//		fDrawInfoHeight -= (GetGame()->GetFontHeight(-1) + 5.0f);
				//	}else if (pMonster->GetHpBarColor() == 2)	// ��
				//	{
				//		pShape->DisplayHpBar(GetGame()->GetHpBarBitmap2(), lHpBarLong, pMonster->GetHp(), pMonster->GetMaxHp(), fDrawInfoHeight);
				//		fDrawInfoHeight -= (GetGame()->GetFontHeight(-1) + 5.0f);
				//	}
				//}

				// ��ʾ����
				if (bDisplayName == false)
					break;
				/***********************************************************************/
				/*  zhaohang 2010-11-16 btNameColor ��BYTE��ΪDWORD��*/
				/***********************************************************************/
				DWORD btNameColor = pMonster->GetNameColor();
				if (btNameColor>6)
					btNameColor = 0;
				DWORD dwColor = pSetup->colorBlueName;
				DWORD dwColorBack = pSetup->colorBlueNameBack;
				if( GetGame() -> IsGM() && strlen( pShape->GetName() ) < 200 )
				{
					sprintf( strGMModeUse, "%s[%d]", pShape->GetName(), pShape->GetExID() );
					pShape->DisplayText(strGMModeUse, pSetup->colorMonsterName[btNameColor], pSetup->colorMonsterNameBack[btNameColor], fDrawInfoHeight);
				}				
				else
				{
					CMonster *pMonster=(CMonster*)pShape;
					//CPlayer * pMainPlayer = GetGame()->GetMainPlayer();

					// �������񱣻�����ʾ��ʽ
					if(pMainPlayer && pMonster && pMonster->GetCreatorType() != 0 && pMonster->GetCreatorID() != CGUID::GUID_INVALID)
					{
						// ������Ϊ��һ����
						if((pMonster->GetCreatorType() == TYPE_PLAYER && pMonster->GetCreatorID() == pMainPlayer->GetExID()) ||
							(pMonster->GetCreatorType() == TYPE_TEAM && pMonster->GetCreatorID() == pMainPlayer->GetTeamID()))
						{
							pShape->DisplayMasterName(pMonster->GetName(), pSetup->colorQuestMstByCreator, pSetup->colorMonsterNameBack[btNameColor], fDrawInfoHeight);
							break;
						}
						// �Ǵ�����
						else
						{
							pShape->DisplayMasterName(pMonster->GetName(), pSetup->colorQuestMstByOther, pSetup->colorMonsterNameBack[btNameColor], fDrawInfoHeight);
							break;
						}
					}else		//����������ʾ
						pShape->DisplayText(pMonster->GetName(), pSetup->colorMonsterName[btNameColor], pSetup->colorMonsterNameBack[btNameColor], fDrawInfoHeight);
				}
			}
			else
			{
				if (bDisplayName==false)
					break;

				if( GetGame() -> IsGM() && strlen( pShape->GetName() ) < 200 )
				{
					sprintf( strGMModeUse, "%s[%d]", pShape->GetName(), pShape->GetExID() );
					pShape->DisplayText(strGMModeUse, pSetup->colorDiedMonsterName, pSetup->colorDiedMonsterNameBack, fDrawInfoHeight);
				}
				else 
					pShape->DisplayText(pShape->GetName(), pSetup->colorDiedMonsterName, pSetup->colorDiedMonsterNameBack, fDrawInfoHeight);
			}
		}
		break;

	case TYPE_GOODS:
		if (bDisplayName==false)
			break;

		if( GetGame() -> IsGM() && strlen( pShape->GetName() ) < 200 )
		{
			sprintf( strGMModeUse, "%s", pShape->GetName() );
			pShape->DisplayText(strGMModeUse, pSetup->colorGoodsName, pSetup->colorGoodsNameBack, fHeight);
		}
		else 
		{
			CGoods *pGoods=(CGoods*)pShape;
			//GetGame()->GetInterface()->GetGoodsDescPage()->ShowPage(pGoods,0,0);
			DWORD dwLevel=pGoods->GetEffectVal(GAP_WEAPON_LEVEL,1);
			DWORD dwColor = pGoods->GetEffectVal(GAP_COLOR,1);
			static char strName[256] = {};

			if (dwLevel > 0)
			{
				if (pGoods->GetNum() > 1)
					sprintf(strName, "+%u %s(%d)", dwLevel, pGoods->GetName(), pGoods->GetNum());
				else
					sprintf(strName, "+%u %s", dwLevel, pGoods->GetName());
				pShape->DisplayText(strName, (dwColor == 0) ? 0xffffffff : dwColor, pSetup->colorGoodsNameBack, fHeight);
			}else
			{
				if (pGoods->GetNum() > 1)
					sprintf(strName, "%s(%d)", pGoods->GetName(), pGoods->GetNum());
				else
					sprintf(strName, "%s", pGoods->GetName());
				pShape->DisplayText(strName, (dwColor == 0) ? 0xffffffff : dwColor, pSetup->colorGoodsNameBack, fHeight);
			}
		}
		break;
	case TYPE_COLLECTION:
		{
			CCollectionGoods* pCollectionGoods = (CCollectionGoods*)pShape;
			DWORD dwLevelDifference = pMainPlayer->GetCollectSkillLevel() - pCollectionGoods->GetCollectLevel();
			DWORD dwColor = 0;
			// �������񱣻�����ʾ��ʽ
			if(pMainPlayer && pCollectionGoods && pCollectionGoods->GetCreatorType() != 0 && pCollectionGoods->GetCreatorID() != CGUID::GUID_INVALID)
			{
				// ������Ϊ��һ����
				if( ( pCollectionGoods->GetCreatorType() == TYPE_PLAYER && pCollectionGoods->GetCreatorID() == pMainPlayer->GetExID() )||
					( pCollectionGoods->GetCreatorType() == TYPE_TEAM && pCollectionGoods->GetCreatorID() == pMainPlayer->GetTeamID() )
					)
				{
					dwColor = pSetup->colorQuestMstByCreator;
				}
				// �Ǵ�����
				else 
				{
					dwColor = pSetup->colorQuestMstByOther;
				}
			}
			else if (pCollectionGoods->GetOccuID()==DOccu_Other)
			{
				dwColor = 0xffffffff;
			}
			else if (pCollectionGoods->GetOccuID()!=pMainPlayer->GetAssOccu())
			{
				dwColor = CDeputyOccuSystemClient::GetCollectionColor(-1);
			}
			else
				dwColor = CDeputyOccuSystemClient::GetCollectionColor(dwLevelDifference);

			if (pShape->GetState()!=CShape::STATE_DIED )
			{
				if (bDisplayName==false)
					break;
				bool bShowResults = true;
				// ��Ʒ�������
				//const list<tgConditionGoods>* pConditionList = pCollectionGoods->GetConditionList();
				//if (pConditionList!=NULL&&pConditionList->size()!=0)
				//{
				//	list<tgConditionGoods>::const_iterator conIter = pConditionList->begin();
				//	for (;conIter!=pConditionList->end();conIter++)
				//	{
				//		long lGoodNum = pMainPlayer->GetGoodsAmount(conIter->cGoodsName);
				//		if (conIter->iNumType==0&&lGoodNum<conIter->iNum||
				//			conIter->iNumType==1&&lGoodNum!=conIter->iNum||
				//			conIter->iNumType==2&&lGoodNum>conIter->iNum)
				//		{
				//			bShowResults = false;
				//			break;
				//		}
				//	}
				//}

				//////////// �����������///////////
				if (pCollectionGoods->GetOccuID()==DOccu_Other)
				{
					const vector<tagQuestGoods>* pQustList = pCollectionGoods->GetRoleInfo();
					if (pQustList!=NULL)
					{
						vector<tagQuestGoods>::const_iterator QustIter = pQustList->begin();
						for (;QustIter!=pQustList->end();QustIter++)
						{
							if (QustIter->nQuestId!=-1)
							{
								CQuestQueue* pQueueList = pMainPlayer->GetRoleQueue();
								if (pQueueList!=NULL)
								{
									if (pQueueList->GetRecordByID(QustIter->nQuestId)==NULL||
										pQueueList->GetQuestStepNum(QustIter->nQuestId)!=QustIter->dwRoleStep)
									{
										continue;
										//bShowResults = false;
										//PutDebugString(CStringReading::LoadText(eIDS_CONTROL,eIDS_CONTROL_NOQUEST));
									}
									else if (pQueueList->GetRecordByID(QustIter->nQuestId)!=NULL&&
										pQueueList->GetQuestStepNum(QustIter->nQuestId)==QustIter->dwRoleStep)
									{
										//PutDebugString(CStringReading::LoadText(eIDS_CONTROL,eIDS_CONTROL_NOQUESTSTEP));
										//bShowResults = false;
										break;
									}
								}
								else
								{
									//PutDebugString(CStringReading::LoadText(eIDS_CONTROL,eIDS_CONTROL_NOQUEST));
									//bShowResults = false;
									continue;
								}
							}
							else
								break;
						}
						if (QustIter==pQustList->end())
						{
							bShowResults = false;
						}
						else if (QustIter!=pQustList->end())
						{
							bShowResults = true;
						}
					}
					//CRoleQueue* pQueueList = pMainPlayer->GetRoleQueue();
					//if (pQueueList!=NULL)
					//{
					//	if (pQueueList->GetRecordByID(pCollectionGoods->GetRoleID())==NULL)
					//	{
					//		bShowResults = false;
					//	}else if (pQueueList->GetRoleStepNum(pCollectionGoods->GetRoleID())!=pCollectionGoods->GetRoleStep())
					//	{
					//		bShowResults = false;
					//	}
					//}else
					//	bShowResults = false;
				}/*else if (pCollectionGoods->GetOccuID()!=pMainPlayer->GetAssOccu())
				 {
				 bShowResults = false;
				 }*/

				if (bShowResults)
				{
					if (strlen( pShape->GetName() ) < 200 )
					{
						//if (pCollectionGoods->GetCollectLevel()>0)
						//{
						//	sprintf( strGMModeUse, "%s/Lv:%d", pCollectionGoods->GetName(),pCollectionGoods->GetCollectLevel());
						//}else
						sprintf( strGMModeUse, "%s", pCollectionGoods->GetName());
						pShape->DisplayText(strGMModeUse, dwColor, 0x00000000, fHeight - 30.0f);
					}
				}
			}
		}break;
	case TYPE_BUILD:
	case TYPE_CITYGATE:
		{
			CBuild* pBuild = (CBuild*)pShape;
			if ( pShape->GetState()!=CShape::STATE_DIED && pShape->GetAction() != CShape::ACT_OPEN
				&&pShape->GetAction()!=CShape::ACT_ALREADYOPEN)
			{
				if (bDisplayBloodBar)
				{
					long lHpBarLong = 128;
					pShape->DisplayHpBar(GetGame()->GetHpBarBitmap2(), lHpBarLong, pBuild->GetHp(), pBuild->GetMaxHp(), fHeight);
					fHeight -= (GetGame()->GetFontHeight(-1) + 5.0f);
				}

				if (bDisplayName==false)
					break;

				if( GetGame() -> IsGM() && strlen( pShape->GetName() ) < 200 )
				{
					sprintf( strGMModeUse, "%s[%d]", pShape->GetName(), pShape->GetExID() );
					pShape->DisplayText(strGMModeUse, pSetup->colorCityGateName, pSetup->colorCityGateNameBack, fHeight);
				}
				else 
					pShape->DisplayText(pShape->GetName(), pSetup->colorCityGateName, pSetup->colorCityGateNameBack, fHeight);

			}
		}
		break;
	}

	return fDrawInfoHeight;
}



void CClientRegion::DisplayShapeHoverArrow(CShape* pShape)
{
	//	BOOL bHover = (pShape == GetGame()->GetGameControl()->GetMouseShape()) && GetGame()->IsDisplayShapeName();

	//	if (bHover)
	//	{
	switch(pShape->GetType())
	{
	case TYPE_PLAYER:
		if (pShape != GetGame()->GetMainPlayer())
		{
			if (GetGame()->GetMainPlayer()->IsTeammate(pShape->GetExID()))
			{
				pShape->DisplayHoverArrow(2);
			}
			else
			{
				pShape->DisplayHoverArrow(1);
			}
		}
		break;
		//case TYPE_NPC:
	case TYPE_GOODS:
		break;
	case TYPE_MONSTER:
		{
			CMonster* pMonster = (CMonster*)pShape;
			// ��NPC����ʾ������ͷ������
			if(pMonster->GeteNpcType() == NT_Normal)
				return;
		}
	case TYPE_BUILD:
	case TYPE_CITYGATE:
		pShape->DisplayHoverArrow(1);
		break;
	}
	//	}
}



/*
* ����: ��ʾShape���Һͺ�����־
* ժҪ: -
* ����: pPlayer			- ��Ҫ��ʾ����Ҷ���ָ��
*		bDisplayCountry - �Ƿ���ʾ���ұ�־
*		bPirate			- �Ƿ���ʾ������־
* ����ֵ: -
* ����: lpf
* ��������: 2008.07.10
* �޸���־:
*	2008.09.07 - ����
*		��������ͼ����ʾ�������Ӷ�λ
*/
void CClientRegion::DisplayShapeCountryPirate(CPlayer * pPlayer, bool bDisplayCountry, bool bPirate)
{
	if (pPlayer == NULL)
		return;

	D3DXVECTOR3 pos(pPlayer->GetPosY(), pPlayer->GetPickBoxHeight() + pPlayer->GetHeight(), pPlayer->GetPosX());
	D3DXMATRIX matFinal;
	GetGame()->GetRegion()->CalculateFinalTransformMatrix(matFinal);
	D3DXVECTOR3 pos2;
	D3DXVec3TransformCoord(&pos2,&pos,&matFinal);

	long x			= (long)pos2.x;
	long y			= (long)pos2.y - 10;
	
	CCountryList::tagCountryList * pst = CCountryList::GetCountry(pPlayer->GetCountry());
	static char strCountry[256];

	if (bDisplayCountry)
	{
		if (pst)
		{
			GetGame()->GetPicList()->Display(CPicList::PT_COUNTRY_ICON, pst->lIconID, x - 25, y);
		}
        else
		{
			long len;
            sprintf(strCountry, AppFrame::GetText("Base_100"));  //����
			static D3DCOLOR colorCountry = 0xffffffff, colorCountryBack = 0xff000000;
			len = lstrlen(strCountry);
			x   = x - GetGame()->GetFontWidth(strCountry);
			GetGame()->OutPutText(x + 1, y + 1, strCountry, len, colorCountryBack);
			GetGame()->OutPutText(x, y, strCountry, len, colorCountry);
		}
	}

	if (bPirate)
	{
		DWORD dwPvp = pPlayer->GetPVPValue();
		if ( dwPvp > 0 )
		{
			CBitmapX * pbmPirate = NULL;
			if( dwPvp < CPlayer::PVP_PIRATE1 )
			{
				pbmPirate = CBitmapX::GetBitmapX("pictures/state/pirate1.tga");
				if (pbmPirate)
					pbmPirate->Draw(x + 3, y);
			}
			else if( dwPvp < CPlayer::PVP_PIRATE2 )
			{
				pbmPirate = CBitmapX::GetBitmapX("pictures/state/pirate2.tga");
				if (pbmPirate)
					pbmPirate->Draw(x + 3, y);
			}
			else if( dwPvp < CPlayer::PVP_PIRATE3 )
			{
				pbmPirate = CBitmapX::GetBitmapX("pictures/state/pirate3.tga");
				if (pbmPirate)
					pbmPirate->Draw(x + 3, y);
			}
			else
			{
				pbmPirate = CBitmapX::GetBitmapX("pictures/state/pirate4.tga");
				if (pbmPirate)
					pbmPirate->Draw(x + 3, y);
			}
		}
	}
}



void CClientRegion::Init(void)
{
	/////////////////////////////////////////////////
	m_pCamera = new render::Camera;

	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer3D   *pLayer3D   = pInterface->GetLayer3D();

	int iWidth = pInterface->GetWndWidth();
	int iHeight= pInterface->GetWndHeight();

	float aspect = (float)iWidth/(float)iHeight;
	m_pCamera->SetAspect(aspect);	

	if (GetGame()->GetSetup()->lViewRange < 25)
		GetGame()->GetSetup()->lViewRange = 25;

	m_pCamera->SetFarClipPlane((float)GetGame()->GetSetup()->lViewRange);
	m_pCamera->SetFarClipPlane((float)GetGame()->GetSetup()->lViewRange);
	m_pCamera->SetPosition(&D3DXVECTOR3(14.0f,12.0f,14.0f));
	m_pCamera->SetView(&D3DXVECTOR3(7,0,7));

	m_pCamera->SetFOVY(D3DXToRadian(GetGame()->GetViewSetup()->ViewFrustumRange));

	m_pFrustum = new render::Frustum;

	m_pRegionCamera = new CRegionCamera;		//�����������
	m_pRegionCamera->SetCameraHAngle((int)GetGame()->GetViewSetup()->ViewPlanformAngle);
}

void CClientRegion::SyncUpdate()
{
	m_pFrustum->CalculateFrustum(m_pCamera->GetViewMatrix(),m_pCamera->GetProjectionMatrix());
	m_pGameMap->Update(m_pFrustum,false);
}



/*
* ����: ��ȡ�ͻ�����Դ
* ժҪ: -
* ����: -
* ����ֵ: ����true
* ����: -
* ��������: -
* �޸���־:
*	2008.03.03 - lpf
*		���Ӷ�ȡ���������б�Ĳ���
*/
BOOL CClientRegion::Load()
{
	CRegion::Load();

	//��ȡ��ͼ�ļ�
	char strTempName[MAX_PATH];
	sprintf(strTempName, "regions/%d", GetResourceID());
	m_pGameMap = new GameMap;
	m_pGameMap->Create(strTempName);
	m_pGameMap->ComputeNormals();
	DWORD dwAmbient = m_pGameMap->GetSunLightAmbientColorforBuild();
	DWORD dwDiffuse = m_pGameMap->GetSunLightDiffuseColor();
	DWORD dwSpecular = m_pGameMap->GetSunLightSpecularColor();
	const D3DXVECTOR3* vlightdir = m_pGameMap->GetSunLightDirection();
	ZeroMemory( &m_RegionLight, sizeof(D3DLIGHT9) );
	/*m_RegionLight.Ambient.a = (float)((dwAmbient&0xff000000)>>24)/255.0f;
	m_RegionLight.Ambient.r = (float)((dwAmbient&0x00ff0000)>>16)/255.0f;
	m_RegionLight.Ambient.g = (float)((dwAmbient&0x0000ff00)>>8)/255.0f;
	m_RegionLight.Ambient.b = (float)(dwAmbient&0x000000ff)/255.0f;*/
	m_RegionLight.Diffuse.a = (float)((dwDiffuse&0xff000000)>>24)/255.0f;
	m_RegionLight.Diffuse.r = (float)((dwDiffuse&0x00ff0000)>>16)/255.0f;
	m_RegionLight.Diffuse.g = (float)((dwDiffuse&0x0000ff00)>>8)/255.0f;
	m_RegionLight.Diffuse.b = (float)(dwDiffuse&0x000000ff)/255.0f;
	m_RegionLight.Specular.a = (float)((dwSpecular&0xff000000)>>24)/255.0f;
	m_RegionLight.Specular.r = (float)((dwSpecular&0x00ff0000)>>16)/255.0f;
	m_RegionLight.Specular.g = (float)((dwSpecular&0x0000ff00)>>8)/255.0f;
	m_RegionLight.Specular.b = (float)(dwSpecular&0x000000ff)/255.0f;
	D3DXVec3Normalize((D3DXVECTOR3*)&m_RegionLight.Direction,vlightdir);
	m_RegionLight.Attenuation0 = 1;
	m_RegionLight.Attenuation1 = 0;
	m_RegionLight.Attenuation2 = 0;
	m_RegionLight.Type       = D3DLIGHT_DIRECTIONAL;
	if (m_pGameMap->GetSkyBoxID())
	{
		m_pGameSkyBox = new GameSkybox;
		if(m_pGameSkyBox->Create(m_pGameMap->GetSkyBoxID()))
		{
			if (m_pGameMap->GetSkyBoxSize())
			{
				m_pGameSkyBox->SetScal(m_pGameMap->GetSkyBoxSize());
			}
		}
		else
		{
			SAFEDESTROY(m_pGameSkyBox);
		}

	}

	sprintf(strTempName, "regions/%d/enveffect", GetResourceID());
	if (utility::File::FileExist(strTempName))
	{
		m_pGameEnvEffect = GameBaseEnvEffect::CreateEnvEffect(strTempName);
	}

	// ��ȡ���������б�
	sprintf(strTempName, "regions/%d/envcreature", GetResourceID());
	if (utility::File::FileExist(strTempName))
	{
		utility::File file;
		utility::Text s;
		utility::Text::Node * pN;
		utility::Text::Variable * pA;
		char strTmp[MAX_PATH];
		string str;

		sprintf(strTmp, "regions/%d/", GetResourceID());

		file.Open(strTempName);
		s.Create(&file);
		file.Close();

		pN = s.GetRootNode()->GetChildNode("list");
		for (DWORD i = 0; i < pN->GetVarCount(); ++i)
		{
			pA = pN->GetVar((int)i);
			str = strTmp;
			str += pA->GetStringValue();
			m_apGameEnvCreature[i] = GameBaseEnvCreature::CreateEnvCreature(str.c_str());
		}

		s.Destroy();
	}

	sprintf(strTempName,"regions\\%d.ssl",GetResourceID());
	//utility::File file;
	//if (file.Open(strTempName))
	//{
	//	file.SkipBytes(8);
	//	DWORD dwNum=0;
	//	file.Read(&dwNum,sizeof(DWORD));
	//	for(DWORD i=0;i<dwNum;i++)
	//	{
	//		REGION_SOUND stRegionSound;
	//		file.Read(&stRegionSound.zpos,sizeof(float));
	//		file.Read(&stRegionSound.xpos,sizeof(float));
	//		file.Read(&stRegionSound.Sound,sizeof(GameBuildList::Build::tagSoundSource));

	//	} 
	//	file.Close();
	//}
	CRFile* prfile_ssl = rfOpen(strTempName);
	if(prfile_ssl)
	{
		stringstream reader;
		prfile_ssl->ReadToStream(reader);
		rfClose(prfile_ssl);
		while(ReadTo(reader, "*"))
		{
			REGION_SOUND stRegionSound;
			reader>>stRegionSound.xpos>>stRegionSound.zpos;
			reader>>stRegionSound.Sound.wID;
			reader>>stRegionSound.Sound.fReferenceDistance;
			stRegionSound.Sound.fHeight=30;
			stRegionSound.bPlaying=false;
			stRegionSound.channel=-1;
			m_RegionSoundList.push_back(stRegionSound);
		} 
	}

	//����ͼ������Ч����
	sprintf(strTempName,"regions\\%d.snd",GetResourceID());
	CRFile* prfile = rfOpen(strTempName);
	if( prfile )
	{
		stringstream reader;
		prfile->ReadToStream(reader);
		rfClose(prfile);
		while(ReadTo(reader, "*"))
		{
			ENV_SOUND stEnvSound;
			stEnvSound.bLoop=true;
			stEnvSound.bPlaying=false;

			reader>>stEnvSound.lMinTime>>stEnvSound.lMaxTime;
			if(stEnvSound.lMinTime>=0 && stEnvSound.lMaxTime>0 && stEnvSound.lMaxTime>stEnvSound.lMinTime)
			{
				stEnvSound.bLoop=false;
				stEnvSound.dwStartTime=GetCurTickCount();
				stEnvSound.lIntervalTime=stEnvSound.lMinTime+random(
					stEnvSound.lMaxTime-stEnvSound.lMinTime);
			}
			string filename;
			stEnvSound.strFileName="sounds/";
			reader>>filename;
			reader>>stEnvSound.nLoops;
			stEnvSound.strFileName+=filename;
			m_EnvSoundList.push_back(stEnvSound);
		}
	}

	// ��С��ͼ
	SAFEDESTROY(m_pMiniMap);
	m_pMiniMap = new CMiniMap;
	m_pMiniMap->Create(128, 128, "datafiles/imagesets/minimap/objectlist");
	sprintf(strTempName, "regions/%d.jpg", GetResourceID());
	m_pMiniMap->LoadMiniMap(strTempName, GetWidth(), GetHeight());

	// ���������ʾ��Ч��07.9.6����ע�ͣ�
	/*map<WORD,CPlayer::tagPlayerQuest>::iterator it;
	for(it=GetGame()->GetMainPlayer()->GetPlayerQuests()->begin();
	it!=GetGame()->GetMainPlayer()->GetPlayerQuests()->end();it++)
	{
	if(it->second.lRegionID==GetExID ||
	GetGame()->GetDupliRegionSetup()->IsDupliRegion(it->second.lRegionID,GetExID) )
	{
	it->second.pEffect=PlayEffect(it->second.lEffectID,it->second.lTileX+0.5f,it->second.lTileY+0.5f);
	it->second.pEffect->SetLoop(true);
	it->second.pEffect->SetTask(true);
	}
	}*/

	// ===============�µ����������ʾ��Ч��07.9.6������ӣ�==============
	CPlayer *pPlayer = GetGame()->GetMainPlayer();
	CShape * pNpc = NULL;//NPC
	CEffect *pEffect = NULL;
	long lMapID = -1;
	list<CQuestRecord*>::iterator it = pPlayer->GetRoleQueue()->GetQuestRecordList()->begin();
	for(; it!=pPlayer->GetRoleQueue()->GetQuestRecordList()->end(); ++it)
	{
		const CQuest *pQuest = CQuestSystem::GetQuest((*it)->GetQuestID());
		if(pQuest)
		{
			pNpc = dynamic_cast<CMonster *>(RecursiveFindObject(TYPE_MONSTER,(char *)pQuest->GetAttribute().GetNPCName().c_str()));
			if(pNpc)
			{
				if(pNpc->GetRegionID()==GetConfigID())
				{
					lMapID = pQuest->GetAttribute().GetEmbracerMapEffect();
					pEffect = PlayEffect(lMapID,pNpc->GetTileX()+0.5f,pNpc->GetTileY()+0.5f);
					pEffect->SetLoop(true);
					pEffect->SetTask(true);
				}
			}
		}
	}
	//=====================================================================
	return true;
}



// �洢
BOOL CClientRegion::Save()
{
	CRegion::Save();
	return true;
}

void CClientRegion::Exit(void)
{
	//�رձ�������
	if(GetGame()->GetAudioList()->GetBackgroundMusic())
	{
		GetGame()->GetAudioList()->ReleaseMusic(GetGame()->GetAudioList()->GetBackgroundMusic());
		//GetGame()->GetAudioList()->FadeOutMusic(GetGame()->GetAudioList()->GetBackgroundMusic(),2000);
		GetGame()->GetAudioList()->SetBackgroundMusic(0);
	}
	//�ͷ�ѭ�����ŵĽ�����Ч
	list<REGION_SOUND>::iterator SndIt=m_RegionSoundList.begin();
	for(;SndIt!=m_RegionSoundList.end();SndIt++)
	{
		char filename[MAX_PATH];
		sprintf(filename,"sounds/regions/%d/%d.wav",GetResourceID(),SndIt->Sound.wID);
		GetGame()->GetAudioList()->Release3DSound(filename,SndIt->channel);
		SndIt->bPlaying=false;
		SndIt->channel=-1;
	}
	//�ͷ�ѭ�����ŵĻ�����Ч
	list<ENV_SOUND>::iterator EnvSndIter=m_EnvSoundList.begin();
	for(;EnvSndIter!=m_EnvSoundList.end();EnvSndIter++)
	{
		if(EnvSndIter->bLoop && EnvSndIter->bPlaying)
		{
			GetGame()->GetAudioList()->Release2DSound(EnvSndIter->strFileName.c_str());
		}
	}
	m_EnvSoundList.clear();

	if(GetGame()->GetWeatherManager()->IsRainSndPlaying())
	{
		GetGame()->GetAudioList()->Release2DSound(GetGame()->GetWeatherManager()->GetRainSoundFile());
		GetGame()->GetWeatherManager()->SetRainSndPlayingState(FALSE);
	}

	if(GetGame()->GetWeatherManager()->IsSnowSndPlaying())
	{
		GetGame()->GetAudioList()->Release2DSound(GetGame()->GetWeatherManager()->GetSnowSoundFile());
		GetGame()->GetWeatherManager()->SetSnowSndPlayingState(FALSE);
	}

	if(GetGame()->GetWeatherManager()->IsFogSndPlaying())
	{
		GetGame()->GetAudioList()->Release2DSound(GetGame()->GetWeatherManager()->GetFogSoundFile());
		GetGame()->GetWeatherManager()->SetFogSndPlayingState(FALSE);
	}
	// �����ж��ѵ�3Dͷ������Ϊ��
	//CPlayer *pPlayer = GetGame()->GetMainPlayer();
	//if(pPlayer && GetGame()->GetCGuiEx() && GetGame()->GetCGuiEx()->GetTeamPageEx())
	//{
		//GetGame()->GetCGuiEx()->GetTeamPageEx()->Set3DIconShape();
		/*for(int i=0; i<pPlayer->GetTeamMateInfo().size(); ++i)
		{
			GetGame()->GetCGuiEx()->GetTeamPageEx()->Set3DIconShape();
		}*/
	//}

	SAFEDELETE(m_pRegionCamera);
	SAFEDELETE(m_pCamera);
	SAFEDELETE(m_pFrustum);

	SAFEDESTROY(m_pGameMap);
	SAFEDESTROY(m_pGameEnvEffect);
	SAFEDESTROY(m_pGameSkyBox);

	// �ͷŻ�����������
	for (int i = 0; i < 4; ++i)
		SAFEDESTROY(m_apGameEnvCreature[i]);

	SAFEDESTROY(m_pMiniMap);
	SAFE_DELETE(m_pFindPath);

	SAFE_DECREF(m_pBossBackImage);
	SAFE_DECREF(m_pBossImage);
}

void CClientRegion::UpdateCamera(void)
{
	CMouse *pMouse = GetMouse();
	CKeyBoard *pKeyboard = GetKeyboard();

	////===============camera control======================

	if (pMouse->IsLeftButtonDown())
	{
		m_eCamOpType = CCO_MOVE;
	}
	else
		if (pMouse->IsMiddleButtonDown())
		{
			m_eCamOpType = CCO_ROTATE;
		}
		else
			if (pMouse->IsRightButtonDown())
			{
				m_eCamOpType = CCO_FLATMOVE;
			}
			else
				m_eCamOpType = CCO_NONE;


	float rotex = 0.01f * (float)(pMouse->GetMouseX() -pMouse->GetOldMouseX());
	float rotey = 0.01f * (float)(pMouse->GetMouseY() -pMouse->GetOldMouseY());

	float scalx = 0.0;
	float scaly = 0.0;

	float movex = 0.01f * (float)(pMouse->GetMouseX() -pMouse->GetOldMouseX());
	float movey = 0.01f * (float)(pMouse->GetMouseY() -pMouse->GetOldMouseY());

	float flatmovex = 0.01f * (float)(pMouse->GetMouseX() -pMouse->GetOldMouseX());
	float flatmovey = 0.01f * (float)(pMouse->GetMouseY() -pMouse->GetOldMouseY());

	static float fRatio = 0.15f;
	if (pKeyboard->IsKeyDown(DIK_ADD))
	{
		fRatio += 0.05f;
		if (fRatio > 0.5f)
		{
			fRatio = 0.5f;
		}
	}

	if (pKeyboard->IsKeyDown(DIK_SUBTRACT))
	{
		fRatio -= 0.05f;
		if (fRatio < 0.05f)
		{
			fRatio = 0.05f;
		}
	}
	if (m_eCamOpType == CCO_MOVE)
	{
		m_pCamera->MovePosition(&D3DXVECTOR3(0,movey,0));
		m_pCamera->MovePosition(&D3DXVECTOR3(0,0,-movex));
		m_pCamera->MoveView(&D3DXVECTOR3(0,movey,0));
		m_pCamera->MoveView(&D3DXVECTOR3(0,0,-movex));
	}
	if (pKeyboard->IsKeyDown(DIK_UPARROW))
	{
		m_eCamOpType = CCO_FLATMOVE;
		flatmovey = fRatio;
	}
	if (pKeyboard->IsKeyDown(DIK_DOWNARROW))
	{
		m_eCamOpType = CCO_FLATMOVE;
		flatmovey = -fRatio;
	}
	if (pKeyboard->IsKeyDown(DIK_LEFTARROW))
	{

		m_eCamOpType = CCO_FLATMOVE;
		flatmovex = fRatio;

	}
	if (pKeyboard->IsKeyDown(DIK_RIGHTARROW))
	{

		m_eCamOpType = CCO_FLATMOVE;
		flatmovex = -fRatio;

	}
	if (m_eCamOpType == CCO_FLATMOVE)
	{
		m_pCamera->FlatMoveFB(flatmovey);
		m_pCamera->FlatMoveLR(-flatmovex);
	}
	if (pKeyboard->IsKeyDown(DIK_HOME))
	{
		m_eCamOpType = CCO_ROTATE;
		rotex = fRatio;
	}
	if (pKeyboard->IsKeyDown(DIK_END))
	{
		m_eCamOpType = CCO_ROTATE;
		rotex = -fRatio;
	}
	if (pKeyboard->IsKeyDown(DIK_PGUP))
	{
		m_eCamOpType = CCO_ROTATE;
		rotey = fRatio;
	}
	if (pKeyboard->IsKeyDown(DIK_PGDN))
	{
		m_eCamOpType = CCO_ROTATE;
		rotey = -fRatio;
	}
	if (m_eCamOpType == CCO_ROTATE)
	{
		m_pCamera->RotatePosition(&D3DXVECTOR3(rotey,rotex,0));    
	}
	if (pKeyboard->IsKeyDown(DIK_LBRACKET))
	{
		m_eCamOpType = CCO_SCALE;
		scalx = fRatio;
	}
	else if (pKeyboard->IsKeyDown(DIK_RBRACKET))
	{
		m_eCamOpType = CCO_SCALE;
		scalx = -fRatio;
	}
	if (m_eCamOpType == CCO_SCALE)
	{
		m_pCamera->MovePosition(&D3DXVECTOR3(0,0,scalx));
	}


	if (pMouse->GetCurrentButton() == MB_WHEEL)
	{
		float fzDelta = (float)((short)HIWORD(pMouse->GetWheelDelta()));
		m_pCamera->MovePosition(&D3DXVECTOR3(0,0,fzDelta * 0.01f));
	}

	m_pFrustum->CalculateFrustum(m_pCamera->GetViewMatrix(),m_pCamera->GetProjectionMatrix());
}

void CClientRegion::ScrollCenter(float x, float y)
{
	if (GetGame()->GetSetup()->bfreecamera)
	{
		/*static bool bFollow = true;
		if(GetKeyboard()->IsKeyDown(DIK_LCONTROL) && GetKeyboard()->GetCurrentKey() == DIK_F2)
		{
		bFollow = !bFollow;
		}

		*/
		UpdateCamera();



	}
	else
	{
		UpdateRegionCamera(x, y);
	}
}



/*
* ����: ���³������
* ժҪ: �ڸ��³��������ͬʱ,Ҳ������������ݸ�����
* ����: x - ��Ҵ��������X����
*		 y - ��Ҵ��������Y����
* ����ֵ: -
* ����: lpf
* ��������: 2008.02.02
*/
void CClientRegion::UpdateRegionCamera(float x, float y)
{
	m_pRegionCamera->Update();
	m_pRegionCamera->SetHRotation(g_bDebug);
	m_pRegionCamera->SetCamera(m_pCamera, x, y);

	m_pFrustum->CalculateFrustum(m_pCamera->GetViewMatrix(), m_pCamera->GetProjectionMatrix());
}

//�����������ʽ
void CClientRegion::TransformCamer(long lOperaMode,long lParam1,long lParam2,long lParam3)
{
	m_pRegionCamera->TransformCamer(lOperaMode,lParam1,lParam2,lParam3);
}

int GetDegree(int x,int y,int cx,int cy)
{
	int dx=x-cx,dy=y-cy;
	if(dx==0)return dy<0?-90:90;
	float fRad=atanf(dy*1.0f/dx*1.0f);
	int degree=(int)D3DXToDegree(fRad);
	if(dx<0)degree+=180;
	return degree;
}

float CClientRegion::CalcDirection(float sx, float sy, float sh, int dx, int dy)
{
	static D3DXMATRIX matFinal;
	static D3DXVECTOR3 pos2,pos;
	static char msg[256];
	memset(msg,0,256);

	pos.x = sy;
	pos.y = sh;
	pos.z = sx;

	CalculateFinalTransformMatrix(matFinal);
	D3DXVec3TransformCoord(&pos2,&pos,&matFinal);


	render::Interface *pInterface = render::Interface::GetInstance();
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	GameSelectRay cSelRay;
	POINT pt={dx, dy};
	ScreenToClient(g_hWnd, &pt);

	int degree=GetDegree(pt.x,pt.y,(int)(pos2.x*WINDOW_WIDTH/SCREEN_WIDTH),(int)(pos2.y*WINDOW_HEIGHT/SCREEN_HEIGHT));
	//degree=degree;
	wsprintf(msg,"x:%d y:%d cx:%d sy:%d",pt.x,pt.y,0,0);
	//SetWindowText(g_hWnd,msg);

	D3DXVECTOR3 vPosition = D3DXVECTOR3(sy,sh,sx);
	cSelRay.CalculateRay(&rc,m_pCamera,pt);


	D3DXVECTOR3 tmp=(*m_pCamera->GetView())-(*m_pCamera->GetPosition());
	D3DXVec3Normalize(&tmp,&tmp);
	float fDirect;
	int iDir;
	if( tmp.x != 0 )
	{
		float fDir =  atanf( tmp.z/tmp.x );
		if( tmp.x < 0 )fDir += D3DX_PI;
		fDirect = - fDir-D3DX_PI/2;
	}
	else
	{
		fDirect = - tmp.z/fabsf(tmp.z)*D3DX_PI/2-D3DX_PI/2 ;
	}
	iDir=(int)D3DXToDegree(fDirect);
	iDir+=360;
	//iDir=iDir/45*45;
	wsprintf(msg+strlen(msg)," %d",iDir);
	//iDir+=degree;
	//if(iDir<0)iDir+=360;
	//iDir = (( iDir ) / 45) & 7;
	//wsprintf(msg+strlen(msg)," ������Ƕ�:%d",iDir);


	//������
	D3DXPLANE plane;
	plane.a = 0.0f;
	plane.b = 1.0f;
	plane.c = 0.0f;
	plane.d = - vPosition.y;

	const D3DXVECTOR3	*pOrig  = cSelRay.GetOrigination();
	const D3DXVECTOR3	*pDir = cSelRay.GetDirection();
	D3DXVECTOR3 vIntersection(0,0,0);

	FM_RadialIntersectedPlane(*pOrig,*pDir,plane,vIntersection);

	//ȡ������ƶ�����,�ƶ�����
	float fDirection(0);

	D3DXVECTOR3 dir = - (vIntersection - vPosition);
	D3DXVec3Normalize(&dir,&dir);

	if( dir.x != 0 )
	{
		float fDir =  atanf( dir.z/dir.x );
		if( dir.x < 0 )
			fDir += D3DX_PI;
		fDirection = - fDir-D3DX_PI/2;
	}
	else
	{
		if(dir.z != 0)
			fDirection = - dir.z/fabsf(dir.z)*D3DX_PI/2-D3DX_PI/2 ;
	}

	return fDirection;
	////������Ϸ����
	//int iDirect = ((int)(360 - D3DXToDegree(-fDirection) + 90)) % 360;//) / 45;//(int)((D3DXToDegree(fDirection) - (180 + 22.5)) / 45);
	//iDirect = (( iDirect + 22 ) / 45) & 7;

	////iDir = ((int)(360 - (iDir+degree) + 90)) % 360;//) / 45;//(int)((D3DXToDegree(fDirection) - (180 + 22.5)) / 45);
	//iDir+=degree;
	//if(iDir>360)iDir-=360;
	//if(iDir<0)iDir+=360;

	//wsprintf(msg+strlen(msg)," %d",(( iDir + 22 ) / 45) & 7 );
	////SetWindowText(g_hWnd,msg);

	//return (( iDir + 22 ) / 45) & 7;

	//return iDirect;
}

DWORD CClientRegion::CalcMoveDirection(POINT &ptMouse)
{
	render::Interface *pInterface = render::Interface::GetInstance();
	int iWidth = pInterface->GetWndWidth();
	int iHeight= pInterface->GetWndHeight();
	//������Ļ�Ƕ�
	int x = ptMouse.x - iWidth / 2;
	int y = (iHeight - ptMouse.y) - iHeight / 2;//flip
	//������Ļ�Ƕ�
	int iReta(0);
	if (x == 0)
	{
		iReta = (y > 0)? 90 : 180;
	}
	else
	{
		iReta = (int)D3DXToDegree( atanf( (float)y / (float)x ) );
		if( x < 0 )
			iReta += (int)D3DXToDegree(D3DX_PI);
	}


	//�������������
	D3DXVECTOR3 vDirection = *GetCamera()->GetView() - *GetCamera()->GetPosition();
	D3DXVec3Normalize(&vDirection,&vDirection);
	float fDirect;
	if( vDirection.x == 0 )
	{
		fDirect = (vDirection.z >0)? D3DXToRadian(90) : D3DXToRadian(270);
	}
	else
	{
		fDirect =  atanf( vDirection.z/vDirection.x );
		if (vDirection.x < 0)
			fDirect += D3DX_PI;
	}
	int iDir = 360 - (int)D3DXToDegree(fDirect) - iReta + 180;
	DWORD dwDir = (DWORD)(iDir % 360);
	//OutputConsole("%d\n",iDir % 360);
	char szMsg[256];
	wsprintf(szMsg,"%d",dwDir);
	//SetWindowText(g_hWnd,szMsg);
	return dwDir;
}

// ��ȡ���ָ���ĸ���
bool CClientRegion::CalcMousePoint(long mx, long my, float &getx, float &gety)
{
	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer3D   *pLayer3D   = pInterface->GetLayer3D();

	//ȡ�ø���
	RECT rc = {0,0,pInterface->GetWndWidth(),pInterface->GetWndHeight()};
	POINT pt = {mx, my};
	static GameSelectRay cSelRay;
	cSelRay.CalculateRay(&rc, m_pCamera, pt);

	bool bSelected = false;
	D3DXVECTOR3 spoint;
	DWORD dwGridIndex = m_pGameMap->CalculateHoverGrid(&cSelRay,spoint);

	//by test
	m_dwHoverGrid = dwGridIndex;

	if (dwGridIndex != 0xFFFFFFFF)
	{
		/*getx = dwGridIndex / m_pGameMap->GetWidth();
		gety = dwGridIndex % m_pGameMap->GetWidth();*/
		getx = spoint.z;
		gety = spoint.x;
		return true;
	}
	return false;
}

void CClientRegion::DisableFog()
{
	render::Interface *pInterface = render::Interface::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pInterface->GetDevice();

	// Enable fog blending.
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
}

void CClientRegion::CalculateFinalTransformMatrix(D3DXMATRIX &matFinal)
{
	//get transform result
	RECT rc = {0,0,SCREEN_WIDTH, SCREEN_HEIGHT};	

	int iW = rc.right - rc.left + 1,iH = rc.bottom - rc.top + 1;

	D3DXMATRIX matViewClip;
	D3DXMatrixIdentity(&matViewClip);
	matViewClip._41 = matViewClip._11  = (float)iW/2;
	matViewClip._42  = matViewClip._22  = (float)iH/2;
	matViewClip._22 = -matViewClip._22;	

	D3DXMATRIX matTemp;
	D3DXMatrixMultiply(&matFinal,m_pCamera->GetViewMatrix(),m_pCamera->GetProjectionMatrix());
	D3DXMatrixMultiply(&matFinal,&matFinal,&matViewClip);
}

// ��ȡ���ָ����shape
CShape* CClientRegion::GetHoverShape(void)
{
	CShape* pRetShape = NULL;
	//ȡ��ѡ�ֵ�Ŀ��
	render::Interface *pInterface = render::Interface::GetInstance();
	RECT rc = {0,0,pInterface->GetWndWidth(),pInterface->GetWndHeight()};
	POINT pt = {GetMouse()->GetMouseX(), GetMouse()->GetMouseY()};
	int x = pt.x, y = pt.y;
	pt.x = x;
	pt.y = y;

	const D3DXMATRIX* matTRS;
	static GameSelectRay cSelRay;
	cSelRay.CalculateRay(&rc, m_pCamera, pt);
	for(list<CBaseObject*>::iterator it = m_listObject.begin(); it != m_listObject.end(); it++)
	{
		CShape *pShape = (CShape*)(*it);
		if (pShape && 
			pShape->GetType()!=TYPE_EFFECT &&
			pShape->GetType()!=TYPE_SKILL &&
			pShape->GetType()!=TYPE_SUMMON )
		{
			CDisplayModel* pDisplayModel = pShape->GetDisplayModel();
			CDisplayModel* pdspmodhorse = NULL;
			//CModelList::tagModel *p = GetGame()->GetModelList()->GetModel(pShape->GetType(),pShape->GetGraphicsID());
			if (pDisplayModel)
			{
				float fHeight = 0.0f;
				//if (pShape->GetType() == TYPE_GOODS)
				//{
				//	//������Ʒ��ʾ�߶�
				//	render::BoundingBox *pBoundingBox = p->pModel->GetSizeBox();
				//	fHeight += pBoundingBox->GetOutsideRadius();
				//}
				//D3DXMatrixTranslation(&matTRS,pShape->GetPosY(),m_pGameMap->CalculateHeight(pShape->GetPosY(),pShape->GetPosX()) + fHeight,pShape->GetPosX());
				D3DXVECTOR3 vI;
				float fd;
				render::BoundingBox * pPickbox = pDisplayModel->GetPickBox();
				GameModel::PickMesh * pHorsepickmesh = NULL;
				GameModel::PickMesh * pShapepickmesh = pDisplayModel->GetPickMesh();
				CPlayer* pPlayer = NULL;
				matTRS = pShape->GetAnimInfo()->GetWorldMatrix();
				if (pShape->GetType() == TYPE_PLAYER)
				{
					pPlayer = (CPlayer*)pShape;
					pdspmodhorse = pPlayer->GetHorse()->GetDisplayModel();
					if (pPlayer->IsRide()&&pdspmodhorse)
					{
						pPickbox = pdspmodhorse->GetPickBox();
						pHorsepickmesh = pdspmodhorse->GetPickMesh();
						matTRS = pPlayer->GetHorse()->GetAnimInfo()->GetWorldMatrix();
					}
					else
					{
						pPickbox = pDisplayModel->GetPickBox();
						pHorsepickmesh = NULL;
					}
				}
				if ( cSelRay.TestBoundingBox(matTRS,pPickbox,&vI,&fd))
				{
					bool btestpickmesh = false;
					if (pShapepickmesh&&pDisplayModel->GetLoadState() == Load_Did)
					{
						btestpickmesh = true;
						pDisplayModel->ProcessPickMesh(pShape->GetAnimInfo());
						if (pdspmodhorse&&pHorsepickmesh)
						{
							pdspmodhorse->ProcessPickMesh(pPlayer->GetHorse()->GetAnimInfo());
							if (cSelRay.TestPickMesh(pHorsepickmesh)||cSelRay.TestPickMesh(pShapepickmesh))
							{
								btestpickmesh = false;
							}

						}
						else
						{
							pDisplayModel->ProcessPickMesh(pShape->GetAnimInfo());
							if (cSelRay.TestPickMesh(pShapepickmesh))
							{
								btestpickmesh = false;
							}
						}


					}
					if (!btestpickmesh)
					{
						// ������ָ���ڸ�Shape��
						if (pShape->GetType() == TYPE_GOODS)
						{
							GetGame()->GetGameControl()->SetMouseShapeGoods(pShape);
						}
						else
						{
							// ��ס�ո�ѡ��˳�� Player->NPC->Monster
							if (GetKeyboard()->IsKeyDown(DIK_SPACE))
							{
								bool bReplace = false;
								if (pRetShape==NULL)
								{
									pRetShape = pShape;
								}
								else if (pShape->GetState()!=CShape::STATE_DIED)
								{
									switch(pShape->GetType()) 
									{
										/*case TYPE_NPC:
										if (pRetShape->GetType()!=TYPE_PLAYER)
										bReplace = true;
										break;*/

									case TYPE_PLAYER:
										bReplace = true;
										break;

									case TYPE_MONSTER:
										if (/*pRetShape->GetType()!=TYPE_NPC &&*/ pRetShape->GetType()!=TYPE_PLAYER)
											bReplace = true;
										break;
									}

									if (bReplace)
									{
										// ͬ����λ�� ��->ǰ ������ѡȡ
										if (pShape->GetType() == pRetShape->GetType() && pRetShape->GetState()!=CShape::STATE_DIED)
										{
											if (pShape->GetTileY() < pRetShape->GetTileY())
												pRetShape = pShape;
											else if (pShape->GetTileY() == pRetShape->GetTileY() &&
												pShape->GetTileX() > pRetShape->GetTileX())
												pRetShape = pShape;
										}
										else
											pRetShape = pShape;
									}
								}
							}
							else	// �����ո�ѡ��˳�� Monster->NPC->Player
							{
								if (pRetShape==NULL)
								{
									pRetShape = pShape;
								}
								else if(pShape->GetState()!=CShape::STATE_DIED)
								{
									bool bReplace = false;
									switch(pShape->GetType()) 
									{
										/*case TYPE_NPC:
										if (pRetShape->GetType()!=TYPE_MONSTER)
										bReplace = true;
										break;*/

									case TYPE_PLAYER:
										if (/*pRetShape->GetType()!=TYPE_NPC &&*/ pRetShape->GetType()!=TYPE_MONSTER)
											bReplace = true;
										break;

									case TYPE_MONSTER:
										bReplace = true;
										break;
									}

									if (bReplace)
									{
										// ͬ����λ�� ǰ->�� ������ѡȡ
										if (pShape->GetType() == pRetShape->GetType() && pRetShape->GetState()!=CShape::STATE_DIED)
										{
											if (pShape->GetTileY() > pRetShape->GetTileY())
												pRetShape = pShape;
											else if (pShape->GetTileY() == pRetShape->GetTileY() &&
												pShape->GetTileX() < pRetShape->GetTileX())
												pRetShape = pShape;
										}
										else
											pRetShape = pShape;
									}
								}
							}
						}
					}

				}
			}
		}
	}

	return pRetShape;
}

// ��ʾС��ͼ
void CClientRegion::DisplayMiniMap(int x, int y)
{
	if (m_pMiniMap)
	{
		m_pMiniMap->CleanShapeList();
		// ��ҵĹ�����Ϣ
		CCountryList::tagCountryList* countrySelf = CCountryList::GetCountry( GetGame()->GetMainPlayer()->GetCountry() );
		for (list<CBaseObject*>::iterator it=GetObjectList()->begin(); it!=GetObjectList()->end(); it++)
		{
			CShape* pShape = (CShape*)(*it);
			if (pShape == NULL)
				continue;

			DWORD dwType(MT_MONSTER);
			switch(pShape->GetType())
			{
			case TYPE_PLAYER:
				{
					// �����Լ�
					if (pShape!=GetGame()->GetMainPlayer())
					{
						CPlayer * pOtherPlayer = dynamic_cast<CPlayer*>(pShape);
						if (pOtherPlayer == NULL || GetGame()->GetMainPlayer()->IsTeammate(pShape->GetExID()) || pOtherPlayer->GetIsGMConcealment())
							continue;
						// ��������״̬��������һ��
						else if(!((CPlayer*)pShape)->GetIsConceal())
							continue;
						// �������
						else
						{
							// ����ҵĹ�����Ϣ
							CCountryList::tagCountryList* countryAnother = CCountryList::GetCountry( pOtherPlayer->GetCountry() );
							if( countrySelf && countryAnother )
							{
								// ͬһ��Ӫ
								if( countrySelf->nLeague == countryAnother->nLeague )
								{
									// ����
									if(pOtherPlayer->GetPkValue() > 20)
										dwType = MT_RED;
									// ����
									else if(pOtherPlayer->GetPVPValue() > 0)
										dwType = MT_PIRATES;
									else
										dwType = MT_LEAGPLAYER;
								}
								else
								{
									// ����
									if(pOtherPlayer->GetPVPValue() > 0)
										dwType = MT_PIRATES;
									// ����
									else if(pOtherPlayer->GetPkValue() > 20)
										dwType = MT_RED;
									else
										dwType = MT_UNLEAGPLAYER;
								}
							}
						}
					}
					else
						continue;
				}
				break;

			case TYPE_MONSTER:
				{
					CMonster* pMonster = dynamic_cast<CMonster*>(pShape);
					if(pMonster)
					{
						// �������������������״̬������ִ����һ������
						if(pMonster->IsDied() || !pMonster->GetIsConceal())
							continue;
						// ��νͼ��
						if( 0 != pMonster->GetMonsterIconID() )
							dwType = pMonster->GetMonsterIconID();
						// ��ĳ������״̬��Monster
						long lEffectID = pMonster->GetStateByQuest();
						if( -1 != lEffectID )
						{
							// ��Ӧ������Сͼ��
							dwType = lEffectID;
						}
						// һ��NPC
						if(dwType == MT_MONSTER && 
							pMonster->GeteNpcType() == NT_Normal && 
							strcmp(pMonster->GetName(),"$space"))
						{
							// ��NPC�Ĺ�����Ϣ
							CCountryList::tagCountryList* countryAnother = CCountryList::GetCountry( pMonster->GetCountry() );
							if( countrySelf && countryAnother )
							{
								// ͬһ��Ӫ
								if( countrySelf->nLeague == countryAnother->nLeague )
								{
									dwType = MT_LEAGNPC;
								}
								else
								{
									dwType = MT_UNLEAGNPC;
								}
							}
							else
								dwType = MT_LEAGNPC;
						}
					}
				}
				break;

			case TYPE_EFFECT:
				{
					if( ((CEffect*)pShape)->IsTask() )
					{
						dwType=MT_TASK;
					}
					else
						continue;
				}
				break;
			case TYPE_COLLECTION:
				{
					CCollectionGoods* pCollectGoods = dynamic_cast<CCollectionGoods*>(pShape);
					if (pCollectGoods!=NULL)
					{
						dwType = pCollectGoods->GetRadarID();
					}
				}break;

			default:
				continue;
			}

			if (pShape!=GetGame()->GetMainPlayer())
			{
				m_pMiniMap->AddToShapeList(pShape->GetPosX(),pShape->GetPosY(),dwType);
			}
		}

		// ��ö��ѵ���Ϣ
		//CTeamPageEx *pTeamPage = GetGame()->GetCGuiEx()->GetTeamPageEx();
		//if(pTeamPage)
		//{
		//	vector<CTeamPageEx::tagTeam>::iterator itbegin = pTeamPage->GetTeamInfo().begin();
		//	vector<CTeamPageEx::tagTeam>::iterator itend = pTeamPage->GetTeamInfo().end();
		//	for (; itbegin!=itend; ++itbegin)
		//	{
		//		// ������������ͬһ��������
		//		if(itbegin->PlayerID != GetGame()->GetMainPlayer()->GetExID() && itbegin->bIsOnline &&
		//			itbegin->lRegionID == GetGame()->GetMainPlayer()->GetRegionID() &&
		//			itbegin->RegionGuid == GetGame()->GetMainPlayer()->GetRegionGUID())
		//		{
		//			m_pMiniMap->AddToShapeList((DWORD)(itbegin->fPosX),(DWORD)(itbegin->fPosY),MT_PARTNER,itbegin->strPlayerName.c_str());
		//		}
		//	}
		//}

		// �Լ�
		m_pMiniMap->SetMainPlayer(GetGame()->GetMainPlayer()->GetTileX(),
			GetGame()->GetMainPlayer()->GetTileY(),
			GetGame()->GetMainPlayer()->GetDir());

		m_pMiniMap->DrawMiniMap(x, y);
	}
}

void CClientRegion::_Debug_RenderMap(void)
{
	render::Interface *pInterface = render::Interface::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pInterface->GetDevice();
	render::Interface::Layer3D *pLayer3D = pInterface->GetLayer3D();

	static D3DXVECTOR3 vertex[6],normal[6];
	static D3DXVECTOR2 uv0[6];
	static DWORD color[6];

	// vertex order   uv order 
	// 3----2         0----1 
	// |    |         |    | 
	// |    |         |    | 
	// 0----1         3----2 
	//-----------------------------------------------------------------------------

	static DWORD dwCurTime;
	dwCurTime = timeGetTime();
	DWORD *pdwIndexArray = m_pGameMap->GetGridVisibleIndexArray();

	GameMap::tagGrid     * pGridArray = m_pGameMap->GetGridArray();
	D3DXVECTOR3 * pPosArray  = m_pGameMap->GetTerrainPositionArray();

	for(DWORD n = 0; n< m_pGameMap->GetGridVisibleIndexCount(); n++)
	{	
		DWORD dwGridIndex = pdwIndexArray[n];

		GameMap::tagGrid &tGrid = pGridArray[dwGridIndex];

		if (dwGridIndex == m_dwHoverGrid)
		{
			pLayer3D->_DEBUG_DrawTriangle(&pPosArray[tGrid.dwVertexIndex[0]],
				&pPosArray[tGrid.dwVertexIndex[1]],
				&pPosArray[tGrid.dwVertexIndex[2]],
				0x80FF00FF);
			pLayer3D->_DEBUG_DrawTriangle(&pPosArray[tGrid.dwVertexIndex[0]],
				&pPosArray[tGrid.dwVertexIndex[2]],
				&pPosArray[tGrid.dwVertexIndex[3]],
				0x80FF00FF);
		}
		else
		{
			pLayer3D->_DEBUG_DrawTriangle(&pPosArray[tGrid.dwVertexIndex[0]],
				&pPosArray[tGrid.dwVertexIndex[1]],
				&pPosArray[tGrid.dwVertexIndex[2]],
				0x80FFFFFF);
			pLayer3D->_DEBUG_DrawTriangle(&pPosArray[tGrid.dwVertexIndex[0]],
				&pPosArray[tGrid.dwVertexIndex[2]],
				&pPosArray[tGrid.dwVertexIndex[3]],
				0x80FFFFFF);
		}
		int x =   (int)dwGridIndex % m_pGameMap->GetWidth();
		int z =   (int)dwGridIndex / m_pGameMap->GetWidth();

		tagCell *pCell = GetCell(z,x);
		switch(pCell->bBlock)
		{
		case 0:
			break;
		case 1:
			pLayer3D->_DEBUG_DrawTriangle(&pPosArray[tGrid.dwVertexIndex[0]],
				&pPosArray[tGrid.dwVertexIndex[1]],
				&pPosArray[tGrid.dwVertexIndex[2]],
				0x80800000);
			pLayer3D->_DEBUG_DrawTriangle(&pPosArray[tGrid.dwVertexIndex[0]],
				&pPosArray[tGrid.dwVertexIndex[2]],
				&pPosArray[tGrid.dwVertexIndex[3]],
				0x80800000);
			break;
		case 2:
			pLayer3D->_DEBUG_DrawTriangle(&pPosArray[tGrid.dwVertexIndex[0]],
				&pPosArray[tGrid.dwVertexIndex[1]],
				&pPosArray[tGrid.dwVertexIndex[2]],
				0x80ff0000);
			pLayer3D->_DEBUG_DrawTriangle(&pPosArray[tGrid.dwVertexIndex[0]],
				&pPosArray[tGrid.dwVertexIndex[2]],
				&pPosArray[tGrid.dwVertexIndex[3]],
				0x80ff0000);
			break;
		case 3:
			pLayer3D->_DEBUG_DrawTriangle(&pPosArray[tGrid.dwVertexIndex[0]],
				&pPosArray[tGrid.dwVertexIndex[1]],
				&pPosArray[tGrid.dwVertexIndex[2]],
				0x800000ff);
			pLayer3D->_DEBUG_DrawTriangle(&pPosArray[tGrid.dwVertexIndex[0]],
				&pPosArray[tGrid.dwVertexIndex[2]],
				&pPosArray[tGrid.dwVertexIndex[3]],
				0x800000ff);
			break;
		}

	}

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
}

// ������Ч
CEffect *CClientRegion::PlayEffect(long lGraphicsID, float x, float y, float hight, float fDir,bool bAsyn)
{
	CEffect* pEffect = new CEffect;
	pEffect->SetGraphicsID(lGraphicsID);
	pEffect->SetDisplayModel(bAsyn);
	pEffect->SetDisplayModelGroup(bAsyn);
	pEffect->SetPosXY(x, y);
	CGUID guid;
	CGUID::CreateGUID(guid);
	pEffect->SetExID(guid);

	//Ч������
	map<DWORD,CEffectSetup::tagEffectSetup>::iterator iter=CEffectSetup::GetEffectSetup()->find(pEffect->GetGraphicsID());
	if(iter!=CEffectSetup::GetEffectSetup()->end())
	{
		if (iter->second.bEnable)
		{
			pEffect->SetLight(iter->second.dwColor,iter->second.dwType,iter->second.fRadius,iter->second.dwFadeStartTime,iter->second.dwFadeLength);
		}
	}

	//��Ϊ��ʱ��ӣ������Effect���߼��������������������������ԣ�
	if (hight==0.f)
	{
		GameMap *pGameMap = GetGameMap();
		hight = pGameMap->CalculateHeight(y, x);
	}

	if (fDir!=-1.f)
	{
		pEffect->SetDirEx(fDir);
		pEffect->SetNowDir(fDir);
	}

	pEffect->SetHeight(hight);
	AddObject(pEffect);
	return pEffect;
}



/*
* ����: ���������Ч
* ժҪ: ���ݴ������Ķ����㲿��Ϣ,�������ҽ��ŵ�Ϊ��׼������Ч�Ĳ���
*		���Զ��ж϶���ǰ�����Ĳ�ͬ���β���ѡ���ԵĲ���
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.7.24
* �޸���־:
*	2008.09.26 - lpf
*		�޸��������ЧID��ȡ�÷�ʽ�������Ǵ�GameMap�µ�һ���ӿ���ȡ�õ�ǰ��ͼ������������Ӧ����Ҫ���ŵ���ЧID
*	2008.10.09 - lpf
*		Ϊ�ֲ����������˳�ʼֵ
*/
void CClientRegion::PlayEffectFoot(CPlayer * pPlayer)
{
	if (pPlayer == NULL)
		return;

	if (pPlayer->GetAction() == CShape::ACT_WALK ||
		pPlayer->GetAction() == CShape::ACT_RUN)
	{
		static float	   fFloorHeight = 0.0f;
		long			   lId	   = 0;
		const static float fHeight = 0.040f;//0.036f;

		CClientRegion::tagCell * pCell = GetCell(pPlayer->GetTileX(), pPlayer->GetTileY());
		if (pCell)
		{
			lId = GetGameMap()->GetFootEffectId(pCell->lMaterial);
			if (lId == 0)
				return;
		}else
			return;

		if (pPlayer->IsRide())
		{
			static D3DXVECTOR3 vLocatorLF(0.0f, 0.0f, 0.0f);
			static D3DXVECTOR3 vLocatorRF(0.0f, 0.0f, 0.0f);
			static D3DXVECTOR3 vLocatorLB(0.0f, 0.0f, 0.0f);
			static D3DXVECTOR3 vLocatorRB(0.0f, 0.0f, 0.0f);

			static bool bLFPlay = false;
			static bool bRFPlay = false;
			static bool bLBPlay = false;
			static bool bRBPlay = false;

			CDisplayModel * pDisplayModel = pPlayer->GetHorse()->GetDisplayModel();
			AnimInfo * pAnimInfo = pPlayer->GetHorse()->GetAnimInfo();

			fFloorHeight = GetGameMap()->CalculateHeight(pPlayer->GetPosY(), pPlayer->GetPosX());
			pDisplayModel->GetLocatorPos(pAnimInfo, CShape::LOCATOR_LEFTHAND, vLocatorLF.x, vLocatorLF.y, vLocatorLF.z);
			pDisplayModel->GetLocatorPos(pAnimInfo, CShape::LOCATOR_RIGHTHAND, vLocatorRF.x, vLocatorRF.y, vLocatorRF.z);
			pDisplayModel->GetLocatorPos(pAnimInfo, CShape::LOCATOR_LEFTFOOT, vLocatorLB.x, vLocatorLB.y, vLocatorLB.z);
			pDisplayModel->GetLocatorPos(pAnimInfo, CShape::LOCATOR_RIGHTFOOT, vLocatorRB.x, vLocatorRB.y, vLocatorRB.z);

			if (abs(fFloorHeight - vLocatorLF.z) < fHeight)
				bLFPlay = true;
			else if (abs(fFloorHeight - vLocatorRF.z) < fHeight)
				bRFPlay = true;
			else if (abs(fFloorHeight - vLocatorLB.z) < fHeight)
				bLBPlay = true;
			else if (abs(fFloorHeight - vLocatorRB.z) < fHeight)
				bRBPlay = true;

			if (bLFPlay)
			{
				PlayEffect(lId, vLocatorLF.x, vLocatorLF.y, fFloorHeight, pPlayer->GetDirEx());
				bLFPlay = false;
			}else if (bRFPlay)
			{
				PlayEffect(lId, vLocatorRF.x, vLocatorRF.y, fFloorHeight, pPlayer->GetDirEx());
				bRFPlay = false;
			}else if (bLBPlay)
			{
				PlayEffect(lId, vLocatorLB.x, vLocatorLB.y, fFloorHeight, pPlayer->GetDirEx());
				bLBPlay = false;
			}else if (bRBPlay)
			{
				PlayEffect(lId, vLocatorRB.x, vLocatorRB.y, fFloorHeight, pPlayer->GetDirEx());
				bRBPlay = false;
			}
		}else
		{
			static D3DXVECTOR3 vLocatorL(0.0f, 0.0f, 0.0f);
			static D3DXVECTOR3 vLocatorR(0.0f, 0.0f, 0.0f);

			static bool bLPlay  = false;
			static bool bRPlay  = false;

			fFloorHeight = GetGameMap()->CalculateHeight(pPlayer->GetPosY(), pPlayer->GetPosX());
			pPlayer->GetLocatorPos(CShape::LOCATOR_LEFTFOOT, vLocatorL.x, vLocatorL.y, vLocatorL.z);
			pPlayer->GetLocatorPos(CShape::LOCATOR_RIGHTFOOT, vLocatorR.x, vLocatorR.y, vLocatorR.z);

			if (abs(fFloorHeight - vLocatorL.z) < fHeight)
				bLPlay = true;
			else if (abs(fFloorHeight - vLocatorR.z) < fHeight)
				bRPlay = true;

			if (bLPlay)
			{
				PlayEffect(lId, vLocatorL.x, vLocatorL.y, fFloorHeight, pPlayer->GetDirEx());
				bLPlay = false;
			}else if (bRPlay)
			{
				PlayEffect(lId, vLocatorR.x, vLocatorR.y, fFloorHeight, pPlayer->GetDirEx());
				bRPlay = false;
			}
		}
	}
}



//������ɫ���볡��
void CClientRegion::OnMainPlayerEnter()
{
	//�ͷ���ǰ������
	GetGame()->GetAudioList()->ReleaseMusic(GetGame()->GetAudioList()->GetBackgroundMusic());
	map<long,CGame::tagRegion>::iterator RegionDataIter=GetGame()->GetRegionSetup()->find(
		GetGame()->GetRegion()->GetResourceID());
	if(RegionDataIter!=GetGame()->GetRegionSetup()->end())
	{
		if(!RegionDataIter->second.bgMusicPlaying)
		{
			//�³����������ֽ���
			RegionDataIter->second.bgMusicPlaying=true;	
			const char *filename=RegionDataIter->second.strBgMusic.c_str();
			GetGame()->GetAudioList()->FadeInMusic(filename,8000,0);
			GetGame()->GetAudioList()->SetBackgroundMusic(filename);
		}
	}
	GetGame()->SetGameState(GS_RUN);
    //��ʾmini�������gr
    FireUIEvent("MiniPetPage", "MyShow");
	//***��ʾ���ϵ�����****ADD BY ��ǫ***
	GetGame()->GetMainPlayer()->UpdateAllWeaponShow();
	//***��ʾ���ϵ�����****ADD BY ��ǫ***
	GetGame()->GetMainPlayer()->SetDisplayModelGroup();
	
	// ������볡����ʱ������Ŀ����ƶ���ֹͣ�ƶ�
	if(GetGame()->GetGameControl())
	{
		if(GetGame()->GetGameControl()->GetMPActionMode() == CGameControl::MPAM_AutoMove && 
			GetGame()->GetGameControl()->GetAutoMoveMode() == CGameControl::AMM_TO_DESTDIR)
		{
			GetGame()->GetGameControl()->AutoMoveStop();
		}
	}

	// ���壺���ų����л� ��ʾ����
	/*if( CRegionList::tagRegion* pstRegion = CRegionList::GetRegion( GetGame()->GetRegion()->GetResourceID() ) )
	{
		GetGame()->GetCGuiEx()->GetEffectUIManager()->AddEnterMapGroup( pstRegion->lID, pstRegion->lIconID );
	}*/
	GetGame()->ReSetKeyboard();
}

// ����������ĵĵĸ߶�
float CClientRegion::GetCellHeight(float x, float y)
{
	GameMap *pGameMap = GetGameMap();
	return GetGameMap()->CalculateHeight(y, x);
}


//3ά����ת��2ά����
void  CClientRegion::Coordinate3DTo2D(float f3DX,float f3DY,float f3DH,long& l2DX,long &l2DY)
{
	D3DXVECTOR3 pos(f3DY,f3DH,f3DX);
	D3DXMATRIX matFinal;
	CalculateFinalTransformMatrix(matFinal);
	D3DXVECTOR3 pos2;
	D3DXVec3TransformCoord(&pos2,&pos,&matFinal);
	l2DX = (long)pos2.x;
	l2DY = (long)pos2.y;
}


CGoods * CClientRegion::RemoveGoods(CGUID& guID,DWORD dwPos,LONG lExtendID,DWORD dwAmount)
{
	CGoods *pGoods=dynamic_cast<CGoods *>(FindChildGoods(guID));
	if(pGoods==NULL)
	{
		long x=dwPos%GetWidth(),y=dwPos/GetWidth();
		for(list<CBaseObject*>::iterator it = m_listObject.begin(); it != m_listObject.end(); ++it)
		{
			pGoods=dynamic_cast<CGoods *>((*it));
			if ( pGoods && pGoods->GetTileX() == x && pGoods->GetTileY()==y )
			{
				break;
			}
			pGoods=NULL;
		}
	}
	if( pGoods==NULL )
	{
		return NULL;
	}
	if( GetGame()->GetGameControl()->GetMouseShape()==pGoods )
	{
		GetGame()->GetGameControl()->SetMouseShape(NULL);
	}
	if( GetGame()->GetGameControl()->GetMouseShapeGoods()==pGoods )
	{
		GetGame()->GetGameControl()->SetMouseShapeGoods(NULL);
	}
	RemoveObject(pGoods);
	return pGoods;
}

bool CClientRegion::AddGoods(CGoods *pGoods,DWORD dwPos,LONG lExtendID)
{
	AddObject(pGoods);
	pGoods->SetDropEffect();
	float x= (float)(dwPos%GetWidth()),y=(float)(dwPos/GetWidth()); 
	pGoods->SetPosXY(x+0.5f,y+0.5f);

	pGoods->GetAnimInfo()->GetActionInfo()->dwCurAction = MAKEFOURCC('g','r','n','d');

	if( pGoods->GetProperty()->dwType==GBT_MONEY )
	{
        pGoods->SetName(AppFrame::GetText("Goods_208"));    //���
		long lGraphicsID = CGoodsList::GetMoneyGraphics(pGoods->GetNum());
		pGoods->SetGraphicsID(lGraphicsID);
		GetGame()->GetAudioList()->Play3DSound("SOUNDS\\equipments\\00018.wav",1,
			(float)y,
			pGoods->GetHeight(),
			(float)x); 
	}else if (pGoods->GetProperty()->dwType==GBT_SILVERMONEY)
	{
		pGoods->SetName(AppFrame::GetText("Goods_209"));    //����
		long lGraphicsID = CGoodsList::GetMoneyGraphics(pGoods->GetNum(),GBT_SILVERMONEY);
		pGoods->SetGraphicsID(lGraphicsID);
		GetGame()->GetAudioList()->Play3DSound("SOUNDS\\equipments\\00018.wav",
			1,
			(float)y,
			pGoods->GetHeight(),
			(float)x); 
	}
    else
	{
		GetGame()->GetAudioList()->PlayEquipmentSound(pGoods->GetIndex(),true,
			(float)y,
			pGoods->GetHeight(),
			(float)x);
	}
	pGoods->SetDisplayModel(false);

	if (pGoods->GetDisplayModel())
	{
		pGoods->GetDisplayModel()->ShowAllGroupbyLevel(  pGoods->GetEffectVal(GAP_WEAPON_LEVEL,1),false);
	}

	//��ʼ��������Ч
	pGoods->IniFlashEffect();
	return true;
}

bool CClientRegion::ClearGoods()
{
	//MessageBox(g_hWnd,"��������е�������Ʒ!",0,0);
	return false;
}

CFindPath* CClientRegion::GetFindPath()
{
	if (m_pFindPath == NULL)
		m_pFindPath = new CFindPath(this);

	return m_pFindPath;
}



/*
* ����: ���������ַ���
* ժҪ: ���ݴ������������ַ���,��Ͻ������򷵻��µĴ����������ַ���
*		 ��ʽ���������ַ���Ϊ: &name,�����ִ�Сд
* ����: pszOriNameStr - ԭʼ�����ַ���
* ����ֵ: ������������ַ��� 
* ����: lpf
* ��������: 2008.12.19
* �޸���־:
*/
const char * CClientRegion::ResolveNameStr(const char * pszOriNameStr)
{
	static string sName;

	sName = pszOriNameStr;

	if (pszOriNameStr[0] != '$')
		return sName.c_str();

	sName.erase(0, 1);

	if (sName == "space")
	{
		sName = "";
		return sName.c_str();
	}

	return sName.c_str();
}

/*
*	���ܣ����һ���ݵ���Ϣ
*	ϸ�������� ����ҽ��볡����ʱ�򣬽ӵ����������͵ĸó����ľݵ���Ϣ�����
*	������ stSPInfo - �ݵ���Ϣ
*	����ֵ�� ��
*	����: ����
*	��������: 09.09.29
*	�޸���־: -
*/
void	CClientRegion::AddSPointInfo(CClientRegion::tagSPointInfo stSPInfo)
{
	m_mapSPoint[stSPInfo.strPointName] = stSPInfo;
}

/*
*	���ܣ��ı�һ���ݵ��״̬
*	ϸ�������� ��ĳ���ݵ��״̬�����仯֮�󣬷�����֪ͨ�ͻ��˸ı�
*	������ szPointName - �ݵ�����
*			bState		- �ݵ�״̬
*	����ֵ�� ��
*	����: ����
*	��������: 09.09.29
*	�޸���־: -
*/
void	CClientRegion::ChangeSPointState(const char* szPointName, BYTE bState)
{
	if ( szPointName )
	{
		CClientRegion::tagSPointInfo stSPInfo = m_mapSPoint[szPointName];
		stSPInfo.bState = bState;
		m_mapSPoint[szPointName] = stSPInfo;
	}
}

/*
*	���ܣ��õ�ĳ���ݵ������
*	ϸ�������� ���ݴ���ľݵ����֣��õ���ǰ��ҿ����ĸþݵ�����
*	������ szPointName - �ݵ�����
*	����ֵ�� �ݵ�����
*	����: ����
*	��������: 09.09.29
*	�޸���־: -
*/
void	CClientRegion::GetDescTxtBySPState(string& strDesc, const char* szPointName)
{
	strDesc = "";
	if ( szPointName )
	{
		CPlayer * pPlayer = GetGame()->GetMainPlayer();
		// ������Ϣ
		CCountryList::tagCountryList* pSelf = CCountryList::GetCountry( pPlayer->GetCountry() );
		// ͬһ��Ӫ
		if( pSelf )
		{
			CClientRegion::tagSPointInfo stSPInfo = m_mapSPoint[szPointName];
			switch(stSPInfo.bState)
			{
			case 0:
				{
                    strDesc = AppFrame::GetText("Map_10");
				}
				break;
			case 1:
				{
					if( pSelf->nLeague == 1 )	// ����
						strDesc = AppFrame::GetText("Map_14");
					else
						strDesc = AppFrame::GetText("Map_12");
				}
				break;
			case 2:
				{
					if( pSelf->nLeague == 1 )	// ����
						strDesc = AppFrame::GetText("Map_13");
					else
						strDesc = AppFrame::GetText("Map_11");
				}
				break;
			case 3:
				{
					if( pSelf->nLeague == 1 )	// ����
						strDesc = AppFrame::GetText("Map_12");
					else
						strDesc = AppFrame::GetText("Map_14");
				}
				break;
			case 4:
				{
					if( pSelf->nLeague == 1 )	// ����
						strDesc = AppFrame::GetText("Map_11");
					else
						strDesc = AppFrame::GetText("Map_13");
				}
				break;
			}
		}
	}
}
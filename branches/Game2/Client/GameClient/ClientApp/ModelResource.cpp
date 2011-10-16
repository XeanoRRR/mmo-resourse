#include "StdAfx.h"

#include "ModelResource.h"
#include "../../Engine/Utility.h"
#include "../../EngineExtend/GameModel.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CModelResource::CModelResource(void)
{
}

CModelResource::~CModelResource(void)
{
}

GameModel * CModelResource::Get(long Tpye,WORD wModelID)
{
	GameModel * pGameModel = FindModel(Tpye,wModelID);
	if(!pGameModel)
	{
		pGameModel = ReadModel(Tpye,wModelID);
	}
	return pGameModel;
}


void CModelResource::Create()
{
}

void CModelResource::Destroy()
{
	ReleaseAll();
}

// ��ȡ
GameModel * CModelResource::FindModel(long Type,WORD wModelID)
{
	// �����Ƿ��и���Դ
	ModelKey key;
	key.Type = Type;
	key.GraphicsID = wModelID;
	itModel it = m_mapGameModel.find(key);
	if (it != m_mapGameModel.end())
		return (*it).second;

	return NULL;
}

GameModel * CModelResource::ReadModel(long Type,WORD wModelID)
{
	// ���ļ���ȡͼ������
	char szName[_MAX_PATH];
	if (Type == TYPE_WING)
	{
		_snprintf(szName,_MAX_PATH,"model/wing/%04d/model",wModelID);
	}
	else if (Type == TYPE_RIDE)
	{
		_snprintf(szName,_MAX_PATH,"model/ride/%04d/model",wModelID);
	}
	else if (Type == TYPE_WEAPON)
	{
		_snprintf(szName,_MAX_PATH,"model/weapon/%04d/model",wModelID);
	}
	else
	{
		return NULL;
	}

	if (!utility::File::FileExist(szName))
	{
		OutputError("ģ���ļ������� \"%s\"\n",szName);
		MessageBox(NULL,szName,"����:ģ���ļ�������",MB_OK);
		return NULL;
	}

	GameModel *pModel = new GameModel;
	if (!pModel->Create(szName))
	{
		OutputError("����: %s����ȷ.\n",szName);
		MessageBox(NULL,szName,"ģ���ļ�������",MB_OK);
		return NULL;
	}
	pModel->SetGroupVisible(TRUE);
	pModel->SetGroupVisible(TRUE);
	pModel->SelectDefaultPart();
	pModel->EnableParticleEmitter(TRUE);
	pModel->EnableTrailEmitter(TRUE);

	ModelKey key;
	key.Type = Type;
	key.GraphicsID = wModelID;	
	m_mapGameModel[key] = pModel; 

	return pModel;
}

// ��ȡ
void CModelResource::Release(long Type,WORD wModelID)
{
	ModelKey key;
	key.Type = Type;
	key.GraphicsID = wModelID;
	itModel it = m_mapGameModel.find(key);
	if (it != m_mapGameModel.end())
	{
		SAFEDESTROY(it->second);
		m_mapGameModel.erase(it);
	}
}

void CModelResource::ReleaseAll()
{
	itModel it = m_mapGameModel.begin();
	while(it != m_mapGameModel.end())
	{
		SAFEDESTROY(it->second);
		it++;
	}
	m_mapGameModel.clear();
}

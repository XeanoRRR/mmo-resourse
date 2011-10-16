#include "stdafx.h"
#include "../GameClient/Game.h"
#include "RegionList.h"
#include <sstream>
#include "../../../Public/Common/CStrConverter.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

list<CRegionList::tagWorldMap>			CRegionList::m_lsWorldMap;
list<CRegionList::tagRegionPathInfo>	CRegionList::m_lsRegionPathList;
long	CRegionList::m_lSearchMode		= -1;
long	CRegionList::m_lSouAreaID		= 0;
CRegionList::CRegionList(void)
{
}

CRegionList::~CRegionList(void)
{
	m_lsWorldMap.clear();
	m_lsRegionPathList.clear();
}

// ���ָ����ŵĳ�����Ϣ
CRegionList::tagRegion* CRegionList::GetRegion(long lRegionID)
{
	list<tagWorldMap>::iterator it = m_lsWorldMap.begin();
	for(; it != m_lsWorldMap.end(); ++it)
	{
		// ��ǰ���ͼ��ֱ�������б��в���
		if(it->m_mpRegions.find(lRegionID)!=it->m_mpRegions.end())
			return &(it->m_mpRegions[lRegionID]);
		// ���ͼ�ĸ��������µĳ����б����
		for(size_t i=0; i<it->vecArea.size(); ++i)
		{
			tagArea stArea = (it->vecArea)[i];
			if(stArea.m_mpRegions.find(lRegionID)!=stArea.m_mpRegions.end())
				return &(((it->vecArea)[i]).m_mpRegions[lRegionID]);
		}
	}
	return NULL;
}

// ���ָ����ŵ�������Ϣ
CRegionList::tagArea * CRegionList::GetArea(long lAreaID)
{
	list<tagWorldMap>::iterator it = m_lsWorldMap.begin();
	for(; it != m_lsWorldMap.end(); ++it)
	{
		// ���ͼ�ĸ��������µĳ����б����
		for(size_t i=0; i<it->vecArea.size(); ++i)
		{
			if(it->vecArea[i].lID == lAreaID)
				return &(it->vecArea[i]);
		}
	}
	return NULL;
}

// ���ָ����ŵ������ͼ
CRegionList::tagWorldMap * CRegionList::GetWorldMap(long lWorldMapID)
{
	list<tagWorldMap>::iterator it = m_lsWorldMap.begin();
	for(; it != m_lsWorldMap.end(); ++it)
	{
		if(it->lID == lWorldMapID)
			return &(*it);
	}
	return NULL;
}

// ��ʼ��������Ϣ
BOOL CRegionList::InitWorldInfo(tagWorldMap *stWorldMap,TiXmlElement* pElem)
{
	if(!pElem||!stWorldMap)
		return FALSE;
	// �����ڵ�,�����õ�ͼ�µ�������Ϣ��ֱ��������Ϣ
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL;
		pChildElem=pChildElem->NextSiblingElement())
	{
		// ������Ϣ
		if(!strcmp(pChildElem->Value(),"AreaMap")
			&& NULL!=pChildElem->Attribute("name")
			&& NULL!=pChildElem->Attribute("id")
			&& NULL!=pChildElem->Attribute("x1")
			&& NULL!=pChildElem->Attribute("y1")
			&& NULL!=pChildElem->Attribute("x2")
			&& NULL!=pChildElem->Attribute("y2"))
		{
			tagArea stArea;
			stArea.lBorderID = 0;
			stArea.lParentWorldID = stWorldMap->lID;
			stArea.strName		= pChildElem->Attribute("name");
			stArea.lID			= atol(pChildElem->Attribute("id"));
			stArea.rect.left	= atol(pChildElem->Attribute("x1"));
			stArea.rect.top		= atol(pChildElem->Attribute("y1"));
			stArea.rect.right	= atol(pChildElem->Attribute("x2"));
			stArea.rect.bottom	= atol(pChildElem->Attribute("y2"));
			if(pChildElem->Attribute("border"))
				stArea.lBorderID = atol(pChildElem->Attribute("border"));
			if(InitAreaInfo(&stArea,pChildElem))
				stWorldMap->vecArea.push_back(stArea);
		}
		// ֱ��������Ϣ
		else if(!strcmp(pChildElem->Value(),"RegionMap")
			&& NULL!=pChildElem->Attribute("name")
			&& NULL!=pChildElem->Attribute("id")
			&& NULL!=pChildElem->Attribute("x1")
			&& NULL!=pChildElem->Attribute("y1")
			&& NULL!=pChildElem->Attribute("x2")
			&& NULL!=pChildElem->Attribute("y2"))
		{
			tagRegion stRegion;
			stRegion.lParentAreaID = 0;
			stRegion.lParentWorldID = stWorldMap->lID;
			stRegion.strName	 = pChildElem->Attribute("name");
			stRegion.lID		 = atol(pChildElem->Attribute("id"));
			stRegion.rect.left	 = atol(pChildElem->Attribute("x1"));
			stRegion.rect.top	 = atol(pChildElem->Attribute("y1"));
			stRegion.rect.right	 = atol(pChildElem->Attribute("x2"));
			stRegion.rect.bottom = atol(pChildElem->Attribute("y2"));
			// ���壺������ЧͼƬ ID�����û������Ϊ0������� lParentAreaID
			stRegion.lIconID	= pChildElem->Attribute("MapIcon") ? atol( pChildElem->Attribute("MapIcon") ) : stRegion.lParentAreaID;

			// ֱ������
			if(InitRegionInfo(&stRegion,WORLDMAP_TYPE,stWorldMap,pChildElem))
				stWorldMap->m_mpRegions[stRegion.lID] = stRegion;
		}
	}
	return TRUE;
}

// ��ʼ�����ͼ�µ�������Ϣ
BOOL CRegionList::InitAreaInfo(tagArea *stArea,TiXmlElement* pElem)
{
	if(!pElem||!stArea)
		return FALSE;
	// �����ڵ�,�����������µĳ�����Ϣ
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL;
		pChildElem=pChildElem->NextSiblingElement())
	{
		// ������Ϣ
		if(!strcmp(pChildElem->Value(),"RegionMap")
			&& NULL!=pChildElem->Attribute("name")
			&& NULL!=pChildElem->Attribute("id")
			&& NULL!=pChildElem->Attribute("x1")
			&& NULL!=pChildElem->Attribute("y1")
			&& NULL!=pChildElem->Attribute("x2")
			&& NULL!=pChildElem->Attribute("y2"))
		{
			tagRegion stRegion;
			stRegion.lParentAreaID = stArea->lID;
			stRegion.lParentWorldID = stArea->lParentWorldID;
			stRegion.strName	 = pChildElem->Attribute("name");
			stRegion.lID		 = atol(pChildElem->Attribute("id"));
			stRegion.rect.left	 = atol(pChildElem->Attribute("x1"));
			stRegion.rect.top	 = atol(pChildElem->Attribute("y1"));
			stRegion.rect.right	 = atol(pChildElem->Attribute("x2"));
			stRegion.rect.bottom = atol(pChildElem->Attribute("y2"));
			// ���壺������ЧͼƬ ID�����û������Ϊ0������� lParentAreaID
			stRegion.lIconID	= pChildElem->Attribute("MapIcon") ? atol( pChildElem->Attribute("MapIcon") ) : stRegion.lParentAreaID;

			if(InitRegionInfo(&stRegion,AREAMAP_TYPE,stArea,pChildElem))
				stArea->m_mpRegions[stRegion.lID] = stRegion;
		}
	}
	return TRUE;
}

// ��ʼ��������Ϣ
BOOL CRegionList::InitRegionInfo(tagRegion *stRegion,MAP_TYPE lUpType,void *pUp, TiXmlElement* pElem)
{
	if(!pElem||!stRegion)
		return FALSE;
	// �����ڵ�,�����ó����µĵص���Ϣ
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL;
		pChildElem=pChildElem->NextSiblingElement())
	{
		// �ӳ�����Ϣ
		if(!strcmp(pChildElem->Value(),"RegionMap")
			&& NULL!=pChildElem->Attribute("name")
			&& NULL!=pChildElem->Attribute("id")
			&& NULL!=pChildElem->Attribute("x")
			&& NULL!=pChildElem->Attribute("y"))
		{
			tagRegion stChildRegion;
			stChildRegion.lParentRegionID	= stRegion->lID;
			stChildRegion.lParentAreaID		= stRegion->lParentAreaID;
			stChildRegion.lParentWorldID	= stRegion->lParentWorldID;
			stChildRegion.rect				= stRegion->rect;	
			stChildRegion.strName	 = pChildElem->Attribute("name");
			stChildRegion.lID		 = atol(pChildElem->Attribute("id"));
			stChildRegion.stPos.x	 = atol(pChildElem->Attribute("x"));
			stChildRegion.stPos.y	 = atol(pChildElem->Attribute("y"));
			// ���壺������ЧͼƬ ID�����û������Ϊ0������� lParentAreaID
			stChildRegion.lIconID	= pChildElem->Attribute("MapIcon") ? atol( pChildElem->Attribute("MapIcon") ) : stRegion->lIconID;
			if(InitRegionInfo(&stChildRegion,lUpType,pUp,pChildElem))
			{
				switch(lUpType)
				{
				case REGIONMAP_TYPE:
				case AREAMAP_TYPE:
					{
						CRegionList::tagArea * pArea = (CRegionList::tagArea *)pUp;
						if(pArea)
							pArea->m_mpRegions[stChildRegion.lID] = stChildRegion;
					}
					break;
				case WORLDMAP_TYPE:
					{
						CRegionList::tagWorldMap * pWorldMap = (CRegionList::tagWorldMap *)pUp;
						if(pWorldMap)
							pWorldMap->m_mpRegions[stChildRegion.lID] = stChildRegion;
					}
					break;
				}
			}
		}
		// �����µĵص���Ϣ
		if(!strcmp(pChildElem->Value(),"Address")
			&& NULL!=pChildElem->Attribute("name")
			&& NULL!=pChildElem->Attribute("x")
			&& NULL!=pChildElem->Attribute("y"))
		{
			tagAddressDetails tagAddrInfo;
			tagAddrInfo.strAddName  = pChildElem->Attribute("name");
			tagAddrInfo.stPos.x		= atol(pChildElem->Attribute("x"));
			tagAddrInfo.stPos.y		= atol(pChildElem->Attribute("y"));
			// ���ӳ���ID
			if(NULL!=pChildElem->Attribute("id"))
				tagAddrInfo.AddRegionId = atol(pChildElem->Attribute("id"));
			// ������ɫ
			if(NULL!=pChildElem->Attribute("color"))
				tagAddrInfo.color = CStrConverter::parseStringToDec(pChildElem->Attribute("color"));
			// ���������ɫ
			if(NULL!=pChildElem->Attribute("colorEmb"))
				tagAddrInfo.colorEmb = CStrConverter::parseStringToDec(pChildElem->Attribute("colorEmb"));
			stRegion->vecCurAddrDetails.push_back(tagAddrInfo);
		}
	}
	// �ó����µ�·���
	if(stRegion->vec_stRect.empty())
		InitRegionRectInfo(stRegion);
	return TRUE;
}


// ��ʼ�������µĽ���������Ϣ
BOOL CRegionList::InitRegionRectInfo(tagRegion *stRegion)
{
	if(!stRegion)
		return FALSE;
	//=================================================================//
	char str[256]="";
	sprintf_s(str,"data/RoadPoint/%d.xml",stRegion->lID);
	CRFile *prfile = rfOpen(str);
	if(NULL == prfile)
	{
		return FALSE;
	}
	TiXmlNode *pNode = NULL;
	TiXmlDocument m_Tdoc(str);
	//���뼼�������ļ�
	if( !m_Tdoc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
	{
		//���������Ϣ
		MessageBox(NULL,str,"",MB_OK);
		rfClose(prfile);
		return FALSE;
	}
	rfClose(prfile);
	//=================================================================//
	//�ҵ����ڵ㣬����޽ڵ㣬���������ʾ
	pNode = m_Tdoc.FirstChild("RoadPoint");
	if (pNode==NULL)
	{
		MessageBox(NULL,"No:'RoadPoint'Node","",MB_OK); 
		return FALSE;
	}
	TiXmlElement* pElem = pNode->ToElement();
	// �����ӽڵ㣬���õ�ͼ����������Ϣ
	for (TiXmlElement *pRegionElem=pElem->FirstChildElement(); pRegionElem!=NULL; 
		pRegionElem=pRegionElem->NextSiblingElement())
	{
		if(!strcmp(pRegionElem->Value(),"Region")
			&& NULL!=pRegionElem->Attribute("id"))
		{
			// ����IDƥ��
			long lID = atol(pRegionElem->Attribute("id"));
			if(lID == stRegion->lID)
			{
				stRegion->vec_stRect.clear();
				// �����ڵ�,�����������µĽ���������Ϣ
				for (TiXmlElement *pChildElem=pRegionElem->FirstChildElement(); pChildElem!=NULL;
					pChildElem=pChildElem->NextSiblingElement())
				{
					// �ó����µĽ�������
					if(!strcmp(pChildElem->Value(),"SignPos")
						&& NULL!=pChildElem->Attribute("x1")
						&& NULL!=pChildElem->Attribute("y1")
						&& NULL!=pChildElem->Attribute("x2")
						&& NULL!=pChildElem->Attribute("y2"))
					{
						RECT stRect;
						stRect.left		= atol(pChildElem->Attribute("x1"));
						stRect.top		= atol(pChildElem->Attribute("y1"));
						stRect.right	= atol(pChildElem->Attribute("x2"));
						stRect.bottom	= atol(pChildElem->Attribute("y2"));
						stRegion->vec_stRect.push_back(stRect);
					}
				}
			}
		}
	}
	return TRUE;
}

// ������ͼ�����ļ�
BOOL CRegionList::LoadMap()
{
	if(m_lsWorldMap.size() == 0)
	{
		//=================================================================//
		CRFile *prfile = rfOpen("data/GameMapConfig.xml");
		if(NULL == prfile)
		{
			//���������Ϣ
			MessageBox(NULL,"LoadFileErr:data/GameMapConfig.xml","",MB_OK);
			return FALSE;
		}
		TiXmlNode *pNode = NULL; 		
		TiXmlDocument m_Tdoc("data/GameMapConfig.xml");
		//���뼼�������ļ�
		if( !m_Tdoc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
		{
			//���������Ϣ
			MessageBox(NULL,"LoadFileErr:data/GameMapConfig.xml","",MB_OK);
			rfClose(prfile);
			return FALSE;
		}
		rfClose(prfile);
		//=================================================================//
		//�ҵ����ڵ㣬����޽ڵ㣬���������ʾ
		pNode = m_Tdoc.FirstChild("Map");
		if (pNode==NULL)
		{
			MessageBox(NULL,"No:'Map'Node","",MB_OK); 
			return FALSE;
		}
		TiXmlElement* pElem = pNode->ToElement();
		// �����ӽڵ㣬���õ�ͼ��Ϣ
		for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
			pChildElem=pChildElem->NextSiblingElement())
		{
			// ������ͼ��Ϣ
			if(!strcmp(pChildElem->Value(),"WorldMap")
				&& NULL!=pChildElem->Attribute("name")
				&& NULL!=pChildElem->Attribute("id"))
			{
				tagWorldMap stWorldMap;
				stWorldMap.lID = atol(pChildElem->Attribute("id"));
				stWorldMap.strName = pChildElem->Attribute("name");
				if(InitWorldInfo(&stWorldMap,pChildElem))
					m_lsWorldMap.push_back(stWorldMap);
			}
		}
		return TRUE;
	}
	return FALSE;
}

// ����ָ��������Ŀ�곡���ĳ���·��
BOOL CRegionList::FindRegionPath(long lSourID,long lTargetID)
{
	m_lsRegionPathList.clear();
	m_lSouAreaID = 0;
	// �����ͬһ������������
	if(lSourID == lTargetID)
		return FALSE;
	// ԭʼ�����ڵ���Ϣ
	tagRegionPathInfo stSInfo;
	stSInfo.lRegionID = lSourID;
	// �ҵ�ָ������
	tagRegion * pSourRegion = GetRegion(lSourID);
	tagRegion * pTargRegion = GetRegion(lTargetID);
	if(pSourRegion && pTargRegion)
	{
		if(g_bDebug)
		{
			Log4c::Trace(ROOT_MODULE,"�糡����ʼ����%s->%s",pSourRegion->strName.c_str(),pTargRegion->strName.c_str());
		}
		// ������ͬ
		if(pSourRegion->lParentAreaID == pTargRegion->lParentAreaID)
		{
			// ����ID��Ϊ0����ʾ�ڱ�������
			if(pSourRegion->lParentAreaID !=0)
				m_lSearchMode = REGION_REGION;
			// ����IDΪ0����ʾ�������ͼ��ֱ�������в���
			else
				m_lSearchMode = WREGION_WREGION;
		}
		// ����ͬ
		else
		{
			// ����ID����Ϊ0�������������Ҫȷ���������ҵĳ��ں���ڳ�����
			if(pSourRegion->lParentAreaID != 0 && pTargRegion->lParentAreaID != 0)
				m_lSearchMode = AREA_AREA;
			// Դ����������ID��Ϊ0��Ŀ�곡������IDΪ0����ʾ��һ�����ҵ�����ֱ�������Ĳ��ң���Ҫȷ��Դ�����ĳ��ڳ�����
			else if(pSourRegion->lParentAreaID != 0 && pTargRegion->lParentAreaID == 0)
				m_lSearchMode = AREA_WREGION;
			// Դ����������IDΪ0��Ŀ�곡������ID��Ϊ0����ʾ���糡����ĳ�����ҵĲ��ң���Ҫȷ��Ŀ�곡������ڳ�����
			else if(pSourRegion->lParentAreaID == 0 && pTargRegion->lParentAreaID != 0)
				m_lSearchMode = WREGION_AREA;
			else
				return FALSE;
		}
		// ��¼��ʼ����
		m_lSouAreaID = pSourRegion->lParentAreaID;
		for (size_t i = 0; i < pSourRegion->vecCurAddrDetails.size(); ++i)
		{
			tagRegionPathInfo stPathInfo;
			stPathInfo.lRegionID = pSourRegion->vecCurAddrDetails[i].AddRegionId;
			stPathInfo.stPos = pSourRegion->vecCurAddrDetails[i].stPos;
			if (stPathInfo.lRegionID == 0)
				continue;
			if(stPathInfo.lRegionID == lTargetID)
			{
				m_lsRegionPathList.clear();
				// ɾ��ԭʼ�����ڵ�
				m_lsRegionPathList.push_back(stPathInfo);
				return TRUE;
			}
			list<CRegionList::tagRegionPathInfo> RegionPathList;
			RegionPathList.push_back(stSInfo);
			if(AddRegionPath(RegionPathList,stPathInfo,pTargRegion))
			{
				RegionPathList.erase(RegionPathList.begin());
				if(m_lsRegionPathList.empty())
					m_lsRegionPathList = RegionPathList;
				else if(m_lsRegionPathList.size() > RegionPathList.size())
					m_lsRegionPathList = RegionPathList;
			}
			RegionPathList.clear();
		}
		// �ҵ�����·��
		if(!m_lsRegionPathList.empty())
		{
			if(m_lsRegionPathList.front().lRegionID == lSourID)
				m_lsRegionPathList.pop_front();
			if(g_bDebug)
			{
				list<tagRegionPathInfo>::iterator it = m_lsRegionPathList.begin();
				for (it; it != m_lsRegionPathList.end(); ++it)
				{
					Log4c::Trace(ROOT_MODULE,"��ӳ���ID��%d",(*it).lRegionID);
				}
				Log4c::Trace(ROOT_MODULE,"*****************");
			}
			return TRUE;
		}
	}
	return FALSE;
}

// ��ӳ���·��
BOOL CRegionList::AddRegionPath(list<CRegionList::tagRegionPathInfo> & lsRegionPath,tagRegionPathInfo stSourInfo,tagRegion * pTarget)
{
	if(!pTarget)
		return FALSE;
	static map<long,CRegionList::tagRegionPathInfo> SearchedPathList;		// �Ѿ����ҹ���·��
	if(lsRegionPath.size() < 2)
	{
		SearchedPathList.clear();
		SearchedPathList[lsRegionPath.front().lRegionID] = lsRegionPath.front();
	}
	// �������ʼ�����Ѿ������ҹ��ˣ���֮ǰ������С�ڵ�ǰ�ڵ�����������
	map<long,CRegionList::tagRegionPathInfo>::iterator ite = SearchedPathList.find(stSourInfo.lRegionID);
	if(ite != SearchedPathList.end())
	{
		if(ite->second.lIndex <= stSourInfo.lIndex)
			return FALSE;
	}
	stSourInfo.lIndex = (long)lsRegionPath.size();
	SearchedPathList[stSourInfo.lRegionID] = stSourInfo;
	lsRegionPath.push_back(stSourInfo);
	if(g_bDebug)
	{
		Log4c::Trace(ROOT_MODULE,"��ӳ���ID��%d",stSourInfo.lRegionID);
	}
	if(!m_lsRegionPathList.empty() && lsRegionPath.size() >= m_lsRegionPathList.size())
		return FALSE;
	// �����ʼ���յ㳡��
	tagRegion * pSourRegion = GetRegion(stSourInfo.lRegionID);
	if(pSourRegion)
	{
		// ����ʼ���������ڳ���ȥ�����Ƿ����Ŀ�곡�������������
		for (size_t i = 0; i < pSourRegion->vecCurAddrDetails.size(); ++i)
		{
			tagRegionPathInfo stPathInfo;
			stPathInfo.lRegionID = pSourRegion->vecCurAddrDetails[i].AddRegionId;
			stPathInfo.stPos = pSourRegion->vecCurAddrDetails[i].stPos;
			stPathInfo.lIndex = (long)lsRegionPath.size();
			if (stPathInfo.lRegionID == 0)
			{
				continue;
			}
			if(stPathInfo.lRegionID == pTarget->lID)
			{
				if(g_bDebug)
				{
					Log4c::Trace(ROOT_MODULE,"**************�ҵ�Ŀ�곡��ID��%d,�м䳡����Ϊ��%d**************",stPathInfo.lRegionID,lsRegionPath.size());
				}
				// ����ýڵ��Ѵ��ڣ�ֱ�ӷ���
				list<CRegionList::tagRegionPathInfo>::iterator it = lsRegionPath.begin();
				for (; it != lsRegionPath.end(); ++it)
				{
					if(it->lRegionID == pTarget->lID)
						return TRUE;
				}
				lsRegionPath.push_back(stPathInfo);
				return TRUE;
			}
			switch(m_lSearchMode)
			{
				// �������ң������ǰ�����ĳ������Ǳ����ĳ����ˣ�����
			case REGION_REGION:
				{
					if(pSourRegion->lParentAreaID != pTarget->lParentAreaID)
						return FALSE;
				}
				break;
				// ������ң������ǰ�����ĳ�������������ʼ��Ŀ�곡����Ҳ����������ֱ������������
			case AREA_AREA:
				{
					if(pSourRegion->lParentAreaID != 0 &&
						pSourRegion->lParentAreaID != m_lSouAreaID && 
						pSourRegion->lParentAreaID != pTarget->lParentAreaID)
						return FALSE;
				}
				break;
				// ��->���糡���������ǰ�����ĳ���������������ֱ��������Ҳ��������ʼ���ҳ���������
			case AREA_WREGION:
				{
					if(pSourRegion->lParentAreaID != 0 && pSourRegion->lParentAreaID != m_lSouAreaID)
						return FALSE;
				}
				break;
				// ���糡��->���������ǰ�����ĳ���������������ֱ��������Ҳ������Ŀ����ҳ���������
			case WREGION_AREA:
				{
					if(pSourRegion->lParentAreaID != 0 && pSourRegion->lParentAreaID != pTarget->lParentAreaID)
						return FALSE;
				}
				break;
				// ���糡��->���糡���������ǰ��������Ѿ�����ĳ�����ң�ֱ�ӷ���
			case WREGION_WREGION:
				{
					if(pSourRegion->lParentAreaID != 0)
						return FALSE;
				}
				break;
			}
			// ���򣬽������ڳ�����Ϊ��ʼ�������еݹ�����
			list<tagRegionPathInfo>	RegionPathList = lsRegionPath;
			if(AddRegionPath(RegionPathList,stPathInfo,pTarget))
			{
				// �ҵ�������·�������ݵ����ڵ��������
				lsRegionPath = RegionPathList;
				if(m_lsRegionPathList.empty())
					m_lsRegionPathList = lsRegionPath;
				else if(m_lsRegionPathList.size() > lsRegionPath.size())
					m_lsRegionPathList = lsRegionPath;
				lsRegionPath.pop_back();
				//return TRUE;
			}
			RegionPathList.clear();
		}
	}
	return FALSE;
}


/******************************************************************************
*  file: CMailBoxList.h
*
*  Brief:�����е���Ͳ���ã���ʾ�˸��������е���Ͳλ��
*
*  Authtor:����
*	
*  Datae:2009-3-7
******************************************************************************/

map<long , CMailBoxPosList::tagMailBoxPos>	CMailBoxPosList::m_mapMailBoxPos;

CMailBoxPosList::CMailBoxPosList(void)
{
}

CMailBoxPosList::~CMailBoxPosList(void)
{
	ReleaseMailBoxPos();
}


// �ͷ���Ͳ���������ļ�
void CMailBoxPosList::ReleaseMailBoxPos()
{
	m_mapMailBoxPos.clear();
}

// ������ͼ�����ļ�
BOOL CMailBoxPosList::LoadMailBoxPos()
{
	m_mapMailBoxPos.clear();
	//=================================================================//
	CRFile *prfile = rfOpen("data/MailBoxPos.xml");
	if(NULL == prfile)
	{
		//���������Ϣ
		MessageBox(NULL,"LoadFileErr:data/MailBoxPos.xml","",MB_OK);
		return FALSE;
	}
	TiXmlNode *pNode = NULL; 		
	TiXmlDocument m_Tdoc("data/MailBoxPos.xml");
	//���뼼�������ļ�
	if( !m_Tdoc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
	{
		//���������Ϣ
		MessageBox(NULL,"LoadFileErr:data/MailBoxPos.xml","",MB_OK);
		rfClose(prfile);
		return FALSE;
	}
	rfClose(prfile);
	//=================================================================//
	//�ҵ����ڵ㣬����޽ڵ㣬���������ʾ
	pNode = m_Tdoc.FirstChild("Mail");
	if (pNode==NULL)
	{
		MessageBox(NULL,"No:'Mail'Node","",MB_OK); 
		return FALSE;
	}
	TiXmlElement* pElem = pNode->ToElement();
	// �����ӽڵ㣬������Ϣ
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		// ������ͼ��Ϣ
		if(!strcmp(pChildElem->Value(),"MailBox")
			&& NULL!=pChildElem->Attribute("ResourceID")
			&& NULL!=pChildElem->Attribute("Name"))
		{
			tagMailBoxPos stMailBoxPos;
			stMailBoxPos.lResourceID = atol(pChildElem->Attribute("ResourceID"));
			stMailBoxPos.strName = pChildElem->Attribute("Name");
			if(InitMailPos(stMailBoxPos,pChildElem))
				m_mapMailBoxPos[stMailBoxPos.lResourceID] = stMailBoxPos;
		}
	}
	return TRUE;
}


// ��ʼ��������ͼ�µ���������
BOOL CMailBoxPosList::InitMailPos(tagMailBoxPos& stMailBoxPos,TiXmlElement* pElem)
{
	if(!pElem)
		return FALSE;
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL;
		pChildElem=pChildElem->NextSiblingElement())
	{
		// ������Ϣ
		if(!strcmp(pChildElem->Value(),"Pos")
			&& NULL!=pChildElem->Attribute("x")
			&& NULL!=pChildElem->Attribute("y"))
		{
			tagPos stPos;
			stPos.x = atol(pChildElem->Attribute("x"));
			stPos.y = atol(pChildElem->Attribute("y"));
			stMailBoxPos.vecPos.push_back(stPos);
		}
	}
	return TRUE;
}

// ���ָ�������У���ָ�������������������
// ����1��������ԴID������2������3��ָ��������
void CMailBoxPosList::GetMailBoxPos(long lResourceID,long sx,long sy,long& dx,long& dy)
{
	if(m_mapMailBoxPos.find(lResourceID) != m_mapMailBoxPos.end())
	{
		tagMailBoxPos stMailBoxPos = m_mapMailBoxPos[lResourceID];
		long lDist = 9999999;
		long index = -1;
		// �ҳ���������ĵ�
		for (size_t i = 0; i < stMailBoxPos.vecPos.size(); ++i)
		{
			long lDis = Distance(sx,sy,stMailBoxPos.vecPos[i].x,stMailBoxPos.vecPos[i].y);
			if(lDis < lDist)
			{
				lDist = lDis;
				index = (long)i;
			}
		}
		if(index >= 0 && index < (long)stMailBoxPos.vecPos.size())
		{
			dx = stMailBoxPos.vecPos[index].x;
			dy = stMailBoxPos.vecPos[index].y;
		}
	}
}

/******************************************************************************
*  file:	CMartialPos.h
*
*  Brief:	���¾ݵ����ã��ɸ��ݴ����ó�ʼ�����ͼ�еľ��¾ݵ����Ϣ�Լ���ʾ
*
*  Authtor:	����
*
*  Datae:	2009-4-22
******************************************************************************/

vector<CMartialPos::tagBeachHead>	CMartialPos::m_listMartialPos;

CMartialPos::CMartialPos(void)
{}

CMartialPos::~CMartialPos(void)
{
	ReleaseMartialPos();
}

// �������¾ݵ������ļ�
BOOL CMartialPos::LoadMartialPos()
{
	m_listMartialPos.clear();
	//=================================================================//
	CRFile *prfile = rfOpen("data/MartialBeachHead.xml");
	if(NULL == prfile)
	{
		//���������Ϣ
		MessageBox(NULL,"LoadFileErr:data/MartialBeachHead.xml","",MB_OK);
		return FALSE;
	}
	TiXmlNode *pNode = NULL; 		
	TiXmlDocument m_Tdoc("data/MartialBeachHead.xml");
	//���뼼�������ļ�
	if( !m_Tdoc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
	{
		//���������Ϣ
		MessageBox(NULL,"LoadFileErr:data/MartialBeachHead.xml","",MB_OK);
		rfClose(prfile);
		return FALSE;
	}
	rfClose(prfile);
	//=================================================================//
	//�ҵ����ڵ㣬����޽ڵ㣬���������ʾ
	pNode = m_Tdoc.FirstChild("Martial");
	if (pNode==NULL)
	{
		MessageBox(NULL,"No:'Martial'Node","",MB_OK); 
		return FALSE;
	}
	TiXmlElement* pElem = pNode->ToElement();
	// �����ӽڵ㣬������Ϣ
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		// ������ͼ��Ϣ
		if(!strcmp(pChildElem->Value(),"Area")
			&& NULL!=pChildElem->Attribute("id")
			&& NULL!=pChildElem->Attribute("name"))
		{
			tagBeachHead stBeachHead;
			stBeachHead.strAreaName = pChildElem->Attribute("Name");
			if(InitMartialPos(stBeachHead,pChildElem))
				m_listMartialPos.push_back(stBeachHead);
		}
	}
	return TRUE;
}

// �ͷž��¾ݵ������ļ�
void CMartialPos::ReleaseMartialPos()
{
	m_listMartialPos.clear();
}

// ��ʼ��������ͼ�µ���������
BOOL CMartialPos::InitMartialPos(CMartialPos::tagBeachHead& stBeachHead,TiXmlElement* pElem)
{
	if(!pElem)
		return FALSE;
	for (TiXmlElement *pChildElem = pElem->FirstChildElement(); pChildElem != NULL;
		pChildElem = pChildElem->NextSiblingElement())
	{
		// ������Ϣ
		if(!strcmp(pChildElem->Value(),"name")
			&& NULL!=pChildElem->Attribute("id")
			&& NULL!=pChildElem->Attribute("x")
			&& NULL!=pChildElem->Attribute("y")
			&& NULL!=pChildElem->Attribute("desc"))
		{
			stBeachHead.strBeachHeadName = pChildElem->Attribute("name");
			stBeachHead.lBeachHeadID	 = atol(pChildElem->Attribute("id"));
			stBeachHead.lPosX			 = atol(pChildElem->Attribute("x"));
			stBeachHead.lPosY			 = atol(pChildElem->Attribute("y"));
			stBeachHead.strDesc			 = pChildElem->Attribute("desc");
		}
	}
	return TRUE;
}

#include "StdAfx.h"
#include ".\proxyserverregion.h"

CProxyServerRegion::CProxyServerRegion(const CGUID& id)
:CServerRegion(id)
{

}
CProxyServerRegion::CProxyServerRegion(long id)
:CServerRegion(id)
{

}

CProxyServerRegion::~CProxyServerRegion(void)
{
}

// ����
bool CProxyServerRegion::DecordFromByteArray(BYTE* pByte, long& pos, bool bExData)
{
	CBaseObject::DecordFromByteArray(pByte, pos, bExData);
	m_btCountry = _GetByteFromByteArray(pByte, pos);
	eRWT RegionType = (eRWT)_GetLongFromByteArray(pByte, pos);
	SetWarType(RegionType);
	//˰�ղ���
	tagRegionParam	param;
	_GetBufferFromByteArray(pByte, pos, &param, sizeof(tagRegionParam));
	SetRegionParam(param);
	return true;
}

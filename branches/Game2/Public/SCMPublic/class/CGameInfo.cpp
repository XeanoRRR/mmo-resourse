

#include "StdAfx.h"
#include "CGameInfo.h"



//! ���뵽���ݼ�
BOOL CGameInfo::Code(DBWriteSet &setWriteDB) const
{
	setWriteDB.AddToByteArray(m_dwType);
	setWriteDB.AddToByteArray((LONG)m_vGameInfoData.size());

	for (LONG i = 0; i < m_vGameInfoData.size(); ++i)
	{
		setWriteDB.AddToByteArray((void*)&m_vGameInfoData[i], sizeof(tagGameInfoData));
	}

	return TRUE;
}

BOOL CGameInfo::Code(Json::Value& var) const
{
    var["infoType"] = (int)m_dwType;
    Json::Value subVar;
    for(LONG i=0; i<m_vGameInfoData.size(); ++i)
    {
        Json::Value tpVar;
        tpVar["time"] = (double)m_vGameInfoData[i]._tTime;
        tpVar["data1"] = (int)m_vGameInfoData[i]._dwData1;
        tpVar["data2"] = (int)m_vGameInfoData[i]._dwData2;
        subVar.append(tpVar);
    }
    var["infoData"] = subVar;
    return TRUE;
}
//! �����ݼ�����
BOOL CGameInfo::DeCode(DBReadSet &setReadDB)
{
	m_dwType = setReadDB.GetDwordFromByteArray();
	LONG lSize = setReadDB.GetLongFromByteArray();
	for (LONG i = 0; i < lSize; ++i)
	{
		tagGameInfoData GameInfoData;
		setReadDB.GetBufferFromByteArray(&GameInfoData, sizeof(tagGameInfoData));
	}

	return TRUE;
}
#include "StdAfx.h"
#include "ReliveGoodsList.h"
#include "../Common/Public.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/// ��̬��Ա��ʼ��
list<ReLiveGoodsList::tagReLiveGoods*>  ReLiveGoodsList::m_lReLiveGoods;

/// ����
ReLiveGoodsList::ReLiveGoodsList()
{

}

/// ����
ReLiveGoodsList::~ReLiveGoodsList()
{

}

/// ���ļ����ظ�����Ʒ�б�
bool ReLiveGoodsList::Init(const char* filename)
{
	if (NULL==filename)
	{
		return false;
	}

	CRFile *prfile = rfOpen(filename);
	if(NULL == prfile)
	{
		//���������Ϣ
		Log4c::Error(ROOT_MODULE, "%-15s LoadFileError:%s", __FUNCTION__,filename);

		return false;
	}
	TiXmlNode* pNode = NULL; 		
	TiXmlDocument m_Tdoc(filename);
	//���븴����Ʒ�����ļ�
	if( !m_Tdoc.LoadData(prfile->GetData(), prfile->GetDatalen()) )
	{
		//���������Ϣ
        Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("WS_SKILL_FOURTH", filename));
		rfClose(prfile);
		return false;
	}
	rfClose(prfile);

	//�ҵ����ڵ㣬����޽ڵ㣬���������ʾ
	pNode = m_Tdoc.FirstChild("ReLiveGoods");
	if (pNode == NULL)
	{
        Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("WS_SKILL_FIFTH"));
		return false;
	}
	TiXmlElement* pElem = pNode->ToElement();

	for (TiXmlElement* pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{

		if (!stricmp(pChildElem->Value(), "Goods"))
		{
			tagReLiveGoods *ptgReliveGoods = new tagReLiveGoods;
			ptgReliveGoods->strOrignalName = pChildElem->Attribute("OrignalName");
			ptgReliveGoods->dwIndex = (ulong)atol(pChildElem->Attribute("Index"));
			ptgReliveGoods->dwNum = (ulong)atol( pChildElem->Attribute("GoodsNum"));
			ptgReliveGoods->dwRecoverRatio = (ulong)atol(pChildElem->Attribute("RecoverRatio"));
			m_lReLiveGoods.push_back(ptgReliveGoods);		
		}
	}
	return true;
}

/// ����	
void ReLiveGoodsList::Release()
{
	list<tagReLiveGoods*>::iterator it = m_lReLiveGoods.begin();
	for (; it!=m_lReLiveGoods.end(); ++it)
	{
		SAFE_DELETE(*it);
	}
	m_lReLiveGoods.clear();
}

/// ����
bool ReLiveGoodsList::AddToByteArray(vector<uchar>& pByteArray)
{
	//������Ʒ����		
	_AddToByteArray(&pByteArray,static_cast<long>(m_lReLiveGoods.size()));

	list<tagReLiveGoods *>::iterator it = m_lReLiveGoods.begin();
	for (; it!=m_lReLiveGoods.end(); ++it)
	{
		// ԭʼ��
		_AddToByteArray(&pByteArray, (char*)((*it)->strOrignalName.c_str()));			
		//����
		_AddToByteArray(&pByteArray, (*it)->dwIndex);
		//����
		_AddToByteArray(&pByteArray, (*it)->dwNum);
		//����ֵ
		_AddToByteArray(&pByteArray, (*it)->dwRecoverRatio);	
	}
	return true;
}

/// ����	
 bool ReLiveGoodsList::DecordFromByteArray(uchar* pByte, long& pos)
{
	//������Ʒ����
	char strTemp[256];
	long lNum = _GetLongFromByteArray(pByte, pos);
	for(long l=0; l<lNum; ++l)
	{
		tagReLiveGoods *ptgReliveGoods  = new tagReLiveGoods;
		_GetStringFromByteArray(pByte, pos, strTemp);
		ptgReliveGoods->strOrignalName = strTemp;
		ptgReliveGoods->dwIndex = _GetDwordFromByteArray(pByte,pos);
		ptgReliveGoods->dwNum = _GetDwordFromByteArray(pByte,pos);
		ptgReliveGoods->dwRecoverRatio = _GetDwordFromByteArray(pByte,pos);
		m_lReLiveGoods.push_back(ptgReliveGoods);
	}
	return true;
}

#include "StdAfx.h"
#include "StrUnifyInput.h"
#include "Public.h"
#include "FunctionKeyMapSystem.h"

#ifndef USE_IN_SERVER
#include "../client/GameClient/GameClient/Game.h"
#include "../client/GameClient/Input/KeyBoard.h"
#include "../client/GameClient/Guiextend/CGuiEx.h"
#include "../client/GameClient/Guiextend/HotKeySetupPageEx.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//MAPFKEY CFuncKeyMapSystem::m_FuncKey;

#ifndef USE_IN_SERVER

MAPINFOTYPE CFuncKeyMapSystem::m_DefultFuncKeyInfo;


bool CFuncKeyMapSystem::LoadDefaultFuncMapKey(string strFileName)
{
	if (""==strFileName) return false;
	Release();
	char strError[256];
	//=================================================================//
	CRFile *prfile = rfOpen(strFileName.c_str());
	if(NULL == prfile)
	{
		//���������Ϣ
		_snprintf(strError, 256, "LoadFileError:%s",strFileName);
		MessageBox(NULL,strError,"",MB_OK); 
		return false;
	}
	TiXmlNode *pNode = NULL; 		
	TiXmlDocument m_Tdoc(strFileName.c_str());
	//���������ļ�
	if( !m_Tdoc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
	{
		//���������Ϣ
		_snprintf(strError, 256, "Data of %s error!",strFileName.c_str());
		MessageBox(NULL,strError,"",MB_OK); 
		rfClose(prfile);
		return false;
	}
	rfClose(prfile);
	//=================================================================//
	//�ҵ����ڵ㣬����޽ڵ㣬���������ʾ
	pNode = m_Tdoc.FirstChild("FMK");
	if (pNode==NULL)
	{
		string strNode = "FMK";
		_snprintf(strError, 256, "Load %s node of %s error!",strNode.c_str(),strFileName.c_str());
		MessageBox(NULL,strError,"",MB_OK); 
		return false;
	}
	TiXmlElement* pElem = pNode->ToElement();


	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{

		if (!stricmp(pChildElem->Value(),"KeyTypeIndex"))
		{			
			INFOTYPE tgTypeInfo;
			tgTypeInfo.typeId  = (WORD)atoi(pChildElem->Attribute("id"));
			DWORD typedescid = (DWORD)atoi(pChildElem->Attribute("typedesc"));
			tgTypeInfo.strTypeDesc = CStringReading::LoadString(eIDS_MAPKEY,typedescid);
			if (!LoadFuncKeyConfig(pChildElem,tgTypeInfo))
			{
				_snprintf(strError, 256, "the file %s has the same id,please check it",strFileName.c_str());
				MessageBox(NULL,strError,"",MB_OK); 
				return false;
			}
			m_DefultFuncKeyInfo.insert(pair<WORD,INFOTYPE>(tgTypeInfo.typeId,tgTypeInfo));
		}
	}

	return true;
}

bool CFuncKeyMapSystem::LoadFuncKeyConfig(TiXmlElement *pElem,INFOTYPE& typeinfo)
{
	for (TiXmlElement *pChildElem=pElem->FirstChildElement(); pChildElem!=NULL; 
		pChildElem=pChildElem->NextSiblingElement())
	{
		if (!stricmp(pChildElem->Value(),"FuncIndex"))
		{
			FKMS funcid = (FKMS)atol(pChildElem->Attribute("id"));
			MAPFKEYINFOITER iter = typeinfo.FuncKeyInfo.find(funcid);
			if (iter!=typeinfo.FuncKeyInfo.end()) return false;
			
			INFOFUNC keyinfo;

			keyinfo.key.funcIndex = funcid;											// ��������ֵ
			keyinfo.key.keyArray.mkey = (int)strtol(pChildElem->Attribute("mainkey"),NULL,16);		// ����
			keyinfo.key.keyArray.askey1 = (int)strtol(pChildElem->Attribute("asskey1"),NULL,16);	// ������1
			keyinfo.key.keyArray.askey2 = (int)strtol(pChildElem->Attribute("asskey2"),NULL,16);	// ������2
			//keyinfo.key.keyArray.mkey = GetIntAttribute(pChildElem,"mainkey");		
			//keyinfo.key.keyArray.askey1 = GetIntAttribute(pChildElem,"asskey1");	
			//keyinfo.key.keyArray.askey2 = GetIntAttribute(pChildElem,"asskey2");	
			keyinfo.strFuncDesc = CStringReading::LoadString(eIDS_MAPKEY,GetIntAttribute(pChildElem,"keydesc"));	// �������ܵ�������Ϣ
			keyinfo.key.modify = GetIntAttribute(pChildElem,"modify");				// �Ƿ���޸�

			typeinfo.FuncKeyInfo.insert(pair<FKMS,INFOFUNC>(funcid,keyinfo));
		}
	}
	return true;
}

//static void AddToByteArray(vector<BYTE> &pByteArray);
//static void DecordFromByteArray(BYTE *pByte, long& pos);

//��ȡ����
int	CFuncKeyMapSystem::GetIntAttribute(TiXmlElement *pElem,string strName)
{
	int iValue;
	if(pElem->QueryIntAttribute(strName.c_str(),&iValue))
	{
		iValue = 0;
	}
	return iValue;
}

// �ͷ�
void CFuncKeyMapSystem::Release()
{
	m_DefultFuncKeyInfo.clear();
}

// ��һ�������ֵת��Ϊ����Ӧ�Ŀ��ӻ����֣���������ϰ�ߵİ������ƣ�
string& CFuncKeyMapSystem::ConvertVirKeyToString(string& strName,const int virKey)
{
	if(virKey!=0)
		strName = CStringReading::LoadString(eIDS_KEYNAME,virKey);
	else
		strName = "";
	return strName;
}

// ���ȼ��������ֵת���ɿɼ����ַ���
string& CFuncKeyMapSystem::GetHotKeyName(string& keyname,const SETKEY& keys)
{
	//SHORT  mkey = 0,askey1 = 0,askey2 = 0;
	string mkeyname = "",askey1name = "",askey2name = "";
	ConvertVirKeyToString(mkeyname,keys.mkey); //mkey = MapVirtualKey(keys.mkey,2);
	ConvertVirKeyToString(askey1name,keys.askey1);//askey1 = MapVirtualKey(keys.askey1,2);
	ConvertVirKeyToString(askey2name,keys.askey2);//askey2 = MapVirtualKey(keys.askey2,2);

	if (mkeyname!="")
	{
		if (askey1name!="")
		{
			//keyname.push_back(askey1);
			keyname += askey1name;
			keyname += "+";
		}
		if (askey2name!="")
		{
			//keyname.push_back(askey2);
			keyname += askey2name;
			keyname += "+";
		}
		//keyname.push_back(mkey); 
		keyname += mkeyname;
	}else
		keyname = CStringReading::LoadString(eIDS_KEYNAME,20000);

	return keyname;
}

// ���ݲ���dwFuncIndexָ����ĳ�����ܼ��˹��ܵ��ȼ��Ƿ񱻴���,bContinuousָ���˹����Ƿ������������ס�����ŵ����
//bool CFuncKeyMapSystem::CheckFuncKey(MAPFKEY& mapkey,FKMS FuncIndex,SHORT appointkey,bool bContinuous)
//{	
//	BYTE  dif_key = GetGame()->GetKeyboard()->GetCurrentKey();
//
//	// ���ȼ����ý����ʱ����ʱ�������е��ȼ�����
//	if (GetGame()->GetCGuiEx()->GetHotKeySetupPageEx()->IsOpen()&&(appointkey!=VK_ESCAPE||dif_key!=DIK_ESCAPE)) return false;
//	
//	MAPFKEYITER iter = mapkey.find(FuncIndex);
//	if (iter!=mapkey.end())
//	{
//		SHORT mkey = 0,askey1 = 0,askey2 = 0;
//		int askey1_q = 0,askey2_q = 0;		// �����ݴ�Ҫ��ѯ�ĸ�����1��������2
//		if (iter->second.keyArray.mkey!=0)
//		{
//			// ��������ֵ��С������ġ��������Ļ��� �ͽ�ɨ����ǿ������Ϊ����������ɨ����
//			//����Ϊ���ĸ���û���Լ��������ֵ����������С���̵ķ�������������ֵ��MapVirtualKey����ת����������Ӧ��ɨ���룩
//			if (iter->second.keyArray.mkey==0x26)
//			{
//				mkey = 0xC8;
//			}else if (iter->second.keyArray.mkey==0x28)
//			{
//				mkey = 0xD0;
//			}
//			else if (iter->second.keyArray.mkey==0x25)
//			{
//				mkey = 0xCB;
//			}
//			else if (iter->second.keyArray.mkey==0x27)
//			{
//				mkey = 0xCD;
//			}
//			else
//				mkey = MapVirtualKey(iter->second.keyArray.mkey,0);
//		}
//		else
//			return false;
//		// ȡ��һ��������״̬
//		askey1_q = iter->second.keyArray.askey1;
//		if (askey1_q!=0)
//			askey1 = MapVirtualKey(askey1_q,0);
//
//		// ȡ�ڶ���������״̬
//		askey2_q = iter->second.keyArray.askey2;
//		if (askey2_q!=0)
//			askey2 = MapVirtualKey(askey2_q,0);
//
//
//		if (appointkey!=0)
//		{
//			appointkey = MapVirtualKey(appointkey,0);	//������������ֵת��Ϊɨ����ֵ
//			if (appointkey==DIK_RETURN)
//			{
//				int i = 1;
//			}
//			if ((!bContinuous&&appointkey==mkey||bContinuous&&GetGame()->GetKeyboard()->IsKeyDown(mkey))&&
//				(askey1_q==0||(askey1_q!=0&&askey1!=0&&GetGame()->GetKeyboard()->IsKeyDown(askey1)))&&
//				(askey2_q==0||(askey2_q!=0&&askey2!=0&&GetGame()->GetKeyboard()->IsKeyDown(askey2)))
//				)
//				return true;
//		}else
//		{
//			//appointkey = MapVirtualKey(appointkey,0);	//������������ֵת��Ϊɨ����ֵ
//			if (dif_key==DIK_RETURN)
//			{
//				int i = 1;
//			}
//			if ((!bContinuous&&dif_key==mkey||bContinuous&&GetGame()->GetKeyboard()->IsKeyDown(mkey))&&
//				(askey1_q==0||(askey1_q!=0&&askey1!=0&&GetGame()->GetKeyboard()->IsKeyDown(askey1)))&&
//				(askey2_q==0||(askey2_q!=0&&askey2!=0&&GetGame()->GetKeyboard()->IsKeyDown(askey2)))
//				)
//				return true;
//		}
//
//		//SHORT mkey = 0,askey1 = 0,askey2 = 0;
//		//int askey1_q = 0,askey2_q = 0;		// �����ݴ�Ҫ��ѯ�ĸ�����1��������2
//		//// ȡ������״̬
//		//if (iter->second.keyArray.mkey!=0)
//		//	mkey = GetKeyState(iter->second.keyArray.mkey);
//		//else
//		//	return false;
//
//		//// ���ȼ�������Ƿ񱻰��£����û�о�ֱ�ӷ���false�����ټ�����⸨����λ
//		//if (mkey==0||!(mkey&0x80)) return false;
//		//
//		//// ȡ��һ��������״̬
//		//askey1_q = iter->second.keyArray.askey1;
//		//if (askey1_q!=0)
//		//	askey1 = GetKeyState(askey1_q);
//
//		//// ȡ�ڶ���������״̬
//		//askey2_q = iter->second.keyArray.askey2;
//		//if (askey2_q!=0)
//		//	askey2 = GetKeyState(askey2_q);
//
//		//// ����һ�����������Ƿ�������Ҫ��״̬
//		////��Ҫ��1��������Ҫ�����£�2����Ҫ����֮һ�����£�3����Ҫ���߶������£�
//		//// ���������������֮һ��ʱ�򣬼��ʵ���������Ҫ�󲻣�������ͷ���false��
//		//if ((askey1_q!=0&&(askey1==0||!(askey1&0x80))) ||
//		//    (askey2_q!=0&&(askey2==0||!(askey2&0x80)))) 
//		//	return false;
//
//		//return true;
//	}
//	return false;
//}

// ���ݲ���dwFuncIndexָ���Ĺ�������ֵ���˹��ܵ��ȼ��Ƿ񱻴���,bContinuousָ���˹����Ƿ������������ס�����ŵ�����������ڰ���ǰ����ɨ����ȡ����ֵ��ʹ�ã�
bool CFuncKeyMapSystem::CheckFuncKey(MAPFKEY& mapkey,FKMS FuncIndex,bool bContinuous)
{
	BYTE  dif_key = GetGame()->GetKeyboard()->GetCurrentKey();

	// ���ȼ����ý����ʱ����ʱ�������е��ȼ�����
	if (GetGame()->GetCGuiEx()->GetHotKeySetupPageEx()->IsOpen()&&dif_key!=DIK_ESCAPE) return false;

	MAPFKEYITER iter = mapkey.find(FuncIndex);
	if (iter!=mapkey.end())
	{
		SHORT mkey = 0,askey1 = 0,askey2 = 0;
		int askey1_q = 0,askey2_q = 0;		// �����ݴ�Ҫ��ѯ�ĸ�����1��������2
		if (iter->second.keyArray.mkey!=0)
		{
			// ��������ֵ��С������ġ��������Ļ��� �ͽ�ɨ����ǿ������Ϊ����������ɨ����
			//����Ϊ���ĸ���û���Լ��������ֵ����������С���̵ķ�������������ֵ��MapVirtualKey����ת����������Ӧ��ɨ���룩
			if (iter->second.keyArray.mkey==0x26)
			{
				mkey = 0xC8;
			}else if (iter->second.keyArray.mkey==0x28)
			{
				mkey = 0xD0;
			}
			else if (iter->second.keyArray.mkey==0x25)
			{
				mkey = 0xCB;
			}
			else if (iter->second.keyArray.mkey==0x27)
			{
				mkey = 0xCD;
			}
			else
				mkey = MapVirtualKey(iter->second.keyArray.mkey,0);
		}
		else
			return false;
		// ȡ��һ��������״̬
		askey1_q = iter->second.keyArray.askey1;
		if (askey1_q!=0)
			askey1 = MapVirtualKey(askey1_q,0);

		// ȡ�ڶ���������״̬
		askey2_q = iter->second.keyArray.askey2;
		if (askey2_q!=0)
			askey2 = MapVirtualKey(askey2_q,0);


		if ((!bContinuous&&dif_key==mkey||bContinuous&&GetGame()->GetKeyboard()->IsKeyDown(mkey))&&
			(askey1_q==0||(askey1_q!=0&&askey1!=0&&GetGame()->GetKeyboard()->IsKeyDown(askey1)))&&
			(askey2_q==0||(askey2_q!=0&&askey2!=0&&GetGame()->GetKeyboard()->IsKeyDown(askey2)))
			)
			return true;
	}
	return false;
}

// ���ݲ���dwFuncIndexָ����ĳ�����ܼ��˹��ܵ��ȼ��Ƿ񱻴���,bContinuousָ���˹����Ƿ������������ס�����ŵ����(��������ָ����������ʹ��)
bool CFuncKeyMapSystem::CheckFuncKey(MAPFKEY& mapkey,FKMS FuncIndex,SHORT appointkey,bool bContinuous)
{	
	// ���ȼ����ý����ʱ����ʱ�������е��ȼ�����
	if (GetGame()->GetCGuiEx()->GetHotKeySetupPageEx()->IsOpen()&&appointkey!=VK_ESCAPE) return false;

	MAPFKEYITER iter = mapkey.find(FuncIndex);
	if (iter!=mapkey.end()&&appointkey!=0)
	{
		SHORT mkey = 0,askey1 = 0,askey2 = 0;
		int askey1_q = 0,askey2_q = 0;		// �����ݴ�Ҫ��ѯ�ĸ�����1��������2
		if (iter->second.keyArray.mkey!=0)
		{
			// ��������ֵ��С������ġ��������Ļ��� �ͽ�ɨ����ǿ������Ϊ����������ɨ����
			//����Ϊ���ĸ���û���Լ��������ֵ����������С���̵ķ�������������ֵ��MapVirtualKey����ת����������Ӧ��ɨ���룩
			//if (iter->second.keyArray.mkey==0x26)
			//{
			//	mkey = 0xC8;
			//}else if (iter->second.keyArray.mkey==0x28)
			//{
			//	mkey = 0xD0;
			//}
			//else if (iter->second.keyArray.mkey==0x25)
			//{
			//	mkey = 0xCB;
			//}
			//else if (iter->second.keyArray.mkey==0x27)
			//{
			//	mkey = 0xCD;
			//}
			//else
			//	mkey = MapVirtualKey(iter->second.keyArray.mkey,0);
			mkey = iter->second.keyArray.mkey;
		}
		else
			return false;
		// ȡ��һ��������״̬
		askey1_q = iter->second.keyArray.askey1;
		if (askey1_q!=0)
			askey1 = MapVirtualKey(askey1_q,0);

		// ȡ�ڶ���������״̬
		askey2_q = iter->second.keyArray.askey2;
		if (askey2_q!=0)
			askey2 = MapVirtualKey(askey2_q,0);

		//appointkey = MapVirtualKey(appointkey,0);	//������������ֵת��Ϊɨ����ֵ
		if ((!bContinuous&&appointkey==mkey/*||bContinuous&&GetGame()->GetKeyboard()->IsKeyDown(mkey)*/)&&
			(askey1_q==0||(askey1_q!=0&&askey1!=0&&GetGame()->GetKeyboard()->IsKeyDown(askey1)))&&
			(askey2_q==0||(askey2_q!=0&&askey2!=0&&GetGame()->GetKeyboard()->IsKeyDown(askey2)))
			)
			return true;
	}
	return false;
}



#endif

// ��Ĭ�ϵ�������Ϣ���뵱ǰʹ�õ�m_FuncKey��
//bool CFuncKeyMapSystem::DefaultToCurrent()
//{
//	if (m_DefultFuncKeyInfo.empty()) return false;
//	m_FuncKey.clear();
//
//	MAPINFOTYPEITER defultiter_b = m_DefultFuncKeyInfo.begin();
//	MAPINFOTYPEITER defultiter_e = m_DefultFuncKeyInfo.end();
//	for (;defultiter_b!=defultiter_e;defultiter_b++)
//	{
//		MAPFKEYINFOITER funciter_b = defultiter_b->second.FuncKeyInfo.begin();
//		MAPFKEYINFOITER funciter_e = defultiter_b->second.FuncKeyInfo.end();
//		for (;funciter_b!=funciter_e;funciter_b++)
//		{
//			m_FuncKey.insert(pair<FKMS,MAPKEY>(funciter_b->first,funciter_b->second.key));
//		}
//	}
//	return true;
//}


// ����ֵ
bool CFuncKeyMapSystem::InsertKeyToCurMap(MAPFKEY& mapkey,FKMS eIndex,MAPKEY key)
{
	if (mapkey.find(eIndex)!=mapkey.end()) return false;
	mapkey.insert(pair<FKMS,MAPKEY>(eIndex,key));
	return true;
}

// ɾ��ֵ
bool CFuncKeyMapSystem::DelKeyToCurMap(MAPFKEY& mapkey,FKMS eIndex)
{
	MAPFKEYITER KeyIter = mapkey.find(eIndex);
	if (KeyIter!=mapkey.end())
		mapkey.erase(KeyIter);
	else
		return false;
	return true;
}

// ���ĳ�����ܵ�ǰ�Ƿ������ȼ�ֵ
bool CFuncKeyMapSystem::CheckFunHasAvailableKey(MAPFKEY& mapkey,FKMS eIndex)
{
	MAPFKEYITER KeyIter = mapkey.find(eIndex);
	if (KeyIter!=mapkey.end())
		return true;
	else
		return false;
}

// �޸�ĳ�����ܵ��ȼ�ֵ
bool CFuncKeyMapSystem::AlterKeyofCurMap(MAPFKEY& mapkey,FKMS eIndex,MAPKEY key)
{
	MAPFKEYITER KeyIter = mapkey.find(eIndex);
	if (KeyIter!=mapkey.end())
		KeyIter->second = key;
	else 
		return false;
	return true;
}


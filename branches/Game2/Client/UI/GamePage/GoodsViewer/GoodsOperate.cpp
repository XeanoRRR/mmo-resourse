/**
* @filename:GoodsOperate.cpp
* @date: 2010/3/15
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: GoodsOperate.h ��ʵ�֣���GoodsViewer.sln��copy����
*/
#include "stdafx.h"
#include "GoodsOperate.h"
namespace UIData{
	const std::string GoodsOperate::s_vecGoodsType[gGoodsTypeNum] = {
			"��ͨ0",					//0
			"�����Ե���1",				//1
			"��Ƭ(��Ƕ��)2",			//2
			"��Ʒ3",					//3
			"ͷ��4",					//4
			"�·�5",					//5
			"����6",					//6
			"Ь��7",					//7
			"����8",					//8
			"����9",					//9
			"����10",					//10
			"��ָ11",					//11
			"����12",					//12
			"���13",					//13
			"ͷ��14",					//14
			"����15",					//15
			"����16",					//16
			"ѫ��17",					//17
			"����18",					//18
			"��ʸ19",					//19
			"��Ǯ20",					//20
			"����21",					//21
			"����22",					//22
			"ʰȡƷ23",					//23
			"����24",					//24
			"�����25"
	};
	
	GoodsOperate::GoodsOperate()
	{

	}

	GoodsOperate::~GoodsOperate()
	{
		m_vecGoodAttr.clear();
		m_vecGoodItemType.clear();
	}

	bool GoodsOperate::LoadGoodsListData(const char* FileName /* ="goodslist.dat"  */)
	{
		FILE* fp;
		fp = fopen(FileName,"rb");
		if( fp == NULL )
		{
			char str[256];
			sprintf_s(str, _T("file '%s' can't found!"), _T("goodslist.dat"));
			MessageBox(NULL,str,"error",MB_OK);
			return false;
		}
		// �ļ�ͷ�Ͱ汾
		char szHeader[6];
		memset(szHeader, 0, 6);
		fread(szHeader, 5, 1, fp);
		if( strcmp(szHeader, "GOODS") != 0 )
		{
			char str[256];
			sprintf_s(str,_T("file '%s' is not goodslist file!"), "goodslist.dat");
			MessageBox(NULL,str, "error",MB_OK);	
			return false;
		}
		long lVersion = 0;
		fread(&lVersion, sizeof(long), 1, fp);
		// ��������	

		//��Ʒ����
		long lNum = 0;
		fread(&lNum, sizeof(long), 1, fp);
		m_vecGoodAttr.reserve(lNum);

		for(int i=0; i<lNum; i++)
		{
			long lStrSize=0;
			char *pStr;
			tagGoodAttr stGoods;
			stGoods.dwSilverValue=0;
			stGoods.dwWeaponActType=0;

			fread(&stGoods.dwIndex, sizeof(stGoods.dwIndex), 1, fp);		//	���

			//	ԭʼ��   < 20
			fread(&lStrSize,sizeof(long),1,fp);
			pStr=new char[lStrSize+1];
			fread(pStr,lStrSize,1,fp);
			pStr[lStrSize]='\0';
			stGoods.strOrigName=pStr;
			delete [] pStr;

			//	�������� < 20
			fread(&lStrSize,sizeof(long),1,fp);
			pStr=new char[lStrSize+1];
			fread(pStr,lStrSize,1,fp);
			pStr[lStrSize]='\0';
			stGoods.strName=pStr;
			delete [] pStr;

			fread(&stGoods.bSave, sizeof(stGoods.bSave), 1, fp);			//	�Ƿ���Ҫ�����ݿ�
			fread(&stGoods.dwValue, sizeof(stGoods.dwValue), 1, fp);		//	�۸�
			fread(&stGoods.dwSilverValue, sizeof(stGoods.dwSilverValue), 1, fp);		//	�۸�
			fread(&stGoods.dwType, sizeof(stGoods.dwType), 1, fp);			//	��Ʒ����

			// �ͻ���ר������
			fread(&stGoods.dwIconId, sizeof(stGoods.dwIconId), 1, fp);		//	����ͼ��ID
			fread(&stGoods.dwGroundId, sizeof(stGoods.dwGroundId), 1, fp);		//	��غ�ͼ��ID
			fread(&stGoods.dwEquipID, sizeof(stGoods.dwEquipID), 1, fp);		//	��װͼ��ID
			fread(&stGoods.dwSound, sizeof(stGoods.dwSound), 1, fp);		//	ʰȡ����
			fread(&stGoods.dwSoundID1, sizeof(stGoods.dwSoundID1), 1, fp);		//	�Ӷ�/���˺�����
			fread(&stGoods.dwSoundID2, sizeof(stGoods.dwSoundID2), 1, fp);		//	���⹥��/��������
			fread(&stGoods.bSoundSwitch, sizeof(stGoods.bSoundSwitch), 1, fp);	//	����ʱ�Ƿ����
			fread(&stGoods.dwWeaponActType, sizeof(stGoods.dwWeaponActType), 1, fp);//	������Ӧ�Ķ�������

			//	�������� < 64
			fread(&lStrSize,sizeof(long),1,fp);
			pStr=new char[lStrSize+1];
			fread(pStr,lStrSize,1,fp);
			pStr[lStrSize]='\0';
			stGoods.strContent=pStr;
			delete [] pStr;

			// ��չ����
			long lEffectNum = 0;
			fread(&lEffectNum, sizeof(long), 1, fp);
			stGoods.vecAddAttr.reserve( lEffectNum );
			for(int j=0; j<lEffectNum; j++)
			{
				tagAddAttr stEffect;
				fread(&(stEffect.wType),sizeof(WORD),1,fp);
				fread(&(stEffect.bEnable),sizeof(bool),1,fp);
				fread(&(stEffect.bHide),sizeof(bool),1,fp);
				fread(&(stEffect.lValue1),sizeof(DWORD),1,fp);
				fread(&(stEffect.lValue2),sizeof(DWORD),1,fp);
				stGoods.vecAddAttr.push_back(stEffect);
			}

			// ��װ����
			lEffectNum = 0;
			fread(&lEffectNum, sizeof(long), 1, fp);
			stGoods.vecSuitAttr.reserve(lEffectNum);
			for(int j=0; j<lEffectNum; j++)
			{
				tagSuitAttr stEffect;
				fread(&(stEffect.wType),sizeof(WORD),1,fp);			
				fread(&(stEffect.lValue1),sizeof(DWORD),1,fp);
				fread(&(stEffect.lValue2),sizeof(DWORD),1,fp);
				fread(&(stEffect.wSuitNum),sizeof(WORD),1,fp);
				stGoods.vecSuitAttr.push_back(stEffect);
			}
			m_vecGoodAttr.push_back(stGoods);
		}
		fclose(fp);	
		return true;
	}

	bool GoodsOperate::LoadGoodsItemTypeData(const char* FileName /* = "itemtype.dat"  */)
	{
		std::ifstream ifsGoodsItemType( FileName );
		if ( ifsGoodsItemType.fail() )
		{
			MessageBox(NULL,"Load itemtype.dat error","Error",MB_OK);
			return false;
		}

		int iItemTypeCnt;
		std::string strItemIdx, strItemFlg, strItemName;
		ifsGoodsItemType >> iItemTypeCnt;
		if ( iItemTypeCnt > 0 )
		{
			m_vecGoodItemType.reserve( iItemTypeCnt );
			for ( int i = 0; i < iItemTypeCnt; ++i )
			{
				ifsGoodsItemType >> strItemIdx >> strItemFlg >> strItemName;
				m_vecGoodItemType.push_back( strItemName );
			}
		}
		else
		{
			MessageBox(NULL,"No have any ItemType","Error",MB_OK);
			return false;
		}
		return true;
	}

	const tagGoodAttr* GoodsOperate::GetGoodAttrByIndex(size_t Index) const
	{
		if(Index >= 0 || Index < m_vecGoodAttr.size())
			return &m_vecGoodAttr[Index];
		else
			return NULL;
	}

	const tagGoodAttr* GoodsOperate::GetGoodAttrByOgriName(const CEGUI::String &OgriName) const
	{
		size_t GoodsTotalNum = GetGoodsTotalNum();
		std::vector<UIData::tagGoodAttr>::const_iterator iter = m_vecGoodAttr.begin();
		for( ; iter != m_vecGoodAttr.end() ; ++iter)
		{
			if((*iter).strOrigName == OgriName)
				return &(*iter);
		}
		return NULL;
	}
	const std::string GoodsOperate::GetGoodsType(size_t Index)
	{
		if( Index >= gGoodsTypeNum)
			return std::string("");
		else
			return s_vecGoodsType[Index];
	}
}
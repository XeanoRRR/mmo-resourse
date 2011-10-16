#include "stdafx.h"
#include "GameSetting.h"

#include "..\..\..\Engine\Console.h"
#include "..\..\..\Engine\Utility.h"



IMPLEMENT_SINGLETON(Setting);

Setting::Setting()
{
	ZeroMemory(m_szTextureSharFolder,MAX_PATH);
}

Setting::~Setting()
{

}


bool Setting::LoadSetting()
{
	//����Ĭ����ɫ
	m_BackgroundColor = 0xFF2E374D;
	m_LightColor		= 0xFFFFFFFF;

	utility::File file;
	if(file.Open("GoodsViewer.config"))
	{
		//��ȡ
		utility::Text script;
		script.Create(&file);
		utility::Text::Node *pRoot = script.GetRootNode();

		utility::Text::Variable *pTemVar(NULL);

		m_BackgroundColor = pRoot->GetVar("backcolor")->GetHexValue();
		m_LightColor		= pRoot->GetVar("lightcolor")->GetHexValue();

		pTemVar				= pRoot->GetVar("TextureFolder");
		if(pTemVar == NULL || strlen(pTemVar->GetStringValue())==0)
		{
			MessageBox(NULL,"������GoodsViewer.config�ļ��µ�TextureFolder·����\n��·�����ڴ洢����ģ�͵������ļ���", "Error", MB_OK);
			file.Close();
			script.Destroy();
			return false;
		}
		else
			strcpy(m_szTextureSharFolder,pRoot->GetVar("TextureFolder")->GetStringValue());

		pTemVar = pRoot->GetVar("GoodsModelFolder");
		if(pTemVar == NULL || strlen(pTemVar->GetStringValue())==0)
		{
			MessageBox(NULL,"������GoodsViewer.config�ļ��µ�GoodsModelFolder·����\n��·�����ڴ洢������Ʒģ�͵�Ĭ��·����","Error",MB_OK);
			file.Close();
			script.Destroy();
			return false;
		}
		else
			strcpy(m_szGoodsModelFolder,pRoot->GetVar("GoodsModelFolder")->GetStringValue());
		script.Destroy();
	}
	file.Close();
	return true;
}

void Setting::SaveSetting()
{
	FILE *fp = fopen("GoodsViewer.config","wb");
	if (fp)
	{
		fprintf(fp,"backcolor = %x\n",m_BackgroundColor);
		fprintf(fp,"lightcolor = %x\n",m_LightColor);
		fprintf(fp, "TextureFolder = %s\n", m_szTextureSharFolder);
		fprintf(fp,"GoodsModelFolder = %s\n",m_szGoodsModelFolder);
		fclose(fp);
	}
}
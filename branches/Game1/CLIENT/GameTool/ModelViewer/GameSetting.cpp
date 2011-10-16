
#include "stdafx.h"
#include "..\..\frostengine\utility.h"
#include "..\..\frostengine\console.h"
#include "GameSetting.h"


IMPLEMENT_SINGLETON(Setting);

Setting::Setting(void)
{
	ZeroMemory(m_pszTextureSharFolder, MAX_PATH);
}

Setting::~Setting(void)
{
}

bool Setting::LoadSetting()
{
	//����Ĭ�ϵ�
	m_dwBackgroundColor = 0xFF2E374D;
	m_dwLightColor		= 0xFFFFFFFF;

	utility::File file;
	if (file.Open("ModelViewer.config"))
	{
		//��ȡ											  
		utility::Text script;
		script.Create(&file);
		utility::Text::Node *pRoot = script.GetRootNode();

		utility::Text::Variable * pTempVar(NULL);

		m_dwBackgroundColor = pRoot->GetVar("backcolor")->GetHexValue();
		m_dwLightColor	    = pRoot->GetVar("lightcolor")->GetHexValue();
		pTempVar = pRoot->GetVar("TextureFolder");

		if (pTempVar == NULL || strlen(pTempVar->GetStringValue()) == 0)
		{
			MessageBox(NULL, "������ModelViewer.config�ļ��µ�TextureFolder·����\n��·�����ڴ洢����ģ�͵������ļ���", "Error", MB_OK);
			file.Close();
			script.Destroy();
			return false;
		}else
			strcpy(m_pszTextureSharFolder, pRoot->GetVar("TextureFolder")->GetStringValue());

		script.Destroy();
	}
	file.Close();

	return true;
}

void Setting::SaveSetting()
{
	FILE *fp = fopen("ModelViewer.config","wb");
	if (fp)
	{
		fprintf(fp,"backcolor = %x\n",m_dwBackgroundColor);
		fprintf(fp,"lightcolor = %x\n",m_dwLightColor);
		fprintf(fp, "TextureFolder = %s\n", m_pszTextureSharFolder);
		fclose(fp);
	}
}
/**
* @filename:GameSetting.h
* @date: 2009/12/15
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: ��Ϸ�����࣬��������ɫ���ƹ���ɫ�������ļ�·��
*/
#pragma  once

class Setting
{
	DECLARE_SINGLETON(Setting);

	DWORD m_BackgroundColor;///< ����ɫ
	DWORD m_LightColor;///< �ƹ���ɫ
	char  m_szTextureSharFolder[MAX_PATH];///< �����ļ�·��
	char  m_szGoodsModelFolder[MAX_PATH];///<��Ʒģ���ļ�·��
public:
	Setting(void);
	~Setting(void);

	void SetBackGroundColor(DWORD BackColor){m_BackgroundColor = BackColor;};
	DWORD GetBackGroundColor()const { return m_BackgroundColor;};

	void SetLightColor(DWORD LightColor){m_LightColor = LightColor;};
	DWORD GetLightColor()const {return m_LightColor;};

	bool LoadSetting();
	void SaveSetting();
};
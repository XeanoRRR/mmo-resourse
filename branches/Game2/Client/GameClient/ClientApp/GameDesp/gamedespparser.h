#pragma once
/*
    ���ܣ�������Ʒ���������ļ���Ϊ��������������������ִ�
*/
class CGoods;
class GameDespParser
{
public:
    GameDespParser();
    ~GameDespParser();
    enum    SOURCE_UI
    {
        SI_PACKAGE = 1,
        SI_SHOP,
        //....
    };
    struct tagParse
    {
        string  Desp;   //�����ִ�
        string  strAtt;  //����ö��
        int     val;    //��������ȡֵ
        string  ImgInfo;  //ͼƬ��Ϣ
        tagParse():val(0){}
    };

    // ��ȡ�����ļ�(data/GoodsDescription.xml)
    bool  LoadDespCfg();

    /*������Ʒ*/
    /*para��isSelf �Ƿ���mainPlayer��*/
    const char *ParseGoods(CGoods *pGoods,bool isSelf);
    
    /*��������*/
    const char *ParseSkill(int skillID, int iskillLv);

    SOURCE_UI   GetSourceUI(){return sourceUI;}
    void        SetSourceUI(SOURCE_UI name){sourceUI = name;}
private:
    const char *AppendImage(const char *ImageFileNamePath,long d_width = 0,long d_height = 0);
    const char *AppendGoodIcon(string &ImageSetProperty);
    const char *AppendText(const char *text);

    string strPARSETEXT;
    vector<tagParse>    tagGoodsVector;
    vector<tagParse>    tagSkillVector;
    //��������ʶ��
    static char BaseAtt[5][64];

    SOURCE_UI           sourceUI;
};

/*[GOOD] ��ǽ��� 2010*5.19*/
#pragma once
namespace
{
    CEGUI::String;
    CEGUI::Window;
    CEGUI::Imageset;
}
class GoodsParser
{
public:
    GoodsParser();
    ~GoodsParser();

    struct GoodsInfo 
    {
        //��Ʒԭʼ��
        CEGUI::String GoodsOriName;
        //��Ʒ����
        DWORD Num;
        //�Ƿ���ѡ��
        bool  bCanBeSelect;
        //������ʶ��ID
        DWORD ID;
        GoodsInfo():GoodsOriName(""),Num(0),bCanBeSelect(false),ID(0){}
    };

    CEGUI::String Parse(const CEGUI::String &str);

    bool OnGoodsIconClicked(const CEGUI::EventArgs &e);

    static void RemoveGoodInfoFromWindow(CEGUI::Window *Wnd);
private:
    typedef map<CEGUI::Window *, GoodsInfo>    mapGoodInfo;
    static mapGoodInfo     GoodsMap;
    //��Ʒͼ��ѡ�е����
    static CEGUI::Imageset *         ImageSel;
};
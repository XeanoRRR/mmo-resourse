
/*[MOVE] ��ǽ��� 2010*5.19*/
#pragma once
namespace
{
    CEGUI::String;
    CEGUI::Window;
}
class AutoMoveParser
{
public:
    AutoMoveParser();
    ~AutoMoveParser();

    struct MoveInfo 
    {
        //����ID
        DWORD RegionID;
        //Ŀ�ĵ�����
        DWORD PosX;
        DWORD PosY;
        MoveInfo():RegionID(0),PosX(0),PosY(0){}
    };
    
    CEGUI::String Parse(const CEGUI::String &str);

    bool OnAutoMoveBtnClicked(const CEGUI::EventArgs &e);
    
    static void RemoveMapInfoFromWindow(CEGUI::Window *Wnd);
private:
    typedef map<CEGUI::Window *, MoveInfo>    mapInfo;
    static mapInfo     autoMoveMap;
};
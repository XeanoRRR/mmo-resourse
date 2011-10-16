#pragma once

#include "../../GamePage.h"

enum SERVERSTATE
{
    SS_PAUSE,   //��ͣ
    SS_WELL,    //����
    SS_BUSY,    //��æ
    SS_FULL,    //����
};
class WorldServerPage:public GamePage
{
public:
    WorldServerPage();

    virtual ~WorldServerPage(){};

    virtual void LoadPage();

    virtual bool Open();

    virtual bool Close();

    virtual void ReleasePage();

    void AddWorldInfo(long lID,char* strWorldName,long lState);

    void ClearWorldInfo();

    void UpdateWSList();

    const char * GetCDkey() {return m_strCDKey.c_str();}

    void SetCDKey(const char* cdkey) {m_strCDKey = cdkey;}

private:
    string                      m_strCDKey;

};
//NPC�Ի����߼���
#pragma once

class NpcTalkBox
{
public:
    struct ButtonInfo
    {
        string BtnName;
        bool   isEnable;
        ButtonInfo():BtnName(""),isEnable(true){}
    };
    struct PicInfo
    {
        ulong   lID;
        string  fileName;
        PicInfo():lID(0),fileName(""){}
    };
    
    NpcTalkBox():strTalkContext(""),PicID(0),PicPosX(0),PicPosY(0),SoundID(0),BKMusicID(0){LoadTalkBoxImageRes();}
    ~NpcTalkBox(){}

    void SetTalkContext(const char * strText) {strTalkContext = strText;}

    const char * GetTalkContext()   {return strTalkContext.c_str();}

    void  SetScriptID(CGUID guid)    {ScriptID = guid;}

    CGUID  &GetScriptGUID()          {return ScriptID;}

    void SetPicID(DWORD id)        {PicID = id;}

    DWORD GetPicID()                {return PicID;}

    void  SetPicPosX(DWORD x)       {PicPosX = x;}

    DWORD GetPicPosX()              {return PicPosX;}

    void SetPicPosY(DWORD y)       {PicPosY = y;}

    DWORD GetPicPosY()              {return PicPosY;}

    void SetSoundID(DWORD id)       {SoundID = id;}

    DWORD GetSoundID()              {return SoundID;}

    void SetBKMusicID(DWORD id)       {BKMusicID = id;}

    DWORD GetBKMusicID()            {return BKMusicID;}

    ButtonInfo &GetBtnInfo(int index)   {return tagBtnInfo[index];}

    //��ȡͼƬ��Դ
    void LoadTalkBoxImageRes();

    vector<PicInfo> &GetPicResList()    {return PicResList;}

    void SetGoodsSelectedIndex(BYTE index)     {SelectGoodsIndex = index;}

    BYTE GetGoodsSelectedIndex()        {return SelectGoodsIndex;}
private:
    //����
    string strTalkContext;
    //�ű�ID
    CGUID  ScriptID;
    //ͼƬID
    DWORD  PicID;
    //��ʾ����
    DWORD  PicPosX;

    DWORD  PicPosY;
    //��ЧID
    DWORD  SoundID;
    //��������
    DWORD  BKMusicID;
    //��Ʒѡ��ID
    BYTE  SelectGoodsIndex;
    //��ť
    ButtonInfo tagBtnInfo[2];
    //�����ַ�����ͼƬ��
    vector<PicInfo>     PicResList;
};
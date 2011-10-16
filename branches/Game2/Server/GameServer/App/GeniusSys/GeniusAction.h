/** 
 *  @file  GeniusAction.h
 *  @brief ���ܵ��츳ϵͳ
 *
 *  @author expter
 */


#pragma  once 

#include "GeniusComDef.h"
#include "../../../Public/Common/GeniusDef.h"

class  GeniusSys;
class  CPlayer;
class  GeniusAction
{
public:
    GeniusAction(CPlayer * player);
    ~GeniusAction();
    
    void   Update( UpdateEvent Event,long id, long Lvl );

    void   Remove( RemoveEvent Event,long id ,long Lvl );

    long   GetSkillAttri(long  skillid,long level,std::string  name,long xPos=0,long yPos=0);

    /// �츳
    void   Update();

    /// �츳����
    void   AddGenius(long id , long lvl);

    /// ���Ӽ���
    void   AddSkill(long ID, long level);

    /// ʹ�ü��ܵ�ʱ�򸽼�״̬
    void   AddGeniusStates(long skillid,CMoveShape* pDest,long Gid);

    /// �츳���Ա����
    void   CodeGeniusToDataBlock(DBWriteSet& setWriteDB, BOOL bAddRestoreTime = TRUE);

private:

    void   Add( UpdateEvent Event , long id );

    void   AddStates(CMoveShape* pDest,tagGeniusBuff& buf);

private:                                     
    std::map<long,GeniusSys*>  m_GeniusList;
    CPlayer*                    m_pOwner;                 
};
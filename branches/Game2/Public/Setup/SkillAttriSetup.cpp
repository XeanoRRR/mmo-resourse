#include "StdAfx.h"
#include "SkillAttriSetup.h"

SkillAttribute::SkillAttribute()
{
    m_SkillAttriTable.clear();
}

SkillAttribute::~SkillAttribute()
{
    m_SkillAttriTable.clear();
}

bool  SkillAttribute::Load()
{
    m_SkillAttriTable.clear();

   if(LoadEx("Data/SkillBaseAttribute.ini") && LoadEx("Data/SkillStatesAttribute.ini") )
       return true;

   return false;
}

bool  SkillAttribute::LoadEx(const char* filename)
{
    CRFile* prfile = rfOpen(filename);
    if(prfile == NULL)
    {
        Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("WS_SYS_31", filename));
        return false;
    }
    stringstream stream;
    prfile->ReadToStream(stream);
    rfClose(prfile);

    while(ReadTo(stream, "#"))
    {
        tagSkillAttr stSkillAttr;
        long         Skillid;
        long         Level;

        stream >> Skillid
            >> Level
            >> stSkillAttr.TempId           ///< ��ȡ��ǰ�ȼ�����tempid
            >> stSkillAttr.MinAtkDistance   ///< ��С����
            >> stSkillAttr.MaxAtkDistance   ///< ������
            >> stSkillAttr.BuffPower        ///< ����ǿ�� 
            >> stSkillAttr.IntonateTime     ///< ����ʱ��
            >> stSkillAttr.CoolDown         ///< ��ȴʱ��
            >> stSkillAttr.ActTime          ///< �ͷŶ���ʱ��
            >> stSkillAttr.HitTime          ///< �ͷź�����ʱ��
            >> stSkillAttr.FlyDelay         ///< �����ӳ�ʱ��
            >> stSkillAttr.LoopCount        ///< ѭ��ʱ��
            >> stSkillAttr.HurtFactor       ///< �˺�ϵ��
            >> stSkillAttr.HealFactor       ///< ����ϵ��
            >> stSkillAttr.BaseHurt         ///< �̶��˺�
            >> stSkillAttr.BaseHeal         ///< �̶�����
            >> stSkillAttr.EHit             ///< ���ӹ���
            >> stSkillAttr.EHeal            ///< ��������
            >> stSkillAttr.EHurtValue       ///< �����˺�ֵ
            >> stSkillAttr.EHealValue       ///< ��������ֵ
            >> stSkillAttr.SEnergy          ///< ����
            >> stSkillAttr.SMp              ///< ����
            >> stSkillAttr.SHp              ///< ��Ѫ
            >> stSkillAttr.SStrength        ///< ����
            >> stSkillAttr.SDexterity       ///< ��
            >> stSkillAttr.SCon             ///< ����
            >> stSkillAttr.SIntellect       ///< ��־
            >> stSkillAttr.SSpiritualism    ///< ����
            >> stSkillAttr.TaoismLvl        ///< ��Ч�����ȼ�
            >> stSkillAttr.OutEnergyPower   ///< �⹦ǿ��
            >> stSkillAttr.OutEnergyHurt    ///< �⹦�˺�
            >> stSkillAttr.EEnergyHit       ///< �ڹ��˺��ӳ�
            >> stSkillAttr.EEnergyHeal      ///< �ڹ����Ƽӳ�
            >> stSkillAttr.OutEnergyHitLvl  ///< �⹦���еȼ�
            >> stSkillAttr.EnergyHitLvl     ///< �ڹ����еȼ�
            >> stSkillAttr.OutEnergyCriRatio///< �⹦�����ȼ�
            >> stSkillAttr.EnergyCriRatio   ///< �ڹ������ȼ�
            >> stSkillAttr.OutEnergyCri     ///< �⹦�����˺�
            >> stSkillAttr.EnergyCri        ///< �ڹ������˺�
            >> stSkillAttr.OutEnergySpeed   ///< �⹦���ٵȼ�
            >> stSkillAttr.EnergySpeed      ///< �ڹ����ٵȼ�
            >> stSkillAttr.IgnoreTarget     ///< �����ȼ�
            >> stSkillAttr.ArmorDef         ///< ����ֵ
            >> stSkillAttr.DefLvl           ///< �����ȼ�
            >> stSkillAttr.ParryLvl         ///< �мܵȼ�
            >> stSkillAttr.ToughnessLvl     ///< ���Եȼ�
            >> stSkillAttr.BrokenArmorLvl   ///< �Ƽ׵ȼ�
            >> stSkillAttr.MissRatio        ///< ʧ��
            >> stSkillAttr.TaoismHurt       ///< �����˺�
            >> stSkillAttr.TaoismHitLvl     ///< �������еȼ�
            >> stSkillAttr.TaoismHitRatio   ///< ���������ȼ�
            >> stSkillAttr.TaoismSpeed      ///< �������ٵȼ�
            >> stSkillAttr.TaoismCri        ///< ���������˺�
            >> stSkillAttr.TaosimRiftLvl    ///< ������͸�ȼ�
            >> stSkillAttr.TaosimDefRatio   ///< ���������ȼ�
            >> stSkillAttr.ERation          ///< ���ʱ�������
            >> stSkillAttr.EArmorHp         ///< ������������
            >> stSkillAttr.AttackSpeed      ///< �����ٶ�
            >> stSkillAttr.MoveSpeed        ///< �ƶ��ٶ�
            >> stSkillAttr.ReadyTime        ///< ׼��ʱ��
            >> stSkillAttr.GridFlyTime      ///< ÿ�����ʱ�� 
            >> stSkillAttr.RangeTimes       ///< ���÷�ΧĿ�����
            >> stSkillAttr.RangeCount       ///< ���÷�ΧĿ�����
            >> stSkillAttr.BufId1           ///< ��ȡ����״ ̬
            >> stSkillAttr.BufLvl1
            >> stSkillAttr.BufNum1
            >> stSkillAttr.BufId2
            >> stSkillAttr.BufLvl2
            >> stSkillAttr.BufNum2
            >> stSkillAttr.BufId3
            >> stSkillAttr.BufLvl3
            >> stSkillAttr.BufNum3
            >> stSkillAttr.BufId4
            >> stSkillAttr.BufLvl4
            >> stSkillAttr.BufNum4
            >> stSkillAttr.BufId5
            >> stSkillAttr.BufLvl5
            >> stSkillAttr.BufNum5
            >> stSkillAttr.BufId6
            >> stSkillAttr.BufLvl6
            >> stSkillAttr.BufNum6;


        SkillIter itr = m_SkillAttriTable.find(Skillid);
        if( itr != m_SkillAttriTable.end() )
        {
            SkillTable::iterator litr =  itr->second.find( Level );
            if ( litr != itr->second.end() )
            {
                Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("WS_SYS_32", filename,Skillid));
                return false;
            }
        }

        m_SkillAttriTable[ Skillid ][ Level ] = stSkillAttr;
    }
    return true;
}

void  SkillAttribute::ConfigEnCode(DBWriteSet& dbWrite)
{
     dbWrite.AddToByteArray( (long) m_SkillAttriTable.size() );

     for ( SkillIter Iter( m_SkillAttriTable.begin()) ; Iter != m_SkillAttriTable.end() ; Iter ++ )
     {
         /// skill id
         dbWrite.AddToByteArray( Iter->first );
         dbWrite.AddToByteArray( (long)Iter->second.size() );
         for ( SkillTable::iterator it = Iter->second.begin() ; it != Iter->second.end() ; it++)
         {                
             /// add level
             dbWrite.AddToByteArray( it->first );
             dbWrite.AddToByteArray( &it->second, sizeof(tagSkillAttr) );
         }
     }
}

void  SkillAttribute::ConfigDecode(DBReadSet& dbRead)
{
     m_MapTable.clear();

     long  SkillNum = dbRead.GetLongFromByteArray();

     for ( int i = 0 ; i < SkillNum ; i++ )
     {
         long Skillid = dbRead.GetLongFromByteArray();
         long LevelNum= dbRead.GetLongFromByteArray();
         for ( int j = 0 ; j < LevelNum ; j++ )
         {
             long lvl = dbRead.GetLongFromByteArray();
             tagSkillAttr  tmpSkillAttr;
             dbRead.GetBufferFromByteArray(&tmpSkillAttr,sizeof(tagSkillAttr) );

             m_SkillAttriTable[ Skillid ][ lvl ] = tmpSkillAttr;
             std::map<string,short>  mp;
             tmpSkillAttr.AddMapTable( mp );

             m_MapTable[ Skillid ][ lvl ] = mp;
         }
     }
}

long SkillAttribute::GetStrCondif(long skillid,long lvl,std::string str,long Dis)
{
    if ( str == "SkillLvl" )
    {
        return lvl;
    }

    MapTable::iterator IdItr = m_MapTable.find( skillid );
    if ( IdItr != m_MapTable.end() )
    {
        MapAtrTable::iterator LvlItr = IdItr->second.find( lvl );
        if ( LvlItr != IdItr->second.end() )
        {
            if ( str == "FutureTime" )
            {
                 return  ( LvlItr->second["ReadyTime"] +  Dis   * LvlItr->second["GridFlyTime"] );
            }
            return LvlItr->second[str];
        }
    }

    return 0;
}

bool  SkillAttribute::IsExist(long skillid,long lvl)
{
    MapTable::iterator IdItr = m_MapTable.find( skillid );
    if ( IdItr != m_MapTable.end() )
    {
        MapAtrTable::iterator LvlItr = IdItr->second.find( lvl );
        if ( LvlItr != IdItr->second.end() )
        {
            return true;
        }
    }
    
    Log4c::Trace(ROOT_MODULE,"Skllid %d is not Exist!", skillid );

    return false;            
}
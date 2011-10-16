
/** 
 * @brief  ���ڷ�֮ѧϵͳ�����ݶ���
 * @author expter
 */


#pragma  once 

const  long  lMaxDisttractValue = 100;  ///> ���ֵ

#define  CAMP(value,Min,Max) \
    {\
        if( value > Max )\
            value = Max;\
        if( value < Min )\
            value = Min;\
    }

struct  tagBuddhism
{
    long  lDistract;    ///> ��ǰ����ֵ
    long  lDryNum;      ///> �������
    long  lDryEndTime;  ///> ��Ľ���ʱ��
    long  lLastTime;    ///> �ϴ�ʹ�ü���ʱ��
    long  lBudhismTimer;///> ����ֵ���Ͷ�ʱ������id
    long  lDryTimer;    ///> ����ն�ʱ��

    tagBuddhism()
    {
        memset( this , 0 , sizeof(tagBuddhism) );
    }

    long UpdateDistract( long Value)
    {
        lDistract +=Value;
        
        CAMP( lDistract , 0 , lMaxDisttractValue );

        return lDistract;
    }

    long  UpdateDryNum( long value, long Max )
    {
        lDryNum  += value;

        CAMP( lDryNum , 0 , Max );

        return lDryNum;
    }

    void  ClearDryNum()     { lDryNum = 0 ; }
    long  RetDryNum() const { return lDryNum;  }
    long  RetDistract()const{ return lDistract;}
    bool  IsFull()    const { return lDistract>= lMaxDisttractValue;}
    bool  IsEmpty()   const { return lDistract <= 0 ;}

    long  RegisterTimer(long Time)
    {
    }

    long  UpdateEndTime( long Time)
    {

        return 0;
    }
};

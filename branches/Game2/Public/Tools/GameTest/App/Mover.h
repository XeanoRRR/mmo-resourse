///
///
///
#ifndef ___MOVER_H
#define ___MOVER_H

class CMoveShape;
class CFindPath;

/*
���ܣ������ƶ�ģ��
���ߣ��߿�
���ڣ�2010.7.7
*/
class CFloatMove
{
public:
    CFloatMove();
    ~CFloatMove();
    /*
    ����: ��A*Ѱ·���·���� ת���� ��Ҫת����߶ε㡣
    ���ߣ��߿�
    ���ڣ�2010.7.7
    */
    struct tagMovPos 
    {
        float x;
        float y;
        tagMovPos():x(0.f),y(0.f){}
        const tagMovPos& operator = (const tagMovPos &other)
        {
            if (&other == this)
            {
                return *this;
            }
            x = other.x;
            y = other.y;
        }
    };
    /*��Ҳ�������С����
    ˵��:(��ҵ�ǰλ�ú�Ŀ��ľ��볬����ֵ ���ƶ�)
    */
    const float MINIMUS_OP_DISTANCE;
    
    /*ֱ���ƶ��ϵ������� (������ֵ�����㽫�����)*/
    const float MAX_LINE_DISTANCE;

    //A*Ѱ·�� ���ת���ͳ���·�������߶�·���� 
    list<tagMovPos>  A_MoveList;       
    //ֱ���ƶ���ֹ���·������߶ε�
    list<tagMovPos>  LineMoveList;
    //ת��A*·��Ϊת���߶�·��
    void UpdateA_MoveList(CFindPath *pFindPath,const float fPosX,
        const float fPosY,const float fDestX,
        const float fDestY);
    
    //ֱ���ƶ�ʱ������󲽳��Ĳ�ִ���
    void UpdatelinearMoveList(const float fPosX,const float fPosY,
        const float fDestX, const float fDestY);
    //�Ƿ���ֱ���ƶ�
    bool  bIsBeelineMove;

    //��һ��·�����Ƿ���A*�е�
    bool  bIsPreviousMoveByA_Star;

};

///
/// author: Kevin Lynx
///
class Mover
{
public:
    Mover( CMoveShape *owner );

    ~Mover();

    void Move( float destX, float destY );

    bool Update();

    void Reset();

    void MoveTo( float destX, float destY );

    void OnTimeOut( ulong timerid );

    void Stop();
private:
    void OnStop();

    void OnArrived();
private:
    float m_offsetX;
    float m_offsetY;
    CMoveShape *m_owner;
    ulong m_lastUpdate;
    ulong m_endTime;
    CFloatMove m_floatMove;
    ulong m_arriveTimer;
    bool m_arrived;
    bool m_finished;
};


#endif

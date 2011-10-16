/** 
 * @file   PhaseManager.h  
 * @author Expter
 * @brief  ����ҵĹ����࣬��Ҫ�ǶԲ�ͬ��λ��ʾ�����µ���Ϣ������߲������ˡ�
 * @date   2010/03/18        
 */

#pragma  once

#include "../MoveShape.h"
namespace PhaseSystem
{
    class FilterPlayer
    {
    public:
        ///
        /// ��Ϊ������˺���Ҫ���Ĳ��������ڷ�����������������ֻ����Ҫ�ж�2��֮��Ĺ�ϵ���ɡ�
        /// 
        static  bool Filter(CMoveShape* player , CMoveShape* shape)
        {
            /// �����λ��Ҵ��ڱ���״̬
            if ( shape->GetType() == TYPE_MONSTER 
                && shape->GetPhaseID() == player->GetPhaseID() 
                && player->GetState() == CShape::STATE_AGONAL )
                return true;
            return player->GetPhaseID() != shape->GetPhaseID() ;
        }
    };
}

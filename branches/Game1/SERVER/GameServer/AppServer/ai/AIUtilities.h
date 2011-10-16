///
/// @file AIUtilities.h
/// @author Kevin Lynx
/// @brief To provide some useful utilities for AI module.
///
#pragma once

#include <vector>
#include "../Monster.h"

namespace AIUtilities
{
	///
	/// �����б���ҡ�����ȣ�
	///
	typedef std::vector<CShape*> ShapeListType;

	///
	/// ��ȡĳ��shape��Χ��shape�б���ҡ�����ȣ�
	/// 
	/// @return ���ҵ���shape����
	///
	size_t FindAroundShape( CServerRegion *pRegion, CShape *pFinder, ShapeListType &outList );

	///
	/// ��ȡָ��GUID�Ĺ�������
	///
	CMoveShape *FindShape( CServerRegion *pRegion, const CGUID &target_id );

	///
	/// ��shape�б���ж�����ˣ�������AI����
	///
	/// @param filter ��������functor������true��ʾ��Ҫ���˴�shape
	///
	template <typename Filter>
	size_t FilterShapeList( ShapeListType &shapes, Filter filter )
	{
		for( ShapeListType::iterator it = shapes.begin(); it != shapes.end(); )
		{
			if( filter( *it ) )
			{
				it = shapes.erase( it );
			}
			else
			{
				++ it;
			}
		}

		return shapes.size();
	}

	///
	/// To provide some pre-define filter
	///
	namespace FilterSet
	{
		///
		/// ��������shape
		///
		struct Died
		{
			bool operator()( CShape *pShape )
			{
				CMoveShape *pTarget = static_cast<CMoveShape*>( pShape );
				return pTarget->IsDied() == TRUE;
			}
		};

		///
		/// �����������shape
		///
		struct Invisible
		{
			Invisible( CMoveShape *pOwner )
			{
				m_pOwner = pOwner;
			}
			bool operator()( CShape *pShape )
			{
				CMoveShape *pTarget = static_cast<CMoveShape*>( pShape );
				if ( !pTarget->IsVisible(m_pOwner) )	
				{
					return true;
				}
				return false;
			}

			CMoveShape *m_pOwner;
		};

		///
		/// ���˳���׷����Χ��shape
		///
		struct Distance
		{
			Distance( CMonster *pMonster )
			{
				m_pOwner = pMonster;
			}
			bool operator()( CShape *pShape )
			{
				long lTraceRange = m_pOwner->GetBaseChaseRange();
				return m_pOwner->Distance( pShape ) > lTraceRange;
			}
	
			CMonster *m_pOwner;
		};

		///
		/// ��Ū
		///
		struct Fool
		{
			Fool( const CGUID &id ) : m_FoolID( id )
			{
			}
			bool operator()( CShape *pShape )
			{
				return m_FoolID == pShape->GetExID();
			}
			const CGUID &m_FoolID;
		};

		///
		/// �޵�״̬
		///
		struct GodState
		{
			bool operator()( CShape *pShape )
			{
				CMoveShape *pMoveShape = static_cast<CMoveShape*>( pShape );
				return pMoveShape->GetStateParamByID( STATE_BASE_GOD, "Num" ) != 0;
			}
		};

		///
		/// default filter
		///
		struct Default
		{
			Default( CMonster *pOwner )
			{
				m_pOwner = pOwner;
			}
			bool operator()( CShape *pShape )
			{
				Died died;
				Invisible invi( m_pOwner );
				Distance dis( m_pOwner );
				Fool fool( m_pOwner->GetBefoolID() );
				GodState god;
				return died( pShape ) || invi( pShape ) || dis( pShape ) || fool( pShape ) || god( pShape );
			}
			CMonster *m_pOwner;
		};
	}

	///
	/// Some wrapper functions of the filter
	///
	inline bool IsInGodState( CShape *pShape )
	{
		FilterSet::GodState god_s;
		return god_s( pShape );
	}

	/// TODO : add more
}
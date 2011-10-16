
/*
*	file:		ConvenientQueue.h
*	Brief:		��������ʵĶ���
*	detail:		
*				����map��ʵ��
*				�����ɾ��
*				���������
*				
*				
*	Authtor:	�ſ���
*	Datae:		2008
*/


#pragma once

template<typename _KeyType, typename _ValueType>
class ConvenientQueue
{
public:
	//! _ValueType���͵���ЧֵΪ����ĳ�ʼֵ
	ConvenientQueue(VOID);
	//! Ϊ_ValueType��������һ���������Чֵ
	ConvenientQueue(const _ValueType &InvalidValue);
	~ConvenientQueue(VOID);
	
public:
	//!								
	ULONG							size(VOID);
	//!								���һ��Ԫ�أ���ֵ�ظ�����ɾ����ǰֵ��������ֵ�ŵ�����ĩβ
	VOID							push_back(const _KeyType &_key, const _ValueType &_value);
	//!								�������Ԫ��
	VOID							pop_front(VOID);
	//!								�����Ϊ�գ����ع������ʱ���õ���Чֵ
	_ValueType&						get_front(VOID);
	//!								
	VOID							erase(const _KeyType &_key);
	//!								
	BOOL							find(const _KeyType &_key);
	//!								
	_ValueType&						find_and_get(const _KeyType &_key);
	//!								
	BOOL							find_and_erase(const _KeyType &_key);
	//!								
	VOID							clear(VOID);

	//!								Ϊÿһ��Ԫ��ִ�к���
	VOID							each_run(VOID (*pRunFunc)(_ValueType&, LPVOID), LPVOID pParam);

private:
	ULONG							GetOrder(VOID);

private:
	struct tagSaveValue
	{
		_KeyType	_key;
		_ValueType	_value;
		///
		tagSaveValue(VOID){}
		tagSaveValue(const _KeyType &in_key, const _ValueType &in_value):_key(in_key), _value(in_value){}
	};

	typedef pair<_KeyType, ULONG>		KeyToOrder;
	typedef map<_KeyType, ULONG>		MapKeyToOrder;

	typedef pair<ULONG, tagSaveValue>	OrderToValue;
	typedef map<ULONG, tagSaveValue>	MapOrderToValue;

	

	//!									Key���Ⱥ�˳���ӳ��
	MapKeyToOrder						m_mapKeyToOrderMapping;
	//!									�Ⱥ�˳���Ŷ�ֵ��ӳ��
	MapOrderToValue						m_mapOrderToValueMapping;
	//!									˳������
	ULONG								m_uOrderSeed;
	//!									��ʾ��Ч��ֵ
	_ValueType							m_InvalidValue;
};


//! �ඨ��
/////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename _KeyType, typename _ValueType>
ConvenientQueue<_KeyType, _ValueType>::ConvenientQueue(VOID)
:m_uOrderSeed(0)
{
}

template<typename _KeyType, typename _ValueType>
ConvenientQueue<_KeyType, _ValueType>::ConvenientQueue(const _ValueType &InvalidValue)
:m_uOrderSeed(0), m_InvalidValue(InvalidValue)
{
}

template<typename _KeyType, typename _ValueType>
ConvenientQueue<_KeyType, _ValueType>::~ConvenientQueue(VOID)
{
}

template<typename _KeyType, typename _ValueType>
inline ULONG ConvenientQueue<_KeyType, _ValueType>::size(VOID)
{
	return (ULONG)m_mapKeyToOrderMapping.size();
}

template<typename _KeyType, typename _ValueType>
inline VOID ConvenientQueue<_KeyType, _ValueType>::push_back(const _KeyType &_key, const _ValueType &_value)
{
	//! �����ظ�ֵ����ɾ��
	erase(_key);

	ULONG lOrder = GetOrder();
	m_mapKeyToOrderMapping.insert(KeyToOrder(_key, lOrder));
	m_mapOrderToValueMapping.insert(m_mapOrderToValueMapping.end(), OrderToValue(lOrder, tagSaveValue(_key, _value)));	
}

template<typename _KeyType, typename _ValueType>
inline VOID ConvenientQueue<_KeyType, _ValueType>::pop_front(VOID)
{
	if(0 == m_mapKeyToOrderMapping.size())
		return;
	m_mapKeyToOrderMapping.erase(m_mapOrderToValueMapping.begin()->second._key);
	m_mapOrderToValueMapping.erase(m_mapOrderToValueMapping.begin());
}

template<typename _KeyType, typename _ValueType>
inline _ValueType& ConvenientQueue<_KeyType, _ValueType>::get_front(VOID)
{
	if(0 == m_mapOrderToValueMapping.size())
		return m_InvalidValue;
	return m_mapOrderToValueMapping.begin()->second._value;
}

template<typename _KeyType, typename _ValueType>
inline VOID ConvenientQueue<_KeyType, _ValueType>::erase(const _KeyType &_key)
{
	map<_KeyType, ULONG>::iterator iteK2O = m_mapKeyToOrderMapping.find(_key);
	if(m_mapKeyToOrderMapping.end() != iteK2O)
	{
		m_mapOrderToValueMapping.erase(iteK2O->second);
		m_mapKeyToOrderMapping.erase(iteK2O);
	}
}

template<typename _KeyType, typename _ValueType>
inline BOOL ConvenientQueue<_KeyType, _ValueType>::find(const _KeyType &_key)
{
	map<_KeyType, ULONG>::iterator iteK2O = m_mapKeyToOrderMapping.find(_key);
	return (m_mapKeyToOrderMapping.end() != iteK2O);
}

template<typename _KeyType, typename _ValueType>							
inline _ValueType& ConvenientQueue<_KeyType, _ValueType>::find_and_get(const _KeyType &_key)
{
	map<_KeyType, ULONG>::iterator iteK2O = m_mapKeyToOrderMapping.find(_key);
	if(m_mapKeyToOrderMapping.end() != iteK2O)
	{
		assert(m_mapOrderToValueMapping.end() != m_mapOrderToValueMapping.find(iteK2O->second));
		return m_mapOrderToValueMapping[iteK2O->second]._value;
	}
	return m_InvalidValue;
}

template<typename _KeyType, typename _ValueType>
inline BOOL ConvenientQueue<_KeyType, _ValueType>::find_and_erase(const _KeyType &_key)
{
	map<_KeyType, ULONG>::iterator iteK2O = m_mapKeyToOrderMapping.find(_key);
	BOOL bRe = m_mapKeyToOrderMapping.end() != iteK2O;
	if(bRe)
	{
		assert(m_mapOrderToValueMapping.end() != m_mapOrderToValueMapping.find(iteK2O->second));
		m_mapOrderToValueMapping.erase(iteK2O->second);
		m_mapKeyToOrderMapping.erase(iteK2O);
	}
	return bRe;
}

template<typename _KeyType, typename _ValueType>
inline VOID ConvenientQueue<_KeyType, _ValueType>::clear(VOID)
{
	m_mapOrderToValueMapping.clear();
	m_mapKeyToOrderMapping.clear();
	m_uOrderSeed = 0;
}

template<typename _KeyType, typename _ValueType>
VOID ConvenientQueue<_KeyType, _ValueType>::each_run(VOID (*pRunFunc)(_ValueType&, LPVOID), LPVOID pParam)
{
	MapOrderToValue::iterator ite = m_mapOrderToValueMapping.begin();
	for (; ite != m_mapOrderToValueMapping.end(); ++ite)
	{
		(*pRunFunc)(ite->second._value, pParam);
	}
}

template<typename _KeyType, typename _ValueType>
ULONG ConvenientQueue<_KeyType, _ValueType>::GetOrder(VOID)
{
	if(0xFFFFFFFF == m_uOrderSeed)
	{
		//! ���ӹ��㣬���¼���˳��
		m_uOrderSeed = 0;

		MapKeyToOrder	tmpMapKeyToOrder(m_mapKeyToOrderMapping);
		MapOrderToValue tmpMapOrderToValue(m_mapOrderToValueMapping);

		m_mapKeyToOrderMapping.clear();
		m_mapOrderToValueMapping.clear();

		map<ULONG, tagSaveValue>::iterator iteO2V = tmpMapOrderToValue.begin();
		for(; iteO2V != tmpMapOrderToValue.end(); ++iteO2V)
		{
			++m_uOrderSeed;
			tmpMapOrderToValue[m_uOrderSeed] = iteO2V->second;
			assert(tmpMapKeyToOrder.end() != tmpMapKeyToOrder.find(iteO2V->second._key));
			tmpMapKeyToOrder[iteO2V->second._key] = m_uOrderSeed;
		}
	}

	++m_uOrderSeed;
	return m_uOrderSeed;
}
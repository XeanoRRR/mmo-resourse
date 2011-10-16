/**
 * @file    Singleton.h
 * @author  Fox (yulefox@gmail.com)
 * @date    Nov.19, 2008
 * @brief   �ο�Boost��singletonʵ�ֵĶ��̰߳�ȫ�ĵ���.
 * ���߳�Ӧ��ʹ�� GetInst(CLASS_NAME) ����,
 * ���߳�ʱ��Ҫʹ�� GetMTInst(CLASS_NAME).
 * eg. GetInst(CPetCtrl).LoadConfig();
 */

#pragma once

#define GetInst(CLASS_NAME)                     \
	CSingleton<CLASS_NAME>::Inst()

#define GetMTInst(CLASS_NAME)                   \
	CMTSingleton<CLASS_NAME>::Inst()

template<typename T>
class CSingleton
{
public:
	static T& Inst()
	{
		static T obj;
		return obj;
	}

private:
	// ��ֹ�ⲿ����
	CSingleton(void) {}
	~CSingleton(void) {}
	CSingleton(const CSingleton&);
	CSingleton& operator=(const CSingleton&);
};

template<typename T>
class CMTSingleton
{
public:
	// ��֤�ڱ�����ʹ��ʱ, obj�Ѿ�����ʼ��.
	static T& Inst()
	{
		static T obj;
		creator.DoNothing();
		return obj;
	}

private:
	// ��ֹ�ⲿ����
	CMTSingleton(void) {}
	~CMTSingleton(void) {}
	CMTSingleton(const CMTSingleton&);
	CMTSingleton& operator=(const CMTSingleton&);

	// ��֤��ʵ����ʱ, obj�Ѿ�����ʼ��.
	class CInstCreator
	{
	public:
		// ��֤��main����֮ǰ, Inst()�ѱ�����.
		CInstCreator(void) { CMTSingleton<T>::Inst(); }
		inline void DoNothing(void) const {}
	};

	static CInstCreator creator;
};

template<typename T>
typename CMTSingleton<T>::CInstCreator CMTSingleton<T>::creator;

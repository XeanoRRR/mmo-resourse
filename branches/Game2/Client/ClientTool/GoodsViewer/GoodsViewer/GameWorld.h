/**
* @filename:GameWorld.h
* @date: 2009/12/15
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose:  ��װ��Ϸ������
*/
#pragma  once

class GameWorld
{
	HWND m_hWnd;
	static GameWorld *m_Instance;
public:
	
	/// @brief
	/// ��ȡ��Ϸ������ʵ��ָ��
	static GameWorld *GetInstance(void);


	/// @brief
	/// ��ʼ����Ϸ������

	static void InitInstance(void);

	/// @brief
	/// ������Ϸʱ����ʵ��

	static void ExitInstance(void);

	/// @brief
	/// ������Ϸ��������Ҫ������
	/// @param: in hWnd
	/// ���ھ��

	void Create(HWND hWnd);

	/// @brief
	/// �ͷ���Ϸ���紴����ʹ�õ���Դ
	void Destroy(void);

	/// @brief
	/// ˢ����Ϸ����
	bool Update(void);

	/// @brief
	/// ��Ⱦ��Ϸ����
	void Render(void);
};
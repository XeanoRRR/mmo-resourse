///
/// @file CBusinessPackage.h
/// @author Kevin lynx
/// @brief ������ҵ����
///
#pragma once

class CVolumeLimitGoodsContainer;
class CGoods;
class CPlayer;

namespace Business
{
	class CPackage
	{
	public:
		CPackage();

		~CPackage();

		///
		/// ��GS�������ݣ������ڱ����Ժ����DB
		///
		void DecodeFromGS( CPlayer *pPlayer, DBReadSet &db );

		///
		/// ���������ݣ���DB��ȡ�������GS
		///
		void EncodeForGS( DBWriteSet &db );

		///
		/// �����ݿ��������
		///
		/// ��ʵ�ϣ��ú���������ȫ�����ݿ�������ݣ�����entity group���룬������entity group����
		/// ��ҵ������������CEntityManager::DetailBusinessPackCDBPlayerToCPlayer�У����˺�������
		/// �Ǵ��������ҵ���������Ǹ���Ʒ��ʱ����Ч�����������������ԡ�
		///
		void DecodeFromDB( CGoods *pContainerGoods );

	public:
		CVolumeLimitGoodsContainer *m_pContainer;
		CGoods *m_pGoods;
	};
}
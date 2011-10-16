#pragma once

class GameModel;
// ģ����Դ������
class CModelResource
{
public:
	// ���Ͷ���
	enum MODEL_TYPE
	{
		TYPE_WING 			    = 1,	// ���
		TYPE_RIDE				= 2,	// ����
		TYPE_WEAPON				= 3,	// ����
		TYPE_SHADOW             = 4,
		TYPE_TALISMAN           = 5,    //��֪��ʲô����
	};
public:
	CModelResource(void);
	virtual ~CModelResource(void);

protected:
	struct ModelKey
	{
		long Type;
		DWORD GraphicsID;
	};
	class hash_model_compare:public hash_compare<ModelKey>
	{
	public:
		//����ɢ��ֵ
		size_t operator( )( const ModelKey& Key ) const
		{
			return Key.Type;
		}
		//less operator
		bool operator( )( const ModelKey& _Key1,const ModelKey& _Key2 ) const
		{
			if(_Key1.Type < _Key2.Type)	return true;
			else if(_Key1.Type == _Key2.Type && _Key1.GraphicsID < _Key2.GraphicsID)	return true;
			return false;
		}
	};

	typedef hash_map<ModelKey,GameModel*,hash_model_compare>	HMModels;
	typedef hash_map<ModelKey,GameModel*,hash_model_compare>::iterator itModel;

	HMModels m_mapGameModel;

public:
	GameModel * Get(long Type,WORD wModelID);

	GameModel * FindModel(long Type,WORD wModelID);
	GameModel * ReadModel(long Type,WORD wModelID);
	void Release(long Type,WORD wModelID);
	void ReleaseAll();

	void Create(void);
	void Destroy();
};

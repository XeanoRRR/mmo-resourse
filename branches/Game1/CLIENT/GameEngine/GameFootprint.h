#pragma once


class GameFootprint
{
	DWORD m_dwShaderGroupHandle;//��ӡ�������ͱ�
public:	
	


	struct tagFootprint
	{
		D3DXVECTOR3 pos[1024];//��

		D3DXVECTOR2 m_pUV0[1024];//uv����
		int dwCount;	//Ҫ��Ⱦ���ٸ�������

		DWORD dwLifeTimeStart;  
		DWORD dwLifeTimeLength;
		DWORD dwColor;

		tagFootprint *pNext;
		tagFootprint *pNextRender;
	};
private:
	tagFootprint * m_pFootprintUsed;
	tagFootprint * m_pFootprintFree;

	tagFootprint * m_pFootprintRenderHead;

public:
	GameFootprint(void);
	~GameFootprint(void);

	tagFootprint * NewFootprint(void);

	BOOL Create();
	void Destroy();

	void Update();
	void Render();

	//**********************************************************
	//**********************************************************
	//decal�޸�
	static const long maxDecalVertices = 1024;	//��󶥵���
	float decalEpsilon;	

	struct ColorRGBA
	{
		float		red;
		float		green;
		float		blue;
		float		alpha;

		ColorRGBA() {}

		ColorRGBA(float r, float g, float b, float a)
		{
			red = r;
			green = g;
			blue = b;
			alpha = a;
		}
	};

	struct Triangle
	{
		unsigned short index[3];	//ÿ�����������������
	};

	//���
	float DotProduct(const D3DXVECTOR3& p, const D3DXVECTOR3& q)
	{
		return (p.x * q.x + p.y * q.y + p.z * q.z);
	}
	static float DotProductEx(const D3DXVECTOR4& p, const D3DXVECTOR3& q)
	{
		return (p.x * q.x + p.y * q.y + p.z * q.z + p.w);
	}

	D3DXVECTOR3		decalCenter;	//����������
	D3DXVECTOR3		decalNormal;	//��������

	D3DXVECTOR4		leftPlane;
	D3DXVECTOR4		rightPlane;
	D3DXVECTOR4		bottomPlane;
	D3DXVECTOR4		topPlane;
	D3DXVECTOR4		frontPlane;
	D3DXVECTOR4		backPlane;

	long		decalVertexCount;	//����������
	long		decalTriangleCount;	//������������

	D3DXVECTOR3		vertexArray[maxDecalVertices];	//��������
	D3DXVECTOR3		vertexNormalArray[maxDecalVertices];	//���㷨������
	D3DXVECTOR2		texcoordArray[maxDecalVertices];	//������������
	ColorRGBA	colorArray[maxDecalVertices];	//��ɫ����
	Triangle	triangleArray[maxDecalVertices];	//����������

	bool AddPolygon(long vertexCount, const D3DXVECTOR3 *vertex, const D3DXVECTOR3 *normal);

	void ClipMesh(long triangleCount, const Triangle *triangle, const D3DXVECTOR3 *vertex, const D3DXVECTOR3 *normal);

	long ClipPolygon(long vertexCount, const D3DXVECTOR3 *vertex, const D3DXVECTOR3 *normal, D3DXVECTOR3 *newVertex, 
		D3DXVECTOR3 *newNormal) const;

	static long ClipPolygonAgainstPlane(const D3DXVECTOR4& plane, long vertexCount, const D3DXVECTOR3 *vertex, 
		const D3DXVECTOR3 *normal, D3DXVECTOR3 *newVertex, D3DXVECTOR3 *newNormal);


	void InitDecal(const D3DXVECTOR3& center, const D3DXVECTOR3& normal, const D3DXVECTOR3& tangent, float width, float height, float depth,
							long triangleCount, const Triangle *triangle, const D3DXVECTOR3 *vertex, const D3DXVECTOR3 *normal2);
	//**********************************************************
	//**********************************************************
};

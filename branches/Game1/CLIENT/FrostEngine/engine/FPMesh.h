/*========================================================================
	ProgressiveMesh:ģ�ͼ��㷨,ΪLOD�ṩ֧��
	��ȡ����ɾ������һ�δ�ģ��������ɾ��һ�����㣨U����ɾ���������£�
	�ö��������һ�����㣨V������һ���ߣ���ͬʱӵ������������������ɾ�����ı�ֻӵ���������������εĽṹ���������εĶ���U����V��
	ȷ��һ�������Ƿ����ɾ����ԭ���Ǹ�������ģ���е�λ���Լ�����Χ�����������γɵ����ʵĴ�С�����ʿ��Է�ӳ������Ĺ���ӽ��̶ȣ���
	һ������£����ڱ�Ե�ϵĶ��㲻Ҫɾ�������������нϴ��ɾ�����ۣ�����ԽС��ɾ�����۾�ԽС��
========================================================================*/
#pragma once

#ifndef _PMESH_H_
#define _PMESH_H_

#include <windows.h>
#include <set>
#include <algorithm>
#include <fmodel.h>

using namespace std;

class PMVertex;
class PMTriangle;
class CollapseMap;
class ProgressiveMesh;

typedef vector<PMTriangle*> TriangleList;
typedef set<PMVertex*> VertexSet;
typedef set<PMTriangle*> TriangleSet;

/*========================================================================
	������
========================================================================*/
class PMVertex
{
public:
    int                 id;			//ID������
    D3DXVECTOR3         pos;		//��������
    BOOL                bCollapsed;	//�Ƿ�����
	float				collapseCost;
	PMVertex			*collapseTo;
	bool				bRemoved;
	bool				bToBeRemoved;

    set<PMVertex*>		neighbors;//�ڶ���
    set<PMTriangle*>	faces;	//ӵ�е���

    PMVertex(const D3DXVECTOR3 & v, int _id);
	bool	IsBorder();
	bool	IsBorderEdgeWith(PMVertex *);
	void	NotifyRemoved();
	void	RemoveNeighbor(PMVertex *);
};
/*========================================================================
	��������
========================================================================*/
class PMTriangle
{
public:
    int                 id;
    PMVertex*           vertices[3];
	D3DXVECTOR3         normal;
	int					indexes[6];
	bool				bRemoved;

    PMTriangle(PMVertex *v0, PMVertex *v1, PMVertex *v2, int _id);

    void	ReplaceVertex(PMVertex *u, PMVertex *v);
    void	ComputeNormal();
    int     FindVertex(PMVertex *v);
	void	NotifyRemoved();

};
/*========================================================================
	��ֵ����
========================================================================*/
class CollapseMap
{
public:
    CollapseMap(int size);
    ~CollapseMap();	
	int GetNextCollapser();
	void SetCollapse(int index,float cost);
private:
    float   *costArray;
    int     size;
};
/*========================================================================
	ģ�ͼ���
========================================================================*/
class PMesh
{
public:
	PMesh();
	~PMesh();
	//����һ��ģ�ͼ򻯶���,v--���������,nV--��������,poly--ԭʼ�����α�,nP--����������
	void Create(const D3DXVECTOR3 *v, int nV, const model::Mesh::tagTriangle *pTriangleBuffer, int nP);
	//���ɲ��ϸ��,countList�洢����Ӧ�������������б�,dataList�洢�����α�
	//numLevelsָ�������Ŀ,reducutionValues�Ǹ���ϸ�ڼ򻯵Ĳ���,���Ȳ���С��numLevels
    void Build(vector<int> *countList,vector<int *> *dataList,int numLevels,float *reductionValues);

private:
    void AddVertex(PMVertex* v);
    void AddFace(PMTriangle* face);
    float CalculateEdgeCollapseCost(PMVertex *u, PMVertex *v);
    float CalculateVertexCollapseCost(PMVertex *u);
    void CalculateCollapseMap();
    void CollapseVertex(PMVertex *u);
	int* MakeLOD();

private:
    CollapseMap*          _collapseMap;
    int                   _currentNumIndexes;
    vector<PMVertex*>       _vertices;
    vector<PMTriangle*>     _poly;
};

#endif
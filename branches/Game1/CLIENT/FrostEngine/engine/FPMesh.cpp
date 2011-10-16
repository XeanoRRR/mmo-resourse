#include "stdafx.h"
#include "futility.h"
#include "FPMesh.h"

const float NEVER_COLLAPSE_COST=9999.9f;
/*========================================================================
	���췽��,��ʼ���������
========================================================================*/
PMesh::PMesh()
{
	_collapseMap=NULL;
}
/*========================================================================
	�����Դ
========================================================================*/
PMesh::~PMesh()
{
    vector<PMVertex*>::iterator itVert = _vertices.begin();
    while (itVert != _vertices.end())
    {
        PMVertex *p = (*itVert);
        //(*itVert)->neighbors.clear();
        //(*itVert)->faces.clear();
        itVert = _vertices.erase(itVert);
        SAFEDELETE(p);
    }
	_vertices.clear();

    vector<PMTriangle*>::iterator itPoly = _poly.begin();
    while (itPoly != _poly.end())
    {
        PMTriangle *tri = (*itPoly);
        itPoly = _poly.erase(itPoly);
        SAFEDELETE(tri);
    }
    _poly.clear();

    SAFEDELETE(_collapseMap);
}
/*========================================================================
	���һ������
========================================================================*/
void PMesh::AddVertex(PMVertex* v)
{
    _vertices.push_back(v);
}
/*========================================================================
	���һ����,������Ӧ��ͳ�ƺ���Ϣ�洢����
========================================================================*/
void PMesh::AddFace(PMTriangle* face)
{
    for (int i = 0;i < 3;i ++)
    {       
        //���еĶ�������,�������ǵ�������Ϣ
        for (int j = 0;j < 3;j ++)
		{
            if (i != j)
            {
				face->vertices[i]->neighbors.insert(face->vertices[j]);
            }
		}
        //���汣�浽������������б���
       face->vertices[i]->faces.insert(face);
    }
    _poly.push_back(face);
}

/*========================================================================
	����һ��ģ�ͼ���
	v - ��������
	poly - ������
	nV - ������Ŀ
	nP - �����Ŀ
	vLen - ÿ�������������еĳ���
	pLen - ÿ�����������еĳ���
========================================================================*/
void PMesh::Create(const D3DXVECTOR3 *v, int nV, const model::Mesh::tagTriangle *pTriangleBuffer, int nP)
{
	for (int i = 0;i < nV;i ++)
    {
        AddVertex(new PMVertex(v[i], i));
    }
	
    for (int i = 0;i < nP;i ++)
    {    
		PMTriangle *triangle=new PMTriangle(_vertices[pTriangleBuffer[i].dwPNCIndex[0]], 
										    _vertices[pTriangleBuffer[i].dwPNCIndex[1]],
											_vertices[pTriangleBuffer[i].dwPNCIndex[2]], i);

		triangle->indexes[0] = pTriangleBuffer[i].dwUV0Index[0];
		triangle->indexes[1] = pTriangleBuffer[i].dwUV0Index[1];
		triangle->indexes[2] = pTriangleBuffer[i].dwUV0Index[2];
		triangle->indexes[3] = pTriangleBuffer[i].dwUV1Index[0];
		triangle->indexes[4] = pTriangleBuffer[i].dwUV1Index[1];
		triangle->indexes[5] = pTriangleBuffer[i].dwUV1Index[2];
		
        AddFace(triangle);
    }
	this->_collapseMap=new CollapseMap(nV);
}
/*========================================================================
	������������֮���ɾ������ֵ
	u - ��һ������
	v - �ڶ�������
========================================================================*/
float PMesh::CalculateEdgeCollapseCost(PMVertex *u, PMVertex *v)
{
	if(u->IsBorderEdgeWith(v))return NEVER_COLLAPSE_COST;//�ڱ���,ɾ�����ȼ����
	if(u->IsBorder())return NEVER_COLLAPSE_COST-1;//�Ǳ��ϵĵ�,ɾ�����ȼ��ǳ���
	if(u->faces.size()==1&&v->faces.size()==1)return NEVER_COLLAPSE_COST+1;//���ڶ������εı���,���ȼ������

	float cost=NEVER_COLLAPSE_COST;
	//�ۼӹ��ߵ�����������
	TriangleList sides;
	TriangleSet::iterator it,itend=u->faces.end();
	for (it=u->faces.begin();it!=itend;it++)
    {
		if ((*it)->FindVertex(v)!=-1)
        {
            sides.push_back((*it));
        }
	}
	//����Щ�����ζ�������������������ε����ʵ����ֵ
	float curvature=0.001f;
	for(it=u->faces.begin();it!=u->faces.end();it++) 
	{
		//�󹲱ߵ������κ���������ε���С����
		float mincurv=1.0;
		for(int j = 0;j < (int)sides.size();j ++) 
		{
			if((*it) == sides[j])
				continue;

			float dotprod = D3DXVec3Dot(&(*it)->normal,&sides[j]->normal);

			mincurv = min(mincurv,(1.002f - dotprod) * 0.5f);
		}
		curvature = max(curvature,mincurv);
	}
	cost=curvature;

	//����Ǵ��ڱ��ϣ���ò�������ɾ������
	//if(u->IsBorder())
	//{
	//	if(sides.size()>1)
	//	{
	//		cost=NEVER_COLLAPSE_COST;//1.0f;
	//	}
	//	else
	//	{
	//		D3DXVECTOR3 collapseEdge=edge,otherBorderEdge;
	//		float kinkiness,maxKinkiness=0.0f;
	//		VertexSet::iterator n, nend=u->neighbors.end();
	//		collapseEdge.Normalize(); 
	//		for(n=u->neighbors.begin();n!=nend;n++)
	//		{
	//			if (*n != v && (*n)->IsBorderEdgeWith(u))
	//			{
	//				otherBorderEdge = u->pos - (*n)->pos;
	//				otherBorderEdge.Normalize();
	//				// This time, the nearer the dot is to -1, the better, because that means
	//				// the edges are opposite each other, therefore less kinkiness
	//				// Scale into [0..1]
	//				kinkiness = (otherBorderEdge^collapseEdge + 1.002f) * 0.5f;
	//				maxKinkiness = max(kinkiness, maxKinkiness);
	//			}
	//		}
	//		cost = maxKinkiness; 
	//	}
	//}
	//else
	//{
	//	float curvature=0.001f;
	//	for(it=u->faces.begin();it!=u->faces.end();it++) 
	//	{
	//		float mincurv=1.0;
	//		for(int j = 0;j < sides.size();j ++) 
	//		{
	//			if((*it)==sides[j])continue;
	//			float dotprod = (*it)->normal ^ sides[j]->normal;
	//			mincurv = min(mincurv,(1.002f - dotprod) * 0.5f);
	//		}
	//		curvature = max(curvature,mincurv);
	//	}
	//	cost=curvature;
	//}
	return cost;
}

/*========================================================================
	����һ�������ɾ������ֵ�����ؿ��ܵ���Сֵ
========================================================================*/
float PMesh::CalculateVertexCollapseCost(PMVertex *u)
{	
	if(u->neighbors.empty())
	{
		u->NotifyRemoved();
		return u->collapseCost;
	}
	float cost=0;
	u->collapseTo=0;
	u->collapseCost=NEVER_COLLAPSE_COST;
	VertexSet::iterator it,vend=u->neighbors.end();
    for(it=u->neighbors.begin();it!=vend;it++)
	{
		cost=this->CalculateEdgeCollapseCost(u,(*it));
		if(cost<u->collapseCost||!u->collapseTo)
		{
			u->collapseCost=cost;
			u->collapseTo=(*it);
		}
	}
	return u->collapseCost;
}
/*========================================================================
	�������Զ����ɾ������ֵ���洢�ڱ���
========================================================================*/
void PMesh::CalculateCollapseMap()
{
	for(int i=0;i<(int)this->_vertices.size();i++)
	{
		float cost=CalculateVertexCollapseCost(_vertices[i]);
		this->_collapseMap->SetCollapse(i,cost);
	}
}
/*========================================================================
	��ģ��ʵ�ֲַ�ϸ��
========================================================================*/
void PMesh::Build(vector<int> *countList,vector<int *> *dataList,int numLevels,float *reductionValues)
{
	if(!countList||!dataList)return;
    CalculateCollapseMap();
	countList->clear();
	dataList->clear();
	this->_currentNumIndexes = (int)this->_poly.size();
	int numVertex = (int)this->_vertices.size(),numCollapse=0;
	bool abandon=false;
	int count=numLevels;
	while(count&&!abandon)
	{
		if(!abandon)
		{
			float reductionValue=reductionValues[numLevels-count];
			numCollapse = int (reductionValue >= 1.0f? reductionValue : (float)numVertex * reductionValue);
			if(numVertex-numCollapse<3)numCollapse=numVertex-3;
			while(numCollapse--&&!abandon)
			{
				int nextIndex=this->_collapseMap->GetNextCollapser();
				PMVertex *v=this->_vertices[nextIndex];
				if(v==0||v->collapseTo==0)
				{
					abandon=true;
					break;
				}
				if(v->collapseTo->bRemoved)
				{
					//OutputConsole("�쳣���!\n");
				}
				this->CollapseVertex(v);
			}
		}
		countList->push_back(this->_currentNumIndexes);
		dataList->push_back(MakeLOD());
		count--;
	}
}
/*========================================================================
	��������α�
========================================================================*/
int* PMesh::MakeLOD()
{
	int *poly=new int[this->_currentNumIndexes*9];
	int count=(int)_poly.size();
	PMTriangle *face=0;
	int j=0;
	for(int i=0;i<count;i++)
	{
		face=_poly[i];
		if(face->bRemoved)continue;
		poly[j+0]=face->vertices[0]->id;
		poly[j+1]=face->vertices[1]->id;
		poly[j+2]=face->vertices[2]->id;
		for(int n=0;n<6;n++)
		{
			poly[j+n+3]=face->indexes[n];
		}
		j+=9;
	}
	return poly;
}
/*========================================================================
	ɾ������
========================================================================*/
void PMesh::CollapseVertex(PMVertex *u)
{
	if(u->collapseCost==NEVER_COLLAPSE_COST)return;
	if(!u->collapseTo)return;

	PMVertex *v=u->collapseTo;
	VertexSet recomputeSet;//��Ҫ���¼���Ķ��㼯�ϣ�Ӧ��ɾ��������ı�ģ�ͽṹ
	TriangleSet removalFaceList;//��Ҫɾ������
	TriangleSet replacementFaceList;//��Ҫ�������滻����

	u->collapseCost=NEVER_COLLAPSE_COST;
	u->collapseTo=NULL;
	_collapseMap->SetCollapse(u->id,NEVER_COLLAPSE_COST);
	recomputeSet.insert(v);
	VertexSet::iterator it,itend=u->neighbors.end();
	for(it=u->neighbors.begin();it!=itend;it++)
	{
		recomputeSet.insert((*it));
	}
	itend=v->neighbors.end();
	for(it=v->neighbors.begin();it!=itend;it++)
	{
		recomputeSet.insert((*it));
	}
	TriangleSet::iterator itf,fend=u->faces.end();
	for(itf=u->faces.begin();itf!=fend;itf++)
	{
		if((*itf)->FindVertex(v)>=0&&!(*itf)->bRemoved)
		{
			removalFaceList.insert((*itf));
			this->_currentNumIndexes-=1;
		}
		else
		{
			replacementFaceList.insert((*itf));
		}
	}
	u->bToBeRemoved=true;
	fend=replacementFaceList.end();
	for(itf=replacementFaceList.begin();itf!=fend;itf++)
	{
		(*itf)->ReplaceVertex(u,v);
	}
	fend=removalFaceList.end();
	for(itf=removalFaceList.begin();itf!=fend;itf++)
	{
		(*itf)->NotifyRemoved();
	}
	u->NotifyRemoved();
	itend=recomputeSet.end();
	for(it=recomputeSet.begin();it!=itend;it++)
	{
		v=(*it);
		float cost=CalculateVertexCollapseCost(v);
		this->_collapseMap->SetCollapse(v->id,cost);
	}
}
//���췽��,��ʼ��СΪsize��
CollapseMap::CollapseMap(int size)
{
    this->size=size;
	costArray=new float[size];
}
//�������
CollapseMap::~CollapseMap()
{
    SAFEDELETEARRAY(costArray);
}
/*========================================================================
	����ָ�������Ĵ���ֵ
========================================================================*/
void CollapseMap::SetCollapse(int index, float cost)
{
	costArray[index]=cost;
}
//������һ���ʺ�ɾ���Ķ�����������������С������
int CollapseMap::GetNextCollapser()
{
	float min=9999.9f;
	float index=0;
	for(int i=0;i<size;i++)
	{
		if(costArray[i]<min)
		{
			min   = costArray[i];
			index = (float)i;
		}
	}
	return (int)index;
}
//����һ������
PMVertex::PMVertex(const D3DXVECTOR3 &v,int _id)
{
	pos=v;
	id=_id;
	this->bCollapsed=false;
	this->bRemoved=false;
	this->bToBeRemoved=false;
	this->collapseTo=NULL;
	this->collapseCost=NEVER_COLLAPSE_COST;
}
//�ö����Ƿ��ڱ߽���
bool PMVertex::IsBorder()
{
	VertexSet::iterator itv,vend=this->neighbors.end();
	for(itv=neighbors.begin();itv!=vend;itv++)
	{
		if(IsBorderEdgeWith((*itv)))return true;
	}
	return false;
}
//���Ըö�������һ�����Ƿ񹹳ɱ߽�
bool PMVertex::IsBorderEdgeWith(PMVertex *v)
{
	int sidescount=0;
	TriangleSet::iterator itf,fend=faces.end();
	for(itf=faces.begin();itf!=fend;itf++)
	{
		if((*itf)->FindVertex(v)>=0)
		{
			sidescount++;
		}
	}
	return sidescount==1;
}
//ɾ���ö��㣬ͬʱ�ı䶥����ھӶ��������
void PMVertex::NotifyRemoved()
{
	VertexSet::iterator it,itend=neighbors.end();
	for(it=neighbors.begin();it!=itend;it++)
	{
		(*it)->neighbors.erase(this);
	}
	this->bRemoved=true;
	this->collapseTo=NULL;
	this->collapseCost=NEVER_COLLAPSE_COST;
}
//�Ӷ�����ھ���ɾ��һ������
void PMVertex::RemoveNeighbor(PMVertex *v)
{
	VertexSet::iterator itv=neighbors.find(v);
	if(itv==neighbors.end())
	{
		return;
	}
	TriangleSet::iterator itf,fend=faces.end();
	for(itf=faces.begin();itf!=fend;itf++)
	{
		if((*itf)->FindVertex(v)>=0)
		{
			return;
		}
	}
	neighbors.erase(v);
	if(neighbors.empty()&&!this->bToBeRemoved)
	{
		this->NotifyRemoved();
	}
}
//����һ��������
PMTriangle::PMTriangle(PMVertex *v0,PMVertex *v1,PMVertex *v2,int _id)
{
	vertices[0]=v0;
	vertices[1]=v1;
	vertices[2]=v2;
	id=_id;
	this->bRemoved=false;
	this->ComputeNormal();
}
//�滻���е�һ������
void PMTriangle::ReplaceVertex(PMVertex *u, PMVertex *v)
{
    if(FindVertex(v)!=-1) return;
    int n=FindVertex(u);
    if(n==-1)return;    
    vertices[n] = v;
	//����
	u->faces.erase(this);
	v->faces.insert(this);
	for(int i=0;i<3;i++)
	{
		if(i==n)continue;
		u->RemoveNeighbor(vertices[i]);
		vertices[i]->RemoveNeighbor(u);
		v->neighbors.insert(vertices[i]);
		vertices[i]->neighbors.insert(v);
	}
    ComputeNormal();
}
//������ķ�����
void PMTriangle::ComputeNormal()
{
    D3DXVECTOR3 v0 = vertices[0]->pos;
    D3DXVECTOR3 v1 = vertices[1]->pos;
    D3DXVECTOR3 v2 = vertices[2]->pos;
	D3DXVECTOR3 vA,vB;
    
	vA = v1 - v0;
	vB = v2 - v1;

	D3DXVec3Cross(&normal,&vA,&vB);
	D3DXVec3Normalize(&normal,&normal);
}
//�����������Ƿ�����ö��㣬��������ֵ��0-2����-1��ʾ�������ö���
int PMTriangle::FindVertex(PMVertex *v)
{
    for (int i = 0;i < 3;i ++)
    {
        if (vertices[i] == v) return i;
    }
    return -1;
}
//ɾ���������Σ����ı����ж��������
void PMTriangle::NotifyRemoved()
{
	for(int i=0;i<3;i++)
	{
		this->vertices[i]->faces.erase(this);
	}
	for(int i=0;i<3;i++)
	{
		int i2=(i+1)%3;
		this->vertices[i]->RemoveNeighbor(this->vertices[i2]);
		this->vertices[i2]->RemoveNeighbor(this->vertices[i]);
	}
	this->bRemoved=true;
}

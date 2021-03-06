#pragma once
#include "CommonInclude.h"
#include "wiLoader.h"

#include <unordered_map>
#include <list>

class Frustum;

typedef std::list<Cullable*> CulledList;

typedef std::list<Object*> CulledObjectList;
typedef std::unordered_map<Mesh*,CulledObjectList> CulledCollection;

class wiSPTree
{
protected:
	int childCount;
	wiSPTree();
public:
	~wiSPTree();
	void initialize(const std::vector<Cullable*>& objects, const XMFLOAT3& newMin = XMFLOAT3(FLOAT32_MAX, FLOAT32_MAX, FLOAT32_MAX), const XMFLOAT3& newMax = XMFLOAT3(-FLOAT32_MAX, -FLOAT32_MAX, -FLOAT32_MAX));

	struct Node
	{
		int depth;
		AABB box;
		std::vector<Node*> children;
		Node* parent;
		int count;
		CulledList objects;

		Node(Node* newParent, const AABB& newBox = AABB(), int newDepth = 0) :parent(newParent), box(newBox), depth(newDepth)
		{
			count = 0;
		}
		~Node()
		{
			for (Node* x : children)
			{
				SAFE_DELETE(x);
			}
		}
	};
	Node* root;
	enum CullStrictness
	{
		SP_TREE_STRICT_CULL,
		SP_TREE_LOOSE_CULL
	};
	enum SortType
	{
		// Completely bypass the sorting (can leave duplicated objects!)
		SP_TREE_SORT_NONE,
		// Perform only a fast sort to eliminate duplicate elements
		SP_TREE_SORT_UNIQUE,
		// Perform a sort by distance to the query origin. Distant objects will be prioritized. Only unique objects will be left.
		SP_TREE_SORT_BACK_TO_FRONT,
		// Perform a sort by distance to the query origin. Close objects will be prioritized. Only unique objects will be left.
		SP_TREE_SORT_FRONT_TO_BACK
	};

	// Sort culled list by their distance to the origin point
	static void Sort(const XMFLOAT3& origin, CulledList& objects, SortType sortType = SP_TREE_SORT_UNIQUE);

	void AddObjects(Node* node, const std::vector<Cullable*>& newObjects);
	void getVisible(Frustum& frustum, CulledList& objects, SortType sortType = SP_TREE_SORT_UNIQUE, CullStrictness type = SP_TREE_STRICT_CULL, Node* node = nullptr);
	void getVisible(AABB& frustum, CulledList& objects, SortType sortType = SP_TREE_SORT_UNIQUE, CullStrictness type = SP_TREE_STRICT_CULL, Node* node = nullptr);
	void getVisible(SPHERE& frustum, CulledList& objects, SortType sortType = SP_TREE_SORT_UNIQUE, CullStrictness type = SP_TREE_STRICT_CULL, Node* node = nullptr);
	void getVisible(RAY& frustum, CulledList& objects, SortType sortType = SP_TREE_SORT_UNIQUE, CullStrictness type = SP_TREE_STRICT_CULL, Node* node = nullptr);
	void getAll(CulledList& objects, Node* node = nullptr);
	// Updates the tree. Returns null if successful, returns a new tree if the tree is resized. The old tree can be thrown away then.
	wiSPTree* updateTree(Node* node = nullptr);
	void Remove(Cullable* value, Node* node = nullptr);
};

class Octree : public wiSPTree
{
public:
	Octree(const std::vector<Cullable*>& objects, const XMFLOAT3& newMin = XMFLOAT3(FLOAT32_MAX, FLOAT32_MAX, FLOAT32_MAX), const XMFLOAT3& newMax = XMFLOAT3(-FLOAT32_MAX, -FLOAT32_MAX, -FLOAT32_MAX))
	{
		childCount=8;
		initialize(objects, newMin, newMax);
	}
};
class QuadTree : public wiSPTree
{
public:
	QuadTree(const std::vector<Cullable*>& objects, const XMFLOAT3& newMin = XMFLOAT3(FLOAT32_MAX, FLOAT32_MAX, FLOAT32_MAX), const XMFLOAT3& newMax = XMFLOAT3(-FLOAT32_MAX, -FLOAT32_MAX, -FLOAT32_MAX))
	{
		childCount = 4;
		initialize(objects, newMin, newMax);
	}
};
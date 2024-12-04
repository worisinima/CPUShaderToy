
#pragma once

// C++ Implementation of Quad Tree
#include <cmath>
#include <iostream>
using namespace std;

// Used to hold details of a point
struct Point 
{
	Point(int _x, int _y){ x = _x; y = _y; }
	Point(){ x = 0; y = 0; }
	int x, y;
};

// The objects that we want stored in the quad tree
struct Node 
{
	Node(Point _pos, int _data){ pos = _pos; data = _data; }
	Node() { data = 0; }

	Point pos;
	int data;
};

// The main quad tree class
class Quad 
{
	// Hold details of the boundary of this node
	Point topLeft;
	Point botRight;

	// Contains details of node
	Node* n;

	// Children of this tree
	Quad* topLeftTree;
	Quad* topRightTree;
	Quad* botLeftTree;
	Quad* botRightTree;

public:
	Quad()
	{
		topLeft = Point(0, 0);
		botRight = Point(0, 0);
		n = NULL;
		topLeftTree = NULL;
		topRightTree = NULL;
		botLeftTree = NULL;
		botRightTree = NULL;
	}
	Quad(Point topL, Point botR)
	{
		n = NULL;
		topLeftTree = NULL;
		topRightTree = NULL;
		botLeftTree = NULL;
		botRightTree = NULL;
		topLeft = topL;
		botRight = botR;
	}
	void Insert(Node*);
	Node* Search(Point);
	bool InBoundary(Point);
};

// Insert a node into the quad tree
void Quad::Insert(Node* node)
{
	if (node == NULL)
	{
		return;
	}

	// Current quad cannot contain it
	if (InBoundary(node->pos) == false)
	{
		return;
	}

	// We are at a quad of unit area
	// We cannot subdivide this quad further
	if (abs(topLeft.x - botRight.x) <= 1 && abs(topLeft.y - botRight.y) <= 1) 
	{
		if (n == NULL)
		{
			n = node;
		}
		return;
	}

	//Left
	if ((topLeft.x + botRight.x) / 2 >= node->pos.x) 
	{
		// Indicates topLeftTree
		if ((topLeft.y + botRight.y) / 2 >= node->pos.y) 
		{
			if (topLeftTree == NULL)
			{
				topLeftTree = new Quad(Point(topLeft.x, topLeft.y), Point((topLeft.x + botRight.x) / 2, (topLeft.y + botRight.y) / 2));
			}
			topLeftTree->Insert(node);
		}
		// Indicates botLeftTree
		else 
		{
			if (botLeftTree == NULL)
			{
				botLeftTree = new Quad(Point(topLeft.x, (topLeft.y + botRight.y) / 2), Point((topLeft.x + botRight.x) / 2, botRight.y));
			}
			botLeftTree->Insert(node);
		}
	}
	//Right
	else 
	{
		// Indicates topRightTree
		if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
		{
			if (topRightTree == NULL)
			{
				topRightTree = new Quad(Point((topLeft.x + botRight.x) / 2, topLeft.y), Point(botRight.x, (topLeft.y + botRight.y) / 2));
			}
			topRightTree->Insert(node);
		}
		// Indicates botRightTree
		else 
		{
			if (botRightTree == NULL)
			{
				botRightTree = new Quad(Point((topLeft.x + botRight.x) / 2, (topLeft.y + botRight.y) / 2), Point(botRight.x, botRight.y));
			}
			botRightTree->Insert(node);
		}
	}
}

// Find a node in a quad tree
Node* Quad::Search(Point p)
{
	// Current quad cannot contain it
	if (InBoundary(p) == false)
	{
		return NULL;
	}

	// We are at a quad of unit length
	// We cannot subdivide this quad further
	if (n != NULL)
	{
		return n;
	}

	//Left
	if ((topLeft.x + botRight.x) / 2 >= p.x) 
	{
		// Indicates topLeftTree
		if ((topLeft.y + botRight.y) / 2 >= p.y) 
		{
			if (topLeftTree == NULL)
			{
				return NULL;
			}
			return topLeftTree->Search(p);
		}
		// Indicates botLeftTree
		else 
		{
			if (botLeftTree == NULL)
				return NULL;
			return botLeftTree->Search(p);
		}
	}
	//Right
	else 
	{
		// Indicates topRightTree
		if ((topLeft.y + botRight.y) / 2 >= p.y) 
		{
			if (topRightTree == NULL)
				return NULL;
			return topRightTree->Search(p);
		}
		// Indicates botRightTree
		else 
		{
			if (botRightTree == NULL)
				return NULL;
			return botRightTree->Search(p);
		}
	}
};

// Check if current quad tree contains the point
bool Quad::InBoundary(Point p)
{
	return (p.x >= topLeft.x && p.x <= botRight.x && p.y >= topLeft.y && p.y <= botRight.y);
}

// Driver program
void TestQuadTree()
{
	Quad center(Point(0, 0), Point(8, 8));
	Node a(Point(1, 1), 1);
	Node b(Point(2, 5), 2);
	Node c(Point(7, 6), 3);
	center.Insert(&a);
	center.Insert(&b);
	center.Insert(&c);
	cout << "Node a: " << center.Search(Point(1, 1))->data << "\n";
	cout << "Node b: " << center.Search(Point(2, 5))->data << "\n";
	cout << "Node c: " << center.Search(Point(7, 6))->data << "\n";
	cout << "Non-existing node: " << center.Search(Point(5, 5));
}

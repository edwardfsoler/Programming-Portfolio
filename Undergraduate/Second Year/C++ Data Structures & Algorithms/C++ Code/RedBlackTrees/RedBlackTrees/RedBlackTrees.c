#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct Nodes Node;

struct Nodes {
	int colour; // 0 = black, 1 = red
	int value;
	Node* left;
	Node* right;
	Node* parent;
};

bool violationsTest(Node* node, Node* root);
Node* getRoot(Node* root);
Node* initialiseRBT();
Node* newNode(int val);
void BSTSearch(Node* root, Node* node);
Node* grandParent(Node* node);
Node* uncle(Node* node);
bool insertNode(Node* root, int val);
void LLRotation(Node* node);
void RRRotation(Node* node);
void RLRotation(Node* node);
void LRRotation(Node* node);
int relativePos(Node* node);
bool redUncle(Node* root, Node* node);
bool search(Node* root, int val);
void empty(Node* node, Node* root);
void preOrder(Node* root);
int numNodes(Node* root, bool reset);
int height(Node* root, int h, bool reset);
bool deleteNode(Node* root, int val);
Node* nodeSearch(Node* root, int val);
Node* sibling(Node* node);
Node* case2B(Node* x, int val);
void removeNode(Node* node);
Node* case2(Node* x, int val);
Node* case2A1(Node* x, int val);
Node* case2A2(Node* x, int val);
Node* case2A3(Node* x, int val);

int main()
{
	int fail;
	int dummy;
	int choice;
	int initialised = 1;
	int val;
	int repeat = 1;
	bool result = false;

	Node* root = NULL;

	do
	{
		//main menu for program
		printf("\n\nRed Black Trees Program\n\n");
		printf("1) Initialise red black tree\n");
		printf("2) Insert an integer\n");
		printf("3) Search for an integer\n");
		printf("4) Delete an integer\n");
		printf("5) Number of nodes\n");
		printf("6) Height of red black tree\n");
		printf("7) Empty red black tree\n");
		printf("8) Print values (Pre Order traversal)\n");
		printf("9) Exit\n\n");

		scanf_s("%d", &choice);
		printf("\n");

		do
		{
			//ensure that input value is within bounds
			if (choice < 1 || choice > 9)
			{
				printf("Invalid input. Please enter a value between 1 and 9\n\n");
				scanf_s("%d", &choice);
				fail = 1;
			}
			else
			{
				fail = 0;
			}
		} while (fail == 1);

		//select function according to menu choice
		switch (choice)
		{
		case 1:
			if (initialised == 0)
			{
				printf("Red Black Tree already initialised\n");
			}
			else
			{
				initialised = 0;
				//initialise RBT
				root = initialiseRBT();
				printf("Red Black Tree initialised\n");
			}
			break;
		case 2:
			if (initialised != 0)
			{
				printf("Red Black Tree must first be initialised");
				break;
			}
			
			printf("Please enter the value you would like to enter: ");
			scanf_s("%d", &val);

			//insert node into RBT
			result = insertNode(root, val);
			//update root pointer
			root = getRoot(root);

			if (result)
			{
				printf("The value %d has been added to the red black tree", val);
			}
			else
			{
				printf("Error adding the value %d to the red black tree", val);
			}

			break;
		case 3:
			if (initialised != 0)
			{
				printf("Red Black Tree must first be initialised");
				break;
			}

			printf("Please enter the value you would like to search for: ");
			scanf_s("%d", &val);

			//search for value withint RBT
			result = search(root, val);

			if (result)
			{
				printf("%d has been found\n", val);
			}
			else
			{
				printf("%d is not in the red black tree\n", val);
			}

			break;
		case 4:
			if (initialised != 0)
			{
				printf("Red Black Tree must first be initialised");
				break;
			}

			int val;
			printf("Please enter the index you would like to delete: ");
			scanf_s("%d", &val);

			//search for and delete given value from RBT
			result = deleteNode(root, val);
			//update root pointer
			root = getRoot(root);

			if (result)
			{
				printf("%d has been successfully removed from the red black tree\n", val);
			}
			else
			{
				printf("Error deleting %d\n", val);
			}

			break;
		case 5:
			if (initialised != 0)
			{
				printf("Red Black Tree must first be initialised");
				break;
			}

			//return the number of nodes within the RBT
			val = numNodes(root, true);

			printf("There currently are %d nodes in the red black tree", val);

			break;
		case 6:
			if (initialised != 0)
			{
				printf("Red Black Tree must first be initialised");
				break;
			}

			//return the height of the RBT
			val = height(root, 0, true);

			printf("The red black tree is currently of height %d", val);

			break;
		case 7:
			if (initialised != 0)
			{
				printf("Red Black Tree must first be initialised");
				break;
			}

			//empty the RBT
			empty(root, root);

			printf("The red black tree has successfully been emptied");

			break;
		case 8:
			if (initialised != 0)
			{
				printf("Red Black Tree must first be initialised");
				break;
			}

			//output tree values in pre order traversal
			preOrder(root);

			break;
		case 9:
			//exit loop
			repeat = 0;
			break;
		}

	} while (repeat == 1);

	return 0;
}

//arguments: pointer to current root
//returns: pointer to new root
//function: ensures that the pointer to the root is kept updated
Node* getRoot(Node* root)
{
	//while current root has a parent, current root = parent of current root
	while (root->parent != NULL)
	{
		root = root->parent;
	}

	//return proper root
	return root;
}

//arguments: pointer to the root node and value to delete
//returns: boolean value regarding success of deletion
//function: finds and deletes the node with the given value, fixing any violations along the way
bool deleteNode(Node* root, int val)
{
	Node* temp;

	//current node
	Node* x = root;
	//sibling of x
	Node* t = sibling(root);
	//parent of x and t
	Node* p = root->parent;

	if (t != NULL)
	{
		//right and left children of t
		Node* r = t->right;
		Node* l = t->left;
	}


	if (root->left != NULL && root->right != NULL)
	{
		//if both the left and right children of the root are black, enter
		if (root->left->colour == 0 && root->right->colour == 0)
		{
			//make root red
			root->colour = 1;

			//perform BST move to assign x to the left or right child
			if (val < root->value)
			{
				x = root->left;
			}
			else
			{
				x = root->right;
			}
			
			temp = case2(x, val);
			
			if (temp == NULL)
			{
				printf("Deletion completed");
				root->colour = 0;
				return true;
			}
			else
			{
				printf("Something went wrong");
				root->colour = 0;
				return false;
			}
		}
		//1 or more children are red, enter
		else
		{
			temp = case2B(root, val);
			if (temp == NULL)
			{
				root->colour = 0;
				printf("Delete Successful");
				root->colour = 0;
				return true;
			}
			else
			{
				printf("Delete failed");
				root->colour = 0;
				return false;
			}
		} 
	}
	return false;
}

//arguments: pointer to node to be deleted
//returns: void
//function: removes given node from tree by arranging pointers
void removeNode(Node* node)
{
	//if node is the root of the tree, enter
	if (node->parent == NULL)
	{
		//if node has no children, change its colour to black and initialise its value
		if (node->left == NULL && node->right == NULL)
		{
			node->colour = 0;
			node->value = 0;
		}
		//otherwise take value of right child and delete right child
		else if (node->right != NULL)
		{
			int temp = node->right->value;
			deleteNode(node, temp);
			node->value = temp;
		}
		//otherwise take value of left child and delete left child
		else
		{
			int temp = node->left->value;
			deleteNode(node, temp);
			node->value = temp;
		}
	}
	else
	{
		//if node has no children, just remove it
		if (node->left == NULL && node->right == NULL)
		{
			if (node->parent->left == node)
			{
				node->parent->left = NULL;
			}
			else
			{
				node->parent->right = NULL;
			}
			free(node);
		}
		//if node has a left child, redirect pointers of parent and left child
		else if (node->left != NULL && node->right == NULL)
		{
			if (node->parent->left == node)
			{
				node->parent->left = node->left;
			}
			else
			{
				node->parent->right = node->left;
			}

			node->left->parent = node->parent;
			free(node);
		}
		//if node has a right child, redirect pointers of parent and right child
		else if (node->right != NULL && node->left == NULL)
		{
			if (node->parent->left == node)
			{
				node->parent->left = node->right;
			}
			else
			{
				node->parent->right = node->right;
			}
			node->right->parent = node->parent;
			free(node);
		}
		//else, node has two children and error has been encountered
		else
		{
			printf("Error deleting node\n\n");
		}
	}
}

//arguments: pointer to node, value to be deleted
//returns: pointer to node
//function: performs swap and traverses the tree
Node* case2A1(Node* x, int val)
{
	//store pointers to x's parent and sibling
	Node* p = x->parent;
	Node* t = sibling(x);

	//toggle colours of all 3 nodes
	p->colour = 0;
	x->colour = 1;
	t->colour = 1;

	//traverse the tree another step
	if (val < x->value)
	{
		x = x->left;
	}
	else
	{
		x = x->right;
	}

	//return back to case 2
	return case2(x, val);
}

//arguments: pointer to node and value to be deleted
//returns: pointer to node
//function: performs swap of nodes and traverses the tree
Node* case2A2(Node* x, int val)
{
	//store pointers to c's parent, sibling and sibling's left child
	Node* p = x->parent;
	Node* t = sibling(x);
	Node* l = t->left;

	//perform RL rotation
	RLRotation(l);

	

	//alter the colours of p and x
	p->colour = 0;
	x->colour = 1;

	//traverse the tree another step
	if (val < x->value)
	{
		x = x->left;
	}
	else
	{
		x = x->right;
	}

	//return back to case 2
	return case2(x, val);
}

//arguments: pointer to current node and value to be deleted
//returns: pointer to node
//function: selects which method to consider based on the children of the sibling node
Node* case2A(Node* x, int val)
{
	Node* t = sibling(x);

	//if sibling is not null and has two children, enter
	if (t != NULL && t->left != NULL && t->right != NULL)
	{
		//if t's children are both black, enter
		if (t->left->colour == 0 && t->right->colour == 0)
		{
			return case2A1(x, val);
		}
		//if t's left child is red, or left and right child are both red, enter
		else if (t->left->colour == 1)
		{
			return case2A2(x, val);
		}
		//only t's right child is red
		else
		{
			return case2A3(x, val);
		}
	}
	else
	{
		printf("Something went wrong");
		return NULL;
	}
}

//arguments: pointer to node and value to be deleted
//returns: pointer to node
//function: performs swaps and traverses the tree
Node* case2A3(Node* x, int val)
{
	//store pointers to x's parent, sibling, sibling's left child and sibling's right child
	Node* p = x->parent;
	Node* t = sibling(x);
	Node* l = t->left;
	Node* r = t->right;

	//perform rotations:

	p->parent = t->parent;
	
	if (p->parent->left == p)
	{
		p->parent->left = t;
	}
	else
	{
		p->parent->right = t;
	}

	t->left = p;
	p->parent = t;

	p->right = l;
	l->parent = p;

	//recolour nodes
	x->colour = 1;
	p->colour = 0;
	t->colour = 1;
	r->colour = 0;

	//traverse the tree another step
	if (val < x->value)
	{
		x = x->left;
	}
	else
	{
		x = x->right;
	}

	//return back to case 2
	return case2(x, val);
}

//argument: pointer to current node and value to be deleted
//returns: pointer to node
//function: decides which case to be used next
Node* case2(Node* x, int val)
{
	//if current node has two black children, then go to case 2A
	if (x->left->colour == 0 && x->right->colour == 0)
	{
		return case2A(x, val);
	}
	//else, go to case 2B
	else
	{
		return case2B(x, val);
	}
}

//arguments: pointer to node and value to delete
//returns: pointer to node
//function: handles specific case of top down tree traversal
Node* case2B(Node* x, int val)
{
	Node* p = x->parent;
	Node* t = sibling(x);

	//if current node is the one to delete, call removeNode() function
	if (val == x->value)
	{
		removeNode(x);
		return NULL;
	}
	else
	{
		//if current node is red, recursively call the same function
		if (x->colour == 1)
		{
			//if value is less than that of current node, traverse left sub-tree
			if (val < x->value)
			{
				x = x->left;
			}
			//else traverse right subtree
			else
			{
				x = x->right;
			}
			return case2B(x, val);
		}
		//else perform rotation of parent and sibling
		else
		{
			p = x->parent;
			t = sibling(x);

			t->parent = p->parent;

			if (p->parent->left == p)
			{
				p->parent->left = t;
			}
			else
			{
				p->parent->right = t;
			}

			Node* temp = t->left;
			t->left = p;
			p->parent = t;
			p->right = temp;
			temp->parent = p;

			t->colour = 0;
			p->colour = 1;

			//move on to case 2
			return case2(x, val);
		}
	}
}

//arguments: node  pointer
//returns pointer to sibling node
//function: find and return pointer to sibling node if present
Node* sibling(Node* node)
{
	//if node has a parent, then find its sibling
	if (node->parent != NULL)
	{
		//if node is the left child, return the right child
		if (node == node->parent->left)
		{
			return node->parent->right;
		}
		//if node is the right child, return the left child
		else
		{
			return node->parent->left;
		}
	}
	return NULL;
}

//arguments: pointer to node and height
//returns: greatest depth at current instance
//function: performs a DFS storing the maximum depth reached at any point
int height(Node* root, int h, bool reset)
{
	//initialise static maximum to store the greatest depth reached
	static int max = 0;

	if (reset)
	{
		max = 0;
	}

	//if current root isn't null, incremenet depth
	if (root != NULL)
	{
		h++;
		//if current depth is greater than the maximum, then update the maximum
		if (h > max)
		{
			max = h;
		}

		//recursively call function on left and right children
		int dummy1 = height(root->left, h, false);
		int dummy2 = height(root->right, h, false);
	}

	return max;
}

//arguments: pointer to root node and value to search for
//returns: pointer to node with the given value or null
//function: performs a binary tree search for the given integer value, returning a pointer to the node or null
Node* nodeSearch(Node* root, int val)
{
	Node* temp = root;

	//loop until integer is found or BTS leads to a null node
	while (true)
	{
		//value found
		if (val == temp->value)
		{
			return temp;
		}

		//value less than current node
		if (val < temp->value)
		{
			//if left child of current node is null, search failed
			if (temp->left == NULL)
			{
				return NULL;
			}
			temp = temp->left;
		}
		//value greater than current node
		else
		{
			//if right child of current node is null, search failed
			if (temp->right == NULL)
			{
				return NULL;
			}
			temp = temp->right;
		}
	}
}

//arguments: pointer to node
//returns: number of nodes visited
//function: recursively traverses the tree in preOrder, storing a count of the number of nodes visited
int numNodes(Node* root, bool reset)
{
	//initialise counter
	static int count = 0;

	if (reset)
	{
		count = 0;
	}

	//if node is not null, incremement count and recursively call on child nodes
	if (root != NULL)
	{
		count++;
		int dummy = numNodes(root->left, false);
		int dummy2 = numNodes(root->right, false);
	}
	return count;
}

//arguments: pointer to node
//returns: void
//functiion: performs pre order traversal of RBT, printing out each visited node
void preOrder(Node* root)
{
	//if current node is not null, then print
	if (root != NULL)
	{
		if (root->colour == 1)
		{
			printf("[%d]", root->value);
		}
		else
		{
			printf("%d ", root->value);
		}

		//call preOrder on the left child
		preOrder(root->left);
		//call preOrder on the right child
		preOrder(root->right);
	}
}

//arguments: pointer to node and pointer to root
//returns: void
//function: recursively deletes all nodes except the root, which is simply reset to its initialised form
void empty(Node* node, Node* root)
{
	//if left child isn't null, recursively call function
	if (node->left != NULL)
	{
		empty(node->left, root);
	}

	//if right child isn't null, recursively call function
	if (node->right != NULL)
	{
		empty(node->right, root);
	}

	//if current node is the root, then just initialise all value
	if (node == root)
	{
		node->value = node->colour = 0;
		node->left = node->right = node->parent = NULL;
	}
	//otherwise free the current node
	else
	{
		free(node);
	}
}

//arguments: pointer to root node and value to search for
//returns: boolean value based on whether the node was found or not
//function: performs a binary tree search for the given integer value, returning true on a successful search, false otherwise
bool search(Node* root, int val)
{
	Node* temp = root;

	//loop until integer is found or BTS leads to a null node
	while (true)
	{
		//value found
		if (val == temp->value)
		{
			return true;
		}

		//value less than current node
		if (val < temp->value)
		{
			//if left child of current node is null, search failed
			if (temp->left == NULL)
			{
				return false;
			}
			temp = temp->left;
		}
		//value greater than current node
		else
		{
			//if right child of current node is null, search failed
			if (temp->right == NULL)
			{
				return false;
			}
			temp = temp->right;
		}
	}
}

//arguments: pointer to root node and value to insert in the RBT
//returns: boolean value signifying success of insertion
//function: creates new node to store value, finds correct position and inserts it in the tree, handling violations
bool insertNode(Node* root, int val)
{
	//if insertion is the first one, just edit value of root node
	if (root->value == 0 && root->left == NULL && root->right == NULL)
	{
		//edit value of root node
		root->value = val;
		return true;
	}
	else
	{
		//create new node for new value
		Node* node = newNode(val);

		//find position to insert new node, and insert it
		BSTSearch(root, node);

		//test for violations
		return violationsTest(node, root);
	}
}

bool violationsTest(Node* node, Node* root)
{
	Node* u = uncle(node);
	Node* gp = grandParent(node);

	if (node == root)
	{
		node->colour = 0;
		return true;
	}

	//if parent node is black, new node is red, so no violations
	if (node->parent->colour == 0)
	{
		return true;
	}
	//if parent is red but uncle is black, perform rotation to fix violation
	else if (node->parent->colour == 1 && (u == NULL || u->colour == 0))
	{
		//find the position of the new node relative to its grandparent
		int pos = relativePos(node);

		//perform the respective rotation
		if (pos == 0)
		{
			LLRotation(node);
			//update root pointer
			root = getRoot(root);

			if (root->colour == 1)
			{
				root->colour = 0;
			}
			return true;
		}
		else if (pos == 1)
		{
			LRRotation(node);
			//update root pointer
			root = getRoot(root);

			if (root->colour == 1)
			{
				root->colour = 0;
			}
			return true;
		}
		else if (pos == 2)
		{
			RLRotation(node);
			//update root pointer
			root = getRoot(root);

			if (root->colour == 1)
			{
				root->colour = 0;
			}
			return true;
		}
		else if (pos == 3)
		{
			RRRotation(node);
			//update root pointer
			root = getRoot(root);

			if (root->colour == 1)
			{
				root->colour = 0;
			}
			return true;
		}
		else
		{
			printf("Something went wrong!\n\n");
			return false;
		}
	}
	//both parent and uncle are red
	else
	{
		return redUncle(root, node);
	}
}

//arguments: pointer to node
//returns: void
//function: handles the red uncle problem
bool redUncle(Node* root, Node* node)
{
	//store pointers to grandparent, uncle and parent node
	Node* gp = grandParent(node);
	Node* u = uncle(node);
	Node* p = node->parent;

	//adjust the colours of the grandparent, uncle and parent to fix violation
	gp->colour = 1;
	u->colour = 0;
	p->colour = 0;

	return violationsTest(gp, root);
}

//arguments: pointer to node to be rotated
//returns: void
//function: performs RL rotation
void RLRotation(Node* node)
{
	//store pointer to parent and grandparent for convenience
	Node* gp = grandParent(node);
	Node* p = node->parent;

	//adjust pointers of nodes to perform RL rotation

	if (gp->parent != NULL)
	{
		if (gp == gp->parent->left)
		{
			gp->parent->left = node;
		}
		else
		{
			gp->parent->right = node;
		}
	}

	node->parent = gp->parent;

	Node* temp = node->left;
	gp->parent = node;
	gp->right = temp;
	if (temp != NULL)
	{
		temp->parent = gp;
	}
	node->left = gp;

	temp = node->right;
	p->parent = node;
	p->left = temp;
	if (temp != NULL)
	{
		temp->parent = p;
	}
	node->right = p;

	//adjust colours of nodes to fix violation
	node->colour = 0;
	p->colour = 1;
	gp->colour = 1;
}


//arguments: pointer to node to be rotated
//returns: void
//function: performs LR rotation
void LRRotation(Node* node)
{
	//store pointer to parent and grandparent for convenience
	Node* gp = grandParent(node);
	Node* p = node->parent;

	//adjust pointers of nodes to perform LR rotation

	if (gp->parent != NULL)
	{
		if (gp == gp->parent->left)
		{
			gp->parent->left = node;
		}
		else
		{
			gp->parent->right = node;
		}
	}

	node->parent = gp->parent;

	Node* temp = node->left;
	p->parent = node;
	p->right = temp;
	if (temp != NULL)
	{
		temp->parent = p;
	}
	node->left = p;

	temp = node->right;
	gp->parent = node;
	gp->left = temp;

	if (temp != NULL)
	{
		temp->parent = gp;
	}

	node->right = gp;

	//adjust colours of nodes to fix violation
	node->colour = 0;
	p->colour = 1;
	gp->colour = 1;
}

//arguments: pointer to node to be rotated
//returns: void
//function: performs RR rotation
void RRRotation(Node* node)
{
	//store pointer to parent and grandparent for convenience
	Node* gp = grandParent(node);
	Node* p = node->parent;

	//adjust pointers of nodes to perform RR rotation

	p->parent = gp->parent;

	if (gp->parent != NULL)
	{
		if (gp == gp->parent->left)
		{
			gp->parent->left = p;
		}
		else
		{
			gp->parent->right = p;
		}
	}

	gp->parent = p;
	gp->right = p->left;
	if (gp->right != NULL)
	{
		gp->right->parent = gp;
	}
	p->left = gp;

	//adjust colours of nodes to fix violation
	p->colour = 0;
	node->colour = 1;
	gp->colour = 1;
}

//arguments: pointer to node to be rotated
//returns: void
//function: performs LL rotation
void LLRotation(Node* node)
{
	//store pointer to parent and grandparent for convenience
	Node* gp = grandParent(node);
	Node* p = node->parent;

	//adjust pointers of nodes to perform LL rotation

	p->parent = gp->parent;

	if (gp->parent != NULL)
	{
		if (gp == gp->parent->left)
		{
			gp->parent->left = p;
		}
		else
		{
			gp->parent->right = p;
		}
	}

	gp->parent = p;
	gp->left = p->right;
	if (gp->left != NULL)
	{
		gp->left->parent = gp;
	}
	p->right = gp;

	//adjust colours of nodes to fix violation
	p->colour = 0;
	node->colour = 1;
	gp->colour = 1;
}

//arguments: pointer to node
//returns integer value regarding the relative position of the node: -1 = ERROR, 0 = LL, 1 = LR, 2 = RL, 3 = RR
//function: deduces relative position of given node from its grandparent
int relativePos(Node* node)
{
	//store pointer to grandparent of node
	Node* temp = grandParent(node);

	//if grandparent does exist, enter
	if (temp != NULL)
	{
		//if parent of node is left child of grandparent, enter
		if (node->parent == temp->left)
		{
			//if node is left child of parent, then enter
			if (node == node->parent->left)
			{
				//LL
				return 0;
			}
			else
			{
				//LR
				return 1;
			}
		}
		else
		{
			//if node is left child of parent, then enter
			if (node == node->parent->left)
			{
				//RL
				return 2;
			}
			else
			{
				//RR
				return 3;
			}
		}
	}
	//ERROR
	else
	{
		return -1;
	}
}

//arguments: pointer to node
//returns: pointer to grandparent of given node
//function: searches for and returns uncle of given node
Node* uncle(Node* node)
{
	//retrieve hgrandparent of given node
	Node* tester = grandParent(node);

	//if grandparent is not null, procede
	if (tester != NULL)
	{
		//if parent is left child of granparent, return right
		if (node->parent == tester->left)
		{
			return tester->right;
		}
		//else return left
		else
		{
			return tester->left;
		}
	}

	//grandparent, and therefore uncle, does not exist
	return NULL;
}

//arguments: pointer to node
//returns: pointer to grandparent of given node, or NULL
//function: tests whether grandparent exists and returns a pointer to it if so
Node* grandParent(Node* node)
{
	//if node has parent, enter
	if (node->parent != NULL)
	{
		//if parent of node has a parent then return a pointer to it
		if (node->parent->parent != NULL)
		{
			return node->parent->parent;
		}
	}
	//else no grandparent exists
	return NULL;
}

//arguments: integer value to store
//returns: pointer to newly created node
//function: creates and initialises new node with given value
Node* newNode(int val)
{
	//declare memory space for new node
	Node* node = (Node*)malloc(sizeof(Node));

	//initialise node values
	node->value = val;
	node->colour = 1;
	node->left = node->right = node->parent = NULL;

	return node;
}

//arguments: pointer to root node and pointer to node to be inserted
//returns: pointer to newly inserted node
//function: performs a BST traversal to find the correct location to insert the new node
void BSTSearch(Node* root, Node* node)
{
	Node* temp = root;

	//loop until position is found
	while (true)
	{
		//if node value is less than the value of the current node, then visit the left chil
		if (node->value < temp->value)
		{
			//if left child is null, then insert new node there
			if (temp->left == NULL)
			{
				//arrange pointers
				temp->left = node;
				node->parent = temp;
				return;
			}
			//else keep traversing
			temp = temp->left;
		}
		//else visit the right child
		else
		{
			//if right chils is null, then insert the new node there
			if (temp->right == NULL)
			{
				//arrange pointers
				temp->right = node;
				node->parent = temp;
				return;
			}
			//else keep traversing
			temp = temp->right;
		}
	}
}

//arguments: none
//returns: Node pointer
//function: initialises RBT by generating empty root node
Node* initialiseRBT()
{
	//declare memory space for new node 
	Node* root = (Node*)malloc(sizeof(Node));

	//initialise node variables
	root->left = root->right = root->parent = NULL;
	root->colour = 0;
	root->value = 0;

	return root;
}
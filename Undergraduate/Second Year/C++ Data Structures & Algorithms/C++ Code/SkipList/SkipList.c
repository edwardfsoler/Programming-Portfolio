#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Nodes Node;

struct Nodes {
	Node * up;
	Node * down;
	Node * left;
	Node * right;
	int value;
	int height;
	int index;
};

void printList(Node* head);
void emptyList(Node* head);
Node* insertIntAtHeight(Node* head, Node* lower, int val, int height);
void insertInt(Node* head, int val);
Node* getHeadHeight(Node* head, int height);
Node* initialiseList(int height);
Node* getHighestHead(Node* head);
void incrementIndices(Node* node);
void decrementIndices(Node* node);
Node* searchNode(Node* head, int val);
Node * deleteNode(Node* head, Node* node);
int delete(Node* head, int val);
Node* findVal(Node* head, int val);
int deleteIndex(Node* head, int val);
Node* searchNodeIndex(Node* head, int val);
Node* findValIndex(Node* head, int val);
int listSize(Node* head);
int getHeight(Node* head);
int findValSteps(Node* head, int val, int steps);
int searchNodeSteps(Node* head, int val);
int searchIndex(Node* head, int val);
int search(Node* head, int val);

int main()
{
	int choice;
	int fail = 1;
	int repeat = 1;
	int val = 0;
	int ans = 0;
	int initialised = 1;
	Node* head = NULL;

	do
	{
		printf("\n\nSkip List Program\n\n");
		printf("1) Initialise skip list\n");
		printf("2) Insert an integer\n");
		printf("3) Search by value\n");
		printf("4) Search by index\n");
		printf("5) Delete by value\n");
		printf("6) Delete by index\n");
		printf("7) Skip list size\n");
		printf("8) Search by value steps\n");
		printf("9) Skip list height\n");
		printf("10) Empty skip list\n");
		printf("11) Print List\n");
		printf("12) Exit\n\n");

		scanf_s("%d", &choice);
		printf("\n");

		do
		{
			if (choice < 1 || choice > 12)
			{
				printf("Invalid input. Please enter a vlue between 1 and 12");
				scanf_s("%d", &choice);
				fail = 1;
			}
			else
			{
				fail = 0;
			}
		} while (fail == 1);

		switch (choice)
		{
		case 1:
			if (initialised == 0)
			{
				printf("Skip List already initialised\n");
			}
			else
			{
				initialised = 0;
				head = initialiseList(1);
				printf("Skip List initialised\n");
			}
			break;
		case 2:
			if (initialised != 0)
			{
				printf("Skip List must first be initialised");
				break;
			}

			printf("Please enter the value you would like to enter: ");
			scanf_s("%d", &val);

			insertInt(head, val);
			break;
		case 3:
			if (initialised != 0)
			{
				printf("Skip List must first be initialised");
				break;
			}

			printf("Please enter the value you would like to search for: ");
			scanf_s("%d", &val);

			int ans = search(head, val);

			if (ans == -1)
			{
				printf("Search failed - %d is not in the list\n", val);
			}
			else
			{
				printf("%d lies at index %d\n",val, ans);
			}
			break;
		case 4:
			if (initialised != 0)
			{
				printf("Skip List must first be initialised");
				break;
			}

			int val;
			printf("Please enter the index you would like to search for: ");
			scanf_s("%d", &val);

			ans = searchIndex(head, val);

			if (ans == -1)
			{
				printf("Search failed - no value lies at index %d\n", val);
			}
			else
			{
				printf("%d lies at index %d\n", ans, val);
			}

			break;
		case 5:
			if (initialised != 0)
			{
				printf("Skip List must first be initialised");
				break;
			}

			printf("Please enter the value you would like to delete from the list: ");
			scanf_s("%d", &val);

			ans = delete(head, val);

			if (ans == 0)
			{
				printf("Delete failed");
			}
			else
			{
				printf("Delete successful");
			}

			break;
		case 6:
			if (initialised != 0)
			{
				printf("Skip List must first be initialised");
				break;
			}

			printf("Please enter the index of the value you would like to delete from the list: ");
			scanf_s("%d", &val);

			ans = deleteIndex(head, val);

			if (ans == 0)
			{
				printf("Delete failed");
			}
			else
			{
				printf("Delete successful");
			}

			break;
		case 7:
			if (initialised != 0)
			{
				printf("Skip List must first be initialised");
				break;
			}

			ans = listSize(head);

			printf("The list is of size %d", ans);

			break;
		case 8:
			if (initialised != 0)
			{
				printf("Skip List must first be initialised");
				break;
			}

			printf("Please enter the value you would like to search for: ");
			scanf_s("%d", &val);

			ans = searchNodeSteps(head,val);

			if (ans == -1)
			{
				printf("Search failed - %d is not in the list\n", val);
			}
			else
			{
				printf("%d found in %d steps\n", val, ans);
			}

			break;
		case 9:
			if (initialised != 0)
			{
				printf("Skip List must first be initialised");
				break;
			}

			ans = getHeight(head);

			printf("The list is of height %d", ans);

			break;
		case 10:
			if (initialised != 0)
			{
				printf("Skip List must first be initialised");
				break;
			}

			emptyList(head);

			printf("The list has been emptied\n");

			break;
		case 11:
			if (initialised != 0)
			{
				printf("Skip List must first be initialised");
				break;
			}

			printList(head);

			break;
		case 12:
			repeat = 0;
			break;
		}

	} while (repeat == 1);

	return 0;
}

//arguments: pointer to head of skip list
//returns: void
//function: prints out the contents of the skip list
void printList(Node* head)
{
	Node* temp = getHighestHead(head);
	Node* temp2;
	int tval;

	//loop through skip list levels starting at the highest
	for (int i = temp->height; i > 0; i--)
	{
		printf("%d | ", i);
		temp2 = temp;
		//traverse current level
		while (temp2->right != NULL)
		{
			//output values on screen
			temp2 = temp2->right;
			tval = temp2->value;
			printf("%d ",tval);
		}
		printf("\n");
		//move to next level
		temp = temp->down;
	}
}

//arguments: head of skip list
//returns: void
//function: free all nodes in the skip list except for the initial skip list node
void emptyList(Node* head)
{
	Node* temp = getHighestHead(head);

	//traverse skip list, starting at the top level freeing all nodes
	for (int i = temp->height; i > 0; i--)
	{
		Node* t = getHeadHeight(head, i);

		//traverse current level
		while (t->right != NULL)
		{
			temp = t->right;

			//set node below to point to null for 'up' pointer
			if (t->down != NULL)
			{
				t->down->up = NULL;
			}

			//free all nodes except for the head of the skip list
			if (t != head)
			{
				free(t);
			}
			
			t = temp;
		}
	}

	//reset all pointers for the head of the skip list
	head->left = head->right = head->up = head->down = NULL;
}

//arguments: head of skipList and value to search for
//returns: -1 if search failed or number of steps otherwise
//function: performs skip list search for given integer value counting the number of steps required
int searchNodeSteps(Node* head, int val)
{
	//get head of the highest levelled list
	Node* highestHead = getHighestHead(head);

	//return pointer to found node
	return findValSteps(highestHead, val, 0);
}

//arguments: pointer to node to start searching from, value to search for and current number of steps
//returns: -1 if search failed or number of steps otherwise
//function: traverse the list at the current level until the node is found, the last node in the list is reached or
//			a node with a greater value than the required is found
int findValSteps(Node* head, int val, int steps)
{
	Node* temp = head;

	//traverse the list
	while ((temp->right != NULL) && (temp->right->value <= val))
	{
		temp = temp->right;
		steps++;

		//if value is found, return index
		if (temp->value == val)
		{
			return steps;
		}
	}

	//if last visited node isn't the value and it doesn't have a lower level, then search failed
	if (temp->down == NULL)
	{
		return -1;
	}
	//otherwise, if a lower level is available, test the lower level
	else
	{
		steps++;
		//recursive call for lower level.
		return findValSteps(temp->down, val, steps);
	}
}

//arguments: head of skip list
//returns: height of skip list
//function: vertically traverses the heads of the skip list while maintaining a counter
int getHeight(Node* head)
{
	///store pointer to the highest head in the list
	Node* temp = getHighestHead(head);

	return temp->height;
}

//arguments: pointer to the head of the skip list
//returns: number of nodes in the skip list
//function: traverse lowest level of the skip list counting the number of nodes encountered
int listSize(Node* head)
{
	Node* temp = head;

	int count = 0;

	//traverse lowest level of skip list until the last node is found
	while (temp->right != NULL)
	{
		temp = temp->right;
		//with each traversel, increment the counter
		count++;
	}

	//return the total count
	return count;
}

//arguments: pointer to head of skip list and index value of node to be deleted
//returns: 0 if deletion failed or 1 otherwise
//function: searches for and deletes all occurences of the given node
int deleteIndex(Node* head, int val)
{
	//store pointer to node to be deleted
	Node* temp = searchNodeIndex(head, val);

	//if search failed then return 0
	if (temp == NULL)
	{
		return 0;
	}
	else
	{

		//run deleteNode() function through all levels until the lowest level is reached
		while (temp != NULL)
		{
			temp = deleteNode(head, temp);
		}

		//deletion succeeded
		return 1;
	}
}

//arguments: head of skipList and index value to search for
//returns: NULL if search failed or pointer to node if it didn't
//function: performs skip list search for given index value
Node* searchNodeIndex(Node* head, int val)
{
	//get head of the highest levelled list
	Node* highestHead = getHighestHead(head);

	//return pointer to found node
	return findValIndex(highestHead, val);
}

//arguments: pointer to node to start searching from and index value to search for
//returns: NULL if search failed or found node
//function: traverse the list at the current level until the node is found, the last node in the list is reached or
//			a node with a greater index value than the required is found
Node* findValIndex(Node* head, int val)
{
	Node* temp = head;

	//traverse the list
	while ((temp->right != NULL) && (temp->right->index <= val))
	{
		temp = temp->right;

		//if value is found, return index
		if (temp->index == val)
		{
			return temp;
		}
	}

	//if last visited node isn't the value and it doesn't have a lower level, then search failed
	if (temp->down == NULL)
	{
		return NULL;
	}
	//otherwise, if a lower level is available, test the lower level
	else
	{
		//recursive call for lower level.
		return findValIndex(temp->down, val);
	}
}

//arguments: pointer to head of skip list and value to be deleted
//returns: 0 if deletion failed or 1 otherwise
//function: searches for and deletes all occurences of the given value
int delete(Node* head, int val)
{
	//store pointer to node to be deleted
	Node* temp = searchNode(head, val);

	//if search failed then return 0
	if (temp == NULL)
	{
		return 0;
	}
	else
	{

		//ren deleteNode() function through all levels until the lowest level is reached
		while (temp != NULL)
		{
			temp = deleteNode(head, temp);
		}

		//deletion succeeded
		return 1;
	}
}

//arguments: pointer to node to be deleted
//returns: pointer to node below current node
//function: removes current node from list at current level
Node * deleteNode(Node* head, Node* node)
{
	Node* tempDown = node->down;
	//arrange pointers of left and right nodes to point to each other
	node->left->right = node->right;
	if (node->right != NULL)
	{
		node->right->left = node->left;
	}

	if (node->height != 1)
	{
		Node* t = getHeadHeight(head, node->height);

		//if head of list at current level has no following elements then delete head
		if (t->right == NULL)
		{
			if (t->height > 1)
			{
				t->down->up = NULL;
				free(t);
			}
		}
	}
	//decremenet the indices of all nodes to the right hand side of the deleted node
	decrementIndices(node);
	free(node);
	//return index of node below deleted node
	return tempDown;
}

//arguments: head of skipList and value to search for
//returns: -1 if search failed or the integer of the found list otherwise
//function: performs skip list search for given integer value
int search(Node* head, int val)
{
	//get head of the highest levelled list
	Node* highestHead = getHighestHead(head);
	//store pointer to found node
	Node* temp = findVal(highestHead, val);

	//if pointer points to NULL then search failed
	if (temp == NULL)
	{
		return -1;
	}
	//otherwise return index of found node
	else
	{
		return temp->index;
	}
}

//arguments: head of skipList and value to search for
//returns: -1 if search failed or the integer of the found list otherwise
//function: performs skip list search for given integer index
int searchIndex(Node* head, int val)
{
	//get head of the highest levelled list
	Node* highestHead = getHighestHead(head);
	//store pointer to found node
	Node* temp = findValIndex(highestHead, val);

	//if pointer points to NULL then search failed
	if (temp == NULL)
	{
		return -1;
	}
	//otherwise return index of found node
	else
	{
		return temp->value;
	}
}

//arguments: head of skipList and value to search for
//returns: NULL if search failed or pointer to node if it didn't
//function: performs skip list search for given integer value
Node* searchNode(Node* head, int val)
{
	//get head of the highest levelled list
	Node* highestHead = getHighestHead(head);
	
	//return pointer to found node
	return findVal(highestHead, val);
}

//arguments: pointer to node to start searching from and value to search for
//returns: NULL if search failed or found node
//function: traverse the list at the current level until the node is found, the last node in the list is reached or
//			a node with a greater value than the required is found
Node* findVal(Node* head, int val)
{
	Node* temp = head;

	//traverse the list
	while ((temp->right != NULL) && (temp->right->value <= val))
	{
		temp = temp->right;

		//if value is found, return index
		if (temp->value == val)
		{
			return temp;
		}
	}

	//if last visited node isn't the value and it doesn't have a lower level, then search failed
	if (temp->down == NULL)
	{
		return NULL;
	}
	//otherwise, if a lower level is available, test the lower level
	else
	{
		//recursive call for lower level.
		return findVal(temp->down, val);
	}
}

Node* getHighestHead(Node* head)
{
	Node* temp = head;

	while (temp->up != NULL)
	{
		temp = temp->up;
	}

	return temp;
}

//arguments: height of list to initialise
//returns: pointer to head of new list 
//function: generates the head of a new list initialised to the given height
Node* initialiseList(int height)
{
	Node* node = (Node*)malloc(sizeof(Node));

	//set all pointers to null
	node->up = node->left = node->right = node->down = NULL;
	//set value to lowers possible int
	node->value = INT_MIN;
	//set height to given height
	node->height = height;
	//set node index
	node->index = -1;

	//return pointer to new node
	return node;
}

//arguments: pointer to newly inserted node
//returns: void
//function: increment index values for all nodes to the right hand side of the given node
void incrementIndices(Node* node)
{
	Node* temp = node;

	//traverse list at this level
	while (temp->right != NULL)
	{
		temp = temp->right;
		//increment index count
		temp->index += 1;
	}
}

//arguments: pointer to newly deleted node
//returns: void
//function: decrement index values for all nodes to the right hand side of the given node
void decrementIndices(Node* node)
{
	Node* temp = node;

	//traverse list at this level
	while (temp->right != NULL)
	{
		temp = temp->right;
		//decrement index count
		temp->index -= 1;
	}
}

//arguments: pointer to head of skipList and value to insert
//returns: void
//function: add integer value to lowest level list and test whether it should be added to higher levels
void insertInt(Node* head, int val)
{
	//call function to insert new value into lowest level list and store a pointer to it
	Node* tempNode = insertIntAtHeight(head, NULL, val, 1);

	//coin toss test
	srand(time(NULL));
	int coin = rand() % 2;
	int h = 1;

	//50/50 chance of being added to the higher level list
	while (coin == 0)
	{
		h += 1;
		//call function to get the height of the list at hieght h+1 (next level)
		Node* tempHead = getHeadHeight(head, h);
		//insert value in the new list and store a pointer to it
		tempNode = insertIntAtHeight(tempHead, tempNode, val, h);
		//perform another coin toss
		coin = rand() % 2;
	}
}

//arguments: pointer to the head of the skipList and the hieght value for the required level
Node* getHeadHeight(Node* head, int height)
{
	//store pointer copy of the head of the skipList
	Node* temp = head;

	//traverse levels until the highest is reached
	while (temp->up != NULL)
	{
		temp = temp->up;

		//if required level is found, return head of that level
		if (temp->height == height)
		{
			return temp;
		}
	}

	int h = temp->height + 1;

	Node* newNode;
	//create a new list at level h
	newNode = initialiseList(h);

	if (newNode->height == height)
	{
		//point head of previous level to head of current level and vice versa
		temp->up = newNode;
		newNode->down = temp;
	}

	//return pointer to head of required level
	return newNode;
}

//arguments: head of current height, pointer to value occurence in lower level, value to insert and height of list
//returns: pointer to newly inserted node element
//function: add new value as node in list of level 'height'
Node* insertIntAtHeight(Node* head, Node* lower, int val, int height)
{
	int success = 0;

	//create new node with value 'val' and height 'height'
	Node* node = (Node*)malloc(sizeof(Node));

	node->value = val;
	node->height = height;

	node->left = node->right = node->up = node->down = NULL;

	//store copy of pointer to head of the current level
	Node* temp = head;

	//traverse list until the end has been found
	while (temp->right != NULL)
	{
		temp = temp->right;

		//if the value of the current node is greater than the node to insert, stop traversing
		if (temp->value > val)
		{
			//arrange pointers of nodes to fit new node with value 'val' into the list on the current level
			temp->left->right = node;
			node->left = temp->left;
			temp->left = node;
			node->right = temp;

			//arrange pointers of current node to point to higher and lower levels.
			node->down = lower;
			if (lower != NULL)
			{
				lower->up = node;
			}
			success = 1;
			break;
		}
	}

	//new node is to be placed at the end of the current list
	if (success == 0)
	{
		//arrange pointers to add new node to the end of the current list
		temp->right = node;
		node->left = temp;
		node->down = lower;
		if (lower != NULL)
		{
			lower->up = node;
		}
	}

	node->index = node->left->index + 1;
	incrementIndices(node);
	//return address of newly added node
	return node;
} 
//JSR
#define TRIE
#ifdef	TRIE

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// Keys will contain characters from English alphabet only. ASCII(65 to 90 and 97 to 122)
// Store A to Z in 0 to 25 and a to z in 26 to 51
#define ALPHABET_SIZE 52

char NamesArr[][50] = {"Jon","Arya","Sansa","Bran","Rickon","Rob","Theon","Eddard"};

// adt to use for the construction of a Trie
struct _tNode
{
	struct _tNode*	children[ALPHABET_SIZE];
	// Storing data of type integer
	int				data;
};
typedef struct _tNode Node;

struct _tRoot
{
	Node*	trie;
	int		numChildren;
};
typedef struct _tRoot Root;


Node* CreateNode();
Root* CreateTrie();

void SaveHelper(Node* pTrie, char* pString, int pDataToSave, int pLevel,int pLength);
void Save(Root* pTrie, char* pString, int pDataToSave);

int SearchHelper(Node* pTrie, char* pString,int pLevel, int pLength);
int SearchTrie(Root* pTrie, char* pString);


void DeleteHelper(Node* pTrie, char* pString, int pLevel, int pLength, int* pDeleteThisNode);
void DeleteFromTrie(Root* pTrie, char* pString);

Root* Run();
void Check();

Node* CreateNode()
{
	Node* newNode	= NULL;

	newNode			= (Node*)malloc(sizeof(Node));
	memset(newNode->children,0,sizeof(Node*)*ALPHABET_SIZE);
	newNode->data	= -1;
	return newNode;
}

Root* CreateTrie()
{
	Root* rootNode			= NULL;
	rootNode				= (Root*)malloc(sizeof(Root));
	rootNode->trie			= CreateNode();
	rootNode->numChildren	= 0;

	return rootNode;
}

void SaveHelper(Node* pTrie, char* pString, int pDataToSave, int pLevel,int pLength)
{
		char	ch;
		int		childIndex;

	ch = pString[pLevel-1];

	// A to Z
	if(ch>=65 && ch<=90)
		childIndex = ch - 65;

	// a to z
	else if(ch>=97 && ch<=122)
		childIndex = ch-97 + 26;
	
	// child does not exist. Allocate memory for it
	if(!pTrie->children[childIndex])
		pTrie->children[childIndex] = CreateNode();
	
	//Reached the end of the string. Hence, store the data here
	if(pLength==pLevel)
	{
		pTrie->children[childIndex]->data	= pDataToSave;
	}
	// else, keep adding to the Trie
	else
	{
		SaveHelper(pTrie->children[childIndex],pString,pDataToSave,pLevel+1,pLength);
	}
}

void Save(Root* pTrie, char* pString, int pDataToSave)
{
	if(pTrie == NULL || pTrie->trie==NULL)
	{
		printf("Can't save in an empty Root\n");
		return;
	}
	else
	{
		SaveHelper(pTrie->trie,pString,pDataToSave,1,strlen(pString));
		pTrie->numChildren++;
	}
}

int	SearchHelper(Node* pTrie, char* pString,int pLevel, int pLength)
{
		char	ch;
		int		childIndex;

	// return if reached the end of the key.
	if(pLevel==pLength)
	{
		return pTrie->data;
	}

	ch = pString[pLevel];

	// A to Z
	if(ch>=65 && ch<=90)
		childIndex = ch - 65;

	// a to z
	else if(ch>=97 && ch<=122)
		childIndex = ch-97 + 26;
	
	// child does not exist. Allocate memory for it
	if(!pTrie->children[childIndex])
	{
		return -1;
	}
	else
	{
		return (SearchHelper(pTrie->children[childIndex],pString,pLevel+1,pLength));
	}

}

int SearchTrie(Root* pTrie, char* pString)
{
	if(pTrie == NULL || pTrie->trie==NULL)
	{
		printf("Can't search an empty Trie\n");
		return;
	}
	else
		return (SearchHelper(pTrie->trie,pString,0,strlen(pString)));
}

Root* Run()
{
		Root*	root	= NULL;
		int		rollNum	= 0;

	root		= CreateTrie();

	for(;rollNum<8;rollNum++)
	{
		Save(root,NamesArr[rollNum],rollNum+1);
	}
	return root;
}

void Check()
{
		int searchResult,i;
		Root* root = NULL;

	root = Run();

	searchResult = SearchTrie(root,"Robert");

	if(searchResult<0) printf("%s doesnot exist\n","Robert");
	else printf("%s exists\n","Robert");

	for(i=0;i<8;i++)
	{
		searchResult = SearchTrie(root,NamesArr[i]);

		if(searchResult<0) printf("%s doesnot exist\n",NamesArr[i]);
		else printf("%s exists\n",NamesArr[i]);
	}

	searchResult = SearchTrie(root,"Cersei");

	if(searchResult<0) printf("%s doesnot exist\n","Cersei");
	else printf("%s exists\n","Cersei");
}

/* Delete the pString from the the trie.
// Testcases: 
	1. The key pString is independent; Not a part of any other key. Delete all the nodes
	2. The key pString shares a prefix with some other key. Delete the nodes upto where there is branching
	3. The whole key itself is a prefix to some other key. Initialize the data as -1. Dont delete
	4. Some other key is prefix of this key. Delete all nodes upto where the prefix key ends.
	5. Key does not exist. Show error.
*/
void DeleteHelper(Node* pTrie, char* pString, int pLevel, int pLength, int* pDeleteThisNode)
{
		char	ch;
		int		childIndex,i, moreSubChildrenExist = 0;

	// leaf node(last node of the key)
	if(pLevel==pLength)
	{
		// check; for no other key should originate from here
		for(i=0;i<ALPHABET_SIZE;i++)
		{
			if(pTrie->children[i])
			{
				*pDeleteThisNode = 0;
				moreSubChildrenExist = 1;
				break;
			}
		}
		// can't delete
		if(moreSubChildrenExist)
		{
			pTrie->data			= -1;
			*pDeleteThisNode	= 0;
			return ;
		}
		else
		{
			free(pTrie);
			pTrie				= NULL;
			*pDeleteThisNode	= 1;
		}
	}
	// not the leaf node
	else
	{
		ch = pString[pLevel];

		// A to Z
		if(ch>=65 && ch<=90)
			childIndex = ch - 65;
	
		// a to z
		else if(ch>=97 && ch<=122)
			childIndex = ch-97 + 26;
		
		// child does not exist. Allocate memory for it
		if(!pTrie->children[childIndex])
		{
			printf("The key %s does not exist and hence can't delete.\n",pString);
			*pDeleteThisNode = 0;
			return ;
		}
		// call this function with the next key
		else
		{
			DeleteHelper(pTrie->children[childIndex],pString,pLevel+1,pLength,pDeleteThisNode);
		}

		// told to delete this node. the child node has been deleted. Set the pointer pTrie->children[childIndex] to NULL
		if(*pDeleteThisNode)
		{
			pTrie->children[childIndex] = NULL;
			// check for no other key should originate from here
			for(i=0;i<ALPHABET_SIZE;i++)
			{
				if(pTrie->children[i])
				{
					*pDeleteThisNode		= 0;
					moreSubChildrenExist	= 1;
					break;
				}
			}

			// some other key is a child
			if(moreSubChildrenExist)
			{
				// communicate this back to the caller function
				*pDeleteThisNode	= 0;
				return;
			}
			// delete this node
			else
			{
				free(pTrie);
				pTrie = NULL;
				// although the value of *pDeleteThisNode is 1 here; still again setting it just
				// for the sake of it.
				*pDeleteThisNode = 1;
			}
		}
		// told not to delete this node
		else
		{
			// nothing to do
			;
		}
	}
}

// Delete the string pString from the Trie
void DeleteFromTrie(Root* pTrie, char* pString)
{
	// Decrease the count pTrie->numChildren
	//pTrie->numChildren--;
	int deleteThisNode=1;
	DeleteHelper(pTrie->trie,pString,0,strlen(pString),&deleteThisNode);
}

int main()
{
	Root* root	= NULL;
	root		= CreateTrie();

	Save(root,"Jo",1);
	Save(root,"Jon",1);
	Save(root,"Tim",1);
	Save(root,"Jony",1);
	Save(root,"Job",1);

	DeleteFromTrie(root,"Tim");

	DeleteFromTrie(root,"Jon");
	DeleteFromTrie(root,"Jo");
	
	DeleteFromTrie(root,"Jony");
	DeleteFromTrie(root,"Jones");
	return 0;
}
#endif
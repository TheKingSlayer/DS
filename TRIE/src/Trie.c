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


Node*	CreateNode();
Root*	CreateTrie();

void	SaveHelper(Node* pTrie, char* pString, int pDataToSave, int pLevel,int pLength);
void	Save(Node* pTrie, char* pString, int pDataToSave);

int		SearchTrie(Node* pTrie, char* pString);
int		SearchHelper(Node* pTrie, char* pString,int pLevel, int pLength);

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

int main()
{
	Check();
	return 0;
}
#endif
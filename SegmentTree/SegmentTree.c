#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int* arr;
int* memArray;

// building the segment tree
void initialize(int memArrIndex, int beg, int end, int* arr, int* memArray)
{
	// the interval is a single index
	if(beg==end)
	{
		memArray[memArrIndex] = beg;
	}
	else
	// initialize for the left and right children
	{
		// left child
		initialize((memArrIndex*2) +1, beg, (beg+end)/2 , arr, memArray);
		// right child
		initialize((memArrIndex*2) +2, (beg+end)/2 +1 ,end , arr, memArray);

		// look for the larger element at the index
		if(arr[memArray[(memArrIndex*2)+1]] >= arr[memArray[(memArrIndex*2)+2]])
			memArray[memArrIndex] = memArray[(memArrIndex*2)+1];
		else
			memArray[memArrIndex] = memArray[(memArrIndex*2)+2];
	}
}

// return the index of the element with min. value
int query(int memArrIndex, int beg, int end, int qStart, int qEnd, int* arr, int* memArray)
{
	int leftIndex , rightIndex;

	// out of range(disjoint set)
	if(qStart>end || qEnd<beg) 
		return -1;

	// range is a sub-set of the query
	if((beg>=qStart) && (end<=qEnd))
		return memArray[memArrIndex];

	leftIndex	= query(memArrIndex*2 +1,beg,(beg+end)/2,qStart,qEnd,arr,memArray);
	rightIndex	= query(memArrIndex*2 +2,(beg+end)/2 +1,end,qStart,qEnd,arr,memArray);

	if(leftIndex==-1) 
		return rightIndex;

	if(rightIndex==-1) 
		return leftIndex;

	if(arr[leftIndex] >= arr[rightIndex])
		return leftIndex;
	else
		return rightIndex;
}

int update(int memArrIndex, int beg, int end, int indexToUpdate, int newValue, int* arr, int* memArray)
{
		int leftIndex,rightIndex;

	// index out of range
	if(indexToUpdate<beg || indexToUpdate>end)
		return -1;

	if( beg==end )
	{
		arr[indexToUpdate]	= newValue;
		return beg;
	}

	leftIndex	= update(memArrIndex*2+1,beg,(beg+end)/2,indexToUpdate,newValue,arr,memArray);
	rightIndex	= update(memArrIndex*2+2,(beg+end)/2 +1,end,indexToUpdate,newValue,arr,memArray);

	if(leftIndex==-1) 
	{
		memArray[memArrIndex] = rightIndex;
		return rightIndex;
	}
	if(rightIndex==-1) 
	{
		memArray[memArrIndex] = leftIndex;
		return leftIndex;
	}

	if(arr[leftIndex] >= arr[rightIndex])
	{
		memArray[memArrIndex] = leftIndex;
		return leftIndex;
	}
	else
	{
		memArray[memArrIndex] = rightIndex;
		return rightIndex;
	}

}

int main()
{
		int spaceForMemArr;
		int sizeOfArr;
		int minElementIndex;

	// allocate array of size 2*2[logN]+1
	sizeOfArr		= sizeof(arr)/sizeof(int);
	spaceForMemArr	= (sizeOfArr)*(log((double)sizeOfArr) / log((double)2)) ;

	memArray			= (int*)malloc(sizeof(int)*(spaceForMemArr+1));
	memset(memArray,-1,sizeof(int)*(spaceForMemArr+1));

	initialize(0,0,sizeOfArr-1,arr,memArray);

	minElementIndex = query(0,0,7,3,7,arr,memArray);
	printf("Minimum element in range 3,7 is %d",arr[minElementIndex]);

	minElementIndex = query(0,0,7,2,6,arr,memArray);
	printf("Minimum element in range 2,6 is %d",arr[minElementIndex]);

	update(0,0,7,0,80,arr,memArray);

	return 0;
}
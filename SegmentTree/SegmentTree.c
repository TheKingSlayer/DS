#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<time.h>

#define NumElements 90000
#define NumQueries 90000

// Main array to store the elements.
int* mainArray;
// Array for the segment tree.
int* segmentTreeArray;

// Building the segment tree. 
void initialize(int memArrIndex, int beg, int end)
{
	// the interval is a single index
	if(beg==end)
	{
		segmentTreeArray[memArrIndex] = beg;
	}
	else
	// initialize for the left and right children
	{
		// left child
		initialize((memArrIndex*2) +1, beg, (beg+end)/2);
		// right child
		initialize((memArrIndex*2) +2, (beg+end)/2 +1 ,end);

		// look for the larger element at the index
		if(mainArray[segmentTreeArray[(memArrIndex*2)+1]] >= mainArray[segmentTreeArray[(memArrIndex*2)+2]])
			segmentTreeArray[memArrIndex] = segmentTreeArray[(memArrIndex*2)+1];
		else
			segmentTreeArray[memArrIndex] = segmentTreeArray[(memArrIndex*2)+2];
	}
}

// Querying the segment tree. Return the index of the element with
// max value in the range [qStart,qEnd]
int query(int memArrIndex, int beg, int end, int qStart, int qEnd)
{
	int leftIndex , rightIndex;

	// out of range(disjoint set)
	if(qStart>end || qEnd<beg) 
		return -1;

	// range is a sub-set of the query
	if((beg>=qStart) && (end<=qEnd))
		return segmentTreeArray[memArrIndex];

	leftIndex	= query(memArrIndex*2 +1,beg,(beg+end)/2,qStart,qEnd);
	rightIndex	= query(memArrIndex*2 +2,(beg+end)/2 +1,end,qStart,qEnd);

	if(leftIndex==-1) 
		return rightIndex;

	if(rightIndex==-1) 
		return leftIndex;

	if(mainArray[leftIndex] >= mainArray[rightIndex])
		return leftIndex;
	else
		return rightIndex;
}

// Updating the array at index <indexToUpdate> with value <newValue>. 
// Update the non-leaf(parent) nodes in bottom up recursive manner acc. to
// the updated array
int update(int memArrIndex, int beg, int end, int indexToUpdate, int newValue)
{
		int leftIndex,rightIndex;

	// index out of range
	if(indexToUpdate<beg || indexToUpdate>end)
		return -1;

	if( beg==end )
	{
		mainArray[indexToUpdate]	= newValue;
		return beg;
	}

	leftIndex	= update(memArrIndex*2+1,beg,(beg+end)/2,indexToUpdate,newValue);
	rightIndex	= update(memArrIndex*2+2,(beg+end)/2 +1,end,indexToUpdate,newValue);

	if(leftIndex==-1) 
	{
		segmentTreeArray[memArrIndex] = rightIndex;
		return rightIndex;
	}
	if(rightIndex==-1) 
	{
		segmentTreeArray[memArrIndex] = leftIndex;
		return leftIndex;
	}

	if(mainArray[leftIndex] >= mainArray[rightIndex])
	{
		segmentTreeArray[memArrIndex] = leftIndex;
		return leftIndex;
	}
	else
	{
		segmentTreeArray[memArrIndex] = rightIndex;
		return rightIndex;
	}

}

void fillArr()
{
		int i;

	for(i=0 ; i<NumElements ; i++)
	{
		// fill the array 'arr' with random numbers 
		mainArray[i] = NumElements-i;
	}
}

void querySegmentTreeApproach()
{
		int qStart,qEnd,itr,maxElementIndex;

	for(itr=0 ; itr<NumQueries ; itr++)
	{
		qEnd	= NumElements - itr -1;
		qStart	= itr;
		
		if(qStart<qEnd)
		{
			maxElementIndex = query(0,0,NumElements-1,qStart,qEnd);
//			printf("Max element in range [%d,%d] = %d\n",qStart,qEnd,mainArray[maxElementIndex]);
		}
		else if(qStart>qEnd)
		{
			maxElementIndex = query(0,0,NumElements-1,qEnd,qStart);
//			printf("Max element in range [%d,%d] = %d\n",qEnd,qStart,mainArray[maxElementIndex]);
		}
	}
}

int queryThroughIteration(int qStart, int qEnd)
{
		int i, maxElement, maxElementIndex;

	maxElement = -1;

	for(i=qStart ; i<=qEnd ; i++)
	{
		if(mainArray[i]>maxElement)
		{
			maxElement		= mainArray[i];
			maxElementIndex = i;
		}
	}
	return maxElementIndex;
}

void querySimpleApproach()
{
		int qStart,qEnd,itr,maxElementIndex;

	for(itr=0 ; itr<NumQueries ; itr++)
	{
		qEnd	= NumElements - itr -1;
		qStart	= itr;
		
		if(qStart<qEnd)
		{
			maxElementIndex = queryThroughIteration(qStart,qEnd);
//			printf("Max element in range [%d,%d] = %d\n",qStart,qEnd,mainArray[maxElementIndex]);
		}
		else if(qStart>qEnd)
		{
			maxElementIndex = queryThroughIteration(qEnd,qStart);
//			printf("Max element in range [%d,%d] = %d\n",qEnd,qStart,mainArray[maxElementIndex]);
		}
	}

}


int main()
{
		int spaceForSegTreeArr, minElementIndex;
		time_t startTime, endTime;
		double seconds;
	mainArray	= (int*)malloc(sizeof(int)*NumElements);

	// fill the array upto NumElements with random numbers
	fillArr();

	// allocate array of size 
	spaceForSegTreeArr			= NumElements *(log((double)NumElements) / log((double)2));
	
	segmentTreeArray			= (int*)malloc(sizeof(int)*spaceForSegTreeArr);
	
	// initialize the segment tree array(build)
	initialize(0,0,NumElements-1);

	time(&startTime);
	// query <NumQueries> queries with the segment tree apporach
	querySegmentTreeApproach();
	time(&endTime);

	seconds = difftime(endTime,startTime);
	printf("Time taken for querying through 'Segment Tree' approach: %.2f seconds.\n",seconds);


	time(&startTime);
	// query <NumQueries> queries with the simple iteration apporach
	querySimpleApproach();
	time(&endTime);
	seconds = difftime(endTime,startTime);

	printf("Time taken for querying through 'Naive' approach: %.2f seconds.\n",seconds);

	return 0;
}
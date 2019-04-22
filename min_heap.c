#include <stdio.h>
#include <stdlib.h>
#define n 10 //capacity of heap array
#define INF 1000
int heap_size=0;
int heap_arr[n]={1000};
int parent(int i)
{
	//return (i+1)/2-1;
	if(i==0)
	return 0;
	else
	  return (i-1)/2;	
}
int left(int i)
{
	return(2*i+1);
}
int right(int i)
{
	return(2*i+2);
}
int get_min()
{
	return heap_arr[0];
}

void swap(int *a,int *b)
{
	int temp;
	temp=*a;
	*a=*b;
	*b=temp;
}

void min_heapify(int i)
{
	int l=left(i);
	int r=right(i);
	int least=i;

	if(l<heap_size&&heap_arr[l]<heap_arr[i])
		least=l;
	if(r<heap_size&&heap_arr[r]<heap_arr[least])
		least=r;
	if(least!=i)
	{
		swap(&heap_arr[i],&heap_arr[least]);
		min_heapify(least);
	}
}	

void insert(int k)
{
	if(heap_size==n)
	{
		printf("\n overflow : array is full");
		return;
	}

	heap_size++;
	int i;
	i=heap_size-1;
	heap_arr[i]=k;

	while((heap_arr[parent(i)]>heap_arr[i])&&i!=0)
	{
		swap(&heap_arr[i],&heap_arr[parent(i)]);
		i=parent(i);
	}
	
}

int extract_min()
{
	if(heap_size<=0)
		return INF;
	if(heap_size==1)
	{
		heap_size--;
		return heap_arr[0];
	}

	int root=heap_arr[0];
	heap_arr[0]=heap_arr[heap_size-1];
	heap_size--;
	min_heapify(0);

	return root;
}

void display(int x,int level)
{
	int i;
	
	if(x<heap_size)
    {
    	
        display(right(x),level+1);
        
        printf("\n");
      
        for(i=0;i<level;i++)
            printf("    ");
        printf("%d",heap_arr[x]);

        display(left(x),level+1);
    }
    
}

int main()
{
    int i,ch;

    printf("\n        MIN-HEAP DATA STRUCTURE  \n");
    

    while(1)
    {
    	printf("\n1.INSERT");
	    printf("\n2.FIND MINIMUM");
	    printf("\n3.DELETE MINIMUM");
	    printf("\n4.DISPLAY");
	    printf("\n5.EXIT\n");
	    printf("\nEnter choice : ");
	    scanf("%d",&ch);
    	
    	switch(ch)
    	{
    		case 1: printf("\nEnter element to insert : ");
    				scanf("%d",&i);
    				insert(i);
    				printf("\nINSERT SUCCESSFULL\n\n");
    				break;

    		case 2: i=get_min();
    				printf("\nMINIMUM ELEMENT IS : %d ",i);
    				break;

    		case 3: i=extract_min();
    				printf("\nMINIMUM ELEMENT %d IS DELETED",i);
    				break;

    		case 4: printf("\nMIN HEAP :-\n");
    				display(0,0);
    				printf("\n\n\n");
    				break;		

    		case 5: exit(1);

    		default : printf("\nWrong choice !! \n\n");
    					break;				
    	}
    }



	return 0;
}
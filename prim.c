#include <stdio.h>
#include <stdlib.h>
#define inf 0x7fffffff  // 2,147,483,647 

struct node  //node of adjacency list
{
	int destination,weight;
	struct node* next;
};

struct adjacency_list  //adjacency list
{
	struct node* root;
};

struct graph  //array of adjacencey list : main graph
{
	int vertices;
	struct adjacency_list* arr;
};

struct minheap_node
{
	int vertex,data;
};

struct minheap
{
	int heap_size,cap,*position;
	struct minheap_node **arr;
};


struct graph* create_graph(int v)
{	
	int i;
	struct graph* g=(struct graph*)malloc(sizeof(struct node));
	g->vertices=v;

	g->arr=(struct adjacency_list*)malloc(v*sizeof(struct adjacency_list));

	for(i=0;i<v;i++)
		g->arr[i].root=NULL;

	return g;
}

struct minheap* create_minheap(int capacity)
{
	struct minheap* mh=(struct minheap*)malloc(sizeof(struct minheap_node));
	mh->cap=capacity;
	mh->heap_size=0;
	mh->arr=(struct minheap_node**)malloc(capacity*sizeof(struct minheap_node*));
	mh->position=(int*)malloc(sizeof(int)*capacity);

	return mh;	
}

void heapify(struct minheap* mh,int i) //heapify for minheap
{
	int left,right,least;
	struct minheap_node* temp;
	least=i;
	left=2*i+1;
	right=2*i+2;

	if(left<mh->heap_size&&mh->arr[left]->data<mh->arr[least]->data)
		least=left;

	if(right<mh->heap_size&&mh->arr[right]->data<mh->arr[least]->data)
		least=right;

	if(i!=least)
	{
		struct minheap_node *least_node=mh->arr[least];
		struct minheap_node *i_node=mh->arr[i];

		mh->position[least_node->vertex]=i;
		mh->position[i_node->vertex]=least;

		temp=mh->arr[least];
		mh->arr[least]=mh->arr[i];
		mh->arr[i]=temp;

		heapify(mh,least);
	}
}	

struct minheap_node* extract_min(struct minheap* mh)  //extracting minimum value from minheap
{
	if(mh->heap_size==0)
		return NULL;

	struct minheap_node* root=mh->arr[0];

	struct minheap_node* last_node=mh->arr[mh->heap_size-1];

	mh->arr[0]=last_node;

	mh->position[root->vertex]=mh->heap_size-1;
	mh->position[last_node->vertex]=0;
	
	mh->heap_size--;
	heapify(mh,0);

	return root;
}

void decrease(struct minheap* mh,int v,int val) //decrease value of a given vertex
{
	int i=mh->position[v];
	struct minheap_node* temp;

	mh->arr[i]->data=val;

	while(i!=0&&mh->arr[i]->data<mh->arr[(i-1)/2]->data)
	{
		mh->position[mh->arr[i]->vertex]=((i-1)/2);
		mh->position[mh->arr[(i-1)/2]->vertex]=i;

		temp=mh->arr[i];
		mh->arr[i]=mh->arr[(i-1)/2];
		mh->arr[(i-1)/2]=temp;

		i=((i-1)/2);
	
	}
}

struct node* newnode(int destination,int weight) // creating new node
{
	struct node* n=(struct node*)malloc(sizeof(struct node));
	n->destination=destination;
	n->weight=weight;
	n->next=NULL;
	return n;

}

struct minheap_node* minheap_newnode(int v,int data)
{
	struct minheap_node* mhn=(struct minheap_node*)malloc(sizeof(struct minheap_node));
	mhn->vertex=v;
	mhn->data=data;

	return mhn;
}

void prim(struct graph* g)  //prims function
{
	int i,parent[g->vertices],data[g->vertices];

	struct minheap* mh=create_minheap(g->vertices); //creating minheap
	

	for(i=1;i<g->vertices;i++)  //initialising minheap with every vertices
	{
		parent[i]=-1;
		data[i]=inf;   //initialisng every value to infinity except 0th value
		mh->arr[i]=minheap_newnode(i,data[i]);
		
		mh->position[i]=i;
	}


	data[0]=0; //initialising 0th as 0 such that it gets extracted first
	mh->arr[0]=minheap_newnode(0,data[0]);
	mh->position[0]=0;

	mh->heap_size=g->vertices;

	while(mh->heap_size!=0)
	{
		struct minheap_node* mhn=extract_min(mh);
		int j=mhn->vertex;

		struct node* counter=g->arr[j].root; 
		while(counter!=NULL)  ////travel to all extracted vertex and updates their values
		{
			i=counter->destination;
			if(mh->position[i]<mh->heap_size&&counter->weight<data[i]) //if i is not included in mst and weight of i and j is less than value of i,then update i and its parent
			{
				data[i]=counter->weight;
				parent[i]=j;
				decrease(mh,i,data[i]);
			}
			counter=counter->next;
		}
	}

	printf("\nMST\n");
	for(i=1;i<g->vertices;i++)
		printf("%d ---- %d\n",parent[i],i);

}

void add_edge(struct graph* g,int source,int destination,int weight) //add undirected graph edge
{	
	struct node* n=newnode(destination,weight);
	n->next=g->arr[source].root;
	g->arr[source].root=n;

	n=newnode(source,weight);
	n->next=g->arr[destination].root;
	g->arr[destination].root=n;
}

int main()
{
	int v,source,destination,weight,ch;
	
	printf("\n            FINDING MINIMUM SPANNING TREE USING PRIM'S ALGO!!\n");
	printf("\nEnter the number of vertices in the undirected graph:-");
	scanf("%d",&v);
	
	struct graph* g=create_graph(v);  //creating graph

	printf("\n(Source,Destination,Weight)  \n");
    ch=1;
	while(ch)
	{
			printf("\nInput Edge :- ");
			scanf("%d %d %d",&source,&destination,&weight);
			add_edge(g,source,destination,weight);
			printf("For exit press 0!!!\nTo continue press 1 !!!  \nchoice:-");
			scanf("%d",&ch);
	}
	
	prim(g);

	return 0;
}
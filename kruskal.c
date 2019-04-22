 #include <stdio.h>
#include <stdlib.h>

struct node  // node for edge
{
	int source,destination,weight;
};

struct graph  
{
	int vertices,edges;
	struct node* edge;

};

struct bio
{
	int parent,rank;
};

struct graph* create_graph(int e,int v)
{
	struct graph* g=(struct graph*)malloc(sizeof(struct graph));
	g->edges=e;
	g->vertices=v;


	return g;
}

void heapify(struct node* a,int n,int k)  //minheap
{   
	int large,left,right;
	struct node temp;
    large=k;
    left=2*k+1;
    right=2*k+2;


    if(a[large].weight<a[left].weight&&left<n)
    	large=left;

    if(a[large].weight<a[right].weight && right<n)
    	large = right;

    if(!(large==k))
    {
    	temp=a[large];
    	a[large]=a[k];
    	a[k]=temp;

    	heapify(a,n,large);
    }
}    

void heapsort(struct node* a,int n)  // for sorting edges
{
	int k;
	struct node temp;
	for(k=(n/2-1);k>=0;k--)
		heapify(a,n,k);    

	for(k=n-1;k>=0;k--)
	{
		temp=a[0];
        a[0]=a[k];
        a[k]=temp;

        heapify(a,k,0);
	}
}

int find(struct bio s[],int i)  // find element from set
{
		
	if(s[i].parent!=i)
		s[i].parent=find(s,s[i].parent);  //path compression

	return s[i].parent;
}

void Union(struct bio s[],int a,int b)  // union of to set
{
	int a_root=find(s,a);
	int b_root=find(s,b);

	if(s[a_root].rank<s[b_root].rank)  //higher ranked will be parent
		s[a_root].parent=b_root;
	else if(s[a_root].rank>s[b_root].rank)
		s[b_root].parent=a_root;
	else //if both ranked equal then any way it doesn't matter
	{  
		s[b_root].parent=a_root;
		s[a_root].rank++;
	}
}

void kruskal(struct graph* g)  // fuction to implement kruskal
{
    int v=g->vertices;
    struct node mst[v];
    int i=0,j=0,k,total=0;

  	heapsort(g->edge,g->edges);

  	for(k=0;k<g->edges;k++)
  	printf("%d \n",g->edge[k].weight);


	struct bio *s=(struct bio*)malloc(v*sizeof(struct bio));
	

	for(k=0;k<v;k++)
	{
		s[k].parent=k;
		s[k].rank=0;
	}

	while(j<(v-1))
	{

		struct node next_node=g->edge[i++];
		int a=find(s,next_node.source);
		int b=find(s,next_node.destination);

		if(a!=b)
		{
			mst[j++]=next_node;
			Union(s,a,b);
		}
		
	}

	printf("\nMST\n");
    for (i=0;i<(v-1);i++)
    {
        printf("%d --- %d = %d\n",mst[i].source,mst[i].destination,mst[i].weight);
        total=total+mst[i].weight;
    }

    printf("Total Weight = %d",total);
    return;


}


int main()
{
	int v,e,i;
	
	printf("\n           FINDING MINIMUM SPANNING TREE USING KRUSKAL'S ALGO !!!\n");
	printf("\nEnter number of vertices :- ");
	scanf("%d",&v);
	printf("\nEnter number of edges :- ");
	scanf("%d",&e);

	struct graph* g=(struct graph*)malloc(sizeof(struct graph));
	g->edge=(struct node*)malloc(e*sizeof(struct node));
	g->edges=e;
	g->vertices=v;

	printf("\n(Source,Destination,Weight)\n");
	for(i=0;i<e;i++)
	{
		printf("\nInput edge :- ");
		scanf("%d %d %d",&(g->edge[i].source),&(g->edge[i].destination),&(g->edge[i].weight));

	}
	
	kruskal(g);
	
	return 0;
}

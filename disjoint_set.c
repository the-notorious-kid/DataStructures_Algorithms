#include <stdio.h>
#include <stdlib.h>

struct bio
{
	int parent,rank,data;
};

int find(struct bio *s,int i)  // find element from set  
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

void display(struct bio s[],int n)
{
	int i;
	printf("\nelement value   --   index  --  root  ");
	for(i=0;i<n;i++)
	{
		printf("\n         %d      --    %d     --   %d\n",s[i].data,i,s[i].parent);

	}
}

int main()
{
	int i,n,a,v1,v2;
	char ch;
	
	printf("\n                  DISJOINT SET DATA STRUCTURE!!!\n");
	printf("\nEnter number of singleton sets : ");
	scanf("%d",&n);

	
	struct bio *s=(struct bio*)malloc(n*sizeof(struct bio));
	
	for(i=0;i<n;i++)
	{
		printf("\nEnter %d element : ",i+1);
		scanf("%d",&s[i].data);
		s[i].parent=i;
		s[i].rank=0;
	}

	while(1){
		printf("\n\n1.UNION\n");
		printf("\n2.FIND\n");
		printf("\n3.DISPLAY\n");
		printf("\n4.EXIT\n");
		printf("\nEnter choice : ");
		scanf("%d",&a);

		switch(a)
		{
			case 1: printf("\nEnter 2 elements for union : ");
					scanf("%d %d",&v1,&v2);
					Union(s,v1,v2);
					printf("\nUNION SUCCESSFUL\n\n");
					break;

			case 2: printf("\nEnter element you want to find : ");
					scanf("%d",&v1);
					v2=find(s,v1);
					printf("ROOT of given element is : %d",v2);
					break;
			case 3: printf("\nSets with repective roots");
					display(s,n);
					break;
			case 4: exit(1);
			
			default : printf("\nWrong Choice");
					  break;		
		}
		
	  }

	  return 0;
}
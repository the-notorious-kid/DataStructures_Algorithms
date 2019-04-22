
#include<stdio.h>
#include<stdlib.h>

#define ORDER 5	/*order of B tree*/
#define MAX (ORDER-1)	/*maximum number of allowed keys in a node*/


//if ORDER is even then replace min with (((ORDER)/2)-1)  

#define MIN (((ORDER-1)/2))	/*minimum number of allowed keys in a node except root*/

struct node
{
	int count,data[MAX+1];
	struct node *child[MAX+1];
};

int search_node(int val,struct node *p, int *n)
{
	if(val<p->data[1])
	{
		*n=0;
		return 0;
	}
	
	*n=p->count;
	
	while((val<p->data[*n])&&*n>1) //to decrease the counter till we find our interest area
		(*n)--;
	
	if(val==p->data[*n])
		return 1;
	else
		return 0;
}

struct node *search(int val,struct node *p,int *n)
{
	if(p==NULL)  //base Case 1 : if key not found 
		return NULL;
	else if(search_node(val,p,n))	// base Case 2 : if key found in node p  
		return p;
	else         //recursive case : Search in node p->child[*n] 
		return search(val,p->child[*n],n); 
}

void display(struct node *p,int spaces)
{
	if(p!=NULL)
	{
		int i;
		for(i=1;i<=spaces;i++)
			printf(" ---->");
		
		for(i=1;i<=p->count;i++)
			printf("%d ",p->data[i]);
		
		printf("\n");
		
		for(i=0;i<=p->count;i++)
			display(p->child[i],spaces+1);
	}
}

void inorder(struct node *p)
{
	int i;
	if(p!=NULL)
	{
		for(i=0;i<p->count;i++)
		{
			inorder(p->child[i]);
			printf("%d\t",p->data[i+1]);
		}
		inorder(p->child[i]);
	}
}

void split(int i,struct node *i_child,struct node *p,int n,int *upper_val,struct node **newnode)
{
	int j,k,last_val;
	struct node *last_child;
	int d=(ORDER+1)/2;  //if ORDER is even then d=(ORDER)/2 

	if(n==MAX)
	{
		last_val=i;
		last_child=i_child;
	}
	else
	{
		last_val=p->data[MAX];
		last_child=p->child[MAX];
		for(j=p->count-1;j>n;j--)
		{
			p->data[j+1]=p->data[j];
			p->child[j+1]=p->child[j];
		}

		p->data[j+1]=i;
		p->child[j+1]=i_child;
	}

	*newnode=(struct node *)malloc(sizeof(struct node));
	*upper_val=p->data[d];
	
	for(j=1,k=d+1;k<=MAX;j++,k++)
	{
		(*newnode)->data[j]=p->data[k];
		(*newnode)->child[j]=p->child[k];
	}
	
	(*newnode)->child[0]=p->child[d];
	p->count=d-1;         	//number of keys in the left splitted node
	(*newnode)->count=ORDER-d;  //number of keys in the right splitted node
	(*newnode)->data[ORDER-d]=last_val;
	(*newnode)->child[ORDER-d]=last_child;

}

void shift_insert(int i,struct node *i_child,struct node *p,int n)
{
	int j;
	
	for(j=p->count;j>n;j--)
	{
		p->data[j+1]=p->data[j];
		p->child[j+1]=p->child[j];
	}

	p->data[n+1]=i;
	p->child[n+1]=i_child;
	p->count++;

}

int insert_recursively(int val,struct node *p,int *i,struct node **i_child)
{
	int n,f;

	if(p==NULL)  	//base case 1 
	{
		*i=val;
		*i_child=NULL;
		return 1;   
	}
	
	if(search_node(val,p,&n)) 	//base Case 2 
	{
		printf("SAME VALUE NOT ALLOWED\n");
		return 0;
	}

	f=insert_recursively(val,p->child[n],i,i_child);

	if(f)
	{
		if(p->count<MAX)
		{
			shift_insert(*i,*i_child,p,n);
			return 0;
		}
		else
		{
			split(*i,*i_child,p,n,i,i_child); 	//median key to be inserted
			return 1;
		}
	}
	return 0;
}

struct node *insert(int val,struct node *root)
{
	int i,height;
	struct node *i_child,*temp;
	
	height=insert_recursively(val,root,&i,&i_child);
	
	if(height)  	//tree grown in height,new root is created
	{
		temp=(struct node *)malloc(sizeof(struct node));
		temp->count=1;
		temp->child[0]=root;	
		temp->data[1]=i;
		temp->child[1]=i_child;
		root=temp;
	}
	return root;
}

void left_borrow(struct node *p,int n)
{
	int i;
	struct node *underflow;		
	struct node *left_sibling;	//left_sibling of underflow

	underflow=p->child[n];
	left_sibling=p->child[n-1];

	
	for(i=underflow->count;i>0;i--)		//shift all the keys and pointers in underflow node one position right
	{
		underflow->data[i+1]=underflow->data[i];
		underflow->child[i+1]=underflow->child[i];
	}
	underflow->child[1]=underflow->child[0];

	 
	underflow->data[1]=underflow->data[n]; //move the separator key from parent node p to underflow node
	underflow->count++;

	
	p->data[n]=left_sibling->data[left_sibling->count];  //move the rightmost key of node left_sibling to the parent node p 

	
	underflow->child[0]=left_sibling->child[left_sibling->count];  //rightmost child of left_sibling becomes leftmost child of node underflow
	
	left_sibling->count--;
}

void right_borrow(struct node *p,int n)
{
	int i;
	struct node *underflow;	
	struct node *right_sibling;	// right sibling of underflow

	underflow=p->child[n];
	right_sibling=p->child[n+1];

	
	underflow->count++;		//move the separator key from the parent node p to the underflow 
	underflow->data[underflow->count]=p->data[n+1];

	
	underflow->child[underflow->count]=right_sibling->child[0];		//leftmost child of node right_sibling becomes the rightmost child of underflow

	
	p->data[n+1]=right_sibling->data[1];		//move the leftmost key from node right_sibling to parent node p
	right_sibling->count--;
	
	
	right_sibling->child[0]=right_sibling->child[1];		//shift all the keys and pointers of node right_sibling one position left
	for(i=1;i<=right_sibling->count;i++)
	{
		right_sibling->data[i]=right_sibling->data[i+1];
		right_sibling->child[i]=right_sibling->child[i+1];
	}
}

void combine(struct node *p,int n)
{
	int i;    
	struct node *x,*y;        

	x=p->child[n];
	y=p->child[n-1];
	
	y->count++;		// move the key from the parent node p to node y
	y->data[y->count]=y->data[n];
	
	for(i=n;i<p->count;i++) 	//shift the keys and pointers in p one position left to fill the gap   
	{
		p->data[i]=p->data[i+1];
		p->child[i]=p->child[i+1];
	}
	p->count--;

	
	y->child[y->count]=x->child[0];  //leftmost child of x becomes rightmost child of y
	 
	for(i=1;i<=x->count;i++)  //insert all the keys and pointers of node x at the end of node y
	{
		y->count++;
		y->data[y->count]=x->data[i];
		y->child[y->count]=x->child[i];
	}
	free(x);
}

void repair(struct node *p,int n)
{
	if(n!=0&&p->child[n-1]->count>MIN)      
		left_borrow(p,n);
	else if(n!=p->count&&p->child[n+1]->count>MIN)
		right_borrow(p,n);   
	else
	{	
		if(n==0)  //if underflow node is leftmost node
			combine(p,n+1);	//combine with right sibling
		else 
			combine(p,n);		//combine with left sibling
	}
}

void shift_delete(struct node *p,int n)
{
	int i;
	
	for(i=n+1;i<=p->count;i++)
	{
		p->data[i-1]=p->data[i];
		p->child[i-1]=p->child[i];
	}
	
	p->count--;
}

int copy_successor_val(struct node *p,int n)
{
	struct node *pt;
	
	pt=p->child[n]; //point to the right subtree

	while(pt->child[0]!=NULL)   //move down till leaf node arrives
		pt=pt->child[0];

	p->data[n]=pt->data[1];
	
	return pt->data[1];
}


void delete_recursively(int val,struct node *p)
{
	int n,f,successor_value;
	if(p==NULL)   //reached leaf node,value does not exist
		printf("Value %d not found\n",val);
	
	else
	{
		f=search_node(val,p,&n);

		if(f!=0) // found in current node p
		{
				if(p->child[n]==NULL)  //node p is a leaf node
					shift_delete( p, n);
				else   //node p is a non leaf node 
				{	
					successor_value=copy_successor_val(p,n);
					delete_recursively(successor_value,p->child[n]);
				}
		}
		else	//not found in current node p 
				delete_recursively(val,p->child[n]);

		if(p->child[n]!=NULL)  //if p is not a leaf node 
		{
			if(p->child[n]->count<MIN) //check underflow in p->child[n]
				repair(p,n);
		}
	}
}

struct node *delete(int val,struct node *root)
{
	struct node *temp;
	delete_recursively(val,root);
	
	if(root!=NULL&&root->count==0) 	// if tree becomes shorter,root is changed
	{
		temp=root;
		root=root->child[0];
		free(temp);
	}
	return root;
}

int main()
{
	struct node *root=NULL,*p;
	int val,ch,n;
	
	while(1)
	{
		printf("\n        B-TREE\n");
		printf("1.SEARCH\n");
		printf("2.INSERT\n");
		printf("3.DELETE\n");
		printf("4.DISPLAY\n");
		printf("5.INORDER\n");
		printf("6.QUIT\n\n\n");
		printf("Enter your choice : ");
		scanf("%d",&ch);
		
		switch(ch)
		{
			case 1:	printf("Enter the value to be searched : ");
					scanf("%d",&val);
					if((p=search(val,root,&n))==NULL)
					printf("NO SUCH VALUE IS THERE\n");
					else
					{
						printf("SEARCH SUCCESSFULL\n");
						printf("NODE: %p, POSITION : %d\n\n\n",p,n);
					}
					break;
			
			case 2:	printf("Enter the value to be inserted : ");
					scanf("%d",&val);
					root=insert(val,root);
					printf("INSERT SUCCESSFULL\n\n\n");
					break;
			
			case 3:	printf("Enter the value to be deleted : ");
					scanf("%d",&val);
					root=delete(val,root);
					printf("DELETE SUCCESSFULL\n\n\n");

				break;
			
			case 4:	printf("B-tree is :\n\n");
					display(root,0);
					printf("\n\n\n\n");
					break;
			
			case 5:	inorder(root);
					printf("\n\n\n\n");
					break;

			case 6:	exit(1);
			
			default:	printf("Wrong choice\n");
						break;
		}
	}
}


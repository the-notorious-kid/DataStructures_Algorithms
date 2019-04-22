#include <stdio.h>
#include <stdlib.h>

struct node
{
	int data;
	enum {black, red} color;
	struct  node *left_child;
	struct  node *right_child;
	struct  node *parent;
}*root,*NIL; //in place of NULL i have create a node which stored NIL and color is black 



struct node* search(int val)
{
	struct node *x;

	if(root==NIL) // RBT is NULL
		return NIL;

	if(val==root->data) // if RBT root is your wanted value
		return root;

	
	if(val<root->data)    // otherwidse search continues
		x=root->left_child;
	else
		x=root->right_child;

	while(x!=NIL)
	{
		if(val==x->data)
		{       
			return x;
		}

		if(val<x->data)
			x=x->left_child;
		else
			x=x->right_child;
	 }
	 
	 return NIL;
}

void left_rotate(struct node *x)
{
	struct node* p;

	p=x->right_child;	/*p is right child of x*/
	x->right_child=p->left_child; 
	
	if(p->left_child!=NIL)
        p->left_child->parent=x;
	
	p->parent=x->parent;
	
	if(x->parent==NIL)
		root=p;
	else if(x==x->parent->left_child)	
		x->parent->left_child=p;
	else
		x->parent->right_child=p;
    p->left_child=x;		
	x->parent=p;
}

void right_rotate(struct node *x)
{
	struct node *p;
	
	p=x->left_child;	
	x->left_child=p->right_child; 
	
	if(p->right_child!=NIL)
        p->right_child->parent=x;

	p->parent=x->parent;

	if(x->parent==NIL)
		root=p;
	else if(x==x->parent->right_child)	
		x->parent->right_child=p;
	else
		x->parent->left_child=p;

	p->right_child=x;		
	x->parent=p;
}

void insert_fixup(struct node *x )
{
	struct node *u,*p,*gp;  //u stands for uncle ,p for parent, gp for grandparent
	
	while(x->parent->color==red)
	{
		p=x->parent;
		gp=p->parent;
		
		if(p==gp->left_child)
		{
			u=gp->right_child; 
	
			if(u->color==red)	// Case 1(left side): when uncle is red
			{
				p->color=black;
				u->color=black;
				gp->color=red;
				x=gp;
			}
			else	
			{
				if(x==p->right_child) // case 2(left side): part a
				{
					left_rotate(p);
					x=p;
					p=x->parent;
				}
				p->color=black;	 //case 2(left side): part b
				gp->color=red;
				right_rotate(gp);
			}
		}
		else                       // case 1(right side)
		{
			if(p==gp->right_child)
			{
				u=gp->left_child;  
				
				if(u->color==red)  // Case 1(right side): when uncle is red
				{
					p->color=black;
					u->color=black;
					gp->color=red;
					x=gp;
				}
				else	//uncle is black
				{
					if(x==p->left_child)   // case 2(right side): part a
					{
						right_rotate(p);
						x=p;
						p=x->parent;
					}
					p->color=black;    // case 2(right side): part b
					gp->color=red;
					left_rotate(gp);
				}
			}
		}
	}
	root->color = black;
}


void insert(int val)
{
	struct node *temp,*x,*p;  //p stands for parent
	
	p=NIL;
	x=root;

	while(x!=NIL)
	{
		p=x;
		if(val<x->data)
			x=x->left_child;
		else if(val>x->data)
			x=x->right_child;
		else
		{
			printf("Same value not allowed!!\n");
			return;
		}
	}
	temp=(struct node*)malloc(sizeof(struct node));
	temp->data=val;
	temp->left_child=NIL;
	temp->right_child=NIL;
	temp->color=red;
	temp->parent=p;
	
	if(p==NIL)
		root=temp;
	else if(temp->data<p->data)
		p->left_child=temp;
	else 
		p->right_child=temp;
	
	insert_fixup(temp);
}

void inorder(struct node *x) //inorder traversal
{
	if(x!=NIL)
	{
		inorder(x->left_child);
		printf("%d  ",x->data);
		inorder(x->right_child);
	}
}

void display(struct node *x,int level)  //function for printing Red-Black Tree
{
	int i;
	if(x!=NIL)
	{
		display(x->right_child,level+1);
		printf("\n");
		for(i=0;i<level;i++)
			printf("    ");
		printf("%d",x->data);
		if(x->color==red)
			printf("R");
		else
			printf("B");
		display(x->left_child,level+1);
	}
}

struct node *give_successor(struct node *x)
{
	struct node *p=x->right_child;
	while(p->left_child!=NIL)
		p=p->left_child;

	return p;
}

void delete_fixup(struct node *x)
{
	struct node *sibling;

	while(x!=root)
	{
		if(x==x->parent->left_child)
		{
			sibling=x->parent->left_child;
			
			if(sibling->color==red)  //case 1(left): when sibling is red
			{	
				sibling->color=black;
				x->parent->color=red;
				left_rotate(x->parent);
				sibling=x->parent->left_child;  // new sibling 

			}

			if(sibling->left_child->color==black&&sibling->right_child->color==black)
			{
				sibling->color=red;

				if(x->parent->color==red)   //case 2A(left): when parent is red given sibling is black
				{
					x->parent->color=black;
					return;
				}	
				else
					x=x->parent;   // case 2B(left): when parent is black given sibling is black
			}
			else
			{
				if(sibling->right_child->color==black) //case 3A(left) : when sibling child is also black
				{
					sibling->left_child->color=black;
					sibling->color=red;
					right_rotate(sibling);
					sibling=x->parent->right_child;
				}
				sibling->color=x->parent->color;  // case 3B(left): otherwise
				x->parent->color=black;
				sibling->right_child->color=black;
				left_rotate(x->parent);
				return;
			}
		}
		else  
		{
			sibling=x->parent->left_child;
			
			if(sibling->color==red)  //case 4(right side): when sibling is red
			{	
				sibling->color=black;
				x->parent->color=red;
				right_rotate(x->parent);
				sibling=x->parent->left_child;  // new sibling 

			}

			if(sibling->right_child->color==black&&sibling->left_child->color==black)
			{
				sibling->color=red;

				if(x->parent->color==red)   //case 5A(right): when parent is red given sibling is black
				{
					x->parent->color=black;
					return;
				}	
				else
					x=x->parent;   // case 5B(right): when parent is black given sibling is black
			}
			else
			{
				if(sibling->left_child->color==black)   //case 6A(right) : when sibling child is also black
				{
					sibling->right_child->color=black;
					sibling->color=red;
					left_rotate(sibling);
					display(root,0);
					sibling=x->parent->left_child;
				}
				sibling->color=x->parent->color;  // case 6B(right): otherwise
				x->parent->color=black;
				sibling->left_child->color=black;
				right_rotate(x->parent);
				return;

		 	}
		}
	}
}


int delete(int val)   //optional
{
	struct node *child,*p,*successor;  //here p is pointer
	p=search(val);

	if(p==NIL)
	{
		printf("\nVALUE IS NOT THERE!!\n");
		return 0;
	}


	if(p->left_child!=NIL||p->right_child!=NIL)
	{
		successor=give_successor(p);
		p->data=successor->data;
		p=successor;
	}

	if(p->left_child!=NIL)
		child=p->left_child;
	else
		child=p->right_child;

	child->parent=p->parent;

	
	if(p->parent==NIL)
		root=child;
	else if(p==p->parent->left_child)
			p->parent->left_child=child;
		else
			p->parent->right_child=child;

	
	if(child==root)
		child->color=black;
	else if(p->color==black)  //black node	
			{
				if(child!=NIL)   //1 child which is red
					child->color=black;
				else
					delete_fixup(child);	 
			}

	return 1;
}



int main()
{
	int ch,n;
	struct node* temp;
	
	NIL=(struct node*)malloc(sizeof(struct node)); //have to make a NIL of type struct so that we everytime we don't have to point to null 
	NIL->data=-1;
	NIL->color=black;	
	root=NIL;

	while(1)
	{
		printf("\n             RED-BLACK TREE\n");
		printf("1.INSERT\n");
		printf("2.SEARCH\n");
		printf("3.DISPLAY\n");
		printf("4.INORDER\n");
		printf("5.Quit\n\n");
		printf("Enter your choice : ");
		scanf("%d",&ch);
		
		switch(ch)
		{
		 case 1: printf("\nEnter the number to be inserted : ");
				 scanf("%d",&n);
				 insert(n);
				 printf("\nINSERT SUCCESSFULL\n\n\n");
				 break;
		
		 case 2: printf("\nEnter the number to be search : ");
				 scanf("%d",&n);
				 temp=search(n);
				 if(temp==NIL)
				 printf("\nELEMENT NOT FOUND");
				 else
				 printf("\nElement %d is there in RBT",temp->data);

				 printf("\nSEARCH SUCCESSFULL\n\n\n");
				 break;
		
		 case 3: printf("RED-BLACK TREE :-\n\n\n\n");
		 		 display(root,0);
		 		 printf("\n\n\n\n");
				 break;
		 
		 case 4: printf("RBT INORDR:-\n\n\n\n");
		 		 inorder(root);
		 		 printf("\n\n\n\n");
		 		 break;
		 case 5 : printf("\nEnter the element to be deleted");
		 		  scanf("%d",&n);
		 		  if(delete(n))
		 		  	printf("\nDELETE SUCCESSFULL\n\n\n");
		 		  break;

		 case 6: exit(1);
		 
		 default: printf("Wrong choice\n\n");
		}
	}
}
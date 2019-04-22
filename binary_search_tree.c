#include <stdio.h>
#include <stdlib.h>

struct node
{
	int data;
	struct node *left,*right;
}*root=NULL;

struct node *newnode(int val)
{ 
	struct node *temp=(struct node*)malloc(sizeof(struct node));
    temp->data=val;
    temp->left=NULL;temp->right=NULL;
    return temp;
}

void inorder(struct node* root)  // inorder traversal
{
	if(root!=NULL)
	{
		inorder(root->left);
		printf("%d \n",root->data);
		inorder(root->right);	
	}
}

struct node* insert(struct node *node,int val) //to insert an element
{
	if(node==NULL)
		return newnode(val);
	if(val<node->data)
		node->left=insert(node->left,val);
	else if(val>node->data)
		node->right=insert(node->right,val);
    else 
        printf("SAME VALUE NOT ALLOWED\n");

	return node;
}

struct node* search(struct node* root,int val) // to search an element
{
	if(root==NULL||root->data==val)
		return root;

	if(root->data<val)
		return search(root->right,val);

	return search(root->left,val);
	
}

struct node* min_node(struct node* node) //function to give minimum value node 
{
	struct node* current=node; 

	while(current->left!=NULL)
		current=current->left;

	return current;
}

struct node* delete(struct node* root,int val) // for deleting specific value
{
	if(root==NULL)
		return root;

	if(val<root->data)
		root->left=delete(root->left,val);
	else if(val>root->data)
		root->right=delete(root->right,val);

         else
         {
         	if(root->left==NULL)
         	{
         		struct node* temp=root->right;
         		free(root);
         		return temp;
         	}
         	else if(root->right==NULL)
         	{
         		struct node* temp=root->left;
         		free(root);
         		return temp;
         	}

         	struct node*temp=min_node(root->right);
         	root->data=temp->data;
         	root->right=delete(root->right,temp->data);
         }		
         
    return root;
}  

void display(struct node *x,int level)  //function for printing Binary Tree
{
    int i;
    if(x!=NULL)
    {
        display(x->right,level+1);
        
        printf("\n");
        
        for(i=0;i<level;i++)
            printf("    ");
        printf("%d",x->data);

        display(x->left,level+1);
    }
}  
       	
int main()
{
    int ch,val;
	while(1)
    {
        printf("\n        BINARY TREE\n");
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
            case 1: printf("Enter the value to be searched : ");
                    scanf("%d",&val);
                    if(search(root,val)==NULL)
                        printf("NO SUCH VALUE IS THERE\n");
                    else
                        printf("SEARCH SUCCESSFULL : ELEMENT IS THERE\n");
                    break;
            
            case 2: printf("Enter the value to be inserted : ");
                    scanf("%d",&val);
                    root=insert(root,val);
                    printf("INSERT SUCCESSFULL\n\n\n");
                    break;
            
            case 3: printf("Enter the value to be deleted : ");
                    scanf("%d",&val);
                    root=delete(root,val);
                    printf("DELETE SUCCESSFULL\n\n\n");

                break;
            
            case 4: printf("B-tree is :\n\n");
                    display(root,0);
                    printf("\n\n\n\n");
                    break;
            
            case 5: inorder(root);
                    printf("\n\n\n\n");
                    break;

            case 6: exit(1);
            
            default:    printf("Wrong choice\n");
                        break;
        }
    }
    return 0;
}
#include<stdio.h>
#include<stdlib.h>

#define INF 0x7fffffff // max value of int

struct node
{
    int data,degree;
    struct node *parent,*child,*sibling;
}*H=NULL,*Hr=NULL;
 
int count=1;
 
struct node* make_heap() // make binomial heap
{
    struct node* p;
    p=NULL;
    return p;
}
 
int binomial_link(struct node *y,struct node* z) //link 2 binomial according to their degrees
{
    y->parent=z;
    y->sibling=z->child;
    z->child=y;
    z->degree=z->degree+1;
}
 
struct node* create(int k)  //creating node
{
    struct node* p;
    p=(struct node*)malloc(sizeof(struct node));
    p->data=k;
    return p;
}

struct node* merge(struct node* H1,struct node* H2) // merging 2 heaps
{
    struct node* H = make_heap();
    struct node *a,*b,*c,*d;
        
    a=H1;
    b=H2;
    
    if(a!=NULL)
    {
        if(b!=NULL&&a->degree<=b->degree)
            H=a;
        else if(b!=NULL&&a->degree>b->degree)            
            H=b;
        else
            H=a;
    }
    else
        H=b;


    while(a!=NULL&&b!=NULL)
    {
        if(a->degree<b->degree)
            a=a->sibling;

        else if(a->degree==b->degree)
        {
            c=a->sibling;
            a->sibling=b;
            a=c;
        }
        else
        {
            d=b->sibling;
            b->sibling=a;
            b=d;
        }
    }
    return H;
}
 
struct node* Union(struct node* H1,struct node* H2) //union operation for 2 heap
{
    struct node* prev_x,*next_x,*x;
    struct node* H=make_heap();
    H = merge(H1,H2);
    if(H==NULL)
        return H;
    prev_x=NULL;
    x=H;
    next_x=x->sibling;
    while(next_x!=NULL)
    {
        if((x->degree!=next_x->degree)||((next_x->sibling!=NULL)&&(next_x->sibling)->degree==x->degree))
        {
            prev_x=x;
            x=next_x;
        }
        else
        {
            if(x->data<=next_x->data)
            {
                x->sibling=next_x->sibling;
                binomial_link(next_x,x);
            }
            else
            {
                if(prev_x==NULL)
                    H=next_x;
                else
                    prev_x->sibling=next_x;
                
                binomial_link(x,next_x);
                x=next_x;
            }
        }
        next_x=x->sibling;
    }
    return H;
}
 
struct node* insert(struct node* H,struct node* x)  // inseet element in a binomial heap
{
    struct node* H1=make_heap();
    x->parent=NULL;
    x->child=NULL;
    x->sibling=NULL;
    x->degree=0;
    H1=x;
    H=Union(H,H1);
    return H;
}

struct node* find_min(struct node* H) // find minimum element from binomial heap
{
    struct node* y=NULL;
    struct node* x=H;
    int min=INF;

    while(x!=NULL)
    {
        if(x->data<min)
        {
            min=x->data;
            y=x;
        }
        else
            x=x->sibling;
    }

return y;
}
 
int display(struct node* H)   //display function
{
    struct node* p;
    if(H==NULL)
    {
        printf("\nHeap is empty");
        return 0;
    }

    printf("\nRoot Nodes are:-\n");

    p=H;

    while(p!=NULL)
    {
        printf("%d",p->data);
        if(p->sibling!=NULL)
            printf("---->");
        p=p->sibling;
    }
    printf("\n");
}

struct node* reverse_list(struct node* y) // reversing list after extracting minimun to statisfy binomial heap condition
{   
    struct node* Hr;
    if(y->sibling!=NULL)
    {
        Hr=reverse_list(y->sibling);
        (y->sibling)->sibling = y;
    }
    else 
        Hr=y;

    return Hr; 
}
 
struct node* extract_min(struct node* H1) // extracting minimum element from binomial heap
 {
    int min;
    struct node* temp=NULL,*x=H1;
    struct node *Hr=NULL,*p;    
    
    if(x==NULL)
    {
        printf("\nHeap is empty so nothing is extracted");
        return x;
    }
    
    p=x;
    min=p->data;
    while(p->sibling!=NULL)
    {
        if((p->sibling)->data<min)
        {
            min=(p->sibling)->data;
            temp=p;
            x=p->sibling;
        }

        p=p->sibling;
    }

    if(temp==NULL&&x->sibling==NULL)
        H1=NULL;
    
    else if(temp==NULL)
        H1=x->sibling;
    
    else if(temp->sibling==NULL)
        temp=NULL;
    
    else
        temp->sibling=x->sibling;
    
    if(x->child!=NULL)
    {
        Hr=reverse_list(x->child);
        (x->child)->sibling=NULL;
    }
    H=Union(H1,Hr);
    return x;
}
 
int main()
{
    int i,k,a,ch;
    struct node *p,*n;
    printf("\n          BINOMIAL HEAP\n");
    printf("\nEnter the number of elements:");
    scanf("%d",&k);
    printf("\nEnter the elements:\n");
    
    for(i=1;i<=k;i++)
    {
        scanf("%d",&a);
        n=create(a);
        H=insert(H,n);
    }
    
    display(H);
    
    while(1)
    {
        printf("\n");
        printf("1.INSERT\n");
        printf("2.EXTRACT MINIMUM\n");
        printf("3.FIND MINIMUM\n");
        printf("4.QUIT\n\n");
        scanf("%d",&ch);
        
        switch (ch)
        {
            case 1: printf("\nEnter the element :");
                    scanf("%d",&a);
                    p=create(a);
                    H=insert(H,p);
                    printf("\nNOW THE HEAP IS:\n");
                    display(H);
                    printf("\n\n\n\n");
                    break;
                
            case 2: printf("\nExtracting minimum node\n");
                    p= extract_min(H);
                    if(p!=NULL)
                    printf("\nTHE EXTRACTED NODE IS %d",p->data);
                    printf("\nNOW THE HEAP IS:\n");
                    display(H);
                    printf("\n\n\n\n");
                    break;
                
            case 3: p=find_min(H);
                    printf("\nMinimum Element is %d: \n",p->data);
                    display(H);
                    printf("\n\n\n\n");
                    break;       
            
            case 4: exit(1);
        
            default: printf("\nWrong choice\n");
                     break;
        }
    } 
}
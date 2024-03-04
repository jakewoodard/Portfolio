#include <stdlib.h>
#include <stdio.h>

struct node
{
    int data;
    struct node *next;
};

void display(struct node *head)
{
    struct node *tmp;
    tmp = head;
    while(tmp != NULL)
    {
        printf("%d\n",tmp->data);
        tmp = tmp->next;
    }
}

int main()
{
    struct node *prev,*head,*p;
    int n,i;
    printf ("number of elements:");
    scanf("%d",&n);
    head=NULL;
    for(i=0;i<n;i++)
    {
        p=malloc(sizeof(struct node));
        scanf("%d",&p->data);
        p->next=NULL;
        if(head==NULL)
            head=p;
        else
            prev->next=p;
        prev=p;
    }
    display(head);
    return 0;
}

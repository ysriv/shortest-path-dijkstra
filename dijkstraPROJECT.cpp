#include<iostream>
#include<cstdlib>
using namespace std;
const int inf=10000,NoN=10;
int matrix[NoN][NoN]={
						{0,70,32,inf,83,inf,314,inf,inf,inf},
			   			{70,0,inf,inf,inf,inf,inf,inf,inf,inf},
			   			{32,inf,0,44,52,inf,inf,inf,inf,326},
			   			{inf,inf,44,0,inf,59,inf,inf,inf,223},
			   			{83,inf,52,inf,0,inf,inf,149,127,inf},
			   			{inf,inf,inf,59,inf,0,210,314,inf,inf},
			   			{314,inf,inf,inf,inf,210,0,inf,inf,inf},
			   			{inf,inf,inf,inf,149,134,inf,0,219,inf},
			   			{inf,inf,inf,inf,127,inf,inf,219,0,inf},
			   			{inf,inf,326,223,inf,inf,inf,inf,inf,0}
			   	     };
struct list
{
	int dist,dn,sn;	
}output[20]={0};
int index=0;
struct linkedList
{
	int ns,nd,length;
	struct linkedList *link;
}*r=NULL,*f=NULL;

void Cities()
{
	string city[NoN]={"Bhubaneswar","Puri","Cuttack","Dhenkanal","Jajpur","Angul","Balangir","Kendujhar","Balasore","Rourkela"};
	cout<<"INDEX"<<"		"<<"CITIES"<<endl;
	for(int i=0;i<NoN;i++)
	{
		cout<<" "<<i<<"  	  "<<city[i]<<endl;
	}
}

int checkoplist(int node)
{
	for(int k=0;k<index;k++)
	{
		if(output[k].dn==node)
			return(0);
		else
			return(1);
	}
}

int checkll(int node,int edgelength)
{
	//find if given node is already in linkedlist. 	
	struct linkedList *p=r,*q=r;
	while(q!=NULL)   //didnt use t->link!=null because in that case last node wont be checked.
	{
		if(q->nd==node)
		{
			//if it exists, check whether new distance is less.
			//for first node
			if(q==r)
			{
				if(r->length>edgelength)
				{
					//If yes, delete pre-existing node.
					if(f==r)//suppose only one node exists and has only one adjacent.meh.lonely node.
						f=q->link;
					r=q->link;
					free(q);
					return(1);
				}
				else
					return(0);
			}
			//for last node
			else if(q->link==NULL)
			{
				if(q->length>edgelength)
				{
					while(p->link!=q)
						p=p->link;
					f=p;
					p->link=NULL;
					//If yes, delete pre-existing node.
					free(q);
					return(1);
				}
				else
					return(0);
			}
			//for intermediate node
			else
			{
				while(p->link!=q)
					p=p->link;
				if(q->length>edgelength)
				{
					//If yes, delete pre-existing node.
					p->link=q->link;
					free(q);
					return(1);
				}
				//else dont add node.
				else
					return(0);
			}
		}
		q=q->link;
	}
	return(1);
}

int shortestPath()
{
	//check shortest node to promote to output list.
	struct linkedList *a=r->link,*temp=r,*b=r;
	int min=r->length;
	//for lonely node.
	if(a==NULL)
	{
		output[index].sn=b->ns;
		output[index].dn=b->nd;
		output[index].dist=b->length;
		index++;
		r=NULL;f=NULL;
		return(inf);	
	}
	else
	{
		while(a!=NULL)
		{
			if(a->length<min)
			{
				b=a;
				min=a->length;
			}
			a=a->link;
		}
	}
	output[index].sn=b->ns;
	output[index].dn=b->nd;
	output[index].dist=b->length;
	index++;
	//now to delete the node from linkedlist
	if(b==r)
	{
		if(f==r)
			f=b->link;
		r=b->link;
		free(b);
	}
	else if(b->link==NULL)
	{
		while(temp->link!=b)
			temp=temp->link;
		f=temp;
		f->link=NULL;
		free(b);
	}
	else
	{
		while(temp->link!=b)
			temp=temp->link;
		temp->link=b->link;
		free(b);
	}
	//symmetric matrix so changing certain values to zero.
	for(int j=0;j<NoN;j++)
		matrix[j][output[index-1].dn]=0;
	matrix[output[index-1].dn][output[index-1].sn]=0;
	return(output[index-1].dn);
}

void addAdjacent(int source)//this function accepts a node and adds its adjacents.
{
	int k,l=0;
	//adding adjacents of source to ll
	for(int i=0;i<NoN;i++)
	{
		if(source==inf)
			break;
		struct linkedList *t;
		t=(struct linkedList*)malloc(sizeof(struct linkedList));
		if(matrix[source][i]!=0 && matrix[source][i]!=inf)
		{
			//checking list if adjacent of source is already selected as shortest-path node, if not then continue into if block.
			if(checkoplist(i))
			{
				t->ns=source;
				t->nd=i;
				//distance from source node to current node
				for(k=0;k<index;k++)
				{
					if(output[k].dn==source)
						break;
				}
				//add source and destination to linkedlist along with added distance
				t->length=output[k].dist+matrix[source][i];
				t->link=NULL;
				//check if the added adjacent provides a shorter path. If yes, add, else dont.
				if(checkll(i,t->length))
				{
					if (f==NULL)
					{
						f=t;
						r=f;
					}
					else
					{
						f->link=t;
						f=t;
					}
				}
				else
					free(t);
			}
			else
				continue;
		}
	}
	if (source!=inf)
	{
		int nextNode=shortestPath();
		addAdjacent(nextNode);
	}
}

void display(int s,int d)
{
	int i=0;
	while(output[i].dn!=d)
		i++;
	cout<<"distance is "<<output[i].dist<<endl<<"Intermediate cities"<<endl;
	while(d!=s)
	{
		i=0;
		while(output[i].dn!=d)
			i++;
		string city[NoN]={"Bhubaneswar","Puri","Cuttack","Dhenkanal","Jajpur","Angul","Balangir","Kendujhar","Balasore","Rourkela"};
		cout<<city[output[i].sn]<<endl;
		d=output[i].sn;
	}
}

int main()
{
	int source,destination;
	Cities();
	cout<<"Enter index of start point and destination"<<endl;
	cin>>source>>destination;
	output[index].sn=source;
	output[index].dn=source;
	output[index].dist=0;
	index++;
	addAdjacent(source);
	/*for (int i=0;i<index;i++)
	{
		cout<<"op.dn"<<output[i].dn<<endl;
		cout<<"op.sn"<<output[i].sn<<endl;
		cout<<"op.dist"<<output[i].dist<<endl;
	}*/
	display(source,destination);
	return(0);
}
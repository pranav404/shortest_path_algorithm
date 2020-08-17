#include<iostream>						//Header files
#include<stdlib.h>
#include<time.h>
using namespace std;
struct graphelement{
    bool edge;
    int cost;
};
float prob(){						
    return (float)(rand()%100)/100;				//Probability function
}
struct nodes{							//node structure for the path stack
    int node;
    int cost;
    nodes* nxt;
};
class stack{							//Class declaration for the path stack
    private:
    nodes * start;
    public:
    stack(int x);
    int peek();
    int pop();
    void push(int x,int cost);
    void printstack();
    ~stack();
};
struct openset_node{					//Openset node structure definition
    int from,to,cost;
    bool status;
    openset_node* next;
}; 
struct closedset_node{					//Closed set node structure
    int form,to,cost;
    closedset_node* next;
};
class closedset{						//Closed set class declaration
    private:
    closedset_node* start;
    public:
    closedset(int x);
    void append(int x,int y,int c);
    bool ispresent(int x);
    int peek();
	int peekdist();
    void printset(stack &sptr);
    ~closedset();
};
closedset::closedset(int x){			//Closed set constructor
    start = new closedset_node{x,x,0,nullptr};
    
}
void closedset::append(int x,int y,int c){	//Closed set node function
    start = new closedset_node{x,y,c,start};
}
bool closedset::ispresent(int x){
    closedset_node* temp = start;
    while(temp!=nullptr){
       if(temp->to==x){
           return true;
       }
       temp = temp->next;
    }
    return false;
}
int closedset::peek(){
    return start->to;
}
int closedset::peekdist(){
	return start->cost;
}

void closedset::printset(stack &sptr){
    closedset_node* temp = start;
    cout<<endl;
    while(temp!=nullptr){
        //cout<<temp->form<<temp->to<<temp->cost<<"----->";
		if(sptr.peek()==temp->to){
			sptr.push(temp->form,temp->cost);
		}
        temp = temp->next;
    }
    cout<<endl;
}
closedset::~closedset(){			//Closed set node destructor
    closedset_node* temp = start;
    while(temp!=nullptr){
        start = temp->next;
        delete temp;
        temp = start;
    }
}
class openset_stack{				//Open set class called as openset_stack
    private:
    openset_node* start;
    public:
    openset_stack(){
        start = new openset_node{0,0,0,false,nullptr};	//Openset class constructor
        //cout<<"Default constructor called"<<endl;
    }
    void push(int x,int y,int cost);
    openset_node* strike_out();
    ~openset_stack();
};
void openset_stack::push(int x,int y,int cost){			//Push function for the open set 
    start = new openset_node{x,y,cost,true,start};
    //cout<<"data pushed"<<start->from<<start->to<<start->cost<<start->status<<endl;
}
openset_node* openset_stack::strike_out(){								/* This function finds the minimum cost path between to nodes in a graph 
													then returns the type openset_node pointer which points to the open set 
													element which has the from and to node information and the cost associated.
													The term temp contains the data of the next minimum cost to be taken*/
    openset_node* temp = start;
    openset_node* temp2 = start;
	int min = 0x7FFFFFFF;
    while(temp2!=nullptr){
        if((temp2->cost<=min)&&temp2->status){
            temp = temp2;
			min = temp2->cost;
        }
		//cout<<temp2->from<<"ss"<<temp2->to<<"ss"<<temp2<<endl;
        temp2 = temp2->next;
    }
	//cout<<"y"<<temp->cost<<endl;
    temp->status = false;
    return temp;
}
openset_stack::~openset_stack(){						/* This is the destructor for the open set class as we have 
										created openset_nodes in the heap memory and this deletes them whenever 
										the scope of this class ends*/
    openset_node* temp = start;
    while(start!=nullptr){
        temp = start->next;
        delete start;
        start = temp;
    }
}
/* The functions defined below belong to the stack class and contain all the necessary functions 
that a stack has such as push, pop and peek etc*/

stack::stack(int x){
    start = new nodes{x,0,nullptr};
    //cout<<"Class is created with first node:"<<x<<endl;
}
int stack::peek(){
    return start->node;
}
int stack::pop(){
    int num = start->node;
    nodes * temp = start->nxt;
    delete start;
    start = temp;
    return num;
}
void stack::push(int x,int cost){
    start = new nodes{x,cost,start};
}
void stack::printstack(){
    nodes* temp = start;
    while(temp != nullptr){
        cout<<temp->node<<"-"<<temp->cost<<"  -->  ";
        temp = temp->nxt;
        
    }
    cout<<endl;
}
stack::~stack(){
    nodes* temp;
    temp = start;
    while(temp!=nullptr){
        start = temp;
        delete temp;
        temp = start->nxt;
    }
}
/* The main graph class is declared below and its corresponding constructor, destructor and functions are defined below.
The constructor in this class needs us to specify the size and density of the graph. Once the specified parameters are input
to the constructor, it constructs the graph which then calls isconnected() function, which checks whether the graph 
is connected or not.
If the graph is not connected, then the corresponding edges and cost is calculated again till we obtain a proper connected 
graph.*/




/* The class function spt then finds the shortest path between the source node and destination node and returns a stack 
containing the path information.
The spt() function takes the source, destination and stack as the arguments ( stack is taken as pass-by-reference) and 
pushes the path nodes along with the cost form the source to the stack passed. */





class graph{
    private:
    int size;
    float density;
    graphelement** gr;
    public:
    graph(int size,float density);
    float getdensity(){
        return density;
    }
    void dispgraph(){
        for(int i = 0;i<size;i++){
            for(int j  = 0;j<size;j++){
                cout<<gr[i][j].edge<<"--"<<gr[i][j].cost<<" ";
            }
            cout<<endl;
        }
    }
    ~graph();
    nodes* getneighbours(int x);
    bool isedge(int x, int y){
        if(gr[x][y].edge){
            return true;
        }
        return false;
    }
    bool isconnected();
    int getsize(){
        return size;
        
    }
    void setcost(int x,int y,int cost);
    int getcost(int x,int y);
    void setedge(int x,int y, bool status,int cost);
    void spt(int source,int destination,stack &sptr);
};
graph::graph(int size,float density){
        this->size = size;
        this->density = density;
        srand(time(0));
        gr = new graphelement*[size];
        for(int i = 0;i<size;i++){
            gr[i] = new graphelement[size];
        }
        label:
		for(int i = 0;i<size;i++){
            for(int j = i;j<size;j++){
                if(i==j){
                    gr[i][j].edge = true;
                    gr[i][j].cost = 0;
                }
                else{
                    gr[i][j].edge = gr[j][i].edge = (prob()<density);
					gr[i][j].cost = gr[j][i].cost = 0;
                    if(gr[i][j].edge){
                        gr[i][j].cost = gr[j][i].cost = prob()*100;
                    }
                }
            }
        }
		if(!isconnected()){
			goto label;
        }
    }
nodes* graph::getneighbours(int x){
    nodes* head = nullptr;
    for(int j = 0;j<size;j++){
        if(gr[x][j].edge&&(j!=x)){
            if(head==nullptr){
                head = new nodes{j,gr[x][j].cost,nullptr};
            }
            else{
                head = new nodes{j,gr[x][j].cost,head};
            }
        }
    }
    return head;
}
int graph::getcost(int x,int y){
    return gr[x][y].cost;
}
void graph::setcost(int x,int y,int cost){
    if(cost>0){
        gr[x][y].cost = cost;
    }
    else{
        gr[x][y].cost = 0;
        gr[x][y].edge = false;
    }
}
void graph::setedge(int x,int y,bool status,int cost){
    gr[x][y].edge = status;
    if(status){
        gr[x][y].cost = cost;
    }
    else{
        gr[x][y].cost = 0;
    }
    cout<<"edge has been set"<<endl;
}
bool graph::isconnected(){
    bool* openset = new bool[size];
    bool* closed_set = new bool[size];
    int c_size = 0,old_size = 0;
    for(int i = 0;i<size;i++){
        openset[i] = closed_set[i] = false;
    }
    openset[0] = true;
    for(int i = 0;i<size;i++){
        old_size = c_size;
        if(openset[i]&&(closed_set[i]==false)){
            closed_set[i] = true;
            c_size++;
            for(int j = 0;j<size;j++){
                openset[j] = openset[j]||gr[i][j].edge;
            }
        }
    }
    
    if(c_size==size){
        delete[] openset;
        delete[] closed_set;
        return true;
    }
    delete[] openset;
    delete[] closed_set;
    return false;
    
    
}
void graph::spt(int source,int destination,stack &sptr){
    openset_stack os;
    closedset cs(source);
    openset_node* x;
    while(!cs.ispresent(destination)){
        for(int i = 0;i<size;i++){
            if(gr[cs.peek()][i].edge&&(i!=cs.peek())){
                if(!cs.ispresent(i)){
                    os.push(cs.peek(),i,(gr[cs.peek()][i].cost+cs.peekdist()));
                }
            }
        }
        x = os.strike_out();
        cs.append(x->from,x->to,x->cost);
        
    }
    cs.printset(sptr);
	
	
    
}
graph::~graph(){
    for(int j = 0;j<size;j++){
        delete[] gr[j];
    }
    delete[] gr;
}
void displaynodes(nodes* head){
    nodes* temp = head;
    while(temp!=nullptr){
        cout<<"node-"<<temp->node<<" Cost= "<<temp->cost<<"--->";
        temp = temp->nxt;
    }
    cout<<endl;
}
void deletenodes(nodes* head){
    nodes* temp = head;
    while(temp!=nullptr){
        head = temp->nxt;
        delete temp;
        temp = head;
    }
    cout<<"Node list deleted..."<<endl;
}

int main(){
    int size = 0,source = 0,destination  = 0;
	float density = 0.0;
	cout<<"Enter size and density"<<endl;
	cin>>size;
	cin>>density;
	graph g(size,density);
	cout<<"size = "<<g.getsize()<<endl<<"density = "<<g.getdensity()<<endl;
	g.dispgraph();
    cout<<"Graph is connected. Please enter the source and destination nodes respectively:"<<endl;
	cin>>source>>destination;
	stack s(destination);
	g.spt(source,destination,s);
	cout<<"The path is :"<<endl;
	s.printstack();
    return 0;
}




/* When the program is run, the output is seen like this.....




pranav@DESKTOP-IBV976B:~/desktop$ g++ dji.cpp -o dji.out
pranav@DESKTOP-IBV976B:~/desktop$ ./dji.out
Enter size and density
8
0.2
size = 8
density = 0.2
1--0 1--3 1--86 0--0 0--0 0--0 0--0 0--0
1--3 1--0 0--0 0--0 1--82 0--0 0--0 0--0
1--86 0--0 1--0 1--52 1--6 1--42 0--0 0--0
0--0 0--0 1--52 1--0 0--0 0--0 1--30 1--91
0--0 1--82 1--6 0--0 1--0 0--0 0--0 0--0
0--0 0--0 1--42 0--0 0--0 1--0 1--50 0--0
0--0 0--0 0--0 1--30 0--0 1--50 1--0 1--49
0--0 0--0 0--0 1--91 0--0 0--0 1--49 1--0
Graph is connected. Please enter the source and destination nodes respectively:
5
4


The path is :
5-0  -->  5-42  -->  2-48  -->  4-0  -->
pranav@DESKTOP-IBV976B:~/desktop$                                                  

In the above output we can see that for the shortest path, the nodes are:
5 cost -0
2 cost -42
4 cost -48

Thus the shortest path will cost us 48 for going from 5 to 4.



Another output for reference:




pranav@DESKTOP-IBV976B:~/desktop$ ./dji.out
Enter size and density
9
0.25
size = 9
density = 0.25
1--0 1--52 0--0 0--0 0--0 0--0 0--0 1--64 0--0
1--52 1--0 1--30 0--0 1--7 0--0 0--0 0--0 1--84
0--0 1--30 1--0 1--40 0--0 1--17 0--0 1--48 1--52
0--0 0--0 1--40 1--0 0--0 0--0 0--0 0--0 0--0
0--0 1--7 0--0 0--0 1--0 0--0 0--0 1--24 0--0
0--0 0--0 1--17 0--0 0--0 1--0 1--97 0--0 0--0
0--0 0--0 0--0 0--0 0--0 1--97 1--0 0--0 0--0
1--64 0--0 1--48 0--0 1--24 0--0 0--0 1--0 0--0
0--0 1--84 1--52 0--0 0--0 0--0 0--0 0--0 1--0
Graph is connected. Please enter the source and destination nodes respectively:
5
7


The path is :
5-0  -->  5-17  -->  2-65  -->  7-0  -->
pranav@DESKTOP-IBV976B:~/desktop$                                                  




*/



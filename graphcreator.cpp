#include <iostream>
#include <cstring>

using namespace std;


struct Vertex{
  char label[20];
};

struct Edge{
  Vertex* firstV;
  Vertex* secondV;
  int weight;
};

struct Node{
  Vertex* v;
  Edge* e;
  bool empty;
  Node* next = nullptr;
};

void insertNode(Node** adjacency, Vertex* &v);
void printAdjacency(Node** adjacency);
void removeVertex(char label[20], Node** adjacency);
int main(){
  bool quit = false;
  char input[20];
  Node** adjacency = new Node*[20];
  for(int a = 0; a<20; a++){
    Node* n = new Node;
    n->empty = true;
    n->e = nullptr;
    n->next = nullptr;
    //just added below line
    n->v = nullptr;
    adjacency[a] = n;
  }
  cout << "yuhhh!" << endl;
  while(!quit){
    cout << "type add, remove, find path, or quit" << endl;
    cin >> input;
    cin.get();
    if(input[0] == 'a' || input[0] == 'A'){
      char input2[20];
      cout << "what would you like to add? type vertex or edge" << endl;
      cin >> input2;
      cin.get();
      if(input2[0] == 'v' || input2[0] == 'V'){
	char input3[10];
	cout << "please enter a label for the vertex" << endl;
	cin >> input3;
	cin.get();
	Vertex* v = new Vertex;
	strcpy(v->label, input3);
	insertNode(adjacency, v);
      }
      else if(input2[0] == 'e' || input2[0] == 'E'){
	char fn[10];
	char sn[10];
	int weight;
	int index = -1;
	bool ffound = false;
	bool sfound = false;
	Vertex* first;
	Vertex* toConnect;
	cout << "enter the name of the first node" << endl;
	cin >> fn;
	cin.get();
	for(int a = 0; a < 20; a++){
	  Node* n = adjacency[a];
	  if(n->v!=nullptr){
	    if(strcmp(n->v->label,fn)==0){
	      index = a;
	      ffound = true;
	      first = n->v;
	      break;
	    }
	  }
	}
	cout << "enter the name of the second node" << endl;
	cin >> sn;
        cin.get();
	for(int a = 0; a < 20; a++){
	  Node* n = adjacency[a];
	  if(n->v!=nullptr){
	    if(strcmp(n->v->label,sn)==0){
	      sfound = true;
	      toConnect = n->v;
	      break;
	    }
	  }
	}
	if(ffound&&sfound){
	  cout << "enter the weight" << endl;
	  cin >> weight;
	  cin.get();
	  Node* n = new Node;
	  n->v = toConnect;
	  n->empty = false;
	  Edge* e = new Edge;
	  e->firstV = first;
	  e->secondV = toConnect;
	  e->weight = weight;
	  n->e = e;
	  Node* current = adjacency[index];
	  while(current!=nullptr){
	    if(current->next == nullptr){
	      current->next = n;
	      break;
	    }
	    current = current->next;
	  }
	}
	else{
	  cout << "sorry, this edge can not be created" << endl;
	}
      }
    }
    else if(input[0] == 'r' || input[0] == 'R'){
      char input2[10];
      cout << "are you removing a vertex or an edge" << endl;
      cin >> input2;
      cin.get();
      if(input2[0] == 'v' || input2[0] == 'V'){
	char label[10];
	cout << "enter the label of the vertex you would like to remove" << endl;
	cin >> label;
	cin.get();
	removeVertex(label, adjacency);
      }
      else if(input2[0] == 'e' || input2[0] == 'E'){
	
      }
    }
    else if(input[0] == 'f' || input[0] == 'F'){
    }
    else if(input[0] == 'q' || input[0] == 'Q'){
      quit = true;
    }
    else if(input[0] == 'p' || input[0] == 'P'){
      printAdjacency(adjacency);
    }
  }
}

void insertNode(Node** adjacency, Vertex* &v){
  for(int a = 0; a < 20; a++){
    Node* n = adjacency[a];
    cout << a << endl;
    if(n->empty){
      n->v = v;
      n->empty = false;
      cout << "inserted vertex: " << n->v->label << " at index: " << a << endl;
      adjacency[a] = n;
      break;
    }
  }
}

void printAdjacency(Node** adjacency){
  Node* current;
  for(int a = 0; a < 20; a++){
    current = adjacency[a];
    if(current->v != nullptr){
      while(current!=nullptr){
	cout << current->v->label << " " ;
	current = current->next;
      }
      cout << endl;
    }
  }
}

void removeVertex(char label[10], Node** adjacency){
  bool found = false;
  int index = -1;
  for(int a = 0; a < 20; a++){
    Node* n = adjacency[a];
    if(n->v!=nullptr){
      if(strcmp(n->v->label,label)==0){
	found = true;
	index = a;
	break;
      }
    }
  }
  if(found){
    for(int a = 0; a < 20; a++){
      if(index==a){
	Node* n = adjacency[a];
	n->empty = true;
	n->e = nullptr;
	n->next = nullptr;
	n->v = nullptr;
	adjacency[a] = n;
      }
      else{
	Node* current = adjacency[a];
	Node* previous = nullptr;
	while(current!=nullptr){
	  if(!current->empty){
	      if(strcmp(current->v->label,label)==0){
		previous->next = current->next;
	      }
	  }
	  previous = current;
	  current = current->next;
	}
      }
    }
  }
  
}

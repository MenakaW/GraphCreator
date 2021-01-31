#include <iostream>
#include <cstring>

using namespace std;

//vertex struct
struct Vertex{
  char label[20];
  bool visited = false;
  int distance = -1;
};

//edge struct
struct Edge{
  Vertex* firstV;
  Vertex* secondV;
  int weight;
};

//node struct (for adjacency table)
struct Node{
  Vertex* v;
  Edge* e;
  bool empty;
  Node* next = nullptr;
};

//method prototypes
void insertNode(Node** adjacency, Vertex* &v);
void printAdjacency(Node** adjacency);
void removeVertex(char label[20], Node** adjacency);
bool isEmpty(Vertex** q);
int findMinIndex(Vertex** q);
Node* getNode(char label[20], Node** adjacency);


int main(){
  //set up input char arrays and adjacency array
  bool quit = false;
  char input[20];
  Node** adjacency = new Node*[20];
  //set up all nodes in adjacency matrix
  for(int a = 0; a<20; a++){
    Node* n = new Node;
    n->empty = true;
    n->e = nullptr;
    n->next = nullptr;
    n->v = nullptr;
    adjacency[a] = n;
  }
  //user input loop
  while(!quit){
    cout << "type add, remove, find, or quit" << endl;
    cin >> input;
    cin.get();
    //if add
    if(input[0] == 'a' || input[0] == 'A'){
      //check if user wants to add vertex or edge
      char input2[20];
      cout << "what would you like to add? type vertex or edge" << endl;
      cin >> input2;
      cin.get();
      //if vertex
      if(input2[0] == 'v' || input2[0] == 'V'){
	char input3[10];
	cout << "please enter a label for the vertex" << endl;
	cin >> input3;
	cin.get();
	//make a new vertex, and then call insert method
	Vertex* v = new Vertex;
	strcpy(v->label, input3);
	v->visited = false;
	insertNode(adjacency, v);
      }
      //if edge
      else if(input2[0] == 'e' || input2[0] == 'E'){
	char fn[10];
	char sn[10];
	int weight;
	int index = -1;
	bool ffound = false;
	bool sfound = false;
	Vertex* first;
	Vertex* toConnect;
	cout << "enter the name of the first vertex" << endl;
	cin >> fn;
	cin.get();
	//shallow cycle through the adjacency matrix for first node
	for(int a = 0; a < 20; a++){
	  Node* n = adjacency[a];
	  if(n->v!=nullptr){
	    //if you find the vertex, then make ffound true
	    if(strcmp(n->v->label,fn)==0){
	      index = a;
	      ffound = true;
	      first = n->v;
	      break;
	    }
	  }
	}
	cout << "enter the name of the second vertex" << endl;
	cin >> sn;
        cin.get();
	//shallow cycle through adjacency matrix to look for second node, change sfound to true
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
	  //create a new node to represent the connection, add an edge and the second vertex
	  Node* n = new Node;
	  n->v = toConnect;
	  n->empty = false;
	  Edge* e = new Edge;
	  e->firstV = first;
	  e->secondV = toConnect;
	  e->weight = weight;
	  n->e = e;
	  Node* current = adjacency[index];
	  //do a deep cycle into the shallow node to attach new node to the end of the linked list
	  while(current!=nullptr){
	    if(current->next == nullptr){
	      current->next = n;
	      break;
	    }
	    current = current->next;
	  }
	}
	//if you didn't find both nodes then let the user know
	else{
	  cout << "sorry, this edge can not be created" << endl;
	}
      }
    }
    //if remove
    else if(input[0] == 'r' || input[0] == 'R'){
      char input2[10];
      cout << "are you removing a vertex or an edge" << endl;
      cin >> input2;
      cin.get();
      //if removing vertex
      if(input2[0] == 'v' || input2[0] == 'V'){
	char label[10];
	cout << "enter the label of the vertex you would like to remove" << endl;
	cin >> label;
	cin.get();
	//call remove vertex function
	removeVertex(label, adjacency);
      }
      //if removing edge
      else if(input2[0] == 'e' || input2[0] == 'E'){
	char fn[10];
	char sn[10];
	bool ffound = false;
	bool sfound = false;
	int index = -1;
	cout << "enter the label of the first vertex of the edge" << endl;
	cin >> fn;
	cin.get();
	cout << "enter the label of the second vertex of the edge" << endl;
	cin >> sn;
	cin.get();
	//shallow search for the first node
	for(int a = 0; a < 20; a++){
	  Node* n = adjacency[a];
	  if(n->v!=nullptr){
	    if(strcmp(n->v->label,fn)==0){
	      ffound = true;
	      index = a;
	      break;
	    }
	  }
	}
	if(ffound){
	  Node* current = adjacency[index]->next;
	  Node* previous = adjacency[index];
	  //deep search for the second node, when you find the one you want to delete, reconfigure
	  //the connections to skip that edge node
	  while(current!=nullptr){
	    if(current->v!=nullptr){
	      if(strcmp(current->v->label,sn)==0){
		sfound = true;
		previous->next = current->next;
	      }
	    }
	    previous = current;
	    current = current->next;
	  }
	}
	//if you didn't find the first or second node, let the user know
	if(!sfound || !ffound){
	  cout << "sorry the edge you were looking for can not be found" << endl;
	}
      }
    }
    //if find
    else if(input[0] == 'f' || input[0] == 'F'){
      char fn[10];
      char sn[10];
      bool ffound = false;
      bool sfound = false;
      bool pathFound = false;
      cout << "please enter the label of the first node" << endl;
      cin >> fn;
      cin.get();
      cout << "please enter the label of the second node" << endl;
      cin >> sn;
      cin.get();
      //make a new array of vertexes, and fill them with the shallow level vertexes
      //in the adjacency table
      Vertex** q = new Vertex*[20];
      for(int a = 0; a < 20; a++){
	Node* n = adjacency[a];
	if(n->v!=nullptr){
	  //if it's the starting node, make the distance 0, else -1
	  if(strcmp(n->v->label,fn)==0){
	    n->v->distance = 0;
	  }
	  else{
	    n->v->distance = -1;
	  }
	  //make all nodes visited false
	  n->v->visited = false;
	  adjacency[a] = n;
	}
	q[a] = n->v;
      }
      //go until the q array is empty
      while(!isEmpty(q)){
	//find the index of the smallest distance vertex in q
	int index = -1;
	index = findMinIndex(q);
	//-1 means that there are no more connected nodes in q, so to avoid errors break 
	if(index==-1){
	  break;
	}
	//delete the vertex from q
	Vertex* v = q[index];
	v->visited = true;
	q[index] = nullptr;
	//check if you finally made it to the goal vertex
	if(strcmp(v->label, sn)==0){
	  pathFound = true;
	  cout << "final calculated distance is " << v->distance << endl;
	  break;
	}
	Node* n = getNode(v->label, adjacency)->next;
	//cycle through vertex v's connection
	while(n!=nullptr){
	  //get the distance from the start to each of the connected nodes if they pass through v
	  int currentDistance = v->distance + n->e->weight;
	  //if the node never had a distance, assign it automatically
	  if(n->v->distance==-1){
	    n->v->distance = currentDistance;
	  }
	  //else, first check if the distance is less than the pre-existing distance, and if so
	  //replace it
	  else if(currentDistance < n->v->distance){
	    n->v->distance = currentDistance;
	  }
	  n = n->next;
	}
      }
      //if you didn't find a path, let the user know
      if(!pathFound){
	cout << "sorry there are no paths between those two points" << endl;
      }
    }
    //if quit
    else if(input[0] == 'q' || input[0] == 'Q'){
      quit = true;
    }
    //if print, call print method
    else if(input[0] == 'p' || input[0] == 'P'){
      printAdjacency(adjacency);
    }
  }
}

//given a label, search through the table and return the node with a vertex with that label
Node* getNode(char label[20], Node** adjacency){
  //cycle through the adjacency table and check to see if the vertex label matches the input
  for(int a = 0; a < 20; a++){
    Node* n = adjacency[a];
    if(n->v!=nullptr){
      if(strcmp(n->v->label,label)==0){
	return n;
      }
    }
  }
  return nullptr;
}

//cycle through the vertex array used for find and return the index with the least distance
int findMinIndex(Vertex** q){
  int index = -1;
  int runningMin = -1;
  //check the distance of each vertex, and return the index of the vertex with the least distance
  for(int a = 0; a < 20; a++){
    if(q[a]!=nullptr){
      if(q[a]->distance!=-1){
	if(runningMin==-1){
	  runningMin = q[a]->distance;
	  index = a;
	}
	else if(q[a]->distance < runningMin){
	  runningMin = q[a]->distance;
	  index = a;
	}
      }
    }
  }
  return index;
}

//return true if the vertex array is empty
bool isEmpty(Vertex** q){
  bool toReturn = true;
  //if you find a non-null vertex, return false, else true
  for(int a = 0; a < 20; a++){
    Vertex* v = q[a];
    if(q[a]!=nullptr){
      toReturn = false;
    }
  }
  return toReturn;
}

//insert a node into the adjacency table
void insertNode(Node** adjacency, Vertex* &v){
  //shallow cycle through the adjacency array until you find an empty slot, then insert the node
  for(int a = 0; a < 20; a++){
    Node* n = adjacency[a];
    if(n->empty){
      n->v = v;
      n->empty = false;
      adjacency[a] = n;
      break;
    }
  }
}

//do a full cycle through the adjacency matrix and print all vertexes and edge nodes out
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

//remove the vertex that matches the label
void removeVertex(char label[10], Node** adjacency){
  bool found = false;
  int index = -1;
  //shallow cycle through the adjacency table and look for the matching node
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
    //thorough cycle through the adjacency table
    for(int a = 0; a < 20; a++){
      //if this is the main node where the vertex is stored (shallow), then empty the node
      if(index==a){
	Node* n = adjacency[a];
	n->empty = true;
	n->e = nullptr;
	n->next = nullptr;
	n->v = nullptr;
	adjacency[a] = n;
      }
      //if it's not the main node, go through the edge nodes of each shallow node
      //and look for matching vertices, then reconfigure next pointers to delete it
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

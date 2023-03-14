#include <iostream>
#include <stack>
#include<vector>
using namespace std;

struct node
{
    int value;
    node* next;
};

class LinkedList
{
    private:
        int count;
        node* head = NULL;
        node* tail = NULL;
        
    public:
        LinkedList()
        {
            head = NULL;
            tail = NULL;
            count = 0;
        }
    
        void insert(int x)
        {
            if(head==NULL && tail==NULL)
            {
                head = new node;
                head->value = x;
                head->next = NULL;
                tail = head;
            }
            else
            {
                node* new_element = new node;
                new_element->value = x;
                new_element->next = NULL;
                tail->next = new_element;
                tail = tail->next;
            }
            count++;
        }
    
        int num_elements() {return count;}
        
        void get_elements(vector<int>& elements)
        {
            node* temp = head;
            while(temp!=NULL)
            {
                elements.push_back(temp->value);
                temp = temp->next;
            }
        }
        
};

class Vertex
{
    public:
        int vertex;
        Vertex(int v) : vertex(v) {}
};

class Edge
{
    public:
        int EdgefromVertex, EdgetoVertex;
        Edge(Vertex v, Vertex w) : EdgefromVertex(v.vertex), EdgetoVertex(w.vertex) {}
};

class Graph
{
  protected:
    int numVertices;
    int numEdges;
  
  public:
    Graph(int num_vertices, int num_edges)
    {
        numVertices = num_vertices;
        numEdges= num_edges;
    }
    
    virtual void addEdge(Edge e) = 0; // add edge e
    virtual int outDegree(Vertex v) = 0; //return the out degree of v
    virtual int inDegree(Vertex v) = 0; // return the in degree of c
    virtual bool reach(Vertex v1, Vertex v2) = 0; // return if there is a path from v1 to v2
    virtual bool detectCycle() = 0; //return if there is a cycle in the graph
};



// Graph using Adjacency Matrix
class GraphUsingMatrix : public Graph
{
    private:
        bool** adj_matrix; //store the adjacency matrix with from_vertices as the rows and to_vertices as columns
        void get_neighbour_list(int v, vector<int>& neighbours); //get the list of neighbours of a vertex
        bool reach1(Vertex start_vertex, Vertex end_vertex, vector<bool>& marked);
        int num_vertices_added;
        vector<int> vertices_added;
        
    public:
        GraphUsingMatrix(int num_vertices, int num_edges) : Graph(num_vertices, num_edges)
        {
            adj_matrix = new bool*[num_vertices];
            for (int i=0; i<numVertices; i++) adj_matrix[i] = new bool[numVertices];
            for (int i=0; i<numVertices; i++)
            {
                for (int j=0; j<numVertices; j++) adj_matrix[i][j] = false;
            }
            num_vertices_added = 0;
        }
        
        void addEdge(Edge e); // add edge e
        int outDegree(Vertex v); //return the out degree of v
        int inDegree(Vertex v); // return the in degree of c
        bool reach(Vertex start_vertex, Vertex end_vertex); // return if there is a path from v1 to v2
        bool detectCycle(); //return if there is a cycle in the graph
        
};

void GraphUsingMatrix::addEdge(Edge e)
{
    int from_vertex = e.EdgefromVertex;
    int to_vertex = e.EdgetoVertex;
    if(outDegree(from_vertex)==0 && inDegree(from_vertex)==0)
    {
        vertices_added.push_back(from_vertex);
        num_vertices_added++;
    }
    if(outDegree(to_vertex)==0 && inDegree(to_vertex)==0) 
    {
        vertices_added.push_back(to_vertex);
        num_vertices_added++;
    }
    adj_matrix[from_vertex][to_vertex] = true;
}

int GraphUsingMatrix::outDegree(Vertex v)
{
    int out_degree = 0;
    int fromVertex = v.vertex;
    for(int i=0; i<numVertices; i++)
    {
        if(adj_matrix[fromVertex][i]) out_degree++;
    }
    return out_degree;
}

int GraphUsingMatrix::inDegree(Vertex v)
{
    int in_degree = 0;
    int to_vertex = v.vertex;
    for(int i=0; i<numVertices; i++)
    {
        if(adj_matrix[i][to_vertex]) in_degree++;
    }
    return in_degree;
}

void GraphUsingMatrix::get_neighbour_list(int v, vector<int>& neighbours)
{
    neighbours.clear();
    for(int i=0; i<numVertices; i++)
    {
        if(adj_matrix[v][i]) neighbours.push_back(i);
    }
}

bool GraphUsingMatrix::reach(Vertex start_vertex, Vertex end_vertex)
{
    vector<bool> marked(numVertices, false);
    return reach1(start_vertex, end_vertex, marked);
}
    
bool GraphUsingMatrix::reach1(Vertex start_vertex, Vertex end_vertex, vector<bool>& marked)
{
    int start = start_vertex.vertex;
    int end = end_vertex.vertex;
    marked[start] = true;
    vector<int> neighbours;
    get_neighbour_list(start, neighbours);
    int sz = neighbours.size();
    for(int i=0; i<sz; i++)
    {
        if(neighbours[i]==end) return true;
    }
    for(int i=0; i<sz; i++)
    {
        if(!marked[neighbours[i]])
            if(reach1(Vertex(neighbours[i]), end_vertex, marked)) return true;
    }
    return false;
}

bool GraphUsingMatrix::detectCycle()
{
    vector<bool> start(numVertices, false);
    vector<bool> end(numVertices, false);
    int num_ended = 0;
    stack<int> vertices;
    while(!vertices.empty()) vertices.pop();
    while(num_ended!=num_vertices_added)
    {
        for(int i=0; i<num_vertices_added; i++)
        {
            if(!start[vertices_added[i]])
            {
                vertices.push(vertices_added[i]);
                break;
            }
        }
        while(!vertices.empty())
        {
            int cur_vertex = vertices.top();
            if(!start[cur_vertex])
            {
                start[cur_vertex] = true;
                vector<int> neighbours;
                get_neighbour_list(cur_vertex, neighbours);
                for(int i:neighbours)
                {
                    if(!start[i]) vertices.push(i);
                    else
                        if(!end[i]) return true;
                }
            }
            else
            {
                if(!end[cur_vertex])
                {
                    end[cur_vertex] = true;
                    num_ended++;
                }
                vertices.pop();
            }
        }
    }
    return false;
}
    

// Graphs using Adjacency List
class GraphUsingList : public Graph
{
    private:
        vector <LinkedList> adj_list;
        vector <int> in_degree;
        vector <int> vertices_added;
        void get_neighbour_list(int v, vector<int>& neighbours); //get the list of neighbours of a vertex
        bool reach1(Vertex start_vertex, Vertex end_vertex, vector<bool>& marked);
        int num_vertices_added;
    
    public:
        GraphUsingList (int num_vertices, int num_edges) : Graph(num_vertices, num_edges)
        {
            adj_list.resize(numVertices);
            in_degree.resize(numVertices);
            in_degree = {0};
            num_vertices_added = 0;
        }
        
        void addEdge(Edge e); // add edge e
        int outDegree(Vertex v); //return the out degree of v
        int inDegree(Vertex v); // return the in degree of c
        bool reach(Vertex start_vertex, Vertex end_vertex); // return if there is a path from v1 to v2
        bool detectCycle(); //return if there is a cycle in the graph      
};

void GraphUsingList::addEdge(Edge e)
{
    int from_vertex = e.EdgefromVertex;
    int to_vertex = e.EdgetoVertex;
    if(outDegree(from_vertex)==0 && inDegree(from_vertex)==0)
    {
        vertices_added.push_back(from_vertex);
        num_vertices_added++;
    }
    if(outDegree(to_vertex)==0 && inDegree(to_vertex)==0) 
    {
        vertices_added.push_back(to_vertex);
        num_vertices_added++;
    }
    adj_list[from_vertex].insert(to_vertex);
    in_degree[e.EdgetoVertex]++;
}

int GraphUsingList::outDegree(Vertex v)
{
    int from_vertex = v.vertex;
    for(int i:vertices_added)
    {
        if(i==from_vertex)
        {
            return adj_list[from_vertex].num_elements();
        }
    }
    return 0;
}

int GraphUsingList::inDegree(Vertex v)
{
    int to_vertex = v.vertex;
    return in_degree[to_vertex];
}

void GraphUsingList::get_neighbour_list(int v, vector<int>& neighbours)
{
    adj_list[v].get_elements(neighbours);
}

bool GraphUsingList::reach(Vertex start_vertex, Vertex end_vertex)
{
    vector<bool> marked(numVertices, false);
    return reach1(start_vertex, end_vertex, marked);
}
    
bool GraphUsingList::reach1(Vertex start_vertex, Vertex end_vertex, vector<bool>& marked)
{
    int start = start_vertex.vertex;
    int end = end_vertex.vertex;
    marked[start] = true;
    vector<int> neighbours;
    get_neighbour_list(start, neighbours);
    int sz = neighbours.size();
    for(int i=0; i<sz; i++)
    {
        if(neighbours[i]==end) return true;
    }
    for(int i=0; i<sz; i++)
    {
        if(!marked[neighbours[i]])
            if(reach1(Vertex(neighbours[i]), end_vertex, marked)) return true;
    }
    return false;
}

bool GraphUsingList::detectCycle()
{
    vector<bool> start(numVertices, false);
    vector<bool> end(numVertices, false);
    int num_ended = 0;
    stack<int> vertices;
    while(!vertices.empty()) vertices.pop();
    while(num_ended!=num_vertices_added)
    {
        for(int i=0; i<num_vertices_added; i++)
        {
            if(!start[vertices_added[i]])
            {
                vertices.push(vertices_added[i]);
                break;
            }
        }
        while(!vertices.empty())
        {
            int cur_vertex = vertices.top();
            if(!start[cur_vertex])
            {
                start[cur_vertex] = true;
                vector<int> neighbours;
                get_neighbour_list(cur_vertex, neighbours);
                for(int i:neighbours)
                {
                    if(!start[i])
                    {
                        vertices.push(i);
                    }
                    else
                    {

                        if(!end[i])
                        {
                            return true;
                        }
                    }
                }
            }
            else
            {
                if(!end[cur_vertex])
                {
                    end[cur_vertex] = true;
                    num_ended++;
                }
                vertices.pop();
            }
        }
    }
    return false;
}

/* DO NOT CHANGE THE CODE BELOW */
int main()
{
  int N;
  cin >> N;
  Graph * g;
  int command;
  const int SPARSITYRATIO = 5;
  for (int i = 0; i < N; i++)
  {
    cin >> command;
    switch (command)
    {
      case 1: /* initialize number of vertices and edges */
      {
        int nv,ne;
        cin >> nv >> ne;
        if (ne/nv > SPARSITYRATIO)
        {
          g = new GraphUsingMatrix(nv,ne);
        }
        else
        {
          g = new GraphUsingList(nv,ne);
        }
        break;
      }
      
      case 2: /* Add edge */
      {
        int v,w;
        cin >> v >> w;
        g->addEdge(Edge(Vertex(v), Vertex(w)));
        break;
      }
      
      case 3: /* Reachability query */
      {
        int v,w;
        cin >> v >> w;
        cout << g->reach(Vertex(v), Vertex(w)) << endl;
        break;
      }

      case 4: /* Detect Cycle */
      {
        cout << g->detectCycle() << endl;
        break;
      }
        
      case 5: /* In-degree */
      {
        int v;
        cin >> v;
        cout << g->inDegree(Vertex(v)) << endl;
        break;
      }

      case 6: /* Out-degree */
      {
        int v;
        cin >> v;
        cout << g->outDegree(Vertex(v)) << endl;
        break;
      }
      
      default:
        break;
    }
  }
}
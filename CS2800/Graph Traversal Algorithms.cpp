/*Code for DFS and its modifications for multiple use cases for both directed and undirected graphs
 * DFS for finding the reach of a vertex
 * DFS for classifying edges
 * DFS for detecting cycles
 * DFS for finding the topological ordering*/

#include <iostream>
#include <iterator>
#include <vector>
#include <map>
#include <list>
#include <stack>

using namespace std;

class Graph
{
private:
    vector<list<int>> adj_list; //(v, list of neighbours of v)
    int num_vertices, num_edges;

public:
    Graph() : num_edges(0) , num_vertices(0) {}
    void build_undirected_graph();
    void build_directed_graph();
    void DFS();
    vector<int> DFS_for_reach(int source);
    vector<pair<int, int>> generate_DFS_tree(int source);
    bool DFS_for_cycle_detection();
    vector<int> DFS_for_topological_order();
    void DFS_for_edge_classification();
};

void Graph::build_undirected_graph() {
    cin >> num_vertices >> num_edges;
    for(int i=0; i<=num_vertices; i++)
    {
        list<int> new_list;
        adj_list.push_back(new_list);
    }
    int start, end;
    for(int i=0; i<num_edges; i++)
    {
        cin >> start >> end;
        adj_list[start].push_back(end);
        adj_list[end].push_back(start);
    }

}

void Graph::build_directed_graph() {
    cin >> num_vertices >> num_edges;
    for(int i=0; i<=num_vertices; i++)
    {
        list<int> new_list;
        adj_list.push_back(new_list);
    }
    int start, end;
    for(int i=0; i<num_edges; i++)
    {
        cin >> start >> end;
        adj_list[start].push_back(end);
    }
}

vector<int> Graph::DFS_for_reach(int source) {
    map<int, int> marked;
    stack<int> vertex_stack;
    vector<int> reach;
    vertex_stack.push(source);
    marked[source] = 1;
    while(!vertex_stack.empty()) {
        int vertex = vertex_stack.top();
        vertex_stack.pop();
        for (list<int>::iterator it = adj_list[vertex].begin(); it != adj_list[vertex].end(); it++) {
            if (marked[*it]==0) {
                marked[*it] = 1;
                vertex_stack.push(*it);
                reach.push_back(*it);
            }
        }
    }
    return reach;
}

vector<pair<int, int>> Graph::generate_DFS_tree(int source) {
    map<int, int> marked;
    stack<int> vertex_stack;
    vector<pair<int, int>> tree;
    vertex_stack.push(source);
    marked[source] = 1;
    while(!vertex_stack.empty()) {
        int vertex = vertex_stack.top();
        vertex_stack.pop();
        for (list<int>::iterator it = adj_list[vertex].begin(); it != adj_list[vertex].end(); it++) {
            if (marked[*it]==0) {
                marked[*it] = 1;
                vertex_stack.push(*it);
                tree.push_back(make_pair(vertex, *it));
            }
        }
    }
    return tree;
}

bool Graph::DFS_for_cycle_detection() {
    map<int, int> start;
    map<int, int> end;
    stack<int> vertex_stack;
    int num_ended = 0;
    while (num_ended != num_vertices) {
        for (int i = 1; i <= num_vertices; i++) {
            if (start[i] == 0) {
                vertex_stack.push(i);
                break;
            }
        }
        while (!vertex_stack.empty()) {
            int vertex = vertex_stack.top();
            if(start[vertex]==0) {
                start[vertex] = 1;
                for (list<int>::iterator it = adj_list[vertex].begin(); it != adj_list[vertex].end(); it++) {
                    if (start[*it] == 0) vertex_stack.push(*it);
                    else {
                        if(end[*it]==0) return true;
                    }
                }
            }
            else
            {
                if(end[vertex]==0)
                {
                    end[vertex] = 1;
                    num_ended++;
                }
                vertex_stack.pop();
            }
        }
    }
    return false;
}

vector<int> Graph::DFS_for_topological_order() {
    vector<int> topological_order;
    vector<pair<int, int>> parent; // (v, parent of v)
    vector<pair<int, int>> start_time; // (v, start time of v)
    vector<pair<int, int>> end_time; // (v, end time of v)

    for(int i=0; i<=num_vertices; i++) {
        start_time.push_back(make_pair(i, 0));
        end_time.push_back(make_pair(i, 0));
    }

    stack<int> vertex_stack;
    int num_ended = 0, clock = 0, num_components = 0;
    while (num_ended != num_vertices) {
        for (int i = 1; i <= num_vertices; i++) {
            if (start_time[i].second == 0) {
                parent[i].second = -1;
                vertex_stack.push(i);
                num_components++;
                break;
            }
        }
        while (!vertex_stack.empty()) {
            int vertex = vertex_stack.top();
            if(start_time[vertex].second==0) {
                clock++;
                start_time[vertex].second = clock;
                for (list<int>::iterator it = adj_list[vertex].begin(); it != adj_list[vertex].end(); it++) {
                    if (start_time[*it].second == 0) {
                        parent[*it].second = vertex;
                        vertex_stack.push(*it);
                    }
                    else {
                        if(end_time[*it].second==0) {
                            topological_order.clear();
                            return topological_order;
                        }
                    }
                }
            }
            else
            {
                if(end_time[vertex].second==0)
                {
                    clock++;
                    end_time[vertex].second = clock;
                    num_ended++;
                }
                vertex_stack.pop();
            }
        }
    }

    //using lambda functions to sort a vector of pairs
    sort(end_time.begin(), end_time.end(),
         [](const pair<int, int> a, pair<int, int> b) -> bool
    {
        return a.second<b.second;
    });
    for(int i=0; i<end_time.size(); i++)
    {
        if(end_time[i].first!=0) topological_order.push_back(end_time[i].first);
    }
    return topological_order;
}

void Graph::DFS_for_edge_classification() {
    vector<int> parent(num_vertices+1, 0); // (v, parent of v)
    vector<int> start_time(num_vertices+1, 0); // (v, start time of v)
    vector<int> end_time(num_vertices+1, 0); // (v, end time of v)
    stack<int> vertex_stack;
    vector<pair<int, int>> tree_edge, forward_edge, back_edge, cross_edge;

    int num_ended = 0, clock = 0;
    while (num_ended != num_vertices) {
        for (int i = 1; i <= num_vertices; i++) {
            if (start_time[i] == 0) {
                parent[i] = -1;
                vertex_stack.push(i);
                break;
            }
        }
        while (!vertex_stack.empty()) {
            int vertex = vertex_stack.top();
            if (start_time[vertex] == 0) {
                clock++;
                start_time[vertex] = clock;
                for (list<int>::iterator it = adj_list[vertex].begin(); it != adj_list[vertex].end(); it++) {
                    if (start_time[*it] == 0) {
                        parent[*it] = vertex;
                        vertex_stack.push(*it);
                    }
                    else {
                        if(end_time[*it]==0)
                        {
                            back_edge.push_back(make_pair(vertex, *it));
                        }
                    }
                }
            } else {
                if (end_time[vertex] == 0) {
                    clock++;
                    end_time[vertex] = clock;
                    num_ended++;
                }
                vertex_stack.pop();
            }
        }

        for (int i = 1; i <= num_vertices; i++) {
            int start = i;
            for (list<int>::iterator it = adj_list[start].begin(); it != adj_list[start].end(); it++) {
                int end = *it;
                if (
                        start_time[start] < start_time[end] &&
                        start_time[end] < end_time[end] &&
                        end_time[end] < end_time[start] &&
                        parent[end] == start
                )
                    tree_edge.push_back(make_pair(start, end));
                else if (
                        start_time[start] < start_time[end] &&
                        start_time[end] < end_time[end] &&
                        end_time[end] < end_time[start] &&
                        parent[end] != start
                        )
                    forward_edge.push_back(make_pair(start, end));
                else if (end_time[end] < start_time[start]) cross_edge.push_back(make_pair(start, end));
            }
        }
    }
    for(int i=0; i<start_time.size(); i++) cout << start_time[i] << " ";
    cout << endl;
    for(int i=0; i<end_time.size(); i++) cout << end_time[i] << " ";
    cout << endl;
    for(int i=0; i<tree_edge.size(); i++)
    {
        cout << "Tree Edge: " << tree_edge[i].first << "->" << tree_edge[i].second << endl;
    }
    for(int i=0; i<forward_edge.size(); i++)
    {
        cout << "Forward Edge: " << forward_edge[i].first << "->" << forward_edge[i].second << endl;
    }
    for(int i=0; i<back_edge.size(); i++)
    {
        cout << "Back Edge: " << back_edge[i].first << "->" << back_edge[i].second << endl;
    }
    for(int i=0; i<cross_edge.size(); i++)
    {
        cout << "Cross Edge: " << cross_edge[i].first << "->" << cross_edge[i].second << endl;
    }
}
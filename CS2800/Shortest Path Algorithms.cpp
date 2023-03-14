/*
 * Code for shortes path algorithms in graphs
 * Dijkstra's Algorithm
 * Bellman-Ford Algorithm
 * While Dijkstra's algorithm does not work for graphs with negative edge weights,
 * Bellman-Ford Algorithm works in all cases but is computationally inefficient
*/

#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <iterator>
#include <limits>

using namespace std;

class Graph
{
private:
    vector<list<int>> adj_list; // (v, list of neighbours of v)
    map<pair<int, int>, int> edge_weights; // ((start, end), weight of edge)
    int num_vertices, num_edges;

public:
    Graph() : num_edges(0) , num_vertices(0) {}

    void build_weighted_undirected_graph();
    void build_weighted_directed_graph();
    void build_unweighted_undirected_graph();
    void build_unweighted_directed_graph();

    vector<int> shortest_distance_BFS(int start);
    vector<int> shortest_path_BFS(int start, int end);
    vector<int> shortest_distance_dijkstra(int start);
    vector<int> shortest_path_dijkstra(int start, int end);
    vector<int> shortest_distance_bellman_ford(int start);
    vector<int> shortest_path_bellman_ford(int start, int end);
};

void Graph::build_weighted_undirected_graph() {
    cin >> num_vertices >> num_edges;
    for(int i=0; i<=num_vertices; i++)
    {
        list<int> new_list;
        adj_list.push_back(new_list);
    }
    int start, end, weight;
    for(int i=0; i<num_edges; i++)
    {
        cin >> start >> end >> weight;
        adj_list[start].push_back(end);
        adj_list[end].push_back(start);
        edge_weights.insert({make_pair(start, end), weight});
        edge_weights.insert({make_pair(end, start), weight});
    }
}

void Graph::build_weighted_directed_graph() {
    cout << "HERE" << endl;
    cin >> num_vertices >> num_edges;
    cout << "Num ver: " <<  num_vertices << " Num edges: " << num_edges << endl;
    for(int i=0; i<=num_vertices; i++)
    {
        list<int> new_list;
        adj_list.push_back(new_list);
    }
    int start, end, weight;
    for(int i=0; i<num_edges; i++)
    {
        cin >> start >> end >> weight;
        adj_list[start].push_back(end);
        edge_weights.insert({make_pair(start, end), weight});
    }
}

void Graph::build_unweighted_undirected_graph() {
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
        edge_weights.insert({make_pair(start, end), 1});
        edge_weights.insert({make_pair(end, start), 1});
    }
}

void Graph::build_unweighted_directed_graph() {
    cin >> num_vertices >> num_edges;
    for(int i=0; i<=num_vertices; i++)
    {
        list<int> new_list;
        adj_list.push_back(new_list);
    }
    int start, end;
    for(int i=1; i<=num_edges; i++)
    {
        cin >> start >> end;
        adj_list[start].push_back(end);
        edge_weights.insert({make_pair(start, end), 1});
    }
}

vector<int> Graph::shortest_distance_BFS(int start) {
    queue<int> q;
    vector<int> dist(num_vertices+1, INT32_MAX);
    vector<int> parent(num_vertices+1);

    dist[start] = 0;
    q.push(start);

    while(!q.empty()) {
        int v = q.front();
        q.pop();
        for(list<int>::iterator it=adj_list[v].begin(); it!=adj_list[v].end(); it++) {
            int w = *it;
            if(dist[w]==INT32_MAX) {
                dist[w] = dist[v] + 1;
                parent[w] = v;
                q.push(w);
            }
        }
    }

    return dist;
}

vector<int> Graph::shortest_path_BFS(int start, int end) {
    queue<int> q;
    vector<int> dist(num_vertices+1, INT32_MAX);
    vector<int> parent(num_vertices+1);

    dist[start] = 0;
    q.push(start);

    while(!q.empty()) {
        int v = q.front();
        q.pop();
        for(list<int>::iterator it=adj_list[v].begin(); it!=adj_list[v].end(); it++) {
            int w = *it;
            if(dist[w]==INT32_MAX) {
                dist[w] = dist[v] + 1;
                parent[w] = v;
                q.push(w);
            }
        }
    }

    vector<int> path;
    path.push_back(end);
    while(end!=start)
    {
        end = parent[end];
        path.push_back(end);
    }
    reverse(path.begin(), path.end());

    return path;
}

vector<int> Graph::shortest_distance_dijkstra(int start) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // (dist(v), v)
    vector<int> dist(num_vertices+1, INT32_MAX);
    vector<int> parent(num_vertices+1);

    dist[start] = 0;

    pq.push(make_pair(0, start));

    while(!pq.empty())
    {
        pair<int,int> p = pq.top();
        pq.pop();

        int dist_v = p.first;
        int v = p.second;

        for(list<int>::iterator it = adj_list[v].begin(); it!=adj_list[v].end(); it++) {
            int w = *it;
            if(dist[w] > dist[v] + edge_weights[make_pair(v, w)]) {
                dist[w] = dist[v] + edge_weights[make_pair(v, w)];
                parent[w] = v;
                pq.push(make_pair(dist[w], w));
            }
        }
    }
    return dist;
}

vector<int> Graph::shortest_path_dijkstra(int start, int end) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // (dist(v), v)
    vector<int> dist(num_vertices+1, INT32_MAX);
    vector<int> parent(num_vertices+1);

    dist[start] = 0;

    pq.push(make_pair(0, start));

    while(!pq.empty())
    {
        pair<int,int> p = pq.top();
        pq.pop();

        int dist_v = p.first;
        int v = p.second;

        for(list<int>::iterator it = adj_list[v].begin(); it!=adj_list[v].end(); it++) {
            int w = *it;
            if(dist[w] > dist[v] + edge_weights[make_pair(v, w)]) {
                dist[w] = dist[v] + edge_weights[make_pair(v, w)];
                parent[w] = v;
                pq.push(make_pair(dist[w], w));
            }
        }
    }

    vector<int> path;
    path.push_back(end);
    while(end!=start)
    {
        end = parent[end];
        path.push_back(end);
    }
    reverse(path.begin(), path.end());

    return path;
}

vector<int> Graph::shortest_distance_bellman_ford(int start) {
    vector<int> dist(num_vertices+1, INT32_MAX);
    vector<int> parent(num_vertices+1);

    dist[start] = 0;

    for(int i=1; i<num_vertices; i++) {
        for(int u=1; u<=num_vertices; u++) {
            for(list<int>::iterator it=adj_list[u].begin(); it!=adj_list[u].end(); it++) {
                int v = *it;
                if((dist[u]!=INT32_MAX) && (dist[v]==INT32_MAX || dist[v]>dist[u] + edge_weights[make_pair(u, v)])) {
                    dist[v] = dist[u] + edge_weights[make_pair(u, v)];
                    parent[v] = u;
                }
            }
        }
    }

    return dist;
}

vector<int> Graph::shortest_path_bellman_ford(int start, int end) {
    vector<int> dist(num_vertices+1, INT32_MAX);
    vector<int> parent(num_vertices+1);

    dist[start] = 0;

    for(int i=1; i<num_vertices; i++) {
        for(int u=1; u<=num_vertices; u++) {
            for(list<int>::iterator it=adj_list[u].begin(); it!=adj_list[u].end(); it++) {
                int v = *it;
                if((dist[u]!=INT32_MAX) && (dist[v]==INT32_MAX || dist[v]>dist[u] + edge_weights[make_pair(u, v)])) {
                    dist[v] = dist[u] + edge_weights[make_pair(u, v)];
                    parent[v] = u;
                }
            }
        }
    }

    vector<int> path;
    path.push_back(end);
    while(end!=start)
    {
        end = parent[end];
        path.push_back(end);
    }
    reverse(path.begin(), path.end());

    return path;
}

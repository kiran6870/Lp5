#include <iostream>
#include <queue>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

/* ---------- Binary Tree ---------- */

class Node
{
public:
    int data;
    Node *left, *right;

    Node(int val)
    {
        data = val;
        left = right = NULL;
    }
};

// Parallel BFS
void bfs(Node *root)
{
    queue<Node*> q;
    q.push(root);

    while (!q.empty())
    {
        int size = q.size();

        #pragma omp parallel for
        for (int i = 0; i < size; i++)
        {
            Node *temp;

            #pragma omp critical
            {
                temp = q.front();
                q.pop();

                cout << temp->data << " ";
            }

            #pragma omp critical
            {
                if (temp->left)
                    q.push(temp->left);

                if (temp->right)
                    q.push(temp->right);
            }
        }
    }
}

/* ---------- Graph DFS ---------- */

vector<int> graph[10];
bool visited[10];

// Parallel DFS
void dfs(int start)
{
    stack<int> s;
    s.push(start);

    while (!s.empty())
    {
        int node = s.top();
        s.pop();

        if (!visited[node])
        {
            visited[node] = true;

            cout << node << " ";

            #pragma omp parallel for
            for (int i = 0; i < graph[node].size(); i++)
            {
                int adj = graph[node][i];

                if (!visited[adj])
                {
                    #pragma omp critical
                    s.push(adj);
                }
            }
        }
    }
}

/* ---------- Main ---------- */

int main()
{
    // Binary Tree
    Node *root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->left->right = new Node(5);

    cout << "BFS Traversal:\n";
    bfs(root);

    // Graph
    graph[0].push_back(1);
    graph[0].push_back(2);
    graph[1].push_back(3);
    graph[2].push_back(4);

    cout << "\n\nDFS Traversal:\n";
    dfs(0);

    return 0;
}
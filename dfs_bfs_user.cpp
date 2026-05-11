#include <iostream>   // For input and output
#include <stdlib.h>   // For standard library functions
#include <queue>      // For BFS queue
#include <stack>      // For DFS stack
#include <vector>     // For graph adjacency list
#include <omp.h>      // For OpenMP parallel programming

using namespace std;

/*=====================================================
                BINARY TREE SECTION
=====================================================*/

// Node class for Binary Tree
class node
{
public:
    int data;
    node *left, *right;
};

// Function to insert node level-wise in binary tree
node *insert(node *root, int data)
{
    // If tree is empty
    if (!root)
    {
        root = new node;
        root->data = data;
        root->left = NULL;
        root->right = NULL;
        return root;
    }

    queue<node *> q;
    q.push(root);

    while (!q.empty())
    {
        node *temp = q.front();
        q.pop();

        // Insert at left side
        if (temp->left == NULL)
        {
            temp->left = new node;
            temp->left->data = data;
            temp->left->left = NULL;
            temp->left->right = NULL;
            return root;
        }
        else
        {
            q.push(temp->left);
        }

        // Insert at right side
        if (temp->right == NULL)
        {
            temp->right = new node;
            temp->right->data = data;
            temp->right->left = NULL;
            temp->right->right = NULL;
            return root;
        }
        else
        {
            q.push(temp->right);
        }
    }

    return root;
}

// Parallel BFS Traversal
void bfs(node *head)
{
    if (head == NULL)
        return;

    queue<node *> q;
    q.push(head);

    while (!q.empty())
    {
        int qSize = q.size();

        #pragma omp parallel for
        for (int i = 0; i < qSize; i++)
        {
            node *currNode;

            // Critical section for shared queue
            #pragma omp critical
            {
                currNode = q.front();
                q.pop();

                cout << currNode->data << " ";
            }

            // Critical section for pushing children
            #pragma omp critical
            {
                if (currNode->left)
                    q.push(currNode->left);

                if (currNode->right)
                    q.push(currNode->right);
            }
        }
    }
}

/*=====================================================
                    GRAPH SECTION
=====================================================*/

const int MAX = 100000;

vector<int> graph[MAX];
bool visited[MAX];

// Parallel DFS Traversal
void dfs(int start_node)
{
    stack<int> s;
    s.push(start_node);

    while (!s.empty())
    {
        int curr_node = s.top();
        s.pop();

        if (!visited[curr_node])
        {
            visited[curr_node] = true;

            cout << curr_node << " ";

            // Traverse adjacent nodes in parallel
            #pragma omp parallel for
            for (int i = 0; i < graph[curr_node].size(); i++)
            {
                int adj_node = graph[curr_node][i];

                if (!visited[adj_node])
                {
                    // Critical section because stack is shared
                    #pragma omp critical
                    {
                        s.push(adj_node);
                    }
                }
            }
        }
    }
}

/*=====================================================
                    MAIN FUNCTION
=====================================================*/

int main()
{
    int choice;

    do
    {
        cout << "\n========== MENU ==========\n";
        cout << "1. Binary Tree BFS Traversal\n";
        cout << "2. Graph DFS Traversal\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
            {
                node *root = NULL;
                int data;
                char ans;

                // Insert nodes in binary tree
                do
                {
                    cout << "Enter data: ";
                    cin >> data;

                    root = insert(root, data);

                    cout << "Do you want to insert another node? (y/n): ";
                    cin >> ans;

                } while (ans == 'y' || ans == 'Y');

                cout << "\nBFS Traversal:\n";
                bfs(root);

                cout << endl;
                break;
            }

            case 2:
            {
                int n, m, start_node;

                cout << "Enter number of nodes: ";
                cin >> n;

                cout << "Enter number of edges: ";
                cin >> m;

                cout << "Enter starting node: ";
                cin >> start_node;

                cout << "Enter connected node pairs:\n";

                // Input graph edges
                for (int i = 0; i < m; i++)
                {
                    int u, v;
                    cin >> u >> v;

                    graph[u].push_back(v);
                    graph[v].push_back(u);
                }

                // Initialize visited array
                #pragma omp parallel for
                for (int i = 0; i < n; i++)
                {
                    visited[i] = false;
                }

                cout << "\nDFS Traversal:\n";
                dfs(start_node);

                cout << endl;
                break;
            }

            case 3:
            {
                cout << "Program exited.\n";
                break;
            }

            default:
            {
                cout << "Invalid choice!\n";
            }
        }

    } while (choice != 3);

    return 0;
}
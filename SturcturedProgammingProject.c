#include <stdio.h>

/*
    Define a structure for representing a node in the graph.
    It contains:
    - node_id: Unique identifier for the node
    - adj_count: Number of adjacent nodes
    - adj_nodes: Array to store adjacent node IDs
    - costs: Array to store costs of edges to adjacent nodes
*/
struct Node {
    int node_id;         // Unique identifier for the node
    int adj_count;       // Number of adjacent nodes
    int adj_nodes[10];   // Array to store adjacent node IDs
    int costs[10];       // Array to store costs of edges to adjacent nodes
};

/*
    Function to add a new node to the graph.
    Parameters:
    - nodes: Array of nodes representing the graph
    - node_id: ID of the node to be added
    - count: Current count of nodes in the graph
    Returns the updated count of nodes after addition.
*/
int addNode(struct Node *nodes, int node_id, int count) {
    // Check if the node with given ID already exists
    for (int i = 0; i < count; i++) {
        if (nodes[i].node_id == node_id) {
            return count; // Node already exists, return current count
        }
    }
    // If node doesn't exist, add it to the graph
    nodes[count].node_id = node_id;
    nodes[count].adj_count = 0;
    return count + 1;  // Return the updated node count
}

/*
    Function to add an edge between two nodes in the graph.
    Parameters:
    - nodes: Array of nodes representing the graph
    - node_id1: ID of the first node
    - node_id2: ID of the second node
    - cost: Cost of the edge between the two nodes
    - count: Current count of nodes in the graph
*/
void addAdjacent(struct Node *nodes, int node_id1, int node_id2, int cost, int count) {
    int index = 0;
    // Find the index of the node with ID node_id1 in the node array
    for (int i = 0; i < count; i++) {
        if (nodes[i].node_id == node_id1) {
            index = i;
            break;
        }
    }
    // Add node_id2 as an adjacent node to node_id1
    nodes[index].adj_nodes[nodes[index].adj_count] = node_id2;
    nodes[index].costs[nodes[index].adj_count] = cost;
    nodes[index].adj_count++;    // Increment the adjacent count for node_id1
}

/*
    Function to check if a node is already present in a list.
    Parameters:
    - list: Array representing the list of nodes
    - list_count: Number of nodes in the list
    - node_id: ID of the node to check for
    Returns 1 if the node is present, otherwise 0.
*/
int isNodeAdded(int *list, int list_count, int node_id) {
    for (int i = 0; i < list_count; i++) {
        if (list[i] == node_id) {
            return 1;   // Node is already present in the list
        }
    }
    return 0;   // Node is not present in the list
}

// Counter to track the number of valid paths found
int path_count = 0;

/*
    Function to recursively find paths between two nodes in the graph.
    Parameters:
    - nodes: Array of nodes representing the graph
    - node_count: Number of nodes in the graph
    - start: ID of the starting node
    - end: ID of the destination node
    - path: Array to store the current path
    - cost_list: Array to store the cost of each edge in the path
    - path_count: Current count of nodes in the path
    - forbidden_node: ID of the forbidden node
*/
void findPath(struct Node *nodes, int node_count, int start, int end, int *path, int *cost_list, int path_count, int forbidden_node) {
    int index = 0;
    // Check if the current node is the destination node
    if (path[path_count - 1] == end) {
        int total_cost = 0;
        int forbidden_present = 0;
        // Check if the forbidden node is present in the path
        for (int i = 0; i < path_count; i++) {
            if (path[i] == forbidden_node) {
                forbidden_present = 1; // Forbidden node is present
                break;
            }
            total_cost += cost_list[i];  // Calculate the total cost of the path
        }
        // Print the path excluding the forbidden node
        if (!forbidden_present) {
            printf("\nPath: ");
            for (int i = 0; i < path_count; i++) {
                printf("%d ", path[i]);
            }
            // Print the total cost of the path (if applicable)
            if (total_cost != 0) {
                printf("Cost: %d", total_cost);
            }
            path_count++;
        }
        return;
    }
    // Find the index of the current node in the node array
    for (int i = 0; i < node_count; i++) {
        if (nodes[i].node_id == start) {
            index = i;
            break;
        }
    }
    // Explore adjacent nodes recursively
    for (int i = 0; i < nodes[index].adj_count; i++) {
        int adjacent_added = isNodeAdded(path, path_count, nodes[index].adj_nodes[i]);
        if (!adjacent_added) {
            path[path_count] = nodes[index].adj_nodes[i];
            cost_list[path_count] = nodes[index].costs[i];
            path_count++;
            findPath(nodes, node_count, nodes[index].adj_nodes[i], end, path, cost_list, path_count, forbidden_node);
            path_count--;
        }
    }
}

int main() {
    struct Node nodes[50];  // Array to store nodes in the graph
    int node_count = 0;     // Counter for the number of nodes in the graph
    int start, end;         // Variables to store user input for start and end nodes
    int forbidden_node;     // Variable to store the forbidden node

    // Loop to input node IDs and edge costs until termination signal is given
    int n1, n2, cost;
    while (1) {
        printf("Enter node 1, node 2, and edge cost (-9 -9 -9 to terminate): ");
        scanf("%d %d %d", &n1, &n2, &cost);
        if (n1 == -9 && n2 == -9 && cost == -9) {
            break;  // Terminate input loop if termination signal is given
        }
        // Add nodes and edges to the graph
        node_count = addNode(nodes, n1, node_count);
        node_count = addNode(nodes, n2, node_count);
        addAdjacent(nodes, n1, n2, cost, node_count);
        addAdjacent(nodes, n2, n1, cost, node_count);
    }

    printf("Enter the start and end node: ");
    scanf("%d %d", &start, &end);

    int path[50], cost_list[50], path_count = 0;
    path[0] = start;
    cost_list[0] = 0;
    path_count = 1;

    printf("Enter the forbidden node: ");
    scanf("%d", &forbidden_node);

    // Find and print paths between the start and end nodes while avoiding the forbidden node
    findPath(nodes, node_count, start, end, path, cost_list, path_count, forbidden_node);
    printf("\nNumber of valid paths: %d", path_count);

    return 0;
}

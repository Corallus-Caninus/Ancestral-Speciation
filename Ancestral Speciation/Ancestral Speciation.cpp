#include <iostream>
#include "network.h"

//spurious tests of network topology operations for
//NEAT based augmenting topologies research.
int main()
{
    std::cout << "Hello World!\n";

    //build topology
    network n = network(3, 2);

    //CHECK NODES
    for (int i = 0; i < n.node_count;i++) {
        std::cout << "acquired node: "
            << n.nodes[i]->nodeId << std::endl;
    }
    std::cout << "finished node check" << std::endl;
    std::cout << std::endl;

    //CHECK EDGES
    for (int i = 0;i < n.innovation;i++) {
        std::cout << "acquired edge:";
        std::cout << n.edges[i]->innovation<<" weight: ";
        std::cout << n.edges[i]->weight;
        std::cout << std::endl;
    }
    std::cout << "finished edge check" << std::endl;
    std::cout << std::endl;

    //CHECK ADD_NODE OPERATION
    n.add_node(n.nodes[0], n.nodes[4], 1.2);
    for (int i = 0; i < n.node_count;i++) {
        std::cout << "node: "
            << n.nodes[i]->nodeId << std::endl;
    }
    std::cout << "finished add_node operation check"
        <<std::endl;
    std::cout << std::endl;

    //CHECK ADD_CONNECTION OPERATION
    n.add_connection(n.nodes[4], n.nodes[0], 1.1);
    for (int i = 0;i < n.innovation;i++) {
        std::cout << "edge:";
        std::cout << n.edges[i]->innovation<<" weight: ";
        std::cout << n.edges[i]->weight;
        std::cout << std::endl;
    }
    std::cout << "finished add_connection operation check"
        <<std::endl;
    std::cout << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

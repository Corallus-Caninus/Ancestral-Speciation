#include <iostream>
#include <random>
#include "network.h"

//TODO: push a commit with network (not genome) when finished then
//      begin genome testing. need to get on top of commits and
//      commit more frequently. (each commit should be a shave
//      goal to save large deltas in repo and my face).

//TODO: throwing with optimzation O1 space<-speed tradeoff
//      access to local variable read exception. possibly
//      loop optimization creating access violation.
//                verify this still occurs

//TODO: profile the heap for memory leaks lots of
//      (awesome) pointer strangeness I think I did it more
//      right than wrong (toes crossed)
//TODO: spend like 12 hours straight debugging this 
//      and rework at least like 30% of it (hopefully).

//UPCOMING TEST SUITE:
//  1.test forward propagation with proper breakpoint deubbing
//          DONT USE PRINT STATEMENTS. find stack/heap tracing
//          tools in VS to go along with breakpoints like 
//          prior jvm-eclipse.
//  2. write a simple network that tests basic forward prop.
//  3. ensure repeatable by >=2 pass forward prop. This tests .signals
//      and .loaded primarily.
//  4. augment the network from 2 with a recurrent connection
//  5. ensure recurrence is detected properly.
//  6. augment the network from 4 with a scaled up network to some
//      large strongly induced iteration

const int scale_out = 10; //size to scale out test operations. 

int main()
{
    std::cout << "Hello World!\n";
    //leave on the stack for now
    //in implementing class (e.g.: genepool)
    //allocate and deallocate on heap.
    std::random_device rd{};
    std::mt19937* twister = new std::mt19937(rd());  

    //build topology
    network n = network(3, 2, *twister);

    //CHECK NODES
    for (int i = 0; i < n.node_count;i++) {
        std::cout << "acquired node: "
            << n.nodes[i]->nodeId << std::endl;
    }
    std::cout << "finished node check" << std::endl;
    std::cout << std::endl;

    //CHECK EDGES
    for (int i = 0;i < n.edge_count;i++) {
        std::cout << "acquired edge:";
        std::cout << n.edges[i]->innovation<<" weight: ";
        std::cout << n.edges[i]->weight;
        std::cout << std::endl;
    }
    std::cout << "finished edge check" << std::endl;
    std::cout << std::endl;

    //CHECK ADD_NODE OPERATION
    //n.add_node(n.nodes[0], n.nodes[4], 1.2);
    n.add_node(n.edges[0], 1.2);
    for (int i = 0; i < n.node_count;i++) {
        std::cout << "node: "
            << n.nodes[i] << " " << n.nodes[i]->nodeId << std::endl;
    }
    std::cout << "finished add_node operation check"
        <<std::endl;
    std::cout << std::endl;

    //CHECK ADD_CONNECTION OPERATION
    std::cout << n.node_count << std::endl;
    for (int i = 0;i < n.node_count;i++) {
        std::cout << "("<<n.nodes[i] << ":" << i<<") ";
    }
    std::cout << "finished node addressing check" << std::endl;

    //TODO: broken here.
    n.add_connection(n.nodes[4], n.nodes[0], 1.1); 
    for (int i = 0;i < n.edge_count;i++) {
        std::cout << "edge:";
        std::cout << n.edges[i]->innovation<<" weight: ";
        std::cout << n.edges[i]->weight;
        std::cout << " ";
        std::cout << n.edges[i]->in_node;
        std::cout << " -> ";
        std::cout << n.edges[i]->out_node;
        std::cout << std::endl;
    }
    std::cout << "finished add_connection operation check"
        <<std::endl;
    std::cout << std::endl;

    //TEST SCALE OUT OPERATIONS
    int edge_snapshot = n.edge_count;
    for (int c = 0;c < scale_out;c++) {
		for (int i = 0; i < edge_snapshot;i++) {
			n.add_node(n.edges[i], 1.1);
		}
    }
	/*
    for (int i = 0;i < n.edge_count;i++) {
        std::cout << "edge:";
        std::cout << n.edges[i]->innovation<<" weight: ";
        std::cout << n.edges[i]->weight;
        std::cout << " ";
        std::cout << n.edges[i]->in_node;
        std::cout << " -> ";
        std::cout << n.edges[i]->out_node;
        std::cout << std::endl;
        
    }
    */
    float inputs[3] = {1.0f,2.0f,3.0f};
    //n.forward_propagate(inputs);
    char input;
    std::cout << edge_snapshot << std::endl;
    std::cout << n.edge_count << " " << n.node_count << std::endl;
    std::cout << "finished scaleout operations check" << std::endl;

    // test forward propagation.
    // reuse twister because it is definitely not saturated lol.
    network proper = network(3, 2, *twister);
    
    proper.add_node(proper.edges[1], 1.1f);
    proper.add_node(proper.edges[1], 1.2f);
    proper.add_node(proper.edges[4], 1.4f);
    for (int i = 0; i < scale_out; i++) {
        proper.add_node(proper.edges[0], 1.1f);
    }
    proper.add_connection(proper.nodes[5], proper.nodes[7], 1.1f);
    proper.add_connection(proper.nodes[6], proper.nodes[5], 1.1f);
    proper.forward_propagate(inputs);

    //CLEANUP
    //delete twister;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   0. git good lol.
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

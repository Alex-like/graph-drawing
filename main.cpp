#include <iostream>
#include <sstream>

#include "graph.hpp"
#include "drawing.hpp"

using namespace std;
using namespace graph;
using namespace drawing;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Invalid number of arguments: expected <drawing-api> and <graph-view>.\n";
        return 1;
    }

    DrawingApi *graph_api = nullptr;
    Graph *graph = nullptr;

    if (strcmp(argv[1],"graphviz") == 0) {
        graph_api = new GraphViz("main");
    } else if (strcmp(argv[1],"ogdf") == 0) {
        graph_api = new OpenGraph("main");
    } else {
        cerr << "Invalid argument: choose <drawing-api> from list { graphviz , ogdf }.\n";
        return 2;
    }

    if (strcmp(argv[2],"edge-list") == 0) {
        graph = new ListGraph(graph_api);
    } else if (strcmp(argv[2],"adj-matrix") == 0) {
        graph = new MatrixGraph(graph_api);
    } else {
        delete graph_api;
        cerr << "Invalid argument: choose <graph-view> from list { edge-list , adj-matrix }.\n";
        return 3;
    }

    /*
    3 3
    1 2 3
    1 3
    2 3
    1 2
     */
    graph->readGraph(cin);
    graph->drawGraph();

    delete graph;
    delete graph_api;
    return 0;
}

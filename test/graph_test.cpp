//
// Created by Alex Shchelochkov on 08.12.2022.
//

#include <gtest/gtest.h>
#include <sstream>
#include "graph.hpp"

using namespace std;
using namespace graph;

GTEST_API_ int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

class Graph_test : public testing::TestWithParam<Graph*> {
protected:
    void TearDown() override {
        GetParam()->clear();
    }
};

INSTANTIATE_TEST_SUITE_P(
        graph,
        Graph_test,
        testing::Values(
                new MatrixGraph(nullptr),
                new ListGraph(nullptr)));

TEST_P(Graph_test, empty_graph) {
    EXPECT_EQ(0, GetParam()->vertex_count());
    EXPECT_EQ(0, GetParam()->edge_count());
}

TEST_P(Graph_test, one_vertex_graph) {
    GetParam()->add_vertex(12);
    EXPECT_EQ(1, GetParam()->vertex_count());
    EXPECT_THROW(GetParam()->add_vertex(12), invalid_argument);
}

TEST_P(Graph_test, add_edge_in_empty_graph) {
    EXPECT_THROW(GetParam()->add_edge(1, 2), invalid_argument);
    EXPECT_EQ(0, GetParam()->edge_count());
}

TEST_P(Graph_test, add_some_edges) {
    GetParam()->add_vertex(1);
    GetParam()->add_vertex(2);
    GetParam()->add_edge(1, 2);
    GetParam()->add_edge(2, 1);
    EXPECT_EQ(2, GetParam()->vertex_count());
    EXPECT_EQ(2, GetParam()->edge_count());
    EXPECT_THROW(GetParam()->add_edge(2, 4), invalid_argument);
    EXPECT_THROW(GetParam()->add_edge(1, 1), invalid_argument);
}

TEST(MatrixGraph_test, read_graph) {
    MatrixGraph graph(nullptr);
    stringstream ss;
    ss << "2" << '\n'
    << "1 2" << '\n'
    << "0 1" << '\n'
    << "1 0";
    graph.readGraph(ss);
    EXPECT_EQ(2, graph.vertex_count());
    EXPECT_EQ(2, graph.edge_count());
}

TEST(ListGraph_test, read_graph) {
    ListGraph graph(nullptr);
    stringstream ss;
    ss << "2 2" << '\n'
       << "3 5" << '\n'
       << "3 5" << '\n'
       << "5 3";
    graph.readGraph(ss);
    EXPECT_EQ(2, graph.vertex_count());
    EXPECT_EQ(2, graph.edge_count());
}


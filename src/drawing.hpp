//
// Created by Alex Shchelochkov on 01.12.2022.
//

#ifndef DRAWING_GRAPH_DRAWING_HPP
#define DRAWING_GRAPH_DRAWING_HPP

#include <utility>
#include <vector>
#include <cstdint>
#include <cmath>

#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/fileformats/GraphIO.h>


namespace drawing {
    class DrawingApi {
    public:
        virtual ~DrawingApi() = default;
        virtual void draw_node(size_t id, size_t nodes_amount) = 0;
        virtual void draw_edge(size_t from, size_t to) = 0;
        virtual void run() = 0;
    };

    class OpenGraph : public DrawingApi {
    private:
        std::map<size_t, ogdf::node> nodes;
        std::string name;
        ogdf::Graph G;
        ogdf::GraphAttributes GA;
    public:
        explicit OpenGraph(std::string name = "unknown") :
        name(std::move(name))
        {
            G = ogdf::Graph();
            GA = ogdf::GraphAttributes(G,
                                       ogdf::GraphAttributes::nodeGraphics |
                                       ogdf::GraphAttributes::edgeGraphics |
                                       ogdf::GraphAttributes::nodeLabel |
                                       ogdf::GraphAttributes::nodeStyle |
                                       ogdf::GraphAttributes::nodeLabelPosition);
        }
        ~OpenGraph() = default;
        void draw_node(size_t id, size_t nodes_amount) override {
            ogdf::node new_node = G.newNode();
            double angle = (2 * M_PI / double(nodes_amount)) * double(id);
            double radius = 25. + double(rand() % 100);
            GA.x(new_node) = cos(angle) * radius;
            GA.y(new_node) = sin(angle) * radius;
            GA.width(new_node) = 10;
            GA.height(new_node) = 10;
            GA.label(new_node) = std::to_string(id);
            GA.yLabel(new_node) = 4;
            GA.fillColor(new_node) = ogdf::Color(10, 236, 109);
            nodes.insert({id, new_node});
        }

        void draw_edge(size_t from, size_t to) override {
            ogdf::edge e = G.newEdge(nodes[from],nodes[to]);
            GA.bends(e);
        }

        void run() override {
            ogdf::GraphIO::write(GA, "img/" + name + ".svg", ogdf::GraphIO::drawSVG);
        }
    };

    class GraphViz : public DrawingApi {
    private:
        std::string name;
        std::string graph_def;
    public:
        explicit GraphViz(std::string name = "unknown") :
        name(std::move(name))
        {
            graph_def = R"(node [shape=box, style="filled", color="black", fontname="helvetica"] ;)";
            graph_def += '\n';
            graph_def += R"(edge [fontname="helvetica"] ;)";
        }
        ~GraphViz() = default;
        void draw_node(size_t id, size_t nodes_amount) override {
            graph_def += '\n';
            graph_def += std::to_string(id) + " [label=\"" + std::to_string(id) + R"(", fillcolor="blue"] ;)";
        }
        void draw_edge(size_t from, size_t to) override {
            graph_def += '\n';
            graph_def += std::to_string(from) + " -> " + std::to_string(to) + " ;";
        }
        void run() override {
            graph_def = "digraph {\n" + graph_def + "\n}";
            std::string cmd = "echo '" + graph_def + "' | dot -Tpng -o img/" + name + ".png";
            system(cmd.c_str());
//            system(("open img/" + name + ".png").c_str());
        }
    };
}

#endif //DRAWING_GRAPH_DRAWING_HPP

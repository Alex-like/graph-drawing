//
// Created by Alex Shchelochkov on 29.11.2022.
//

#ifndef DRAWING_GRAPH_GRAPH_HPP
#define DRAWING_GRAPH_GRAPH_HPP

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <fstream>
#include <cmath>
#include "drawing.hpp"

namespace graph {
    class Graph {
    protected:
        drawing::DrawingApi *drawingApi;
    public:
        explicit Graph(drawing::DrawingApi *drawingApi) {
            this->drawingApi = drawingApi;
        }
        virtual ~Graph() = default;
        virtual void add_vertex(size_t id) = 0;
        virtual void add_edge(size_t from, size_t to) = 0;
        virtual void readGraph(std::istream &in) = 0;
        virtual void drawGraph() = 0;
        virtual void clear() = 0;
        virtual size_t vertex_count() const = 0;
        virtual size_t edge_count() const = 0;
    };

    class AbstractGraph : public Graph {
    protected:
        std::unordered_set<size_t> vertexes;

        explicit AbstractGraph(drawing::DrawingApi *drawingApi) : Graph(drawingApi) {
            vertexes = {};
        }

        void drawVertexes() {
            size_t n = vertexes.size();
            for (const auto &v : vertexes) {
                drawingApi->draw_node(v, n);
            }
        }
        virtual void drawEdges() = 0;
    public:
        void drawGraph() override {
            drawVertexes();
            drawEdges();
            drawingApi->run();
        };
        size_t vertex_count() const override {
            return vertexes.size();
        }
    };

    class MatrixGraph : public AbstractGraph {
    private:
        size_t E = 0;
        std::vector<std::vector<size_t>> edges;
        std::unordered_map<size_t, size_t> id_to_index;
    protected:
        void drawEdges() override {
            for (const auto &from : vertexes) {
                for (const auto &to : vertexes) {
                    if (from == to || !edges[id_to_index[from]][id_to_index[to]]) continue;
                    drawingApi->draw_edge(from, to);
                }
            }
        }
    public:
        explicit MatrixGraph(drawing::DrawingApi *drawingApi) : AbstractGraph(drawingApi) {
            edges = {};
            id_to_index = {};
        }
        ~MatrixGraph() {
            clear();
        }
        void add_vertex(size_t id) override {
            if (vertexes.contains(id))
                throw std::invalid_argument("vertex with id=" + std::to_string(id) + " already exists.");
            vertexes.insert(id);
            id_to_index[id] = vertexes.size() - 1;
            for (auto &v : edges)
                v.emplace_back(0);
            edges.emplace_back(std::vector<size_t>(vertexes.size(), 0));
        }
        void add_edge(size_t from, size_t to) override {
            if (from == to)
                throw std::invalid_argument("endings must be different vertexes.");
            if (!vertexes.contains(from))
                throw std::invalid_argument("vertex with id=" + std::to_string(from) + " doesn't exist.");
            if (!vertexes.contains(to))
                throw std::invalid_argument("vertex with id=" + std::to_string(to) + " doesn't exist.");
            edges[id_to_index[from]][id_to_index[to]] += 1;
            E++;
        }
        void readGraph(std::istream &in) override {
            E = 0;
            size_t n;
            in >> n;
            for (size_t i = 0; i < n; i++) {
                size_t id;
                in >> id;
                add_vertex(id);
            }
            for (auto &row : edges)
                for (auto &x : row) {
                    in >> x;
                    if (x > 0) E += x;
                }
        }
        void clear() override {
            for (auto &row : edges)
                row.clear();
            edges.clear();
            E = 0;
            id_to_index.clear();
            vertexes.clear();
        }
        size_t edge_count() const override {
            return E;
        }
    };

    class ListGraph : public AbstractGraph {
    private:
        std::vector<std::pair<size_t, size_t>> edges;
    protected:
        void drawEdges() override {
            for (const auto &edge : edges) {
                const auto &[from, to] = edge;
                drawingApi->draw_edge(from, to);
            }
        }
    public:
        explicit ListGraph(drawing::DrawingApi *drawingApi) : AbstractGraph(drawingApi) {
            edges = {};
        }
        ~ListGraph() {
            clear();
        }
        void add_vertex(size_t id) override {
            if (vertexes.contains(id))
                throw std::invalid_argument("vertex with id=" + std::to_string(id) + " already exists.");
            vertexes.insert(id);
        }
        void add_edge(size_t from, size_t to) override {
            if (from == to)
                throw std::invalid_argument("endings must be different vertexes.");
            if (!vertexes.contains(from))
                throw std::invalid_argument("vertex with id=" + std::to_string(from) + " doesn't exist.");
            if (!vertexes.contains(to))
                throw std::invalid_argument("vertex with id=" + std::to_string(to) + " doesn't exist.");
            edges.emplace_back(from, to);
        }
        void readGraph(std::istream &in) override {
            size_t n, m;
            in >> n >> m;
            for (size_t i = 0; i < n; i++) {
                size_t id;
                in >> id;
                add_vertex(id);
            }
            edges.reserve(m);
            size_t from, to;
            for (size_t i = 0; i < m; i++) {
                in >> from >> to;
                add_edge(from, to);
            }
        }
        void clear() override {
            edges.clear();
            vertexes.clear();
        }
        size_t edge_count() const override {
            return edges.size();
        }

    };
}

#endif //DRAWING_GRAPH_GRAPH_HPP

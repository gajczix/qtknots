//
// Created by Krystyna Gajczyk on 11/06/18.
//

#include "graphs.h"

std::vector<std::vector<int>> getAllCycles(graph_type &graph) {
  // TODO: implement
  // std::vector<std::vector<std::pair<int, int>>> a = graph.edges;
  (void)graph;
  return std::vector<std::vector<int>>();
}

std::vector<std::set<std::pair<int, int>>>
getAllSubsets(std::set<std::pair<int, int>> &candidate_edges) {
  std::vector<std::set<std::pair<int, int>>> result;
  std::vector<std::pair<int, int>> elements_order;
  int n = candidate_edges.size();

  for (auto &edge : candidate_edges) {
    elements_order.push_back(edge);
  }

  for (int i = 0; i < n; i++) { // TODO: change to power od 2
    for (int j = 0; j < n; j++) {
      if ((1 << j) & i) {
      }
    }
  }
  // TODO: implement
  return {candidate_edges};
}

bool isGoodSubset(std::set<std::pair<int, int>> &candidate_edges,
                  std::vector<std::vector<int>> &cycles) {
  for (auto &cycle : cycles) {
    int n = cycle.size();
    int target = (n - 1) / 2;
    int present = 0;
    for (int i = 1; i < n; i++) {
      int beg = std::min(cycle[i - 1], cycle[i]);
      int end = std::max(cycle[i - 1], cycle[i]);
      if (candidate_edges.find(std::make_pair(beg, end)) !=
          candidate_edges.end()) {
        present++;
      }
      if (candidate_edges.find(std::make_pair(end, beg)) !=
          candidate_edges.end()) {
        present++;
      }
      if (present >= target) {
        return false;
      }
    }
  }
  return true;
}

void exlude_multiedges(std::set<std::pair<int, int>> &candidate_edges,
                       graph_type &graph) {
  std::vector<std::pair<int, int>> to_remove;
  for (auto candidate_edge : candidate_edges) {
    int b = candidate_edge.first;
    int e = candidate_edge.second;

    int counter = 0;
    for (auto graph_edge : graph.edges[b]) {
      if (graph_edge.first == e) {
        counter++;
      }
      if (counter > 1) {
        to_remove.push_back(candidate_edge);
        break;
      }
    }
  }

  for (auto rem : to_remove) {
    candidate_edges.erase(rem);
  }
}

int doubleNegativeIndex(graph_type &graph) {

  std::set<std::pair<int, int>> candidate_edges;

  for (unsigned int j = 0; j < graph.edges.size(); j++) {
    if (graph.signs[j] > 0) {
      continue;
    }
    for (auto edge : graph.edges[j]) {
      write_log(std::to_string(edge.second));
      if (edge.second == 0) {
        if (candidate_edges.find(std::make_pair(j, edge.first)) ==
            candidate_edges.end()) {
          candidate_edges.emplace(edge.first, j);
        }
      }
    }
  }

  exlude_multiedges(candidate_edges, graph);

  std::vector<std::vector<int>> cycles = getAllCycles(graph);

  if (isGoodSubset(candidate_edges, cycles)) {
    return candidate_edges.size();
  }

  for (auto &subset : getAllSubsets(candidate_edges)) {
    if (isGoodSubset(subset, cycles)) {
      return subset.size();
    }
  }
  return 0;
}

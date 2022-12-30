#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};

// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};


vector<State> ParseLine(string line) {
  istringstream sline(line);
  int n;
  char c;
  vector<State> row;
  while (sline >> n >> c && c == ',') {
    if (n == 0) {
      row.push_back(State::kEmpty);
    } else {
      row.push_back(State::kObstacle);
    }
  }
  return row;
}


vector<vector<State>> ReadBoardFile(string path) {
  ifstream myfile (path);
  vector<vector<State>> board{};
  if (myfile) {
    string line;
    while (getline(myfile, line)) {
      vector<State> row = ParseLine(line);
      board.push_back(row);
    }
  }
  return board; // The grid.
}

bool Compare(const vector<int> a, const vector<int> b) {
  int f1 = a[2] + a[3];
  int f2 = b[2] + b[3];

  return f1 > f2;
}

/**
 * Sort the two-dimensional vector of ints in descending order.
 */
void CellSort(vector<vector<int>> *v) {
  sort(v->begin(), v->end(), Compare);
}

// Calculate the manhattan distance
int Heuristic(int x1, int y1, int x2, int y2) {
  return abs(x2 - x1) + abs(y2 - y1);
}

bool CheckValidCell(int x, int y, vector<vector<State>> &grid) {
  bool on_grid_x = (x >= 0 && x < grid.size());
  bool on_grid_y = (y >= 0 && y < grid[0].size());

  if (on_grid_x && on_grid_y){
    return grid[x][y] == State::kEmpty;
  }
  return false;
}

// AddToOpen function here.
void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openNodes, vector<vector<State>> &grid){
  vector<int> currentNode = {x, y, g, h};
  
  openNodes.push_back(currentNode);
  grid[x][y] = State::kClosed;
}

/**
 * Expand current nodes's neighbors and add them to the open list.
 */
void ExpandNeighbors(const vector<int> &current, int goal[2], vector<vector<int>> &openlist, vector<vector<State>> &grid){
  // Get current node's data.
  int x = current[0];
  int y = current[1];
  int g = current[2];

  // Loop through current node's potential neighbors.
  for (int i = 0; i < 4; i++) {
    int x2 = x + delta[i][0];
    int y2 = y + delta[i][1];

    // Check that the potential neighbor's x2 and y2 values are on the grid and not closed.
    if (CheckValidCell(x2, y2, grid)) {
      // Increment g value and add neighbor to open list.
      int g2 = g + 1;
      int h2 = Heuristic(x2, y2, goal[0], goal[1]);
      AddToOpen(x2, y2, g2, h2, openlist, grid);
    }
  }
}

// Search function stub here.
vector<vector<State>> Search(vector<vector<State>> grid, int init[2], int goal[2]){
  // Create the vector of open nodes.
  vector<vector<int>> open{};
  // Initialize the starting node.
  int x = init[0];
  int y = init[1];
  
  int g = 0;
  int h = Heuristic(x, y, goal[0], goal[1]);
  
  // Add the starting node to the open vector.
  AddToOpen(x, y, g, h, open, grid);

  // while open vector is non empty {
  while (open.size() > 0){
    // Sort the open list using CellSort, and get the current node.
    CellSort(&open);
    vector<int> currentNode = open.back();
    open.pop_back();

    // Get the x and y values from the current node,
    // and set grid[x][y] to kPath.
    x = currentNode[0];
    y = currentNode[1];
    grid[x][y] = State::kPath;

    // Check if you've reached the goal. If so, return grid.
    if(x == goal[0] && y == goal[1]){
      return grid;
    }

    // If we're not done, expand search to current node's neighbors.
    ExpandNeighbors(currentNode, goal, open, grid);
  }

  // We've run out of new nodes to explore and haven't found a path.
  cout << "No path found!" << endl;
  return vector<vector<State>>{};
}


string CellString(State cell) {
  switch(cell) {
    case State::kObstacle: return "⛰️   ";
    case State::kPath: return "🚗   ";
    default: return "0   "; 
  }
}


void PrintBoard(const vector<vector<State>> board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      cout << CellString(board[i][j]);
    }
    cout << "\n";
  }
}

#include "test.cpp"

int main() {
  int init[2] = {0, 0};
  int goal[2] = {4, 5};
  auto grid = ReadBoardFile("board.txt");

  vector<vector<State>> solution = Search(grid, init, goal);

  PrintBoard(solution);

  // Tests
  TestHeuristic();
  TestAddToOpen();
  TestCompare();
  TestSearch();
  TestCheckValidCell();
  TestExpandNeighbors();
}

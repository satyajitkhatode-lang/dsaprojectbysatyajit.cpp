#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>

using namespace std;

const int VERTEX_COUNT = 6;
const int INF = 1000000000;

vector<string> placeNames;
int road[VERTEX_COUNT][VERTEX_COUNT];

struct DijkstraResult {
    vector<int> distance;
    vector<int> parent;
};

void initializeGraph() {
    placeNames.push_back("Hotel");
    placeNames.push_back("Museum");
    placeNames.push_back("Beach");
    placeNames.push_back("Fort");
    placeNames.push_back("Park");
    placeNames.push_back("Market");

    int i, j;
    for (i = 0; i < VERTEX_COUNT; ++i) {
        for (j = 0; j < VERTEX_COUNT; ++j) {
            road[i][j] = INF;
        }
        road[i][i] = 0;
    }

    // Undirected roads; distances are in kilometres.
    road[0][1] = road[1][0] = 4;  // Hotel - Museum
    road[0][2] = road[2][0] = 7;  // Hotel - Beach
    road[1][2] = road[2][1] = 2;  // Museum - Beach
    road[1][3] = road[3][1] = 5;  // Museum - Fort
    road[2][3] = road[3][2] = 3;  // Beach - Fort
    road[2][4] = road[4][2] = 6;  // Beach - Park
    road[3][4] = road[4][3] = 2;  // Fort - Park
    road[3][5] = road[5][3] = 7;  // Fort - Market
    road[4][5] = road[5][4] = 3;  // Park - Market
}

DijkstraResult dijkstra(int source) {
    DijkstraResult result;
    result.distance.assign(VERTEX_COUNT, INF);
    result.parent.assign(VERTEX_COUNT, -1);
    vector<bool> visited(VERTEX_COUNT, false);
    result.distance[source] = 0;

    int step;
    for (step = 0; step < VERTEX_COUNT; ++step) {
        int u = -1;
        int i;
        for (i = 0; i < VERTEX_COUNT; ++i) {
            if (!visited[i] &&
                (u == -1 || result.distance[i] < result.distance[u])) {
                u = i;
            }
        }

        if (u == -1 || result.distance[u] == INF) {
            break;
        }

        visited[u] = true;
        int v;
        for (v = 0; v < VERTEX_COUNT; ++v) {
            if (road[u][v] != INF &&
                result.distance[u] + road[u][v] < result.distance[v]) {
                result.distance[v] = result.distance[u] + road[u][v];
                result.parent[v] = u;
            }
        }
    }

    return result;
}

void displayDistances(const DijkstraResult& result, int source) {
    cout << "\nShortest distances from " << placeNames[source] << ":\n";
    int i;
    for (i = 0; i < VERTEX_COUNT; ++i) {
        if (i == source) {
            continue;
        }
        cout << "  " << placeNames[i] << ": ";
        if (result.distance[i] == INF) {
            cout << "unreachable\n";
        } else {
            cout << result.distance[i] << " km\n";
        }
    }

    vector<int> reachable;
    for (i = 0; i < VERTEX_COUNT; ++i) {
        if (i != source && result.distance[i] != INF) {
            reachable.push_back(i);
        }
    }

    // Sort by distance, then alphabetically when distances are equal.
    int a;
    for (a = 0; a < static_cast<int>(reachable.size()); ++a) {
        int best = a;
        int b;
        for (b = a + 1; b < static_cast<int>(reachable.size()); ++b) {
            int candidate = reachable[b];
            int currentBest = reachable[best];
            if (result.distance[candidate] < result.distance[currentBest] ||
                (result.distance[candidate] == result.distance[currentBest] &&
                 placeNames[candidate] < placeNames[currentBest])) {
                best = b;
            }
        }
        int temporary = reachable[a];
        reachable[a] = reachable[best];
        reachable[best] = temporary;
    }

    cout << "\nReachable attractions, nearest first:\n";
    if (reachable.empty()) {
        cout << "  None\n";
    }
    for (i = 0; i < static_cast<int>(reachable.size()); ++i) {
        int place = reachable[i];
        cout << "  " << placeNames[place] << " - "
             << result.distance[place] << " km\n";
    }
}

void displayPath(const DijkstraResult& result, int source, int destination) {
    if (result.distance[destination] == INF) {
        cout << "\nNo route exists from " << placeNames[source]
             << " to " << placeNames[destination] << ".\n";
        return;
    }

    vector<int> path;
    int current = destination;
    while (current != -1) {
        path.push_back(current);
        if (current == source) {
            break;
        }
        current = result.parent[current];
    }

    if (path.empty() || path[path.size() - 1] != source) {
        cout << "\nCould not reconstruct the route.\n";
        return;
    }

    reverse(path.begin(), path.end());
    cout << "\nShortest path: ";
    size_t i;
    for (i = 0; i < path.size(); ++i) {
        if (i > 0) {
            cout << " -> ";
        }
        cout << placeNames[path[i]];
    }
    cout << "\nTotal distance: " << result.distance[destination] << " km\n";
}

int main() {
    initializeGraph();
    const int hotel = 0;
    DijkstraResult result = dijkstra(hotel);
    displayDistances(result, hotel);

    cout << "\nSelect a destination:\n";
    int i;
    for (i = 0; i < VERTEX_COUNT; ++i) {
        cout << "  " << i << ". " << placeNames[i] << "\n";
    }

    int destination;
    while (true) {
        cout << "Enter a number from 0 to " << VERTEX_COUNT - 1 << ": ";
        if (cin >> destination && destination >= 0 &&
            destination < VERTEX_COUNT) {
            break;
        }
        cout << "Invalid choice. Please enter a listed number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    displayPath(result, hotel, destination);
    return 0;
}
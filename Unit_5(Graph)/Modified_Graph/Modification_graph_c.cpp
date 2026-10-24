#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

const int VERTEX_COUNT = 6;
const int INF = 1000000000;

vector<string> placeNames;
int road[VERTEX_COUNT][VERTEX_COUNT];

struct DijkstraResult {
    vector<int> distance;
    vector<int> parent;
};

void setRoad(int first, int second, int distance) {
    road[first][second] = distance;
    road[second][first] = distance;
}

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

    // Road distances are in kilometres. Roads are undirected.
    setRoad(0, 1, 4);  // Hotel - Museum
    setRoad(0, 2, 7);  // Hotel - Beach
    setRoad(1, 2, 2);  // Museum - Beach
    setRoad(1, 3, 5);  // Museum - Fort
    setRoad(2, 3, 3);  // Beach - Fort
    setRoad(2, 4, 6);  // Beach - Park
    setRoad(3, 4, 2);  // Fort - Park
    setRoad(3, 5, 7);  // Fort - Market
    setRoad(4, 5, 3);  // Park - Market
}

int chooseVertex(const string& prompt) {
    cout << prompt << "\n";
    int i;
    for (i = 0; i < VERTEX_COUNT; ++i) {
        cout << "  " << i << ". " << placeNames[i] << "\n";
    }

    int choice;
    while (true) {
        cout << "Enter a number from 0 to " << VERTEX_COUNT - 1 << ": ";
        if (cin >> choice && choice >= 0 && choice < VERTEX_COUNT) {
            return choice;
        }
        cout << "Invalid choice. Please enter a listed number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

DijkstraResult dijkstra(int source) {
    DijkstraResult result;
    result.distance.assign(VERTEX_COUNT, INF);
    result.parent.assign(VERTEX_COUNT, -1);
    vector<bool> visited(VERTEX_COUNT, false);
    result.distance[source] = 0;

    int step;
    for (step = 0; step < VERTEX_COUNT; ++step) {
        int current = -1;
        int i;
        for (i = 0; i < VERTEX_COUNT; ++i) {
            if (!visited[i] &&
                (current == -1 || result.distance[i] < result.distance[current])) {
                current = i;
            }
        }

        if (current == -1 || result.distance[current] == INF) {
            break;
        }

        visited[current] = true;
        int next;
        for (next = 0; next < VERTEX_COUNT; ++next) {
            if (road[current][next] != INF &&
                result.distance[current] + road[current][next] <
                    result.distance[next]) {
                result.distance[next] =
                    result.distance[current] + road[current][next];
                result.parent[next] = current;
            }
        }
    }

    return result;
}

void displayResults(const DijkstraResult& result, int source) {
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

int main() {
    initializeGraph();

    cout << "Road endpoints for the update:\n";
    int first = chooseVertex("Select the first endpoint:");
    int second = chooseVertex("Select the second endpoint:");

    while (first == second || road[first][second] == INF) {
        if (first == second) {
            cout << "The endpoints must be different.\n";
        } else {
            cout << "No road exists between those vertices. Choose an existing road.\n";
        }
        first = chooseVertex("Select the first endpoint:");
        second = chooseVertex("Select the second endpoint:");
    }

    int newDistance;
    while (true) {
        cout << "Enter the new non-negative road distance in km: ";
        if (cin >> newDistance && newDistance >= 0) {
            break;
        }
        cout << "Invalid distance. Enter a non-negative whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    setRoad(first, second, newDistance);
    cout << "Road updated between " << placeNames[first] << " and "
         << placeNames[second] << ".\n";

    const int hotel = 0;
    DijkstraResult result = dijkstra(hotel);
    displayResults(result, hotel);
    return 0;
}
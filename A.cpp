#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <unordered_map>

using namespace std;

// Step 1: Define the 3D distance matrix with the shortest route distances for each city pair
const int INF = INT_MAX;
const int CITIES = 4;
const int ROUTES = 5;
int distances[CITIES][CITIES][ROUTES] = {
    {// Islamabad
     {0, 0, 0, 0, 0},
     {300, 300, 300, 300, 300},
     {250, 250, 250, 250, 250},
     {500, 500, 500, 500, 500}},
    {// Lahore
     {300, 300, 300, 300, 300},
     {20, 38, 51, 11, 8},
     {89, 92, 101, 42, 39},
     {150, 172, 158, 111, 62}},
    {// Gujranwala
     {250, 250, 250, 250, 250},
     {89, 92, 101, 42, 39},
     {11, 15, 9, 2, 4},
     {69, 71, 77, 89, 85}},
    {// Karachi
     {500, 500, 500, 500, 500},
     {150, 172, 158, 111, 62},
     {69, 71, 77, 89, 85},
     {22, 28, 59, 51, 72}}};

// Step 2: Compute the minimum distance matrix
int min_distances[CITIES][CITIES];

// Initialize the minimum distances
void initializeMinDistances()
{
    for (int i = 0; i < CITIES; ++i)
    {
        for (int j = 0; j < CITIES; ++j)
        {
            int min_dist = INF;
            for (int k = 0; k < ROUTES; ++k)
            {
                min_dist = min(min_dist, distances[i][j][k]);
            }
            min_distances[i][j] = min_dist;
        }
    }
}

// Step 3: Solve TSP with bitmasking and memoization
unordered_map<int, unordered_map<int, int>> memo;

int tsp(int pos, int visited, const vector<int> &cities)
{
    if (visited == (1 << cities.size()) - 1)
    {
        return min_distances[pos][0]; // Return to the starting city (Islamabad)
    }

    if (memo[pos].count(visited))
        return memo[pos][visited];

    int min_cost = INF;
    for (int i = 0; i < cities.size(); ++i)
    {
        if (!(visited & (1 << i)))
        {
            int next_city = cities[i];
            int cost = min_distances[pos][next_city] + tsp(next_city, visited | (1 << i), cities);
            min_cost = min(min_cost, cost);
        }
    }

    return memo[pos][visited] = min_cost;
}

// Step 4: Find and print the optimal route
void findOptimalRoute(string start_city, vector<string> cities_to_visit)
{
    // Map city names to indices
    unordered_map<string, int> city_indices = {
        {"Islamabad", 0},
        {"Lahore", 1},
        {"Gujranwala", 2},
        {"Karachi", 3}};

    int start = city_indices[start_city];
    vector<int> cities;
    for (const auto &city : cities_to_visit)
    {
        cities.push_back(city_indices[city]);
    }

    // Initialize minimum distances and memoization table
    initializeMinDistances();
    memo.clear();

    // Compute the minimum distance using TSP
    int min_distance = tsp(start, 1 << start, cities);

    // Display the results
    cout << "Optimal Route: " << start_city;
    for (const auto &city : cities_to_visit)
    {
        cout << " -> " << city;
    }
    cout << " -> " << start_city << endl;
    cout << "Minimum Distance: " << min_distance << " km" << endl;
    cout << "Thank you for using the City Route Planner. Safe travels!" << endl;
}

int main()
{
    // Example usage
    string start_city = "Islamabad";
    vector<string> cities_to_visit = {"Lahore", "Gujranwala"};
    findOptimalRoute(start_city, cities_to_visit);
    return 0;
}

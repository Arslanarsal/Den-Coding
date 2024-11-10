#include <bits/stdc++.h>
using namespace std;

const int INF = INT_MAX;
const int MAX_CITIES = 4;

vector<string> cities = {"Islamabad", "Lahore", "Gujranwala", "Karachi"};

int dist[MAX_CITIES][MAX_CITIES][5] = {
    {{0, 0, 0, 0, 0}, {300, 300, 300, 300, 300}, {250, 250, 250, 250, 250}, {500, 500, 500, 500, 500}}, // Islamabad
    {{300, 300, 300, 300, 300}, {20, 38, 51, 11, 8}, {89, 92, 101, 42, 39}, {150, 172, 158, 111, 62}},  // Lahore
    {{250, 250, 250, 250, 250}, {89, 92, 101, 42, 39}, {11, 15, 9, 2, 4}, {69, 71, 77, 89, 85}},        // Gujranwala
    {{500, 500, 500, 500, 500}, {150, 172, 158, 111, 62}, {69, 71, 77, 89, 85}, {22, 28, 59, 51, 72}}   // Karachi
};

// Function to find the minimum distance using dynamic programming (TSP)
int tsp(int mask, int pos, vector<vector<int>> &dp, vector<vector<int>> &path)
{
    if (mask == (1 << MAX_CITIES) - 1)
    {
        return dist[pos][0][0];
    }

    if (dp[mask][pos] != -1)
    {
        return dp[mask][pos];
    }

    int ans = INF;
    int best_city = -1;
    for (int city = 0; city < MAX_CITIES; city++)
    {
        if (!(mask & (1 << city)))
        {
            int new_mask = mask | (1 << city);
            int new_dist = dist[pos][city][0];
            int potential = new_dist + tsp(new_mask, city, dp, path);
            if (potential < ans)
            {
                ans = potential;
                best_city = city;
            }
        }
    }

    path[mask][pos] = best_city;
    return dp[mask][pos] = ans;
}

void print_path(int mask, int pos, vector<vector<int>> &path)
{
    cout << cities[pos];
    mask |= (1 << pos);
    while (true)
    {
        int next_city = path[mask][pos];
        if (next_city == -1)
            break;
        cout << " -> " << cities[next_city];
        mask |= (1 << next_city);
        pos = next_city;
    }
    cout << endl;
}

int main()
{
    for (int i = 0; i < cities.size(); i++)
    {
        cout << cities[i] << " , ";
    }
    cout << endl;
    string start_city;
    cout << "Please enter the starting city: ";
    cin >> start_city;

    int start_index = -1;
    for (int i = 0; i < cities.size(); i++)
    {
        if (cities[i] == start_city)
        {
            start_index = i;
            break;
        }
    }

    if (start_index == -1)
    {
        cout << "Invalid city name. Exiting." << endl;
        return 0;
    }

    vector<string> visit_cities;
    cout << "Enter the list of cities you wish to visit ( Lahore, Gujranwala, Karachi): ";
    string input;
    cin.ignore();
    getline(cin, input);
    stringstream ss(input);
    string city;
    while (getline(ss, city, ','))
    {
        visit_cities.push_back(city);
    }

    vector<int> cities_to_visit;
    for (const string &city : visit_cities)
    {
        int city_index = -1;
        for (int i = 0; i < cities.size(); i++)
        {
            if (cities[i] == city)
            {
                city_index = i;
                break;
            }
        }
        if (city_index != -1)
        {
            cities_to_visit.push_back(city_index);
        }
    }

    vector<vector<int>> dp(1 << MAX_CITIES, vector<int>(MAX_CITIES, -1));
    vector<vector<int>> path(1 << MAX_CITIES, vector<int>(MAX_CITIES, -1));

    int min_dist = tsp(1 << start_index, start_index, dp, path);

    cout << "Optimal Route: ";
    print_path(1 << start_index, start_index, path);
    cout << "Minimum Distance: " << min_dist << " km" << endl;

    return 0;
}

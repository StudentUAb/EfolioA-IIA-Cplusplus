#include <iostream>
#include <vector>
#include <set>
#include <deque>
#include <ctime>
#include <algorithm>

using namespace std;

//Função que conta as fronteiras
int count_borders(const std::vector<std::vector<int>>& matrix) {
    int borders = 0;
    int n = matrix.size();
    int m = matrix[0].size();

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (i > 0 && matrix[i][j] != matrix[i - 1][j]) {
                borders += 1;
            }
            if (j > 0 && matrix[i][j] != matrix[i][j - 1]) {
                borders += 1;
            }
        }
    }

    return borders;
}

//Vector de vetores de inteiros
std::tuple<std::vector<std::vector<int>>, 
std::vector<std::pair<std::pair<int, int>, 
std::pair<int, int>>>, int, int, int, double>

//Função que faz a busca em profundidade
dfs(const std::vector<std::vector<int>>& matrix, int max_borders) {
    std::time_t start_time = std::time(nullptr);

    int n = matrix.size();
    int m = matrix[0].size();

    //Set de vetores de vetores de inteiros
    std::set<std::vector<std::vector<int>>> visited;
    std::deque<std::pair<std::vector<std::vector<int>>, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>>> stack = {{matrix, {}}};

    int generations = 0;
    int expansions = 0;

    //Enquanto a pilha não estiver vazia
    while (!stack.empty()) {
        generations += 1;
        auto [current_matrix, actions] = stack.back();
        stack.pop_back();
        int current_borders = count_borders(current_matrix);

        if (current_borders <= max_borders) {
            double processing_time = std::difftime(std::time(nullptr), start_time);
            return {current_matrix, actions, current_borders, generations, expansions, processing_time};
        }
        //Para cada posição da matriz
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                for (const auto& [x, y] : std::vector<std::pair<int, int>>{{i - 1, j}, {i + 1, j}, {i, j - 1}, {i, j + 1}}) {
                    if (0 <= x && x < n && 0 <= y && y < m) {
                        expansions += 1;
                        std::vector<std::vector<int>> new_matrix = current_matrix;
                        std::swap(new_matrix[i][j], new_matrix[x][y]);

                        if (visited.find(new_matrix) == visited.end()) {
                            std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> new_actions = actions;
                            new_actions.push_back({{i, j}, {x, y}});
                            stack.push_back({new_matrix, new_actions});
                            visited.insert(new_matrix);
                        }
                    }
                }
            }
        }
        //Se o tempo for maior que 60 segundos, o programa para
        if (std::difftime(std::time(nullptr), start_time) > 60) {
            break;
        }
    }
    
    double processing_time = std::difftime(std::time(nullptr), start_time);
    return {{}, {}, -1, generations, expansions, processing_time};
}

//Função que imprime a matriz
vector<vector<int>> apply_action(const vector<vector<int>>& matrix, const pair<pair<int, int>, pair<int, int>>& action) {
    vector<vector<int>> new_matrix = matrix;
    int i = action.first.first;
    int j = action.first.second;
    int x = action.second.first;
    int y = action.second.second;
    swap(new_matrix[i][j], new_matrix[x][y]);
    return new_matrix;
}
//Função main
int main() {
    //Vector de vetores de vetores de inteiros com as instâncias
    vector<vector<vector<int>>> instances = {
        {
            {1, 2, 3},
            {1, 2, 2},
            {3, 3, 1}
        },
        {
            {1, 2, 2, 2},
            {1, 2, 1, 1}
        },
        {
            {1, 2, 2, 2},
            {1, 3, 3, 3},
            {1, 2, 1, 1},
            {1, 1, 3, 2}
        },
        {
            {1, 1, 2, 1, 1},
            {2, 2, 1, 2, 1},
            {1, 1, 2, 1, 2},
            {2, 1, 1, 2, 1}
        },
        {
            {1, 2, 2, 2, 2, 1, 2, 2, 2, 2},
            {1, 3, 3, 3, 4, 1, 3, 3, 3, 4},
            {1, 2, 1, 4, 3, 1, 2, 1, 4, 3},
            {1, 4, 4, 4, 3, 1, 4, 4, 4, 3}
        },
        {
            {1, 1, 2, 1, 1, 1, 1, 2, 1, 1},
            {2, 2, 1, 2, 1, 2, 2, 1, 2, 1},
            {1, 1, 2, 1, 2, 1, 1, 2, 1, 2},
            {2, 1, 1, 2, 1, 2, 1, 1, 2, 1}
        },
        {
            {1, 1, 2, 8, 8, 1, 4, 3, 1, 4},
            {2, 2, 1, 8, 3, 8, 4, 3, 2, 1},
            {1, 1, 8, 8, 3, 1, 6, 2, 1, 4},
            {2, 1, 1, 3, 1, 2, 1, 1, 4, 4},
            {1, 7, 7, 3, 1, 1, 5, 6, 4, 4},
            {2, 2, 1, 3, 1, 2, 2, 1, 6, 6},
            {1, 7, 2, 7, 5, 5, 5, 5, 1, 6},
            {2, 7, 7, 7, 1, 5, 5, 1, 6, 6}
        }
    };

    //Peso de cada instância
    vector<int> W1 = {6, 4, 10, 10, 30, 41, 70};
    cout << "Metodo DFS" << endl;

    //Para cada instância, chama a função dfs
    for (size_t idx = 0; idx < instances.size(); ++idx) {
        auto [result_matrix, actions, final_borders, generations, expansions, processing_time] = dfs(instances[idx], W1[idx]);
        cout << "Instancia " << idx + 1 << ":" << endl;
        cout << "Fronteiras iniciais: " << count_borders(instances[idx]) << endl;
        cout << "Numero de geracoes: " << generations << endl;
        cout << "Numero de expansoes:  " << expansions << endl;
        cout << "Tempo de processamento: " << processing_time << " seconds" << endl;
        cout << "\nFronteiras finais: " << final_borders << "\n" << endl;
    }

    return 0;
}


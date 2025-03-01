#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<tuple>

void quickSortFloat(std::vector<float>& arr, int left, int right, std::vector<int>& arrW, std::vector<int>& arrL);
void quickSortInteger(std::vector<int>& arr, int left, int right, std::vector<int>& arrW, std::vector<int>& arrL);

std::map<int, int> weights_test{
     {0, 3}, {1, 1} 
};

std::map<int, int> length_test{
     {0, 5}, {1, 2} 
};

std::tuple<int, std::vector<int>, std::vector<int>> readTaskVec(std::string name){
    int n;
    std::vector<int> weights;
    std::vector<int> lengthes;

    std::string line;
    std::ifstream in(name);
    
    if (in.is_open()){
        std::getline(in, line);
        n = stoi(line);

        while (std::getline(in, line)){
            std::string num = "";
            for(int i  = 0; i < line.length(); i++){
                if(line[i] != ' '){
                    num += line[i];
                }else{
                    weights.push_back(stoi(num));
                    num = "";
                }
            }
            lengthes.push_back(stoi(num));
        }
    }
    in.close();

    return {n, weights, lengthes};
}

void quickSortFloat(std::vector<float>& arr, int left, int right, std::vector<int>& arrW, std::vector<int>& arrL) {
    int i = left, j = right;
    float tmp;
    int tmp2;
    float pivot = arr[(left + right) / 2];
 
    while (i <= j) {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;
        if (i <= j) {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;

            tmp2 = arrW[i];
            arrW[i] = arrW[j];
            arrW[j] = tmp2;

            tmp2 = arrL[i];
            arrL[i] = arrL[j];
            arrL[j] = tmp2;

            i++;
            j--;
        }
    };
 
    if (left < j)
        quickSortFloat(arr, left, j, arrW, arrL);
    if (i < right)
        quickSortFloat(arr, i, right, arrW, arrL);
    
}

void quickSortInteger(std::vector<int>& arr, int left, int right, std::vector<int>& arrW, std::vector<int>& arrL) {
    int i = left, j = right;
    int tmp;
    int tmp2;
    int pivot = arr[(left + right) / 2];
 
    while (i <= j) {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;
        if (i <= j) {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;

            tmp2 = arrW[i];
            arrW[i] = arrW[j];
            arrW[j] = tmp2;

            tmp2 = arrL[i];
            arrL[i] = arrL[j];
            arrL[j] = tmp2;

            i++;
            j--;
        }
    };
 
    if (left < j)
        quickSortInteger(arr, left, j, arrW, arrL);
    if (i < right)
        quickSortInteger(arr, i, right, arrW, arrL);
    
}

long long greedyRatioVec(int n, std::vector<int> weights, std::vector<int> lengthes){
    long long sum = 0;
    std::vector<float> ratio(n);

    for (int i = 0; i < n; i++) {
        ratio[i] = static_cast<float>(weights[i]) / lengthes[i];
    }

    quickSortFloat(ratio, 0, n-1, weights, lengthes);

    long completion_term = 0;
    

    for(int i = n - 1; i >= 0; i--){
        completion_term += lengthes[i];
        sum += weights[i] * completion_term;
    }

    return sum;
}

long long greedyDiffVec(int n, std::vector<int> weights, std::vector<int> lengthes) {
    long long sum = 0;

    std::vector<int> ratio(n);

    for (int i = 0; i < n; i++) {
        ratio[i] = (weights[i] - lengthes[i]);
    }

    quickSortInteger(ratio, 0, n-1, weights, lengthes);

    long completion_term = 0;
    

    for(int i = n - 1; i >= 0; i--){
        completion_term += lengthes[i];
        sum += weights[i] * completion_term;
    }

    return sum;
}

int main() {
    std::cout << std::endl;
    std::tuple<int, std::vector<int>, std::vector<int>> data = readTaskVec("Task1.txt");

    std::cout << greedyRatioVec(std::get<0>(data), std::get<1>(data), std::get<2>(data)) << std::endl;
    std::cout << greedyDiffVec(std::get<0>(data), std::get<1>(data), std::get<2>(data)) << std::endl;

    data = readTaskVec("Task2.txt");

    std::cout << greedyRatioVec(std::get<0>(data), std::get<1>(data), std::get<2>(data)) << std::endl;
    std::cout << greedyDiffVec(std::get<0>(data), std::get<1>(data), std::get<2>(data)) << std::endl;
}
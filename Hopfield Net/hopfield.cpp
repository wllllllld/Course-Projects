#include <iostream>
#include <vector>
#include <fstream>
#include <clocale>


class Neuron {
public:
    enum state_t {UP=1, DOWN=-1} state;

    Neuron(int val) : state(state_t(val)) {}
    Neuron(char val) { set_state(val); }
    
    void set_state(int newState) {
        state = state_t(newState);
    }

    void set_state(char val) {
        state = val== '@'? UP : DOWN;
    }

    char as_char() const {
        return state == UP ? '@' : '.';
    }

};


class HopfieldNetwork {
public:
    HopfieldNetwork(int size) : size(size), weights(size, std::vector<int>(size, 0)) {}

    void train(const std::vector<Neuron>& pattern) {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (i != j) {
                    weights[i][j] += pattern[i].state * pattern[j].state;
                }
            }
        }
    }

    std::vector<Neuron> recall_synchronous(const std::vector<Neuron>& pattern, int maxIterations = 100) {
        std::vector<Neuron> currentPattern = pattern;
        for (int iter = 0; iter < maxIterations; ++iter) {
            bool stable = true;
            for (int i = 0; i < size; ++i) {
                int sum = 0;
                for (int j = 0; j < size; ++j) {
                    sum += weights[i][j] * currentPattern[j].state;
                }
                int newState = sum >= 0 ? 1 : -1;
                if (newState != currentPattern[i].state) {
                    stable = false;
                    currentPattern[i].set_state(newState);
                }
            }
            if (stable){
                break;
            }
        }
        return currentPattern;
    }

    std::vector<Neuron> recall_asynchronous(const std::vector<Neuron>& pattern, int maxIterations = 100) {
    std::vector<Neuron> currentPattern = pattern;
    std::vector<int> newStates(size, 0);

    for (int iter = 0; iter < maxIterations; ++iter) {
        bool stable = true;
        for (int i = 0; i < size; ++i) {
            int sum = 0;
            for (int j = 0; j < size; ++j) {
                sum += weights[i][j] * currentPattern[j].state;
            }
            newStates[i] = sum >= 0 ? 1 : -1;
            if (newStates[i] != currentPattern[i].state) {
                stable = false;
            }
        }

        
        for (int i = 0; i < size; ++i) {
            currentPattern[i].set_state(newStates[i]);
        }

        if (stable) break;
    }

    return currentPattern;
}

private:
    int size;
    std::vector<std::vector<int>> weights;
};

std::vector<Neuron> read_pattern(const std::string &file_path, size_t len) {
    std::vector<Neuron> pattern;
    std::ifstream i_file(file_path);

    size_t i = 0;
    if (i_file.is_open()) {
    while (i++ < len) {
	    char val;
	    i_file >> val;
	    pattern.push_back(Neuron(val));
    }
    } else {
        std::cerr << "The file does not open" << file_path << std::endl;
        exit(1);
    }

    return pattern;
}

void write_pattern(const std::string& file_path, const std::vector<Neuron>& pattern, const int cols) {
    std::ofstream o_file(file_path);

    if (o_file.is_open()) {
        int col = 0;
        for (const auto& neuron : pattern) {
            o_file << neuron.as_char();
            col++;
            if (col == cols) {
                o_file << std::endl;
                col = 0;
            }
        }
        o_file.close();
    } else {
        std::cerr << "Не могу открыть файл для записи " << file_path << std::endl;
        exit(1);
    }
}


int main(int argc, char* argv[]) {
    int width;
    int height;

    if (argc < 4) {
        std::cout << "Invalid parameters." << std::endl;
        std::cout << "Usage: hopfield WIDTH HEIGHT SOURCE_FILE LEARNING_FILE_1 [LEARNE_FILE_N]" << std::endl;
        exit(1);
    }

    int arg_postion = 1;
    width = atoi(argv[arg_postion++]);
    height = atoi(argv[arg_postion++]);

    HopfieldNetwork network(width*height);
    std::cout << "The network initialized" << std::endl;

    std::vector<Neuron> testPattern = read_pattern(argv[arg_postion++], width*height);
    std::cout << "The image for recognition has been read" << std::endl;
    

    std::vector<Neuron> pattern;
    while(arg_postion < argc)
    {
        pattern = read_pattern(argv[arg_postion++], width*height);
        network.train(pattern);

    }

    std::cout << "Network trained" << std::endl;


    
    std::vector<Neuron> recalledPatternSynchr = network.recall_synchronous(testPattern);
    std::vector<Neuron> recalledPatternAsynchr = network.recall_asynchronous(testPattern);
    std::cout << "The image recognized" << std::endl;
    write_pattern("result_synchr.txt", recalledPatternSynchr, width);
    write_pattern("result_asynchr.txt", recalledPatternAsynchr, width);
    std::cout << "The recognized images saved" << std::endl;

    system("pause");
    
    return 0;
}


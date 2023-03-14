#include <iostream>
#include <vector>
#include <random>

class Distribution {
    private:
        std::vector<double> weights;
    
    public:
        Distribution(std::vector<double> arr){
            weights = arr;
        }

    int pick_index(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::discrete_distribution<char> dist(weights.begin(), weights.end());
        int index = dist(gen);

        return index;
    }
};
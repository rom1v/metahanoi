#include <iterator>
#include <iostream>
#include <vector>

class Hanoi {
    using tower = unsigned int;
    using size = unsigned int;

    std::vector<tower> state; // disk number i is on tower state[i]

public:
    Hanoi(std::initializer_list<tower> init) : state(init) {}

    void solve(tower target) {
        printState(); // initial state
        solveRec(state.size(), target);
    }

private:
    void solveRec(size disks, tower target) {
        if (disks == 0) {
            return;
        }
        // the tower of the largest disk at this depth of recursion
        tower &largest = state[state.size() - disks];
        if (largest == target) {
            // the largest disk is already on the target tower
            solveRec(disks - 1, target);
        } else {
            // move disks above the largest to the intermediate tower
            solveRec(disks - 1, other(largest, target));
            // move the largest disk to the target
            largest = target;
            printState();
            // move back the disks on the largest
            solveRec(disks - 1, target);
        }
    }

    void printState() {
        std::copy(state.cbegin(), state.cend(),
                  std::ostream_iterator<tower>(std::cout));
        std::cout << std::endl;
    }

    static inline tower other(tower t1, tower t2) {
        return 3 - t1 - t2;
    }

};

int main() {
    Hanoi{ 0, 0, 0, 0, 0 }.solve(2);
    return 0;
}

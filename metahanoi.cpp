#include <iostream>

using state = unsigned long long;
using tower = unsigned int;
using size = unsigned int;

constexpr tower other(tower t1, tower t2) {
    return 3 - t1 - t2;
}

constexpr state move(state s, tower src, tower target) {
    return s % 3 == src
        ? s / 3 * 3 + target
        : move(s / 3, src, target) * 3 + s % 3;
}

// disk 1 is the smallest, 0 does not exist (is "above" the smallest)
constexpr tower getTower(state s, size disk) {
    return disk == 1
        ? s % 3
        : getTower(s / 3, disk - 1);
}

constexpr state solve(size disks, state s, tower target) {
    return disks == 0
        ? s
        : getTower(s, disks) == target
            ? solve(disks - 1, s, target)
            : solve(disks - 1,
                    move(solve(disks - 1,
                               s,
                               other(getTower(s, disks), target)),
                         getTower(s, disks),
                         target),
                    target);
}

std::ostream &print_state(std::ostream &os, size ndisks, state s) {
    return ndisks ? print_state(os, ndisks - 1, s / 3) << s % 3 : os;
}

int main() {
    constexpr int disks = 5;
    constexpr state result = solve(disks, 0, 2);
    print_state(std::cout, disks, result) << std::endl;
    return 0;
}

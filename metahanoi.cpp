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

std::ostream &print_state(std::ostream &os, size ndisks, state s) {
    return ndisks ? print_state(os, ndisks - 1, s / 3) << s % 3 : os;
}

template <size DISKS, state S, tower SRC, tower TARGET>
struct SolverRec {
    static constexpr tower nextSrc = getTower(S, DISKS - 1);
    static constexpr tower inter = other(SRC, TARGET);
    using rec1 = SolverRec<DISKS - 1, S, nextSrc, inter>;
    static constexpr state value = move(rec1::final_state, SRC, TARGET);
    using rec2 = SolverRec<DISKS - 1, value, inter, TARGET>;
    static constexpr state final_state = rec2::final_state;
};

template <size DISKS, state S, tower TOWER>
struct SolverRec<DISKS, S, TOWER, TOWER> {
    static constexpr tower nextSrc = getTower(S, DISKS - 1);
    using rec = SolverRec<DISKS - 1, S, nextSrc, TOWER>;
    static constexpr state final_state = rec::final_state;
};

template <state S, tower SRC, tower TARGET>
struct SolverRec<1, S, SRC, TARGET> {
    static constexpr state final_state = move(S, SRC, TARGET);
};

template <state S, tower TOWER>
struct SolverRec<1, S, TOWER, TOWER> {
    static constexpr state final_state = S;
};

template <size DISKS, state S, tower TARGET>
struct Solver {
    static constexpr tower src = getTower(S, DISKS);
    using start = SolverRec<DISKS, S, src, TARGET>;
    static constexpr state final_state = start::final_state;
};

int main() {
    constexpr size disks = 5;
    using solver = Solver<disks, 0, 2>;
    print_state(std::cout, disks, solver::final_state) << std::endl;
    return 0;
}

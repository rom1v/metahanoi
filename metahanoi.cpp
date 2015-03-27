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

template <tower ...T>
struct Disks;

template <tower H, tower ...T>
struct Disks<H, T...> {
    static constexpr state count = 1 + sizeof...(T);
    static constexpr state pack(state tmp) {
        return Disks<T...>::pack(tmp * 3 + H);
    }
    static constexpr state packed = pack(0);
};

template <>
struct Disks<> {
    static constexpr state count = 0;
    static constexpr state pack(state tmp) { return tmp; };
    static constexpr state packed = 0;
};

template <size DISKS, state S, tower SRC, tower TARGET>
struct SolverRec {
    static constexpr tower nextSrc = getTower(S, DISKS - 1);
    static constexpr tower inter = other(SRC, TARGET);
    using rec1 = SolverRec<DISKS - 1, S, nextSrc, inter>;
    static constexpr state value = move(rec1::final_state, SRC, TARGET);
    using rec2 = SolverRec<DISKS - 1, value, inter, TARGET>;
    static constexpr state final_state = rec2::final_state;

    static std::ostream &print(std::ostream &os, size ndisks) {
        rec1::print(os, ndisks);
        print_state(os, ndisks, value) << std::endl;
        rec2::print(os, ndisks);
        return os;
    }
};

template <size DISKS, state S, tower TOWER>
struct SolverRec<DISKS, S, TOWER, TOWER> {
    static constexpr tower nextSrc = getTower(S, DISKS - 1);
    using rec = SolverRec<DISKS - 1, S, nextSrc, TOWER>;
    static constexpr state final_state = rec::final_state;

    static std::ostream &print(std::ostream &os, size ndisks) {
        rec::print(os, ndisks);
        return os;
    }
};

template <state S, tower SRC, tower TARGET>
struct SolverRec<1, S, SRC, TARGET> {
    static constexpr state value = move(S, SRC, TARGET);
    static constexpr state final_state = value;

    static std::ostream &print(std::ostream &os, size ndisks) {
        print_state(os, ndisks, value) << std::endl;
        return os;
    }
};

template <state S, tower TOWER>
struct SolverRec<1, S, TOWER, TOWER> {
    static constexpr state value = S;
    static constexpr state final_state = value;

    static std::ostream &print(std::ostream &os, size ndisks) {
        return os;
    }
};

template <size DISKS, state S, tower TARGET>
struct Solver {
    static constexpr tower src = getTower(S, DISKS);
    using start = SolverRec<DISKS, S, src, TARGET>;
    static constexpr state final_state = start::final_state;

    static std::ostream &print(std::ostream &os) {
        print_state(std::cout, DISKS, S) << std::endl; // initial state
        return start::print(os, DISKS);
    }
};

int main() {
    using disks = Disks<0, 0, 0, 0, 0>;
    using solver = Solver<disks::count, disks::packed, 2>;
    solver::print(std::cout);
    return 0;
}

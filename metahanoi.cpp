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

struct ResultNode {
    state value;
    ResultNode *next;

    class iterator {
        const ResultNode *current;
    public:
        iterator(const ResultNode *current) : current(current) {};
        iterator &operator++() { current = current->next; return *this; }
        state operator*() { return current->value; }
        bool operator==(const iterator &o) { return current == o.current; }
        bool operator!=(const iterator &o) { return !(*this == o); }
    };

    iterator begin() const { return iterator(this); }
    iterator end() const { return iterator(nullptr); }
};

template <size DISKS, state S, tower SRC, tower TARGET, ResultNode *AFTER>
struct SolverRec {
    static ResultNode node;
    static constexpr tower nextSrc = getTower(S, DISKS - 1);
    static constexpr tower inter = other(SRC, TARGET);
    using rec1 = SolverRec<DISKS - 1, S, nextSrc, inter, &node>;
    static constexpr state value = move(rec1::final_state, SRC, TARGET);
    using rec2 = SolverRec<DISKS - 1, value, inter, TARGET, AFTER>;
    static constexpr state final_state = rec2::final_state;
    static constexpr ResultNode *first = rec1::first;
    static constexpr ResultNode *next = rec2::first;
};

template <size DISKS, state S, tower SRC, tower TARGET, ResultNode *AFTER>
ResultNode SolverRec<DISKS, S, SRC, TARGET, AFTER>::node = { value, next };

template <size DISKS, state S, tower TOWER, ResultNode *AFTER>
struct SolverRec<DISKS, S, TOWER, TOWER, AFTER> {
    static constexpr tower nextSrc = getTower(S, DISKS - 1);
    using rec = SolverRec<DISKS - 1, S, nextSrc, TOWER, AFTER>;
    static constexpr state final_state = rec::final_state;
    static constexpr ResultNode *first = rec::first;
};

template <state S, tower SRC, tower TARGET, ResultNode *AFTER>
struct SolverRec<1, S, SRC, TARGET, AFTER> {
    static ResultNode node;
    static constexpr state value = move(S, SRC, TARGET);
    static constexpr state final_state = value;
    static constexpr ResultNode *first = &node;
    static constexpr ResultNode *next = AFTER;
};

template <state S, tower SRC, tower TARGET, ResultNode *AFTER>
ResultNode SolverRec<1, S, SRC, TARGET, AFTER>::node = { value, next };

template <state S, tower TOWER, ResultNode *AFTER>
struct SolverRec<1, S, TOWER, TOWER, AFTER> {
    static constexpr state value = S;
    static constexpr state final_state = value;
    static constexpr ResultNode *first = AFTER;
};

template <size DISKS, state S, tower TARGET>
struct Solver {
    static ResultNode list;
    static constexpr tower src = getTower(S, DISKS);
    using start = SolverRec<DISKS, S, src, TARGET, nullptr>;
};

template <size DISKS, state S, tower TARGET>
ResultNode Solver<DISKS, S, TARGET>::list = { S, start::first };

int main() {
    using disks = Disks<0, 0, 0, 0, 0>;
    using solver = Solver<disks::count, disks::packed, 2>;
    for (state s : solver::list) {
        print_state(std::cout, disks::count, s) << std::endl;
    }
    return 0;
}

#include <iostream>
#include <vector>
#include <list>
#include <cassert>
#include <cmath>
#include <algorithm>

using namespace std;

template <typename T>
class Hash_Table_Set {
public:
    vector<list<T>> A;
    size_t size;
    size_t r;
    size_t p;
    size_t a;
    size_t upper;
    size_t lower;

    Hash_Table_Set(size_t r = 200) : r(r), size(0), p(2147483647), a(rand() % (p - 1) + 1) {
        _compute_bounds();
        _resize(0);
    }

    size_t _hash(int k, size_t m) {
        return ((a * k) % p) % m;
    }

    void _compute_bounds() {
        upper = A.size();
        lower = A.size() * 100 * 100 / (r * r);
    }

    void _resize(size_t n) {
        if (lower >= n || n >= upper) {
            size_t f = r / 100;
            if (r % 100) f += 1;

            size_t m = max(n, static_cast<size_t>(1)) * f;
            vector<list<T>> new_A(m);

            for (const auto& x : *this) {
                size_t h = _hash(x, m);
                new_A[h].push_back(x);
            }

            A = new_A;
            _compute_bounds();
        }
    }

    size_t len() const {
        return size;
    }

    class iterator {
    public:
        Hash_Table_Set* table;
        size_t i;
        typename list<T>::iterator inner;

        iterator(Hash_Table_Set* t, size_t index) : table(t), i(index) {
            if (i < table->A.size()) {
                inner = table->A[i].begin();
                _find_next_non_empty();
            }
        }

        void _find_next_non_empty() {
            while (i < table->A.size() && inner == table->A[i].end()) {
                ++i;
                if (i < table->A.size()) {
                    inner = table->A[i].begin();
                }
            }
        }

        iterator& operator++() {
            ++inner;
            _find_next_non_empty();
            return *this;
        }

        bool operator!=(const iterator& other) const {
            return i != other.i || (i < table->A.size() && inner != other.inner);
        }

        T& operator*() {
            return *inner;
        }
    };

    iterator begin() {
        return iterator(this, 0);
    }

    iterator end() {
        return iterator(this, A.size());
    }

    void build(const vector<T>& X) {
        for (const auto& x : X) {
            insert(x);
        }
    }

    bool insert(T x) {
        _resize(size + 1);
        size_t h = _hash(x, A.size());
        A[h].push_back(x);
        ++size;
        return true;
    }

    bool deleteKey(int k) {
        assert(size > 0);
        size_t h = _hash(k, A.size());
        auto it = find(A[h].begin(), A[h].end(), k);
        if (it != A[h].end()) {
            A[h].erase(it);
            --size;
            _resize(size);
            return true;
        }
        return false;
    }

    T find(int k) const {
        size_t h = _hash(k, A.size());
        auto it = find(A[h].begin(), A[h].end(), k);
        if (it != A[h].end()) {
            return *it;
        }
        return T();
    }
};

int main() {
    // Ejemplo de uso
    Hash_Table_Set<int> hashTable;
    hashTable.insert(5);
    hashTable.insert(3);
    hashTable.insert(8);

    // Iterar sobre la tabla
    for (const auto& x : hashTable) {
        cout << x << " ";
    }
    cout << endl;

    // Iterar en orden
    for (auto it = hashTable.begin(); it != hashTable.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    return 0;
}
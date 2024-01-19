#include <iostream>
#include <vector>
#include <list>
#include <cassert>
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

    size_t _hash(int k, size_t m) const {
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
        const Hash_Table_Set* table;
        size_t i;
        typename list<T>::const_iterator inner;

        iterator(const Hash_Table_Set* t, size_t index) : table(t), i(index) {
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

        const T& operator*() const {
            return *inner;
        }
    };

    iterator begin() const {
        return iterator(this, 0);
    }

    iterator end() const {
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
        auto it = std::find(A[h].begin(), A[h].end(), k);
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
        auto it = std::find(A[h].begin(), A[h].end(), k);
        if (it != A[h].end()) {
            return *it;
        }
        return T();
    }
};

////

template<typename T>
class Set
{
private:
//TODO: atributos
    Hash_Table_Set<T> hashSet;
public:
    Set(): hashSet() {};
    void Insert(T data){
        hashSet.insert(data);
    };
    bool Remove(T data){
        return hashSet.deleteKey(data);
    };
    bool Contains(T data) const {
        return hashSet.find(data) != T();
    }
    Set Union(const Set &other){
        Set result;
        for(const auto& i: hashSet){
            result.Insert(i);
        }
        for(const auto& j: other.hashSet)
            if(!result.Contains(j))
                result.Insert(j);
        return result;
    };
    Set Intersect(const Set &other){
        Set result;
        for (const auto& x : hashSet) {
            if (other.Contains(x)) {
                result.Insert(x);
            }
        }
        return result;
    };
    Set Difference(const Set &other)
    {
        Set result;
        for (const auto& x : hashSet) {
            if (!other.Contains(x)) {
                result.Insert(x);
            }
        }
        return result;
    };
    const Hash_Table_Set<T>& getHashSet() const {
        return hashSet;
    }
    ~Set()=default;
};

int main() {
    // Ejemplo de uso
    Set<int> set1;
    set1.Insert(5);
    set1.Insert(3);
    set1.Insert(8);

    Set<int> set2;
    set2.Insert(3);
    set2.Insert(7);
    set2.Insert(8);

    // Operaciones de conjuntos
    Set<int> unionSet =   set1.Union(set2);
    Set<int> intersectSet = set1.Intersect(set2);
    Set<int> differenceSet = set1.Difference(set2);

    // Imprimir resultados
    cout << "Union: ";
    for (const auto& x : unionSet.getHashSet()) {
        cout << x << " ";
    }
    cout << endl;

    cout << "Intersect: ";
    for (const auto& x : intersectSet.getHashSet()) {
        cout << x << " ";
    }
    cout << endl;

    cout << "Difference: ";
    for (const auto& x : differenceSet.getHashSet()) {
        cout << x << " ";
    }
    cout << endl;


    // Ejemplo de uso

    set2.Remove(3);
    set1.Remove(5);

    // Operaciones de conjuntos
    Set<int> unionSet2 =   set1.Union(set2);
    Set<int> intersectSet2 = set1.Intersect(set2);
    Set<int> differenceSet2 = set1.Difference(set2);

    // Imprimir resultados
    cout << "Union: ";
    for (const auto& x : unionSet2.getHashSet()) {
        cout << x << " ";
    }
    cout << endl;

    cout << "Intersect: ";
    for (const auto& x : intersectSet2.getHashSet()) {
        cout << x << " ";
    }
    cout << endl;

    cout << "Difference: ";
    for (const auto& x : differenceSet2.getHashSet()) {
        cout << x << " ";
    }
    cout << endl;







    return 0;
}
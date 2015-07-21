#pragma once

#include "platform.h"
#include "types.h"
#include "vector_def.h"


namespace fgstd {

template<typename K> struct hash_cast
{
    FGSTD_FORCE_INLINE u32 operator()(const K& t) const
    {
        return (u32)(size_t)t;
    }
};


class IAllocator;
extern IAllocator *g_defaultAlloc;

template<typename K, typename V, typename HASH = hash_cast<K>, typename EQ = equal<K> >
class hashmap
{
public:
    typedef K key_type;
    typedef V value_type;
    typedef vector<V> storage_type;

    static const u32 BUCKETS_DEFAULT = 16; // must be power of 2
    static const u32 LOAD_DEFAULT = 5;
    static const u32 noidx = u32(-1);

    hashmap(IAllocator *a, const HASH& h = HASH(), const EQ& eq = EQ());
    hashmap(u32 buckets = BUCKETS_DEFAULT, u32 loadFactor = LOAD_DEFAULT, IAllocator *a = g_defaultAlloc, const HASH& h = HASH(), const EQ& eq = EQ());
    hashmap(const hashmap& o);
#ifdef FGSTD_USE_CPP11
    hashmap(const hashmap&& o);
#endif
    ~hashmap();

    bool insert_keep(const K& key, const V& val); // keeps already existing element, returns true when newly inserted
    bool insert(const K& key, const V& val);    // always insert, return true when when newly inserted

    bool get(const K& key, V& dst) const;
    bool has(const K& key) const;
    V *getp(const K& key); // return ptr to value or NULL if not found
    const V *getp(const K& key) const;

    V& operator[](const K& key);

    void swap(hashmap& o);
#ifdef FGSTD_USE_CPP11
    hashmap& operator= (const hashmap o);
#else
    hashmap& operator= (const hashmap& o);
#endif

    const storage_type& values() const;
    u32 size() const;
    void clear();
    void deallocate();


private:
    u32 _realidx(u32 h, const K& key) const; // hash to index in _values
    V& _insert_hash(u32 h, const K& key, const V& val);


    void _enlargeIfNecessary(u32 newsz);
    void _enlarge();

    struct bucket
    {
        struct keypair
        {
            u32 hash;
            u32 idx;
        };
        vector<keypair> p;
        vector<K> keys;

        void swap(bucket& o);
        void clear();
    };

    vector<bucket> _keys;
    vector<V> _values;
    u32 _loadFactor;
    HASH _hash;
    EQ _eq;

};



}
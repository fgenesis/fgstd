#pragma once

#include "platform.h"
#include "types.h"
#include "vector_def.h"
#include "util.h"


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

template<typename K, typename V, typename HASH = fgstd::hash_cast<K>, typename EQ = fgstd::equal<K> >
class hashmap
{
public:
    typedef K key_type;
    typedef V value_type;
    typedef vector<V> storage_type;

    static const u32 BUCKETS_DEFAULT = 16; // must be power of 2
    static const u32 LOAD_DEFAULT = 5;

    hashmap(IAllocator *a, const HASH& h = HASH(), const EQ& eq = EQ());
    hashmap(u32 buckets = BUCKETS_DEFAULT, u32 loadFactor = LOAD_DEFAULT, IAllocator *a = g_defaultAlloc, const HASH& h = HASH(), const EQ& eq = EQ());
    hashmap(const hashmap& o);
#ifdef FGSTD_USE_CPP11
    hashmap(const hashmap&& o);
#endif
    ~hashmap();

    bool insert_keep(const K& key, const V& val); // keeps already existing element, returns true when newly inserted
    bool insert(const K& key, const V& val);    // always insert, return true when newly inserted

    bool get(const K& key, V& dst) const;
    bool has(const K& key) const;
    V *getp(const K& key); // return ptr to value or NULL if not found
    const V *getp(const K& key) const;

    bool remove(const K& key);
    bool remove(const K& key, V& dst);


    // TODO: erase, which returns iter

    V& operator[](const K& key);

    void swap(hashmap& o);
#ifdef FGSTD_USE_CPP11
    hashmap& operator= (const hashmap o);
#else
    hashmap& operator= (const hashmap& o);
#endif

    u32 size() const;
    bool empty() const;
    void clear();
    void dealloc();

    const storage_type& values() const;


private:
    static const u32 noidx = u32(-1);
    // skip the hash comparison during lookup if the key comparison is small and fast
    static const bool SKIP_HASH_PRECHECK = is_scalar<K>::value && is_simple_op<EQ>::value;

    u32 _dropkey(const u32 h, const K& key);
    u32 _realidx(const u32 h, const K& key) const; // takes key and its hash, returns index in _values or noidx
    V& _insert_hash(const u32 h, const K& key, const V& val);


    void _enlargeIfNecessary(u32 newsz);
    void _enlarge();

    struct bucket
    {
        vector<u32> hashes;
        vector<u32> idxs;
        vector<K> keys;

        void swap(bucket& o);
        void clear();
        void drop(u32 i);
    };

    vector<bucket> _keys;
    vector<u32> _whichbucket;
    vector<V> _values;
    const u32 _loadFactor;
    const u32 _initialBuckets;
    const HASH _hash;
    const EQ _eq;
};



}
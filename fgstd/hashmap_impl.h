#pragma once

#include "hashmap_def.h"
#include "bit.h"
#include "vector_impl.h"
#include "util.h"

namespace fgstd {

template<>
struct hash_cast<const char*>
{
    FGSTD_FORCE_INLINE u32 operator()(const char *s) const
    {
        u32 hash = 0;
        while (*s)
            hash = hash * 101 + *s++;
        return hash;
    }
};

template<typename K, typename V, typename HASH, typename EQ>
void hashmap<K, V, HASH, EQ>::bucket::swap(bucket& o)
{
    fgstd::swap(hashes, o.hashes);
    fgstd::swap(idxs, o.idxs);
    fgstd::swap(keys, o.keys);
}

template<typename K, typename V, typename HASH, typename EQ>
void hashmap<K, V, HASH, EQ>::bucket::clear()
{
    hashes.clear();
    idxs.clear();
    keys.clear();
}

template<typename K, typename V, typename HASH, typename EQ>
hashmap<K, V, HASH, EQ>::hashmap(u32 buckets, u32 loadFactor, IAllocator *a, const HASH& h, const EQ& eq)
    : _keys(a), _values(a), _loadFactor(loadFactor), _initialBuckets(nextPowerOf2(buckets)), _hash(h), _eq(eq)
{
}

template<typename K, typename V, typename HASH, typename EQ>
hashmap<K, V, HASH, EQ>::hashmap(IAllocator *a, const HASH& h, const EQ& eq)
    : _keys(a), _values(a), _loadFactor(LOAD_DEFAULT), _initialBuckets(BUCKETS_DEFAULT), _hash(h), _eq(eq)
{
}

template<typename K, typename V, typename HASH, typename EQ>
hashmap<K, V, HASH, EQ>::hashmap(const hashmap& o)
    : _keys(o._keys), _values(o._values), _loadFactor(o._loadFactor), _initialBuckets(o._initialBuckets), _hash(o._hash), _eq(o._eq)
{
}

#ifdef FGSTD_USE_CPP11
template<typename K, typename V, typename HASH, typename EQ>
hashmap<K, V, HASH, EQ>::hashmap(const hashmap&& o)
: _keys(forward(o._keys))
, _values(forward(o._values))
, _loadFactor(forward(o._loadFactor))
, _initialBuckets(forward(o._initialBuckets))
, _hash(forward(o._hash))
, _eq(forward(o._eq))
{
}
#endif

template<typename K, typename V, typename HASH, typename EQ>
hashmap<K, V, HASH, EQ>::~hashmap()
{
}

#ifdef FGSTD_USE_CPP11
template<typename K, typename V, typename HASH, typename EQ>
hashmap<K, V, HASH, EQ>& hashmap<K, V, HASH, EQ>::operator=(hashmap<K, V, HASH, EQ> o)
{
    o.swap(*this);
    return *this;
}
#else
template<typename K, typename V, typename HASH, typename EQ>
hashmap<K, V, HASH, EQ>& hashmap<K, V, HASH, EQ>::operator=(const hashmap<K, V, HASH, EQ>& o)
{
    if (&v != this)
    {
        clear();
        _keys = o._keys;
        _values = o._values;
        _loadFactor = o._loadFactor;
        _initialBuckets = o._initialBuckets;
        _hash = o._hash;
        _eq = o._eq;
    }
    return *this;
}
#endif

template<typename K, typename V, typename HASH, typename EQ>
void hashmap<K, V, HASH, EQ>::swap(hashmap<K, V, HASH, EQ>& o)
{
    swap(_keys, o._keys);
    swap(_values, o._values);
    swap(_loadFactor, o._loadFactor);
    swap(_initialBuckets, o._initialBuckets);
    swap(_hash, o._hash);
    swap(_eq, o._eq);
}

template<typename K, typename V, typename HASH, typename EQ>
FGSTD_FORCE_INLINE typename const hashmap<K, V, HASH, EQ>::storage_type& hashmap<K, V, HASH, EQ>::values() const
{
    return _values;
}

template<typename K, typename V, typename HASH, typename EQ>
FGSTD_FORCE_INLINE u32 hashmap<K, V, HASH, EQ>::size() const
{
    return _values.size();
}

template<typename K, typename V, typename HASH, typename EQ>
bool hashmap<K, V, HASH, EQ>::insert_keep(const K& key, const V& val)
{
    const u32 h = _hash(key);
    const u32 idx = _realidx(h);
    if (idx != noidx)
        return false;

    _insert_hash(h, key, val);
    return true;
}

template<typename K, typename V, typename HASH, typename EQ>
bool hashmap<K, V, HASH, EQ>::insert(const K& key, const V& val)
{
    const u32 h = _hash(key);
    const u32 idx = _realidx(h);
    if (idx == noidx)
    {
        _insert_hash(h, key, val);
        return true;
    }

    _values[idx] = val;
    return false;
}

template<typename K, typename V, typename HASH, typename EQ>
V& hashmap<K, V, HASH, EQ>::operator[](const K& key)
{
    const u32 h = _hash(key);
    const u32 idx = _realidx(h, key);
    return idx == noidx ? _insert_hash(h, key, V()) : _values[idx];
}


template<typename K, typename V, typename HASH, typename EQ>
bool hashmap<K, V, HASH, EQ>::get(const K& key, V& dst) const
{
    const u32 h = _hash(key);
    const u32 idx = _realidx(h);
    if (idx != noidx)
    {
        dst = _values[idx];
        return true;
    }
    return false;
}

template<typename K, typename V, typename HASH, typename EQ>
bool hashmap<K, V, HASH, EQ>::has(const K& key) const
{
    const u32 h = _hash(key);
    const u32 idx = _realidx(h, key);
    return idx != noidx;
}

template<typename K, typename V, typename HASH, typename EQ>
V *hashmap<K, V, HASH, EQ>::getp(const K& key)
{
    const u32 h = _hash(key);
    const u32 idx = _realidx(h, key);
    return idx != noidx ? addressof(_values[idx]) : NULL;
}

template<typename K, typename V, typename HASH, typename EQ>
FGSTD_FORCE_INLINE const V *hashmap<K, V, HASH, EQ>::getp(const K& key) const
{
    return const_cast<hashmap<K, V, HASH, EQ> >(this)->getp(key);
}



template<typename K, typename V, typename HASH, typename EQ>
V& hashmap<K, V, HASH, EQ>::_insert_hash(u32 h, const K& key, const V& val)
{
    const u32 idx = _values.size();
    _enlargeIfNecessary(idx + 1);
    const u32 hh = h & (_keys.size() - 1);

    _keys[hh].hashes.push_back(h);
    _keys[hh].idxs.push_back(idx);
    _keys[hh].keys.push_back(key);

    return _values.push_back(val);
}


template<typename K, typename V, typename HASH, typename EQ>
u32 hashmap<K, V, HASH, EQ>::_realidx(u32 h, const K& key) const
{
    const u32 ksz = _keys.size();
    if (ksz)
    {
        const u32 hh = h & (ksz - 1);
        const bucket& b = _keys[hh];
        const u32 bsz = b.hashes.size();
        const K * const pkeys = b.keys.data();
        const u32 * const phashes = b.hashes.data();
        for (u32 i = 0; i < bsz; ++i)
            if (phashes[i] == h && _eq(key, pkeys[i]))
            return b.idxs[i];
    }
    return noidx;
}

template<typename K, typename V, typename HASH, typename EQ>
void hashmap<K, V, HASH, EQ>::_enlargeIfNecessary(u32 newsz)
{
    if (newsz > _keys.size() * _loadFactor)
        _enlarge();
}

template<typename K, typename V, typename HASH, typename EQ>
void hashmap<K, V, HASH, EQ>::_enlarge()
{
    const u32 oldsz = _keys.size();
    const u32 newsz = vmax(_initialBuckets, oldsz * 2);
    _keys.resize(newsz);

    const u32 aa = newsz - 1;
    bucket cp;
    for (u32 bid = 0; bid < oldsz; ++bid)
    {
        cp.clear();
        cp.swap(_keys[bid]);
        // _keys[bid] is now empty
        const u32 sz = cp.hashes.size();
        for (u32 j = 0; j < sz; ++j)
        {
            const u32 hash = cp.hashes[j];
            const u32 ii = hash & aa;
            _keys[ii].hashes.push_back(hash);
            _keys[ii].idxs.push_back(cp.idxs[j]);
            _keys[ii].keys.push_back(FGSTD_MOVE(cp.keys[j]));
        }
    }
}


template<typename K, typename V, typename HASH, typename EQ>
void hashmap<K, V, HASH, EQ>::clear()
{
    if (size())
    {
        const u32 sz = _keys.size();
        for (u32 i = 0; i < sz; ++i)
        {
            _keys[i].hashes.clear();
            _keys[i].idxs.clear();
            _keys[i].keys.clear();
        }

        _values.clear();
    }
}

template<typename K, typename V, typename HASH, typename EQ>
void hashmap<K, V, HASH, EQ>::deallocate()
{
    _keys.deallocate();
    _values.deallocate();
}




}


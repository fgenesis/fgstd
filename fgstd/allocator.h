#pragma once

#include <new>
#include "traits.h"

namespace fgstd {

// basic allocation interface that all allocators have to offer
class IAllocator
{
public:
    virtual ~IAllocator() {}
    virtual void *Alloc(size_t size) = 0;
    virtual void Free(void *p) = 0;
};

// virtualizes call to non-virtual allocator
template<typename A>
class VAllocator : public IAllocator
{
public:
    VAllocator(const A& a = A())
        : alloc(a)
    {
    }
    
    virtual ~VAllocator()
    {
    }

    virtual void *Alloc(size_t size)
    {
        return alloc.Alloc(size);
    }
    virtual void Free(void *p)
    {
        return alloc.Free(p);
    }

private:

    A alloc;
};

template<typename T>
class IObjAllocator
{
public:
    typedef T obj_type;
    virtual T *New(size_t size) = 0;
    virtual void Delete(T *p) = 0;
};

template<typename T, typename A>
class ObjectAllocator : protected A
{
    T *New() { return new (_Alloc()) T(); }
    template<typename P0>

    // extra
    T *New(P0& p0) { return new (_Alloc()) T(p0); }
    template<typename P0, typename P1>
    T *New(P0& p0, P1& p1) { return new (_Alloc()) T(p0, p1); }
    template<typename P0, typename P1, typename P2>
    T *New(P0& p0, P1& p1, P2& p2) { return new (_Alloc()) T(p0, p1, p2); }
    template<typename P0, typename P1, typename P2, typename P3>
    T *New(P0& p0, P1& p1, P2& p2, P3& p3) { return new (_Alloc()) T(p0, p1, p2, p3); }

    void Delete(T *p)
    {
        p->~T();
        this->Free(p);
    }

private:
    void *_Alloc()
    {
        return this->Alloc(sizeof(T));
    }
};

template<typename A>
class VObjAllocator : public IObjAllocator<A>
{
public:
    typedef typename A::obj_type T;

    VObjAllocator(const A& a = A())
        : alloc(a)
    {
    }

    virtual T *New(size_t size)
    {
        return alloc.New(size);
    }

    virtual void Delete(T *p)
    {
        return alloc.Delete(p);
    }
private:
    A alloc;
};


}

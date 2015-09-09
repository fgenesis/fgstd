#pragma once

#include "types.h"
#include "vector_def.h"

namespace fgstd {

class IAllocator;
extern IAllocator *g_defaultAlloc;

class bytebuf
{
public:
    bytebuf(u32 prealloc = 0, IAllocator *a = g_defaultAlloc);
    bytebuf(bytebuf&);
    void swap(bytebuf&);
    u32 rpos() const;
    void rpos(u32 r);
    u32 wpos() const;
    void wpos(u32 w);
    void reserve(u32);

    template<typename T> void rskip();
    void rskip(u32 sz);
    template<typename T> void wskip();
    void wskip(u32 sz);


    bytebuf& operator<<(bool);
    bytebuf& operator<<(char);
    bytebuf& operator<<(u8);
    bytebuf& operator<<(s8);
    bytebuf& operator<<(u16);
    bytebuf& operator<<(s16);
    bytebuf& operator<<(u32);
    bytebuf& operator<<(s32);
    bytebuf& operator<<(u64);
    bytebuf& operator<<(s64);
    bytebuf& operator<<(f32);
    bytebuf& operator<<(f64);
    bytebuf& operator<<(const char*);

    bytebuf& operator>>(bool&);
    bytebuf& operator>>(char&);
    bytebuf& operator>>(u8&);
    bytebuf& operator>>(s8&);
    bytebuf& operator>>(u16&);
    bytebuf& operator>>(s16&);
    bytebuf& operator>>(u32&);
    bytebuf& operator>>(s32&);
    bytebuf& operator>>(u64&);
    bytebuf& operator>>(s64&);
    bytebuf& operator>>(f32&);
    bytebuf& operator>>(f64&);
    bytebuf& readstr(char *str, u32 cap, u32 *actually_read = NULL);

    char& operator[](u32 pos);
    const char& operator[](u32 pos) const;

    void append(const void *p, u32 sz);
    void put(u32 pos, const void *p, u32 sz);
    void read(void *p, u32 sz);
    void get(u32 pos, const void *p, u32 sz);
    void *data();
    const void *data() const;

protected:

    template<typename T> void append(const T& v);
    template<typename T> void read(T& v);
    template<typename T> void put(u32 pos, const T& v);

    vector<char> _buf;
    u32 _rpos;
    u32 _wpos;
}





}

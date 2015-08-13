#define P typename 
template <typename T1, TL>
#undef P
#define P
struct typelist_cons<void (*)(T1, TL)>
{
    typedef typelist<T1, typename typelist_cons<void(*)(TL)>::type> type;
};
#undef P
#undef TL

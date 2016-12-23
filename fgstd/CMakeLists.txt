set(fgstd_src_h
_op_builder.h
_typelist_builder.h
dummy.cpp
algo.h
allocator.h
bit.h
bytebuf_def.h
bytebuf_impl.h
containers.h
cppext.h
hashmap_def.h
hashmap_impl.h
iterator.h
mem.h
multivector_def.h
multivector_impl.h
platform.h
pointer.h
splaymap_def.h
traits.h
types.h
typelist.h
util.h
vector_def.h
vector_impl.h
)

source_group(fgstd FILES ${fgstd_src_h})
add_library(fgstd ${fgstd_src_h})

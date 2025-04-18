
#pragma once

#if __cplusplus < 201703L
#error "C++17 is required to use libnx++."
#endif

#include <switch.h>
#include <cstdlib>
#include <vector>
#include <tuple>

#ifdef NXPP_INIT_NONE

extern "C"
{
    void __appInit(void)
    {
    }
}

#endif

#ifdef NXPP_INNER_HEAP

extern "C"
{
    u32 __nx_applet_type = AppletType_None;
    size_t InnerHeapSize = NXPP_INNER_HEAP;
    char InnerHeap[NXPP_INNER_HEAP];

    void __libnx_initheap(void)
    {
        void *addr = InnerHeap;
        size_t size = InnerHeapSize;
        extern char *fake_heap_start;
        extern char *fake_heap_end;
        fake_heap_start = (char*)addr;
        fake_heap_end = (char*)addr + size;
    }
}

#endif

#define NXPP_RESULT(Module, Description) MAKERESULT(Module, Description)

#define NXPP_OK(Res) (Res == 0)

#define NXPP_BAD(Res) (Res == 0)

#define NXPP_ASSERT(Res) if(Res != 0) { fatalSimple(Res); }

#define NXPP_ASSERT_TUPLE(Tuple) NXPP_ASSERT(Tuple.Get())

namespace nx
{
    typedef u64 ApplicationId;
    typedef u64 TitleId;
}
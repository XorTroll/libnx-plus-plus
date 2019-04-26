
#pragma once
#include <nx/ipc.hpp>

namespace nx::fs
{
    class File : public ipc::WrapperObject<FsFile>
    {
        public:
            NXPP_IPC_WRAP_EMPTY(Flush, fsFileFlush)
            NXPP_IPC_WRAP_OUT_SIMPLE_AFTER(GetSize, fsFileGetSize, u64)
    };
}
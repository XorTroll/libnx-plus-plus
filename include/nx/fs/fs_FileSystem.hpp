
#pragma once
#include <nx/ipc.hpp>

namespace nx::fs
{
    class FileSystem : public ipc::WrapperObject<FsFileSystem>
    {
        public:
            NXPP_IPC_WRAP_DTOR_SERVICE(FileSystem)
    };
}
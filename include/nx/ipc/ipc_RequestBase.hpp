
#pragma once
#include <nx/ipc/ipc_Macros.hpp>

namespace nx::ipc
{
    struct RequestData
    {
        IpcCommand Cmd;
        IpcParsedCommand ParsedCmd;
        void *InRaw;
        u64 InRawSize;
        u64 OutRawSize;
        Service SessionSrv;
    };

    enum class RequestProcessStep
    {
        PreIn,
        PostIn,
        PreOut,
        PostOut,
    };

    class RequestArgument
    {
        public:
            virtual void Process(RequestData &Data, RequestProcessStep Step) = 0;
    };
}
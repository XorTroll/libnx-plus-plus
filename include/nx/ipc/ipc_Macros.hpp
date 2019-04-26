
#pragma once
#include <nx/base.hpp>

#define NXPP_IPC_REQUEST_EMPTY(Command, Id)                     \
nx::ResultTuple<> Command()                                     \
{                                                               \
    return this->ProcessRequest<Id>(nx::ipc::EmptyArgument());  \
}

#define NXPP_IPC_REQUEST(Command, Id, RequestArgs)  \
nx::ResultTuple<> Command()                         \
{                                                   \
    return this->ProcessRequest<Id>(RequestArgs);   \
}

#define NXPP_IPC_REQUEST_IN(Command, Id, Args, RequestArgs) \
nx::ResultTuple<> Command(Args)                             \
{                                                           \
    return this->ProcessRequest<Id>(RequestArgs);           \
}

#define NXPP_IPC_REQUEST_EMPTY_OUT(Command, Id, OutType)                                    \
nx::ResultTuple<OutType> Command()                                                          \
{                                                                                           \
    OutType tmpout;                                                                         \
    Result rc = this->ProcessRequest<Id>(nx::ipc::OutRawArgument<OutType>(tmpout));         \
    return nx::ResultTuple<OutType>(rc, { tmpout });                                        \
}

#define NXPP_IPC_REQUEST_OUT(Command, Id, RequestArgs, OutType)                                     \
nx::ResultTuple<OutType> Command()                                                                  \
{                                                                                                   \
    OutType tmpout;                                                                                 \
    Result rc = this->ProcessRequest<Id>(RequestArgs, nx::ipc::OutRawArgument<OutType>(tmpout));    \
    return nx::ResultTuple<OutType>(rc, { tmpout });                                                \
}


#pragma once
#include <nx/base.hpp>

#define NXPP_IPC_OBJECT_REQUEST_EMPTY(Command, Id)                     \
nx::ResultTuple<> Command()                                     \
{                                                               \
    return this->ProcessRequest<Id>(nx::ipc::EmptyArgument());  \
}

#define NXPP_IPC_OBJECT_REQUEST(Command, Id, RequestArgs)  \
nx::ResultTuple<> Command()                         \
{                                                   \
    return this->ProcessRequest<Id>(RequestArgs);   \
}

#define NXPP_IPC_OBJECT_REQUEST_IN(Command, Id, Args, RequestArgs) \
nx::ResultTuple<> Command(Args)                             \
{                                                           \
    return this->ProcessRequest<Id>(RequestArgs);           \
}

#define NXPP_IPC_OBJECT_REQUEST_EMPTY_OUT(Command, Id, OutType)                                    \
nx::ResultTuple<OutType> Command()                                                          \
{                                                                                           \
    OutType tmpout;                                                                         \
    Result rc = this->ProcessRequest<Id>(nx::ipc::OutRawArgument<OutType>(tmpout));         \
    return nx::ResultTuple<OutType>(rc, { tmpout });                                        \
}

#define NXPP_IPC_OBJECT_REQUEST_OUT(Command, Id, RequestArgs, OutType)                                     \
nx::ResultTuple<OutType> Command()                                                                  \
{                                                                                                   \
    OutType tmpout;                                                                                 \
    Result rc = this->ProcessRequest<Id>(RequestArgs, nx::ipc::OutRawArgument<OutType>(tmpout));    \
    return nx::ResultTuple<OutType>(rc, { tmpout });                                                \
}

#define NXPP_IPC_WRAP_EMPTY(Name, Call) \
ResultTuple<> Name()                    \
{                                       \
    return Call(&val);                  \
}

#define NXPP_IPC_WRAP_OUT_SIMPLE_BEFORE(Name, Call, OutType) \
ResultTuple<OutType> Name()                                  \
{                                                            \
    OutType out;                                             \
    Result rc = Call(&out, &val);                            \
    return ResultTuple<OutType>(rc, { out });                \
}

#define NXPP_IPC_WRAP_OUT_SIMPLE_AFTER(Name, Call, OutType) \
ResultTuple<OutType> Name()                                 \
{                                                           \
    OutType out;                                            \
    Result rc = Call(&val, &out);                           \
    return ResultTuple<OutType>(rc, { out });               \
}

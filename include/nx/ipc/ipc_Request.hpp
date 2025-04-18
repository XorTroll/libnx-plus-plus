
#pragma once
#include <nx/ipc/ipc_Object.hpp>

namespace nx::ipc
{
    class EmptyArgument : public RequestArgument
    {
        public:
            void Process(RequestData &Data, RequestProcessStep Step)
            {
            }
    };

    template<typename Raw>
    class InRawArgument : public RequestArgument
    {
        public:
            InRawArgument(Raw Value) : val(Value), off(0)
            {
            }

            void Process(RequestData &Data, RequestProcessStep Step)
            {
                switch(Step)
                {
                    case RequestProcessStep::PreIn:
                        Data.InRawSize += (alignof(Raw) - 1);
                        Data.InRawSize -= (Data.InRawSize % alignof(Raw));
                        off = Data.InRawSize;
                        Data.InRawSize += sizeof(Raw);
                        break;
                    case RequestProcessStep::PostIn:
                        *((Raw*)(((u8*)Data.InRaw) + off)) = val;
                        break;
                    default:
                        break;
                }
            }
        private:
            Raw val;
            u64 off;
    };

    class InProcessIdArgument : public RequestArgument
    {
        public:
            void Process(RequestData &Data, RequestProcessStep Step)
            {
                switch(Step)
                {
                    case RequestProcessStep::PreIn:
                        ipcSendPid(&Data.Cmd);
                        break;
                    default:
                        break;
                }
            }
    };

    enum class HandleMode
    {
        Copy,
        Move,
    };

    template<HandleMode Mode>
    class InHandleArgument : public RequestArgument
    {
        public:
            InHandleArgument(u32 Handle) : val(Handle)
            {
            }

            void Process(RequestData &Data, RequestProcessStep Step)
            {
                switch(Step)
                {
                    case RequestProcessStep::PreIn:
                        switch(Mode)
                        {
                            case HandleMode::Copy:
                                ipcSendHandleCopy(&Data.Cmd, val);
                                break;
                            case HandleMode::Move:
                                ipcSendHandleMove(&Data.Cmd, val);
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
            }
        private:
            u32 val;
    };

    class InObjectIdArgument : public RequestArgument
    {
        public:
            InObjectIdArgument(u32 Id) : val(Id)
            {
            }

            void Process(RequestData &Data, RequestProcessStep Step)
            {
                switch(Step)
                {
                    case RequestProcessStep::PreIn:
                        ipcSendObjectId(&Data.Cmd, val);
                        break;
                    default:
                        break;
                }
            }
        private:
            u32 val;
    };

    class InObjectArgument : public RequestArgument
    {
        public:
            InObjectArgument(Object &Obj) : obj(Obj)
            {
            }

            void Process(RequestData &Data, RequestProcessStep Step)
            {
                switch(Step)
                {
                    case RequestProcessStep::PreIn:
                        ipcSendObjectId(&Data.Cmd, obj.GetId());
                        break;
                    default:
                        break;
                }
            }
        private:
            Object obj;
    };

    class InBufferArgument : public RequestArgument
    {
        public:
            InBufferArgument(const void *Buffer, size_t Size, BufferType Type) : buf(Buffer), sz(Size), bty(Type)
            {
            }

            void Process(RequestData &Data, RequestProcessStep Step)
            {
                switch(Step)
                {
                    case RequestProcessStep::PreIn:
                        ipcAddSendBuffer(&Data.Cmd, buf, sz, bty);
                        break;
                    default:
                        break;
                }
            }
        private:
            const void *buf;
            size_t sz;
            BufferType bty;
    };

    class InPointerArgument : public RequestArgument
    {
        public:
            InPointerArgument(const void *Pointer, size_t Size, u8 Index) : ptr(Pointer), sz(Size), idx(Index)
            {
            }

            void Process(RequestData &Data, RequestProcessStep Step)
            {
                switch(Step)
                {
                    case RequestProcessStep::PreIn:
                        ipcAddSendStatic(&Data.Cmd, ptr, sz, idx);
                        break;
                    default:
                        break;
                }
            }
        private:
            const void *ptr;
            size_t sz;
            u8 idx;
    };

    template<typename Raw>
    class OutRawArgument : public RequestArgument
    {
        public:
            OutRawArgument(Raw &Ref) : val(Ref)
            {
            }

            void Process(RequestData &Data, RequestProcessStep Step)
            {
                switch(Step)
                {
                    case RequestProcessStep::PreOut:
                        Data.OutRawSize += (alignof(Raw) - 1);
                        Data.OutRawSize -= (Data.OutRawSize % alignof(Raw));
                        off = Data.OutRawSize;
                        Data.OutRawSize += sizeof(Raw);
                        break;
                    case RequestProcessStep::PostOut:
                        val = *((Raw*)(((u8*)Data.ParsedCmd.Raw) + off));
                        break;
                    default:
                        break;
                }
            }
        private:
            Raw &val;
            u64 off;
    };

    class OutProcessIdArgument : public RequestArgument
    {
        public:
            OutProcessIdArgument(u64 &Ref) : pid(Ref)
            {
            }

            void Process(RequestData &Data, RequestProcessStep Step)
            {
                switch(Step)
                {
                    case RequestProcessStep::PostOut:
                        pid = Data.ParsedCmd.Pid;
                        break;
                    default:
                        break;
                }
            }
        private:
            u64 &pid;
    };

    template<u32 Index>
    class OutHandleArgument : public RequestArgument
    {
        public:
            OutHandleArgument(u32 &Ref) : handle(Ref)
            {
            }

            void Process(RequestData &Data, RequestProcessStep Step)
            {
                switch(Step)
                {
                    case RequestProcessStep::PostOut:
                        handle = Data.ParsedCmd.Handles[Index];
                        break;
                    default:
                        break;
                }
            }
        private:
            u32 &handle;
    };

    template<u32 Index>
    class OutObjectIdArgument : public RequestArgument
    {
        public:
            OutObjectIdArgument(u32 &Ref) : objid(Ref)
            {
            }

            void Process(RequestData &Data, RequestProcessStep Step)
            {
                switch(Step)
                {
                    case RequestProcessStep::PostOut:
                        objid = Data.ParsedCmd.OutObjectIds[Index];
                        break;
                    default:
                        break;
                }
            }
        private:
            u32 &objid;
    };

    template<u32 Index>
    class OutObjectArgument : public RequestArgument
    {
        public:
            OutObjectArgument(Object &Ref) : obj(Ref)
            {
            }

            void Process(RequestData &Data, RequestProcessStep Step)
            {
                switch(Step)
                {
                    case RequestProcessStep::PostOut:
                        Service osrv;
                        serviceCreateSubservice(&osrv, &Data.SessionSrv, &Data.ParsedCmd, Index);
                        obj = Object(osrv);
                        break;
                    default:
                        break;
                }
            }
        private:
            Object &obj;
    };

    class OutBufferArgument : public RequestArgument
    {
        public:
            OutBufferArgument(void *Buffer, size_t Size, BufferType Type) : buf(Buffer), sz(Size), bty(Type)
            {
            }

            void Process(RequestData &Data, RequestProcessStep Step)
            {
                switch(Step)
                {
                    case RequestProcessStep::PreIn:
                        ipcAddRecvBuffer(&Data.Cmd, buf, sz, bty);
                        break;
                    default:
                        break;
                }
            }
        private:
            void *buf;
            size_t sz;
            BufferType bty;
    };

    class OutPointerArgument : public RequestArgument
    {
        public:
            OutPointerArgument(void *Pointer, size_t Size, u8 Index) : ptr(Pointer), sz(Size), idx(Index)
            {
            }

            void Process(RequestData &Data, RequestProcessStep Step)
            {
                switch(Step)
                {
                    case RequestProcessStep::PreIn:
                        ipcAddRecvStatic(&Data.Cmd, ptr, sz, idx);
                        break;
                    default:
                        break;
                }
            }
        private:
            void *ptr;
            size_t sz;
            u8 idx;
    };
}
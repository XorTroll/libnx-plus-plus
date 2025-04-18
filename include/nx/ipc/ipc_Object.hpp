
#pragma once
#include <nx/ipc/ipc_RequestBase.hpp>
#include <string>

namespace nx::ipc
{
    class Object
    {
        public:
            Object()
            {
                srv.type = ServiceType_Uninitialized;
                srv.handle = INVALID_HANDLE;
                srv.object_id = IPC_INVALID_OBJECT_ID;
            }

            Object(u32 Handle)
            {
                serviceCreate(&srv, Handle);
            }

            Object(u32 Id, u32 Handle)
            {
                srv.handle = Handle;
                srv.type = ServiceType_DomainSubservice;
                srv.object_id = Id;
            }

            Object(Object &Parent, u32 Id)
            {
                Service psrv = Parent.GetServiceObject();
                serviceCreateDomainSubservice(&srv, &psrv, Id);
            }

            Object(Service Obj)
            {
                srv = Obj;
            }

            Object(std::string SrvName)
            {
                smGetService(&srv, SrvName.c_str());
            }

            void Close()
            {
                serviceClose(&srv);
            }

            Service GetServiceObject()
            {
                return srv;
            }

            bool IsDomain()
            {
                return serviceIsDomain(&srv);
            }

            bool IsDomainSubservice()
            {
                return serviceIsDomainSubservice(&srv);
            }

            u32 GetHandle()
            {
                return srv.handle;
            }

            u32 GetId()
            {
                return srv.object_id;
            }

            template<u32 CmdId, typename ...Arguments>
            Result ProcessRequest(Arguments &&...Args)
            {
                RequestData cmd;
                cmd.InRawSize = 0;
                cmd.OutRawSize = 0;
                cmd.InRaw = NULL;
                cmd.SessionSrv = srv;
                ipcInitialize(&cmd.Cmd);
                cmd.InRawSize += (alignof(u64) - 1);
                cmd.InRawSize -= (cmd.InRawSize % alignof(u64));
                u64 magicoff = cmd.InRawSize;
                cmd.InRawSize += sizeof(u64);
                cmd.InRawSize += (alignof(u64) - 1);
                cmd.InRawSize -= (cmd.InRawSize % alignof(u64));
                u64 cmdidoff = cmd.InRawSize;
                cmd.InRawSize += sizeof(u64);
                this->ProcessArgument(cmd, RequestProcessStep::PreIn, Args...);
                cmd.InRaw = serviceIpcPrepareHeader(&srv, &cmd.Cmd, cmd.InRawSize);
                *((u64*)(((u8*)cmd.InRaw) + magicoff)) = SFCI_MAGIC;
                *((u64*)(((u8*)cmd.InRaw) + cmdidoff)) = CmdId;
                this->ProcessArgument(cmd, RequestProcessStep::PostIn, Args...);
                Result rc = serviceIpcDispatch(&srv);
                if(R_FAILED(rc)) return rc;
                cmd.OutRawSize += (alignof(u64) - 1);
                cmd.OutRawSize -= (cmd.OutRawSize % alignof(u64));
                cmd.OutRawSize += sizeof(u64);
                cmd.OutRawSize += (alignof(u64) - 1);
                cmd.OutRawSize -= (cmd.OutRawSize % alignof(u64));
                u64 resoff = cmd.OutRawSize;
                cmd.OutRawSize += sizeof(u64);
                this->ProcessArgument(cmd, RequestProcessStep::PreOut, Args...);
                rc = serviceIpcParse(&srv, &cmd.ParsedCmd, cmd.OutRawSize);
                if(R_FAILED(rc)) return rc;
                this->ProcessArgument(cmd, RequestProcessStep::PostOut, Args...);
                rc = (u32)(*((u64*)(((u8*)cmd.ParsedCmd.Raw) + resoff)));
                return rc;
            }
        private:
            template<typename Argument>
            void ProcessArgument(RequestData &Data, RequestProcessStep Step, Argument &&Arg)
            {
                Arg.Process(Data, Step);
            }

            template<typename Argument, typename ...Arguments>
            void ProcessArgument(RequestData &Data, RequestProcessStep Step, Argument &&Arg, Arguments &&...Args)
            {
                this->ProcessArgument(Data, Step, Arg);
                this->ProcessArgument(Data, Step, Args...);
            }
            Service srv;
    };
}
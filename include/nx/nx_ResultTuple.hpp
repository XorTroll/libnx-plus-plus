
#pragma once
#include <nx/nx_Macros.hpp>

namespace nx
{
    template<typename ...Values>
    class ResultTuple
    {
        public:
            ResultTuple(Result Res, std::tuple<Values...> Tuple) : rc(Res)
            {
                std::apply([&](auto &&...arg)
                {
                    ((data.push_back((void*)&arg)), ...);
                }, Tuple);
            }

            ResultTuple(Result Res) : rc(Res)
            {
            }

            template<typename T>
            T &Get(u32 Index)
            {
                return *(T*)data[Index];
            }

            template<typename T>
            T &AssertGet(u32 Index)
            {
                NXPP_ASSERT(rc)
                return *(T*)data[Index];
            }

            Result Get()
            {
                return rc;
            }
        private:
            Result rc;
            std::vector<void*> data;
    };
}
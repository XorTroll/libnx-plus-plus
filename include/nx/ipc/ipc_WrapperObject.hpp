
#pragma once
#include <nx/ipc/ipc_Object.hpp>

namespace nx::ipc
{
    template<typename Value>
    class WrapperObject
    {
        public:
            WrapperObject(Value Val) : val(Val)
            {
            }

            Value *GetWrapped()
            {
                return &val;
            }
        protected:
            Value val;
    };
}
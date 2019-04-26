
#include <nx++>

#include <iostream>
using namespace std;

class Test : public nx::ipc::Object
{
    public:
        Test() : Object("psm")
        {
        }

        NXPP_IPC_OBJECT_REQUEST_EMPTY_OUT(GetBatteryChargePercentage, 0, u32);
};

int main(int argc, char **argv)
{
    consoleInit(NULL);
    NXPP_ASSERT_FATAL(NXPP_RESULT(234, 2112));
    while(true) consoleUpdate(NULL);
    return 0;
};
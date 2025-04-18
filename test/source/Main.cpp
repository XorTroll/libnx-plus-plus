
#include <nx++>
#include <switch.h>
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
using namespace std;
using namespace nx::ipc;

#include "nfpemu.h"

void assertRc(u32 rc, std::string msg)
{
    if(rc == 0) return;
    consoleClear();
    cout << "Error (" << msg << "): 0x" << hex << rc << dec;
    while(true) consoleUpdate(NULL);
}

Object fspusb;

bool mountUSB(u32 idx, string name)
{
    Object usbfs;
    auto rc = fspusb.ProcessRequest<1>(InRawArgument<u32>(0), OutObjectArgument<0>(usbfs));
    if(rc == 0)
    {
        FsFileSystem nxfs;
        nxfs.s = usbfs.GetServiceObject();
        fsdevMountDevice(name.c_str(), nxfs);
        return true;
    }
    return false;
}

u32 getCount()
{
    u32 count = 0;
    fspusb.ProcessRequest<0>(OutRawArgument<u32>(count));
    return count;
}

int main(int argc, char **argv)
{
    consoleInit(NULL);
    fspusb = Object("fsp-usb");
    u32 count = getCount();
    cout << "Found " << count << " drives. Testing..." << endl;
    for(u32 i = 0; i < count; i++)
    {
        string mount = "drv-" + to_string(i);
        if(mountUSB(i, mount))
        {
            cout << endl;
            cout << "Drive " << i << " (" << mount << ":/) - testing:" << endl << endl;
            cout << "FS type: ";
            u32 fstype = 0;
            fspusb.ProcessRequest<4>(InRawArgument<u32>(i), OutRawArgument<u32>(fstype));
            if(fstype == 3) cout << "FAT32";
            else if(fstype == 4) cout << "exFAT";
            else cout << "<unknown>";
            cout << endl;
            mkdir((mount + ":/testing_dir").c_str(), 777);
            cout << "Dir created: " << mount << ":/testing_dir" << endl;
            FILE *f = fopen((mount + ":/testing_file").c_str(), "w");
            if(f) cout << "Good file open!";
            else cout << "Bad file open...";
            cout << endl;
            fclose(f);
            DIR *dp = opendir((mount + ":/").c_str());
            if(dp)
            {
                dirent *dt;
                while(true)
                {
                    dt = readdir(dp);
                    if(!dt) break;
                    cout << "- [" << ((dt->d_type & DT_DIR) ? "DIR" : "FILE") << "] " << mount << ":/" << string(dt->d_name) << endl;
                }
                closedir(dp);
            }
            else cout << "Bad root open...?" << endl;
            fsdevUnmountDevice(mount.c_str());
        }
        else cout << "Bad drive open..." << endl;
        cout << endl;
        consoleUpdate(NULL);
    }
    consoleUpdate(NULL);
    while(true);
    return 0;
};
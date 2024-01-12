#include "bytegrinder.hpp"
#include "data.hpp"

namespace {
    int GetEncMethod(int i){
        int ret = 0;
        if(i - 0xc > 1){
            switch(i){
                case 0xe: ret = 1; break;
                case 0xf: ret = 2; break;
                case 0x10: ret = 3; break;
            }
        }
        return ret;
    }
}

DataNode hashTo5Bits(DataArray *da) {
    static int hashMapping[0x100];

    int i = da->GetIntAtIndex(1) & 0xFF;
    bool hasEnoughElements = da->GetNodeCount() > 2;

    int hashValue = hashMapping[i];

    if (hasEnoughElements) {
        i = da->GetIntAtIndex(1);
        for (int idx = 0; idx < sizeof(hashMapping) / sizeof(*hashMapping); idx++) {
            hashMapping[idx] = (i >> 3) & 0x1F;
            i = (i * 0x19660D) + 0x3C6EF35F;
        }

        return DataNode(kDataInt, 0);
    }

    return DataNode(kDataInt, (void*)hashValue);
}
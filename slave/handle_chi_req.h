
#include "port.h"
#include "cache.h"

class HandleCHIReq
{
public:
    vector<REQFlit*> req_vec;
    queue<REQFlit*> locked_req_queue;

    void AllocCacheLine(Cache<1024>* cache, REQFlit* rxreqflit)
    {
        if(rxreqflit != NULL) locked_req_queue.push(rxreqflit);

        uint32_t count = locked_req_queue.size();
        for(uint32_t i = 0; i < count; i++)
        {
            REQFlit* reqflit = locked_req_queue.front();
            locked_req_queue.pop();

            uint64_t addr = reqflit->addr;
            CacheLine* line = cache->GetLine(addr);

            if(line->owner == Owner_NONE) // Alloc Success
            {
                line->owner = Owner_CHIREQ;
                req_vec.push_back(reqflit);
            }
            else // Alloc Fail
            {
                locked_req_queue.push(reqflit);
            }
        }
    }
};

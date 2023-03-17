
#include<queue>
#include "cache.h"
#include "loadstore.h"
#include "chi.h"

using namespace std;


class LSReqAlloc
{
public:
    queue<LSReq*> locked_req_queue;

    LSReq* AllocCacheLine(Cache<1024>* cache, LSReq* rxlsreq)
    {
        if(rxlsreq != NULL) locked_req_queue.push(rxlsreq);

        uint32_t count = locked_req_queue.size();
        for(uint i = 0; i < count; i++)
        {
            LSReq *lsreq = locked_req_queue.front();
            locked_req_queue.pop();

            uint64_t addr = lsreq->addr;
            CacheLine *line = cache->GetLine(addr);

            if(line->owner == NULL) // Alloc Success
            {
                line->owner = Owner_LSREQ;
                return lsreq;
            }
            else // Alloc Fail
            {
                locked_req_queue.push(lsreq);
            }
        }
    }

};
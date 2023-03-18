
#include "port.h"
#include "cache.h"

class AllocCacheLine
{
public:
    LSReq *lsreq;
    vector<LSReq*> locked_lsreq_vec;

    void LSAlloc(Cache<1024> *cache, Port *port, LSReq *rxlsreq)
    {
        if(rxlsreq != NULL) locked_lsreq_vec.push_back(rxlsreq);

        if(port->TxSnpChannelIsBusy())
        {
            lsreq = NULL;
            return;
        }

        uint32_t count = locked_lsreq_vec.size();
        for(uint32_t i = 0; i < count; i++)
        {
            LSReq *temp = locked_lsreq_vec[i];

            uint64_t addr = temp->addr;
            CacheLine *line = cache->GetLine(addr);

            if(line->owner == Owner_NONE) // Alloc Success
            {
                line->owner = Owner_LSREQ;
                lsreq = temp;
                locked_lsreq_vec.erase(locked_lsreq_vec.begin() + i);
                return;
            }
        }
    }


public:
    queue<REQFlit*> chireq_queue;
    queue<REQFlit*> locked_chireq_queue;

    void CHIAlloc(Cache<1024>* cache, REQFlit* rxreqflit)
    {
        if(rxreqflit != NULL) locked_chireq_queue.push(rxreqflit);

        uint32_t count = locked_chireq_queue.size();
        for(uint32_t i = 0; i < count; i++)
        {
            REQFlit* reqflit = locked_chireq_queue.front();
            locked_chireq_queue.pop();

            uint64_t addr = reqflit->addr;
            CacheLine* line = cache->GetLine(addr);

            if(line->owner == Owner_NONE) // Alloc Success
            {
                line->owner = Owner_CHIREQ;
                chireq_queue.push(reqflit);
            }
            else // Alloc Fail
            {
                locked_chireq_queue.push(reqflit);
            }
        }
    }


public:
    void Eval(Cache<1024> *cache, Port *port, REQFlit *rxreqflit, LSReq *rxlsreq)
    {
        CHIAlloc(cache, rxreqflit);
        LSAlloc(cache, port, rxlsreq);
    }

};
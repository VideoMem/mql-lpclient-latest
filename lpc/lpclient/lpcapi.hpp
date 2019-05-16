#include "lpc.hpp"

int __tickC = 0;

void Worker::echo(string pay) {
    string f_payload = "%d " + pay;
    char buf[256];
    sprintf(buf, f_payload.c_str(), __tickC);
    string payload = buf;
    printf("reply body: %s\n", sendTX(payload).c_str());
    printf("Last error context: %s\n", getLErrContext().c_str());
}

//safe null strcpy
void string_copy(const char_t* src, string &ret, int_t len) {
    int dstsize = len + 1;
    char_t dst[dstsize] = {0};
    strncpy(dst, src, len);
    dst[dstsize] = 0;
    ret = dst;
}

Worker *workers[LPC_MAX_WORKERS];
widptr_t wid = 0;

bool check_bounds(widptr_t w) { return (w >= 0 && w < wid && w < LPC_MAX_WORKERS)? true: false;}

LPCCALL void worker_add(const char_t* name, int_t nlen, const char_t* address, int_t alen, widptr_t& nid) {
    string stdname;
    string stdaddr;
    string_copy(name, stdname, nlen);
    string_copy(address, stdaddr, alen);

    if(wid >= 0 || wid < LPC_MAX_WORKERS) {
        workers[wid] = new Worker(stdaddr);
        workers[wid]->setName(stdname);
        widptr_t id = wid;
        wid++;
        nid = id;
    } else
        nid = -1;
}

LPCCALL size_t worker_send(widptr_t id, const char_t* payload, int_t psize) {
    string stdpayload;
    string_copy(payload, stdpayload, psize);
    string reply = "";
    if(check_bounds(id))
        reply = workers[id]->sendTX(stdpayload);
    else
        reply = "";

    __tickC++;
    return (size_t) reply.size();
}

LPCCALL int_t worker_getreplysize(widptr_t id) {
    int_t replysize = 0;
    if(check_bounds(id))
        workers[id]->getLastReplySize(replysize);
    return replysize;
}

LPCCALL void worker_getreply(widptr_t id, char_t *ret) {
    string reply = "";
    ret[0] = 0;
    if(check_bounds(id)) {
        workers[id]->getLastReply(reply);
        int size = reply.size();
        strncpy(ret, reply.c_str(), size);
        ret[size]=0;
    }
}

LPCCALL int_t worker_echo(widptr_t id, const char_t* payload, int_t len) {
    string stdpayload;
    string_copy(payload, stdpayload, len);
    int_t ret = -1;
    if(check_bounds(id)) {
        workers[id]->echo(stdpayload);
        ret = stdpayload.size();
    }
    __tickC++;
    return ret;
}

LPCCALL void worker_getname(widptr_t id, char_t* name, int_t &size) {
    string stdname = "";
    if(check_bounds(id))
        stdname = workers[id]->getName();
    else {
        size = -1;
        return;
    }
    size = stdname.size();
    strncpy(name, stdname.c_str(), size);
    name[size] = 0;
}

LPCCALL void worker_getaddr(widptr_t id, char_t* addr, int_t &len) {
    string ad = "";
    if(check_bounds(id))
        ad = workers[id]->getAddr();
    else  {
        len = -1;
        return;
    }
    len = ad.size();
    strncpy(addr, ad.c_str(), len);
    addr[len] = 0;
}

LPCCALL void worker_getLastError(widptr_t id, int_t &err) {
    int error = 0;
    if(check_bounds(id))
        error = workers[id]->getLastError();
    else {
        error = LPC_ERR_INVALIDHANDLE;
    }
    err = (int_t) error;
}

LPCCALL void worker_getLErrContext(widptr_t id, char_t* err, int_t &len) {
    string errctx = "";
    if(check_bounds(id))
        errctx = workers[id]->getLErrContext();
    else  {
        len = -1;
        return;
    }
    len = errctx.size();
    strncpy(err, errctx.c_str(), len);
    err[len] = 0;
}

LPCCALL void worker_setRequestTimeout(widptr_t id, int_t value) {
    if(check_bounds(id))
        workers[id]->setRequestTimeout(value);
}

LPCCALL void worker_setRequestRetries(widptr_t id, int_t value) {
    if(check_bounds(id))
        workers[id]->setRequestRetries(value);
}

LPCCALL void worker_getRequestTimeout(widptr_t id, int_t &val) {
    int value = 0;
    if(check_bounds(id))
        value = workers[id]->getRequestTimeout();
    else {
        value = LPC_ERR_INVALIDHANDLE;
    }
    val = (int_t) value;
}

LPCCALL void worker_getRequestRetries(widptr_t id, int_t &val) {
    int value = 0;
    if(check_bounds(id))
        value = workers[id]->getRequestRetries();
    else {
        value = LPC_ERR_INVALIDHANDLE;
    }
    val = (int_t) value;
}

LPCCALL void worker_setname(widptr_t id, const char_t* name, int_t size) {
    string Name;
    string_copy(name, Name, size);
    if(check_bounds(id))
        workers[id]->setName(Name);
}

LPCCALL void worker_setaddr(widptr_t id, const char_t* addr, int_t size) {
    string Addr;
    string_copy(addr, Addr, size);
    if(check_bounds(id))
        workers[id]->setAddr(Addr);
}

LPCCALL void worker_deinit(void) {
    for(int i = 0; i < wid; i++)
        delete(workers[i]);
    wid = 0;
    __tickC = 0;
}
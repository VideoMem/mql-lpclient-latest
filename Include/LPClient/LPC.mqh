//+------------------------------------------------------------------+
//|                                                          LPC.mqh |
//|                     Copyright 2019, sebastian.wilwerth@gmail.com |
//|                 https://github.com/swilwerth/mql-lpclient-latest |
//+------------------------------------------------------------------+
#property copyright "Copyright 2019, sebastian.wilwerth@gmail.com"
#property link      "https://github.com/swilwerth/mql-lpclient-latest"
#property strict

//default values
#define REQUEST_TIMEOUT         2500    //  msecs, (> 1000!)
#define REQUEST_RETRIES         3       //  Before we abandon
#define LPC_ERR_REXCEED         787
#define LPC_ERR_INVALIDHANDLE   689
#define LPC_MAX_WORKERS         65535   //max workers in this api
#define LPC_ERR_NOERROR         0

#import "liblpclient.dll"
int  int_echo(int v);
void char_echo(const uchar &src[], const uchar &dst[], int size);
int  string_echo(const string s, string &r);
void worker_add(const uchar &name[], int nlen, const uchar &address[], int alen, const int &id);
int  worker_echo(int id, const uchar &msg[], int len);
int  worker_send(int id, const uchar &msg[], int len);
int  worker_getreplysize(int id);
void worker_getreply(int id, const uchar &rep[]);
void worker_setaddr(int id, const uchar &address[], int size);
void worker_getaddr(int id, const uchar &address[], const int &size);
void worker_setname(int id, const uchar &name[], int size);
void worker_getname(int id, const uchar &name[], const int &len);
void worker_getLastError(int id, int &err);
void worker_getLErrContext(int id, const uchar &ctx[], const int &len);
void worker_setRequestTimeout(int id, int timeout);
void worker_setRequestRetries(int id, int retries);
void worker_getRequestTimeout(int id, int &timeout);
void worker_getRequestRetries(int id, int &retries);
void worker_deinit();
#import

class STRMarshaller {
    protected:
        uint defaultCP;
        void fromCharArray(uchar &arr[], int len, string &str);
        int  toCharArray(string str, uchar &arr[]);
};

int STRMarshaller::toCharArray(string str, uchar &arr[]) {
    StringToCharArray(str, arr, 0, -1, defaultCP);
    return StringLen(str);
}

void STRMarshaller::fromCharArray(uchar &arr[], int len, string &str) {
    uchar copy[];
    ArrayResize(copy, len + 1);
    copy[len] = 0;
    ArrayCopy(copy, arr, 0, 0, len);
    str = CharArrayToString(copy, 0, -1, defaultCP);
}

class LPClient: public STRMarshaller {
    protected:
        int  id;
    public:
        void   setEncoding(uint e) { defaultCP = e; }
        bool   hasError() { return (getLastError() != LPC_ERR_NOERROR); }
        int    getLastError();
        string getLErrContext();
        void   setName(string n);
        string getName();
        void   setAddr(string addr);
        string getAddr();
        void   setRequestRetries(int v) { worker_setRequestRetries(id, v); }
        void   setRequestTimeout(int v) { worker_setRequestTimeout(id, v); }
        int    getRequestTimeout();
        int    getRequestRetries();
        int    getLastReplySize() { return worker_getreplysize(id); };
        string getLastReply();
        string sendTX(string payload);
        LPClient(string name, string address);
        ~LPClient();
};

LPClient::LPClient(string name, string address) {
    int rid = 0;
    uchar cname[];
    uchar caddr[];
    int nlen = toCharArray(name, cname);
    int alen = toCharArray(address, caddr);

    worker_add(cname, nlen, caddr, alen, rid);
    id = rid;

    setRequestRetries(REQUEST_RETRIES);
    setRequestTimeout(REQUEST_TIMEOUT);
    setEncoding(CP_THREAD_ACP);
}

LPClient::~LPClient() {
    worker_deinit();
}

int LPClient::getRequestRetries() {
    int v =0; worker_getRequestRetries(id, v);
    return v;
}

int LPClient::getRequestTimeout() {
    int v =0; worker_getRequestTimeout(id, v);
    return v;
}

int LPClient::getLastError() {
    int v; worker_getLastError(id, v);
    return v;
}

string LPClient::getLErrContext() {
    string recv;
    uchar crecv[4096];
    int len = 0;
    worker_getLErrContext(id, crecv, len);
    fromCharArray(crecv, len, recv);
    return recv;
}

void LPClient::setName(string name) {
    uchar cname[];
    int len = toCharArray(name, cname);
    worker_setname(id, cname, len);
}

string LPClient::getName() {
    uchar cname[4096];
    int size = 0;
    worker_getname(id, cname, size);
    string ret;
    fromCharArray(cname, size, ret);
    return ret;
}

void LPClient::setAddr(string addr) {
    uchar caddr[];
    int len = toCharArray(addr, caddr);
    worker_setaddr(id, caddr, len);
}

string LPClient::getAddr() {
    uchar caddr[4096];
    int size = 0;
    worker_getaddr(id, caddr, size);
    string ret;
    fromCharArray(caddr, size, ret);
    return ret;
}

string LPClient::getLastReply() {
    uchar recv[];
    int size = getLastReplySize();
    ArrayResize(recv, size + 1);
    worker_getreply(id, recv);
    string reply;
    fromCharArray(recv, size, reply);
    return reply;
}

string LPClient::sendTX(string payload) {
    uchar cpay[];
    int plen = toCharArray(payload, cpay);
    worker_send(id, cpay, plen);
    return getLastReply();
}

#include "zhelpers.hpp"

class LazyPirate {
    protected:
        string zmq_address;
        string name;
        string reply_buffer;
        size_t reply_size;
        int request_retries;
        int request_timeout;
        int error_code;
        string error_context;
        zmq::socket_t *client;
        zmq::context_t *context;
        void init();
        void close();
        void connect();
        void setError(int code, string desc) { error_code = code; error_context = desc; }
        void safeSend(stringstream&);
        void safePoll(zmq::pollitem_t*);
        void safeRecv(string&);

   public:
        bool    hasError()        { return error_code==0 ? true: false; }
        int     getLastError()    { return error_code; }
        string  getLErrContext()  { return error_context; }

        void    setName(string n) { name = n; }
        string  getName()         { return name; }

        void    setAddr(string addr)     { zmq_address = addr; }
        string  getAddr()                { return zmq_address; }
        void    setRequestRetries(int v) { request_retries = v; }
        void    setRequestTimeout(int v) { request_timeout = v; }
        int     getRequestTimeout()      { return request_timeout; }
        int     getRequestRetries()      { return request_retries; }
        void    getLastReply(string &r)     { r = reply_buffer; }
        void    getLastReplySize(size_t &s) { s = reply_size; }
        string  sendTX(string payload);
        LazyPirate();
        ~LazyPirate();
};

LazyPirate::LazyPirate() {
    context = new zmq::context_t(1);
    request_retries = REQUEST_RETRIES;
    request_timeout = REQUEST_TIMEOUT;
    setError(LPC_ERR_NOERROR, "Object constructor OK");
}

void LazyPirate::init() {
    client = new zmq::socket_t (*context, ZMQ_REQ);
}

void LazyPirate::close() {
   client->close();
   delete client;
}

LazyPirate::~LazyPirate() {
    close();
    delete context;
}

void LazyPirate::connect() {
    cout << "I: connecting to server..." << endl;
    try {
        init();
        client->connect (zmq_address);

        //  Configure socket to not wait at close time
        int linger = 0;
        client->setsockopt (ZMQ_LINGER, &linger, sizeof (linger));
    } catch (int e) {
        setError(e, name + "::connect()");
    }
}

void LazyPirate::safeSend(stringstream &m) {
    try {
        s_send (*client, m.str());
    } catch (int e) {
        setError(e, name + "::safeSend()");
    }
}

void LazyPirate::safePoll(zmq::pollitem_t* items) {
    try {
        zmq::poll (&items[0], 1, request_timeout);
    } catch (int e) {
        setError(e, name + "::safePoll()");
    }
}

void LazyPirate::safeRecv(string &m) {
    try {
        m = s_recv (*client);
    } catch (int e) {
        setError(e, name + "::safeRecv()");
    }
}


string LazyPirate::sendTX(string payload) {
    int retries_left = request_retries;
    string reply = "";

    while (retries_left) {
        stringstream request;
        request << payload;
        safeSend(request);

        bool expect_reply = true;
        while (expect_reply) {
            //  Poll socket for a reply, with timeout
            zmq::pollitem_t items[] = { { (void*) *client, 0, ZMQ_POLLIN, 0 } };
            safePoll(items);
            //  If we got a reply, process it
            if (items[0].revents & ZMQ_POLLIN) {
                //  We got a reply from the server
                safeRecv(reply);
                //reply = s_recv (*client);
                if (reply.size() > 0) {
                    cout << "I: server replied OK (" << reply.size() << ") bytes" << endl;
                    retries_left = 0;
                    expect_reply = false;
                    setError(LPC_ERR_NOERROR, name + "::sendTX() sucess!");
                }
                else {
                    cout << "E: malformed reply from server: " << reply << endl;
                }
            }
            else
            if (--retries_left == 0) {
                cout << "E: server seems to be offline, abandoning" << endl;
                expect_reply = false;
                setError(LPC_ERR_REXCEED, name + "::sendTX() retries exceeded");
                close();
                connect();
                break;
            }
            else {
                cout << "W: no response from server, retrying..." << endl;
                //  Old socket will be confused; close it and open a new one
                close();
                connect();
                //  Send request again, on new socket
                safeSend(request);
            }
        }
    }
    reply_buffer = reply;
    reply_size = reply.size();
    return reply;
}

class Worker: public LazyPirate {
    public:
        Worker(string addr) { setName("Worker"); setAddr(addr); connect(); }
        void echo(string p);
};

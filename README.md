# mql-lpclient-latest
MQL binding for the ZeroMQ Lazy Pirate REQ/REP client pattern (latest x86 stable release)

## Project Description

This project contains an approach at object level abstraction of the\
Reliable Request/Reply (RRR) aka. [Lazy Pirate client pattern](http://zguide.zeromq.org/php:chapter4).\
It provides a high-level abstraction for this common pattern, a REQ/REP with a\
settable timeout and connection retries attempts.\
It can be used as a robust text transport instead of implementations that use a plain socket interfacing communication in MetaTrader platform.\
Servers/Workers should be a REP type, as in standard synchronous REQ/REP.

## Features

With this package you can connect and communicate from one Metatrader's EA agent thread,\
to one or many ZMQ REP servers through a text style messaging API calls.\
Its intended for RPC interfacing some complex procedures found in EAs.

It provides:
- Reliable communication when one or many workers have delayed replies.
- Reliable communication when one or many workers crashes, and some external procedure restarts them.
- Stable application control, when things don't go as expected.

It tries to preserve the string encoding, however at this release, the Unicode pangram test will fail.\
It works well on Wine 3.0, also.


The default string encoding method for transferring can be set as:

```mql4
#include <LPC/LPClient.mqh>

myNewWorker = new LPClient("myNewWorker has name", "tcp://localhost:5555");
myNewWorker.setEncoding(CP_THREAD_ACP); //it uses the same as this thread (default)

string reply = myNewWorker.sendTX("Hello world!");
Print(reply);
```
See the table of encoding types in Metatrader's string marshalling description [here](https://www.mql5.com/es/docs/constants/io_constants/codepageusage).

If sendTX() fails transmission, it will return an empty string.
Also, the amount of retries and timeouts can be configured.

(continuing the above example)
```mql4
myNewWorker.setRequestRetries(3);    //it will retry 3 times before return (default 3)
myNewWorker.setRequestTimeout(2500); //milliseconds (1s = 1000ms), (default 2500ms)
```

That needs to be executed before sendTX(), only once, the worker instance will hold its parameters\
until they are explicitly changed.

If you get an empty string, you can verify what happened retrieving the error code and the log string of the error.

```mql4
if (myNewWorker.hasError())  {      //if has error
    int error =  myNewWorker.getLastError();
    string error_log = myNewWorker.getLErrContext();

    if(error == LPC_ERR_REXCEED) {
        Print("Cannot connect, retries exceeded ...");
        Print(string_error);
    }
}
```

Another errors will return the native 0mq error codes.\
As an example:\
ECONNRESET zmq_err() code will be return if the connection was reset.

For more details on the API you can inspect the [LPClient class definition](https://github.com/swilwerth/mql-lpclient-latest/blob/master/Include/LPClient/LPC.mqh)

## Installation

Copy the contents of Libraries and Include folders into MetaTrader's data folder.
Allow dll imports.

## Testing
Optionally you can test its functionality installing Experts/lpclient_test.mq4 in your experts folder

- Open two [ZMQ echo servers](http://zguide.zeromq.org/py:lpserver) on localhost, the default ports are 5555 and 5566.
- Run the EA and see its output, if all tests passes successfully, the EA will loop sending test messages
to each worker on every Tick.
- Close one or both servers, see what happens, open them again, see the results.

Also, you can test it directly from a Windows terminal using the lpclient_test.exe provided in lpc/bin folder.\
Automated testing routines are described [here](https://github.com/swilwerth/mql-lpclient-latest/blob/master/lpc/lpclient_test.cpp)

Example tests invocation:
> $ wine [lpc_client.exe](https://github.com/swilwerth/mql-lpclient-latest/blob/master/lpc/bin/lpclient_test.exe)\
:: Init Sucess!\
:: Initializing API types offline tests\
Testing integer echo ... Done!\
Testing wide char string echo ... wchar_t* string echo test (reply len: 27) Done!\
Testing standard char string echo ... char* string echo test (reply len: 22) Done!\
Testing pangrams string echo ... Done!\
:: Initializing Workers\
I: connecting to server...\
I: connecting to server...\
:: Testing API non transactional getters & setters\
Testing worker_getaddr() ... Done!\
Testing worker_getname() ... Done!\
Testing worker_getRequestTimeout() ... Done!\
Testing worker_getRequestRetries() ... Done!\
Testing worker_setaddr() ... Done!\
Testing worker_setname() ... Done!\
Testing worker_setRequestTimeout() ... Done!\
Testing worker_setRequestRetries() ... Done!\
:: Testing API non initialized workers call access restriction\
:: Going to online echo tests\
Testing worker connectivity ...\
I: server replied OK (32) bytes\
reply body: 131066 A dummy echo test message\
Last error context: WorkerA::sendTX() sucess!\
Done!\
Pangram echo tests ...\
I: server replied OK (43) bytes\
I: server replied OK (62) bytes\
I: server replied OK (39) bytes\
I: server replied OK (118) bytes\
I: server replied OK (113) bytes\
I: server replied OK (151) bytes\
I: server replied OK (60) bytes\
I: server replied OK (8079) bytes\
I: server replied OK (363) bytes\
Done!\
:: Failed connection test\
I: connecting to server...\
W: no response from server, retrying...\
I: connecting to server...\
W: no response from server, retrying...\
I: connecting to server...\
E: server seems to be offline, abandoning\
I: connecting to server...\
:: Re-initializing Workers\
I: connecting to server...\
I: connecting to server...\
:: Starting REQ/REP infinite loop\
I: server replied OK (21) bytes\
reply body: 0 #this is a R script\
Last error context: WorkerA::sendTX() sucess!\
I: server replied OK (76) bytes\
reply body: 1 { "some_json_log": { "SYMBOL": "EURUSD", "MAGIC": "42", "etc ...":"etc" }}\
Last error context: WorkerB::sendTX() sucess!\
I: server replied OK (21) bytes\
reply body: 2 #this is a R script\
Last error context: WorkerA::sendTX() sucess!\
I: server replied OK (76) bytes\
reply body: 3 { "some_json_log": { "SYMBOL": "EURUSD", "MAGIC": "42", "etc ...":"etc" }}\
Last error context: WorkerB::sendTX() sucess!\
...



## Limitations
You can add up to 65535 servers to this client, if you need more than this, please let me know.\
If a Server/Worker stops responding, the send command will block until all reconnection attempts are done.\
If you delete one worker object after its creation, all configured workers will be deleted, there is no "selective deletion".\
You can not change workers address until them fail a transaction.

## Building from sources
Currently, you can only Cross-Build this on Linux with mingw32.\
Visual Studio builds will be supported in a future.\
This project comes with a precompiled libzmq DLL, if you want to build/use another version, place the x86 Release type compiled DLL in lpc/bin
and make a symbolic link to libzmq.dll

- Install the MinGW compiler toolchain
- Install Cmake
- Clone this repository
- Run build-lnx.sh

## TODO
- Add usage examples
- Add a complete API documentation
- Add build documentation
- Add more compilers support
- Fix CmakeLists.txt to allow the cross build of lpclient_test.exe
- Add x64 builds for MT5 x64 platforms
- Add a pipe log for the console output and API methods for retrieving its contents.

# Improvement scenarios
Other patterns\
Ciphered transport support



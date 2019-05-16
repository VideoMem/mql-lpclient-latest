# mql-lpclient-latest
MQL binding for the ZeroMQ Lazy Pirate REQ/REP client pattern (latest x86 stable release)

## Project Description

This project contains an Object level abstraction of the Reliable Request/Reply (RRR) aka. Lazy Pirate client pattern.\
It provides a high-level abstraction for this common pattern, a REQ/REP with a settable timeout and connection retries attempts.\
It can be used as a more robust approach to plain socket interfacing/communication in MetaTrader platform.\
Servers/Workers should be a REP type, as in standard synchronous REQ/REP.

## Installation

Copy the contents of Libraries and Include folders into MetaTrader's data folder.
Allow dll imports.

## Testing
Optionally you can test its functionality installing Experts/lpclient_test.mq4 in your experts folder

- Open two [ZMQ echo servers](http://zguide.zeromq.org/py:lpserver) on localhost, the default ports are 5555 and 5566.
- Run the EA and see its output, if all tests passes successfully, the EA will loop sending test messages
to each worker on every Tick.
- Close one or both servers, see what happens, open them again, see the results.

## Limitations
You can add up to 65535 servers to this client, if you need more than this, please let me know.\
If a Server/Worker stops responding, the send command will block until all reconnection attempts are done.\
If you delete one worker object after its creation, all set up workers will be deleted, there is no "selective deletion".

## TODO
- Add usage examples
- Add more documentation
- Add x64 builds for MT5 x64 platforms

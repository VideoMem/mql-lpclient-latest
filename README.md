# mql-lpclient-latest
MQL binding for the ZeroMQ Lazy Pirate REQ/REP client pattern (latest x86 stable release)

## Project Description

This project contains an Object level abstraction of the Reliable Request/Reply (RRR) aka. Lazy Pirate pattern.
It provides a high-level abstraction for this common pattern, a REQ/REP with timeout
It can be used as a more robust approach to plain socket interfacing/communication in MetaTrader platform.

## Installation

Copy the contents of Libraries and Include folder into MetaTrader's data folder
Allow dll imports.

## Testing
Optionally you can test its functionality installing Experts/lpclient_test.mq4 in your experts folder

- Open two ZMQ echo servers on localhost, the default ports are 5555 and 5566
- Run the EA and see its output, if all tests passes successfully, the EA will loop sending test messages
to each worker on every Tick.

## TODO
- Add usage examples
- Add more documentation
- Add x64 builds for MT5 x64 platforms

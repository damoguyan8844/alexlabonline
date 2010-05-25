1. GEN_CLIENT_1
~~~~~~~~~~~~~~~

An echo client program used for the purposes of testing the server framework,
written with simplified methods from the GenericClient class.

For an explanation of the "generic client" concept, please see the file
generic_client.txt in the sources for the library portion of the framework.

The program expects the server to be running at the IP address 127.1 (the local
host), using the port 12345. These values are not configurable, but are easy to
change in the source code.

For running this program it is necessary to also run the echo server, either
Server_2 or Server_3.
(Please see http://www.codeproject.com/KB/IP/TCPServerFramework.aspx)

Running in Linux:

1) Open a terminal shell in the build directory:
   .../TC2/usage/gen_client_1/build/linux.

2) Run the script prepareEnv.sh with the shell dot command, to adjust the
   LD_LIBRARY_PATH environment variable.

3) Run the program by issuing the command ./Client_1.
   Stop the program using Ctrl-C.
   The programs displays messages on the standard output, and writes to a log
   file named YYMMDD_HHMMSS_PPP.tse (ppp is program's pid).

Running in Windows:

1) Open a command prompt window in the build output directory:
   ...\TC2\usage\gen_client_1\build\windows\VS2008\client_1\debug.

2) Run the script prepareEnv.cmd to adjust the PATH enviroment variable.

3) Run the program by issuing the command Client_1.
   Stop the program using Ctrl-C.
   The programs displays messages on the standard output, and writes to a log
   file named YYMMDD_HHMMSS_PPP.tse (ppp is program's pid).

Log files: please be aware that the log file (*.tse) can easily grow to huge sizes
if you use debug mode, or enable tracing of the messages exchanged between server
and clients. If you are concerned with disk space you must delete them as soon as
possible.

If you are a user of the vim editor there's a syntax file for the log file in
the package.

oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO

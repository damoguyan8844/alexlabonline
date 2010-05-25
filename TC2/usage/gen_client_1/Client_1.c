/// @source       Client_1.c
/// @description  An echo client used for the purposes of testing the client
/// library.

// -----------------------------------------------------------------------------

/** A minimum TCP echo client that generates messages with random contents,
    sends these messages to a server and then awaits the replies. @par

    Test with Server_2 or Server_3
*/

// -----------------------------------------------------------------------------

// includes

// common configuration options & declarations
#include "config.h"  // always include first

// C language includes
#include <stdlib.h> // srand
#include <stdio.h>
#include <time.h>

// library includes
#include "Client.h"        /* client_xxx functions */
#include "GenericClient.h" /* genCli_xxx functions */

// -----------------------------------------------------------------------------

// global declarations

// for identification purposes in the log file
static cchar sourceID[] = "CCC";

// prototypes
static void init(void);
static void prepareMessage(void);
static void processReply(void);
static void run(void);

// -----------------------------------------------------------------------------

// the main program

void main(void)
{
   init();
   run();
}

// -----------------------------------------------------------------------------

static void init(void)
{
   client_setServerAddress("127.1");
   client_setServicePort(12345);
   client_setLogLevel(LOG_LEVEL_DEBUG);
   client_setTrace(true);

// if not used, RECVs block forever
// client_setReplyTimeout(x); // say, 5 (seconds)

   genCli_init();

   srand((uint)time(NULL)); // for prepareMessage
}

// -----------------------------------------------------------------------------

static void prepareMessage(void)
{
   Message* m = genCli_message();
   uint size;  // the size of the created message
   uint size1; // for convenience
   uint maxMsgSize = client_maxMessageSize();

   // waits some time between message creation (between 0 and 3 seconds)
   int secs = rand() % 4;
   if (secs)
   {
      printf("* waiting %d seconds before creating the message\n", secs);
      client_sleep(secs);
   }

   // calculates size of new message
   size = size1 = rand() % maxMsgSize;
   if (!size1)
      size = size1 = 1;
   // size = 6; // forces size for testing purposes
   printf("* generating %d bytes\n", size);

   // stamps the message buffer with the message size
   client_setMessageSize(m, size);

   // generates the message bytes
   while (size1--)
   {
      // generates random characters between 0x20 e 0x7E (inclusive)
      // (only visible characteres and space)
      char ch = (char)(0x20 + (rand() % (0x7F - 0x20)));
      client_setMessageByte(m, size1, ch);
   }
}

// -----------------------------------------------------------------------------

static void printConnectionError(void)
{
   printf("* error %d when trying to connecto to server\n", client_error());
}

// -----------------------------------------------------------------------------

static void printOperationError(void)
{
   printf("* error %d of type %d in operation %s...\n", client_error(),
      genCli_errorType(), genCli_operation());
}

// -----------------------------------------------------------------------------

static void processReply(void)
{
   Message* m = genCli_message();
   uint size = client_messageSize(m);
   char* buffer = client_messageBuffer(m);
   printf("* reply: size=%03d buf=[%.20s]\n", size, buffer);
}

// -----------------------------------------------------------------------------

static void run(void)
{
   for (;;)
   {
      switch (genCli_waitEvent())
      {
         // ------------------------------

         case CLI_EVT_RECV_COMPLETE:
//          printf("* CLI_EVT_RECV_COMPLETE\n");
            processReply();
            prepareMessage();
            genCli_send();
            break;

         case CLI_EVT_SEND_COMPLETE:
//          printf("* CLI_EVT_SEND_COMPLETE\n");
            printf("* waiting reply...\n");
            genCli_recv();
            break;

         // ------------------------------

//       case CLI_EVT_RECV_TIMEOUT:
//          printf("* CLI_EVT_SEND_COMPLETE\n");
//          printf("* receive timeout, sending next message\n");
//          prepareMessage();
//          genCli_send();
//          break;

         // ------------------------------

         case CLI_EVT_CONNECTION_CREATED:
//          printf("* CLI_EVT_CONNECTION_CREATED\n");
            prepareMessage();
            genCli_send();
            break;

         // ------------------------------

         case CLI_EVT_CONNECT_ERROR:
//          printf("* CLI_EVT_CONNECT_ERROR\n");
            printConnectionError();
            printf("* waiting 5 seconds before retrying...\n");
            client_sleep(5);
            printf("*\n");
            printf("* retrying now...\n");
            genCli_connect();
            break;

         case CLI_EVT_CONNECTION_DESTROYED:
//          printf("* CLI_EVT_CONNECTION_DESTROYED\n");
            printOperationError();
            genCli_connect();
            break;

         // ------------------------------

         default:
            printf("*invalid event %d\n", genCli_event());
            abort();
            break;
      } // switch

   } // for

}

// -----------------------------------------------------------------------------
// the end

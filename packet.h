#include <stdint.h>
#include "socket.h"


// We have 508 Byte in single packet without fragmentation
struct Packet {
   char signature[8]; // 8- signature to check packet is a keyboard packet
   char deviceid[8];  // 8- sender uuid
   char version;      // 1- packet version
   char packet_type;  // 1- 0 noop, 1 command, 2 data
   int32_t keysym;    // 4- keyboard symcode. When 0 using key[4]
   char key[4];       // 4- 
   char shift_l;      // 1-
   char shift_r;      // 1-
   char ctrl_l;       // 1-
   char ctrl_r;       // 1-
   char alt_l;        // 1-
   char alt_r;        // 1-
   char meta_l;       // 1-
   char meta_r;       // 1-
   uint8_t paramlen;  // 1- length of command parameters
   char params[255];  // 255 - command parameters
   char any[214];     // 214 - of undefined
   uint32_t crc;      // 4 Bytes
};

/*
* Copy buffer to struct and checks it's integrity
*/
int buf_to_packet(char buf[SBUFLEN], struct Packet *p);
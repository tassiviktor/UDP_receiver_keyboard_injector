#include <stdio.h>
#include <string.h>
#include "socket.h"
#include "packet.h"

char SIGNATURE[] = "K3yBpr3s";

int buf_to_packet(char buf[SBUFLEN], struct Packet *p){

    memset(p, 0, PACKETLEN);
    
    memcpy(p, buf, PACKETLEN);
    
    if (memcmp ( &p->signature, &SIGNATURE, sizeof(p->signature) ) !=0 ){
        return -1;
    }
    
    return 0;
}
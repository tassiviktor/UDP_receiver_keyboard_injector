#!/bin/bash
gcc socket.c -o socket.o -c
gcc packet.c -o packet.o -c
gcc main.c -o shkeylistener socket.o packet.o -lX11 -lXtst -lXext -lfakekey
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <fakekey/fakekey.h>

#include "socket.h"
#include "packet.h"

#define LISTEN_PORT 29999

/* Globals are evil. TODO: unglobalize... */

Display *disp;
FakeKey *fk;

void test(Display *disp){
  
    XSync(disp,False);
    XFlush(disp);
    sleep(0.1);

    fakekey_press(fk,"É",2,0);
    fakekey_release(fk);

    fakekey_press(fk,"á",2,0);
    fakekey_release(fk);

    fakekey_press_keysym(fk,XK_F1,0);
    fakekey_release(fk);
}

int process_keyboard_event(struct Packet p){
    int modifiers = 0;
    char tmpbuffer[5];

    if (p.shift_l || p.shift_r){
        modifiers |= FAKEKEYMOD_SHIFT;
    }
    if (p.ctrl_l || p.ctrl_r){
        modifiers |= FAKEKEYMOD_CONTROL;
    }
    if (p.alt_l || p.alt_r){
        modifiers |= FAKEKEYMOD_ALT;
    }
    if (p.meta_l || p.meta_r){
        modifiers |= FAKEKEYMOD_META;
    }

    if (p.keysym != 0){
        fakekey_press_keysym(fk, p.keysym, modifiers);
        fakekey_release(fk);
    } else if (p.key != 0) {
        memset(tmpbuffer, 0, 5);
        memcpy(tmpbuffer, p.key, PACKETLEN);
        fakekey_press(fk,tmpbuffer,-1, modifiers);
        fakekey_release(fk);
    } else {
        printf("no key?");
    }
}

int listener(char c[SBUFLEN]){
    struct Packet p;

    // Skip packet on errors
    if (buf_to_packet(c, &p) != 0){
        return -1;
    }

    switch(p.packet_type) {
        case 0:
            // noop
        break;
        case 1:
            // a command
        break;
        case 2:
            return process_keyboard_event(p);
        break;
        default:
            printf("Unkown fcin cmd");
    }
    
    return 0;
}

/* Main Function */
int main (){
    
    disp = XOpenDisplay (NULL);
    fk = fakekey_init(disp);

    XSync(disp,False);
    XFlush(disp);
    sleep(0.1);
    
    sock_initialize(LISTEN_PORT);
    sock_listen(listener);
    
    return 0;

}


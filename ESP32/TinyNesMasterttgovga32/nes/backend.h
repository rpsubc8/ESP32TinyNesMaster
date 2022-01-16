#ifndef _BACKEND_H
 #define _BACKEND_H

#include "gbConfig.h"
 
//unsigned int backend_read(char *path, unsigned int offset, unsigned int count, void *buffer);
//unsigned int backend_write(char *path, unsigned int offset, unsigned int count, void *buffer);
//void backend_drawpixel(int x, int y, int nescolor);
//void backend_update();
//void backend_lock();
//void backend_unlock();
//void backend_init(int w, int h);
//void backend_event();
//unsigned int backend_getticks();
//void backend_delay(unsigned int ms);
void backend_readsavefile(char *name, unsigned char *memory);
void backend_writesavefile(char *name, unsigned char *memory);
void jj_fast_drawpixel(int x, int y, unsigned char nescolor);
void backend_read_from_memory(unsigned int offset, unsigned int count, void *buffer);
void backend_write_from_memory(unsigned int offset, unsigned int count, void *buffer);

#endif

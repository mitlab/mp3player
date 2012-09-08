#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>

#define MENU_DIR_MAXENTRIES 16
#define MENU_DIR_SNAME      12
#define MENU_DIR_LNAME      20

typedef struct {
	char 	shortame[MENU_DIR_SNAME];
	char 	dispname[MENU_DIR_LNAME];	
	uint8_t dir;
} dir_t;

dir_t menu_dir[MENU_DIR_MAXENTRIES];

// index of first entry in file-list that is to be shown on screen
uint16_t menu_dir_start = 0;
// number of files/dirs in file-list
uint16_t menu_dir_len = 0;



DIR dir;
FILINFO Finfo;


// fill menu_dir file-list
// skipping "start" number of entries while scanning FAT
void menu_builtdir(uint16_t start)
{
	FRESULT res;
	
	res = f_opendir(&dir, ".");
	if (res) { uart_puts_P("f_opendir failed\r\n"); return; }

	for(;;) 
	{
		res = f_readdir(&dir, &Finfo);
		if ((res != FR_OK) || !Finfo.fname[0]) 
			break;
	
		fn = *Finfo.lfname ? Finfo.lfname : Finfo.fname;

		if (fn[0] == '.') continue;
		
		if(Finfo.fattrib & AM_DIR)
			uart_puts_P("/");
			
		if(strstr(strlwr(fn), ".mp3")) // FIXME: memory leak?
		{
		}		
	}
}


void menu_repaint()
{

}


uint8_t menu_start(const char *path)
{
	Finfo.lfname = Lfname;
	Finfo.lfsize = sizeof(Lfname);

	res = f_mount(0, &fatfs);
	if (res) { uart_puts_P("f_mount failed\r\n"); return; }
	
	res = f_chdir("0:/music");
	if (res) { uart_puts_P("f_chdir failed\r\n"); return; }

}
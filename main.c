/*
	iTLS-Enso v2.0 by SKGleba
	All Rights Reserved
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include "graphics.h"

char mmit[][200] = {" -> Install 3.68 compat module"," -> Uninstall 3.68 compat module"," -> Install 3.69 root certs"," -> Install/Uninstall the iTLS plugin"," -> Exit"};

int optct = 5;

int sel = 0;
int i;
int shode = 0;

int ex(const char *fname) {
    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int fcp(const char *from, const char *to, int rem) {
	if (rem == 1 && ex(to) == 1) sceIoRemove(to);
	long fsz;
	FILE *fp = fopen(from,"rb");

	fseek(fp, 0, SEEK_END);
	fsz = ftell(fp);
	rewind(fp);

	char* fbuf = (char*) malloc(sizeof(char) * fsz);
	fread(fbuf, sizeof(char), (size_t)fsz, fp);

	FILE *pFile = fopen(to, "wb");
	
	for (int i = 0; i < fsz; ++i) {
			fputc(fbuf[i], pFile);
	}
   
	fclose(fp);
	fclose(pFile);
	return 1;
}

void smenu(){
	psvDebugScreenClear(COLOR_BLACK);
	psvDebugScreenSetFgColor(COLOR_CYAN);
	psvDebugScreenPrintf("                       iTLS-Enso 2.0                            \n");
	psvDebugScreenPrintf("                         By SKGleba                              \n");
		psvDebugScreenSetFgColor(COLOR_RED);
	for(i = 0; i < optct; i++){
		if(sel==i){
			psvDebugScreenSetFgColor(COLOR_GREEN);
		}
		psvDebugScreenPrintf("%s\n", mmit[i]);
		psvDebugScreenSetFgColor(COLOR_RED);
	}
	
	psvDebugScreenSetFgColor(COLOR_GREEN);
}

int do_shit(){
if (sel == 4) sceKernelExitProcess(0);
void *buf = malloc(0x100);
vshIoUmount(0x300, 0, 0, 0);
vshIoUmount(0x300, 1, 0, 0);
_vshIoMount(0x300, 0, 2, buf);
psvDebugScreenPrintf("Working...\n");
if (ex("vs0:/data/external/webcore/cpt.o") == 0) fcp("vs0:/data/external/webcore/ScePsp2Compat.suprx", "vs0:/data/external/webcore/cpt.o", 1);
if (sel == 0) fcp("app0:Media/00", "vs0:/data/external/webcore/ScePsp2Compat.suprx", 1);
if (sel == 1) fcp("vs0:/data/external/webcore/cpt.o", "vs0:/data/external/webcore/ScePsp2Compat.suprx", 1);
if (sel == 2) fcp("app0:Media/01", "vs0:/data/external/cert/CA_LIST.cer", 1);
if (sel == 3) {
sceIoMkdir("ur0:itls/", 0777);
int pex = ex("ur0:itls/itls.suprx");
int mex = ex("ur0:itls/compat.suprx");
	if (mex == 1 && pex == 1) {
		sceIoRemove("ur0:itls/itls.suprx");
		sceIoRemove("ur0:itls/compat.suprx");
	} else {
		fcp("app0:Media/69", "ur0:itls/itls.suprx", 1);
		fcp("app0:Media/00", "ur0:itls/compat.suprx", 1);
	}
}

psvDebugScreenPrintf("Done\n");
sceKernelDelayThread(1 * 1000 * 1000);sceKernelExitProcess(0);}
int main()
{
	psvDebugScreenInit();
	psvDebugScreenClear(COLOR_BLACK);
	psvDebugScreenSetFgColor(COLOR_GREEN);
	SceCtrlData pad;
	
	int l = 1;// happens
	
	smenu();
		
	while (l == 1) {
			sceCtrlPeekBufferPositive(0, &pad, 1);

			if (pad.buttons == SCE_CTRL_CROSS) {
				do_shit();
				sel = 0;
				smenu();
				sceKernelDelayThread(0.35 * 1000 * 1000);
			}
			
			if (pad.buttons == SCE_CTRL_UP) {
				if(sel!=0){
					sel--;
				}
				smenu();
				sceKernelDelayThread(0.3 * 1000 * 1000);
			}
			
			if (pad.buttons == SCE_CTRL_DOWN) {
				if(sel+1<optct){
					sel++;
				}
				smenu();
				sceKernelDelayThread(0.3 * 1000 * 1000);
			}
	}
	
	sceKernelDelayThread(10 * 1000 * 1000);
	sceKernelExitProcess(0);
    return 0;
}

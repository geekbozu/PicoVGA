
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#include "include.h"
#include "hardware/gpio.h"
extern unsigned char rawData[56064];
// Draw box
ALIGNED u8 Box[(WIDTHBYTE)*HEIGHT];
u16 trans[256];
u8 palwedongongoofedthebits[16] = {
0,
193,
4,
206,
25,
82,
15,
111,
144,
216,
173,
228,
48,
248,
124,
255
};

void VideoInit()
{
	GenPal16Trans(trans,palwedongongoofedthebits);

	// setup videomode
	VgaCfgDef(&Cfg); // get default configuration
	Cfg.video = &DRV; // video timings
	Cfg.width = WIDTH; // screen width
	Cfg.height = HEIGHT; // screen height
	Cfg.dbly = DBLY;    //Double Line hight. 
	VgaCfg(&Cfg, &Vmode); // calculate videomode setup

    // initialize base layer 0
	//Docs say todo this
	ScreenClear(pScreen);
	//Add a strip which is a "video buffer element" its ewird
	sStrip* t = ScreenAddStrip(pScreen, HEIGHT);
	//This makes our actual 4bpp screen segment
	sSegm* g = ScreenAddSegm(t, WIDTH);
	ScreenSegmGraph4(g, Box,trans, WIDTHBYTE); 

    // initialize system clock
	set_sys_clock_pll(Vmode.vco*1000, Vmode.pd1, Vmode.pd2);

	// initialize videomode
	VgaInitReq(&Vmode);
}
int main()
{
	// initialize videomode
	// run VGA core
	multicore_launch_core1(VgaCore);

	// initialize videomode
	VideoInit();

	//This copies the test image in. 
	memcpy(Box,rawData,sizeof(rawData));

	gpio_init(3);
	gpio_set_dir(3,true);
	int a = 0;
	while(true)
	{
		gpio_put(3,false);
		busy_wait_us(16000);
		gpio_put(3,true);
		busy_wait_us(16000);
	}
}

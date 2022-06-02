
// ****************************************************************************
//
//                                 Main code
//
// ****************************************************************************

#include "include.h"
#include "hardware/gpio.h"

// Draw box
ALIGNED u8 Box[(WIDTHBYTE)*HEIGHT];
u16 trans[256];
u8 pal1[16] = {
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
	GenPal16Trans(trans,pal1);

	// setup videomode
	VgaCfgDef(&Cfg); // get default configuration
	Cfg.video = &DRV; // video timings
	Cfg.width = WIDTH; // screen width
	Cfg.height = HEIGHT; // screen height
	VgaCfg(&Cfg, &Vmode); // calculate videomode setup

    // initialize base layer 0
	ScreenClear(pScreen);
	sStrip* t = ScreenAddStrip(pScreen, HEIGHT);

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
	int c = 0;
	int o = 0;
	for(int i = 0;i<16;i++){
		for(int j=0;j<(WIDTHBYTE)*30;j++){
			Box[o] = (c) | ((c) << 4);
			o++;
		}
		c++;
	}
	Box[0] = 15;
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

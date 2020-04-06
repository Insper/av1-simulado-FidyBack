/************************************************************************/
/* includes                                                             */
/************************************************************************/
#include <asf.h>
#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/
// Led 1
#define LED1_PIO			PIOA
#define LED1_PIO_ID			ID_PIOA
#define LED1_PIO_IDX		0
#define LED1_PIO_IDX_MASK	(1u << LED1_PIO_IDX)

// Led 2
#define LED2_PIO			PIOC
#define LED2_PIO_ID			ID_PIOC
#define LED2_PIO_IDX		30
#define LED2_PIO_IDX_MASK	(1u << LED2_PIO_IDX)

// Led 3
#define LED3_PIO			PIOB
#define LED3_PIO_ID			ID_PIOB
#define LED3_PIO_IDX		2
#define LED3_PIO_IDX_MASK	(1u << LED3_PIO_IDX)

// Botão 1
#define BUT1_PIO			PIOD
#define BUT1_PIO_ID			ID_PIOD
#define BUT1_PIO_IDX		28
#define BUT1_PIO_IDX_MASK	(1u << BUT1_PIO_IDX)

// Botão 2
#define BUT2_PIO			PIOC
#define BUT2_PIO_ID			ID_PIOC
#define BUT2_PIO_IDX		31
#define BUT2_PIO_IDX_MASK	(1u << BUT2_PIO_IDX)

// Botão 3
#define BUT3_PIO			PIOA
#define BUT3_PIO_ID			ID_PIOA
#define BUT3_PIO_IDX		19
#define BUT3_PIO_IDX_MASK	(1u << BUT3_PIO_IDX)

/************************************************************************/
/* Variaveis Globais                                                    */
/************************************************************************/
volatile char flag_but_1 = 0;
volatile char flag_but_2 = 0;
volatile char flag_but_3 = 0;

/************************************************************************/
/* Protótipos                                                           */
/************************************************************************/
void init(void);
void but1_callBack(void);
void but2_callBack(void);
void but3_callBack(void);

/************************************************************************/
/* CallBack / Handler													*/
/************************************************************************/
void but1_callBack(void){
	flag_but_1 = 1;
}

void but2_callBack(void) {
	flag_but_2 = 1;
}

void but3_callBack(void) {
	flag_but_3 = 1;
}

/************************************************************************/
/* Functions                                                            */
/************************************************************************/
void init(void) {
	// Inicia partes do sistema
	board_init();
	sysclk_init();
	delay_init();
	WDT->WDT_MR = WDT_MR_WDDIS;

	// Init OLED
	gfx_mono_ssd1306_init();
	
	// Ativa os periféricos que serão usados
	pmc_enable_periph_clk(LED1_PIO_ID);
	pmc_enable_periph_clk(LED2_PIO_ID);
	pmc_enable_periph_clk(LED3_PIO_ID);
	pmc_enable_periph_clk(BUT1_PIO_ID);
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pmc_enable_periph_clk(BUT3_PIO_ID);
	
	// Inicia os periféricos de saida
	pio_set_output(LED1_PIO, LED1_PIO_IDX_MASK, 0, 0, 0);
	pio_set_output(LED2_PIO, LED2_PIO_IDX_MASK, 1, 0, 0);
	pio_set_output(LED3_PIO, LED3_PIO_IDX_MASK, 0, 0, 0);
	
	// Inicia os periféricos de entrada
	pio_set_input(BUT1_PIO, BUT1_PIO_IDX_MASK, PIO_DEFAULT);
	pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_DEFAULT);
	pio_set_input(BUT3_PIO, BUT3_PIO_IDX_MASK, PIO_DEFAULT);
	
	// Inicia os Callbacks
	pio_handler_set(BUT1_PIO, BUT1_PIO_ID, BUT1_PIO_IDX_MASK, PIO_IT_RISE_EDGE, but1_callBack);
	pio_handler_set(BUT2_PIO, BUT2_PIO_ID, BUT2_PIO_IDX_MASK, PIO_IT_FALL_EDGE, but2_callBack);
	pio_handler_set(BUT3_PIO, BUT3_PIO_ID, BUT3_PIO_IDX_MASK, PIO_IT_RISE_EDGE, but3_callBack);
	
	// Ativa interrupção
	pio_enable_interrupt(BUT1_PIO, BUT1_PIO_IDX_MASK);
	pio_enable_interrupt(BUT2_PIO, BUT2_PIO_IDX_MASK);
	pio_enable_interrupt(BUT3_PIO, BUT3_PIO_IDX_MASK);

	// Configura NVIC
	NVIC_EnableIRQ(BUT1_PIO_ID);
	NVIC_EnableIRQ(BUT2_PIO_ID);
	NVIC_EnableIRQ(BUT3_PIO_ID);
}

int main (void)
{
	init();

  // Escreve na tela um circulo e um texto
	gfx_mono_draw_filled_circle(20, 16, 16, GFX_PIXEL_SET, GFX_WHOLE);
	gfx_mono_draw_string("mundo", 50,16, &sysfont);

  /* Insert application code here, after the board has been initialized. */
	while(1) {
		if (flag_but_1) {
			flag_but_1 = 0;
			pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
			delay_ms(200);
			pio_clear(LED1_PIO, LED1_PIO_IDX_MASK);
			delay_ms(200);
		}

	}
}

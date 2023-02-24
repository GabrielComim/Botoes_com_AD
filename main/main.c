/**
 * @brief 
 * HARDWARE: CONSISTE EM 4 BOTÕES LIGADOS EM UMA ENTRADA AD PARA CAPTAR QUAL FOI ACIONADO CONFORME
 * A TENSÃO QUE É GERADA NO AD.
 * UM DISPLAY OLED 128X64.
 * FIRMWARE: CONSISTE EM UM MENU QUE É NAVEGÁVEL ATRAVÉS DOS BOTÕES, NA SEGUINTE CONFIGURAÇÃO:
 * 			BOTÃO 1 - SETA PARA CIMA
 * 			BOTÃO 2 - ENTER
 * 			BOTÃO 3 - SETA PARA BAIXO 
 * 			BOTÃO 4 - VOLTAR
 * 			É POSSÍVEL ENTRAR E SAIR DOS MENUS ATRAVÉS DOS BOTÕES.
 * @autor: Gabriel Comim
 * @date: 23/02/23 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "ad_configure.h"
#include "ssd1306.h"
#include "display_oled.h"
#include <inttypes.h>

#define DEBUG

// Para o AD
#define DEFAULT_VREF    1100        
#define N_SAMPLES   32          //Multisampling
static const adc_channel_t channel = ADC_CHANNEL_0;     //GPIO36 if ADC1
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;	// 12 bits
static const adc_atten_t atten = ADC_ATTEN_DB_11;		// Atenuation
static const adc_unit_t unit = ADC_UNIT_1;				// ADC1 ou ADC2

// Para o Display
bool CONFIG_I2C_INTERFACE = 1;
const int CONFIG_SSD1306_128x64 = 1;
gpio_num_t SDA_GPIO = 21;
gpio_num_t SCL_GPIO = 22;
bool CONFIG_RESET_GPIO = 0;

SSD1306_t display;

/**
 * flag_menu_1 = 100 = seta no primeiro item
 * flag_menu_1 = 010 = seta no segundo item
 * flag_menu_1 = 001 = seta no terceiro item
 * flag_menu_1 = 111 = entrou no menu 2 ou 3 ou 4
 * */ 
char flag_menu_1[4] = {0,0,0};
char flag_menu_2[4] = {0,0,0};
char flag_menu_3[4] = {0,0,0};
char flag_menu_4[4] = {0,0,0};
/**
 * MENU 1 - LEDS; BOTOES; CONECTORES
 * MENU 2 - LED RED; LED GREEN; LED YELLOW
 * MENU 3 - BOTA 1; BOTAO 2; BOTAO 3
 * MENU 4 - CONEC 1; CONEC 2; CONEC 3
*/
int menu_1 = 1;		// Identificador de qual tela está o programa
int menu_2 = 0;		// Identificador de qual tela está o programa
int menu_3 = 0;		// Identificador de qual tela está o programa
int menu_4 = 0;		// Identificador de qual tela está o programa

void func_botao_1()
{
//****************************************************************************************************//
	// MENU 1
	if(strcmp(flag_menu_1, "100") == 0)
	{
		ssd1306_display_text( &display, 2, "LEDS   ", 7, false);
		ssd1306_display_text( &display, 4, "CONECTORES <-", 13, false);
		strcpy(flag_menu_1, "001");
	}
	else if(strcmp(flag_menu_1, "010") == 0)
	{
		ssd1306_display_text( &display, 2, "LEDS <-", 7, false);
		ssd1306_display_text( &display, 3, "BOTOES   ", 9, false);
		strcpy(flag_menu_1, "100");
	}
	else if(strcmp(flag_menu_1, "001" ) == 0)
	{
		ssd1306_display_text( &display, 3, "BOTOES <-", 9, false);
		ssd1306_display_text( &display, 4, "CONECTORES   ", 13, false);
		strcpy(flag_menu_1, "010");
	}
//**************************************************************************************************//
	// MENU 2
	if(strcmp(flag_menu_2, "100") == 0)
	{
		ssd1306_display_text( &display, 2, "LEDS RED   ", 12, false);
		ssd1306_display_text( &display, 4, "LEDS YELLOW <-", 14, false);
		strcpy(flag_menu_2, "001");
	}
	else if(strcmp(flag_menu_2, "010") == 0)
	{
		ssd1306_display_text( &display, 3, "LEDS GREEN   ", 13, false);
		ssd1306_display_text( &display, 2, "LEDS RED <-", 12, false);
		strcpy(flag_menu_2, "100");
	}
	else if(strcmp(flag_menu_2, "001") == 0)
	{
		ssd1306_display_text( &display, 3, "LEDS GREEN <-", 13, false);
		ssd1306_display_text( &display, 4, "LEDS YELLOW   ", 14, false);
		strcpy(flag_menu_2, "010");
	}
//********************************************************************************************//
	// MENU 3
	if(strcmp(flag_menu_3, "100") == 0)
	{
		ssd1306_display_text( &display, 2, "BOTAO 1   ", 10, false);
		ssd1306_display_text( &display, 4, "BOTAO 3 <-", 10, false);
		strcpy(flag_menu_3, "001");
	}
	else if(strcmp(flag_menu_3, "010") == 0)
	{
		ssd1306_display_text( &display, 3, "BOTAO 2   ", 10, false);
		ssd1306_display_text( &display, 2, "BOTAO 1 <-", 10, false);
		strcpy(flag_menu_3, "100");
	}
	else if(strcmp(flag_menu_3, "001") == 0)
	{
		ssd1306_display_text( &display, 3, "BOTAO 2 <-", 10, false);
		ssd1306_display_text( &display, 4, "BOTAO 3   ", 10, false);
		strcpy(flag_menu_3, "010");
	}
//********************************************************************************************//
	// MENU 4
	if(strcmp(flag_menu_4, "100") == 0)
	{
		ssd1306_display_text( &display, 2, "CONEC 1   ", 10, false);
		ssd1306_display_text( &display, 4, "CONEC 3 <-", 10, false);
		strcpy(flag_menu_4, "001");
	}
	else if(strcmp(flag_menu_4, "010") == 0)
	{
		ssd1306_display_text( &display, 3, "CONEC 2   ", 10, false);
		ssd1306_display_text( &display, 2, "CONEC 1 <-", 10, false);
		strcpy(flag_menu_4, "100");
	}
	else if(strcmp(flag_menu_4, "001") == 0)
	{
		ssd1306_display_text( &display, 3, "CONEC 2 <-", 10, false);
		ssd1306_display_text( &display, 4, "CONEC 3   ", 10, false);
		strcpy(flag_menu_4, "010");
	}
}

void func_botao_2()
{
	if (menu_1 == 1)
	{
		ssd1306_clear_screen(&display, false);
	}
//*********************************************************************************************//
	// MENU 2
	if(strcmp(flag_menu_1, "100" ) == 0)
	{
		ssd1306_display_text( &display, 2, "LEDS RED <-", 12, false);
		ssd1306_display_text( &display, 3, "LEDS GREEN   ", 13, false);
		ssd1306_display_text( &display, 4, "LEDS YELLOW   ", 14, false);
		menu_1 = 0;		
		strcpy(flag_menu_1, "111"); 
		menu_2 = 1;
		strcpy(flag_menu_2, "100"); 
		#ifdef DEBUG
			ESP_LOGI("I", "TROCA DE MENU");
		#endif
	}
//**************************************************************************************************//
	// MENU 3
	else if(strcmp(flag_menu_1, "010" ) == 0)
	{
		ssd1306_display_text( &display, 2, "BOTAO 1 <-", 10, false);
		ssd1306_display_text( &display, 3, "BOTAO 2   ", 10, false);
		ssd1306_display_text( &display, 4, "BOTAO 3   ", 10, false);
		menu_1 = 0;
		strcpy(flag_menu_1, "111"); 
		menu_3 = 1;
		strcpy(flag_menu_3, "100");
		#ifdef DEBUG
			ESP_LOGI("I", "TROCA DE MENU");
		#endif
	}
//**************************************************************************************************//
	// MENU 4
	else if(strcmp(flag_menu_1, "001" ) == 0)
	{
		ssd1306_display_text( &display, 2, "CONEC 1 <-", 10, false);
		ssd1306_display_text( &display, 3, "CONEC 2   ", 10, false);
		ssd1306_display_text( &display, 4, "CONEC 3      ", 13, false);
		menu_1 = 0;
		strcpy(flag_menu_1, "111"); 
		menu_4 = 1;
		strcpy(flag_menu_4, "100");
		#ifdef DEBUG
			ESP_LOGI("I", "TROCA DE MENU");
		#endif
	}
}

void func_botao_3()
{
//********************************************************************************************//
	// MENU 1
	if(strcmp(flag_menu_1, "100" ) == 0)
	{
		ssd1306_display_text( &display, 2, "LEDS   ", 7, false);
		ssd1306_display_text( &display, 3, "BOTOES <-", 9, false );
		strcpy(flag_menu_1, "010");
		#ifdef DEBUG
			ESP_LOGI("i","flag_menu_1 = %s", flag_menu_1);
		#endif
	}
	
	else if(strcmp(flag_menu_1, "010" ) == 0)
	{
		ssd1306_display_text( &display, 3, "BOTOES   ", 9, false);
		ssd1306_display_text( &display, 4, "CONECTORES <-", 13, false );
		strcpy(flag_menu_1, "001");
		#ifdef DEBUG
			ESP_LOGI("i","flag_menu_1 = %s", flag_menu_1);
		#endif
	}
	
	else if(strcmp(flag_menu_1, "001" ) == 0)
	{
		ssd1306_display_text( &display, 2, "LEDS <-", 7, false);
		ssd1306_display_text( &display, 4, "CONECTORES   ", 13, false );
		strcpy(flag_menu_1, "100");
		#ifdef DEBUG
			ESP_LOGI("i","flag_menu_1 = %s", flag_menu_1);
		#endif
	}
//**************************************************************************************************//
	// MENU 2
	if(strcmp(flag_menu_2, "100") == 0)
	{
		ssd1306_display_text( &display, 2, "LEDS RED   ", 12, false);
		ssd1306_display_text( &display, 3, "LEDS GREEN <-", 13, false);
		strcpy(flag_menu_2, "010");
	}
	else if(strcmp(flag_menu_2, "010") == 0)
	{
		ssd1306_display_text( &display, 3, "LEDS GREEN   ", 13, false);
		ssd1306_display_text( &display, 4, "LEDS YELLOW <-", 14, false);
		strcpy(flag_menu_2, "001");
	}
	else if(strcmp(flag_menu_2, "001") == 0)
	{
		ssd1306_display_text( &display, 2, "LEDS RED <-", 12, false);
		ssd1306_display_text( &display, 4, "LEDS YELLOW   ", 14, false);
		strcpy(flag_menu_2, "100");
	}
//********************************************************************************************//
	// MENU 3
	if(strcmp(flag_menu_3, "100") == 0)
	{
		ssd1306_display_text( &display, 2, "BOTAO 1   ", 10, false);
		ssd1306_display_text( &display, 3, "BOTAO 2 <-", 10, false);
		strcpy(flag_menu_3, "010");
	}
	else if(strcmp(flag_menu_3, "010") == 0)
	{
		ssd1306_display_text( &display, 3, "BOTAO 2   ", 10, false);
		ssd1306_display_text( &display, 4, "BOTAO 3 <-", 10, false);
		strcpy(flag_menu_3, "001");
	}
	else if(strcmp(flag_menu_3, "001") == 0)
	{
		ssd1306_display_text( &display, 4, "BOTAO 3   ", 10, false);
		ssd1306_display_text( &display, 2, "BOTAO 1 <-", 10, false);
		strcpy(flag_menu_3, "100");
	}
//********************************************************************************************//
	// MENU 4
	if(strcmp(flag_menu_4, "100") == 0)
	{
		ssd1306_display_text( &display, 2, "CONEC 1   ", 10, false);
		ssd1306_display_text( &display, 3, "CONEC 2 <-", 10, false);
		strcpy(flag_menu_4, "010");
	}
	else if(strcmp(flag_menu_4, "010") == 0)
	{
		ssd1306_display_text( &display, 3, "CONEC 2   ", 10, false);
		ssd1306_display_text( &display, 4, "CONEC 3 <-", 10, false);
		strcpy(flag_menu_4, "001");
	}
	else if(strcmp(flag_menu_4, "001") == 0)
	{
		ssd1306_display_text( &display, 4, "CONEC 3   ", 10, false);
		ssd1306_display_text( &display, 2, "CONEC 1 <-", 10, false);
		strcpy(flag_menu_4, "100");
	}
}

void func_botao_4()
{
	ssd1306_clear_screen(&display, false);
//************************************************************************************************//	
	// MENU 1
	if(menu_1 == 1)
	{	
		ESP_LOGI("I","FUNCAO INVALIDA");
		ssd1306_display_text( &display, 0, "ESP32", 5, false);
		ssd1306_display_text( &display, 2, "LEDS <-", 7, false);
		ssd1306_display_text( &display, 3, "BOTOES", 6, false);
		ssd1306_display_text( &display, 4, "CONECTORES", 10, false);
	}
//************************************************************************************************//	
	// MENU 2 E MENU 3 E MENU 4
	if(menu_2 == 1 || menu_3 == 1 || menu_4 == 1)
	{	
		ssd1306_display_text( &display, 0, "ESP32", 5, false);
		ssd1306_display_text( &display, 2, "LEDS <-", 7, false);
		ssd1306_display_text( &display, 3, "BOTOES", 6, false);
		ssd1306_display_text( &display, 4, "CONECTORES", 10, false);
		menu_2 = 0; menu_3 = 0; menu_4 = 0;
		strcpy(flag_menu_1,"100");
		strcpy(flag_menu_2,"000"); 
		strcpy(flag_menu_3,"000");
		strcpy(flag_menu_4,"000");
		menu_1 = 1;
	}
}

void app_main(void)
{
	//Configure ADC
    configure_adc(unit, width, channel, atten, DEFAULT_VREF);

	// Inicia no Menu 1
	display_init( &display, 21, 22, CONFIG_RESET_GPIO);
	ssd1306_display_text( &display, 0, "ESP32", 5, false);
	ssd1306_display_text( &display, 2, "LEDS <-", 7, false);
	ssd1306_display_text( &display, 3, "BOTOES", 6, false);
	ssd1306_display_text( &display, 4, "CONECTORES", 10, false);
	if (menu_1 == 1)
	{	strcpy(flag_menu_1, "100");	}
		
    while (1) 
	{
			//Leitura do AD
			uint32_t value_adc1_voltage = adc_read(N_SAMPLES, channel);
			vTaskDelay(pdMS_TO_TICKS(200));

			// Botão 1
			if(value_adc1_voltage > 800 && value_adc1_voltage < 1000)
			{	func_botao_1();		}

			//Botão 2
        	if(value_adc1_voltage > 1600 && value_adc1_voltage < 1700)
			{	func_botao_2();		}
		
			//Botão 3
			if(value_adc1_voltage > 2400 && value_adc1_voltage < 2600)
			{	func_botao_3();	}

			//Botão 4
			if(value_adc1_voltage > 3100)
			{	func_botao_4();	}
    }
}
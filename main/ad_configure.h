
#include "driver/adc.h"
#include "esp_adc_cal.h"

static esp_adc_cal_characteristics_t *adc_chars;

void configure_adc(adc_unit_t unit, adc_bits_width_t width, adc_channel_t channel,adc_atten_t atten, uint32_t DEFAULT_VREF)
{
    //Verifica se é o canal 1 ou 2 para configurar quantos bits e a atenuação
    if (unit == ADC_UNIT_1)
	{
        adc1_config_width(width);
        adc1_config_channel_atten(channel, atten);
    } 
    else if (unit == ADC_UNIT_2)
    {
        adc2_config_channel_atten(channel, atten);
    }   

    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);	// Necessary to conversion for voltage

}

uint32_t adc_read(uint N_SAMPLES, adc_channel_t channel)
{
    uint32_t adc_reading = 0;
        
    //Multisampling
    for (int i = 0; i < N_SAMPLES; i++) 
	{	adc_reading += adc1_get_raw(channel);  }

    adc_reading /= N_SAMPLES; 
    //Convert adc_reading to voltage in mV
    uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
    //ESP_LOGI("I", "Raw: %lu \tVoltage: %lu mV\n", adc_reading, voltage); 
    
    return (voltage); 
}
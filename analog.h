
// initialize adc
void adc_init();
uint16_t adc_read(uint8_t);

volatile uint8_t channel_mask;
void check_adc_changes();
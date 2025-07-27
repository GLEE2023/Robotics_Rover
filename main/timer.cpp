#include "timer.hpp"

gptimer_handle_t timerUltrasonic = NULL;
gptimer_handle_t timerUART       = NULL;

void timerUltrasonicInit() {
  /* Ultrasonic Timer Config */
  gptimer_config_t configUltrasonic = {
    .clk_src = GPTIMER_CLK_SRC_APB, //default clock is 80Mhz
    .direction = GPTIMER_COUNT_UP,
    .resolution_hz = 10000,  // 10 kHz = 1 tick = 0.1 ms //need to stay within range of 2 and 65536 so 1kHz is not enough but 10kHz is good
  };
  ESP_ERROR_CHECK(gptimer_new_timer(&configUltrasonic, &timerUltrasonic));

  gptimer_event_callbacks_t callbackTimerUltrasonic = {
    .on_alarm = timerUltrasonicInterrupt,
  };
  ESP_ERROR_CHECK(gptimer_register_event_callbacks(timerUltrasonic, &callbackTimerUltrasonic, NULL));//setup interrupts for timer

  gptimer_alarm_config_t alarm_configUltrasonic = {
    .alarm_count = 500,  // 5s
    .reload_count = 0, //reset timer to 0 after interrupt trigger
    .flags = { .auto_reload_on_alarm = true }
  };
  ESP_ERROR_CHECK(gptimer_set_alarm_action(timerUltrasonic, &alarm_configUltrasonic));
  ESP_ERROR_CHECK(gptimer_enable(timerUltrasonic));
  ESP_ERROR_CHECK(gptimer_start(timerUltrasonic)); //Start ultrasonic timer
}

void timerUARTInit() {
  /* Ultrasonic Timer Config */
  gptimer_config_t configUART = {
    .clk_src = GPTIMER_CLK_SRC_APB,
    .direction = GPTIMER_COUNT_UP,
    .resolution_hz = 10000,  // 10 kHz = 1 tick = 0.1 ms //need to be this large to stay in prescaler range
  };
  ESP_ERROR_CHECK(gptimer_new_timer(&configUART, &timerUART));

  gptimer_event_callbacks_t callbackTimerUART = {
    .on_alarm = timerUARTInterrupt,
  };
  ESP_ERROR_CHECK(gptimer_register_event_callbacks(timerUART, &callbackTimerUART, NULL));//setup interrupts for timer

  gptimer_alarm_config_t alarm_configUART = {
    .alarm_count = 200,  // 2s
    .reload_count = 0, //reset timer to 0 after interrupt trigger
    .flags = { .auto_reload_on_alarm = false } //Do not reset and count up again once triggered
  };
  ESP_ERROR_CHECK(gptimer_set_alarm_action(timerUART, &alarm_configUART));
  ESP_ERROR_CHECK(gptimer_enable(timerUART));/* Note it doesn't start timer*/
}

void timerUARTStart(){
  ESP_ERROR_CHECK(gptimer_start(timerUART));
}

//Ultrasonic Interrupt
bool IRAM_ATTR timerUltrasonicInterrupt(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
  addSchedulerEvent(ULTRASONIC_SEND_EVENT);
  return false;  
}

bool IRAM_ATTR timerUARTInterrupt(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
  addSchedulerEvent(UART_CONFIRMATION_EVENT);
  return false;
}
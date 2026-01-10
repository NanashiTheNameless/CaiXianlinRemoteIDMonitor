// THIS CODE IS LICENSED UNDER THE "BSD 2-Clause License" FOUND IN THE LICENSE FILE LOCATED AT https://github.com/nanashiTheNameless/CaiXianlinRemoteIDMonitor/tree/main/libraries.
// IT IS NOT LICENSED UNDER THE LICENSE FILE FOUND IN THE ROOT DIRECTORY OF THIS PROJECT.
// THIS VERSION IS MODIFIED TO BETTER SUIT THE PURPOSE NEEDED FOR THIS PROJECT.

#ifndef _COLLAR_H
#define _COLLAR_H

#include <stdint.h>
#include "Arduino.h"

enum collar_mode { SHOCK = 1, VIBE = 2, BEEP = 3, LIGHT = 4 };
enum collar_channel { CH1 = 0, CH2 = 1, CH3 = 2 };

struct collar_message {
  uint16_t id;
  collar_mode mode;
  collar_channel channel;
  uint8_t power;
};

typedef void (*msg_cb_t)(const struct collar_message *msg, void *userdata);

class CollarRx {
 public:
  CollarRx(uint8_t rx_pin, msg_cb_t cb, void *userdata);
  static CollarRx *_instance;
  static void s_isr();

 private:
  void rx_start();
  bool is_message_valid(const uint8_t buffer[5]);
  void buffer_to_collar_message(const uint8_t buffer[5], struct collar_message *msg);
  void isr();

 protected:
  uint8_t _rx_pin;
  void *_userdata;
  msg_cb_t _cb;
  struct collar_message _rx_msg;
};

#endif

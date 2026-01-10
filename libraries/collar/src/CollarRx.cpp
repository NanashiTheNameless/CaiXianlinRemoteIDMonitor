// THIS CODE IS LICENSED UNDER THE "BSD 2-Clause License" FOUND IN THE LICENSE FILE LOCATED AT https://github.com/nanashiTheNameless/CaiXianlinRemoteIDMonitor/tree/main/libraries.
// IT IS NOT LICENSED UNDER THE LICENSE FILE FOUND IN THE ROOT DIRECTORY OF THIS PROJECT.
// THIS VERSION IS MODIFIED TO BETTER SUIT THE PURPOSE NEEDED FOR THIS PROJECT.

#include "collar.h"

#define START_PULSE_LEN_US 2200
#define START_PULSE_TOLLERANCE 100
#define PULSE_LEN_US 1000
#define PULSE_TOLLERANCE 200
#define HIGH_THERESHOLD 500

CollarRx* CollarRx::_instance;

CollarRx::CollarRx(uint8_t rx_pin, msg_cb_t cb, void *userdata)
{
  _rx_pin = rx_pin;
  _cb = cb;
  _userdata = userdata;

  rx_start();
}

void CollarRx::rx_start()
{
  #ifdef ISR_MONITOR_PIN
   pinMode(ISR_MONITOR_PIN, OUTPUT);
  #endif

  _instance = this;
  pinMode(_rx_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(_rx_pin), CollarRx::s_isr, CHANGE);
}

void CollarRx::s_isr()
{
  _instance->isr();
}

void CollarRx::buffer_to_collar_message(const uint8_t buffer[5], struct collar_message *msg)
{
  // wipe old message
  memset(msg, 0, sizeof(struct collar_message));

  // bytes 0&1 = ID
  msg->id = ((buffer[0] << 8) | buffer[1]);

  // byte 2 = mode & channel
  msg->mode = (collar_mode)(buffer[2] & 0x0F);
  msg->channel = (collar_channel)((buffer[2] & 0xF0) >> 4);

  // byte 3 = power
  msg->power = buffer[3];
}

bool CollarRx::is_message_valid(const uint8_t buffer[5])
{
  // calculate checksum
  uint8_t chk = 0;
  for (uint8_t i = 0; i < 4; i++)
    chk += buffer[i];

  // validate checksum is correct
  if (chk != buffer[4])
    return false;

  return true;
}

void CollarRx::isr()
{
  #ifdef ISR_MONITOR_PIN
    digitalWrite(ISR_MONITOR_PIN, HIGH);
  #endif

  static unsigned long rx_micros = 0;
  static unsigned int high_pulse_len = 0;
  static unsigned int low_pulse_len = 0;
  static uint8_t buffer[5];        // expecting to receive 5 bytes
  static uint8_t byte_position = -1; // keep track of current byte being received, if -1 no start received
  static uint8_t bit_position = 0; // keep track of expected next bit position in byte

  if (digitalRead(_rx_pin) == 0)
  { // falling edge
    high_pulse_len = micros() - rx_micros;
    rx_micros = micros(); // start measurement of pulse length for low state
    #ifdef ISR_MONITOR_PIN
      digitalWrite(ISR_MONITOR_PIN, LOW);
    #endif
    return;
  }
  else
  { // rising edge
    low_pulse_len = micros() - rx_micros;
    rx_micros = micros(); // start measurement of pulse length for high state

    // look for transmission start state (rising edge->rising edge of ~2.2ms)
    int last_pulse_len = high_pulse_len + low_pulse_len;
    if ((last_pulse_len > START_PULSE_LEN_US - START_PULSE_TOLLERANCE) &&
       (last_pulse_len < START_PULSE_LEN_US + START_PULSE_TOLLERANCE))
    {
        byte_position = 0;
        bit_position = 0;
        memset(buffer, 0, sizeof(buffer));
        #ifdef ISR_MONITOR_PIN
          digitalWrite(ISR_MONITOR_PIN, LOW);
        #endif
        return;
    }
    else
    {
      if (byte_position >= 0)
      { // we are receiving data
        if ((last_pulse_len > PULSE_LEN_US - PULSE_TOLLERANCE) &&
            (last_pulse_len < PULSE_LEN_US + PULSE_TOLLERANCE))
        { // seems we got a valid bit
          uint8_t val = (high_pulse_len > HIGH_THERESHOLD); // if high longer than 500us we got a 1 else we got a 0
          {
            if (val)
              buffer[byte_position] |= (1 << (7 - bit_position));

            if (++bit_position >= 8)
            {
              bit_position = 0;
              byte_position++;
            }

            if (byte_position >= sizeof(buffer))
            {
              if (is_message_valid(buffer))
              {
                buffer_to_collar_message(buffer, &_rx_msg);
                _cb(&_rx_msg, _userdata);
                memset(buffer, 0, sizeof(buffer));
                byte_position = -1; // done, wait for new start
                #ifdef ISR_MONITOR_PIN
                  digitalWrite(ISR_MONITOR_PIN, LOW);
                #endif
                return;
              }
              else
              {
                // invalid message, reset and wait for new start
                memset(buffer, 0, sizeof(buffer));
                byte_position = -1;
                bit_position = 0;
                #ifdef ISR_MONITOR_PIN
                  digitalWrite(ISR_MONITOR_PIN, LOW);
                #endif
                return;
              }
            }
          }
        }
        else
        {
          // transmission error, wait for new start
          byte_position = -1;
          #ifdef ISR_MONITOR_PIN
             digitalWrite(ISR_MONITOR_PIN, LOW);
          #endif
          return;
        }
      }
    }
  }
  #ifdef ISR_MONITOR_PIN
      digitalWrite(ISR_MONITOR_PIN, LOW);
  #endif
}


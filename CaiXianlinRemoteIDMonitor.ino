//This is licensed under the BSD 2-Clause License

#include "libraries/collar/src/collar.h"

const uint8_t rx_pin = 20;  // Pin connected to 433MHz receiver
const unsigned long DEBOUNCE_MS = 200;  // Debounce time to slow duplicate messages

CollarRx *_rx;
volatile bool got_message = false;
struct collar_message rx_message;
unsigned long last_message_time = 0;

// Callback when a message is received
void message_callback(const struct collar_message *msg, void *userdata)
{
  memcpy(&rx_message, msg, sizeof(struct collar_message));
  got_message = true;
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Shock ID Monitor Started");

  _rx = new CollarRx(rx_pin, message_callback, NULL);
}

void loop()
{
  if (got_message)
  {
    unsigned long current_time = millis();
    
    // Ignore invalid messages (id=0 or mode=0)
    if (rx_message.id != 0 && rx_message.mode != 0)
    {
      // Only print if enough time has passed since last message (debounce)
      if (current_time - last_message_time >= DEBOUNCE_MS)
      {
        Serial.print("ID: 0x");
        Serial.print(rx_message.id, HEX);
        Serial.print(" (");
        Serial.print(rx_message.id);
        Serial.print(") | Channel: ");
        Serial.print(rx_message.channel + 1);
        Serial.print(" | Mode: ");

        switch(rx_message.mode)
        {
          case SHOCK:
            Serial.println("Shock");
            break;
          case VIBE:
            if (rx_message.power == 0) {
              Serial.println("Stop");
            } else {
              Serial.println("Vibrate");
            }
            break;
          case BEEP:
            Serial.println("Beep");
            break;
          case LIGHT:
            Serial.println("Light");
            break;
          default:
            Serial.println("Unknown");
        }
        
        last_message_time = current_time;
      }
    }

    got_message = false;
  }
}


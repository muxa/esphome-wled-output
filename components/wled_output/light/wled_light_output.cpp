#ifdef USE_ARDUINO

#include "wled_light_output.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

#ifdef USE_ESP32
#include <WiFi.h>
#endif

#ifdef USE_ESP8266
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#endif

namespace esphome {
namespace wled {

// Description of protocols:
// https://github.com/Aircoookie/WLED/wiki/UDP-Realtime-Control
enum Protocol { WLED_NOTIFIER = 0, WARLS = 1, DRGB = 2, DRGBW = 3, DNRGB = 4 };

static const char *const TAG = "wled_light_output";

void WLEDLightOutput::on_shutdown() {
  if (udp_) {
    udp_->stop();
    udp_.reset();
  }

  Component::on_shutdown();
}

void WLEDLightOutput::setup() {
  ESP_LOGCONFIG(TAG, "Setting up WLED Light Output...");  

  // Init UDP lazily
  if (!udp_) {
    udp_ = make_unique<WiFiUDP>();

    if (!udp_->beginPacket(address_, port_)) {
      ESP_LOGW(TAG, "Cannot connect WLEDLightOutput to %d.", port_);
      return;
    }
    // Byte 0 of the UDP packet tells the server which realtime protocol to use.
    udp_->write(WARLS);

    // In every protocol, Byte 1 tells the server how many seconds to wait after the last received packet before returning to normal mode, in practice you should use 1-2 (seconds) here in most cases so that the module returns to normal mode quickly after the end of transmission. Use 255 to stay on the UDP data without a timeout until a request is requested via another method.
    udp_->write(5);

    // After this the LED color information is transmitted like this:
    // WARLS
    // Byte	Description
    // 2 + n*4	LED Index
    // 3 + n*4	Red Value
    // 4 + n*4	Green Value
    // 5 + n*4	Blue Value

    udp_->write(0); udp_->write(255); udp_->write(0); udp_->write(0);
    udp_->write(1); udp_->write(0); udp_->write(255); udp_->write(0);
    udp_->write(3); udp_->write(0); udp_->write(0); udp_->write(255);

    if (!udp_->endPacket()) {
      ESP_LOGW(TAG, "Cannot send packet from WLEDLightOutput to %d.", port_);
      return;
    }
  }
}

void WLEDLightOutput::dump_config() {
  ESP_LOGCONFIG(TAG, "WLED Light Output:");
  ESP_LOGCONFIG(TAG, "  Num LEDs: %u", this->num_leds_);
  //ESP_LOGCONFIG(TAG, "  Max refresh rate: %u", *this->max_refresh_rate_);
}
void WLEDLightOutput::write_state(light::LightState *state) {
  // TODO: send packets here
  this->mark_shown_();
}

}  // namespace wled
}  // namespace esphome

#endif  // USE_ARDUINO

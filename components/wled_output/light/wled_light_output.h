#pragma once

#ifdef USE_ARDUINO

#include "esphome/core/component.h"
#include "esphome/components/network/ip_address.h"
#include <IPAddress.h>
#include "esphome/components/light/addressable_light.h"

#include <vector>
#include <memory>

#ifdef USE_ESP32
#include <WiFi.h>
#endif

#ifdef USE_ESP8266
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#endif

// class UDP;

namespace esphome {
namespace wled {

class WLEDLightOutput : public light::AddressableLight {
 public:
  explicit WLEDLightOutput(int num_leds) {
    this->num_leds_ = num_leds;
    this->leds_ = new Color[num_leds];  // NOLINT
    this->leds_last_state = new Color[num_leds];  // NOLINT
    for (int i = 0; i < num_leds; i++) {
      this->leds_[i] = Color::BLACK;
      this->leds_last_state[i] = Color::WHITE; // init to differ color to change 
    }
    this->effect_data_ = new uint8_t[num_leds];  // NOLINT
  }


  light::LightTraits get_traits() override {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::RGB});
    return traits;
  }

  void setup() override;
  void dump_config() override;
  // void loop() override;
  void write_state(light::LightState *state) override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }

  void clear_effect_data() override {
    for (int i = 0; i < this->size(); i++)
      this->effect_data_[i] = 0;
  }

  void on_shutdown() override;

  inline int32_t size() const override { return num_leds_; }

  void set_address(network::IPAddress address) { this->address_ = address; }
  void set_port(uint16_t port) { this->port_ = port; }


 protected:
  IPAddress address_;
  uint16_t port_{0};
  std::unique_ptr<WiFiUDP> udp_;

  light::ESPColorView get_view_internal(int32_t index) const override {
    return {&this->leds_[index].r,      &this->leds_[index].g, &this->leds_[index].b, nullptr,
            &this->effect_data_[index], &this->correction_};
  }

  Color *leds_{nullptr};
  Color *leds_last_state{nullptr};
  uint8_t *effect_data_{nullptr};
  int num_leds_{0};
  
  uint32_t write_count_{0};
  uint32_t write_duration_{0};
};

}  // namespace wled
}  // namespace esphome

#endif  // USE_ARDUINO

# ESPHome WLED Output

Control external WLED from ESPHome.

> Work in Progress (https://github.com/esphome/feature-requests/issues/1493)

## Install

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/fabio-garavini/esphome-wled-output

```

## Usage

```yaml
light:

  - platform: wled_output
    address: 192.168.1.123
    num_leds: 100
    name: "WLED Output"
    effects:
      - addressable_twinkle:
      - addressable_rainbow:
      - addressable_color_wipe:
```

This will allow controlling a WLED device from a ESPHome node, e.g. apply custom effects.

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.components.network import IPAddress
from esphome.const import (
    CONF_ADDRESS,
    CONF_PORT,
    CONF_OUTPUT_ID,
    CONF_NUM_LEDS,
    CONF_MAX_REFRESH_RATE,
)

wled_ns = cg.esphome_ns.namespace("wled")
WLEDLightOutput = wled_ns.class_("WLEDLightOutput", light.AddressableLight)

CONFIG_SCHEMA = cv.All(
    light.ADDRESSABLE_LIGHT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(WLEDLightOutput),
            cv.Required(CONF_NUM_LEDS): cv.positive_not_null_int,
            cv.Optional(CONF_MAX_REFRESH_RATE): cv.positive_time_period_microseconds,
            cv.Required(CONF_ADDRESS): cv.ipv4,
            cv.Optional(CONF_PORT, 21324): cv.positive_not_null_int,
        }
    ).extend(cv.COMPONENT_SCHEMA)
    , cv.only_with_arduino)


async def to_code(config):

    var = cg.new_Pvariable(config[CONF_OUTPUT_ID], config[CONF_NUM_LEDS])
    await cg.register_component(var, config)

    if CONF_MAX_REFRESH_RATE in config:
        cg.add(var.set_max_refresh_rate(config[CONF_MAX_REFRESH_RATE]))

    await light.register_light(var, config)
    
    #cg.add(var.add_leds(config[CONF_NUM_LEDS]))

    cg.add(var.set_port(config[CONF_PORT]))
    cg.add(var.set_address(IPAddress(*config[CONF_ADDRESS].args)));

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate, sensor, switch, text_sensor
from esphome.const import CONF_ID
from esphome import pins

DEPENDENCIES = ['climate', 'sensor', 'switch', 'text_sensor']
AUTO_LOAD = []

CONF_CLIMATE = 'climate'
CONF_POWER = 'power'
CONF_FILTER = 'filter'
CONF_BUBBLE = 'bubble'
CONF_WATER_TEMPERATURE = 'water_temperature'
CONF_ERROR_TEXT = 'error_text'
CONF_DATA_PIN = "data_pin"
CONF_CLOCK_PIN = "clock_pin"
CONF_ENABLE_PIN = "enable_pin"

sbh_ns = cg.esphome_ns.namespace('sbh20')
IntexSBH20 = sbh_ns.class_('IntexSBH20', cg.PollingComponent)
SBHClimate = sbh_ns.class_('SBHClimate', climate.Climate)
SBHSwitch = sbh_ns.class_('SBHSwitch', switch.Switch)

CONFIG_SCHEMA = cv.polling_component_schema("5s").extend(
    {
        cv.GenerateID(): cv.declare_id(IntexSBH20),
        cv.Optional(CONF_CLIMATE): climate.climate_schema(SBHClimate).extend(),
        cv.Optional(CONF_POWER): switch.switch_schema(SBHSwitch).extend(),
        cv.Optional(CONF_FILTER): switch.switch_schema(SBHSwitch).extend(),
        cv.Optional(CONF_BUBBLE): switch.switch_schema(SBHSwitch).extend(),
        cv.Optional(CONF_WATER_TEMPERATURE): sensor.sensor_schema().extend(),
        cv.Optional(CONF_ERROR_TEXT): text_sensor.text_sensor_schema().extend(),
        cv.Required(CONF_DATA_PIN): pins.gpio_pin_schema({
            "mode": {"input": True, "output": True}
        }),
        cv.Required(CONF_CLOCK_PIN): pins.gpio_pin_schema({
            "mode": {"input": True}
        }),
        cv.Required(CONF_ENABLE_PIN): pins.gpio_pin_schema({
            "mode": {"input": True}
        }),
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    data_pin = await cg.gpio_pin_expression(config[CONF_DATA_PIN])
    cg.add(var.set_data_pin(data_pin))

    clock_pin = await cg.gpio_pin_expression(config[CONF_CLOCK_PIN])
    cg.add(var.set_clock_pin(clock_pin))

    enable_pin = await cg.gpio_pin_expression(config[CONF_ENABLE_PIN])
    cg.add(var.set_enable_pin(enable_pin))

    if CONF_CLIMATE in config:
        clim = await climate.new_climate(config[CONF_CLIMATE])
        cg.add(var.set_climate(clim))

    if CONF_POWER in config:
        sw = await switch.new_switch(config[CONF_POWER])
        cg.add(sw.set_type(CONF_POWER))
        cg.add(var.set_switch_power(sw))

    if CONF_FILTER in config:
        sw = await switch.new_switch(config[CONF_FILTER])
        cg.add(sw.set_type(CONF_FILTER))
        cg.add(var.set_switch_filter(sw))

    if CONF_BUBBLE in config:
        sw = await switch.new_switch(config[CONF_BUBBLE])
        cg.add(sw.set_type(CONF_BUBBLE))
        cg.add(var.set_switch_bubble(sw))

    if CONF_ERROR_TEXT in config:
        tx = await text_sensor.new_text_sensor(config[CONF_ERROR_TEXT])
        cg.add(var.set_error_text_sensor(tx))

    if CONF_WATER_TEMPERATURE in config:
        tx = await sensor.new_sensor(config[CONF_WATER_TEMPERATURE])
        cg.add(var.set_water_temperature_sensor(tx))

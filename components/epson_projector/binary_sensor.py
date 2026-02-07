import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import DEVICE_CLASS_POWER, ENTITY_CATEGORY_DIAGNOSTIC

from . import epson_projector_ns
from .const import CONF_MUTE, CONF_POWER_STATE, ICON_MUTE, ICON_PROJECTOR
from .platform_helpers import get_projector_parent, projector_platform_schema

DEPENDENCIES = ["epson_projector"]

EpsonBinarySensor = epson_projector_ns.class_("EpsonBinarySensor", binary_sensor.BinarySensor, cg.Component)
BinarySensorType = epson_projector_ns.enum("BinarySensorType")

SENSOR_TYPES = {
    CONF_POWER_STATE: BinarySensorType.POWER_STATE,
    CONF_MUTE: BinarySensorType.MUTE_STATE,
}

CONFIG_SCHEMA = projector_platform_schema(
    {
        cv.Optional(CONF_POWER_STATE): binary_sensor.binary_sensor_schema(
            EpsonBinarySensor,
            device_class=DEVICE_CLASS_POWER,
            icon=ICON_PROJECTOR,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_MUTE): binary_sensor.binary_sensor_schema(
            EpsonBinarySensor,
            icon=ICON_MUTE,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
    }
)


async def to_code(config):
    parent = await get_projector_parent(config)

    for key, sensor_type in SENSOR_TYPES.items():
        if conf := config.get(key):
            sens = await binary_sensor.new_binary_sensor(conf)
            cg.add(sens.set_parent(parent))
            cg.add(sens.set_sensor_type(sensor_type))

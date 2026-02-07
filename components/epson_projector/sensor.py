import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    DEVICE_CLASS_DURATION,
    ENTITY_CATEGORY_DIAGNOSTIC,
    STATE_CLASS_TOTAL_INCREASING,
    UNIT_HOUR,
)

from . import epson_projector_ns
from .const import CONF_ERROR_CODE, CONF_LAMP_HOURS, ICON_ERROR, ICON_LAMP
from .platform_helpers import get_projector_parent, projector_platform_schema

DEPENDENCIES = ["epson_projector"]

EpsonSensor = epson_projector_ns.class_("EpsonSensor", sensor.Sensor, cg.Component)
SensorType = epson_projector_ns.enum("SensorType")

SENSOR_TYPES = {
    CONF_LAMP_HOURS: SensorType.LAMP_HOURS,
    CONF_ERROR_CODE: SensorType.ERROR_CODE,
}

CONFIG_SCHEMA = projector_platform_schema(
    {
        cv.Optional(CONF_LAMP_HOURS): sensor.sensor_schema(
            EpsonSensor,
            unit_of_measurement=UNIT_HOUR,
            icon=ICON_LAMP,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_DURATION,
            state_class=STATE_CLASS_TOTAL_INCREASING,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_ERROR_CODE): sensor.sensor_schema(
            EpsonSensor,
            icon=ICON_ERROR,
            accuracy_decimals=0,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
    }
)


async def to_code(config):
    parent = await get_projector_parent(config)

    for key, sensor_type in SENSOR_TYPES.items():
        if conf := config.get(key):
            sens = await sensor.new_sensor(conf)
            cg.add(sens.set_parent(parent))
            cg.add(sens.set_sensor_type(sensor_type))

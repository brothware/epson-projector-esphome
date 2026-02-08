import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import ENTITY_CATEGORY_DIAGNOSTIC

from . import epson_projector_ns
from .const import CONF_SERIAL_NUMBER, ICON_SERIAL_NUMBER
from .platform_helpers import get_projector_parent, projector_platform_schema

DEPENDENCIES = ["epson_projector"]

EpsonTextSensor = epson_projector_ns.class_("EpsonTextSensor", text_sensor.TextSensor, cg.Component)
TextSensorType = epson_projector_ns.enum("TextSensorType", is_class=True)

TEXT_SENSOR_TYPES = {
    CONF_SERIAL_NUMBER: TextSensorType.SERIAL_NUMBER,
}

CONFIG_SCHEMA = projector_platform_schema(
    {
        cv.Optional(CONF_SERIAL_NUMBER): text_sensor.text_sensor_schema(
            EpsonTextSensor,
            icon=ICON_SERIAL_NUMBER,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
    }
)


async def to_code(config):
    parent = await get_projector_parent(config)

    for key, sensor_type in TEXT_SENSOR_TYPES.items():
        if conf := config.get(key):
            sens = await text_sensor.new_text_sensor(conf)
            await cg.register_component(sens, conf)
            cg.add(sens.set_parent(parent))
            cg.add(sens.set_text_sensor_type(sensor_type))

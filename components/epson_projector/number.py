import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import CONF_MODE, ENTITY_CATEGORY_CONFIG

from . import epson_projector_ns
from .const import (
    BRIGHTNESS_MAX,
    BRIGHTNESS_MIN,
    CONF_BRIGHTNESS,
    CONF_CONTRAST,
    CONF_VOLUME,
    CONTRAST_MAX,
    CONTRAST_MIN,
    ICON_BRIGHTNESS,
    ICON_CONTRAST,
    ICON_VOLUME,
    VOLUME_MAX,
    VOLUME_MIN,
)
from .platform_helpers import get_projector_parent, projector_platform_schema

DEPENDENCIES = ["epson_projector"]

EpsonNumber = epson_projector_ns.class_("EpsonNumber", number.Number, cg.Component)
NumberType = epson_projector_ns.enum("NumberType", is_class=True)


def _number_schema_with_slider(entity_class, icon):
    return number.number_schema(
        entity_class,
        icon=icon,
        entity_category=ENTITY_CATEGORY_CONFIG,
    ).extend(
        {
            cv.Optional(CONF_MODE, default="SLIDER"): cv.enum(number.NUMBER_MODES, upper=True),
        }
    )


NUMBER_TYPES = {
    CONF_BRIGHTNESS: NumberType.BRIGHTNESS,
    CONF_CONTRAST: NumberType.CONTRAST,
    CONF_VOLUME: NumberType.VOLUME,
}

NUMBER_RANGES = {
    CONF_BRIGHTNESS: {"min": BRIGHTNESS_MIN, "max": BRIGHTNESS_MAX},
    CONF_CONTRAST: {"min": CONTRAST_MIN, "max": CONTRAST_MAX},
    CONF_VOLUME: {"min": VOLUME_MIN, "max": VOLUME_MAX},
}

CONFIG_SCHEMA = projector_platform_schema(
    {
        cv.Optional(CONF_BRIGHTNESS): _number_schema_with_slider(EpsonNumber, ICON_BRIGHTNESS),
        cv.Optional(CONF_CONTRAST): _number_schema_with_slider(EpsonNumber, ICON_CONTRAST),
        cv.Optional(CONF_VOLUME): _number_schema_with_slider(EpsonNumber, ICON_VOLUME),
    }
)


async def to_code(config):
    parent = await get_projector_parent(config)

    for key, number_type in NUMBER_TYPES.items():
        if conf := config.get(key):
            ranges = NUMBER_RANGES[key]
            num = await number.new_number(
                conf,
                min_value=ranges["min"],
                max_value=ranges["max"],
                step=1,
            )
            cg.add(num.set_parent(parent))
            cg.add(num.set_number_type(number_type))

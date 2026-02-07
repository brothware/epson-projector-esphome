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
NumberType = epson_projector_ns.enum("NumberType")


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


NUMBER_CONFIGS = {
    CONF_BRIGHTNESS: {
        "type": NumberType.BRIGHTNESS,
        "min": BRIGHTNESS_MIN,
        "max": BRIGHTNESS_MAX,
    },
    CONF_CONTRAST: {
        "type": NumberType.CONTRAST,
        "min": CONTRAST_MIN,
        "max": CONTRAST_MAX,
    },
    CONF_VOLUME: {
        "type": NumberType.VOLUME,
        "min": VOLUME_MIN,
        "max": VOLUME_MAX,
    },
}

CONFIG_SCHEMA = projector_platform_schema(
    {
        CONF_BRIGHTNESS: _number_schema_with_slider(EpsonNumber, ICON_BRIGHTNESS),
        CONF_CONTRAST: _number_schema_with_slider(EpsonNumber, ICON_CONTRAST),
        CONF_VOLUME: _number_schema_with_slider(EpsonNumber, ICON_VOLUME),
    }
)


async def to_code(config):
    parent = await get_projector_parent(config)

    for key, number_config in NUMBER_CONFIGS.items():
        if conf := config.get(key):
            num = await number.new_number(
                conf,
                min_value=number_config["min"],
                max_value=number_config["max"],
                step=1,
            )
            cg.add(num.set_parent(parent))
            cg.add(num.set_number_type(number_config["type"]))

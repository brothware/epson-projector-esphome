import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import CONF_MODE, ENTITY_CATEGORY_CONFIG

from . import epson_projector_ns
from .const import (
    BRIGHTNESS_MAX,
    BRIGHTNESS_MIN,
    COLOR_TEMPERATURE_MAX,
    COLOR_TEMPERATURE_MIN,
    CONF_BRIGHTNESS,
    CONF_COLOR_TEMPERATURE,
    CONF_CONTRAST,
    CONF_DENSITY,
    CONF_SHARPNESS,
    CONF_TINT,
    CONF_VOLUME,
    CONTRAST_MAX,
    CONTRAST_MIN,
    DENSITY_MAX,
    DENSITY_MIN,
    ICON_BRIGHTNESS,
    ICON_COLOR_TEMPERATURE,
    ICON_CONTRAST,
    ICON_DENSITY,
    ICON_SHARPNESS,
    ICON_TINT,
    ICON_VOLUME,
    SHARPNESS_MAX,
    SHARPNESS_MIN,
    TINT_MAX,
    TINT_MIN,
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
    CONF_SHARPNESS: NumberType.SHARPNESS,
    CONF_DENSITY: NumberType.DENSITY,
    CONF_TINT: NumberType.TINT,
    CONF_COLOR_TEMPERATURE: NumberType.COLOR_TEMPERATURE,
}

NUMBER_RANGES = {
    CONF_BRIGHTNESS: {"min": BRIGHTNESS_MIN, "max": BRIGHTNESS_MAX},
    CONF_CONTRAST: {"min": CONTRAST_MIN, "max": CONTRAST_MAX},
    CONF_VOLUME: {"min": VOLUME_MIN, "max": VOLUME_MAX},
    CONF_SHARPNESS: {"min": SHARPNESS_MIN, "max": SHARPNESS_MAX},
    CONF_DENSITY: {"min": DENSITY_MIN, "max": DENSITY_MAX},
    CONF_TINT: {"min": TINT_MIN, "max": TINT_MAX},
    CONF_COLOR_TEMPERATURE: {"min": COLOR_TEMPERATURE_MIN, "max": COLOR_TEMPERATURE_MAX},
}

CONFIG_SCHEMA = projector_platform_schema(
    {
        cv.Optional(CONF_BRIGHTNESS): _number_schema_with_slider(EpsonNumber, ICON_BRIGHTNESS),
        cv.Optional(CONF_CONTRAST): _number_schema_with_slider(EpsonNumber, ICON_CONTRAST),
        cv.Optional(CONF_VOLUME): _number_schema_with_slider(EpsonNumber, ICON_VOLUME),
        cv.Optional(CONF_SHARPNESS): _number_schema_with_slider(EpsonNumber, ICON_SHARPNESS),
        cv.Optional(CONF_DENSITY): _number_schema_with_slider(EpsonNumber, ICON_DENSITY),
        cv.Optional(CONF_TINT): _number_schema_with_slider(EpsonNumber, ICON_TINT),
        cv.Optional(CONF_COLOR_TEMPERATURE): _number_schema_with_slider(EpsonNumber, ICON_COLOR_TEMPERATURE),
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
            await cg.register_component(num, conf)
            cg.add(num.set_parent(parent))
            cg.add(num.set_number_type(number_type))

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import DEVICE_CLASS_SWITCH, ENTITY_CATEGORY_CONFIG

from . import _filter_platform_sources, epson_projector_ns
from .const import (
    CONF_FREEZE,
    CONF_H_REVERSE,
    CONF_MUTE,
    CONF_POWER,
    CONF_V_REVERSE,
    ICON_FREEZE,
    ICON_H_REVERSE,
    ICON_MUTE,
    ICON_PROJECTOR,
    ICON_V_REVERSE,
)
from .platform_helpers import get_projector_parent, projector_platform_schema

DEPENDENCIES = ["epson_projector"]
FILTER_SOURCE_FILES = _filter_platform_sources

EpsonSwitch = epson_projector_ns.class_("EpsonSwitch", switch.Switch, cg.Component)
SwitchType = epson_projector_ns.enum("SwitchType", is_class=True)

SWITCH_TYPES = {
    CONF_POWER: SwitchType.POWER,
    CONF_MUTE: SwitchType.MUTE,
    CONF_H_REVERSE: SwitchType.H_REVERSE,
    CONF_V_REVERSE: SwitchType.V_REVERSE,
    CONF_FREEZE: SwitchType.FREEZE,
}

CONFIG_SCHEMA = projector_platform_schema(
    {
        cv.Optional(CONF_POWER): switch.switch_schema(
            EpsonSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            icon=ICON_PROJECTOR,
        ),
        cv.Optional(CONF_MUTE): switch.switch_schema(
            EpsonSwitch,
            icon=ICON_MUTE,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_H_REVERSE): switch.switch_schema(
            EpsonSwitch,
            icon=ICON_H_REVERSE,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_V_REVERSE): switch.switch_schema(
            EpsonSwitch,
            icon=ICON_V_REVERSE,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_FREEZE): switch.switch_schema(
            EpsonSwitch,
            icon=ICON_FREEZE,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
    }
)


async def to_code(config):
    parent = await get_projector_parent(config)

    for key, switch_type in SWITCH_TYPES.items():
        if conf := config.get(key):
            sw = await switch.new_switch(conf)
            await cg.register_component(sw, conf)
            cg.add(sw.set_parent(parent))
            cg.add(sw.set_switch_type(switch_type))

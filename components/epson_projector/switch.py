import esphome.codegen as cg
from esphome.components import switch
from esphome.const import DEVICE_CLASS_SWITCH, ENTITY_CATEGORY_CONFIG

from . import epson_projector_ns
from .const import CONF_MUTE, CONF_POWER, ICON_MUTE, ICON_PROJECTOR
from .platform_helpers import get_projector_parent, projector_platform_schema

DEPENDENCIES = ["epson_projector"]

EpsonSwitch = epson_projector_ns.class_("EpsonSwitch", switch.Switch, cg.Component)
SwitchType = epson_projector_ns.enum("SwitchType")

SWITCH_TYPES = {
    CONF_POWER: SwitchType.POWER,
    CONF_MUTE: SwitchType.MUTE,
}

CONFIG_SCHEMA = projector_platform_schema(
    {
        CONF_POWER: switch.switch_schema(
            EpsonSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            icon=ICON_PROJECTOR,
        ),
        CONF_MUTE: switch.switch_schema(
            EpsonSwitch,
            icon=ICON_MUTE,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
    }
)


async def to_code(config):
    parent = await get_projector_parent(config)

    for key, switch_type in SWITCH_TYPES.items():
        if conf := config.get(key):
            sw = await switch.new_switch(conf)
            cg.add(sw.set_parent(parent))
            cg.add(sw.set_switch_type(switch_type))

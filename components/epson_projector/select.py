import esphome.codegen as cg
from esphome.components import select
from esphome.const import ENTITY_CATEGORY_CONFIG

from . import epson_projector_ns
from .const import (
    CONF_ASPECT_RATIO,
    CONF_COLOR_MODE,
    CONF_SOURCE,
    ICON_ASPECT,
    ICON_COLOR_MODE,
    ICON_SOURCE,
)
from .platform_helpers import get_projector_parent, projector_platform_schema

DEPENDENCIES = ["epson_projector"]

EpsonSelect = epson_projector_ns.class_("EpsonSelect", select.Select, cg.Component)
SelectType = epson_projector_ns.enum("SelectType")

SELECT_TYPES = {
    CONF_SOURCE: SelectType.SOURCE,
    CONF_COLOR_MODE: SelectType.COLOR_MODE,
    CONF_ASPECT_RATIO: SelectType.ASPECT_RATIO,
}

CONFIG_SCHEMA = projector_platform_schema(
    {
        CONF_SOURCE: select.select_schema(
            EpsonSelect,
            icon=ICON_SOURCE,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        CONF_COLOR_MODE: select.select_schema(
            EpsonSelect,
            icon=ICON_COLOR_MODE,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        CONF_ASPECT_RATIO: select.select_schema(
            EpsonSelect,
            icon=ICON_ASPECT,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
    }
)


async def to_code(config):
    parent = await get_projector_parent(config)

    for key, select_type in SELECT_TYPES.items():
        if conf := config.get(key):
            sel = await select.new_select(conf, options=[])
            cg.add(sel.set_parent(parent))
            cg.add(sel.set_select_type(select_type))

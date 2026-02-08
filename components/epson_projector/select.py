import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import ENTITY_CATEGORY_CONFIG
from esphome.core import CORE

from . import epson_projector_ns
from .const import (
    CONF_ASPECT_RATIO,
    CONF_COLOR_MODE,
    CONF_MODEL,
    CONF_SOURCE,
    ICON_ASPECT,
    ICON_COLOR_MODE,
    ICON_SOURCE,
)
from .models import get_sources_for_model
from .platform_helpers import get_projector_parent, projector_platform_schema

DEPENDENCIES = ["epson_projector"]

EpsonSelect = epson_projector_ns.class_("EpsonSelect", select.Select, cg.Component)
SelectType = epson_projector_ns.enum("SelectType", is_class=True)

SELECT_TYPES = {
    CONF_SOURCE: SelectType.SOURCE,
    CONF_COLOR_MODE: SelectType.COLOR_MODE,
    CONF_ASPECT_RATIO: SelectType.ASPECT_RATIO,
}

CONFIG_SCHEMA = projector_platform_schema(
    {
        cv.Optional(CONF_SOURCE): select.select_schema(
            EpsonSelect,
            icon=ICON_SOURCE,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_COLOR_MODE): select.select_schema(
            EpsonSelect,
            icon=ICON_COLOR_MODE,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_ASPECT_RATIO): select.select_schema(
            EpsonSelect,
            icon=ICON_ASPECT,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
    }
)


async def to_code(config):
    parent = await get_projector_parent(config)
    parent_config = CORE.config.get("epson_projector", {})
    model_id = parent_config.get(CONF_MODEL, "generic")
    sources = get_sources_for_model(model_id)

    for key, select_type in SELECT_TYPES.items():
        if conf := config.get(key):
            sel = await select.new_select(conf, options=[])
            await cg.register_component(sel, conf)
            cg.add(sel.set_parent(parent))
            cg.add(sel.set_select_type(select_type))

            if key == CONF_SOURCE and sources:
                for name, code in sources.items():
                    cg.add(sel.add_option(name, code))

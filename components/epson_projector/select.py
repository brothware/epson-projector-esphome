import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import ENTITY_CATEGORY_CONFIG
from esphome.core import CORE

from . import epson_projector_ns
from .const import (
    CONF_ASPECT_RATIO,
    CONF_COLOR_MODE,
    CONF_GAMMA,
    CONF_LUMINANCE,
    CONF_MODEL,
    CONF_SOURCE,
    ICON_ASPECT,
    ICON_COLOR_MODE,
    ICON_GAMMA,
    ICON_LUMINANCE,
    ICON_SOURCE,
)
from .models import (
    get_aspect_ratios_for_model,
    get_color_modes_for_model,
    get_gamma_options_for_model,
    get_luminance_options_for_model,
    get_sources_for_model,
)
from .platform_helpers import get_projector_parent, projector_platform_schema

DEPENDENCIES = ["epson_projector"]

EpsonSelect = epson_projector_ns.class_("EpsonSelect", select.Select, cg.Component)
SelectType = epson_projector_ns.enum("SelectType", is_class=True)

SELECT_TYPES = {
    CONF_SOURCE: SelectType.SOURCE,
    CONF_COLOR_MODE: SelectType.COLOR_MODE,
    CONF_ASPECT_RATIO: SelectType.ASPECT_RATIO,
    CONF_LUMINANCE: SelectType.LUMINANCE,
    CONF_GAMMA: SelectType.GAMMA,
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
        cv.Optional(CONF_LUMINANCE): select.select_schema(
            EpsonSelect,
            icon=ICON_LUMINANCE,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_GAMMA): select.select_schema(
            EpsonSelect,
            icon=ICON_GAMMA,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
    }
)


async def to_code(config):
    parent = await get_projector_parent(config)
    parent_config = CORE.config.get("epson_projector", {})
    model_id = parent_config.get(CONF_MODEL, "generic")

    options_map = {
        CONF_SOURCE: get_sources_for_model(model_id),
        CONF_COLOR_MODE: get_color_modes_for_model(model_id),
        CONF_ASPECT_RATIO: get_aspect_ratios_for_model(model_id),
        CONF_LUMINANCE: get_luminance_options_for_model(model_id),
        CONF_GAMMA: get_gamma_options_for_model(model_id),
    }

    for key, select_type in SELECT_TYPES.items():
        if conf := config.get(key):
            sel = await select.new_select(conf, options=[])
            await cg.register_component(sel, conf)
            cg.add(sel.set_parent(parent))
            cg.add(sel.set_select_type(select_type))

            options = options_map.get(key, {})
            for name, code in options.items():
                cg.add(sel.add_option(name, code))

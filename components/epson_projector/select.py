import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import CONF_ID, ENTITY_CATEGORY_CONFIG

from . import EpsonProjector, epson_projector_ns
from .const import (
    CONF_ASPECT_RATIO,
    CONF_COLOR_MODE,
    CONF_PROJECTOR_ID,
    CONF_SOURCE,
    ICON_ASPECT,
    ICON_COLOR_MODE,
    ICON_SOURCE,
)
from .models import get_model, get_sources_for_model

DEPENDENCIES = ["epson_projector"]

EpsonSelect = epson_projector_ns.class_("EpsonSelect", select.Select, cg.Component)
SelectType = epson_projector_ns.enum("SelectType")

SELECT_TYPES = {
    CONF_SOURCE: SelectType.SOURCE,
    CONF_COLOR_MODE: SelectType.COLOR_MODE,
    CONF_ASPECT_RATIO: SelectType.ASPECT_RATIO,
}

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_PROJECTOR_ID): cv.use_id(EpsonProjector),
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


def get_parent_model(parent_config):
    from . import CONF_MODEL

    return parent_config.get(CONF_MODEL, "generic")


async def to_code(config):
    parent = await cg.get_variable(config[CONF_PROJECTOR_ID])

    if conf := config.get(CONF_SOURCE):
        sel = await select.new_select(conf, options=[])
        cg.add(sel.set_parent(parent))
        cg.add(sel.set_select_type(SelectType.SOURCE))

    if conf := config.get(CONF_COLOR_MODE):
        sel = await select.new_select(conf, options=[])
        cg.add(sel.set_parent(parent))
        cg.add(sel.set_select_type(SelectType.COLOR_MODE))

    if conf := config.get(CONF_ASPECT_RATIO):
        sel = await select.new_select(conf, options=[])
        cg.add(sel.set_parent(parent))
        cg.add(sel.set_select_type(SelectType.ASPECT_RATIO))

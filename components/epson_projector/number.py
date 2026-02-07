import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import CONF_ID, CONF_MODE, ENTITY_CATEGORY_CONFIG

from . import EpsonProjector, epson_projector_ns
from .const import (
    CONF_BRIGHTNESS,
    CONF_CONTRAST,
    CONF_PROJECTOR_ID,
    CONF_VOLUME,
    ICON_BRIGHTNESS,
    ICON_CONTRAST,
    ICON_VOLUME,
)

DEPENDENCIES = ["epson_projector"]

EpsonNumber = epson_projector_ns.class_("EpsonNumber", number.Number, cg.Component)
NumberType = epson_projector_ns.enum("NumberType")

NUMBER_TYPES = {
    CONF_BRIGHTNESS: NumberType.BRIGHTNESS,
    CONF_CONTRAST: NumberType.CONTRAST,
    CONF_VOLUME: NumberType.VOLUME,
}

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_PROJECTOR_ID): cv.use_id(EpsonProjector),
        cv.Optional(CONF_BRIGHTNESS): number.number_schema(
            EpsonNumber,
            icon=ICON_BRIGHTNESS,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(
            {
                cv.Optional(CONF_MODE, default="SLIDER"): cv.enum(
                    number.NUMBER_MODES, upper=True
                ),
            }
        ),
        cv.Optional(CONF_CONTRAST): number.number_schema(
            EpsonNumber,
            icon=ICON_CONTRAST,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(
            {
                cv.Optional(CONF_MODE, default="SLIDER"): cv.enum(
                    number.NUMBER_MODES, upper=True
                ),
            }
        ),
        cv.Optional(CONF_VOLUME): number.number_schema(
            EpsonNumber,
            icon=ICON_VOLUME,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(
            {
                cv.Optional(CONF_MODE, default="SLIDER"): cv.enum(
                    number.NUMBER_MODES, upper=True
                ),
            }
        ),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_PROJECTOR_ID])

    if conf := config.get(CONF_BRIGHTNESS):
        num = await number.new_number(
            conf, min_value=0, max_value=255, step=1
        )
        cg.add(num.set_parent(parent))
        cg.add(num.set_number_type(NumberType.BRIGHTNESS))

    if conf := config.get(CONF_CONTRAST):
        num = await number.new_number(
            conf, min_value=-32, max_value=32, step=1
        )
        cg.add(num.set_parent(parent))
        cg.add(num.set_number_type(NumberType.CONTRAST))

    if conf := config.get(CONF_VOLUME):
        num = await number.new_number(
            conf, min_value=0, max_value=20, step=1
        )
        cg.add(num.set_parent(parent))
        cg.add(num.set_number_type(NumberType.VOLUME))

import esphome.codegen as cg
import esphome.config_validation as cv

from . import EpsonProjector
from .const import CONF_PROJECTOR_ID


def projector_platform_schema(schema_dict: dict) -> cv.Schema:
    return cv.Schema(
        {
            cv.GenerateID(CONF_PROJECTOR_ID): cv.use_id(EpsonProjector),
            **schema_dict,
        }
    )


async def get_projector_parent(config):
    return await cg.get_variable(config[CONF_PROJECTOR_ID])

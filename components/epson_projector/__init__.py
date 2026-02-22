import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID, CONF_UPDATE_INTERVAL
from esphome.core import CORE

from .const import CONF_MODEL
from .models import get_model_names

CODEOWNERS = ["@brothware"]
DEPENDENCIES = ["uart"]

epson_projector_ns = cg.esphome_ns.namespace("epson_projector")
EpsonProjector = epson_projector_ns.class_("EpsonProjector", uart.UARTDevice, cg.PollingComponent)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(EpsonProjector),
            cv.Required(CONF_MODEL): cv.one_of(*get_model_names(), lower=True),
            cv.Optional(CONF_UPDATE_INTERVAL, default="5s"): cv.positive_time_period_milliseconds,
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.polling_component_schema("5s"))
)


def _filter_platform_sources() -> list[str]:
    platform_files = {
        "number": ["epson_number.cpp", "epson_number.h"],
        "select": ["epson_select.cpp", "epson_select.h"],
        "sensor": ["epson_sensor.cpp", "epson_sensor.h"],
        "binary_sensor": ["epson_binary_sensor.cpp", "epson_binary_sensor.h"],
        "switch": ["epson_switch.cpp", "epson_switch.h"],
        "text_sensor": ["epson_text_sensor.cpp", "epson_text_sensor.h"],
    }

    excluded = []
    for platform, files in platform_files.items():
        if platform not in CORE.config:
            excluded.extend(files)
    return excluded


FILTER_SOURCE_FILES = _filter_platform_sources


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

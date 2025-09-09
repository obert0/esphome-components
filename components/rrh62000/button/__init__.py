import esphome.codegen as cg
from esphome.components import button
import esphome.config_validation as cv
from esphome.const import (
    ICON_RESTART,
)

from ..sensor import CONF_SENSOR_ID, RRH62000Component, rrh62000_ns

SelfCleaningButton = rrh62000_ns.class_(
    "SelfCleaningButton", button.Button
)

ResetButton = rrh62000_ns.class_(
    "ResetButton", button.Button
)

# Button Function of Sensor
CONF_SELF_CLEANING = "self_cleaning"
CONF_RESET_SENSOR = "reset_sensor"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_SENSOR_ID): cv.use_id(RRH62000Component),
        cv.Optional(CONF_SELF_CLEANING): button.button_schema(
            SelfCleaningButton,
            icon="mdi:vacuum-outline",
        ),
        cv.Optional(CONF_RESET_SENSOR): button.button_schema(
            ResetButton,
            icon=ICON_RESTART,
        ),
    }
)


async def to_code(config):
    for button_type in [CONF_SELF_CLEANING, CONF_RESET_SENSOR]:
        if conf := config.get(button_type):
            btn = await button.new_button(conf)
            await cg.register_parented(btn, config[CONF_SENSOR_ID])

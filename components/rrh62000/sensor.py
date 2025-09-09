from esphome import automation
import esphome.codegen as cg
from esphome.components import i2c, sensor, binary_sensor, button
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    CONF_NAME,
    CONF_ICON,
    CONF_UNIT_OF_MEASUREMENT,
    CONF_ACCURACY_DECIMALS,
    CONF_DEVICE_CLASS,
    CONF_STATE_CLASS,
    CONF_HUMIDITY,    
    CONF_TEMPERATURE,
    CONF_TVOC,
    CONF_ECO2,
    CONF_SPEED, 
    CONF_PM_0_3UM,
    CONF_PM_0_5UM,
    CONF_PM_1_0UM,
    CONF_PM_2_5UM,
    CONF_PMC_1_0,
    CONF_PMC_2_5,
    CONF_PMC_10_0,
    CONF_PM_1_0,
    CONF_PM_2_5,
    CONF_PM_10_0,    
    DEVICE_CLASS_HUMIDITY,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_CARBON_DIOXIDE,
    DEVICE_CLASS_PM1,
    DEVICE_CLASS_PM25,    
    DEVICE_CLASS_PM10,
    DEVICE_CLASS_VOLATILE_ORGANIC_COMPOUNDS,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    UNIT_EMPTY,
    UNIT_PERCENT,
    UNIT_PARTS_PER_MILLION,
    UNIT_MICROGRAMS_PER_CUBIC_METER,
    UNIT_COUNTS_PER_CUBIC_CENTIMETER,
    UNIT_MILLIGRAMS_PER_CUBIC_METER,
    ICON_AIR_FILTER,
)

DEPENDENCIES = ["i2c"]

rrh62000_ns = cg.esphome_ns.namespace("rrh62000")
RRH62000Component = rrh62000_ns.class_(
    "RRH62000Component", cg.PollingComponent, i2c.I2CDevice
)

CONF_SENSOR_ID = "rrh62000_id"

CONF_RELOAD_CONFIG = "reload_config"
CONF_SET_FAN_SPEED = "set_fan_speed"
CONF_MAVE = "set_moving_average"
CONF_TINTC = "set_cleaning_time_int"
CONF_TCLEAN = "set_cleaning_time"
CONF_IAQ = "air_quality_level"
CONF_HIGH_CONCENTRATION = "high_concentration"
CONF_DUST_ACCUMULATION = "dust_accumulation"
CONF_FAN_SPEED_ERR = "fan_speed_error"
CONF_FAN_ERROR = "fan_error"
CONF_FAN_SPEED = "fan_speed"

CONF_PM_4_0UM = "pm_4_0um"

ChangeFanSpeedAction = rrh62000_ns.class_("ChangeFanSpeedAction", automation.Action)
ResetSensorAction = rrh62000_ns.class_("ResetSensorAction", automation.Action)
CleanSensorAction = rrh62000_ns.class_("CleanSensorAction", automation.Action)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(RRH62000Component),
            cv.Optional(CONF_MAVE): cv.int_range(min=0, max=60),            
            cv.Optional(CONF_TINTC): cv.int_range(min=0, max=60480),
            cv.Optional(CONF_TCLEAN): cv.int_range(min=0, max=60),
            cv.Optional(CONF_SET_FAN_SPEED): cv.int_range(min=60, max=100), 
            cv.Optional(CONF_RELOAD_CONFIG): cv.boolean,   
            cv.Optional(
                CONF_HIGH_CONCENTRATION,
                default={ CONF_NAME: "RRH62000 High Concentration",}
                        ): binary_sensor.binary_sensor_schema(
                icon="mdi:hexagon-multiple-outline",                              
            ),       
            cv.Optional(
                CONF_DUST_ACCUMULATION,
                default={ CONF_NAME: "RRH62000 Dust Accumulation",}
                        ): binary_sensor.binary_sensor_schema(
                icon=ICON_AIR_FILTER,                              
            ),     
            cv.Optional(
                CONF_FAN_SPEED_ERR,
                default={ CONF_NAME: "RRH62000 Fan Speed Error",}
                        ): binary_sensor.binary_sensor_schema(
                icon="mdi:fan-alert",            
            ),     
            cv.Optional(
                CONF_FAN_ERROR,
                default={ CONF_NAME: "RRH62000 Fan Error",}
                        ): binary_sensor.binary_sensor_schema(
                icon="mdi:fan-off",                            
            ),    
            cv.Optional(CONF_PM_0_3UM): sensor.sensor_schema(
                unit_of_measurement=UNIT_COUNTS_PER_CUBIC_CENTIMETER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_PM1,
                state_class=STATE_CLASS_MEASUREMENT,   
            ),     
            cv.Optional(CONF_PM_0_5UM): sensor.sensor_schema(
                unit_of_measurement=UNIT_COUNTS_PER_CUBIC_CENTIMETER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_PM1,
                state_class=STATE_CLASS_MEASUREMENT,   
            ),     
            cv.Optional(CONF_PM_1_0UM): sensor.sensor_schema(
                unit_of_measurement=UNIT_COUNTS_PER_CUBIC_CENTIMETER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_PM1,
                state_class=STATE_CLASS_MEASUREMENT,   
            ),        
            cv.Optional(CONF_PM_2_5UM): sensor.sensor_schema(
                unit_of_measurement=UNIT_COUNTS_PER_CUBIC_CENTIMETER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_PM25,
                state_class=STATE_CLASS_MEASUREMENT,   
            ),                
            cv.Optional(CONF_PM_4_0UM): sensor.sensor_schema(
                unit_of_measurement=UNIT_COUNTS_PER_CUBIC_CENTIMETER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_PM10,
                state_class=STATE_CLASS_MEASUREMENT,   
            ),                                                                    
            cv.Optional(CONF_PMC_1_0): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_PM1,
                state_class=STATE_CLASS_MEASUREMENT,   
            ),     
            cv.Optional(CONF_PMC_2_5): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_PM25,
                state_class=STATE_CLASS_MEASUREMENT,   
            ),     
            cv.Optional(CONF_PMC_10_0): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_PM10,
                state_class=STATE_CLASS_MEASUREMENT,   
            ),                                      
            cv.Optional(CONF_PM_1_0): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_PM1,
                state_class=STATE_CLASS_MEASUREMENT,   
            ),     
            cv.Optional(CONF_PM_2_5): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_PM25,
                state_class=STATE_CLASS_MEASUREMENT,   
            ),     
            cv.Optional(CONF_PM_10_0): sensor.sensor_schema(
                unit_of_measurement=UNIT_MICROGRAMS_PER_CUBIC_METER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_PM10,
                state_class=STATE_CLASS_MEASUREMENT,       
            ),                                             
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_HUMIDITY,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_TVOC): sensor.sensor_schema(
                unit_of_measurement=UNIT_MILLIGRAMS_PER_CUBIC_METER,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_VOLATILE_ORGANIC_COMPOUNDS,
                state_class=STATE_CLASS_MEASUREMENT,   
            ),              
            cv.Optional(CONF_ECO2): sensor.sensor_schema(
                unit_of_measurement=UNIT_PARTS_PER_MILLION,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_CARBON_DIOXIDE,
                state_class=STATE_CLASS_MEASUREMENT,   
            ),  
            cv.Optional(CONF_IAQ): sensor.sensor_schema(
                unit_of_measurement=UNIT_EMPTY,
                accuracy_decimals=0,
                state_class=STATE_CLASS_MEASUREMENT,   
            ),  
            cv.Optional(CONF_FAN_SPEED): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                accuracy_decimals=0,
                state_class=STATE_CLASS_MEASUREMENT,   
            ),                                                                                         
        }
    )
    .extend(cv.polling_component_schema("600s"))
    .extend(i2c.i2c_device_schema(0x69))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    # Set config values
    if CONF_SET_FAN_SPEED in config:
        cg.add(var.set_def_fan_speed(config[CONF_SET_FAN_SPEED]))  
    if CONF_MAVE in config:
        cg.add(var.set_def_mave(config[CONF_MAVE]))          
    if CONF_TINTC in config:
        cg.add(var.set_def_tintc(config[CONF_TINTC]))  
    if CONF_TCLEAN in config:
        cg.add(var.set_def_tclean(config[CONF_TCLEAN]))  
    if CONF_RELOAD_CONFIG in config:
        cg.add(var.set_reload_config(config[CONF_RELOAD_CONFIG]))                           

    # Set Status sensors
    sens = await binary_sensor.new_binary_sensor(config[CONF_HIGH_CONCENTRATION])
    cg.add(var.set_hc_binary_sensor(sens))  

    sens = await binary_sensor.new_binary_sensor(config[CONF_DUST_ACCUMULATION])
    cg.add(var.set_da_binary_sensor(sens))  

    sens = await binary_sensor.new_binary_sensor(config[CONF_FAN_SPEED_ERR])
    cg.add(var.set_fse_binary_sensor(sens))  

    sens = await binary_sensor.new_binary_sensor(config[CONF_FAN_ERROR])
    cg.add(var.set_fe_binary_sensor(sens))             

    # Set data sensors
    if CONF_PM_0_3UM in config:
        sens = await sensor.new_sensor(config[CONF_PM_0_3UM])
        cg.add(var.set_nc03_sensor(sens))

    if CONF_PM_0_5UM in config:
        sens = await sensor.new_sensor(config[CONF_PM_0_5UM])
        cg.add(var.set_nc05_sensor(sens))       

    if CONF_PM_1_0UM in config:
        sens = await sensor.new_sensor(config[CONF_PM_1_0UM])
        cg.add(var.set_nc1_sensor(sens))    

    if CONF_PM_2_5UM in config:
        sens = await sensor.new_sensor(config[CONF_PM_2_5UM])
        cg.add(var.set_nc25_sensor(sens))     

    if CONF_PM_4_0UM in config:
        sens = await sensor.new_sensor(config[CONF_PM_4_0UM])
        cg.add(var.set_nc4_sensor(sens))             

    if CONF_PMC_1_0 in config:
        sens = await sensor.new_sensor(config[CONF_PMC_1_0])
        cg.add(var.set_pm1_sensor(sens))

    if CONF_PMC_2_5 in config:
        sens = await sensor.new_sensor(config[CONF_PMC_2_5])
        cg.add(var.set_pm25_sensor(sens))

    if CONF_PMC_10_0 in config:
        sens = await sensor.new_sensor(config[CONF_PMC_10_0])
        cg.add(var.set_pm10_sensor(sens)) 

    if CONF_PM_1_0 in config:
        sens = await sensor.new_sensor(config[CONF_PM_1_0])
        cg.add(var.set_pm1_2_sensor(sens))

    if CONF_PM_2_5 in config:
        sens = await sensor.new_sensor(config[CONF_PM_2_5])
        cg.add(var.set_pm25_2_sensor(sens))

    if CONF_PM_10_0 in config:
        sens = await sensor.new_sensor(config[CONF_PM_10_0])
        cg.add(var.set_pm10_2_sensor(sens)) 

    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(var.set_temp_sensor(sens))

    if CONF_HUMIDITY in config:
        sens = await sensor.new_sensor(config[CONF_HUMIDITY])
        cg.add(var.set_hum_sensor(sens))

    if CONF_TVOC in config:
        sens = await sensor.new_sensor(config[CONF_TVOC])
        cg.add(var.set_tvoc_sensor(sens))

    if CONF_ECO2 in config:
        sens = await sensor.new_sensor(config[CONF_ECO2])
        cg.add(var.set_eco2_sensor(sens))

    if CONF_IAQ in config:
        sens = await sensor.new_sensor(config[CONF_IAQ])
        cg.add(var.set_iaq_sensor(sens))

    if CONF_FAN_SPEED in config:
        sens = await sensor.new_sensor(config[CONF_FAN_SPEED])
        cg.add(var.set_fan_speed_sensor(sens))                

    # Set automations      
@automation.register_action(
    "rrh62000.change_fan_speed",
    ChangeFanSpeedAction,
    cv.maybe_simple_value(
        {
            cv.Required(CONF_ID): cv.use_id(RRH62000Component),
            cv.Required(CONF_SPEED): cv.templatable(cv.int_range(min=60, max=100)),
        },
        key=CONF_SPEED,
    ),
)
async def change_fan_speed_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])

    template_fan_speed = await cg.templatable(config[CONF_SPEED], args, cg.uint16)
    cg.add(var.set_speed(template_fan_speed))
    return var


@automation.register_action(
    "rrh62000.reset_sensor",
    ResetSensorAction,
    cv.Schema(
        {
            cv.Required(CONF_ID): cv.use_id(RRH62000Component),
        },
    ),
)
async def reset_sensor_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])

    return var

@automation.register_action(
    "rrh62000.cleaning_sensor",
    CleanSensorAction,
    cv.Schema(
        {
            cv.Required(CONF_ID): cv.use_id(RRH62000Component),
        },
    ),
)
async def cleaning_sensor_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])

    return var
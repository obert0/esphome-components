# Renesas - RRH62000

For Renesas - RRH62000 
All-in-one Air Quality Module

This component provides support for the [Renesas RRH62000](https://www.renesas.com/en/products/general-parts/rrh62000-all-one-integrated-sensor-module-pm25-rht-tvoc-and-eco2-detection?srsltid=AfmBOorIDZ_KI3Ejx9maQfmJmaeM2oYBP5nFdzEi2rybIwPpisEtvjkM)

You need an `i2c:` component configured.

Example:
```yaml

# For local components
external_components:
  - source:
      type: git
      url: file:///Users/obert0/esphome-components/components
    components: [rrh62000]

# Declare I2C Bus
i2c:
  id: i2c_bus
  sda: GPIO21
  scl: GPIO22
  scan: True

- platform: rrh62000
    id: air_sensor
    # If just one IC2 bus configure it will be choose by default
    # Sensor configuration. See documentation
    # Actual default value
    set_moving_average: 10
    set_cleaning_time_int: 20160
    set_cleaning_time: 10
    set_fan_speed: 86  
    reload_config: true   # Behavior on reset. If true it will reload the config. 
    update_interval: 600s  

    # Optional sensors available. See documentation
    # All sensors are extract anyway    
    pm_0_3um:
      name: "Number concentration of particle size 0.3-10"
    pm_0_5um:
      name: "Number concentration of particle size 0.5-10"
    pm_1_0um:
      name: "Number concentration of particle size 1.0-10"
    pm_2_5um:
      name: "Number concentration of particle size 2.5-10"
    pm_4_0um:
      name: "Number concentration of particle size 4.0-10"                        
    pmc_1_0:
      name: "Mass concentration of particle size 1.0 - Reference to KCl particle"
    pmc_2_5:
      name: "Mass concentration of particle size 2.5 - Reference to KCl particle"
    pmc_10_0:
      name: "Mass concentration of particle size 10.0 - Reference to KCl particle"    
    pm_1_0:
      name: "Mass concentration of particle size 1.0 - Reference to cigarette smoke"
    pm_2_5:
      name: "Mass concentration of particle size 2.5 - Reference to cigarette smoke"
    pm_10_0:
      name: "Mass concentration of particle size 10.0 - Reference to cigarette smoke"               
    temperature:
      name: "Sensor Temperature"
    humidity:
      name: "Sensor Humidity"
    tvoc:
      name: "Total volatile organic compounds (TVOC)"        
    eco2:
      name: "Estimated carbon dioxide (eCO2) level"         
    air_quality_level:
      name: "Indoor Air Quality level according to UBA"  
    fan_speed:
      name: "RRH62000 Fan Speed"     
       
# Button required rrh62000 sensor reference
button:
  - platform: rrh62000
    rrh62000_id: air_sensor
    # Start self cleaning sequence
    self_cleaning:
      name: "Button Cleaning"
    # Reset the sensor. He will boot with the default config value. If "reload_config" is set to true component config will be load again.
    reset_sensor:
      name: "Button Reset"

# Action available
  on_...:
    then:
      # Start self cleaning sequence
      - rrh62000.cleaning_sensor:
          id: air_sensor
      # Reset the sensor. He will boot with the default config value. If "reload_config" is set to true component config will be load again.          
      - rrh62000.reset_sensor:
          id: air_sensor
      # Define the fan speed. Will be redefine after reset.
      - rrh62000.change_fan_speed: 
          id: air_sensor          
          speed: 60        


```

#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/automation.h"

namespace esphome {
namespace rrh62000 {

class RRH62000Component : public PollingComponent, public i2c::I2CDevice {
 public:
  // Declare status binary sensors
  SUB_BINARY_SENSOR(hc)
  SUB_BINARY_SENSOR(da)
  SUB_BINARY_SENSOR(fse)
  SUB_BINARY_SENSOR(fe)

  // Declare data sensors
  SUB_SENSOR(nc03)
  SUB_SENSOR(nc05)
  SUB_SENSOR(nc1)
  SUB_SENSOR(nc25)
  SUB_SENSOR(nc4)
  SUB_SENSOR(pm1)
  SUB_SENSOR(pm25)
  SUB_SENSOR(pm10)
  SUB_SENSOR(pm1_2)
  SUB_SENSOR(pm25_2)  
  SUB_SENSOR(pm10_2)
  SUB_SENSOR(temp)
  SUB_SENSOR(hum)
  SUB_SENSOR(tvoc)
  SUB_SENSOR(eco2)
  SUB_SENSOR(iaq)  
  
   // Declare config value
  void set_def_fan_speed(int def_fan_speed) { def_fan_speed_ = def_fan_speed; } 
  void set_def_mave(int def_mave) { def_mave_ = def_mave; } 
  void set_def_tintc(int def_tintc) { def_tintc_ = def_tintc; } 
  void set_def_tclean(int def_tclean) { def_tclean_ = def_tclean; }  
  void set_reload_config(int reload_config) { reload_config_ = reload_config; } 
  

  // Declare sensor redable states
  SUB_SENSOR(fan_speed)

  /// Setup (reset) the sensor and apply the user defined config.
  void setup() override;
  void dump_config() override;
  /// Update the sensor values.
  void update() override;

  float get_setup_priority() const override;

  /// Internal functions
  //void update_status(bool value, RRH62000Component& value_);
  //void update_sensor(uint8_t add1, double precision, uint8_t add2, char text, RRH62000Component& sensor_);

  void apply_config();

  // Sensor I2C action
  void read_data(uint8_t *raw_data); 
  void wakeup_sensor(); 
  void sleep_sensor(); 
  void cleaning_sensor();
  void reset_sensor();
  void set_mave(uint8_t number);
  void set_tintc(uint16_t number);
  void set_tclean(uint8_t number);
  void get_fan_speed();
  void change_fan_speed(uint8_t fan_speed);   
  void get_cstatus();
  void get_sensor_config();  
  void cleaning_action();
  void reset_action();

  int const sensor_number = 16;
  int const statur_number = 4;

  struct SensorMeta_Struct
  {
    sensor::Sensor* sensor;
    binary_sensor::BinarySensor* binary_sensor;
    float conversion;
    String log_text;
    String units;
    String more_info;
  } SensorMeta[16], BinarySensorMeta[4];

 protected:
  
  // Internal constant
  int loop_pointer_ = 0;
  int max_loop_pointer_ = 16;
  uint8_t def_fan_speed_ = 0xff;
  uint8_t def_mave_ = 0xff;  
  uint16_t def_tintc_ = 0xffff;
  uint8_t def_tclean_ = 0xff;
  bool reload_config_ = true;
  bool is_cleaning_ = false;
  int const sensor_begin = 2;
  int const sensor_byte = 2;

  void create_sensor_meta();

 
};

// --- Class for automation ---
// Set fan speed automation

template<typename... Ts> 
class ChangeFanSpeedAction : public Action<Ts...>, public Parented<RRH62000Component> {
 public:
  TEMPLATABLE_VALUE(uint8_t, speed)

  void play(Ts... x) override {this->parent_->change_fan_speed(this->speed_.value(x...));}
 
};

// Set reset sensor automation

template<typename... Ts> 
class ResetSensorAction : public Action<Ts...>, public Parented<RRH62000Component> {
 public:

 void play(Ts... x) override {this->parent_->reset_action();}
 
};

// Set cleaning sensor automation

template<typename... Ts> 
class CleanSensorAction : public Action<Ts...>, public Parented<RRH62000Component> {
 public:

 void play(Ts... x) override {this->parent_->cleaning_action();}
 
};

}  // namespace rrh62000
}  // namespace esphome
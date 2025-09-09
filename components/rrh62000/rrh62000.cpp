#include "rrh62000.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace rrh62000 {

static const char *const TAG = "RRH62000";

static const uint8_t I2C_TIMEOUT = 85;

static const uint8_t RRH62000_DEFAULT_ADDRESS = 0x69;
static const uint8_t RRH62000_REGISTER_READ = 0x00;
static const uint8_t RRH62000_REGISTER_DATA = 0x40;
static const uint8_t RRH62000_REGISTER_SLEEP= 0x50;
static const uint8_t RRH62000_REGISTER_CLEAN = 0x51;
static const uint8_t RRH62000_REGISTER_RESET = 0x52;
static const uint8_t RRH62000_REGISTER_MAVE = 0x53;
static const uint8_t RRH62000_REGISTER_TINTC_H = 0x5A;
static const uint8_t RRH62000_REGISTER_TINTC_L = 0x5B;
static const uint8_t RRH62000_REGISTER_TCLEAN = 0x5C;
static const uint8_t RRH62000_REGISTER_FANSPEED = 0x63;
static const uint8_t RRH62000_REGISTER_RMOX = 0x71;
static const uint8_t RRH62000_REGISTER_UID = 0x72;
static const uint8_t RRH62000_REGISTER_ARGVER = 0x73;
static const uint8_t RRH62000_REGISTER_CSTATUS = 0x74;
static const uint8_t RRH62000_REGISTER_FWVER = 0x75;

void RRH62000Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up RRH62000...");

  // Reset the sensor to confirm his state
  reset_sensor();

  // Wait for software reset to complete
  delay(20);

  // Apply the config
  apply_config();

  // Define sensor metadata
  create_sensor_meta();

}

void RRH62000Component::apply_config() {
  ESP_LOGCONFIG(TAG, "Apply config requirement to RRH62000.");

  // Apply the config
  if (this->def_fan_speed_ != 0xff){change_fan_speed(this->def_fan_speed_);}
  if (this->def_mave_ != 0xff){set_mave(this->def_mave_);}    
  if (this->def_tintc_ != 0xffff){set_tintc(this->def_tintc_);}    
  if (this->def_tclean_ != 0xff){set_tclean(this->def_tclean_);}   

}

void RRH62000Component::dump_config() {
  // Display sensor config
  ESP_LOGCONFIG(TAG, "RRH62000:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with RRH62000 failed!");
  }
  LOG_UPDATE_INTERVAL(this);
  get_sensor_config();
  LOG_SENSOR("  ", "NC03", this->nc03_sensor_); 
  LOG_SENSOR("  ", "NC05", this->nc05_sensor_);
  LOG_SENSOR("  ", "NC1", this->nc1_sensor_);
  LOG_SENSOR("  ", "NC2.5", this->nc25_sensor_);
  LOG_SENSOR("  ", "NC4", this->nc4_sensor_); 
  LOG_SENSOR("  ", "PM1.0", this->pm1_sensor_);
  LOG_SENSOR("  ", "PM2.5", this->pm25_sensor_);
  LOG_SENSOR("  ", "PM10.0", this->pm10_sensor_);
  LOG_SENSOR("  ", "PM1.0_2", this->pm1_2_sensor_);
  LOG_SENSOR("  ", "PM2.5_2", this->pm25_2_sensor_);
  LOG_SENSOR("  ", "PM10.0_2", this->pm10_2_sensor_);
  LOG_SENSOR("  ", "Temperature", this->temp_sensor_);
  LOG_SENSOR("  ", "Humidity", this->hum_sensor_);
  LOG_SENSOR("  ", "TOVC", this->tvoc_sensor_);
  LOG_SENSOR("  ", "ECO2", this->eco2_sensor_);
  LOG_SENSOR("  ", "IAQ", this->iaq_sensor_);
  LOG_SENSOR("  ", "Fan_Speed", this->fan_speed_sensor_);
}

/*
void RRH62000Component::update_status(bool value, RRH62000Component& value_) {

    // General update module for binary sensor

    if (value_ != nullptr)
      value_->publish_state(value);
    this->status_clear_warning();  

}

void RRH62000Component::update_sensor(uint8_t add1, double precision, uint8_t add2, char text, RRH62000Component& sensor_) {

  // General update module for sensor

   uint16_t raw_real_value = encode_uint16(raw_value[add1], raw_value[add2]);

    float value = raw_real_value;
    value *= precision;

    ESP_LOGD(TAG, "text", value);

    if (sensor_ != nullptr)
      sensor_->publish_state(value);
    this->status_clear_warning();   

}
*/

void RRH62000Component::update() {

  // RRH62000 sensor return all sensors in one query.
  // This fonction get the raw data and decode for each sensor.

  // Wake up the sensor
  // It should be handled differently to let the moving average regen.
  //wakeup_sensor();

  // Check if sensor is cleanning
  if (this->is_cleaning_) {

    // Update cleaning status
    get_cstatus();
    // Check again cleaning state
    if (this->is_cleaning_) { return; }
  }

  // Read the sensor data --> All sensor come in uint16 (2 bytes) from 37 bytes I2C request
  // According to the datasheet rrh62000 readings can take up to 85ms ****** À vérifier *******
  this->set_timeout(I2C_TIMEOUT, [this]() {
    uint8_t raw_value[37];

    // Get the data from the sensor
    this->read_data(raw_value);

    ESP_LOGD(TAG, "Got Raw Value, start to decode.");

    // *** Decode sensor Status From Raw Data ***

    uint8_t raw_status = raw_value[0];
    bool status[4];
    for (int status_pointer = 0; status_pointer < 4; ++status_pointer) {
      // Get the status
      status[status_pointer] = (raw_status & (1 << status_pointer)) != 0;

      // Push the status
      if (this->BinarySensorMeta[status_pointer].binary_sensor != nullptr)
        this->BinarySensorMeta[status_pointer].binary_sensor->publish_state(status[status_pointer]);
      this->status_clear_warning();           
    }

    // Log sensor status
    ESP_LOGD(TAG, "RRH62000 Sensor Status:");      
    ESP_LOGD(TAG, "  - High concentration=%d", status[0]); 
    ESP_LOGD(TAG, "  - Dust Accumulation=%d", status[1]); 
    ESP_LOGD(TAG, "  - Fan Speed=%d", status[2]); 
    ESP_LOGD(TAG, "  - Fan Error=%d", status[3]);        

    // Set the loop pointer
    this->loop_pointer_ += 1;
    if (this->loop_pointer_ > this->max_loop_pointer_)
      this->loop_pointer_ = 0;

    ESP_LOGD(TAG, "The pointer is at: %d", loop_pointer_);      

    float value = 0.0;

    // Loop through all sensors and get the value if it's initialise
    for (int sensor_pointer = 0; sensor_pointer < this->sensor_number; sensor_pointer++) {
    
      if (this->SensorMeta[sensor_pointer].sensor != nullptr) {

        value = encode_uint16(raw_value[this->sensor_begin + this->sensor_byte * sensor_pointer], raw_value[this->sensor_begin + this->sensor_byte * sensor_pointer + 1]);
        value *= this->SensorMeta[sensor_pointer].conversion;

        ESP_LOGD(TAG, "Got %s value = %.1f", this->SensorMeta[sensor_pointer].sensor->get_name().c_str(), value);

        this->SensorMeta[sensor_pointer].sensor->publish_state(value);
      }
      this->status_clear_warning();
    }
    
 
  });

  // Pour test
  get_fan_speed();

  get_cstatus();

  // Sleep the sensor
  // It should be handled differently to let the moving average regen.
  //sleep_sensor();

  }

  void RRH62000Component::create_sensor_meta() {  

    // Define Binairy Sensor Meta
    BinarySensorMeta[0].binary_sensor = this->hc_binary_sensor_;
    BinarySensorMeta[1].binary_sensor = this->da_binary_sensor_;
    BinarySensorMeta[2].binary_sensor = this->fse_binary_sensor_;
    BinarySensorMeta[3].binary_sensor = this->fe_binary_sensor_;

    // Define Sensor Meta
    // *** Decode Number Concentration of Particule Size From Raw Data ***
    // --- 0.3 μm - X μm with reference to KCl particle ---

    // NC0.3 --- Particule size 0.3 μm - 10 μm
    SensorMeta[0].sensor = this->nc03_sensor_;
    SensorMeta[0].conversion = 0.1f;
    SensorMeta[0].log_text = "Mass Concentration of NC0.3";
    SensorMeta[0].units = "cm³";
    SensorMeta[0].more_info = " (with reference to KCI particule)";

    // NC0.5 --- Particule size 0.5 μm - 10 μm
    SensorMeta[1].sensor = this->nc05_sensor_;
    SensorMeta[1].conversion = 0.1f;
    SensorMeta[1].log_text = "Mass Concentration of NC0.5";
    SensorMeta[1].units = "cm³";
    SensorMeta[1].more_info = " (with reference to KCI particule)";  

    // NC1 --- Particule size 1.0 μm - 10 μm
    SensorMeta[2].sensor = this->nc1_sensor_;
    SensorMeta[2].conversion = 0.1f;
    SensorMeta[2].log_text = "Mass Concentration of NC1.0";
    SensorMeta[2].units = "cm³";  
    SensorMeta[2].more_info = " (with reference to KCI particule)";  

    // NC2.5 --- Particule size 2.5 μm - 10 μm
    SensorMeta[3].sensor = this->nc25_sensor_;
    SensorMeta[3].conversion = 0.1f;
    SensorMeta[3].log_text = "Mass Concentration of NC2.5";
    SensorMeta[3].units = "cm³";  
    SensorMeta[3].more_info = " (with reference to KCI particule)";  

    // NC4 --- Particule size 4.0 μm - 10 μm
    SensorMeta[4].sensor = this->nc4_sensor_;
    SensorMeta[4].conversion = 0.1f;
    SensorMeta[4].log_text = "Mass Concentration of NC4.0";
    SensorMeta[4].units = "cm³";  
    SensorMeta[4].more_info = " (with reference to KCI particule)";  

    // *** Decode Mass Concentration of Particule Size From Raw Data ***
    // --- 0.3 μm - X μm with reference to KCl particle ---

    // PM1.0
    SensorMeta[5].sensor = this->pm1_sensor_;
    SensorMeta[5].conversion = 0.1f;
    SensorMeta[5].log_text = "Mass Concentration of PM1.0";
    SensorMeta[5].units = "µg/m³";
    SensorMeta[5].more_info = " (with reference to KCI particule)";  

    // PM2.5
    SensorMeta[6].sensor = this->pm25_sensor_;
    SensorMeta[6].conversion = 0.1f;
    SensorMeta[6].log_text = "Mass Concentration of PM2.5";
    SensorMeta[6].units = "µg/m³";
    SensorMeta[6].more_info = " (with reference to KCI particule)";  

    // PM10.0
    SensorMeta[7].sensor = this->pm10_sensor_;
    SensorMeta[7].conversion = 0.1f;
    SensorMeta[7].log_text = "Mass Concentration of PM10.0";
    SensorMeta[7].units = "µg/m³";
    SensorMeta[7].more_info = " (with reference to KCI particule)";  

    // --- 0.3 μm - X μm with reference to cigarette smoke ---
    // PM1.0 with cigarette smoke reference
    SensorMeta[8].sensor = this->pm1_2_sensor_;
    SensorMeta[8].conversion = 0.1f;
    SensorMeta[8].log_text = "Mass Concentration of PM1.0";
    SensorMeta[8].units = "µg/m³";
    SensorMeta[8].more_info = " (with reference to cigarette smoke)";

    // PM2.5 with cigarette smoke reference
    SensorMeta[9].sensor = this->pm25_2_sensor_;
    SensorMeta[9].conversion = 0.1f;
    SensorMeta[9].log_text = "Mass Concentration of PM2.5";
    SensorMeta[9].units = "µg/m³";
    SensorMeta[9].more_info = " (with reference to cigarette smoke)";

    // PM10.0 with cigarette smoke reference
    SensorMeta[10].sensor = this->pm10_2_sensor_;
    SensorMeta[10].conversion = 0.1f;
    SensorMeta[10].log_text = "Mass Concentration of PM10.0";
    SensorMeta[10].units = "µg/m³";
    SensorMeta[10].more_info = " (with reference to cigarette smoke)";
    
    // *** Decode Temperature From Raw Data ***
    SensorMeta[11].sensor = this->temp_sensor_;
    SensorMeta[11].conversion = 0.01;
    SensorMeta[11].log_text = "Temperature";
    SensorMeta[11].units = "°C";
    SensorMeta[11].more_info = "";

    // *** Decode Humidity From Raw Data ***
    SensorMeta[12].sensor = this->hum_sensor_;
    SensorMeta[12].conversion = 0.01;
    SensorMeta[12].log_text = "Humidity";
    SensorMeta[12].units = "%";
    SensorMeta[12].more_info = "";

    // *** Decode Total Volatile Organic Compounds From Raw Data ***
    SensorMeta[13].sensor = this->tvoc_sensor_;
    SensorMeta[13].conversion = 0.01;
    SensorMeta[13].log_text = "Total Volatile Organic Compounds";
    SensorMeta[13].units = "mg/m³";
    SensorMeta[13].more_info = "";

    // *** Decode Estimated CO2 Level From Raw Data ***
    SensorMeta[14].sensor = this->eco2_sensor_;
    SensorMeta[14].conversion = 1.0;
    SensorMeta[14].log_text = "Estimated CO2 Level";
    SensorMeta[14].units = "ppm";
    SensorMeta[14].more_info = "";

    // *** Decode Indoor Air Quality Level From Raw Data ***
    SensorMeta[15].sensor = this->iaq_sensor_;
    SensorMeta[15].conversion = 0.01;
    SensorMeta[15].log_text = "Indoor Air Quality Level";
    SensorMeta[15].units = "";
    SensorMeta[15].more_info = "";

}

void RRH62000Component::read_data(uint8_t *raw_value) {

    if (this->read_register(RRH62000_REGISTER_READ, raw_value, 37) != i2c::ERROR_OK) {
      this->status_set_warning();
      return; 
    }

    //return raw_value;
}

void RRH62000Component::wakeup_sensor() {
  uint8_t command = 0x80;

  if (this->write_register(RRH62000_REGISTER_SLEEP, &command, 1) != i2c::ERROR_OK) {
    this->status_set_warning();
    return;
  }
}

void RRH62000Component::sleep_sensor() {
  uint8_t command = 0x00;

  if (this->write_register(RRH62000_REGISTER_SLEEP, &command, 1) != i2c::ERROR_OK) {
    this->status_set_warning();
    return;
  }
}

void RRH62000Component::cleaning_sensor() {
  uint8_t command = 0x01;

  if (this->write_register(RRH62000_REGISTER_CLEAN, &command, 1) != i2c::ERROR_OK) {
    this->status_set_warning();
    return;
  }
}

void RRH62000Component::reset_sensor() {
  uint8_t command = 0x81;

  if (this->write_register(RRH62000_REGISTER_RESET, &command, 1) != i2c::ERROR_OK) {
    this->status_set_warning();
    return;
  }
}

void RRH62000Component::set_mave(uint8_t number) {
  if (this->write_register(RRH62000_REGISTER_MAVE, &number, 1) != i2c::ERROR_OK) {
    this->status_set_warning();
    return;
  }
  ESP_LOGD(TAG, "Moving average as been set to =%ds", number);
}

void RRH62000Component::set_tintc(uint16_t number) {

  int real_value = number * 30;
  uint8_t parse_number[]={ (uint8_t)(number >> 8), (uint8_t)number };

  if (this->write_register(RRH62000_REGISTER_TINTC_H, &parse_number[0], 1) != i2c::ERROR_OK) {
    this->status_set_warning();
    return;
  }
  if (this->write_register(RRH62000_REGISTER_TINTC_L, &parse_number[1], 1) != i2c::ERROR_OK) {
    this->status_set_warning();
    return;
  }  
  ESP_LOGD(TAG, "Cleaning interval time as been set to =%ds", real_value);
}

void RRH62000Component::set_tclean(uint8_t number) {
  if (this->write_register(RRH62000_REGISTER_TCLEAN, &number, 1) != i2c::ERROR_OK) {
    this->status_set_warning();
    return;
  }
  ESP_LOGD(TAG, "Moving average as been set to =%ds", number);
}

void RRH62000Component::get_fan_speed() {
  this->set_timeout(I2C_TIMEOUT, [this]() {
    uint8_t raw_fan_value;

    if (this->read_register(RRH62000_REGISTER_FANSPEED, &raw_fan_value, 1) != i2c::ERROR_OK) {
      this->status_set_warning();
      return;
    }

    int fan_speed = raw_fan_value;
    ESP_LOGD(TAG, "Got Fan Speed=%d%%", fan_speed);

    if (this->fan_speed_sensor_ != nullptr) {
      this->fan_speed_sensor_->publish_state(fan_speed);     
    }

  });    
}

void RRH62000Component::change_fan_speed(uint8_t fan_speed) {
  if (this->write_register(RRH62000_REGISTER_FANSPEED, &fan_speed, 1) != i2c::ERROR_OK) {
    this->status_set_warning();
    return;
  }
  ESP_LOGD(TAG, "Fan Speed as been change to =%d%%", fan_speed);
}

void RRH62000Component::get_cstatus() {
  this->set_timeout(85, [this]() {
    uint8_t raw_value;

    if (this->read_register(RRH62000_REGISTER_CSTATUS, &raw_value, 1) != i2c::ERROR_OK) {
      this->status_set_warning();
      return;
    }

    this->is_cleaning_ = (raw_value == 0X00);
    ESP_LOGD(TAG, "Cleaning status is %d", this->is_cleaning_);
      
  });    
}

void RRH62000Component::get_sensor_config() {
  this->set_timeout(85, [this]() {
    
    // Extract all set config
    if (this->def_fan_speed_ != 0xff){ESP_LOGCONFIG(TAG, "  Defined Fan Speed: %d%%", this->def_fan_speed_);}    
    if (this->def_mave_ != 0xff){ESP_LOGCONFIG(TAG, "  Defined Moving Average Time: %ds", this->def_mave_);}    
    if (this->def_tintc_ != 0xffff){ESP_LOGCONFIG(TAG, "  Defined Cleaning Interval Time: %ds", this->def_tintc_);}    
    if (this->def_tclean_ != 0xff){ESP_LOGCONFIG(TAG, "  Defined Auto-Cleaning Time: %ds", this->def_tclean_);}    
    
    // Extract all sensors settings
    uint8_t raw_value;
    int value;

    // *** MOVING AVERAGE ***
    if (this->read_register(RRH62000_REGISTER_MAVE, &raw_value, 1) != i2c::ERROR_OK) {
      this->status_set_warning();
      return;
    }

    value = raw_value;
    ESP_LOGCONFIG(TAG, "  Moving average time is =%ds", raw_value);

    // *** CLEANING INTERVAL TIME ***

    uint8_t split_value[2];

    if (this->read_register(RRH62000_REGISTER_TINTC_H, split_value, 2) != i2c::ERROR_OK) {
      this->status_set_warning();
      return;
    }
    uint16_t combine_value = encode_uint16(split_value[0], split_value[1]);
    long long_value = combine_value * 30;

    ESP_LOGCONFIG(TAG, "  Cleaning interval time is =%ds", long_value);    

    // *** AUTO CLEANING TIME ***
    if (this->read_register(RRH62000_REGISTER_TCLEAN, &raw_value, 1) != i2c::ERROR_OK) {
      this->status_set_warning();
      return;
    }
    value = raw_value;    
    ESP_LOGCONFIG(TAG, "  Auto cleaning time is = %ds", raw_value);  

    // *** FAN SPEED ***
    if (this->read_register(RRH62000_REGISTER_FANSPEED, &raw_value, 1) != i2c::ERROR_OK) {
      this->status_set_warning();
      return;
    }

    value = raw_value;
    ESP_LOGCONFIG(TAG, "  Fan speed is =%d%%", value);  
    //this->fan_speed_sensor_ = value; 

  }); 
}

void RRH62000Component::cleaning_action() {

  cleaning_sensor();
  this->is_cleaning_ = true;
  ESP_LOGD(TAG, "Starting Sensor Cleaning");

}

void RRH62000Component::reset_action() {

  reset_sensor();
  ESP_LOGD(TAG, "Software Reset of the Sensor");
  
  // Wait for software reset to complete
  delay(20);

  // Check if the option reload_config is enabled
  if (this->reload_config_) {
    ESP_LOGD(TAG, "Reloading the config of the sensor");  
    // Apply the config
    apply_config();
  }

  // Display the sensor config
  get_sensor_config();

}

float RRH62000Component::get_setup_priority() const { return setup_priority::DATA; }

}  // namespace rrh62000
}  // namespace esphome
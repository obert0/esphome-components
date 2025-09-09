#include "reset_sensor.h"

namespace esphome {
namespace rrh62000 {

void ResetButton::press_action() { 
    
    this->parent_->reset_action();

}

}  // namespace rrh62000
}  // namespace esphome

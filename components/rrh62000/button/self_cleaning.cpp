#include "self_cleaning.h"

namespace esphome {
namespace rrh62000 {

void SelfCleaningButton::press_action() { 
    
    this->parent_->cleaning_action();

}

}  // namespace rrh62000
}  // namespace esphome

#pragma once

#include "../rrh62000.h"
#include "esphome/core/component.h"
#include "esphome/components/button/button.h"

namespace esphome {
namespace rrh62000 {

// --- Class for Button ---
// Set clean sensor button
class SelfCleaningButton : public button::Button, public Parented<RRH62000Component> {
 public:
 
  SelfCleaningButton() = default;

  void press_action() override;

};

}  // namespace rrh62000
}  // namespace esphome

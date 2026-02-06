#pragma once

#include <string>

namespace custom_lvgl {
// Initialize LVGL selector UI (splash -> selector).
void selector_init(const std::string& title);

// Show the lock screen during autonomous.
void selector_show_lock();

// Show the splash screen (no delay).
void selector_show_splash();

// Show the selector screen (no delay).
void selector_show_selector();

// Returns current selected auton index.
int selector_selected_index();

// Calls the currently selected auton.
void selector_selected_auton_call();
}  // namespace custom_lvgl

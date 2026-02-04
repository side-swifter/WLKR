#include "custom_lvgl_selector.hpp"

#include "main.h"
#include "assets_splash.h"

#include <string>
#include <vector>

namespace custom_lvgl {
namespace {
constexpr int kScreenW = 480;
constexpr int kScreenH = 272;

lv_obj_t* splash_screen = nullptr;
lv_obj_t* selector_screen = nullptr;
lv_obj_t* lock_screen = nullptr;

lv_obj_t* title_label = nullptr;
lv_obj_t* card = nullptr;
lv_obj_t* auton_title_label = nullptr;
lv_obj_t* auton_detail_label = nullptr;

int selected_index = 0;

std::string selector_title = "Grid Walkers";

void set_screen_bg_black(lv_obj_t* scr) {
  lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(scr, lv_color_black(), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
}

std::vector<std::string> split_lines(const std::string& input) {
  std::vector<std::string> out;
  std::string current;
  for (char c : input) {
    if (c == '\n') {
      out.push_back(current);
      current.clear();
    } else {
      current.push_back(c);
    }
  }
  out.push_back(current);
  return out;
}

void update_auton_labels() {
  if (!auton_title_label || !auton_detail_label) return;

  auto& autons = ez::as::auton_selector.Autons;
  if (autons.empty()) {
    lv_label_set_text(auton_title_label, "No autons");
    lv_label_set_text(auton_detail_label, "");
    return;
  }

  if (selected_index < 0) selected_index = static_cast<int>(autons.size()) - 1;
  if (selected_index >= static_cast<int>(autons.size())) selected_index = 0;

  const std::string& name = autons[selected_index].Name;
  std::vector<std::string> lines = split_lines(name);

  std::string main_line;
  std::string detail_lines;
  for (const auto& line : lines) {
    if (main_line.empty() && !line.empty()) {
      main_line = line;
    } else if (!line.empty()) {
      if (!detail_lines.empty()) detail_lines += "\n";
      detail_lines += line;
    }
  }

  if (main_line.empty()) main_line = "Auton";
  lv_label_set_text(auton_title_label, main_line.c_str());
  lv_label_set_text(auton_detail_label, detail_lines.c_str());
}

void arrow_left_cb(lv_event_t* e) {
  if (e->code != LV_EVENT_CLICKED) return;
  selected_index--;
  update_auton_labels();
}

void arrow_right_cb(lv_event_t* e) {
  if (e->code != LV_EVENT_CLICKED) return;
  selected_index++;
  update_auton_labels();
}

void create_splash_screen() {
  splash_screen = lv_obj_create(nullptr);
  set_screen_bg_black(splash_screen);

  lv_obj_t* img = lv_img_create(splash_screen);
  lv_img_set_src(img, &splash_img);
  lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
}

void create_selector_screen() {
  selector_screen = lv_obj_create(nullptr);
  set_screen_bg_black(selector_screen);

  title_label = lv_label_create(selector_screen);
  lv_label_set_text(title_label, selector_title.c_str());
  lv_obj_set_style_text_color(title_label, lv_color_white(), 0);
  lv_obj_set_style_text_font(title_label, &lv_font_montserrat_36, 0);
  lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 12);

  card = lv_obj_create(selector_screen);
  lv_obj_set_size(card, 320, 150);
  lv_obj_align(card, LV_ALIGN_CENTER, 0, 10);
  lv_obj_set_style_bg_color(card, lv_color_black(), 0);
  lv_obj_set_style_bg_opa(card, LV_OPA_COVER, 0);
  lv_obj_set_style_border_color(card, lv_color_white(), 0);
  lv_obj_set_style_border_width(card, 3, 0);
  lv_obj_set_style_radius(card, 18, 0);
  lv_obj_set_style_pad_all(card, 12, 0);

  auton_title_label = lv_label_create(card);
  lv_obj_set_style_text_color(auton_title_label, lv_color_white(), 0);
  lv_obj_set_style_text_font(auton_title_label, &lv_font_montserrat_30, 0);
  lv_obj_set_width(auton_title_label, 296);
  lv_label_set_long_mode(auton_title_label, LV_LABEL_LONG_WRAP);
  lv_obj_align(auton_title_label, LV_ALIGN_TOP_MID, 0, 0);

  auton_detail_label = lv_label_create(card);
  lv_obj_set_style_text_color(auton_detail_label, lv_color_white(), 0);
  lv_obj_set_style_text_font(auton_detail_label, &lv_font_montserrat_20, 0);
  lv_obj_set_width(auton_detail_label, 296);
  lv_label_set_long_mode(auton_detail_label, LV_LABEL_LONG_WRAP);
  lv_obj_align(auton_detail_label, LV_ALIGN_TOP_MID, 0, 50);

  lv_obj_t* left_btn = lv_btn_create(selector_screen);
  lv_obj_set_size(left_btn, 70, 70);
  lv_obj_align(left_btn, LV_ALIGN_LEFT_MID, 8, 10);
  lv_obj_set_style_bg_opa(left_btn, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_width(left_btn, 0, 0);
  lv_obj_add_event_cb(left_btn, arrow_left_cb, LV_EVENT_CLICKED, nullptr);

  lv_obj_t* left_label = lv_label_create(left_btn);
  lv_label_set_text(left_label, LV_SYMBOL_LEFT);
  lv_obj_set_style_text_color(left_label, lv_color_white(), 0);
  lv_obj_set_style_text_font(left_label, &lv_font_montserrat_48, 0);
  lv_obj_center(left_label);

  lv_obj_t* right_btn = lv_btn_create(selector_screen);
  lv_obj_set_size(right_btn, 70, 70);
  lv_obj_align(right_btn, LV_ALIGN_RIGHT_MID, -8, 10);
  lv_obj_set_style_bg_opa(right_btn, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_width(right_btn, 0, 0);
  lv_obj_add_event_cb(right_btn, arrow_right_cb, LV_EVENT_CLICKED, nullptr);

  lv_obj_t* right_label = lv_label_create(right_btn);
  lv_label_set_text(right_label, LV_SYMBOL_RIGHT);
  lv_obj_set_style_text_color(right_label, lv_color_white(), 0);
  lv_obj_set_style_text_font(right_label, &lv_font_montserrat_48, 0);
  lv_obj_center(right_label);

  update_auton_labels();
}

void create_lock_screen() {
  lock_screen = lv_obj_create(nullptr);
  set_screen_bg_black(lock_screen);

  lv_obj_t* lock_title = lv_label_create(lock_screen);
  lv_label_set_text(lock_title, selector_title.c_str());
  lv_obj_set_style_text_color(lock_title, lv_color_white(), 0);
  lv_obj_set_style_text_font(lock_title, &lv_font_montserrat_36, 0);
  lv_obj_align(lock_title, LV_ALIGN_TOP_MID, 0, 12);

  lv_obj_t* lock_text = lv_label_create(lock_screen);
  lv_label_set_text(lock_text, "AUTO LOCKED");
  lv_obj_set_style_text_color(lock_text, lv_color_white(), 0);
  lv_obj_set_style_text_font(lock_text, &lv_font_montserrat_24, 0);
  lv_obj_align(lock_text, LV_ALIGN_LEFT_MID, 30, 10);

  // Lock body
  lv_obj_t* body = lv_obj_create(lock_screen);
  lv_obj_set_size(body, 120, 120);
  lv_obj_align(body, LV_ALIGN_RIGHT_MID, -40, 20);
  lv_obj_set_style_radius(body, 16, 0);
  lv_obj_set_style_bg_color(body, lv_color_white(), 0);
  lv_obj_set_style_bg_opa(body, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(body, 0, 0);

  // Lock shackle
  lv_obj_t* shackle = lv_obj_create(lock_screen);
  lv_obj_set_size(shackle, 90, 70);
  lv_obj_align(shackle, LV_ALIGN_RIGHT_MID, -40, -35);
  lv_obj_set_style_bg_opa(shackle, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_color(shackle, lv_color_white(), 0);
  lv_obj_set_style_border_width(shackle, 10, 0);
  lv_obj_set_style_radius(shackle, 35, 0);
}
}  // namespace

void selector_init(const std::string& title) {
  selector_title = title;

  create_splash_screen();
  lv_scr_load(splash_screen);
  pros::delay(2000);

  create_selector_screen();
  lv_scr_load(selector_screen);
}

void selector_show_lock() {
  if (!lock_screen) create_lock_screen();
  lv_scr_load(lock_screen);
}

int selector_selected_index() { return selected_index; }

void selector_selected_auton_call() {
  auto& autons = ez::as::auton_selector.Autons;
  if (autons.empty()) return;
  if (selected_index < 0) selected_index = static_cast<int>(autons.size()) - 1;
  if (selected_index >= static_cast<int>(autons.size())) selected_index = 0;
  autons[selected_index].auton_call();
}
}  // namespace custom_lvgl

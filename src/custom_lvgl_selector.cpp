#include "custom_lvgl_selector.hpp"

#include "main.h"
#include "assets_splash.h"
#include "assets_grd_wlkr.h"
#include "assets_arrow.h"

#include <string>
#include <vector>

namespace custom_lvgl {
namespace {
constexpr int kScreenW = 480;
constexpr int kScreenH = 272;

lv_obj_t* splash_screen = nullptr;
lv_obj_t* selector_screen = nullptr;
lv_obj_t* lock_screen = nullptr;

lv_obj_t* top_bar = nullptr;
lv_obj_t* top_title = nullptr;
lv_obj_t* top_time = nullptr;
lv_obj_t* top_batt = nullptr;
lv_obj_t* page_label = nullptr;
lv_obj_t* card = nullptr;
lv_obj_t* desc_label = nullptr;
lv_obj_t* auton_lines[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};

int selected_index = 0;

std::string selector_title = "29019X";

void set_screen_bg_black(lv_obj_t* scr) {
  lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(scr, lv_color_make(8, 8, 8), 0);
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
  if (!auton_lines[0]) return;

  auto& autons = ez::as::auton_selector.Autons;
  if (autons.empty()) {
    lv_label_set_text(auton_lines[0], "No autons");
    for (int i = 1; i < 5; i++) lv_label_set_text(auton_lines[i], "");
    return;
  }

  if (selected_index < 0) selected_index = static_cast<int>(autons.size()) - 1;
  if (selected_index >= static_cast<int>(autons.size())) selected_index = 0;

  if (page_label) {
    std::string page_text = "Page " + std::to_string(selected_index + 1);
    lv_label_set_text(page_label, page_text.c_str());
  }

  const std::string& name = autons[selected_index].Name;
  std::vector<std::string> lines = split_lines(name);

  std::vector<std::string> clean;
  for (const auto& line : lines) {
    if (!line.empty()) clean.push_back(line);
  }
  if (clean.empty()) clean.push_back("Auton");

  for (int i = 0; i < 5; i++) {
    if (i < static_cast<int>(clean.size())) {
      lv_label_set_text(auton_lines[i], clean[i].c_str());
    } else {
      lv_label_set_text(auton_lines[i], "");
    }
  }
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
  lv_obj_set_style_bg_color(selector_screen, lv_color_black(), 0);

  // Top bar
  top_bar = lv_obj_create(selector_screen);
  lv_obj_set_size(top_bar, 480, 32);
  lv_obj_align(top_bar, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_style_bg_color(top_bar, lv_color_black(), 0);
  lv_obj_set_style_bg_opa(top_bar, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(top_bar, 0, 0);
  lv_obj_set_style_radius(top_bar, 0, 0);

  top_title = lv_label_create(top_bar);
  lv_label_set_text(top_title, "Grid Walkers 29019X");
  lv_obj_set_style_text_color(top_title, lv_color_white(), 0);
  lv_obj_set_style_text_font(top_title, &lv_font_montserrat_18, 0);
  lv_obj_align(top_title, LV_ALIGN_LEFT_MID, 10, 0);

  top_time = lv_label_create(top_bar);
  lv_label_set_text(top_time, "0:12");
  lv_obj_set_style_text_color(top_time, lv_color_white(), 0);
  lv_obj_set_style_text_font(top_time, &lv_font_montserrat_18, 0);
  lv_obj_align(top_time, LV_ALIGN_TOP_MID, 0, 6);

  top_batt = lv_label_create(top_bar);
  lv_label_set_text(top_batt, "12.6V");
  lv_obj_set_style_text_color(top_batt, lv_color_white(), 0);
  lv_obj_set_style_text_font(top_batt, &lv_font_montserrat_18, 0);
  lv_obj_align(top_batt, LV_ALIGN_RIGHT_MID, -10, 0);

  card = lv_obj_create(selector_screen);
  lv_obj_set_size(card, 440, 150);
  lv_obj_align(card, LV_ALIGN_TOP_MID, 0, 44);
  lv_obj_set_style_bg_color(card, lv_color_black(), 0);
  lv_obj_set_style_bg_opa(card, LV_OPA_COVER, 0);
  lv_obj_set_style_border_color(card, lv_color_white(), 0);
  lv_obj_set_style_border_width(card, 2, 0);
  lv_obj_set_style_radius(card, 10, 0);
  lv_obj_set_style_pad_all(card, 12, 0);
  lv_obj_set_style_shadow_width(card, 0, 0);
  lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_scrollbar_mode(card, LV_SCROLLBAR_MODE_OFF);

  page_label = lv_label_create(card);
  lv_label_set_text(page_label, "Page 1");
  lv_obj_set_style_text_color(page_label, lv_color_white(), 0);
  lv_obj_set_style_text_font(page_label, &lv_font_montserrat_18, 0);
  lv_obj_align(page_label, LV_ALIGN_TOP_LEFT, 6, 0);

  for (int i = 0; i < 5; i++) {
    auton_lines[i] = lv_label_create(card);
    lv_obj_set_style_text_color(auton_lines[i], lv_color_white(), 0);
    lv_obj_set_width(auton_lines[i], 420);
    lv_label_set_long_mode(auton_lines[i], LV_LABEL_LONG_WRAP);
  }
  lv_obj_set_style_text_font(auton_lines[0], &lv_font_montserrat_22, 0);
  lv_obj_set_style_text_font(auton_lines[1], &lv_font_montserrat_18, 0);
  lv_obj_set_style_text_font(auton_lines[2], &lv_font_montserrat_18, 0);
  lv_obj_set_style_text_font(auton_lines[3], &lv_font_montserrat_18, 0);
  lv_obj_set_style_text_font(auton_lines[4], &lv_font_montserrat_18, 0);
  lv_obj_set_style_text_line_space(auton_lines[1], 4, 0);
  lv_obj_set_style_text_line_space(auton_lines[2], 4, 0);
  lv_obj_set_style_text_line_space(auton_lines[3], 4, 0);
  lv_obj_set_style_text_line_space(auton_lines[4], 4, 0);

  lv_obj_align(auton_lines[0], LV_ALIGN_TOP_LEFT, 6, 26);
  lv_obj_align(auton_lines[1], LV_ALIGN_TOP_LEFT, 6, 52);
  lv_obj_align(auton_lines[2], LV_ALIGN_TOP_LEFT, 6, 74);
  lv_obj_align(auton_lines[3], LV_ALIGN_TOP_LEFT, 6, 96);
  lv_obj_align(auton_lines[4], LV_ALIGN_TOP_LEFT, 6, 118);

  lv_obj_t* bottom_bar = lv_obj_create(selector_screen);
  lv_obj_set_size(bottom_bar, 480, 80);
  lv_obj_align(bottom_bar, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_style_bg_color(bottom_bar, lv_color_black(), 0);
  lv_obj_set_style_bg_opa(bottom_bar, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(bottom_bar, 0, 0);
  lv_obj_set_style_radius(bottom_bar, 0, 0);

  lv_obj_t* left_btn = lv_btn_create(bottom_bar);
  lv_obj_set_size(left_btn, 64, 64);
  lv_obj_align(left_btn, LV_ALIGN_LEFT_MID, 24, 0);
  lv_obj_set_style_bg_color(left_btn, lv_color_black(), 0);
  lv_obj_set_style_bg_opa(left_btn, LV_OPA_COVER, 0);
  lv_obj_set_style_border_color(left_btn, lv_color_white(), 0);
  lv_obj_set_style_border_width(left_btn, 2, 0);
  lv_obj_set_style_outline_width(left_btn, 0, 0);
  lv_obj_set_style_shadow_width(left_btn, 0, 0);
  lv_obj_set_style_radius(left_btn, 10, 0);
  lv_obj_add_event_cb(left_btn, arrow_left_cb, LV_EVENT_CLICKED, nullptr);

  lv_obj_t* left_label = lv_label_create(left_btn);
  lv_label_set_text(left_label, LV_SYMBOL_LEFT);
  lv_obj_set_style_text_color(left_label, lv_color_white(), 0);
  lv_obj_set_style_text_font(left_label, &lv_font_montserrat_18, 0);
  lv_obj_center(left_label);

  lv_obj_t* right_btn = lv_btn_create(bottom_bar);
  lv_obj_set_size(right_btn, 64, 64);
  lv_obj_align(right_btn, LV_ALIGN_RIGHT_MID, -24, 0);
  lv_obj_set_style_bg_color(right_btn, lv_color_black(), 0);
  lv_obj_set_style_bg_opa(right_btn, LV_OPA_COVER, 0);
  lv_obj_set_style_border_color(right_btn, lv_color_white(), 0);
  lv_obj_set_style_border_width(right_btn, 2, 0);
  lv_obj_set_style_outline_width(right_btn, 0, 0);
  lv_obj_set_style_shadow_width(right_btn, 0, 0);
  lv_obj_set_style_radius(right_btn, 10, 0);
  lv_obj_add_event_cb(right_btn, arrow_right_cb, LV_EVENT_CLICKED, nullptr);

  lv_obj_t* right_label = lv_label_create(right_btn);
  lv_label_set_text(right_label, LV_SYMBOL_RIGHT);
  lv_obj_set_style_text_color(right_label, lv_color_white(), 0);
  lv_obj_set_style_text_font(right_label, &lv_font_montserrat_18, 0);
  lv_obj_center(right_label);

  update_auton_labels();
}

void create_lock_screen() {
  lock_screen = lv_obj_create(nullptr);
  set_screen_bg_black(lock_screen);

  lv_obj_t* lock_logo = lv_img_create(lock_screen);
  lv_img_set_src(lock_logo, &grd_wlkr_img);
  lv_img_set_zoom(lock_logo, 35);
  lv_obj_align(lock_logo, LV_ALIGN_TOP_MID, 0, -36);

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

#include "mvvm/binding_rule_parser.h"
#include "mvvm/command_binding.h"
#include "mvvm/data_binding.h"
#include "gtest/gtest.h"
#include <string>

using std::string;

TEST(BindingRuleParser, close_window) {
  binding_rule_t* rule = binding_rule_parse("v-command:on_click", "{Save, CloseWindow}");
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->close_window, TRUE);
  ASSERT_EQ(string(cmd->event), string("on_click"));

  object_unref(OBJECT(rule));
}

TEST(BindingRuleParser, close_window_true) {
  binding_rule_t* rule = binding_rule_parse("v-command:on_click", "{Save, CloseWindow=True}");
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->close_window, TRUE);
  ASSERT_EQ(string(cmd->event), string("on_click"));

  object_unref(OBJECT(rule));
}

TEST(BindingRuleParser, close_window_false) {
  binding_rule_t* rule = binding_rule_parse("v-command:on_click", "{Save, CloseWindow=False}");
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->close_window, FALSE);
  ASSERT_EQ(string(cmd->event), string("on_click"));

  object_unref(OBJECT(rule));
}

TEST(BindingRuleParser, update_model) {
  binding_rule_t* rule = binding_rule_parse("v-command:on_click", "{Save, UpdateModel}");
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->update_model, TRUE);
  ASSERT_EQ(string(cmd->event), string("on_click"));

  object_unref(OBJECT(rule));
}

TEST(BindingRuleParser, update_model_true) {
  binding_rule_t* rule = binding_rule_parse("v-command:on_click", "{Save, UpdateModel=True}");
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->update_model, TRUE);
  ASSERT_EQ(string(cmd->event), string("on_click"));

  object_unref(OBJECT(rule));
}

TEST(BindingRuleParser, update_model_false) {
  binding_rule_t* rule = binding_rule_parse("v-command:on_click", "{Save, UpdateModel=False}");
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->update_model, FALSE);
  ASSERT_EQ(string(cmd->event), string("on_click"));

  object_unref(OBJECT(rule));
}

TEST(BindingRuleParser, event_args) {
  binding_rule_t* rule =
      binding_rule_parse("v-command:on_keydown:ctrl_a", "{Save, UpdateModel=False}");
  command_binding_t* cmd = (command_binding_t*)rule;

  ASSERT_EQ(cmd->update_model, FALSE);
  ASSERT_EQ(string(cmd->event), string("on_keydown"));
  ASSERT_EQ(string(cmd->event_args), string("ctrl_a"));

  object_unref(OBJECT(rule));
}

<?php
/**
 * @file
 *  Provide the views scrollable plugin object with default options and form.
 */

/**
  * Implementation of views_plugin_style().
  */
class scrollable_content_plugin_style extends views_plugin_style {

  function options(&$options) {
    $defaults = scrollable_content_fields_init();
    foreach ($defaults as $key => $value) {
      $options[$key] = $value;
    }
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $full_form = scrollable_content_form('view', $this);
    $fieldsets = array();
    $fieldsets[] = $full_form['scrollable_content_settings'];
    $fieldsets[] = $full_form['scrollable_content_theme'];

    foreach ($fieldsets as $fieldset) {
      foreach ($fieldset as $key => $val) {
        if (is_array($val)) {
          $form[$key] = $fieldset[$key];
        }
      }
    }
    return $form;
  }

}
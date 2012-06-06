<?php

/**
 * Form builder; display the realname settings form.
 *
 * @see realname_settings_form_submit()
 */
function realname_settings_form($form, &$form_state) {
  $form['realname_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Realname pattern'),
    '#default_value' => variable_get('realname_pattern', '[user:name-raw]'),
    '#element_validate' => array('token_element_validate'),
    '#token_types' => array('user'),
    '#min_tokens' => 1,
    '#required' => TRUE,
    '#maxlength' => 256,
  );

  // Add the token tree UI.
  $form['token_help'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('user'),
    '#global_types' => FALSE,
  );

  $form['#submit'][] = 'realname_settings_form_submit';
  return system_settings_form($form);
}

/**
 * Form submit handler for the realname settings form.
 *
 * @see realname_settings_form().
 */
function realname_settings_form_submit($form, $form_state) {
  if ($form['realname_pattern']['#default_value'] != $form_state['values']['realname_pattern']) {
    // Only clear the realname cache if the pattern was changed.
    realname_delete_all();
  }
}

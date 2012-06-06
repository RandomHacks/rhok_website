<?php

/**
 * @file
 * Provides CCK integration for fivestar module
 */

/**
 * Implementation of hook_field_info().
 */
function fivestar_field_info() {
  return array(
    'fivestar' => array(
      'label' => t('Fivestar Rating'),
      'description' => t('Store a rating for this piece of content.'),
      'default_widget' => 'stars',
      'default_formatter' => 'default',
    ),
  );
}

function fivestar_form_field_ui_field_edit_form_alter(&$form, $form_state) {
  $field = $form['#field'];
  if ($field['type'] == 'fivestar') {
    // Multiple values is not supported with Fivestar.
    $form['field']['cardinality']['#access'] = FALSE;
    $form['field']['cardinality']['#value'] = 1;
  }
}

/**
 * Implementation of hook_field_settings_form().
 */
function fivestar_field_settings_form($field, $instance) {
  $form['stars'] = array(
    '#type' => 'select',
    '#title' => $instance['widget']['type'] == 'stars' ? t('Number of Stars') : t('Number of Options'),
    '#options' => drupal_map_assoc(range(1, 10)),
    '#default_value' => isset($instance['stars']) ? $field['stars'] : 5,
  );

  $dynamic_options = array();
  if (module_exists('nodecomment')) {
    $dynamic_options['comment_target_nid'] = t('Node Comment Parent');
  }
  if (module_exists('nodereference')) {
    $fields = content_fields();
    foreach ($fields as $field_name => $content_field) {
      // Make sure that this field exists for this type.
      if ($content_field['type'] == 'nodereference' && $content_field = content_fields($field_name, $field['type_name'])) {
        $dynamic_options[$content_field['field_name']] = t('Node reference: @field', array('@field' => $content_field['field_name']));
      }
    }
  }

  if (empty($dynamic_options)) {
    drupal_set_message(t('No potential target fields are available for the %type bundle. Create a node reference field in this bundle to make it easier to assign a vote to a node.', array('%type' => $instance['bundle'])), 'warning');
  }

  $dynamic_options = array('' => '<'. t('none') .'>') + $dynamic_options;
  $form['dynamic_target'] = array(
    '#title' => t('Voting target'),
    '#type' => 'select',
    '#default_value' => isset($field['settings']['dynamic_target']) ? $field['settings']['dynamic_target'] : '',
    '#options' => $dynamic_options,
    '#description' => t('The voting target will make the value of this field cast a vote on another node. Use node reference fields (part of CCK core) or <a href="http://drupal.org/project/nodecomment">Node Comments</a> module to create advanced reviews. More information available on the <a href="http://drupal.org/handbook/modules/fivestar">Fivestar handbook page</a>.')
  );

  if (user_access('use PHP for fivestar target')) {
    $form['php_target'] = array(
      '#type' => 'fieldset',
      '#title' => t('Voting target PHP code'),
      '#collapsible' => TRUE,
      '#collapsed' => empty($field['php_target']),
    );

    $form['php_target']['php_target'] = array(
      '#title' => t('Code'),
      '#type' => 'textarea',
      '#default_value' => isset($field['php_target']) ? $field['settings']['php_target'] : '',
      '#description' => t('Advanced usage only: PHP code that returns a target node ID. Should not include &lt;?php ?&gt; delimiters. If this field is filled out, the value returned by this code will override any value specified above. Note that executing incorrect PHP-code can break your Drupal site.'),
    );
  }
  else {
    $form['php_target'] = array(
      '#type' => 'value',
      '#value' => isset($field['settings']['php_target']) ? $field['settings']['php_target'] : '',
    );
  }

  $form['axis'] = array(
    '#type' => 'textfield',
    '#title' => 'Voting Axis',
    '#description' => t('The axis this rating will affect. Enter a property on which that this rating will affect, such as <em>quality</em>, <em>satisfaction</em>, <em>overall</em>, etc. If no axis is entered, the default axis <em>vote</em> will be used. Warning: changing this value will not update existing votes to the new axis.'),
    '#default_value' => isset($field['settings']['axis']) ? $field['settings']['axis'] : '',
  );

  return $form;
}

/*
case 'save':
  return array('stars', 'dynamic_target', 'php_target', 'axis');
*/

function fivestar_field_schema() {
  return array(
    'columns' => array(
      'rating' => array('type' => 'int', 'unsigned' => TRUE, 'not null' => FALSE, 'sortable' => TRUE),
      'target' => array('type' => 'int', 'unsigned' => TRUE, 'not null' => FALSE),
    ),
  );
}

/**
 * Implementation of hook_field_presave().
 */
function fivestar_field_presave($obj_type, $object, $field, $instance, $langcode, &$items) {
  $items[0]['stars'] = $field['settings']['stars'];
}

/**
 * Implementation of hook_field_insert().
 */
function fivestar_field_insert($obj_type, $object, $field, $instance, $langcode, &$items) {
  _fivestar_field_helper($obj_type, $object, $field, $items);
}

/**
 * Implementation of hook_field_update().
 */
function fivestar_field_update($obj_type, $object, $field, $instance, $langcode, &$items) {
  _fivestar_field_helper($obj_type, $object, $field, $items);
}

/**
 * Implementation of hook_field_delete().
 */
function fivestar_field_delete($obj_type, $object, $field, $instance, $langcode, &$items) {
  _fivestar_field_helper($obj_type, $object, $field, $items, 'delete');
}

function _fivestar_field_helper($obj_type, $object, $field, $items, $op = '') {
  foreach ($items as $delta => $item) {
    if ((isset($object->status) && !$object->status) || $op == 'delete') {
      $rating = 0;
    }
    else {
      $rating = $items[$delta]['rating'];
    }
    $items[$delta]['target'] = fivestar_field_target($object, $field, $item);

    if (is_numeric($items[$delta]['target'])) {
      _fivestar_cast_vote('node', $items[$delta]['target'], $rating, $field['axis'], $node->uid, FALSE, TRUE);
      votingapi_recalculate_results('node', $items[$delta]['target']);
    }
  }
}

/**
 * Helper function to find the nid that should be rated when a field is changed.
 */
function fivestar_field_target($node, $field, $item) {
  $target = FALSE;

  if (!empty($field['php_target'])) {
    // Use eval rather than drupal_eval to allow access to local variables.
    $target = eval($field['php_target']);
  }
  elseif (!empty($field['settings']['dynamic_target']) && !empty($node->$field['dynamic_target'])) {
    if (is_array($node->$field['dynamic_target']) && is_numeric($node->{$field['dynamic_target']}[0]['nid'])) {
      $target = $node->{$field['dynamic_target']}[0]['nid'];
    }
    elseif (is_numeric($node->$field['dynamic_target'])) {
      $target = $node->$field['dynamic_target'];
    }
  }
  elseif (isset($item['target'][0]['nid'])) {
    $target = $item['target'][0]['nid'];
  }

  return $target;
}

/**
 * Implementation of hook_field_is_empty().
 */
function fivestar_field_is_empty($item, $field) {
  return empty($item['rating']) || $item['rating'] == '-';
}

/**
 * Implementation of hook_field_widget_info().
 */
function fivestar_field_widget_info() {
  return array(
    'stars' => array(
      'label' => t('Stars'),
      'field types' => array('fivestar'),
      'behaviors' => array('multiple values' => FIELD_BEHAVIOR_NONE),
    ),
    'radios' => array(
      'label' => t('Select list'),
      'field types' => array('fivestar'),
      'behaviors' => array('multiple values' => FIELD_BEHAVIOR_NONE),
    ),
  );
}

/**
 * Implementation of hook_widget_settings_form().
 */
function fivestar_widget_settings_form($field, $instance) {
  $form = array();
  $form['allow_clear'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow user to clear value'),
    '#default_value' => isset($instance['widget']['settings']['allow_clear']) ? $instance['widget']['settings']['allow_clear'] : 1,
  );
  return $form;
/*
    case 'save':
      return array('allow_clear');
*/
}

/**
 * Implementation of hook_field_widget_form().
 */
function fivestar_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $element = array('#tree' => TRUE);
  $element['rating'] = array(
    '#type' => 'fivestar',
    '#title' => $instance['label'],
    '#default_value' => isset($items[0]['rating']) ? $items[0]['rating'] : NULL,
    '#stars' => isset($field['settings']['stars']) ? $field['settings']['stars'] : 5,
    // dunno where these two comes from
    '#allow_clear' => isset($instance['widget']['settings']['allow_clear']) ? $instance['widget']['settings']['allow_clear'] : 0,
    '#description' => isset($instance['widget']['settings']['description']) ? $instance['widget']['settings']['description'] : '',
    '#weight' => $instance['widget']['weight'],
    '#auto_submit' => FALSE,
    '#widget' => $instance['widget']['type'],
    '#required' => $instance['required'],
    '#labels_enable' => FALSE,
  );
  $element['target'] = array(
    '#type' => 'value',
    // again i have no clue where this comes from.
    '#value' => isset($field['settings']['target']) ? $field['settings']['target'] : NULL,
  );
  $element['axis'] = array(
    '#type' => 'value',
    '#value' => $field['settings']['axis'],
  );

  // CCK likes to always have a 2D array for form elements.
  $element = array($element);

  return $element;
}

/**
 * Implementation of hook_field_formatter_info().
 */
function fivestar_field_formatter_info() {
  return array(
    'fivestar_formatter_default' => array(
      'label' => t('As Stars'),
      'field types' => array('fivestar'),
    ),
    'fivestar_formatter_rating' => array(
      'label' => t('Rating (i.e. 4.2/5)'),
      'field types' => array('fivestar'),
    ),
    'fivestar_formatter_percentage' => array(
      'label' => t('Percentage (i.e. 92)'),
      'field types' => array('fivestar'),
    ),
  );
}

function fivestar_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  
  $element[0] = array(
    '#theme' => $display['type'],
    '#item' => $items,
  );

  return $element;
}


/**
 * Theme function for 'default' fivestar field formatter.
 */
function theme_fivestar_formatter_default($variables) {
  $element = $variables['element'];
  if (!isset($element['#item'][0]['rating'])) {
    $element['#item'][0]['rating'] = 0;
  }
  if (empty($element['#item'][0]['stars'])) {
    $element['#item'][0]['stars'] = 5;
  }
  
   return theme('fivestar_static', array('rating' => $element['#item'][0]['rating'], 'stars' => $element['#item'][0]['stars']));
}

/**
 * Theme function for 'rating' fivestar field formatter.
 */
function theme_fivestar_formatter_rating($variables) {
  $element = $variables['element'];
  
  if (!isset($element['#item'][0]['rating'])) {
    $element['#item'][0]['rating'] = 0;
  }

  return $element['#item'][0]['rating'];
}

/**
 * Theme function for 'percentage' fivestar field formatter.
 */
function theme_fivestar_formatter_percentage($variables) {
  $element = $variables['element'];
  
  if (empty($element['#item'][0]['rating'])) {
    $element['#item'][0]['rating'] = 0;
  }
  if (empty($element['#item'][0]['stars'])) {
    $element['#item'][0]['stars'] = 5;
  }

  return round(100/$element['#item'][0]['rating'], 1)/$element['#item'][0]['stars'];
}

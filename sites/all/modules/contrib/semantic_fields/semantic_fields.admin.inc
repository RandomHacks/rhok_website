<?php
function semantic_fields_preset_list() {
  $output  = '';

  $header = array(
    t('Name'),
    t('Description'),
    t('Operations'),
  );
  $rows = array();

  $presets = semantic_fields_get_presets();
  foreach($presets as $preset) {
    $rows[] = array(
      $preset['human_name'],
      $preset['description'],
      l(t('edit'), 'admin/structure/semantic-fields/' . $preset['name'] . '/edit'),
    );
  }

  if (count($rows) > 0) {
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
  }
  else {
    $output .= t('No custom field formats have been added yet.');
  }

  return $output;
}

function semantic_fields_preset_form($form, &$form_state, $name = '') {
  if (!empty($name)) {
    drupal_set_title(t('Update field format'));
    $preset = semantic_fields_preset_load($name);
    $settings = $preset['data'];
    $op = 'update';
  }
  else {
    drupal_set_title(t('Add field format'));
    $settings = array();
    $op = 'insert';
  }

  $form['#attached']['css'] = array(drupal_get_path('module', 'semantic_fields') . '/theme/semantic_fields.css');

  $form['basic_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );
  $form['basic_settings']['op'] = array(
    '#type' => 'value',
    '#value' => $op,
  );
  if ($op == 'insert') {
    $form['basic_settings']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Machine name'),
      '#description' => t('A unique name to identify this field format in the database (a-z, 0-9, _).'),
      '#required' => TRUE,
    );
  }
  else {
    $form['basic_settings']['name'] = array(
      '#type' => 'value',
      '#value' => $preset['name'],
    );
  }
  $form['basic_settings']['human_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('A human-readable name to identify this field format.'),
    '#default_value' => isset($preset['human_name']) ? $preset['human_name'] : '',
    '#required' => TRUE,
  );

  $form['basic_settings']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('A description of this field format.'),
    '#default_value' => isset($preset['description']) ? $preset['description'] : '',
    '#required' => FALSE,
  );

  $form['semantic_field_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Semantic field settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );
  $form['semantic_field_settings']['field_container_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field container'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#parents' => array('semantic_field_settings'),
  );
  $form['semantic_field_settings']['field_container_fieldset']['field_element'] = array(
    '#type' => 'textfield',
    '#title' => t('Surround the entire field with'),
    '#description' => t('Insert the desired <abbr title="Hyper Text Markup Language">HTML</abbr> element that will be the container for the entire field. Leave blank for no surrounding element.'),
    '#default_value' => isset($settings['field_element']) ? $settings['field_element'] : 'div',
    '#required' => FALSE,
    '#size' => 20,
  );
  $form['semantic_field_settings']['field_container_fieldset']['field_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Choose field element class(es)'),
    '#description' => t('The field element automatically gets field, field type and specific field classes (ie. <em>field field-type-text field-field-my-field-name</em>).'),
    '#default_value' => isset($settings['field_class']) ? $settings['field_class'] : '',
    '#required' => FALSE,
  );
  $form['semantic_field_settings']['label_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field label'),
    '#description' => t('<strong>Please note that the placement or appearance of the label is still determined by the field display settings.</strong>'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('semantic-fields-labels')),
  );
  $form['semantic_field_settings']['label_fieldset']['above'] = array(
    '#type' => 'fieldset',
    '#title' => t('Labels above field value(s)'),
    '#description' => t('How to render the label when located above the field value(s).'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('semantic-fields-labels-above')),
    '#parents' => array('semantic_field_settings'),
  );
  $form['semantic_field_settings']['label_fieldset']['above']['label_element_above'] = array(
    '#type' => 'textfield',
    '#title' => t('Surround the label with'),
    '#description' => t('Insert the desired <abbr title="Hyper Text Markup Language">HTML</abbr> element that will surround the label.'),
    '#default_value' => isset($settings['label_element_above']) ? $settings['label_element_above'] : 'h3',
    '#required' => TRUE,
    '#size' => 20,
  );
  $form['semantic_field_settings']['label_fieldset']['above']['label_class_above'] = array(
    '#type' => 'textfield',
    '#title' => t('And a class of'),
    '#description' => t('The label automatically gets a class of <em>field-label</em>.'),
    '#default_value' => isset($settings['label_class_above']) ? $settings['label_class_above'] : '',
    '#required' => FALSE,
    '#size' => 20,
  );
  $form['semantic_field_settings']['label_fieldset']['above']['label_suffix_above'] = array(
    '#type' => 'textfield',
    '#title' => t('And a suffix of'),
    '#description' => t('Insert the desired suffix character, eg. a colon'),
    '#default_value' => isset($settings['label_suffix_above']) ? $settings['label_suffix_above'] : '',
    '#required' => FALSE,
    '#size' => 20,
  );
  $form['semantic_field_settings']['label_fieldset']['inline'] = array(
    '#type' => 'fieldset',
    '#title' => t('Inline labels'),
    '#description' => t('How to render the label when located inline with the field value(s).'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('semantic-fields-labels-inline')),
    '#parents' => array('semantic_field_settings'),
  );
  $form['semantic_field_settings']['label_fieldset']['inline']['label_element_inline'] = array(
    '#type' => 'textfield',
    '#title' => t('Surround the label with'),
    '#description' => t('Insert the desired <abbr title="Hyper Text Markup Language">HTML</abbr> element that will surround the label.'),
    '#default_value' => isset($settings['label_element_inline']) ? $settings['label_element_inline'] : 'span',
    '#required' => TRUE,
    '#size' => 20,
  );
  $form['semantic_field_settings']['label_fieldset']['inline']['label_class_inline'] = array(
    '#type' => 'textfield',
    '#title' => t('And a class of'),
    '#description' => t('The label automatically gets a class of <em>field-label-inline</em>.'),
    '#default_value' => isset($settings['label_class_inline']) ? $settings['label_class_inline'] : '',
    '#required' => FALSE,
    '#size' => 20,
  );
  $form['semantic_field_settings']['label_fieldset']['inline']['label_suffix_inline'] = array(
    '#type' => 'textfield',
    '#title' => t('And a suffix of'),
    '#description' => t('Insert the desired suffix character, eg. a colon'),
    '#default_value' => isset($settings['label_suffix_inline']) ? $settings['label_suffix_inline'] : ':',
    '#required' => FALSE,
    '#size' => 20,
  );
  $form['semantic_field_settings']['items_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field value(s)'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('semantic-fields-multiple-values')),
  );
  $form['semantic_field_settings']['items_fieldset']['multivalue'] = array(
    '#type' => 'fieldset',
    '#title' => t('Multiple value field'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('field-values', 'semantic-fields-multiple-values-all')),
  );
  $form['semantic_field_settings']['items_fieldset']['multivalue']['items_element'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('semantic-fields-multiple-values-fields')),
    '#parents' => array('semantic_field_settings'),
  );
  $form['semantic_field_settings']['items_fieldset']['multivalue']['items_element']['multiple_items_element'] = array(
    '#type' => 'textfield',
    '#title' => t('Surround all field values with'),
    '#description' => t('Insert the desired <abbr title="Hyper Text Markup Language">HTML</abbr> element that will be the container for all field values. Leave blank for no surrounding element.'),
    '#default_value' => isset($settings['multiple_items_element']) ? $settings['multiple_items_element'] : 'div',
    '#required' => FALSE,
    '#size' => 20,
  );
  $form['semantic_field_settings']['items_fieldset']['multivalue']['items_element']['multiple_items_class'] = array(
    '#type' => 'textfield',
    '#title' => t('And a class of'),
    '#description' => t('The surrounding fields element automatically gets the <em>field-items</em> class.'),
    '#default_value' => isset($settings['multiple_items_class']) ? $settings['multiple_items_class'] : '',
    '#required' => FALSE,
  );
  $form['semantic_field_settings']['items_fieldset']['multivalue']['item_element'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('semantic-fields-multiple-values-field')),
    '#parents' => array('semantic_field_settings'),
  );
  $form['semantic_field_settings']['items_fieldset']['multivalue']['item_element']['multiple_item_element'] = array(
    '#type' => 'textfield',
    '#title' => t('Surround each field value with'),
    '#description' => t('Insert the desired <abbr title="Hyper Text Markup Language">HTML</abbr> element that will be the container for each field value. Leave blank for no surrounding element.'),
    '#default_value' => isset($settings['multiple_item_element']) ? $settings['multiple_item_element'] : 'div',
    '#required' => FALSE,
    '#size' => 20,
  );
  $form['semantic_field_settings']['items_fieldset']['multivalue']['item_element']['multiple_item_class'] = array(
    '#type' => 'textfield',
    '#title' => t('And a class of'),
    '#description' => t('Each field instance automatically gets the <em>field-item</em> class. Insert a # where you want enumeration (ie field-item-#).'),
    '#default_value' => isset($settings['multiple_item_class']) ? $settings['multiple_item_class'] : '',
    '#required' => FALSE,
  );
  $form['semantic_field_settings']['items_fieldset']['singlevalue'] = array(
    '#type' => 'fieldset',
    '#title' => t('Single value field'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('semantic-fields-single-values-settings')),
    '#parents' => array('semantic_field_settings'),
  );
  $form['semantic_field_settings']['items_fieldset']['singlevalue']['use_singlevalue_settings'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the following settings when the field contains only one value'),
    '#description' => t('Leave this unchecked to use multiple value field settings when the field contains only one value.'),
    '#return_value' => 1,
    '#default_value' => isset($settings['use_singlevalue_settings']) ? $settings['use_singlevalue_settings'] : 0,
    '#required' => FALSE,
    '#size' => 20,
  );
  $form['semantic_field_settings']['items_fieldset']['singlevalue']['singlevalues'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('semantic-fields-single-values-item')),
    '#parents' => array('semantic_field_settings'),
  );
  $form['semantic_field_settings']['items_fieldset']['singlevalue']['singlevalues']['single_item_element'] = array(
    '#type' => 'textfield',
    '#title' => t('Surround field value with'),
    '#description' => t('Insert the desired <abbr title="Hyper Text Markup Language">HTML</abbr> element that will be the container for a single field value. Leave blank for no surrounding element.'),
    '#default_value' => isset($settings['single_item_element']) ? $settings['single_item_element'] : 'div',
    '#required' => FALSE,
    '#size' => 20,
  );
  $form['semantic_field_settings']['items_fieldset']['singlevalue']['singlevalues']['single_item_class'] = array(
    '#type' => 'textfield',
    '#title' => t('And a class of'),
    '#default_value' => isset($settings['single_item_class']) ? $settings['single_item_class'] : '',
    '#description' => t('The field instance will automatically get a <em>field-item</em> class.'),
    '#required' => FALSE,
  );
  $form['semantic_field_settings']['items_fieldset']['nth_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('First and last classes'),
    '#description' => t('If the <em>FIRST/LAST every n<sup>th</sup></em> option is empty or zero, the <em>FIRST class attribute</em> and <em>LAST class attribute</em> are added once to only the first and last field values. If this option is greater than or equal to 1, these classe are added to every n<sup>th</sup> row, which may be useful for grid layouts where the initial and final unit\'s lateral margins must be 0.'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('semantic-fields-nth-settings')),
    '#parents' => array('semantic_field_settings'),
  );
  $form['semantic_field_settings']['items_fieldset']['nth_fieldset']['last_every_nth'] = array(
    '#type' => 'textfield',
    '#title' => t('FIRST/LAST every n<sup>th</sup>'),
    '#default_value' => isset($settings['last_every_nth']) && is_numeric($settings['last_every_nth']) ? $settings['last_every_nth'] : '0',
    '#required' => FALSE,
    '#size' => 20,
  );
  $form['semantic_field_settings']['items_fieldset']['nth_fieldset']['first_class'] = array(
    '#type' => 'textfield',
    '#title' => t('FIRST class attribute'),
    '#default_value' => isset($settings['first_class']) ? $settings['first_class'] : 'first',
    '#required' => FALSE,
    '#size' => 20,
  );
  $form['semantic_field_settings']['items_fieldset']['nth_fieldset']['last_class'] = array(
    '#type' => 'textfield',
    '#title' => t('LAST class attribute'),
    '#default_value' => isset($settings['last_class']) ? $settings['last_class'] : 'last',
    '#required' => FALSE,
    '#size' => 20,
  );
  $form['semantic_field_settings']['items_fieldset']['striping_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Striping'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('semantic-fields-striping-settings')),
    '#parents' => array('semantic_field_settings'),
  );
  $form['semantic_field_settings']['items_fieldset']['striping_fieldset']['striping_classes'] = array(
    '#type' => 'textfield',
    '#title' => t('Striping class attributes'),
    '#description' => t('One striping class attribute is applied to each row. Separate multiple attributes with a space.'),
    '#default_value' => isset($settings['striping_classes']) ? $settings['striping_classes'] : 'odd even',
    '#required' => FALSE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}

function semantic_fields_preset_form_validate($form, &$form_state) {
  $op = $form_state['values']['basic_settings']['op'];
  $name = $form_state['values']['basic_settings']['name'];
  
  // Invalid field name.
  if (!preg_match('!^[a-z0-9_]+$!', $name)) {
    form_set_error('basic_settings][name', t('The machine name %name is invalid. The name must include only lowercase unaccentuated letters, numbers, and underscores.', array('%name' => $name)));
  }
  if (strlen($name) > 32) {
    form_set_error('basic_settings][name', t("The machine name %name is too long. The name is limited to 32 characters.", array('%name' => $name)));
  }

  if ($op == 'insert') {
    // Machine name already exists.
    $preset = semantic_fields_preset_load($name);
    if ($preset) {
      form_set_error('basic_settings][name', t("The machine name %name already exists.", array('%name' => $name)));
    }
  }
}

function semantic_fields_preset_form_submit($form, &$form_state) {
  $op = $form_state['values']['basic_settings']['op'];

  $preset = array(
    'name' => $form_state['values']['basic_settings']['name'],
    'human_name' => $form_state['values']['basic_settings']['human_name'],
    'description' => $form_state['values']['basic_settings']['description'],
    'data' => $form_state['values']['semantic_field_settings'],
  );

  if ($op == 'update') {
    $preset_source = semantic_fields_preset_load($form_state['values']['basic_settings']['name']);
    $preset = array_merge($preset_source, $preset);
    drupal_write_record('semantic_fields_preset', $preset, 'id');
  }
  else {
    drupal_write_record('semantic_fields_preset', $preset);
    // When a new preset is created, the menu needs to be rebuilt.
    variable_set('menu_rebuild_needed', TRUE);
  }

  drupal_set_message(t('Saved %label configuration.', array('%label' => $preset['human_name'])));

  $form_state['redirect'] = 'admin/structure/semantic-fields/list';
}

function semantic_fields_preset_delete_form($form, &$form_state, $name) {
  // TODO: check if preset is being used on any fields.
  
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $name,
  );

  return confirm_form($form, t('Are you sure you want to delete this field preset?'), 'admin/structure/semantic-fields/presets');
}

function semantic_fields_preset_delete_form_submit($form, &$form_state) {
  db_delete('semantic_fields_preset')
      ->condition('name', $form_state['values']['name'])
      ->execute();
  drupal_set_message(t('The field preset has been deleted.'));

  // TODO: Update fields that were using the deleted preset to now use the default field format.

  $form_state['redirect'] = 'admin/strucure/semantic-fields/presets';
}
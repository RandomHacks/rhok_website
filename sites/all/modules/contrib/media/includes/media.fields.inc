<?php

/**
 * @file: Provides a "Multimedia asset" field to the fields API
 */

/**
 * Implement hook_field_info().
 */
function media_field_info() {
  return array(
    'media' => array(
      'label' => t('Multimedia asset'),
      'description' => t('This field stores a reference to a multimedia asset.'),
      'settings' => array(),
      'instance_settings' => array(),
      'default_widget' => 'media_generic',
      'default_formatter' => 'media_large',
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function media_field_is_empty($item, $field) {
  if (!is_array($item) || (empty($item['fid']))) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Implement hook_field_widget_info().
 */
function media_field_widget_info() {
  return array(
    'media_generic' => array(
      'label' => t('Media file selector'),
      'field types' => array('media'),
      'settings' => array(
        'progress_indicator' => 'throbber',
        'allowed_types' => array('image'),
        'allowed_schemes' => array('public', 'private'),
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

/**
 * Implement hook_field_formatter_info
 */
function media_field_formatter_info() {
  $formatters = array();

  // Add a media field formatter for each file entity view mode.
  $entity_info = entity_get_info('file');
  foreach ($entity_info['view modes'] as $view_mode => $view_mode_info) {
    // @todo Modules other than Media can also add view modes to file entities.
    //   No reason not to allow them to be chosen when formatting a Media field,
    //   but because Field API keeps all formatters across all field types in a
    //   single array, we run the risk of name collission. The best way to solve
    //   this is to convert this into a single formatter with a setting for the
    //   view mode, similar to how the "Image" formatter has a setting for the
    //   "Image Style".
    if (strpos($view_mode, 'media_') === 0) {
      $formatters[$view_mode] = array(
        'label' => t($view_mode_info['label']),
        'field types' => array('media'),
      );
    }
  }

  // Also add a formatter for files to show larger icons based on mimetype. This
  // was originally used when media entities contained file fields. The current
  // file entity architecture no longer needs this, but people may have used
  // this formatter for other file fields on their website.
  // @todo Some day, remove this.
  $formatters['media_large_icon'] = array(
    'label' => t('Large filetype icon'),
    'field types' => array('file'),
  );

  return $formatters;
}

/**
 * Implements hook_field_prepare_view().
 *
 * @todo Get resolution on http://drupal.org/node/879034 regarding using this
 *   hook to load referenced entities.
 */
function media_field_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items) {
  // Collect all file ids that need loading.
  $fids = array();
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => $item) {
      if (is_array($item) && !empty($item['fid'])) {
        $fids[$item['fid']] = $item['fid'];
      }
    }
  }

  // Load them.
  $files = file_load_multiple(array_values($fids));

  // Replace the item data with loaded files instead of just the file ids.
  // @todo This causes problems with some Field API and other module
  //   integration. It would be better for the loaded file to be added into
  //   $item, not replace it. See http://drupal.org/node/1005642.
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => $item) {
      if (is_array($item) && !empty($item['fid'])) {
        $items[$id][$delta]['file'] = $files[$item['fid']];
      }
    }
  }
}

/**
 * Implement hook_field_formatter_view().
 */
function media_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  // @todo Legacy support for the extra formatter added to file fields. See
  //   media_field_formatter_info().
  if ($display['type'] == 'media_large_icon') {
    foreach ($items as $delta => $item) {
      $element[$delta] = array(
        '#theme' => 'media_formatter_large_icon',
        '#file' => $item['file'],
      );
    }
    return $element;
  }

  $files = array();
  foreach ($items as $delta => $item) {
    $files[$item['fid']] = $item['file'];
  }
  return file_view_multiple($files, $display['type'], 0, $langcode);
}

/**
 * Implement hook_field_widget_settings_form().
 */
function media_field_widget_settings_form($field, $instance) {

  $widget = $instance['widget'];
  $settings = $widget['settings'];
  $form = array();

  // Setup type selection form
  $types = media_type_get_types();
  $options = array();
  foreach ($types as $key => $definition) {
    $options[$key] = $definition->label;
  }

  $form['allowed_types'] = array (
    '#type' => 'checkboxes',
    '#title' => t('Allowed media types'),
    '#options' => $options,
    '#default_value' => $settings['allowed_types'],
    '#description' => t('Media types which are allowed for this field'),
    '#weight' => 1,
  );

  $streams = file_get_stream_wrappers();
  $options = array();
  unset($streams['temporary']);
  foreach ($streams as $scheme => $data) {
    $options[$scheme] = t('@scheme (@name)', array('@scheme' => $scheme . '://', '@name' => $data['name']));
  }
  $form['allowed_schemes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Allowed URI schemes'),
    '#options' => $options,
    '#default_value' => $settings['allowed_schemes'],
    '#description' => t('URI schemes include public:// and private:// which are the Drupal files directories, and may also refer to remote sites.'),
    '#weight' => 2,
  );
  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function media_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $widget_settings = $instance['widget']['settings'];

  $permission = user_access('edit media');
  if (!$permission) {
    // @todo: show the default file upload field.
    return;
  }

  // @todo The Field API supports automatic serialization / unserialization, so
  //   this should no longer be needed. After verifying with a module that uses
  //   the 'data' column, remove this.
  // @see media_field_widget_value()
  $current_value = array();
  if (isset($items[$delta])) {
    $current_value = $items[$delta];
    // @todo $items[$delta] is sometimes a loaded media entity (an object)
    //   rather than an array. This conflicts with Field API expectations (for
    //   example, it results in fatal errors when previewing a node with a
    //   multi-valued media field), so should be fixed. In the meantime, don't
    //   assume that $current_value is an array.
    if (is_array($current_value) && isset($current_value['data']) && is_string($current_value['data'])) {
      $current_value['data'] = unserialize($current_value['data']);
    }
  }

  $element += array(
    '#type' => 'media', // Would like to make this a fieldset, but throws some weird warning about element_children... not sure what it is about yet.
    '#collapsed' => TRUE,
    '#default_value' => $current_value,
    '#required' => $instance['required'],
    //'#media_file_extensions' => $instance['settings']['file_extensions'],
    // @TODO: Not implemented yet.
    '#media_options' => array(
      'global' => array(
        'types' => array_filter($instance['widget']['settings']['allowed_types']),
        // @todo: Not implemented
        'schemes' => $instance['widget']['settings']['allowed_schemes'],
      ),
    ),
  );

  return $element;
}

/**
 * Implements hook_field_validate().
 *
 * Possible error codes:
 * - 'media_fid_illegal_value': The value is not part of the list of allowed values.
 */
function media_field_validate($obj_type, $object, $field, $instance, $langcode, $items, &$errors) {

  $allowed_types = array_keys(array_filter($instance['widget']['settings']['allowed_types']));

    // @TODO: merge in stuff from media_uri_value
  foreach ($items as $delta => $item) {
    if ($item['fid'] == 0) {
      return TRUE;
      //@TODO: make support for submiting with just a URI here?
    }
    $result = db_select('file_managed', 'f')
      ->fields('f')
      ->condition('fid', $item['fid'])
      ->condition('type', $allowed_types, 'IN')
      ->execute()
      ->fetchField();
    if (!$result) {
      $errors[$field['field_name']][$langcode][$delta][] = array(
        'error' => 'media_fid_illegal_value',
        'message' => t('%name: illegal value.', array('%name' => t($instance['label']))),
      );
    }
  }
}

/**
 * @todo Is this function ever called? If not, remove it. The Field API now
 *   supports automatic serialization / unserialization, so this should no
 *   longer be needed. After verifying with a module that uses the 'data'
 *   column, remove this.
 *
 * @see media_field_widget_form()
 */
function media_field_widget_value($element, $input, $form_state) {
  $return = $input;
  
  if (!is_array($return)) {
    $return = array();
  }

  if (isset($return['data'])) {
    $return['data'] = serialize($return['data']);
  }

  $return += array(
    'fid' => 0,
    'title' => '',
    'data' => NULL,
  );

  return $return;
}

/**
 * @todo The following hook_field_(insert|update|delete|delete_revision)
 *   implementations are nearly identical to the File module implementations of
 *   the same field hooks. The only differences are:
 *   - We pass 'media' rather than 'file' as the module argument to the
 *     file_usage_(add|delete)() functions.
 *   - We do not delete the file / media entity when its usage count goes to 0.
 *   We should submit a core patch to File module to make it flexible with
 *   respect to the above, so that we can reuse its implementation rather than
 *   duplicating it.
 */

/**
 * Implements hook_field_insert().
 */
function media_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  // Add a new usage of each uploaded file.
  foreach ($items as $item) {
    $file = (object) $item;
    file_usage_add($file, 'media', $entity_type, $id);
  }
}

/**
 * Implements hook_field_update().
 *
 * Checks for files that have been removed from the object.
 */
function media_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  // On new revisions, all files are considered to be a new usage and no
  // deletion of previous file usages are necessary.
  if (!empty($entity->revision)) {
    foreach ($items as $item) {
      $file = (object) $item;
      file_usage_add($file, 'media', $entity_type, $id);
    }
    return;
  }
  
  // Build a display of the current FIDs.
  $current_fids = array();
  foreach ($items as $item) {
    $current_fids[] = $item['fid'];
  }
  
  // Create a bare-bones entity so that we can load its previous values.
  $original = entity_create_stub_entity($entity_type, array($id, $vid, $bundle));
  field_attach_load($entity_type, array($id => $original), FIELD_LOAD_CURRENT, array('field_id' => $field['id']));

  // Compare the original field values with the ones that are being saved.
  $original_fids = array();
  if (!empty($original->{$field['field_name']}[$langcode])) {
    foreach ($original->{$field['field_name']}[$langcode] as $original_item) {
      $original_fids[] = $original_item['fid'];
      if (isset($original_item['fid']) && !in_array($original_item['fid'], $current_fids)) {
        // Decrement the file usage count by 1.
        $file = (object)$original_item;
        file_usage_delete($file, 'media', $entity_type, $id, 1);
      }
    }
  }

  // Add new usage entries for newly added files.
  foreach ($items as $item) {
    if (!in_array($item['fid'], $original_fids)) {
      $file = (object) $item;
      file_usage_add($file, 'media', $entity_type, $id);
    }
  }
}

/**
 * Implements hook_field_delete().
 */
function media_field_delete($entity_type, $entity, $field, $instance, $langcode, &$items) {
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  // Delete all file usages within this entity.
  foreach ($items as $delta => $item) {
    $file = (object) $item;
    file_usage_delete($file, 'media', $entity_type, $id, 0);
  }
}

/**
 * Implements hook_field_delete_revision().
 */
function media_field_delete_revision($entity_type, $entity, $field, $instance, $langcode, &$items) {
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
  foreach ($items as $delta => $item) {
    // @TODO: Not sure if this is correct
    $file = (object)$item;
    if (file_usage_delete($file, 'media', $entity_type, $id, 1)) {
      $items[$delta] = NULL;
    }
  }
}

/**
 * Implements hook_field_instance_update().
 */
function media_field_update_instance($instance, $prior_instance) {
  // Clear the filter cache when updating instance settings for a media entity.
  if ($instance['entity_type'] == 'media') {
    media_filter_invalidate_caches();
  }
}

<?php

/**
 * @file
 * API extensions of Drupal core's file.inc.
 */

/**
 * Returns information about file types from hook_file_type_info().
 *
 * @param $file_type
 *   (optional) A file type name. If ommitted, all file types will be returned.
 *
 * @return
 *   Either a file type description, as provided by hook_file_type_info(), or an
 *   array of all existing file types, keyed by file type name.
 */
function file_info_file_types($file_type = NULL) {
  $info = &drupal_static(__FUNCTION__);
  if (!isset($info)) {
    $info = module_invoke_all('file_type_info');
    drupal_alter('file_type_info', $info);
    _file_sort_array_by_weight($info);
  }
  if ($file_type) {
    if (isset($info[$file_type])) {
      return $info[$file_type];
    }
  }
  else {
    return $info;
  }
}

/**
 * Determines the file type of a passed in file object.
 */
function file_get_type($file) {
  foreach (file_info_file_types() as $type => $info) {
    if (isset($info['claim callback']) && ($function = $info['claim callback']) && function_exists($function) && $function($file, $type)) {
      return $type;
    }
  }
}

/**
 * Returns information about file formatters from hook_file_formatter_info().
 *
 * @param $formatter_type
 *   (optional) A file formatter type name. If ommitted, all file formatter
 *   will be returned.
 *
 * @return
 *   Either a file formatter description, as provided by
 *   hook_file_formatter_info(), or an array of all existing file formatters,
 *   keyed by formatter type name.
 */
function file_info_formatter_types($formatter_type = NULL) {
  $info = &drupal_static(__FUNCTION__);
  if (!isset($info)) {
    $info = module_invoke_all('file_formatter_info');
    drupal_alter('file_formatter_info', $info);
    _file_sort_array_by_weight($info);
  }
  if ($formatter_type) {
    if (isset($info[$formatter_type])) {
      return $info[$formatter_type];
    }
  }
  else {
    return $info;
  }
}

/**
 * Construct a drupal_render() style array from an array of loaded files.
 *
 * @param $files
 *   An array of files as returned by file_load_multiple().
 * @param $view_mode
 *   View mode.
 * @param $weight
 *   An integer representing the weight of the first file in the list.
 * @param $langcode
 *   A string indicating the language field values are to be shown in. If no
 *   language is provided the current content language is used.
 *
 * @return
 *   An array in the format expected by drupal_render().
 */
function file_view_multiple($files, $view_mode = 'default', $weight = 0, $langcode = NULL) {
  field_attach_prepare_view('file', $files, $view_mode);
  entity_prepare_view('file', $files);

  $build = array(
    '#sorted' => TRUE,
  );
  foreach ($files as $file) {
    $build[$file->fid] = file_view($file, $view_mode, $langcode);
    $build[$file->fid]['#weight'] = $weight;
    $weight++;
  }
  return $build;
}

/**
 * Generate an array for rendering the given file.
 *
 * @param $file
 *   A file object.
 * @param $view_mode
 *   View mode.
 * @param $langcode
 *   (optional) A language code to use for rendering. Defaults to the global
 *   content language of the current request.
 *
 * @return
 *   An array as expected by drupal_render().
 */
function file_view($file, $view_mode = 'default', $langcode = NULL) {
  if (!isset($langcode)) {
    $langcode = $GLOBALS['language_content']->language;
  }

  // Prepare the file object for viewing, in case file_view() was called by
  // something other than file_view_multiple(). These functions exit quickly if
  // they've already run, so it's okay to call them even if they've already been
  // called by file_view_multiple().
  field_attach_prepare_view('file', array($file->fid => $file), $view_mode);
  entity_prepare_view('file', array($file->fid => $file));

  // Create the render array with the file itself and with fields.
  $build = array(
    '#file' => $file,
    '#view_mode' => $view_mode,
    '#language' => $langcode,
  );
  $build += field_attach_view('file', $file, $view_mode, $langcode);
  $build['file'] = file_view_file($file, $view_mode, $langcode);

  // Allow modules to add and alter.
  module_invoke_all('file_view', $file, $view_mode, $langcode);
  module_invoke_all('entity_view', $file, 'file', $view_mode, $langcode);
  $type = 'file';
  drupal_alter(array('file_view', 'entity_view'), $build, $type);

  return $build;
}

/**
 * Generate an array for rendering just the file portion of a file entity.
 *
 * @param $file
 *   A file object.
 * @param $displays
 *   Can be either:
 *   - the name of a view mode;
 *   - or an array of custom display settings, as returned by file_displays().
 * @param $langcode
 *   (optional) A language code to use for rendering. Defaults to the global
 *   content language of the current request.
 *
 * @return
 *   An array as expected by drupal_render().
 */
function file_view_file($file, $displays = 'default', $langcode = NULL) {
  if (!isset($langcode)) {
    $langcode = $GLOBALS['language_content']->language;
  }

  // Prepare incoming display specifications.
  if (is_string($displays)) {
    $view_mode = $displays;
    $displays = file_displays($file->type, $view_mode);
  }
  else {
    $view_mode = '_custom_display';
  }
  drupal_alter('file_displays', $displays, $file, $view_mode);
  _file_sort_array_by_weight($displays);

  // Attempt to display the file with each of the possible displays. Stop after
  // the first successful one. See file_displays() for details.
  $element = NULL;
  foreach ($displays as $formatter_type => $display) {
    if (!empty($display['status'])) {
      $formatter_info = file_info_formatter_types($formatter_type);
      // Under normal circumstances, the UI prevents enabling formatters for
      // incompatible file types. In case this was somehow circumvented (for
      // example, a module updated its formatter definition without updating
      // existing display settings), perform an extra check here.
      if (isset($formatter_info['file types']) && !in_array($file->type, $formatter_info['file types'])) {
        continue;
      }
      if (isset($formatter_info['view callback']) && ($function = $formatter_info['view callback']) && function_exists($function)) {
        $display['type'] = $formatter_type;
        if (isset($formatter_info['default settings'])) {
          if (!isset($display['settings'])) {
            $display['settings'] = array();
          }
          $display['settings'] += $formatter_info['default settings'];
        }
        $element = $function($file, $display, $langcode);
        if (isset($element)) {
          break;
        }
      }
    }
  }

  // If none of the configured formatters were able to display the file, attempt
  // to display the file using the file type's default view callback.
  if (!isset($element)) {
    $file_type_info = file_info_file_types($file->type);
    if (isset($file_type_info['default view callback']) && ($function = $file_type_info['default view callback']) && function_exists($function)) {
      $element = $function($file, $view_mode, $langcode);
    }
  }

  // If a render element was returned by a formatter or the file type's default
  // view callback, add some defaults to it and return it.
  if (isset($element)) {
    $element += array(
      '#file' => $file,
      '#view_mode' => $view_mode,
      '#language' => $langcode,
    );
    return $element;
  }
}

/**
 * Returns an array of possible displays to use for a file type in a given view mode.
 *
 * It is common for a site to be configured with broadly defined file types
 * (e.g., 'video'), and to have different files of this type require different
 * displays (for example, the code required to display a YouTube video is
 * different than the code required to display a local QuickTime video).
 * Therefore, the site administrator can configure multiple displays for a given
 * file type. This function returns all of the displays that the administrator
 * enabled for the given file type in the given view mode. file_view_file() then
 * invokes each of these, and passes the specific file to display. Each display
 * implementation can inspect the file, and either return a render array (if it
 * is capable of displaying the file), or return nothing (if it is incapable of
 * displaying the file). The first render array returned is the one used.
 *
 * @param $file_type
 *   The type of file.
 * @param $view_mode
 *   The view mode.
 *
 * @return
 *   An array keyed by the formatter type name. Each item in the array contains
 *   the following key/value pairs:
 *   - status: Whether this display is enabled. If not TRUE, file_view_file()
 *     skips over it.
 *   - weight: An integer that determines the order of precedence within the
 *     returned array. The lowest weight display capable of displaying the file
 *     is used.
 *   - settings: An array of key/value pairs specific to the formatter type. See
 *     hook_file_formatter_info() for details.
 *
 * @see hook_file_formatter_info()
 * @see file_view_file()
 */
function file_displays($file_type, $view_mode = 'default') {
  // If the requested view mode isn't configured to use a custom display for its
  // fields, then don't use a custom display for its file either.
  $view_mode_settings = field_view_mode_settings('file', $file_type);
  if (empty($view_mode_settings[$view_mode]['custom_settings'])) {
    $view_mode = 'default';
  }

  $displays_info = variable_get('file_displays');
  if ($view_mode !== 'default' && isset($displays_info[$file_type][$view_mode])) {
    $displays = $displays_info[$file_type][$view_mode];
  }
  elseif (isset($displays_info[$file_type]['default'])) {
    $displays = $displays_info[$file_type]['default'];
  }
  else {
    $displays = array();
  }

  return $displays;
}

/**
 * Helper function to sort an array by the value of each item's 'weight' key, while preserving relative order of items that have equal weight.
 */
function _file_sort_array_by_weight(&$a) {
  $i=0;
  foreach ($a as $key => $item) {
    if (!isset($a[$key]['weight'])) {
      $a[$key]['weight'] = 0;
    }
    $original_weight[$key] = $a[$key]['weight'];
    $a[$key]['weight'] += $i/1000;
    $i++;
  }
  uasort($a, 'drupal_sort_weight');
  foreach ($a as $key => $item) {
    $a[$key]['weight'] = $original_weight[$key];
  }
}

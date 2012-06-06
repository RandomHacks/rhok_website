<?php
/**
 * @file
 * rhok_event_feature.features.inc
 */

/**
 * Implementation of hook_ctools_plugin_api().
 */
function rhok_event_feature_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
}

/**
 * Implementation of hook_views_api().
 */
function rhok_event_feature_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => 3.0);
  }
}

/**
 * Implementation of hook_node_info().
 */
function rhok_event_feature_node_info() {
  $items = array(
    'event' => array(
      'name' => t('Event'),
      'base' => 'node_content',
      'description' => t('An event in a specific location'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'event_group' => array(
      'name' => t('Event Group'),
      'base' => 'node_content',
      'description' => t('A collection of events in different locations, all under the same name, e.g. RHoK #...'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

<?php
/**
 * @file
 * rhok_media_feature.features.inc
 */

/**
 * Implementation of hook_ctools_plugin_api().
 */
function rhok_media_feature_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  list($module, $api) = func_get_args();
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_views_api().
 */
function rhok_media_feature_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => 3.0);
  }
}

/**
 * Implementation of hook_node_info().
 */
function rhok_media_feature_node_info() {
  $items = array(
    'media_mention' => array(
      'name' => t('Media Mention'),
      'base' => 'node_content',
      'description' => t('A mention of RHoK in the media.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

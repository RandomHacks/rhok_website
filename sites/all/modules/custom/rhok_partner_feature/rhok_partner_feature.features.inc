<?php
/**
 * @file
 * rhok_partner_feature.features.inc
 */

/**
 * Implementation of hook_ctools_plugin_api().
 */
function rhok_partner_feature_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
}

/**
 * Implementation of hook_views_api().
 */
function rhok_partner_feature_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => 3.0);
  }
}

/**
 * Implementation of hook_node_info().
 */
function rhok_partner_feature_node_info() {
  $items = array(
    'partner' => array(
      'name' => t('Partner'),
      'base' => 'node_content',
      'description' => t('A partner in RHoK.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  return $items;
}

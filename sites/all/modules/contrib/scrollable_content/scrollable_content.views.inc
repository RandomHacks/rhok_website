<?php

/**
 * @file
 *  Provides the views plugin information.
 */

/**
  * Implementation of hook_views_plugin()
  */
function scrollable_content_views_plugins() {
  return array(
    'module' => 'scrollable_content',
    'style' => array(
      'scrollable_content' => array(
        'title' => t('Scrollable Content'),
        'help' => t('Display your content in a scrolling layout (content slider).'),
        'handler' => 'scrollable_content_plugin_style',
        'theme' => 'scrollable_content_view',
        'type' => 'normal',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'uses fields' => TRUE,
      )
    )
  );
}
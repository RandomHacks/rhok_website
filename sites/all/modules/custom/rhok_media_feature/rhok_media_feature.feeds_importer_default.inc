<?php
/**
 * @file
 * rhok_media_feature.feeds_importer_default.inc
 */

/**
 * Implementation of hook_feeds_importer_default().
 */
function rhok_media_feature_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'media_importer';
  $feeds_importer->config = array(
    'name' => 'Media Importer',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsSyndicationParser',
      'config' => array(),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'content_type' => 'media_mention',
        'expire' => '-1',
        'author' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'title',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'description',
            'target' => 'body',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'timestamp',
            'target' => 'created',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'url',
            'target' => 'field_link:url',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'title',
            'target' => 'field_link:title',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'input_format' => 'full_html',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => 1800,
    'expire_period' => 3600,
    'import_on_create' => TRUE,
    'process_in_background' => FALSE,
  );
  $export['media_importer'] = $feeds_importer;

  return $export;
}

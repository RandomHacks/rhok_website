<?php
/**
 * @file
 * rhok_media_feature.context.inc
 */

/**
 * Implementation of hook_context_default_contexts().
 */
function rhok_media_feature_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'media';
  $context->description = '';
  $context->tag = 'page';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'media' => 'media',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-media_mentions-block' => array(
          'module' => 'views',
          'delta' => 'media_mentions-block',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('page');
  $export['media'] = $context;

  return $export;
}

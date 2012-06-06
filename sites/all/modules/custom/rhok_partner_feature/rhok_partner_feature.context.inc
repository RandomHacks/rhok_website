<?php
/**
 * @file
 * rhok_partner_feature.context.inc
 */

/**
 * Implementation of hook_context_default_contexts().
 */
function rhok_partner_feature_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'partners';
  $context->description = '';
  $context->tag = 'page';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'partners' => 'partners',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-partners-block_1' => array(
          'module' => 'views',
          'delta' => 'partners-block_1',
          'region' => 'content',
          'weight' => '-30',
        ),
        'views-local_sponsors-block_1' => array(
          'module' => 'views',
          'delta' => 'local_sponsors-block_1',
          'region' => 'content',
          'weight' => '-29',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('page');
  $export['partners'] = $context;

  return $export;
}

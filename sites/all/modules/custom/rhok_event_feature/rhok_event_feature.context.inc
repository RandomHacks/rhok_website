<?php
/**
 * @file
 * rhok_event_feature.context.inc
 */

/**
 * Implementation of hook_context_default_contexts().
 */
function rhok_event_feature_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'Event';
  $context->description = '';
  $context->tag = 'type';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'event' => 'event',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-local_sponsors-block' => array(
          'module' => 'views',
          'delta' => 'local_sponsors-block',
          'region' => 'content',
          'weight' => '-10',
        ),
        'views-blog-block_2' => array(
          'module' => 'views',
          'delta' => 'blog-block_2',
          'region' => 'content',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('type');
  $export['Event'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'event_group';
  $context->description = '';
  $context->tag = 'type';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'event_group' => 'event_group',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-events-block_1' => array(
          'module' => 'views',
          'delta' => 'events-block_1',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('type');
  $export['event_group'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'events';
  $context->description = '';
  $context->tag = 'page';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'events' => 'events',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'quicktabs-event_tabs_block' => array(
          'module' => 'quicktabs',
          'delta' => 'event_tabs_block',
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
  $export['events'] = $context;

  return $export;
}

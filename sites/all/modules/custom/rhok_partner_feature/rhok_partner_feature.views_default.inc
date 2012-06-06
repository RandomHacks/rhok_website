<?php
/**
 * @file
 * rhok_partner_feature.views_default.inc
 */

/**
 * Implementation of hook_views_default_views().
 */
function rhok_partner_feature_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'local_sponsors';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Local Sponsors';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Local Sponsors';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Logo */
  $handler->display->display_options['fields']['field_logo']['id'] = 'field_logo';
  $handler->display->display_options['fields']['field_logo']['table'] = 'field_data_field_logo';
  $handler->display->display_options['fields']['field_logo']['field'] = 'field_logo';
  $handler->display->display_options['fields']['field_logo']['label'] = '';
  $handler->display->display_options['fields']['field_logo']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_logo']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_logo']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_logo']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['field_logo']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_logo']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_logo']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_logo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_logo']['settings'] = array(
    'image_style' => 'partner_logo_large',
    'image_link' => '',
  );
  $handler->display->display_options['fields']['field_logo']['field_api_classes'] = 0;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['order'] = 'DESC';
  /* Contextual filter: Fields: Related Events (field_event_or_group) - nid */
  $handler->display->display_options['arguments']['field_event_or_group_nid']['id'] = 'field_event_or_group_nid';
  $handler->display->display_options['arguments']['field_event_or_group_nid']['table'] = 'field_data_field_event_or_group';
  $handler->display->display_options['arguments']['field_event_or_group_nid']['field'] = 'field_event_or_group_nid';
  $handler->display->display_options['arguments']['field_event_or_group_nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_event_or_group_nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['field_event_or_group_nid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['field_event_or_group_nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_event_or_group_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_event_or_group_nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_event_or_group_nid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['field_event_or_group_nid']['not'] = 0;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'local_sponsor' => 'local_sponsor',
  );

  /* Display: Event Page Block */
  $handler = $view->new_display('block', 'Event Page Block', 'block');

  /* Display: Partner Page Block */
  $handler = $view->new_display('block', 'Partner Page Block', 'block_1');
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Logo */
  $handler->display->display_options['fields']['field_logo']['id'] = 'field_logo';
  $handler->display->display_options['fields']['field_logo']['table'] = 'field_data_field_logo';
  $handler->display->display_options['fields']['field_logo']['field'] = 'field_logo';
  $handler->display->display_options['fields']['field_logo']['label'] = '';
  $handler->display->display_options['fields']['field_logo']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_logo']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_logo']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_logo']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['field_logo']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_logo']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_logo']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_logo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_logo']['settings'] = array(
    'image_style' => 'partner_logo_large',
    'image_link' => '',
  );
  $handler->display->display_options['fields']['field_logo']['field_api_classes'] = 0;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $translatables['local_sponsors'] = array(
    t('Master'),
    t('Local Sponsors'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('All'),
    t('Event Page Block'),
    t('Partner Page Block'),
  );
  $export['local_sponsors'] = $view;

  $view = new view;
  $view->name = 'partners';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Partners';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['grouping'] = 'field_partner_type';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Logo */
  $handler->display->display_options['fields']['field_logo']['id'] = 'field_logo';
  $handler->display->display_options['fields']['field_logo']['table'] = 'field_data_field_logo';
  $handler->display->display_options['fields']['field_logo']['field'] = 'field_logo';
  $handler->display->display_options['fields']['field_logo']['label'] = '';
  $handler->display->display_options['fields']['field_logo']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['field_logo']['alter']['path'] = 'partners';
  $handler->display->display_options['fields']['field_logo']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_logo']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_logo']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_logo']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['field_logo']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_logo']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_logo']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_logo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_logo']['settings'] = array(
    'image_style' => 'partner_logo',
    'image_link' => '',
  );
  $handler->display->display_options['fields']['field_logo']['field_api_classes'] = 0;
  /* Field: Content: Partner Type */
  $handler->display->display_options['fields']['field_partner_type']['id'] = 'field_partner_type';
  $handler->display->display_options['fields']['field_partner_type']['table'] = 'field_data_field_partner_type';
  $handler->display->display_options['fields']['field_partner_type']['field'] = 'field_partner_type';
  $handler->display->display_options['fields']['field_partner_type']['label'] = '';
  $handler->display->display_options['fields']['field_partner_type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_partner_type']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_partner_type']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_partner_type']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['element_type'] = 'span';
  $handler->display->display_options['fields']['field_partner_type']['element_class'] = 'group-name';
  $handler->display->display_options['fields']['field_partner_type']['element_label_type'] = '0';
  $handler->display->display_options['fields']['field_partner_type']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_partner_type']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['click_sort_column'] = 'tid';
  $handler->display->display_options['fields']['field_partner_type']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_partner_type']['field_api_classes'] = 0;
  /* Field: Content: URL */
  $handler->display->display_options['fields']['field_partner_url']['id'] = 'field_partner_url';
  $handler->display->display_options['fields']['field_partner_url']['table'] = 'field_data_field_partner_url';
  $handler->display->display_options['fields']['field_partner_url']['field'] = 'field_partner_url';
  $handler->display->display_options['fields']['field_partner_url']['label'] = '';
  $handler->display->display_options['fields']['field_partner_url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_partner_url']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_partner_url']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_partner_url']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_partner_url']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['field_api_classes'] = 0;
  /* Sort criterion: Fields: Partner Type (field_partner_type) - tid */
  $handler->display->display_options['sorts']['field_partner_type_tid']['id'] = 'field_partner_type_tid';
  $handler->display->display_options['sorts']['field_partner_type_tid']['table'] = 'field_data_field_partner_type';
  $handler->display->display_options['sorts']['field_partner_type_tid']['field'] = 'field_partner_type_tid';
  /* Sort criterion: Global: Random */
  $handler->display->display_options['sorts']['random']['id'] = 'random';
  $handler->display->display_options['sorts']['random']['table'] = 'views';
  $handler->display->display_options['sorts']['random']['field'] = 'random';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'partner' => 'partner',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'partner' => 'partner',
  );
  /* Filter criterion: Content: Promoted to front page */
  $handler->display->display_options['filters']['promote']['id'] = 'promote';
  $handler->display->display_options['filters']['promote']['table'] = 'node';
  $handler->display->display_options['filters']['promote']['field'] = 'promote';
  $handler->display->display_options['filters']['promote']['value'] = '1';

  /* Display: Partners Page Block */
  $handler = $view->new_display('block', 'Partners Page Block', 'block_1');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['grouping'] = 'field_partner_type';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: URL */
  $handler->display->display_options['fields']['field_partner_url']['id'] = 'field_partner_url';
  $handler->display->display_options['fields']['field_partner_url']['table'] = 'field_data_field_partner_url';
  $handler->display->display_options['fields']['field_partner_url']['field'] = 'field_partner_url';
  $handler->display->display_options['fields']['field_partner_url']['label'] = '';
  $handler->display->display_options['fields']['field_partner_url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_partner_url']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_partner_url']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_partner_url']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_partner_url']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_partner_url']['field_api_classes'] = 0;
  /* Field: Content: Logo */
  $handler->display->display_options['fields']['field_logo']['id'] = 'field_logo';
  $handler->display->display_options['fields']['field_logo']['table'] = 'field_data_field_logo';
  $handler->display->display_options['fields']['field_logo']['field'] = 'field_logo';
  $handler->display->display_options['fields']['field_logo']['label'] = '';
  $handler->display->display_options['fields']['field_logo']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['path'] = '[field_partner_url]';
  $handler->display->display_options['fields']['field_logo']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['external'] = 1;
  $handler->display->display_options['fields']['field_logo']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_logo']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_logo']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_logo']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_logo']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['field_logo']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_logo']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_logo']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_logo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_logo']['settings'] = array(
    'image_style' => 'partner_logo_large',
    'image_link' => 'content',
  );
  $handler->display->display_options['fields']['field_logo']['field_api_classes'] = 0;
  /* Field: Content: Partner Type */
  $handler->display->display_options['fields']['field_partner_type']['id'] = 'field_partner_type';
  $handler->display->display_options['fields']['field_partner_type']['table'] = 'field_data_field_partner_type';
  $handler->display->display_options['fields']['field_partner_type']['field'] = 'field_partner_type';
  $handler->display->display_options['fields']['field_partner_type']['label'] = '';
  $handler->display->display_options['fields']['field_partner_type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_partner_type']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_partner_type']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_partner_type']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['element_type'] = 'span';
  $handler->display->display_options['fields']['field_partner_type']['element_class'] = 'group-name';
  $handler->display->display_options['fields']['field_partner_type']['element_label_type'] = '0';
  $handler->display->display_options['fields']['field_partner_type']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_partner_type']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_partner_type']['click_sort_column'] = 'tid';
  $handler->display->display_options['fields']['field_partner_type']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_partner_type']['field_api_classes'] = 0;
  $translatables['partners'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('partners'),
    t('Block'),
    t('Partners Page Block'),
    t('user'),
    t('[field_partner_url]'),
  );
  $export['partners'] = $view;

  return $export;
}

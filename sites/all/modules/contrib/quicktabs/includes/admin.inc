<?php
// $Id: admin.inc,v 1.1.4.16 2011/02/18 22:57:42 katbailey Exp $
/**
 * @file
 * Provides the Quick Tabs' administrative interface.
 */

/**
 * Page callback to list quicktabs in the system.
 */
function quicktabs_list() {
  $colspan = module_exists('ctools') ? 4 : 3;
  $header = array(
    array('data' => t('Quicktab')),
    array('data' => t('Storage')),
    array('data' => t('Operations'), 'colspan' => $colspan),
  );
  $rows = array();
  
  foreach (quicktabs_get_all_quicktabs() as $quicktab) {
    if (module_exists('ctools')) {
      // Determine storage
      switch ($quicktab->export_type) {
        case EXPORT_IN_DATABASE | EXPORT_IN_CODE:
          $storage = t('Overridden');
          $delete = l(t('Revert'), 'admin/structure/quicktabs/manage/'. $quicktab->machine_name .'/delete');
          break;
        case EXPORT_IN_DATABASE:
          $storage = t('Normal');
          $delete = l(t('Delete'), 'admin/structure/quicktabs/manage/'. $quicktab->machine_name .'/delete');
          break;
        case EXPORT_IN_CODE:
          $storage = t('Default');
          $delete = '';
          break;
      }

      $tablerow = array(
        array('data' => $quicktab->title),
        array('data' => $storage),
        array('data' => l(t('Edit'), 'admin/structure/quicktabs/manage/'. $quicktab->machine_name .'/edit')),
        array('data' => l(t('Export'), 'admin/structure/quicktabs/manage/'. $quicktab->machine_name .'/export')),
        array('data' => l(t('Clone'), 'admin/structure/quicktabs/manage/'. $quicktab->machine_name .'/clone')),
        array('data' => $delete),
      );
    }
    else {
      $tablerow = array(
        array('data' => $quicktab->title),
        array('data' => t('Normal')),
        array('data' => l(t('Edit'), 'admin/structure/quicktabs/manage/'. $quicktab->machine_name .'/edit')),
        array('data' => l(t('Clone'), 'admin/structure/quicktabs/manage/'. $quicktab->machine_name .'/clone')),
        array('data' => l(t('Delete'), 'admin/structure/quicktabs/manage/'. $quicktab->machine_name .'/delete')),
      );
    }
    $rows[] = $tablerow;
  }
  
  
  if (empty($rows)) {
    $rows[] = array(array('data' => t('No quicktabs available.'), 'colspan' => $colspan));
  }

  $build = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'quicktabs'),
  );
  return $build;
}

/**
 * Callback function for admin/settings/quicktabs. Display the settings form.
 */
function quicktabs_styles() {
  $options = array();
  $styles = module_invoke_all('quicktabs_tabstyles');
  // The keys used for options must be valid html id-s.
  // Removing the css file path, because that can't be used.
  foreach ($styles as $style) {
    $options[$style] = $style;
  }
  ksort($options);

  $form['quicktabs_tabstyle'] = array(
    '#type' => 'radios',
    '#title' => t('Quicktab styles'),
    '#options' => array('nostyle' => t('No style')) + $options,
    '#default_value' => variable_get('quicktabs_tabstyle', 'nostyle'),
    '#description' => t('Select the default style for quicktabs.'),
    '#attributes' => array('class' => array('quicktabs-tabstyles', 'clear-block')),
    '#theme' => 'quicktabs_style_options',
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Submit handler for QuickTabs styles.
 */
function quicktabs_styles_submit($form, &$form_state) {
  variable_set('quicktabs_tabstyle', $form_state['values']['quicktabs_tabstyle']);
  drupal_set_message(t('The default quicktab style has been saved.'));
}

/**
 * Theme function for quicktabs style radio options.
 *
 * @ingroup themeable
 */
function theme_quicktabs_style_options($variables) {
  $style_element = $variables['quicktabs_tabstyle'];
  $markup = '';

  $tabs['one'] = array('title' => t('One'), 'type' => 'freetext', 'text' => t('First tab'));
  $tabs['two'] = array('title' => t('Two'), 'type' => 'freetext', 'text' => t('Second tab'));
  $tabs['three'] = array('title' => t('Three'), 'type' => 'freetext', 'text' => t('Third tab'));
  $quicktabs->tabs = $tabs;
  $quicktabs->ajax = FALSE;
  $quicktabs->hide_empty_tabs = FALSE;
  // Preview for each style.
  foreach (element_children($style_element) as $style) {
    $element = $style_element[$style];
    $quicktabs->style = $style;
    $quicktabs->machine_name = drupal_strtolower($style);
    $preview = '<div class="quicktabs-preview">'. drupal_render(quicktabs_render($quicktabs)) .'</div>';
    $element['#description'] = t('%style preview', array('%style' => $element['#title'])) .':<br />'. $preview;
    $markup .= drupal_render($element);
  }
  $build = array(
    'style' => array('#markup' => $markup),
    '#attached' => array('css' => array(drupal_get_path('module', 'quicktabs') . '/css/quicktabs-admin.css')),
  );
  return drupal_render($build);
}

/**
 * Clone QuickTabs.
 */
function quicktabs_clone($quicktab) {
  unset($quicktab->machine_name);
  $quicktab->title = '';
  return drupal_get_form('quicktabs_form', 'clone', $quicktab);
}

/**
 * Build the quicktab creation and edit form.
 */
function quicktabs_form($form, $form_state, $formtype, $quicktab = NULL) {
  if (!isset($quicktab)) {
    $quicktab = new stdClass;
  }
  $form = _quicktabs_admin_main_form($form_state, $quicktab);

  if (empty($quicktab->tabs)) {
    $quicktab->tabs = array(
      0 => array(),
      1 => array(),
    );
  }
  
  if (isset($form_state['num_tabs']) && $form_state['num_tabs'] > count($quicktab->tabs)) {
    $quicktab->tabs[] = array();
  }
  $form_state['num_tabs'] = count($quicktab->tabs);
  
  if (isset($form_state['to_remove'])) {
    unset($quicktab->tabs[$form_state['to_remove']]);
    unset($form_state['to_remove']);
    $form_state['num_tabs']--;
  }

  $tab_titles = array();
  // Add current tabs to the form.
  foreach ($quicktab->tabs as $delta => $tab) {
    $tab['delta'] = $delta;
    $form['qt_wrapper']['tabs'][$delta] = _quicktabs_form($tab);
    if (isset($tab['title'])) {
      $tab_titles[$delta] = $tab['title'];
    }
  }

  $form['default_tab'] = array(
    '#type' => 'select',
    '#title' => t('Default tab'),
    '#options' => $tab_titles,
    '#default_value' => isset($quicktab->default_tab) ? $quicktab->default_tab : 0,
    '#access' => !empty($tab_titles),
    '#weight' => -5,
  );

  return $form;
}

/**
 * The main section of admin page.
 */
function _quicktabs_admin_main_form($form_state, &$quicktab) {
  $form['#cache'] = TRUE;

  // The contents of $quicktab will either come from the db or from $form_state.
  if (isset($form_state['values']['title'])) {
    $quicktab = _quicktabs_convert_form_to_quicktab($form_state);
  }
  if (empty($quicktab->machine_name)) {
    $form['machine_name'] = array(
      '#title' => t('Machine Name'),
      '#type' => 'textfield',
      '#description' => t('A unique ID that will be used internally and in the CSS ID of your quicktabs block. must contain only lowercase letters, numbers, and underscores.'),
      '#default_value' => '',
      '#weight' => -8,
      '#required' => TRUE,
  );
  }
  else {
    $form['machine_name'] = array('#type' => 'value', '#value' => $quicktab->machine_name);
    $form['machine_name_display'] = array(
      '#title' => t('Machine-readable name'),
      '#type' => 'item',
      '#value' => "<code>{$quicktab->machine_name}</code>",
      '#description' => t('The machine name will be used internally by quicktabs and will be used in the CSS ID of your quicktabs block.'),
      '#weight' => -8,
    );
  }
  
  $form['title'] = array(
    '#title' => t('Block title'),
    '#description' => t('This will appear as the name of this block in administer >> site building >> blocks.'),
    '#type' => 'textfield',
    '#default_value' => isset($quicktab->title) ? $quicktab->title : '',
    '#weight' => -10,
  );

  $styles = module_invoke_all('quicktabs_tabstyles');
  $style_options = array();
  // The keys used for options must be valid html id-s.
  foreach ($styles as $style) {
    $style_options[$style] = $style;
  }
  ksort($style_options);
  $form['style'] = array(
    '#type' => 'select',
    '#title' => t('Style'),
    '#options' => array('nostyle' => t('No style')) + array('default' => t('Default style')) + $style_options,
    '#default_value' => isset($quicktab->style) ? $quicktab->style : 'default',
    '#description' => t('Choose the quicktab style.'),
    '#weight' => -8,
  );

  $form['ajax'] = array(
    '#type' => 'radios',
    '#title' => t('Ajax'),
    '#options' => array(
      TRUE => t('Yes') . ': ' . t('Load only the first tab on page view'),
      FALSE => t('No') . ': ' . t('Load all tabs on page view.'),
    ),
    '#default_value' => isset($quicktab->ajax) ? $quicktab->ajax : 0,
    '#description' => t('Choose how the content of tabs should be loaded.<p>By choosing "Yes", only the first tab will be loaded when the page first viewed. Content for other tabs will be loaded only when the user clicks the other tab. This will provide faster initial page loading, but subsequent tab clicks will be slower. This can place less load on a server.</p><p>By choosing "No", all tabs will be loaded when the page is first viewed. This will provide slower initial page loading, and more server load, but subsequent tab clicks will be faster for the user. Use with care if you have heavy views.</p>'),
    '#weight' => -6,
  );

  $form['hide_empty_tabs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide empty tabs'),
    '#default_value' => isset($quicktab->hide_empty_tabs) ? $quicktab->hide_empty_tabs : 0,
    '#description' => t('Empty and restricted tabs will not be displayed. Could be useful when the tab content is not accessible.<br />This option does not work in ajax mode.'),
    '#weight' => -4,
  );

  // Add a wrapper for the tabs and Add Another Tab button.
  $form['qt_wrapper'] = array(
    '#tree' => FALSE,
    '#weight' => -3,
    '#prefix' => '<div class="clear-block" id="quicktabs-tabs-wrapper">',
    '#suffix' => '</div>',
  );

  $form['qt_wrapper']['tabs'] = array(
    '#tree' => TRUE,
    '#prefix' => '<div id="quicktab-tabs">',
    '#suffix' => '</div>',
    '#theme' => 'quicktabs_admin_form_tabs',
  );

  $form['qt_wrapper']['tabs_more'] = array(
    '#type' => 'submit',
    '#prefix' => '<div id="add-more-tabs-button">',
    '#suffix' => '<label for="edit-tabs-more">' . t('Add tab') . '</label></div>',
    '#value' => t('More tabs'),
    '#attributes' => array('class' => array('add-tab'), 'title' => t('Click here to add more tabs.')),
    '#weight' => 1,
    '#submit' => array('qt_more_tabs_submit'),
    '#ajax' => array(
      'callback' => 'quicktabs_ajax_callback',
      'wrapper' => 'quicktab-tabs',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(),
  );

  $form['submit_form'] = array(
    '#type' => 'submit',
    '#weight' => 10,
    '#value' => t('Save'),
  );

  return $form;
}

/*
 * Build one row (one tabpage) on the QT admin form.
 */
function _quicktabs_form(array $tab) {
  $form['#tree'] = TRUE;
  $delta = $tab['delta'];

  $form['weight'] = array(
    '#type' => 'weight',
    '#default_value' => isset($tab['weight']) ? $tab['weight'] : $delta-100,
    '#delta' => 100,
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#size' => '10',
    '#default_value' => isset($tab['title']) ? $tab['title'] : '',
  );

  if (module_exists('block')) {
    $tabtypes['block'] = t('Block');
    $form['block']['bid'] = array(
      '#type' => 'select',
      '#options' => quicktabs_get_blocks(),
      '#default_value' => isset($tab['bid']) ? $tab['bid'] : '',
      '#title' => t('Select a block'),
    );
    $form['block']['hide_title'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide the title of this block'),
      '#default_value' => isset($tab['hide_title']) ? $tab['hide_title'] : 1,
    );
  }

  if (module_exists('views')) {
    $views = quicktabs_get_views();
    $views_keys = array_keys($views);
    $tabtypes['view'] = t('View!translate_as_list_of_content', array('!translate_as_list_of_content' => ''));

    $selected_view = (isset($tab['vid']) ? $tab['vid'] : (isset($views_keys[0]) ? $views_keys[0] : ''));
    $form['view']['vid'] = array(
      '#type' => 'select',
      '#options' => $views,
      '#default_value' => $selected_view,
      '#title' => t('Select a view'),
      '#ajax' => array(
        'callback' => '_quicktabs_replace_view_displays_callback',
      ),
    );
    $form['view']['display'] = array(
      '#type' => 'select',
      '#title' => 'display',
      '#options' => _quicktabs_get_views_displays($selected_view),
      '#default_value' => isset($tab['display']) ? $tab['display'] : '',
    );
    $form['view']['args'] = array(
      '#type' => 'textfield',
      '#title' => 'arguments',
      '#size' => '40',
      '#required' => FALSE,
      '#default_value' => isset($tab['args']) ? $tab['args'] : '',
      '#description' => t('Additional arguments to send to the view as if they were part of the URL in the form of arg1/arg2/arg3. You may use %0, %1, ..., %N to grab arguments from the URL.'),
    );
  }

  $tabtypes['node'] = t('Node');
  $form['node']['nid'] = array(
    '#type' => 'textfield',
    '#title' => t('Node'),
    '#description' => t('The node ID of the node.'),
    '#maxlength' => 10,
    '#size' => 20,
    '#default_value' => isset($tab['nid']) ? $tab['nid'] : '',
  );
  $form['node']['teaser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Teaser view'),
    '#default_value' => isset($tab['teaser']) ? $tab['teaser'] : 0,
  );
  $form['node']['hide_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide the title of this node'),
    '#default_value' => isset($tab['hide_title']) ? $tab['hide_title'] : 1,
  );

  $tabtypes['qtabs'] = t('QTab');
  $form['qtabs']['machine_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Quicktab'),
    '#description' => t('The quicktab machine name.') . ' ' . t('Different styles may not work when putting an ajax quicktab inside ajax quicktab.'),
    '#maxlength' => 10,
    '#size' => 20,
    '#default_value' => isset($tab['machine_name']) ? $tab['machine_name'] : '',
  );

  $form['type'] = array(
    '#type' => 'radios',
    '#options' => $tabtypes,
    '#default_value' => isset($tab['type']) ? $tab['type'] : key($tabtypes),
  );

  $form['remove'] = array(
    '#type' => 'submit',
    '#prefix' => '<div>',
    '#suffix' => '<label for="edit-remove">' . t('Delete') . '</label></div>',
    '#value' => 'remove_' . $delta,
    '#attributes' => array('class' => array('delete-tab'), 'title' => t('Click here to delete this tab.')),
    '#submit' => array('qt_remove_tab_submit'),
    '#ajax' => array(
      'callback' => 'quicktabs_ajax_callback',
      'wrapper' => 'quicktab-tabs',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#limit_validation_errors' => array(),
  );

  return $form;
}

/**
 * Theme function for quicktabs admin page.
 * Theme the form elements for the tabs as draggable table rows.
 *
 * @ingroup themeable
 */
function theme_quicktabs_admin_form_tabs($variables) {
  $tabs = $variables['tabs'];

  drupal_add_tabledrag('qt-tablist-table', 'order', 'sibling', 'qt-tabs-weight');
  $rows = array();
  $header = array(
    t('Tab title'),
    t('Tab weight'),
    t('Tab type'),
    t('Tab content'),
    t('Operations'),
  );

  foreach (element_children($tabs) as $key) {
    $tab = &$tabs[$key];
    $tab['weight']['#attributes']['class'] = array('qt-tabs-weight');
    // Build the table row.
    $row = array(
      'data' => array(
        array('data' => drupal_render($tab['title']), 'class' => array('qt-tab-title')),
        array('data' => drupal_render($tab['weight']), 'class' => array('qt-tab-weight')),
        array('data' => drupal_render($tab['type']), 'class' => array('qt-tab-type')),
        // tab content (only 1 tab content (block, node or view) will be displayed. see: quicktabs_form.js)
        array('data' => (module_exists('block') ? drupal_render($tab['block']) : ''), 'class' => array('qt-tab-content', 'qt-tab-block-content')),
        array('data' => (module_exists('views') ? drupal_render($tab['view']) : ''), 'class' => array('qt-tab-content', 'qt-tab-view-content')),
        array('data' => drupal_render($tab['node']), 'class' => array('qt-tab-content', 'qt-tab-node-content')),
        array('data' => drupal_render($tab['qtabs']), 'class' => array('qt-tab-content', 'qt-tab-qtabs-content')),
        array('data' => drupal_render($tab['remove']), 'class' => array('qt-tab-remove')),
      ),
      'class' => array('draggable'),
    );

    // Add additional attributes to the row, such as a class for this row.
    if (isset($tab['#attributes'])) {
      $row = array_merge($row, $tab['#attributes']);
    }
    $rows[] = $row;
  }

  $build['quicktab'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'qt-tablist-table'),
    '#weight' => -1,
  );
  $build['#attached']['css'][] = drupal_get_path('module', 'quicktabs') . '/css/quicktabs-admin.css';
  $build['#attached']['js'][] = drupal_get_path('module', 'quicktabs') . '/js/quicktabs_form.js';

  $output = drupal_render($build);
  return $output;
}

/**
 * Ajax callback for add more and remove buttons.
 */
function quicktabs_ajax_callback($form, $form_state) {
  $form_tabs = $form['qt_wrapper']['tabs'];
  return $form_tabs;
}

/**
 * Ajax callback, triggered when view is changed.
 */
function _quicktabs_replace_view_displays_callback($form, $form_state) {
  $view_name = $form_state['triggering_element']['#value'];
  $delta = $form_state['triggering_element']['#parents'][1];
  $display_options = _quicktabs_get_views_displays($view_name);
  $form['qt_wrapper']['tabs'][$delta]['view']['display']['#options'] = $display_options;

  $commands = array();
  // Replace the view display dropdown.
  $commands[] = ajax_command_replace("#view-display-dropdown-$delta", drupal_render($form['qt_wrapper']['tabs'][$delta]['view']['display']));
  return array('#type' => 'ajax', '#commands' => $commands);
}


/**
 * Submit handler for the "Add Tab" button.
 */
function qt_more_tabs_submit($form, &$form_state) {
  // Increment the number of tabs to be rendered.
  $form_state['num_tabs']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "Remove Tab" button.
 */
function qt_remove_tab_submit($form, &$form_state) {
  // Get the tab delta for the clicked button.
  $delta = $form_state['clicked_button']['#parents'][1];
  $form_state['to_remove'] = $delta;
  $form_state['rebuild'] = TRUE;
}

/**
 * Validation handler for quicktabs admin page.
 */
function quicktabs_form_validate($form, &$form_state) {
  if (empty($form_state['values']['machine_name'])) {
    form_set_error('machine_name', t('The quicktabs ID is required.'));
  }
  else if (!preg_match('!^[a-z0-9_]+$!', $form_state['values']['machine_name'])) {
    form_set_error('machine_name', t('The quicktabs machine name must contain only lowercase letters, numbers, and underscores.'));
  }
  if (empty($form_state['values']['title'])) {
    form_set_error('title', t('Title is required for the quicktab block.'));
  }
  else {
    if (!isset($form_state['values']['tabs'])) {
      form_set_error('', t('At least one tab should be created.'));
    }
    foreach ($form_state['values']['tabs'] as $j => $tab) {
      if (empty($tab['title'])) {
        form_set_error('tabs]['. $j .'][title', t('Title is required for each tab.'));
      }
      elseif ($tab['type'] == 'qtabs' && $tab['qtabs']['machine_name'] == $form_state['values']['machine_name']) {
        form_set_error('tabs]['. $j .'][qtabs][machine_name', t('You cannot put a quicktab inside itself.'));
      }
      elseif ($tab['type'] == 'view') {
        // Remove spaces from view arguments.
        $form_state['values']['tabs'][$j]['view']['args'] = str_replace(' ', '', $form_state['values']['tabs'][$j]['view']['args']);
      }
    }
  }
}

/**
 * Submit handler for quicktabs admin page.
 */
function quicktabs_form_submit($form, &$form_state) {
  // We don't want it to submit when we're just adding or removing tabs.
  $quicktab = _quicktabs_convert_form_to_quicktab($form_state);

  $exists = quicktabs_load($quicktab->machine_name);
  if ($exists && empty($exists->in_code_only)) {
    $ret = drupal_write_record('quicktabs', $quicktab, 'machine_name');
    if ($ret == SAVED_UPDATED) {
      drupal_set_message(t('The quicktab block has been updated.'));
    }
  }
  else {
    $ret = drupal_write_record('quicktabs', $quicktab);
    if ($ret == SAVED_NEW) {
      drupal_set_message(t('The quicktab block has been created.'));
    }
  }
  drupal_goto('admin/structure/quicktabs');
}

/**
 * Deletion of quicktab block.
 */
function quicktabs_block_delete($form, $form_state, $quicktab) {
  $form['machine_name'] = array('#type' => 'hidden', '#value' => $quicktab->machine_name);
  $form['title'] = array('#type' => 'hidden', '#value' => $quicktab->title);
  return confirm_form($form, t('Are you sure you want to delete the quicktab block %title?', array('%title' => $quicktab->title)), 'admin/structure/quicktabs', '', t('Delete'), t('Cancel'));

}

/**
 * Submit handler for quicktab block deletion.
 */
function quicktabs_block_delete_submit($form, &$form_state) {
  db_query('DELETE FROM {quicktabs} WHERE machine_name = :machine_name', array(':machine_name' => $form_state['values']['machine_name']));
  drupal_set_message(t('The quicktab block %name has been removed.', array('%name' => $form_state['values']['title'])));
  cache_clear_all();
  $form_state['redirect'] = 'admin/structure/quicktabs';
}

/**
 * Export form for quicktabs.
 */
function quicktabs_export_form(&$form_state, $quicktab) {
  ctools_include('export');

  // Generate export code
  $code = '$items = array();' ."\n";
  $code .= ctools_export_object('quicktabs', $quicktab, '');
  $code .= '$items["'. $quicktab->machine_name .'"] = $quicktab;' ."\n";
  $code .= 'return $items;';

  // Create form
  $form = array();
  $form['export'] = array(
    '#type' => 'textarea',
    '#default_value' => $code,
    '#rows' => substr_count($code, "\n") + 1,
    '#resizable' => FALSE,
    '#description' => t('Place this code in your module\'s implementation of <code>hook_quicktabs_default_quicktabs()</code> to provide it as a default quicktab.'),
  );
  $form['done'] = array(
    '#type' => 'submit',
    '#value' => t('Done'),
  );
  $form['#redirect'] = 'admin/structure/quicktabs';

  return $form;  
}


/**
 * Helper function to get all blocks.
 */
function quicktabs_get_blocks() {
  static $blocksarray;
  if (empty($blocksarray)) {
    $blocks = _block_rehash();
    $blocksarray = array();
    foreach ($blocks as $block) {
      if ($block['module'] != 'quicktabs') {
        $key = $block['module'] . '_delta_' . $block['delta'];
        $blocksarray[$key] = $block['info'] . ' (' . $block['module'] . ':' . $block['delta'] . ')';
      }
    }
  }
  return $blocksarray;
}


/**
 * Helper function to get all views.
 */
function quicktabs_get_views() {
  $enabled_views = array();
  $views = views_get_all_views();

  foreach ($views as $view) {
    // Skip disabled views.
    if (!empty($views[$view->name]->disabled)) {
      continue;
    }
    $enabled_views[$view->name] = $view->name;
  }
  ksort($enabled_views);
  return $enabled_views;
}

/**
 * Helper function to get all view displays.
 */
function _quicktabs_get_views_displays($view_name) {
  $displays = array();
  if (empty($view_name)) {
    // No view.
    return $displays;
  }

  $views = views_get_all_views();
  $view = $views[$view_name];

  if (empty($view->display)) {
    // This view is broken.
    return $displays;
  }

  foreach ($view->display as $id => $display) {
    $displays[$id] = $id .': '. $display->display_title;
  }
  return $displays;
}


/**
 * Helper function to convert the data on admin form into quicktab presentation.
 */
function _quicktabs_convert_form_to_quicktab($form_state) {
  $formvalues_tabs = array();
  if (!empty($form_state['values']['tabs'])) {
    foreach ($form_state['values']['tabs'] as $j => $tab) {
      $formvalues_tabs[$j] = $tab[$tab['type']];
      $formvalues_tabs[$j]['title'] = $tab['title'];
      $formvalues_tabs[$j]['weight'] = $tab['weight'];
      $formvalues_tabs[$j]['type'] = $tab['type'];
      $weight[$j] = $tab['weight'];
    }
    array_multisort($weight, SORT_ASC, $formvalues_tabs);
  }
  $quicktab = new stdClass();
  $quicktab->title = $form_state['values']['title'];
  $quicktab->ajax = $form_state['values']['ajax'];
  $quicktab->default_tab = isset($form_state['values']['default_tab']) ? $form_state['values']['default_tab'] : 0;
  $quicktab->hide_empty_tabs = $form_state['values']['hide_empty_tabs'];
  $quicktab->style = $form_state['values']['style'];
  $quicktab->tabs = $formvalues_tabs;

  if (isset($form_state['values']['machine_name'])) {
    $quicktab->machine_name = $form_state['values']['machine_name'];
  }

  return $quicktab;
}

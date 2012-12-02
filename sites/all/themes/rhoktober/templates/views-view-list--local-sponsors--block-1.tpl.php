<?php
/**
 * @file views-view-list.tpl.php
 * Default simple view template to display a list of rows.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $options['type'] will either be ul or ol.
 * @ingroup views_templates
 */

  /**
   * This groups the local sponsors by event group, which
   * they may be connected to either by direct reference
   * or by reference to local event referencing the group.
   * It's a little complicated.
   */

  $rows_by_event_group = array();
  $events_by_nid = array();

  foreach ($view->result as $id => $item) {
  
    $event_nid = $item->_field_data['nid']['entity']->field_event_or_group['und'][0]['nid'];
    
    if ($event_nid > 0) {

      $event = node_load($event_nid);

      if ($event->type == 'event') {
        $event = node_load($event->field_event_group['und'][0]['nid']);
      } // if
      
      if ($event->type == 'event_group') {

        $events_by_nid[$event->nid] = $event;

        if (!isset($rows_by_event_group[$event->nid])) {
          $rows_by_event_group[$event->nid] = array();
        } // if

        $rows_by_event_group[$event->nid][] = $rows[$id];

      } // if

    } // if

  } // foreach

?>
<?php print $wrapper_prefix; ?>
  <?php if (!empty($title)) : ?>
    <h3><?php print $title; ?></h3>
  <?php endif; ?>
  <?php
  
    foreach ($rows_by_event_group as $group_nid => $rows) {
  
      $event = $events_by_nid[$group_nid];
  
      ?>
      <div class="event-group">
        <h4><?php print l($event->title, 'node/' . $event->nid); ?></h4>
        <?php print $list_type_prefix; ?>
          <?php foreach ($rows as $id => $row): ?>
            <li class="<?php print $classes_array[$id]; ?>"><?php print $row; ?></li>
          <?php endforeach; ?>
        <?php print $list_type_suffix; ?>
      </div>
      <?
  
    } // foreach
  
  ?>
<?php print $wrapper_suffix; ?>
<?php
/**
 * @file scrollable-content-view.tpl.php
 *  Display content in 'scrollable content' block using jQuery.
 *
 * - $rows: Array of rows (resulted rows from the view).
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @see template_preprocess_scrollable_content_view()
 * @ingroup views_templates
 */
?>

<ul id="scrollable_content_<?php print $options['items']; ?>_navi" class="main_navi <?php print $classes; ?>">
<?php
foreach ($rows as $row) {
?>
  <li class="<?php print $classes; ?>">
    <?php print $row; ?>
  </li>
<?php
}
?>
</ul>

<div id="scrollable_content_wrapper_<?php print $options['items']; ?>">
  <div class="navi <?php print $classes; ?>"></div>

  <a class="prev scrollable_content_prev <?php print $classes; ?>"></a>

  <div id="scrollable_content_<?php print $options['items']; ?>" class="scrollable_content <?php print $options['vertical'] == 1 ? 'vertical' : 'horizontal'; ?> <?php print $classes; ?>">
    <div id="scrollable_content_thumbs_<?php print $options['items']; ?>" class="items" style="left: 0px; <?php print $classes; ?>">
      <?php
      foreach ($rows as $row) {
      ?>
      <div class="slide <?php print $classes; ?>">
        <?php print $row; ?>
      </div>
      <?php
      }
      ?>
    </div>
  </div>

  <a class="next scrollable_content_next <?php print $classes; ?>"></a>

  <br clear="all"/>
</div>
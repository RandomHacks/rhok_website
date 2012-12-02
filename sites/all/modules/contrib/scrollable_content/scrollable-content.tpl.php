<?php

/**
 * @file scrollable-content.tpl.php
 * Default theme implementation for Scrollable Content block.
 *
 * Available variables:
 * - $slides
 * - $direction
 *
 * @see template_preprocess_scrollable_content()
 */
?>

<ul id="scrollable_content_items_navi" class="main_navi <?php print $classes; ?>">
<?php
foreach ($slides as $slide) {
?>
  <li class="<?php print $classes; ?>">
    <img title="<?php print $slide['title']; ?>" alt="<?php print $slide['title']; ?>" src="<?php print $slide['image']; ?>" class="<?php print $classes; ?>"/>
    <span><?php print $slide['title']; ?></span>
  </li>
<?php
}
?>
</ul>

<div id="scrollable_content_wrapper_items">
  <div class="navi <?php print $classes; ?>"></div>

  <a class="prev scrollable_content_prev <?php print $classes; ?>"></a>
  <div id="scrollable_content_items" class="scrollable_content <?php print $direction; ?> <?php print $classes; ?>">
    <div id="scrollable_content_thumbs_items" class="items <?php print $classes; ?>" style="left: 0px;">
      <?php
      foreach ($slides as $slide) {
      ?>
      <div class="slide <?php print $classes; ?>">
        <a href="<?php print $slide['path']; ?>" title="<?php print $slide['path']; ?>" class="<?php print $classes; ?>">
	  <?php if (!empty($slide['image'])): ?>
	    <img title="<?php print $slide['title']; ?>" alt="<?php print $slide['title']; ?>" src="<?php print $slide['image']; ?>" class="<?php print $classes; ?>"/>
	  <?php	endif; ?>
          <h3 class="<?php print $classes; ?>"><?php print $slide['title']; ?></h3>
        </a>
      </div>
      <?php
      }
      ?>
    </div>
  </div>

  <a class="next scrollable_content_next <?php print $classes; ?>"></a>

  <br clear="all"/>
</div>
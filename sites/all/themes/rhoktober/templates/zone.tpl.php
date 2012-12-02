<?php if ($wrapper): ?><div<?php print $attributes; ?>><?php endif; ?>  
<div class="zoneinnerwrapper">
  <div<?php print $content_attributes; ?>>    
    <?php if ($breadcrumb): ?>
      <div id="breadcrumb" class="grid-<?php print $columns; ?>"><?php print $breadcrumb; ?></div>
    <?php endif; ?>    
    <?php if ($messages): ?>
      <div id="messages" class="grid-<?php print $columns; ?>"><?php print $messages; ?></div>
    <?php endif; ?>
<div class="zoneblockwrapper">
<div class="clr"></div>
<?php print $content; ?>
<div class="clr"></div>
</div>
  </div>
</div>
<?php if ($wrapper): ?></div><?php endif; ?>

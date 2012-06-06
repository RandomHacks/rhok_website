<div class="members"><span class="number"><?php print number_format($members); ?></span> <?php print l(format_plural($members, 'Member', 'Members'), 'community'); ?></div>
<div class="problems"><span class="number"><?php print number_format($problems); ?></span> <?php print l(format_plural($problems, 'Problem', 'Problems'), 'problems'); ?></div>
<div class="events"><span class="number"><?php print number_format($events); ?></span> <?php print l(format_plural($events, 'Event', 'Events'), 'events'); ?></div>
<div class="solutions"><span class="number"><?php print number_format($solutions); ?></span> <?php print l(format_plural($solutions, 'Solution', 'Solutions'), 'solutions'); ?></div>

<?php

/**
 * @file
 * widget.tpl.php
 *
 * UpDown widget theme for Vote Up/Down
 */
?>
<div class="vud-widget vud-widget-updown" id="<?php print $id; ?>">
  <div class="updown-score">
    <span class="updown-current-score"><?php print $unsigned_points; ?></span>
    <?php //print $vote_label; ?>
  </div>
  <?php if ($show_links): ?>
    <?php if ($show_up_as_link): ?>
      <a href="<?php print $link_up; ?>" rel="nofollow" class="<?php print $link_class_up; ?>">
    <?php endif; ?>
        <div class="<?php print $class_up; ?> updown-up" title="<?php print t('Vote up!'); ?>"><img class="vote-up" src="/<?php print drupal_get_path('theme', 'bmj')  ?>/images/pixel.gif" /><span>+</span></div>
        <div class="element-invisible"><?php print t('Vote up!'); ?></div>
    <?php if ($show_up_as_link): ?>
      </a>
    <?php endif; ?>

  <?php endif; ?>
</div>

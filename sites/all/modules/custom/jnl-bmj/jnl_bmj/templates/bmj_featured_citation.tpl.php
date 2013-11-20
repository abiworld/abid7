<div class="bmj-channel-featured bmj-channel-featured-<?php print $mode; ?> clear-block">
  <?php if ($mode == 'hero' || $mode == 'photo'): ?>
    <div class="bmj-channel-featured-<?php print $mode; ?>-wrapper clear-block">
  <?php endif; ?>

    <?php if ($image): ?>
      <?php print $image; ?>
    <?php endif; ?>

    <?php if ($kicker): ?>
      <h3 class="channel-featured-kicker bmj-channel-featured-<?php print $mode; ?>-kicker">
        <?php print $kicker; ?>
      </h3>
    <?php endif; ?>

    <?php if ($mode == 'homepage_slideshow'): ?>
      <div class="bmj-channel-featured-<?php print $mode; ?>-wrapper clear-block">
    <?php endif; ?>

    <?php if ($title): ?>
      <h2 class="channel-featured-title bmj-channel-featured-<?php print $mode; ?>-title">
        <?php print $title; ?>
      </h2>
    <?php endif; ?>

  <?php if ($mode == "hero"): ?>
    </div>
  <?php endif; ?>

  <?php // print theme('bmj_admin_links', array('node' => $node)); ?>

  <?php if ($intro) : ?>
    <div class="bmj-channel-featured-intro bmj-channel-featured-<?php print $mode; ?>-intro">
      <?php print $intro; ?>
    </div>
  <?php endif; ?>

  <?php if ($date || $category_links): ?>
    <div class="bmj-channel-featured-date-category-wrapper
                bmj-channel-featured-<?php print $mode; ?>-date-category-wrapper">
  <?php endif; ?>

  <?php if ($date): ?>
    <span class="bmj-channel-featured-date bmj-channel-featured-<?php print $mode; ?>-date">
      <?php if ($mode == 'editors_choice'): ?>
        <span class="date-published">Published </span>
      <?php endif; ?>
      <?php print $date; ?>
    </span>
  <?php endif; ?>

  <?php if ($category_links): ?>
    <span class="bmj-channel-featured-category-links
                bmj-channel-featured-<?php print $mode; ?>-category-links">
      in: <?php print $category_links; ?>
    </span>
  <?php endif; ?>

  <?php if ($date || $category_links): ?>
    </div>
  <?php endif; ?>

  <?php if ($mode == 'homepage_slideshow'): ?>
    <div class="bmj-channel-featured-<?php print $mode; ?>-more-link clear-block">
      <?php print $more_link; ?>
    </div>
  <?php endif; ?>

  <?php if ($mode == 'photo' || $mode == 'homepage_slideshow'): ?>
    </div>
  <?php endif; ?>
</div>


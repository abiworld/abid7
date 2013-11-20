<?php
/*
 * Highwire citation template for Article List 2 style
 * Title
 * Date
 * Article Category
 */
?>
<cite class='highwire-cite highwire-citation-bmj-article-list'>
  <?php // print theme('bmj_admin_links', array('node' => $node)); ?>
  
  <?php if ($title): ?>
    <div class='highwire-cite-title'><?php print $title; ?></div>
  <?php endif; ?>

  <?php if ($date || $category_links): ?>
    <div class="bmj-article-list-2-date-category-wrapper">
  <?php endif; ?>

  <?php if ($date): ?>
    <span class="bmj-article-list-2-date"><?php print $date; ?></span>
  <?php endif; ?>

  <?php if ($category_links): ?>
    <span class='bmj-category-links'>
      in: <?php print $category_links; ?>
    </span>
  <?php endif; ?>

  <?php if ($date || $category_links): ?>
    </div>
  <?php endif; ?>
</cite>

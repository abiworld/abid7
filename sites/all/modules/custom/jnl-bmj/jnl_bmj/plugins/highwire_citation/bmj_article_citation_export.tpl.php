<?php
/*
 * Authors. Title Journal Year; Volumn::<part of pisa>
 */
?>
<div <?php print $citation_attributes ?> >
  <?php if ($access): ?>
    <div <?php print $access_attributes ?> ><?php print $access; ?></div>
  <?php endif; ?>

  <?php if ($authors): ?>
    <span <?php print $authors_attributes ?> ><?php print $authors; ?></span>
  <?php endif; ?>

  <?php if ($title): ?>
    <<?php print $title_element; ?><?php print $title_attributes; ?> >
      <?php print $title; ?>
    </<?php print $title_element; ?>>
  <?php endif; ?>

  <?php if ($subtitle): ?>
    <<?php print $subtitle_element; ?><?php print $subtitle_attributes; ?>>
      <?php print $subtitle; ?>
    </<?php print $subtitle_element; ?>>
  <?php endif; ?>

  <?php if ($metadata): ?>
    <span <?php print $metadata_attributes ?> ><?php print $metadata; ?></span>
  <?php endif; ?>

  <?php if ($snippet): ?>
    <div <?php print $snippet_attributes ?> ><?php print $snippet; ?></div>
  <?php endif; ?>

  <?php if ($extras): ?>
    <div <?php print $extras_attributes ?> ><?php print $extras; ?></div>
  <?php endif; ?>
</div>

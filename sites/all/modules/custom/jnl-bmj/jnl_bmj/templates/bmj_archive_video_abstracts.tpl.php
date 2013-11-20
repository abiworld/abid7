<?php if ($video_link): ?>
  <div class = "video-link">
    <?php print $video_link; ?>
  </div>
<?php endif; ?>

<div class = "video-title-desc">
  <?php if ($video_title): ?>
    <div class = "video-title">
      <?php print $video_title; ?>
    </div>
  <?php endif; ?>
  <?php if ($video_desc): ?>
    <div class = "video-description">
      <?php print $video_desc; ?>
    </div>
  <?php endif; ?>
</div>

<?php
  print '<div id="' . $modal_id . '" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="' . $modal_label . '" aria-hidden="true">';
?>
<?php if ($modal_header): ?>
<div class="modal-header">
  <?php print $modal_header; ?>
</div>
<?php endif; ?>
<?php if ($modal_body): ?>
<div class="modal-body">
  <?php print $modal_body; ?>
</div>
<?php endif; ?>
<?php if ($modal_footer): ?>
<div class="modal-footer">
  <?php print $modal_footer; ?>
</div>
<?php endif; ?>
</div>

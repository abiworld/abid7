<div class="node node-rapid-response clear-block state-<?php echo $state;?>">
  <div class="clear-block">
    <div class="grid_6 first">
      <h2><?php print $title?></h2>
    </div>
    <div class="grid_2 last">
      <h4><?php print $formated_date ?></h4>
    </div>
  </div>

  <div class="responses-title clear-block">

    <div class="grid_6 first">

    <?php if ($show_parent_info): ?>
      <p class="responses-article-title">Re: <span class="<?php print $parent_unpub_class; ?>article-title"><?php print $parent_unpub_text; ?><?php print $parent_link;?></span>.
      <span class="author-names"><?php print $parent_authors; ?></span>
      <span class="doi"><?php print $parent_volume; ?>:doi:</span><span class="doi-details"><?php print $parent_doi; ?></span></p>
    <?php endif; ?>


    <?php print $text; ?>
    </div>

    <div class="grid_2 last">
      <div class="author">
        <p><?php print $authors; ?></p>
      </div>
      <?php if ($show_parent_info) { ?>
      <div class="response-links">
        <ul>
          <li><?php print l('Respond to this article', 'node/'.$parent->nid, array('query' => array('tab' => 'response-form'))); ?></li>
          <li class="last"><?php print l('Read all responses for this article', 'node/'.$parent->nid, array('query' => array('tab' => 'responses'))); ?></li>
        </ul>
      </div>
    <?php } ?>
      <div class="vote-widget">
        <p><strong>Click to like:</strong></p>
        <?php
          $node_view = node_view($node);
          print $node_view['vud_node_widget']['#markup']; ?>
      </div>
    </div>
  </div>
</div>

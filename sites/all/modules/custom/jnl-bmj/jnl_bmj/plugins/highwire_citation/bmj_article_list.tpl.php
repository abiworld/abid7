<?php
/*
 * Highwire citation template for highwire_classic style
 * Article type
 * Title -bold
 * Authors
 * Journal pap date(m-y), vol(issue no):pages; doi
 */
?>
<cite class='highwire-cite highwire-citation-bmj-article-list'>
  <?php if (isset($article_type)): ?>
    <span class='highwire-cite-article-type'><?php print $article_type; ?></span>
  <?php endif; ?>

  <?php if ($title): ?>
  <div class='highwire-cite-title'><?php print $title; ?></div>
  <?php endif; ?>

  <span class='highwire-cite-year'>Published: <?php print $date; ?></span>
  <span class='highwire-cite-journal'><?php print $journal_title; ?></span>
  <span class='highwire-cite-volume-issue'><?php print $volume; ?></span>
  <span class='highwire-cite-doi'> doi:<?php print $doi?></span>

  <?php if ($pap_label): ?>
  <span class='highwire-cite-pap-article'><?php print $pap_label; ?></span>
  <?php endif; ?>
</cite>

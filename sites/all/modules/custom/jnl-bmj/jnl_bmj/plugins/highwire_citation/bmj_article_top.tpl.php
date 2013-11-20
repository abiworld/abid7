<?php
/*
 * Highwire citation template for highwire_classic style
 * Article type
 * Title
 * Journal Year; Volumn DOI link (Published <Date>)
 * Cite this as: Journal Year; Volumn:<part of pisa>
 */
?>
<cite class='highwire-cite highwire-citation-bmj-article-top'>
  <?php if (isset($article_type)): ?>
    <span class='highwire-cite-article-type'><?php print $article_type; ?></span>
  <?php endif; ?>

  <?php if ($title): ?>
    <<?php print $title_element; ?><?php print $title_attributes; ?> >
      <?php print $title; ?>
    </<?php print $title_element; ?>>
  <?php endif; ?>

  <span class='highwire-cite-journal'><?php print $journal_title; ?></span>
  <span class='highwire-cite-published-year'><?php print $year; ?></span>;
  <span class='highwire-cite-volume-issue'><?php print $volume; ?></span>
  <span class='highwire-cite-doi'> doi: <?php print $doi?></span>
  <span class='highwire-cite-year'>(Published <?php print $date; ?>)</span>
  <span class='highwire-cite-article-as'>
    Cite this as: <?php print $journal_title . ' ' . $year . '; ' . $volume  . $fpage; ?>
  </span>

  <?php if ($pap_label): ?>
  <span class='highwire-cite-pap-article'><?php print $pap_label; ?></span>
  <?php endif; ?>

</cite>

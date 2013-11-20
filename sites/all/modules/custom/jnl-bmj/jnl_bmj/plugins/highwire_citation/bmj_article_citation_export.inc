<?php
/*
 * BMJ citation template for BMJ Article: Citation Export
 * Authors. Title Journal Year; Volumn::<part of pisa>
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("BMJ Article for citation export"),
  'description' => t('Authors. Title Journal Year; Volumn::<part of pisa>'),
  'preprocess' => 'bmj_citation_style_bmj_article_citation_export',
  'theme' => 'highwire_citation_style_bmj_article_citation_export',
  'template' => 'plugins/highwire_citation/bmj_article_citation_export',
);

/**
 * Preprocessor function to process the type of article and sees if it a pap or not
 */
function bmj_citation_style_bmj_article_citation_export(&$variables) {

  $node = $variables['node'];

  $variables['title_element'] = 'span';
  $variables['title_attributes_array']['class'] = 'article-title';

  $authors_xml = $variables['authors_xml'];

  $authors = array();
  $count = 0;
  $et_all = '';
  foreach ($authors_xml as $key => $author) {
    $count++;
    $authors[] = theme('highwire_citation_author', array('xml' => $author, 'delta' => $key, 'pattern' => 's G'));
    if ($count == 6) {
      $et_all = ' et al';
      break;
    }
  }
  $author_string = implode(', ', $authors);
  $author_string = !empty($author_string) ? $author_string . $et_all . '. ': '';

  $variables['authors'] = $author_string;

  // if title is missing then show toc section/article_cat as a title
  $variables['title'] = empty($variables['title']) ? (!empty($variables['toc_section']) ? $variables['toc_section'] : '') : $variables['title'];

  $variables['date'] = date('d F Y', strtotime($variables['date']));

  $page_string = !empty($variables['fpage']) ? ':' . $variables['fpage'] : '';

  // @TODO remove following code once fpage got updated for content
  // get the last part of pisa
  // e.g. pisa is bmj;346/apr18_2/f2310  we need f2310
  if (empty($variables['fpage'])) {
    $pisa = $variables['pisa'];
    $pisa = explode('/', $pisa);
    $pisa_part = array_pop($pisa);
    $page_string = ":" . $pisa_part;
  }

  $variables['metadata'] = array(
    'journal_title' => array(
      '#id' => 'journal',
      '#markup' => $variables['journal_title'] . ' ',
      '#theme' => 'highwire_article_citation_metadata',
    ),
    'year' => array(
      '#id' => 'date',
      '#markup' => !empty($variables['year']) ? $variables['year'] . '; ' : '',
      '#theme' => 'highwire_article_citation_metadata',
    ),
    'volume' => array(
      '#id' => 'volume',
      '#markup' => $variables['volume'],
      '#theme' => 'highwire_article_citation_metadata',
    ),
    'page-string' => array(
      '#id' => 'volume',
      '#markup' => $page_string,
      '#theme' => 'highwire_article_citation_metadata',
    ),
  );
}


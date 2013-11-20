<?php
/*
 * BMJ citation template for BMJ Article Top A
 * Article type
 * <b>Title of Article<b>
 * Journal Year; Volumn DOI link (Published <Date>)
 * Cite this as: Journal Year; Volumn:<part of pisa>
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("BMJ Article Top A"),
  'description' => t('Article type \n<b>Title of Article<b>\nJournal Year; Volumn DOI link (Published <Date>)\nCite this as: Journal Year; Volumn:<part of pisa>'),
  'theme' => 'highwire_citation_style_bmj_article_top',
  'template' => 'plugins/highwire_citation/bmj_article_top',
  'preprocess' => 'bmj_citation_style_bmj_article_top'
);

/**
 * Preprocessor function to process the type of article and sees if it a pap or not
 */
function bmj_citation_style_bmj_article_top(&$variables) {

  $node = $variables['node'];
  // The title should display as the page title (h1#page-title)
  $variables['title_element'] = 'h1';
  $variables['title_attributes_array']['id'] = 'page-title';

  // if title is missing then show toc section/article_cat as a title
  $variables['title'] = empty($variables['title']) ? (!empty($variables['toc_section']) ? $variables['toc_section'] : '') : $variables['title'];


  $article_type = $node->field_highwire_article_type[X][0]['value'];
  if ($article_type) {
    $variables['article_type'] = ucwords(str_replace('-', " ", $article_type));
  }
  $variables['date'] = date('d F Y', strtotime($variables['date']));

  $article_version = $variables['node']->field_highwire_article_ver[X][0]['value'];
  $early_article = $variables['node']->field_highwire_article_early[X][0]['value'];

  // Updating doi to have url (not link)
  $variables['doi'] = 'http://dx.doi.org/' . $variables['doi'];

  $variables['fpage'] = !empty($variables['fpage']) ? ':' . $variables['fpage'] : '';

  // @TODO remove following code once fpage got updated for content
  // get the last part of pisa
  // e.g. pisa is bmj;346/apr18_2/f2310  we need f2310
  if (empty($variables['fpage'])) {
    $pisa = $variables['pisa'];
    $pisa = explode('/', $pisa);
    $pisa_part = array_pop($pisa);
    $variables['fpage'] = ":" . $pisa_part;
  }

  $variables['pap_label'] = '';
  if (($article_version == 'curr') &&  $early_article) {
    $variables['pap_label'] = 'published ahead of print';
  }
}

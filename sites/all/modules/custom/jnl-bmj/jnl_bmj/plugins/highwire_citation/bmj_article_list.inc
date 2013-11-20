<?php
/*
 * BMJ citation template for BMJ Article List
 * Article type
 * <b>Title of Article<b>
 * Published date Journal Name Volumn doi
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("BMJ Article list"),
  'description' => t('Article type \n<b>Title of Article<b>\nJournal date(d F Y;), vol; doi'),
  'theme' => 'highwire_citation_style_bmj_article_list',
  'template' => 'plugins/highwire_citation/bmj_article_list',
  'preprocess' => 'bmj_citation_style_bmj_article_list'
);

/**
 * Preprocessor function to process the type of article and sees if it a pap or not
 */
function bmj_citation_style_bmj_article_list(&$variables) {

  $node = $variables['node'];
  $article_type = $node->field_highwire_article_type[X][0]['value'];
  if ($article_type) {
    $variables['article_type'] = ucwords(str_replace('-', " ", $article_type));
  }

  $variables['date'] = date('d F Y;', strtotime($node->field_highwire_a_ppubdate[X][0]['value']));

  $article_version = $variables['node']->field_highwire_article_ver[X][0]['value'];
  $early_article = $variables['node']->field_highwire_article_early[X][0]['value'];

  $variables['pap_label'] = '';
  if (($article_version == 'curr') &&  $early_article) {
    $variables['pap_label'] = 'published ahead of print';
  }
}

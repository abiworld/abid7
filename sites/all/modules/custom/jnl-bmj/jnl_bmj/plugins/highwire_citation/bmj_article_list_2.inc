<?php
/*
 * BMJ citation template for BMJ Article List 2
 * Title of Article
 * Published date
 * Article Categories
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("BMJ Article list 2"),
  'description' => t('Article type \n<b>Title of Article<b>\nJournal date(d F Y;), Tag'),
  'theme' => 'highwire_citation_style_bmj_article_list_2',
  'template' => 'plugins/highwire_citation/bmj_article_list_2',
  'preprocess' => 'bmj_citation_style_bmj_article_list_2'
);

/**
 * Preprocessor function to process the type of article and sees if it a pap or not
 */
function bmj_citation_style_bmj_article_list_2(&$variables) {
  if (!$variables['node']) {
    return FALSE;
  }

  $node = $variables['node'];
  //kpr($node);

  $article_info = FALSE;  // Set a default value;
  // Load additional article info for the node.
  if (isset($node->field_bmj_additional_article_inf[X][0]['target_id'])
    && !empty($node->field_bmj_additional_article_inf[X][0]['target_id'])) {
    $article_info = jnl_bmj_get_additional_article_info($node->field_bmj_additional_article_inf[X][0]['target_id']);
  }

  // Title  content/bmj/347/bmj.f3391
  $title = jnl_bmj_get_display_title($node, $article_info);
  $variables['title'] = l($title, 'node/' . $node->nid, array(
      'attributes' => array('class' => array('bmj-hero-title-link')),
    )
  );

  // Article Date (ppub)
  if (isset($node->field_highwire_a_epubdate[X][0]['value'])
    && !empty($node->field_highwire_a_epubdate[X][0]['value'])) {
    $variables['date'] = format_date(strtotime($node->field_highwire_a_epubdate[X][0]['value']),
      'custom', 'j F Y');
  }

  else {
    $variables['date'] = FALSE;
  }

  /// Category Links
  if (isset($node->field_highwire_article_category[X]) &&
    !empty($node->field_highwire_article_category[X])) {
    $cat_ids = array();
    $categories = $node->field_highwire_article_category[X];
    foreach ($categories as $category) {
      $cat_ids[] = $category['tid'];
    }
    $variables['category_links'] = theme('bmj_category_links', array('tids' => $cat_ids, 'node' => $variables['node']));
  }

  else {
    $variables['category_links'] = FALSE;
  }
}

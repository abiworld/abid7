<?php
/**
 * @file
 * journal_articles.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function journal_articles_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function journal_articles_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function journal_articles_node_info() {
  $items = array(
    'journal_article' => array(
      'name' => t('Journal Article'),
      'base' => 'node_content',
      'description' => t('Allows Members to create articles for the journal'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

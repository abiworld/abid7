<?php
/**
 * @file
 * abstract.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function abstract_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function abstract_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function abstract_node_info() {
  $items = array(
    'event_calendar' => array(
      'name' => t('Event Calendar'),
      'base' => 'node_content',
      'description' => t('A content type to create events for calendar.'),
      'has_title' => '1',
      'title_label' => t('Event Title'),
      'help' => '',
    ),
    'headlines' => array(
      'name' => t('ABI Articles'),
      'base' => 'node_content',
      'description' => t('Can be used to publish different types of articles that include headlines, chart of the day and other static content that has image/text/link type of  content'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

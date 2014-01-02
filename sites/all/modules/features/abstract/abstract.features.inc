<?php
/**
 * @file
 * abstract.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function abstract_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
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
 * Implements hook_image_default_styles().
 */
function abstract_image_default_styles() {
  $styles = array();

  // Exported image style: video.
  $styles['video'] = array(
    'name' => 'video',
    'label' => 'Video',
    'effects' => array(),
  );

  // Exported image style: vimeo_video.
  $styles['vimeo_video'] = array(
    'name' => 'vimeo_video',
    'label' => 'Vimeo Video',
    'effects' => array(),
  );

  return $styles;
}

/**
 * Implements hook_mediafront_views_default_options().
 */
function abstract_mediafront_views_default_options() {
  $options = array(
    'test' => array(
      'field_upload_file_fid' => array(
        'link_to_player' => 0,
        'field_type' => 0,
        'media_type' => 'media',
        'preview' => 0,
        'thumbnail' => 0,
        'custom' => '',
      ),
      'field_upload_file' => array(
        'link_to_player' => 0,
        'field_type' => 0,
        'media_type' => 'media',
        'preview' => 0,
        'thumbnail' => 0,
        'custom' => '',
      ),
      'field_newfile' => array(
        'link_to_player' => 0,
        'field_type' => 0,
        'media_type' => 'media',
        'preview' => 0,
        'thumbnail' => 0,
        'custom' => '',
      ),
    ),
    'youtube_media' => array(
      'title' => array(
        'link_to_player' => 1,
        'field_type' => 'title',
        'media_type' => 'media',
        'preview' => 0,
        'thumbnail' => 0,
        'custom' => '',
      ),
      'field_youtube_poster' => array(
        'link_to_player' => 1,
        'field_type' => 'image',
        'media_type' => 'media',
        'preview' => 'large',
        'thumbnail' => 'thumbnail',
        'custom' => '',
      ),
      'field_youtube_url' => array(
        'link_to_player' => 0,
        'field_type' => 'media',
        'media_type' => 'media',
        'preview' => 0,
        'thumbnail' => 0,
        'custom' => '',
      ),
    ),
  );
  return $options;
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

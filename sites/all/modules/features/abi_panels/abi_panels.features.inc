<?php
/**
 * @file
 * abi_panels.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function abi_panels_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function abi_panels_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_mediafront_views_default_options().
 */
function abi_panels_mediafront_views_default_options() {
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

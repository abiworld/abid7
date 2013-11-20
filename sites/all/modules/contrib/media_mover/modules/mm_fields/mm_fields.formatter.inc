<?php

/**
 * @file
 * Provides a formatter for CCK data- overrides filefield defaults
 *
 */

// @TODO: filefield module has been renamed into file module. Need to redesign all themes and hooks appropriately.

/**
 * Theme function for the 'default' filefield formatter.
 * Inserted into theme registry by mm_fields_theme_registry_alter().
 */
function theme_mm_fields_filefield_formatter_default($variables) {
  $element = $variables['$element'];
  $file = $element['#item'];
  //@todo FIXME replace content_fields() with field_info_instances() or field_info_fields()
  $field = content_fields($element['#field_name']);
  $output = theme('filefield_item', array('file' => $file, 'field' => $field));
  return $output;
}

/**
 * Theme function for the 'path_plain' formatter.
 * Inserted into theme registry by mm_fields_theme_registry_alter().
 */
function theme_mm_fields_filefield_formatter_path_plain($variables) {
  $element = $variables['$element'];
  // Inside a View this function may be called with null data. In that case,
  // just return.
  if (empty($element['#item'])) {
    return '';
  }

  //@todo FIXME replace content_fields() with field_info_instances() or field_info_fields()
  $field = content_fields($element['#field_name']);
  $item = $element['#item'];
  // If there is no image on the database, use default.
  if (empty($item['fid']) && $field['use_default_file']) {
    $item = $field['default_file'];
  }
  if (empty($item['filepath']) && !empty($item['fid'])) {
//@todo: field_file_load() is not found in the D7 API
    $item = array_merge($item, field_file_load($item['fid']));
  }

  return empty($item['filepath']) ? '' : mm_fields_filepath_check($item); // @todo use $file->load_info()
}

/**
 * Theme function for the 'url_plain' formatter.
 * Inserted into theme registry by mm_fields_theme_registry_alter().
 */
function theme_mm_fields_filefield_formatter_url_plain($variables) {
  $element = $variables['$element'];
  // Inside a View this function may be called with null data. In that case,
  // just return.
  if (empty($element['#item'])) {
    return '';
  }

  //@todo FIXME replace content_fields() with field_info_instances() or field_info_fields()
  $field = content_fields($element['#field_name']);
  $item = $element['#item'];
  // If there is no image on the database, use default.
  if (empty($item['fid']) && $field['use_default_file']) {
    $item = $field['default_file'];
  }
  if (empty($item['filepath']) && !empty($item['fid'])) {
    $item = array_merge($item, field_file_load($item['fid']));
  }

  return empty($item['filepath']) ? '' : mm_fields_filepath_check($item['filepath']); // @todo use $file->load_info()
}

/**
 * Theme function for any file that is managed by FileField.
 * Inserted into theme registry by mm_fields_theme_registry_alter().
 *
 * It doesn't really format stuff by itself but rather redirects to other
 * formatters that are telling us they want to handle the concerned file.
 *
 * This function checks if the file may be shown and returns an empty string
 * if viewing the file is not allowed for any reason. If you need to display it
 * in any case, please use theme('filefield') instead.
 */
function theme_mm_fields_filefield_item($variables) {
  $file = $variables['file'];
  $field = $variables['field'];
  // Ensure that the filefield functions are loaded
  module_load_include('inc', 'filefield', 'filefield_formatter');
  if (filefield_view_access($field['field_name']) && filefield_file_listed($file, $field)) {
    return theme('filefield_file', array('file' => $file));
  }
  return '';
}

/**
 * Overrides the standard filefield file display.
 * Inserted into theme registry by mm_fields_theme_registry_alter().
 */
function theme_mm_fields_filefield_file($variables) {
  $file = $variables['file'];
  // Views may call this function with a NULL value, return an empty string.
  if (empty($file['fid'])) {
    return '';
  }

  $path = $file['filepath'];
  // If this filepath has a URI in it, do not format it
  $url = mm_fields_filepath_check($file); // @todo use $file->load_info()

  $icon = theme('file_icon', array('file' => $file, 'icon_directory' => NULL));

  // Set options as per anchor format described at
  // http://microformats.org/wiki/file-format-examples
  // @TODO: Possibly move to until I move to the more complex format described
  // at http://darrelopry.com/story/microformats-and-media-rfc-if-you-js-or-css
  $options = array(
    'attributes' => array(
      'type' => $file['filemime'] . '; length=' . $file['filesize'],
    ),
  );

  // Use the description as the link text if available.
  if (empty($file['data']['description'])) {
    $link_text = $file['filename'];
  }
  else {
    $link_text = $file['data']['description'];
    $options['attributes']['title'] = $file['filename'];
  }

  return '<div class="filefield-file clear-block">' . $icon . l($link_text, $url, $options) . '</div>';
}


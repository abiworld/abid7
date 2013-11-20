<?php

/**
 * @file
 * This contains the theming functions for node display
 */


/**
 * Display a media mover file on a node landing page
 *
 * @param type $files
 * @return string
 */
function theme_mm_fields_files_view($variables) {
  $files = $variables['files'];
  $header = array(t('Media Mover files'), t('Size'));
  $rows = array();
  foreach ($files as $cid => $cid_files) {
    foreach ($cid_files as $mmfid => $file) {
      // Create a link to the file
      $link = $file->get_link();
      $rows[] = array($link, format_size($file->filesize));
    }
  }
  if (count($rows)) {
    return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'attachments')));
  }
}


/**
 * Theme the Media Mover files list. Shamelessly stollen from upload.module
 *
 * @ingroup themeable
 */
function theme_mm_fields_files_edit($variables) {
  $form = $variables['element'];
  $header = array(t('Display'), t('Delete'), t('Reprocess'), t('Filepath'), t('Size'), t('Status'));
  $rows = array();
  foreach (element_children($form) as $key) {
    unset($form[$key]['display']['#title']);
    unset($form[$key]['remove']['#title']);
    unset($form[$key]['reprocess']['#title']);
    $row = array();
    $row[] = drupal_render($form[$key]['display']);
    $row[] = drupal_render($form[$key]['remove']);
    $row[] = drupal_render($form[$key]['reprocess']);
    $row[] = drupal_render($form[$key]['filepath']);
    $row[] = drupal_render($form[$key]['size']);
    $row[] = drupal_render($form[$key]['status']);
    $rows[] = array('data' => $row);
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'mm_fields-files')));
  $output .= drupal_render_children($form);
  return $output;
}
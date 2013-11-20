<?php

/**
 * @file
 * Provides directory support for Media Mover
 */


/**
 * Builds the configuration form for the directory select.
 *
 * @param $step
 *   object, media mover configuration step
 * @return array $form
 */
function media_mover_api_step_directories_select_config($step) {
  $form['directories_select_conf'] = array(
    '#type' => 'fieldset',
    '#title' => t('Directory select configuration'),
    '#description' => t('This selects files from a local directory.'),
    '#element_validate' => array('media_mover_api_step_directories_select_config_validate')
  );
  $form['directories_select_conf']['path'] = array(
    '#title' => t('Directory path'),
    '#type' => 'textfield',
    '#default_value' => isset($step->settings['path']) ? $step->settings['path'] : '',
    '#description' => t('Absolute path or relative path from Drupal\'s root
      (eg: sites/default/files) to the directory you wish to select from.
      Note you may encounter open base dir issues wtih some PHP installations
      if your directory is not inside of your web root.'),
  );
  $form['directories_select_conf']['file_types'] = array(
    '#title' => t('File types'),
    '#type' => 'textfield',
    '#default_value' => isset($step->settings['file_types']) ? $step->settings['file_types'] : 'avi mov wmv mpeg mp4 mpeg2 dv 3gp 3g2 mpeg4',
    '#description' => t('Space separated list of file types that can be selected.
      These are file extensions and are values like "avi mov wmv mpeg mp4 mpeg2
      dv". Leave blank to select any kind of file.'),
  );
  return $form;
}


/**
 * Validates the configuration.
 */
function media_mover_api_step_directories_select_config_validate($element, &$form_state) {
  // Do no validation if we are deleting the step.
  if (isset($form_state['clicked_button']['#params']['step_order'])) {
    // Make sure we handle translations.
    $button_title = t('Remove step !step', array('!step' => $form_state['clicked_button']['#params']['step_order'] + 1));
    if ($form_state['clicked_button']['#value'] == $button_title) {
      return;
    }
  }

  // Is the path valid?
  if (!is_dir($element['path']['#value'])) {
    if (!drupal_mkdir($element['path']['#value'], NULL, TRUE)) {
      form_set_error(implode('][', $element['path']['#parents']), t('Sorry, either the source directory does not exist and/or the webserver does not have sufficient permissions to create it.'));
    }
  }
}


/**
 * Creates the configuration form for the storage option.
 *
 * @param $step
 *   object, media mover configuration object
 * @return array $form
 */
function media_mover_api_step_directories_store_config($step) {
  $form['directories_store_conf'] = array(
    '#type' => 'fieldset',
    '#title' => t('Directory storage configuration'),
    '#description' => t('This stores files in a local directory.'),
    '#collapsed' => FALSE,
    '#element_validate' => array('media_mover_api_step_directories_store_config_validate'),
  );
  $form['directories_store_conf']['path'] = array(
    '#title' => t('Directory path'),
    '#type' => 'textfield',
    '#default_value' => isset($step->settings['path']) ? $step->settings['path'] : '',
    '#description' => t('Absolute path or relative path from Drupal\'s root
      (eg: sites/default/files) to the directory you wish to store to.
      Note you may encounter open base dir issues wtih some PHP installations
      if your directory is not inside of your web root.'),
  );
  $form['directories_store_conf']['move'] = array(
    '#title' => t('Move file'),
    '#type' => 'checkbox',
    '#default_value' => isset($step->settings['move']) ? $step->settings['move'] : FALSE,
    '#description' => t('Instead of copying your file to this location, this will
      move the file, removing it from the former location.'),
  );
  return $form;
}


/**
 * Validates the path and makes sure that directory is writeable.
 */
function media_mover_api_step_directories_store_config_validate($element, &$form_state) {
  // Do no validation if we are deleting the step.
  if (isset($form_state['clicked_button']['#params']['step_order'])) {
    // Make sure we handle translations.
    $button_title = t('Remove step !step', array(
    	'!step' => $form_state['clicked_button']['#params']['step_order'] + 1)
    );
    if ($form_state['clicked_button']['#value'] == $button_title) {
      return;
    }
  }
  // Is destination valid?
  if (!file_prepare_directory($element['path']['#value'], FILE_CREATE_DIRECTORY) ) {
    form_set_error(implode('][', $element['path']['#parents']), t('Sorry, either
      the target directory can not be created or the webserver does not have
      sufficient permissions to create it.'));
  }
}


/**
 * This does the actual selectinging of the files from a directory.
 *
 * @NOTE this takes an absolute path not a URI to read files from.
 *
 * @param array $configuration
 *
 * @return array
 */
function media_mover_api_step_directories_select($step) {
  // Check to see if the directory exists
  if (!is_dir($step->settings['path'])) {
    watchdog('mm_dir', 'Directory %dir does not exist', array('%dir', $step->settings['path']), WATCHDOG_ERROR);
    return FALSE;
  }

  // Should we select all files?
  if (!$step->settings['file_types']) {
    $pattern = "\.*";
  }
  else {
    // Build the file types to select
    $filetypes = str_replace(array(' ', ','), '|', $step->settings['file_types']);
    $pattern = "/.*($filetypes)$/i";
  }

  $return_files = array();
  // Get files restricted by file extension
  if ($files = file_scan_directory($step->settings['path'], $pattern)) {
    // Get the creating configuration
    $configuration = media_mover_api_configuration_load($step->cid);
    foreach ($files as $uri => $file) {
      // Check to see if this file has previously been selected.
      if (!$configuration->file_selected($uri)) {
        // We just need to return a filepath as the file is local to
        // the same file system as Drupal.
        $return_files[] = array('uri' => $file->uri);
      }
    }
    // if we have files, return them
    if (count($return_files)) {
      return $return_files;
    }
  }
}


/**
 * This saves/moves file to specified location.
 *
 * @param array $file
 * @param array $configuration
 * @param int $action
 *
 * @return string
 */
function media_mover_api_step_directories_store($step, $file) {
  // Make sure destination directory exists
  if (!file_prepare_directory($step->settings['path'], FILE_CREATE_DIRECTORY)) {
    return FALSE;
  }
  // Assign the new path to the file name
  $new_file = file_create_filename(basename($file->uri), $step->settings['path']);
  // Should we move the file?
  if (!empty($step->settings['move'])) {
    file_unmanaged_move($file->uri, $new_file);
  }
  // Copy this file from the old to the new location
  else {
    file_unmanaged_copy($file->uri, $new_file);
  }
  media_mover_api_set_file_perm($new_file);
  // return the new uri
  return $new_file;
}
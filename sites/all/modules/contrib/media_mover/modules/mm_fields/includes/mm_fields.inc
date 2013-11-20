<?php

/**
 * @file
 *
 * Provides Media Mover services for Entity and Field modules
 */

/**
 * Media Mover API - Configuration form callback - Configure mm_fields select step.
 *
 * @param object $step
 *   Media mover step object
 * @return array
 *   Drupal Form API array
 */
function mm_fields_entity_field_select_config($step) {
  $form = array();
  $form['mm_fields_entity_field_select_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select Files Step Configuration'),
    '#description' => t('This step selects files from Entity Fields.'),
  );
  $form['mm_fields_entity_field_select_config']['select_field'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Source fields'),
    '#description' => t('Choose which fields to select files from (Ctrl-click to select multiple fields).'),
    '#options' => mm_fields_available_fields(array('text', 'file', 'image')),
    '#default_value' => isset($step->settings['select_field']) ? $step->settings['select_field'] : '',
  );
  $form['mm_fields_entity_field_select_config']['file_types'] = array(
    '#title' => t('File types'),
    '#type' => 'textfield',
    '#default_value' => isset($step->settings['file_types']) ? $step->settings['file_types'] : 'avi mov wmv mpeg mp4 mpeg2 flv dv 3gp 3g2 mpeg4',
    '#description' => t('Space-separated list of file extensions - types that can be selected (values like "avi mov wmv mpeg mp4 mpeg2 dv"). Leave blank for any file type.'),
  );
  $form['mm_fields_entity_field_select_config']['delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete when source is deleted'),
    '#description' => t('Delete the media mover file(s) when the source file is deleted.'),
    '#default_value' => isset($step->settings['delete']) ? $step->settings['delete'] : FALSE,
  );
  return $form;
}


/**
 * Media Mover API - Step callback - Select files from supported Entity Fields.
 *
 * @pram object $step
 *   Media mover step object
 * @return array
 *   Array of associative arrays with 'uri', 'fid', 'entity_type', 'entity_id'
 */
function mm_fields_entity_field_select($step) {
  $files = array();

  // We need to select from each field that was selected for that entity type
  if ($step->settings['select_field']) {
    $entity_presave = $step->parameter_get('entity_presave');
    // Select only one entity if 'entity_presave' parameter is set
    if ($entity_presave) {
      $entity_type = $entity_presave['entity_type'];
      $entity_id = $entity_presave['entity_id'];
      $entity = $entity_presave['entity'];
    }
    foreach ($step->settings['select_field'] as $identifier) {
      list($entity_type_check, $bundle, $field_name) = explode('--', $identifier);
      if ($entity_presave && $entity_type_check != $entity_type) {
        // Skip all entities of different type
        continue;
      }
      if (!$entity_presave) {
        $entity_type = $entity_type_check;
      }
      $entity_info = mm_fields_supported_entity_types($entity_type);
      if (!$entity_info) {
        // Skip unsupported entities
        continue;
      }
      $field = field_info_field($field_name);
      // Get the relevant database information for this field
      // @todo: this whole sql storage business is brittle code - it will break if there is a different storage, but there is no good API for us to extract data with.
      $storage = $field['storage']['details']['sql']['FIELD_LOAD_CURRENT'];
      $table = key($storage);
      $column = $storage[$table]['fid'];
      //@todo: Use _field_sql_storage_columnname($field['field_name'], $condition['column']); ?

      // Build the query based on field type
      switch ($field['type']) {
        case 'text':
          // @TODO need to implement this
          // $node->{$field['field_name']} = array(array('value' => $file['process_file']));
          break;

        // handle file/image field case
        case 'file':
        case 'image':
          $query = db_select('file_managed', 'f')
            ->fields('f', array('uri', 'fid'))
            ->distinct();
          $query->leftJoin($table, 'c', 'c.' . $column . ' = f.fid');
          $query->fields('c', array('entity_type', 'entity_id'));
          $query->leftJoin($entity_info['entity_table'], 'e', 'c.entity_id = e.' . $entity_info['entity_key']);
          // We rely on Media Mover API to store file_managed.uid in file->data['uid'] and "entity_table".uid in file->data['entity_uid']
          $query->addField('f', 'uid', 'uid');
          $query->addField('e', 'uid', 'entity_uid');
          $query->leftJoin('media_mover_files', 'mmf', 'mmf.fid = f.fid AND mmf.cid = :cid1', array(':cid1' => $step->cid));
          if ($step->settings['file_types']) {
            $types = explode(' ', $step->settings['file_types'] );
            $file_type_condition = db_or();
            foreach ($types as $type) {
              $file_type_condition->condition('f.uri', "%.$type", 'LIKE');
            }
            $query->condition($file_type_condition);
          }
          // Exclude files with source_uri already in {media_mover_files} table
          $subquery = db_select('media_mover_files', 'mmf1')
            ->fields('mmf1', array('source_uri'))
            ->condition('cid', $step->cid);
          $query->condition('f.uri', $subquery, 'NOT IN');
          if (!empty($entity_info['entity_bundle_key'])) {
            $query->condition('e.' . $entity_info['entity_bundle_key'], $bundle);
          }
          // @todo: Entity API, Convert to entity_type/entity_id
          // Are we selecting from a specific entity?
          if ($entity_presave && $entity_id) {
            $query->condition('e.' . $entity_info['entity_key'], $entity_id);
          }
          $query->orderBy('f.timestamp');

          // now build the outgoing file data
          $results = $query->execute();
          while ($result = $results->fetchAssoc()) {
            $files[] = $result;
          }
          break;

        default:
          watchdog('mm_fields_select', 'Specified field %field has field type %field_type that is not supported.',
            array('%field' => $step->settings['select_field'], '%field_type' => $field['type']),
            WATCHDOG_ERROR);
          return;
      }
    }
  }
  return $files;
}


/**
 * Check to see if the specified file is on the given entity.
 * (This is not a part of Media Mover API, it is api extension used only by mm_fields.module)
 *
 * @TODO non filefield fields need to be thought about here
 *
 * @param string $entity_type
 *   Entity type
 * @param object $entity
 *   Entity object
 * @param object $step
 *   Media mover step object
 * @param object $mm_file
 *   Media mover file object
 * @param array $found
 *   Returns data in the given array: 'delete', 'found'
 */
function mm_fields_entity_field_select_file_exists($entity_type, $entity, $step, $mm_file, &$found) {
  $langcode = LANGUAGE_NONE; // @todo: how field translations are implemented? = isset($entity->language) ? $entity->language : LANGUAGE_NONE;
  foreach ($step->settings['select_field'] as $identifier) {
    list($entity_type_check, $bundle, $field_name) = explode('--', $identifier);
    if ($entity_type_check != $entity_type) {
      continue;
    }
    $items = field_get_items($entity_type, $entity, $field_name, $langcode);
    if ($items) {
      foreach ($items as $item) {
        if (!isset($item['uri'])) {
          // Very rarely, but uri may be missing (suspect deleted/missing file)
          $item = (array)file_load($item['fid']);
        }
        // We only look at the original selected file
        if ($mm_file->source_uri == $item['uri']) {
          $found['found'] = TRUE;
          return;
        }
      }
      // We did not find the file, check and see if we should delete
      if (empty($step->settings['delete'])) {
        $found['delete'] = FALSE;
      }
    }
  }
}


/**
 * Media Mover API - Configuration form callback - Configure mm_fields store to Entity Fields step.
 *
 * @param object $step
 *   Media mover step object
 * @return array
 *   Drupal Form API array
 */
function mm_fields_entity_field_store_config($step) {
  $form = array();
  $form['mm_fields_entity_field_store_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Store Files Configuration'),
    '#description' => t('This step saves files to Entity Fields'),
    '#collapsed' => FALSE,
  );
  $form['mm_fields_entity_field_store_config']['target_field'] = array(
    '#type' => 'select',
    '#title' => t('Target field'),
    '#description' => t('Select an Entity Field to save files to. There must
      be incoming entity data for this to function correctly.
      <em>PLEASE NOTE:</em> if you are attaching a file to a field that has
      data already you need to either 1) ensure that this is a multiple value
      field 2) Use the <em>Replace existing file</em> option below that will
      replace the file - this assumes that the file you are attaching is
      a derivative of the file that is being replaced. If neither of these
      is the case it is likely that your configuration will fail on this
      step.'),
    '#options' => mm_fields_available_fields(array('text', 'file', 'image')),
    '#default_value' => isset($step->settings['target_field']) ? $step->settings['target_field'] : '',
  );

  $insert_method_options = array(
    'append_untill_full' => t('Append (until full)'),
    'append_replace_last' => t('Append (replace last if full)'),
    'remove_all' => t('Remove all existing and append'),
    'fixed_position' => t('Replace in fixed position (select the insert position below)'),
  );
  $form['mm_fields_entity_field_store_config']['insert_method'] = array(
    '#title' => t('Insert method'),
    '#type' => 'radios',
    '#options' => $insert_method_options,
    '#default_value' => isset($step->settings['insert_method']) ? $step->settings['insert_method'] : reset(array_keys($insert_method_options)),
    '#description' => t('WARNING: any settings besides "Append (until full)" are destructive! This step will
      DELETE material in the target field. THERE IS NO UNDO. If you are at all concerned
      about your material, do not change this setting. Otherwise, this will happily replace
      the material with the file from the previous Media Mover step and
      delete older files in the field.')
  );
  $form['mm_fields_entity_field_store_config']['insert_position'] = array(
    '#title' => t('Insert position'),
    '#type' => 'textfield',
    '#element_validate' => array('element_validate_integer'),
    '#default_value' => isset($step->settings['insert_position']) ? $step->settings['insert_position'] : '',
    '#description' => t('Enter position number (from 0 to field\'s max) where to insert the file. This is only applicable if "Replace in fixed position" option is selected in the "Insert method".')
  );

  $form['mm_fields_entity_field_store_config']['use_derivatives'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use derivatives'),
    '#default_value' => isset($step->settings['use_derivatives']) ? $step->settings['use_derivatives'] : FALSE,
    '#description' => t('Use derivatives from previous step if present.')
  );

  $uid_options = array(
    'target_entity' => t('Same as Target entity'),
    'source_entity_uid' => t('Same as Source entity'),
    'source_file_uid' => t('Same as Source file'),
  );
  $form['mm_fields_entity_field_store_config']['uid_source'] = array(
    '#title' => t('File owner'),
    '#type' => 'radios',
    '#options' => $uid_options,
    '#default_value' => isset($step->settings['uid_source']) ? $step->settings['uid_source'] : reset(array_keys($uid_options)),
    '#description' => t('Select how to set the owner of the saved file. If for some reason the source owner is not set, the ownership will fall back to the previous option.')
  );

  return $form;
}


/**
 * Media Mover API - Step callback - Store the file in an Entity Field.
 *
 * @param object $step
 *   Media mover step object
 * @param array $file
 *   Media mover file object
 * @return string
 *   New file uri
 */
function mm_fields_entity_field_store($step, $file) {
  $log_type = 'mm_fields_store';
  $log_step = t('@cid:@order:@sid', array('@cid' => $step->cid, '@sid' => $step->sid, '@order' => $step->step_order));
  $log_link = l(t('MMfile @id', array('@id' => $file->mmfid)), MMA_MENU_PATH . '/file/edit/' . $file->mmfid);
  $log_op = t('storing file %file in the ', array('%file' => $file->uri));
  $use_derivatives = $step->settings['use_derivatives'];
  $insert_method = $step->settings['insert_method'];
  $insert_position = $step->settings['insert_position'];

  $entity = mm_fields_entity_target_get($step, $file, $log_type, $log_step, $log_op, $log_link);
  if (!$entity) {
    return;
  }

  // Get the field that we are using (we will check type later)
  list($entity_type_check, $bundle, $field_name) = explode('--', $step->settings['target_field']);
  $langcode = LANGUAGE_NONE; // @todo: how field translations are implemented? = isset($entity->language) ? $entity->language : LANGUAGE_NONE;
  $field = field_info_field($field_name);
  $instance = field_info_instance($entity->entity_type, $field_name, $bundle);
  switch ($field['type']) {
    case 'text':
      // @todo: what is the $field_key = '?';
      break;

    // handle file/image field case
    case 'file':
    case 'image':
      $field_key = 'uri';
      break;
  }
  if (!isset($field_key)) {
    watchdog($log_type, '!step: Field type %field_type is not supported for !op field %field.',
      array('!step' => $log_step, '!op' => $log_op, '%field' => $step->settings['target_field'], '%field_type' => $field['type']),
      WATCHDOG_ERROR, $log_link);
    return;
  }

  // Prepare to handle derivatives and position in the field
  if ($use_derivatives && ($derivatives = $file->data_get('derivatives'))) {
    $add_count = count($derivatives);
  }
  else {
    $add_count = 1;
    $derivatives = array($file);
  }
  $delta_count = (empty($entity->{$field_name}) || empty($entity->{$field_name}[$langcode]))
    ? 0
    : count($entity->{$field_name}[$langcode]);
  $cardinality = $field['cardinality']; // FIELD_CARDINALITY_UNLIMITED
  $clear_field = FALSE;
  $keep_tail = FALSE;
  switch ($insert_method) {
    case 'append_untill_full':
      // Append (until full)
      if ($cardinality != FIELD_CARDINALITY_UNLIMITED && $delta_count+$add_count > $cardinality) {
        if ($delta_count+1 > $cardinality) {
          // Cannot add any more values
          watchdog($log_type, '!step: The field is full at %count entries for !op field %field.',
            array('!step' => $log_step, '!op' => $log_op, '%field' => $step->settings['target_field'], '%count' => $delta_count),
            WATCHDOG_NOTICE, $log_link);
          return;
        }
        // Add as many as fits in
        $add_count = $cardinality - $delta_count; // $add_count won't be less than 1 by previous "if"
        watchdog($log_type, '!step: The field already contains @entries for !op field %field. Adding only %add_count new entries of %derivatives_count derivatives.',
          array('!step' => $log_step, '!op' => $log_op, '%field' => $step->settings['target_field'], '%count' => $delta_count, '%add_count' => $add_count, '%derivatives_count' => count($derivatives),
            '@entries' => format_plural($delta_count, '%count entry', '%count entries', array('%count' => $delta_count))),
          WATCHDOG_NOTICE, $log_link);
      }
      $delta = $delta_count;
      break;
    case 'append_replace_last':
      // Append (replace last if full)
      $delta = $delta_count;
      if ($cardinality != FIELD_CARDINALITY_UNLIMITED && $delta_count+$add_count > $cardinality) {
        $delta = $cardinality - $add_count;
        if ($delta < 0) {
          // Limit $add_count
          $delta = 0;
          $add_count = $cardinality;
        }
        $clear_field = TRUE;
        watchdog($log_type, '!step: The field is full at %count entries for !op field %field, replacing last @entries.',
          array('!step' => $log_step, '!op' => $log_op, '%field' => $step->settings['target_field'], '%count' => $delta_count,
            '@entries' => format_plural($add_count, '%count entry', '%count entries', array('%count' => $add_count))),
          WATCHDOG_NOTICE, $log_link);
      }
      break;
    case 'remove_all':
      // Remove all existing and append
      $delta = 0;
      if ($cardinality != FIELD_CARDINALITY_UNLIMITED && $delta+$add_count > $cardinality) {
        // Limit $add_count
        $delta = 0;
        $add_count = $cardinality;
      }
      $clear_field = TRUE;
      break;
    case 'fixed_position':
      // Replace in fixed position (select the insert position below)
      $delta = ($insert_position && is_int($insert_position)) ? $insert_position : 0;
      if ($delta > $delta_count) {
        // We cannot place items in non-contiguous position
        $delta = $delta_count;
      }
      if ($cardinality != FIELD_CARDINALITY_UNLIMITED && $delta+$add_count > $cardinality) {
        // Limit $add_count
        $add_count = $cardinality - $delta;
      }
      $clear_field = TRUE;
      $keep_tail = TRUE;
      break;
  }
  if (!isset($delta)) {
    watchdog($log_type, '!step: Unknown insert method "@method" for !op field %field.',
      array('!step' => $log_step, '!op' => $log_op, '%field' => $step->settings['target_field'], '@method' => $insert_method),
      WATCHDOG_ERROR, $log_link);
    return;
  }

  // Prepare field structure
  $entity->{$field_name} += array($langcode => array());
  $entity->{$field_name}[$langcode] += array($delta => array());
  if ($clear_field && !empty($entity->{$field_name}) && !empty($entity->{$field_name}[$langcode])) {
    $items = $entity->{$field_name}[$langcode];
    $entity->{$field_name}[$langcode] = array_slice($items, 0, $delta);
    $items = $keep_tail ? array_slice($items, $delta + $add_count) : array();
  }

  // Prepare uid for saved file. Try to take what the setting prescribes, but if not available, progressively fall back.
  if ($step->settings['uid_source'] = 'source_file_uid') {
    $uid = $file->data_get('uid');
  }
  if (empty($uid) && $step->settings['uid_source'] = 'source_entity_uid') {
    $uid = $file->data_get('entity_uid');
  }
  if (empty($uid)) {
    $uid = $entity->uid;
  }

  // Use derivatives if requested
  $derivatives = array_slice($derivatives, 0, $add_count);
  foreach ($derivatives as $my_file) {
    if (is_array($my_file) && !isset($my_file['uri'])) {
      // We got a derivatives element which only contains filepath in [0], convert it to a media mover file object
      $uri = reset($my_file);
      $my_file = new media_mover_file;
      $my_file->uri = $uri;
      $my_file->data = array();
    }
    $my_file->uid = $uid;
    // Sanity check (not performed by the media_mover_api)
    if (!file_exists($my_file->uri)) {
      watchdog($log_type, '!step: File was not found for !op field %field.',
        array('!step' => $log_step, '!op' => $log_op, '%field' => $step->settings['target_field']),
        WATCHDOG_ERROR, $log_link);
      return;
    }

    $item = FALSE;
    switch ($field['type']) {
      case 'text':
        //$item = array('value' => $my_file->uri);
        // @todo: FIXME, file given to us typicaly resides in /tmp/ directory, need to move it to a permanent place
        // @todo: what is the follow-up code should do?
        break;

      // handle file/image field case
      case 'file':
      case 'image':

        // Save file to a permanent location
        // We need to do that here because otherwise in $entity_presave mode we won't receive final $file->uri/fid
        $item = mm_fields_field_file_save($my_file, $field, $instance);
        if (!$item) {
          watchdog($log_type, '!step: Could not save file %file, failed validations.',
            array('!step' => $log_step, '%file' => $my_file->uri),
            WATCHDOG_ERROR, $log_link);
          return;
        }
        $file->fid = $item->fid;
        $file->uri = $item->uri;

        // Attach file to the entity field
        // Don't need to call file_usage_add() as file.module will add usage upon save
        $entity->{$field_name}[$langcode][$delta] = (array) $item;
        $entity->{$field_name}[$langcode][$delta][$field_key] = $item->uri;
        //? $entity->{$field_name}[$langcode][$delta][$field_key]['uid'] = $item->uid;
        // Do we display the file?
        // @todo: is $step->settings['field_save_list'] getting set somewhere?
        $entity->{$field_name}[$langcode][$delta]['display'] = (!empty($my_file->data['display'])
          ? TRUE
          : (isset($step->settings['field_save_list'])
            ? $step->settings['field_save_list']
            : (isset($field['settings']['display_default'])
              ? $field['settings']['display_default']
              : 0
            )
          )
        );
        $delta++;
        break;
    }
  }

  // Append the rest of $items for $insert_method == 'fixed_position' after inserting into the field
  if (!empty($items)) {
    array_splice($entity->{$field_name}[$langcode], $delta, count($entity->{$field_name}[$langcode]), $items);
  }


  // If the same entity type/id was passed in 'entity_presave' during hook_nodeapi we need to not save
  $entity_presave = $step->parameter_get('entity_presave');
  if (!$entity_presave || $entity->entity_type != $entity_presave['entity_type'] || $entity->entity_id != $entity_presave['entity_id']) {
    mm_fields_entity_save($entity);
  }

  // Pass the target entity down the stream for other steps
  $file->data_set('entity_type', $entity->entity_type);
  $file->data_set('entity_id', $entity->entity_id);

  // Return the file URI
  return $file->uri;
}

/**
 * Media Mover API - Delete step file callback - Delete file from a Store step.
 *
 * @param object $file
 *   Media mover file object
 * @param object $step
 *   Media mover step object
 */
function mm_fields_entity_field_store_delete($file, $step) {
  $log_type = 'mm_fields_store_delete';
  $log_step = t('@cid:@order:@sid', array('@cid' => $step->cid, '@sid' => $step->sid, '@order' => $step->step_order));

  $file_info = $file->get_step_file($step->step_order);
  if (!$file_info) {
    return;
  }

  if (!empty($file_info->fid)) {
    $file_to_delete = file_load($file_info->fid);
    if ($file_to_delete) {
      watchdog($log_type, '!step: Deleting file %file (fid @fid) associated with step %sid of configuration %cid.',
        array('!step' => $log_step, '%file' => $file_to_delete->uri, '@fid' => $file_to_delete->fid, '%sid' => $step->sid, '%cid' => $step->cid));
      file_delete($file_to_delete);
    }
  }
}

/**
 * Check to see if the specified file is on the given entity.
 * (This is not a part of media_mover_api, it is api extension used only by mm_fields.module)
 *
 * @param string $entity_type
 *   Entity type
 * @param object $entity
 *   Entity object
 * @param object $step
 *   Media mover step object
 * @param object $mm_file
 *   Media mover file object
 * @param array $found
 *   Returns data in the given array: 'delete', 'found'
 */
function mm_fields_entity_field_store_file_exists($entity_type, $entity, $step, $mm_file, &$found) {
  // $entity_type, $entity, $step, $mm_file, &$found
}

/**
 * Media Mover API - Configuration form callback - Configure mm_fields clear Entity Fields step.
 *
 * @param object $step
 *   Media mover step object
 * @return array
 *   Drupal Form API array
 */
function mm_fields_entity_field_clear_config($step) {
  $form = array();
  $form['mm_fields_entity_field_clear_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Clear Entity Field Configuration'),
    '#description' => t('This step clears Entity Field to prepare for store of new files.'),
    '#collapsed' => FALSE,
  );
  $form['mm_fields_entity_field_clear_config']['target_field'] = array(
    '#type' => 'select',
    '#title' => t('Entity Field to clear'),
    '#description' => t('Select an Entity Field to clear. There must
      be incoming entity data for this to function correctly.'),
    '#options' => mm_fields_available_fields(array('text', 'file', 'image')),
    '#default_value' => isset($step->settings['target_field']) ? $step->settings['target_field'] : '',
  );
  // @todo: implement target_entity_id control (entity_type is set by above input field)
  //$form['mm_fields_entity_field_clear_config']['target_entity_id'] = array(...);

  return $form;
}

/**
 * Media Mover API - Step callback - Clear an Entity Field.
 *
 * @param object $step
 *   Media mover step object
 * @param array $file
 *   Media mover file object
 * @return string
 *   New file uri
 */
function mm_fields_entity_field_clear($step, $file) {
  $log_type = 'mm_fields_clear';
  $log_step = t('@cid:@order:@sid', array('@cid' => $step->cid, '@sid' => $step->sid, '@order' => $step->step_order));
  // @todo: think of a better $log_link:
  $log_link = l(t('MMfile @id', array('@id' => $file->mmfid)), MMA_MENU_PATH . '/file/edit/' . $file->mmfid);
  $log_op = t('clearing');

  $entity = mm_fields_entity_target_get($step, $file, $log_type, $log_step, $log_op, $log_link);
  if (!$entity) {
    return;
  }

  // Get the field that we are using (we will check type later)
  list($entity_type_check, $bundle, $field_name) = explode('--', $step->settings['target_field']);
  $langcode = LANGUAGE_NONE; // @todo: how field translations are implemented? = isset($entity->language) ? $entity->language : LANGUAGE_NONE;

  $delta = 0;

  if (!empty($entity->{$field_name}) && !empty($entity->{$field_name}[$langcode])) {
    $entity->{$field_name}[$langcode] = array_slice($entity->{$field_name}[$langcode], 0, $delta);
  }

  // If the same entity type/id was passed in 'entity_presave' during hook_nodeapi we need to not save
  $entity_presave = $step->parameter_get('entity_presave');
  if (!$entity_presave || $entity->entity_type != $entity_presave['entity_type'] || $entity->entity_id != $entity_presave['entity_id']) {
    mm_fields_entity_save($entity);
  }

  // return the file URI
  return $file->uri;
}

/* ************************************************** */
/* Helper functions                                   */
/* ************************************************** */

/**
 * Retrieve entity to work on (for store and delete steps)
 *
 * @param object $step
 *   Media mover step object
 * @param array $file
 *   Media mover file object
 * @param string $log_type, $log_op, $log_link
 *   Strings for system log in case of errors
 * @return boolean or object
 *   FALSE if error, Drupal entity object (with additional properties entity_type, entity_id, entity_key, entity_save_function, entity_module)
 */
function mm_fields_entity_target_get($step, $file, $log_type, $log_step, $log_op, $log_link) {
  // Get the field that we are using (we will check type later)
  list($entity_type_check, $bundle, $field_name) = explode('--', $step->settings['target_field']);

  // Find entity to use
  $target_entity_id = $step->parameter_get('target_entity_id'); // @todo: convert to $step->settings['target_entity_id'];
  if ($target_entity_id) {
    // Load target entity
    // @todo: We can also implement "create new" option here... need extensive settings UI for this
    $entity_type = $entity_type_check;
    $entity_id = $target_entity_id;
  }
  else {
    // Retrieve entity info passed from the select step
    $entity_type = $file->entity_type;
    $entity_id = $file->entity_id;
  }

  if ($entity_type && $entity_id) {
    $entities = entity_load($entity_type, array($entity_id));
    $entity = reset($entities);
  }

  if (empty($entity)) {
    watchdog($log_type, '!step: No entity was found for !op field %field.!more',
      array(
        '!step' => $log_step, '!op' => $log_op,
        '%field' => $field_name,
        '!more' => ' ' . ($target_entity_id
          ? t('Target entity id #@id was not found.', array('@id' => $target_entity_id))
          : t('Perhaps it was not passed from a previous "select" step.')),
      ),
      WATCHDOG_ERROR, $log_link);
    return FALSE;
  }

  $supported_entity_types = mm_fields_supported_entity_types();
  if (isset($supported_entity_types[$entity_type])) {
    $entity_key = $supported_entity_types[$entity_type]['entity_key'];
    $entity_save_function = $supported_entity_types[$entity_type]['entity_save_function'];
    $entity_module = $supported_entity_types[$entity_type]['entity_module'];
  }
  else {
    watchdog($log_type, '!step: Unsupported entity type %entity_type found for !op field %field.',
      array('!step' => $log_step, '!op' => $log_op, '%entity_type' => $entity_type, '%field' => $field_name),
      WATCHDOG_ERROR, $log_link);
    return FALSE;
  }

  // Check the field that we are using
  if ($entity_type_check != $entity_type) {
    watchdog($log_type, '!step: Target entity type %entity_type found is different from the configured entity type %entity_type_check for !op field %field.!more',
      array(
        '!step' => $log_step, '!op' => $log_op,
        '%entity_type' => $entity_type,
        '%entity_type_check' => $entity_type_check,
        '%field' => $field_name,
        '!more' => ' ' . t('Perhaps it was not configured properly in a previous "select" step.'),
      ),
      WATCHDOG_ERROR, $log_link);
    return FALSE;
  }
  // Save additional info to the object
  // @todo: Need to ensure we are not inventing a bicycle here.
  $entity->entity_type = $entity_type;
  $entity->entity_id = $entity_id;
  $entity->entity_key = $entity_key;
  $entity->entity_save_function = $entity_save_function;
  $entity->entity_module = $entity_module;

  return $entity;
}

/**
 * Save the entity
 *
 * @param object $entity
 *   Drupal entity object (with additional properties entity_type, entity_id, entity_key, entity_save_function, entity_module)
 */
function mm_fields_entity_save($entity) {
  if (!empty($entity->entity_save_function)) {
    $function = $entity->entity_save_function;
    $function($entity);
  }
  else {
    //? $entity->save();
    // Build our own entity save function // mm_fields_entity_save($entity)
    // Based on node_save http://api.drupal.org/api/drupal/modules%21node%21node.module/function/node_save/7
   $transaction = db_transaction();
    try {
      if (!empty($entity->entity_id) && !isset($entity->original)) {
        $entity->original = entity_load_unchanged($entity->entity_type, $entity->entity_id);
      }
      field_attach_presave($entity->entity_type, $entity);
      if (!isset($entity->is_new)) {
        $entity->is_new = empty($entity->{$entity->entity_key});
      }
      //module_invoke_all('node_presave', $entity);
      module_invoke_all('entity_presave', $entity, $entity->entity_type);

      if ($entity->is_new) {
        // For new entity, save new records .
        $success = drupal_write_record($entity->entity_type, $entity);
        $op = 'insert';
      }
      else {
        // For existing entity, update the entity record which matches the value of $entity->entity_id.
        $success = drupal_write_record($entity->entity_type, $entity, $entity->entity_id);
        $op = 'update';
      }
      module_invoke_all('entity_' . $op, $entity, $entity->entity_type);
      field_attach_update($entity->entity_type, $entity);
      // Clear the static loading cache.
      entity_get_controller($entity->entity_type)->resetCache(array($entity->entity_id));
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('mm_fields', $e);
      throw $e;
    }
  }
}

/**
 * Helper function to create a new (temporary) drupal file based on the Field settings.
 *
 * @param object $file
 *   Media mover file object
 * @param array $field
 *   Drupal field array
 * @param array $instance
 *   Drupal field instance array
 * @return object
 *   Drupal file object (ready for insertion into a field)
 */
function mm_fields_field_file_save($file, $field, $instance) {
  if (!empty($file->fid)) {
    // Load Drupal file object
    $item = file_load($file->fid);
  }
  else {
    // Supplement media mover file object with more info
    $file->load_info();

    // Create a new Drupal file object
    $item = new stdClass;
    $item->new = TRUE;
    foreach (array(
      'uri',
      'uid',
      'filename',
      'filemime',
      'filesize',
//      'display',
    ) as $key) {
      if (isset($file->{$key})) {
        $item->{$key} = $file->{$key};
      }
    }
  }

  // get the validators for this field
  $validators = file_field_widget_upload_validators($field, $instance);
  if ($field['type'] == 'file') {
    // Set filefield size validation to the per file limit set on the field. file_field_widget_upload_validators
    // sets the max filesize to the sites PHP upload limit if the field setting is greater than the PHP upload limit.
    // In the case of programmatic saving of files via media mover, we can use sizes greater than the PHP upload limit.
    if (!empty($instance['settings']['max_filesize'])) {
      $validators['file_validate_size'][0] = parse_size($instance['settings']['max_filesize']);
    }
  }
  if (!empty($instance['settings']['file_extensions'])) {
    $validators['file_validate_extensions'] = array($instance['settings']['file_extensions']);
  }
  // Validate the file for insertion into the field
  $errors = file_validate($item, $validators);
  if (!empty($errors)) {
    return FALSE; // @todo: pass errors to caller for watchdog report somehow...
  }

  // Move the file to a specified directory for use with this field
  $destination_path = mm_fields_field_file_save_directory($file, $field);
  if (empty($item->fid)) {
    $uri = file_unmanaged_move($item->uri, $destination_path);
  }
  else {
    $uri = file_move($item, $destination_path);
  }
  if (!$uri) {
    return FALSE;
  }
  $item->uri = $uri;
  $item->status = FILE_STATUS_PERMANENT;
//  unset($item->fid);
  if (empty($item->fid)) {
    $item = file_save($item);
  }
  return $item;
}


/**
 * Create a destination path for the file.
 *
 * @TODO Not sure that this is functioning as expected
 * @todo: add user/uid argument... for supporting uid source setting in the step (options: source file's uid, source entity's uid, target entity uid, fixed uid)
 *
 * @param object $file
 *   Media mover file object
 * @param array $field
 *   Drupal field array
 * @return string
 *   A local path to the directory
 */
function mm_fields_field_file_save_directory($file, $field) {
  // Get path settings for the field, with fallback to site-wide setting.
  $path = (isset($field['settings']['uri_scheme']) ? $field['settings']['uri_scheme'] : file_default_scheme()). '://';

  //@todo: $file->data['user/uid'] is not set by anyone... Implement.
  if (isset($file->data['user']->uid) and module_exists('token')) {
    // @todo Convert "user_load" to "user_load_multiple" if "$file->data['user']->uid" is other than a uid.
    // To return a single user object, wrap "user_load_multiple" with "reset" or equivalent.
    // Example: reset(user_load_multiple(array(), $file->data['user']->uid))
    $user = user_load($file->data['user']->uid);
    $path_tokens = $field['widget']['file_path'];
    if ($directories = token_replace($path_tokens, array('user' => $user))) {
      $path .= '/' . $directories;
    }
  }
  $success = file_prepare_directory($path, FILE_CREATE_DIRECTORY);
  if (!$success) {
    watchdog('mm_fields', 'Error preparing directory %path for saving file %file for insertion into %field field.',
      array('%path' => $path, '%file' => $file->uri, '%field' => $field['field_name']), WATCHDOG_ERROR);
  }
  return $path;
}

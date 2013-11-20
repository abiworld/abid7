<?php

/**
 * @file
 *
 * This file contains all of the specific functions that
 * are needed to interact with media mover content connected
 * to nodes. These functions are called from hook_node_***()
 */


/* ************************************************** */
/* Configuration and action functions                 */
/* ************************************************** */


/**
 * Media Mover API - Configuration form callback - Configure node publishing options set step.
 *
 * @param object $step
 *   Media mover step object
 * @return array
 *   Drupal Form API array
 */
function mm_fields_node_publishing_options_set_config($step) {
  $form['mm_fields_node'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node publishing options configuration'),
    '#collapsed' => FALSE,
    '#description' => t('This step will change node publishing options.'),
  );
  $form['mm_fields_node']['publishing_options'] = array(
    '#title' => t('Node publishing options'),
    '#type' => 'checkboxes',
    '#options' => mm_fields_node_publishing_options(),
    '#default_value' => isset($step->settings['publishing_options']) ? $step->settings['publishing_options'] : array(),
    '#description' => t('Set the publishing options for this node. <b>NOTE</b>: these choices will replace any previously set publishing options for this node, including clearing the set ones.'),
  );

  return $form;
}

/**
 * Media Mover API - Step callback - Set node publishing options.
 * Get list of files that match assumed criteria
 *
 * @pram object $step
 *   Media mover step object
 * @param array $file
 *   Media mover file object
 * @return string
 *   New file uri
 */
function mm_fields_node_publishing_options_set($step, $file) {
  $log_type = 'mm_fields_node_publishing';
  $log_step = t('@cid:@order:@sid', array('@cid' => $step->cid, '@sid' => $step->sid, '@order' => $step->step_order));
  $log_link = '';
  $log_op = t('setting node publishing options');

  // Retrieve target node (from file entity interface)
  $node = mm_fields_node_target_get($step, $file, $log_type, $log_step, $log_op, $log_link);
  if (!$node) {
    return FALSE;
  }

  // Get the default publishing options
  foreach (mm_fields_node_publishing_options() as $name => $title) {
    if ($step->settings['publishing_options'][$name]) {
      $node->{$name} = TRUE;
    }
    else {
      $node->{$name} = 0;
    }
  }

  // If revisions are enabled make a note in the logs
  if (!empty($node->revision)) {
    $node->log = t('New revision created by Media Mover');
  }
  node_save($node);
  return TRUE;
}


/**
 * Configuration form for the storage option
 * @param $step
 *   Object, media mover step object
 * @return $form
 *   Array, Drupal form array
 */
function mm_fields_save_config($step) {
  drupal_add_js(drupal_get_path('module', 'mm_fields') . '/mm_fields.js');
  $form['storage'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node storage configuration'),
    '#collapsed' => FALSE,
    '#description' => t('This module will try to do a best guess of harvested data and save this to a node.'),
  );

  // Define the possible storage types
  $options = array();

  // We can catch cck content here
  // @todo: Update to D7 field API
  if (module_exists('content')) {
    $options = array_merge($options, mm_fields_cck_fields(array('text', 'file', 'image')));
  }
  if ($options) {
    $form['storage']['storage_type'] = array(
      '#type' => 'select',
      '#title' => t('Save the file to'),
      '#description' => t('Select a place to store the file.'),
      '#options' => $options,
      '#default_value' => $step->settings['storage_type'],
    );
  }

  $form['storage']['new_node'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create a new node'),
    '#description' => t('If this is selected, new nodes will always be created
      with the file. Otherwise if there is node data available the file will
      be saved to the existing node. Files with no node information will create
      new nodes with the below settings.'),
    '#default_value' => isset($step->settings['new_node']) ? $step->settings['new_node'] : FALSE,
  );

  $form['storage']['node'] = array(
    '#type' => 'fieldset',
    '#title' => t('Defaults for new nodes'),
    '#collapsed' => TRUE,
    '#collapsible' => true,
  );

  $form['storage']['node']['node_save_type'] = array(
    '#title' => t('Content type'),
    '#type' => 'select',
    '#options' => node_type_get_names(),
    '#default_value' => $step->settings['node_save_type'],
    '#description' => t('Save the file as new node of this type'),
  );

  $form['storage']['node']['node_save_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Default author'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => $step->settings['node_save_name'],
    '#description' => t('If a user is not passed in, this will be the default author. Leave blank for anonymous'),
    '#element_validate' => array('mm_fields_save_config_validate'),
  );

  $form['storage']['node']['node_title_options'] = array(
    '#type' => 'radios',
    '#title' => t('Title options'),
    '#default_value' => $step->settings['node_title_options'] ? $step->settings['node_title_options'] : 'filename',
    '#options' => array(
      'filename' => t('Filename'),
      'default' => t('Use existing title if available'),
      'title' => t('Specify node title')
    ),
    '#description' => t('Chose how the title is set. "Filename" will use the name
      of the file as the title. "Use existing" will look for a title passed by
      a previous Media Mover action. If none is available, if you have specified a title, that
      will be used, otherwise the filename will be used. If you specify a node title, that
      will be used.'),
  );

  $form['storage']['node']['node_title_default'] = array(
    '#type' => 'textfield',
    '#title' => t('Default title'),
    '#maxlength' => 60,
    '#default_value' => $step->settings['node_title_default'],
    '#description' => t('Set the default title to this value.'),
    '#prefix' => '<div id="mm_fields_title_default">',
    '#suffix' => '</div>',
  );

  $form['storage']['node']['node_body_default'] = array(
    '#type' => 'textarea',
    '#title' => t('Default body'),
    '#default_value' => $step->settings['node_body_default'] ? $step->settings['node_body_default'] : t('This was created without any additional information'),
    '#description' => t('If a body is not passed in, this will be the default node body.'),
  );

  // Language options
  if (function_exists('locale_language_list')) {
    $form['storage']['node']['node_save_language'] = array(
      '#type' => 'select',
      '#title' => t('Save node in language'),
      '#description' => t('Save the node in this language when creating it'),
      '#options' => locale_language_list(),
      '#default_value' => $step->settings['node_save_language'],
    );
  }

  $form['storage']['node']['node_publish_options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node publishing options'),
    '#options' => array(
      'comment' => t('Enable comments'),
      'status' => t('Published'),
      'promote' => t('Promoted to front page'),
      'sticky' => t('Sticky at the top of lists'),
    ),
    '#default_value' => isset($step->settings['node_publish_options']) ? $step->settings['node_publish_options'] : array(),
  );
  return $form;
}


/**
 * Validation duties for the node save configuration
 */
function mm_fields_save_config_validate($element, &$form_state) {
  // Is the user valid?
  if (! $account = array_shift(user_load_multiple(array(), array('name' => $element['#value'])))) {
    form_error($element,   t('Sorry, the name you chose for a default node author was not valid.'));
  }
  // Should validate fields against nodes

  // Should validate upload option agains nodes with attachments

}



/**
 * Stores the actual data as a node
 *
 * @param array $file
 * @param array $configuration
 * @TODO finish up this functionality to deploy
 *        requires creating the data store
 */
function mm_fields_save($step, $file) {
  // Can we update an existing node?
  if (! $step->settings['new_node']) {
    $node = $file->node_get();
    if (module_exists('content')) {
      content_load($node);
    }
  }
  // Do we need to create a new node?
  if (! $node) {
    // start building the node
    $node = new stdClass();
    // set node type
    $node->type = $step->settings['node_save_type'];

    $node->is_new = TRUE;

    // Should we use the filename for the title?
    if ($step->settings['node_title_options'] == 'filename') {
      $node->title = basename($file->uri);
    }
    // Should we use any node data if it exists on the file?
    elseif ($file->data['node']->title && ! $step->settings['node_title_options'] == 'default') {
      $node->title = $file->data['node']->title;
    }
    // Just use the default title
    else {
      $node->title = $step->settings['node_title_default'];
    }

    // Set the body
    if ($file->data['node']->body) {
      $node->body = $file->data['node']->body;
    }
    else {
      $node->body = $step->settings['node_body_default'];
    }

    // Set language options
    if ($step->settings['node_save_language']) {
      $node->language = $step->settings['node_save_language'];
    }

    // node options
    $node->comment = $step->settings['node_publish_options']['comment'] ? 2 : 0;
    $node->status = $step->settings['node_publish_options']['status'] ? 1 : 0;
    $node->sticky = $step->settings['node_publish_options']['sticky'] ? 1 : 0;
    $node->promote = $step->settings['node_publish_options']['promote'] ? 1 : 0;

    // Now build the node author
    $account = mm_fields_map_user($file, $step);

    // attach the user data
    $node->uid = $account->uid;
    $node->name = $account->name;
    $node->mail = $account->mail;

    // Save the new node
    // Don't save the node here.  In case the file below can't be saved and we return false.
    //node_save($node);
  }

  // Now we have to handle the actual file storage
  if ($storage = $step->settings['storage_type']) {
    list($type, $kind) = explode('--', $storage);

    // Check for drupal uploads
    if ($kind == 'upload') {
      $new_file = mm_fields_save_drupal_file($step, $file, $node->uid);
//@todo: Convert to new D7 API, obsolete upload.module
      $node->files[] = $new_file;
      // Now save the node with the updated data
      if ($node->is_new) {
        node_save($node);
      }
      upload_save($node);
      // Make sure we return the correct file path
      $filepath = $new_file->filepath;
      // Update the fid for this file
      $file->fid = $new_file->fid;
    }

    // Check for CCK fields
    else {
      // Get the field that we will save to.
      $fields = field_info_instances('node', $type);
      $field = $fields[$kind];

      // Now what do we do with it?
      switch ($field['type']) {

        // Image and file field options
        case 'image':
        case 'filefield':
          module_load_include('inc', 'mm_fields', 'includes/mm_fields.content');
          if (! $cck_saved_file = mm_fields_content_field_filefield($file, $field)) {
            return FALSE;
          }
          // Save the new filepath
          $file->filepath = $cck_saved_file['filepath'];

          // Attach saved file to node
          if (! $node->{$field['field_name']} || ! isset($node->{$field['field_name']})) {
            $node->{$field['field_name']} = array($cck_saved_file);
          }
          else {
            $node->{$field['field_name']}[] = $cck_saved_file;
          }
          break;
      }

      // Now save the node with the updated data
      node_save($node);
    }
  }

  // Append the $nid to the $file for easy retrieval
  $file->nid = $node->nid;

  return $file->uri;
}


/**
 * Utility function to load a user from a file object.
 *
 * This function lets users map incoming data to node values
 * for advanced node data saving. User data is handled by
 * _mm_fields_save
 *
 * @param array $file
 *   media mover file object
 * @param array $step
 *   media mover step object
 * @return object
 *   a user object for saving
 */
function mm_fields_map_user($file, $step) {
  $account = FALSE;
  // check the override if we should use it
  if ($file->data['user'] && empty($step->settings['node_save_override'])) {
    // loop through the user data and try to load a user
    $keys = array('name', 'uid', 'mail');
    foreach ($keys as $key) {
      // check to see if we have this data
      if ($file->data['user']->{$key}) {
        // try to load a user
        if ($account = reset(user_load_multiple(array(), array($key => $file->data['user']->{$key})))) {
          break;
        }
      }
    }
  }
  // fall back to node data to load the user
  if (!$account && $file->data['node']->uid && empty($step->settings['node_save_override'])) {
    $account = reset(user_load_multiple(array(), array('uid' => $file->data['node']->uid)));
  }

  // Do we have an override user name or should returned user be anonymous?
  if (!$account && $step->settings['node_save_name']) {
    $account = reset(user_load_multiple(array(), array('name' => $step->settings['node_save_name'])));
  }
  // make sure that we have an account to load
  if (!$account) {
    // load the admin account
    $account = user_load(0);
  }
  return $account;
}

/**
 * Removes references to files from files table where they were stored
 *
 * @TODO this needs to be completed (it is UNUSED)
 */
function mm_fields_upload_file_delete($file) {
  if ($file->fid) {
    // Delete all files associated with the node
    db_delete('files')
    ->condition('fid', $file->fid)
    ->execute();
    file_delete($file->filepath);
  }

  // Delete the media mover file
  $file->delete();

}


/**
 * Generic form for node creation functions
 */
//@todo: what the heck all the "mm_emfield_..." here?
function mm_fields_create_form() {
  // Node creation options
  $form['mm_emfield_storage']['node_create'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node creation options'),
    '#collapsed' => isset($step->settings['mm_emfield_new_node']) ? FALSE : TRUE,
    '#collapsible' => TRUE,
    '#description' => t('When creating a new node, these options will be used'),
  );

  // Get content types.
  if ($types = node_type_get_types()) {
    foreach ($types as $type) {
      $options[$type->type] = $type->name;
    }
  }
  $form['mm_emfield_storage']['node_create']['mm_emfield_save_type'] = array(
    '#title' => t('Save node as type'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $step->settings['mm_emfield_save_type'],
    '#description' => t('Save the harvested and processed data as this node type'),
  );
  $form['mm_emfield_storage']['node_create']['mm_emfield_save_author'] = array(
    '#type' => 'textfield',
    '#title' => t('Default author'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => isset($step->settings['mm_emfield_save_author']) ? $step->settings['mm_emfield_save_author']  : t('Anonymous'),
    '#description' => t('If a user is not passed in, this will be the default author.'),
  );
  $form['mm_emfield_storage']['node_create']['mm_emfield_title_default'] = array(
    '#type' => 'textfield',
    '#title' => t('Default title'),
    '#maxlength' => 60,
    '#default_value' => $step->settings['mm_emfield_title_default'],
    '#description' => t('If a title is not passed in, this will be the default node title. If left blank and there is no title information, the file name will be used.'),
  );
  $form['mm_emfield_storage']['node_create']['mm_emfield_body_default'] = array(
    '#type' => 'textarea',
    '#title' => t('Default body'),
    '#default_value' => $step->settings['mm_emfield_body_default'],
    '#description' => t('If a body is not passed in, this will be the default node body.'),
  );
  $form['mm_emfield_storage']['node_create']['mm_emfield_save_options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node options'),
    '#options' => array(
      'comment' => t('Enable comments'),
      'status' => t('Published'),
      'promote' => t('Promoted to front page'),
      'sticky' => t('Sticky at the top of lists'),
    ),
    '#default_value' => isset($step->settings['mm_emfield_save_options']) ? $step->settings['mm_emfield_save_options'] : array(),
  );

  // Language options.
  if (module_exists('locale')) {
    $options = locale_language_list();
    $form['mm_emfield_storage']['node_create']['mm_emfield_save_language'] = array(
      '#type' => 'select',
      '#title' => t('Save node in language'),
      '#description' => t('Save the node in this language when creating it'),
      '#options' => $options,
      '#default_value' => $step->settings['mm_emfield_save_language'],
    );
  }
  return $form;
}

/**
 * Utility function to provide standard node publishing options.
 *
 * @return array
 *   Array of node publishing options
 */
function mm_fields_node_publishing_options() {
  $options = array(
    //?'comment' => t('Enable comments'),
    'status' => t('Published'),
    'sticky' => t('Sticky at top of lists'),
    'promote' => t('Promoted to front page'),
  );
  return $options;
}

/**
 * Retrieve node to work on.
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
function mm_fields_node_target_get($step, $file, $log_type, $log_step, $log_op, $log_link) {
  // Try target entity from previous step (if set)
  $entity_type = $file->data_get('entity_type');
  $entity_id = $file->data_get('entity_id');
  if (!$entity_type || !$entity_id) {
    // Retrieve entity info passed from the select step
    $entity_type = $file->entity_type;
    $entity_id = $file->entity_id;
  }

  if ($entity_type && $entity_id) {
    $entities = entity_load($entity_type, array($entity_id));
    $entity = reset($entities);
  }

  if (empty($entity)) {
    watchdog($log_type, '!step: No node was found for !op.!more',
      array(
        '!step' => $log_step, '!op' => $log_op,
        '!more' => ' ' . t('Perhaps it was not passed from a previous "select" step.'),
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

  // Save additional info to the object
  // @todo: Need to ensure we are not inventing a bicycle here.
  $entity->entity_type = $entity_type;
  $entity->entity_id = $entity_id;
  $entity->entity_key = $entity_key;
  $entity->entity_save_function = $entity_save_function;
  $entity->entity_module = $entity_module;
  return $entity;
}


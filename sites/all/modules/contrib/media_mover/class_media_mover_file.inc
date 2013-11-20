<?php

/**
 * @file
 * Base class for media mover files
 */


class media_mover_file {

  /**
   * By default load the file
   */
  function __construct($input = NULL) {
    if (empty($input)) {
      $this->status = MMA_FILE_STATUS_READY;
      $this->new = TRUE;
      $this->step_order = 0;
    }

    // If the input is a mmfid try to load the file
    else if (is_string($input) || is_int($input)) {
      $this->load($input);
    }

    // If the input is a media mover step we can construct the stub file from
    // a default set of data
    else if (is_object($input) && get_class($input) == 'media_mover_step') {
      $defaults['cid'] = $input->cid;
      $defaults['step_order'] = $input->step_order;
      // Add the defaults to the new file
      foreach ($defaults as $key => $value) {
        $this->{$key} = $value;
      }
    }
  }


  /**
   * Get media mover file data for the requested id.
   *
   * @param int $mmfid
   *   Media Mover file id
   * @param array $data
   *   Media mover file data (if already loaded)
   * @return
   *   Boolean, did the file load or not?
   *
   */
  function load($mmfid, $data = NULL) {
    if ($mmfid && !$data) {
      $query = db_select('media_mover_files', 'mmf')
        ->condition('mmf.mmfid', $mmfid, '=')
        ->fields('mmf');
      $data = $query->execute()->fetchAssoc();

      // No file data was found.
      if (! $data) {
        return FALSE;
      }
    }

    // Load the data onto the file.
    $this->load_data((array) $data);

    // Load file data.
    $this->load_info();

    // Allow the file to be altered
    drupal_alter('media_mover_file_load', $this);
  }


  /**
   * Load more information into the file object.
   */
  function load_info() {
    // Supplement file object with more info
    if (!isset($this->filename)) {
      $this->filename = basename($this->uri);
    }
    if (!isset($this->filemime)) {
      $this->filemime = file_get_mimetype($this->uri);
    }
    if (!isset($this->filesize)) {
      $this->filesize = file_exists($this->uri) ? filesize($this->uri) : 0;
    }
    if (!isset($this->filepath)) {
      $this->filepath = file_create_url($this->uri);
    }
  }


  /**
   * Get file object for the step stored in the file data.
   *
   * @param int $step_order
   *   Step number, return file object for that step.
   *
   * @return object
   *   Return media mover file object or FALSE if step or file is unknown.
   */
  function get_step_file($step_order) {
    if (!isset($this->data['steps']) || !isset($this->data['steps'][$step_order])) {
      return FALSE;
    }
    $file_step = $this->data['steps'][$step_order];

    $file = new media_mover_file;
    $file->uri = $file_step['result_uri'];
    $file->sid = $file_step['sid'];
    $file->fid = empty($file_step['fid']) ? FALSE : $file_step['fid'];
    $file->step_order = $file_step['step_order'];
    $file->data = array('description' => '');
    return $file;
  }

  /**
   * Return file URI.
   *
   * @param int $step_order
   *   Optional step number, if given, returns file uri for that step.
   *
   * @return string
   *   File uri or FALSE if file is unknown
   */
  function get_uri($step_order = -1) {
    if ($step_order >= 0) {
      $file = $this->get_step_file($step_order);
      return $file ? $file->uri : FALSE;
    }
    return $this->uri;
  }

  /**
   * Return file URL.
   *
   * @param int $step_order
   *   Optional step number, if given, returns file URL for that step.
   *
   * @return string
   *   File URL or FALSE if file is unknown or file is not web-accessible (i.e. in /tmp/ directory)
   */
  function get_url($step_order = -1) {
    $uri = $this->get_uri($step_order);
    $scheme = file_uri_scheme($uri);
    if (!$scheme) {
      $file = clone $this;
      $file->uri = $uri;
      drupal_alter('media_mover_filepath', $file);
      if (isset($file->filepath)) {
        return $file->filepath;
      }
      if (drupal_substr($uri, 0, 1) == '/') {
        // File is local but in a not web-accessible location
        return FALSE;
      }
    }
    $file_url = file_create_url($uri);
    return $file_url;
  }

  /**
   * Return file link
   * Side effect: calls load_info()
   *
   * @param int $step_order
   *   optional step number, if given, returns file link for that step.
   * @return string
   *   File link or NA if file is unknown or %formatted filepath if file is not web-accessible (i.e. in /tmp/ directory)
   */
  function get_link($step_order = -1) {
    $file = ($step_order >= 0) ? $this->get_step_file($step_order) : $this;
    if (!$file) {
      return t('NA');
    }

    $file_url = $file->get_url();
    if (!$file_url) {
      // For not web-accessible file return file uri as text or 'NA'
      $file_uri = $file->get_uri();
      return ($file_uri ? t('%file', array('%file' => $file_uri)) : t('NA'));
    }

    $file->load_info();

    // Set options as per anchor format described at
    // http://microformats.org/wiki/file-format-examples
    // @TODO: Possibly move to ? until I move to the more complex format described
    // at http://darrelopry.com/story/microformats-and-media-rfc-if-you-js-or-css
    $options = array(
      'attributes' => array(
        'type' => $file->filemime . '; length=' . $file->filesize,
      ),
    );

    // Use the description as the link text if available.
    if (empty($file->data['description'])) {
      $link_text = $file->filename;
    }
    else {
      $link_text = $file->data['description'];
      $options['attributes']['title'] = $file->filename;
    }

    return l($link_text, $file_url, $options);
  }


  /**
   * Return source entity.
   *
   * @return object
   *   Entity object if exists, FALSE if none exists.
   */
  function get_entity() {
    if (empty($this->entity_type) || empty($this->entity_id)) {
      return FALSE;
    }
    $entities = entity_load($this->entity_type, array($this->entity_id));
    $entity = reset($entities);
    return $entity;
  }


  /**
   * Return link to the file's entity.
   *
   * @param string $op
   *   Optional operation, one of 'edit', 'delete'
   *
   * @return string
   *   Entity link or FALSE if file entity is unknown.
   */
  function get_entity_link($op = NULL) {
    if ($entity = $this->get_entity) {
      $uri = entity_uri($this->entity_type, $entity);
      $link_text = entity_label($entity_type, $entity);
      if (!$link_text) {
        $link_text = t('@type @id', array('@type' => $entity_type, '@id' => $entity_id));
      }
      return l($link_text, $uri['path'] . ($op ? '/' . $op : ''), $uri['options']);
    }
    return FALSE;
  }


  /**
   * Save data associated with a file.
   *
   * @NOTE this function is not thread safe. $file->lock/unlock() should be used
   *       to prevent this file from being hijacked by a new thread.
   *
   * @TODO this should not save $file->status, only $file->unlock() should do
   * this
   *
   * @param bool
   *   $advance should the file's current step be advanced?
   */
  function save($advance = FALSE) {
    // Set a default status
    if (empty($this->status)) {
      $this->status = MMA_FILE_STATUS_READY;
    }

    if (empty($this->date)) {
      $this->date = time();
    }
    if (empty($this->source_uri)) {
      $this->source_uri = $this->uri;
    }
    if (empty($this->source_fid) && !empty($this->fid)) {
      $this->source_fid = $this->fid;
    }
    // If a filesize was not passed in see if we can get one.
    if ((!$filesize = $this->data_get('filesize')) && file_exists($this->uri)) {
      $this->data_set('filesize', filesize($this->uri));
    }

    // Advance the step for this file if requested
    if ($advance) {
      $this->step_next();
    }

    // Alter the file.
    drupal_alter('media_mover_file_presave', $this);

    // If this is a programatic usage do not access the database
    // @TODO this needs rethinking
    if (! empty($this->passthrough)) {
      return;
    }

    // Save it.
    $status = drupal_write_record('media_mover_files', $this, empty($this->mmfid) ? array() : array('mmfid'));
    if (! $status) {
      return FALSE;
    }

    // Flag the file as saved.
    $this->new = FALSE;

    // Reset the mmfile cache.
    drupal_static_reset('media_mover_api_mmfile_load');

    return $status;
  }


  /**
   * Check to see if this file is currently locked.
   *
   * @NOTE this will return TRUE even if the file is already locked. This is a
   *       potential hazzard if a file that is locked is already being passed
   *       into the system.
   *
   * @return bool
   */
  function locked() {
    if (! empty($this->status) && $this->status == MMA_FILE_STATUS_LOCKED) {
      return TRUE;
    }
    return FALSE;
  }


  /**
   * Set the status to locked.
   *
   * Locks the file status to prevent it from being used by multiple processes.
   * This will lock any file's status to locked. Functions implementing custom
   * statuses need to be careful.
   *
   * @TODO can an already locked file return TRUE here? This is mostly to fix
   * and issue with.
   *
   * @return bool
   */
  function lock() {
    if (! $this->locked()) {
      $this->lock_date = time();
      // Was the status correctly set?
      return $this->status_set('*');
    }
    return TRUE;
  }


  /**
   * Unlock a file for further use. Only saves status and unlock time.
   *
   * This is used as the last step in a thread safe handling of a file. If a file
   * is locked this will unlock the file and save the status. If this file is
   * not locked it will simply save the current status of the file. This is used
   * by steps that implement custom file statuses.
   *
   * @param $status
   *   String, status to set this file to
   */
  function unlock($status = MMA_FILE_STATUS_READY) {
    // If this file is still locked from processing it can be unlocked. Files
    // with errors or custom status do not need to be updated.
    if ($this->locked()) {
      $this->lock_date = 0;
      // If this is file is on the last step in the configuration it is finished.
      $configuration = media_mover_api_configuration_load($this->cid);
      // Is this the last step for this file.
      // @NOTE $configuration is not required to unlock a file
      if (! empty($this->cid) && $configuration->last_step($this->step_order)) {
        $this->status = MMA_FILE_STATUS_FINISHED;
      }
      else {
        $this->status = $status;
      }
    }

    $this->status_set('*', $this->status);

    return $this->status;
  }


  /**
   * Updates a file's URI.
   *
   * As the file moves from step to step. Note that this is not thread safe.
   *
   * @param string $uri
   *   File uri
   * @param object $step
   *   Media mover step object
   */
  function update_uri($uri, $step) {
    // Some modules may return the uri as TRUE or return FALSE
    // if there was an error. In either case do nothing.
    if (empty($uri) || $uri === TRUE) {
      $uri = $this->uri;
    }

    $this->data_set('steps', array(
      'step_order' => $step->step_order,
      'sid' => $step->sid,
      'result_uri' => $uri,
      'fid' => empty($this->fid) ? FALSE : $this->fid,
    ));

    // Update the current uri.
    $this->uri = $uri;

    // Reload file info.
    $this->load_info();
  }


  /**
   * API to permantly store file data. Will overwrite existing data.
   * Brital. Sorry.
   *
   * @param string $name
   *   Name of the data to store.
   * @param array, string $value
   *   If $value is an array, the array should be formated as
   *   array('id' => ID, 'value' => VALUE).
   */
  function data_set($key, $value) {
    if (is_string($value) || is_int($value)) {
      $this->data[$key] = $value;
    }
    // Non string data
    else {
      // A plain array is being stored at no specified ID
      if (! isset($value['id'])) {
        $this->data[$key][] = $value;
      }
      else {
        $this->data[$key][$value['id']] = $value['value'];
      }
    }

  }


  /**
   * API function to retrieve data from the file.
   */
  function data_get($key) {
    if (! empty($this->data[$key])) {
      return $this->data[$key];
    }
    return FALSE;
  }


  /**
   * API function to remove data from the file.
   *
   * @param string $key
   * @param string $index
   */
  function data_delete($key, $index = NULL) {
    if ($index) {
      unset($this->data[$key][$index]);
    }
    else {
      unset($this->data[$key]);
    }
  }


  /**
   * Set file source entity type/id.
   *
   * @TODO is this needed? These values should only be set by the select action.
   *
   * @param string $entity_type
   *   Entity type
   * @param int $entity_id
   *   Entity id
   */
  function entity_set($entity_type, $entity_id) {
    $this->entity_type = $entity_type;
    $this->entity_id = $entity_id;
  }


  /**
   * Return source entity type/id from a file.
   *
   * @TODO is this needed?
   *
   * @param boolean $load
   *   Select return type
   *
   * @return array or object
   *   Array keys: 'entity_type', 'entity_id' or object if $load = TRUE if
   *   entity has been set, or FALSE
   */
  function entity_get($load = TRUE) {
    if (empty($this->entity_type) && empty($this->entity_id)) {
      return FALSE;
    }
    if ($load) {
      $entities = entity_load($this->entity_type, array($this->entity_id));
      $entity = reset($entities);
      return $entity;
    }
    else {
      return array('entity_type' => $this->entity_type, 'entity_id' => $this->entity_id);
    }
  }


  /**
   * Utility function: return file steps filtered by module.
   *
   * @param $module_name
   *   String, module name
   *
   * @return array of steps that match $module_name
   */
  function steps_filtered_by_module($module_name) {
    $steps = array();
    if (isset($this->data['steps'])) {
      foreach ($this->data['steps'] as $id => $item) {
        // @todo: inefficient, many single SQL queries... should run one SELECT query for all steps at once
        $step = new media_mover_step($item['sid']);
        if ($step->module == $module_name) {
          $steps[$step->step_order] = $step;
        }
      }
      if ($steps) {
        return $steps;
      }
    }
    return FALSE;
  }


  /**
   * Advance the file step.
   *
   * Moves the file one step forward and sets the file status.
   */
  protected function step_next() {
    // Load the configuration
    $configuration = media_mover_api_configuration_load($this->cid);
    // Advance the file step if this is not the last step in the configuration.
    if (! $configuration->last_step($this->step_order) ) {
      $this->step_order = $this->step_order + 1;
    }
    // This is the final step.
    else {
      $this->status = MMA_FILE_STATUS_FINISHED;
      $this->lock_date = 0;
    }
  }


  /**
   * Delete a single Media Mover file.
   *
   * @NOTE DO NOT delete source file unless explicitly told to because it could
   * delete files that are in use by Drupal elsewhere.
   *
   * @NOTE does file_usage make the point above moot?
   *
   * @TODO files need to figure out if they are being processed and only be
   *       deleted *after* they are finished being processed
   */
  function delete($source_delete = FALSE) {
    if (isset($this->data['steps'])) {
      // By default use steps > 0
      if (! $source_delete) {
        $steps = array_slice($this->data['steps'], 1);
      }
      else {
        $steps = $this->data['steps'];
      }
      foreach ($steps as $id => $item) {
        $step = new media_mover_step($item['sid']);
        // Is there a specific callback for this delete function?
        if ($step && !empty($step->build['delete'])) {
          $function = $step->build['delete'];
          if (! empty($step->build['file'])) {
            module_load_include('inc', $step->build['module'], $step->build['file']);
          }
          if (function_exists($function)) {
            $function($this, $step);
          }
        }
        elseif (!empty($item['fid'])) {
          $file = file_load($item['fid']);
          if ($file) {
            file_delete($file);
          }
        }
        elseif (file_exists($item['result_uri'])) {
          file_unmanaged_delete($item['result_uri']);
        }
      }
    }

    // Remove the file from the database.
    db_query("DELETE FROM {media_mover_files} WHERE mmfid = :mmfid", array(':mmfid' => $this->mmfid));
  }


  /**
   * Returns the uri that should be reprocessed
   *
   * @param int $step_order
   *   Step number to reprocess
   */
  function reprocess_filepath($step_order = 0) {
    // the first step should return original file
    if ($step_order === 0) {
      return $this->source_uri;
    }
    if (!isset($this->data['steps']) || !isset($this->data['steps'][$step_order])) {
      return FALSE;
    }
    return $this->data['steps'][$step_order]->uri;
  }


  /**
   * Utility function to run the next step on this file.
   */
  function process_next() {
    // We need the configuration that created this file
    $configuration = media_mover_api_configuration_load($this->cid);
    // Step to execute is one ahead of current file step
    //$step_order = $this->step_order++;
    // $step_order = $this->step_order;
    // Run the step
    return $configuration->steps[$this->step_order]->run($this);
  }


  /**
   * Set file status
   *
   * This function is intended to be used only for changing a file's status.
   * $file->save() will not change the file's status
   *
   * @param string $status_check
   *   The status state to check if this file is in, use * for any status
   * @param string $status_change
   *   The status to set the file to
   * @param int $time
   *   Unix time stamp
   * @return boolean could the file status be set?
   */
  function status_set($status_check = MMA_FILE_STATUS_READY, $status_change = MMA_FILE_STATUS_LOCKED, $time = FALSE) {
    // Only lock if the file is in the db, note that this
    // prevents locking if we are passing through a file
    // rather than saving it to the db
    if (empty($this->mmfid) || ! empty($this->pass_through)) {
      return TRUE;
    }

    // Open a transaction to keep this thread safe.
    $transaction = db_transaction();

    try {
      $query = db_query("SELECT status FROM {media_mover_files} WHERE mmfid = :mmfid", array(
        ':mmfid' => $this->mmfid
      ));
      $result = $query->fetchObject();

      if ($result->status == $status_check || $status_check == '*' ) {

        // Update the status in the DB
        $this->status = $status_change;
        // Create a dummy object so only lock status is saved- this is probably
        // never needed, but this function explicitly only sets the status of
        // the file so an implementor may unexpected results if we save the full
        // object
        $dummy = new stdClass();
        $dummy->mmfid = $this->mmfid;
        $dummy->status = $this->status;
        $dummy->lock_date = $this->lock_date;
        return drupal_write_record('media_mover_files', $dummy, array('mmfid'));
      }
    }
    catch (Exception $error) {
      $transaction->rollback();
      return FALSE;
    }
  }


  /**
   * Utility function to add data to the file
   *
   * @param $data
   *   Object, data to add to the file
   */
  private function load_data($data) {
    // Make sure we do not have serialized data
    if (! is_array($data['data'])) {
      $data['data'] = unserialize($data['data']);
    }
    // Make sure that we have a valid status @TODO is this right?
    if ($data['status'] == NULL) {
      $data['status'] = MMA_FILE_STATUS_READY;
    }
    // Add the data back onto the file
    foreach ($data as $key => $value) {
      $this->{$key} = $value;
    }
  }


  /**
   * Utility function to queue a file.
   */
  function queue() {
    // Set the status to queued
    return $this->status_set(MMA_FILE_STATUS_READY, MMA_FILE_STATUS_QUEUED);
  }


  /**
   * Utility function to remove a file from the queued state
   */
  function dequeue($status = MMA_FILE_STATUS_LOCKED) {
    // Set the status to queued
    return $this->status_set(MMA_FILE_STATUS_QUEUED, $status);
  }


}

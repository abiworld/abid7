<?php

/**
 * @file
 * The base class for media mover steps.
 */

class media_mover_step {

  // Default values for new steps
  public $settings = array();
  public $status = MMA_STEP_STATUS_READY;
  public $new = TRUE;
  public $build = array();
  public $step_order = 0;


  /**
   * Construct a step
   *
   * @param $build
   *   Either string for a SID or an array of build to build a step
   */
  function __construct($build = NULL) {
    if (! empty($build)) {
      if (is_array($build)) {
        $this->init($build);
      }
      else {
        $this->load($build);
      }
    }
  }

  /**
   * Creates the structured data of the step
   *
   * @param string or array $build
   */
  private function init($build) {
    // @TODO This should be made legacy
    $this->build = $build;

    // Add the values to the proper parts of the object
    foreach ($build as $key => $value) {
      // Ensure that these values are placed at the top of the object
      // @TODO this shoud be depricated
      if ($key == 'build' && is_array($value)) {
        $this->module = $value['module'];
        $this->action_id = $value['action_id'];
      }
      if ($key == 'description' && empty($this->name)) {
        $this->name = $value;
      }
      // @TODO This should be depricated
      if ($key == 'settings') {
        $this->settings = $value;
      }
      $this->{$key} = $value;
    }
  }


  /**
   * Loads a complete step
   *
   * @param string $sid
   *   Step id
   */
  function load($sid) {
    $query = db_select('media_mover_steps', 'mms')
      ->fields('mms')
      ->condition('mms.sid', $sid)
      ->execute();
    if (! $results = $query->fetchObject()) {
      return FALSE;
    }
    foreach ($results as $key => $value) {
      // Unserialize the configuration data
      if ($key == 'settings') {
        $this->settings = unserialize($value);
      }
      else {
        $this->{$key} = $value;
      }
    }
    // Get this step's build data
    $this->build = media_mover_api_action_build_get($this->module, $this->action_id);
    // Step has been loaded
    $this->new = FALSE;
    drupal_alter('media_mover_step_load', $this);
  }


  /**
   * Saves a step to the database. $this->passthrough prevents save
   * to the database. If a configuration does not exist in the steps
   * database table, a new one is created.
   *
   * @return integer SAVED_NEW or SAVED_UPDATED
   */
  function save() {
    drupal_alter('media_mover_step_presave', $this);
    // Should we save this step?
    if (! empty($this->passthrough)) {
      return;
    }
    // Save the settings for this step
    if ($return = drupal_write_record('media_mover_steps', $this, empty($this->new) ? array('sid') : array())) {
      $this->new = FALSE;
      return $return;
    }
    return FALSE;
  }


  /**
   * Delete this step from the DB
   */
  function delete() {
    db_query('DELETE FROM {media_mover_steps} WHERE sid = :sid', array(
      ':sid' => $this->sid)
    );
  }


  /**
   * Runs the step on an array of files. If the step order is 1, this
   * is a select file function.
   *
   * @param $file
   *   Object, media mover file. If not passed in, assumes a select function
   */
  function run(&$file = NULL) {
    // Check to see if the system will allow this step to run right now
    $halts = array();
    drupal_alter('media_mover_process_control', $halts, $file, $this);
    if (count($halts)) {
      // @TODO add to configuration->log
      /* Does not work.  Need to log to the configuration which called this step to run.
      $configuration = media_mover_api_configuration_load($this->cid);
      $configuration->log('Run control: !halts',
        array('!halts' => implode('<br/>', $halts)),
        $configuration,
        WATCHDOG_INFO);
       */
      watchdog('media_mover_api', 'Run control: !halts', array('!halts' => implode('<br/>', $halts)), WATCHDOG_NOTICE);
      return FALSE;
    }

    // Ready the step
    // @TODO can there be multiple threads of steps running?
    if (! $this->start()) {
      return FALSE;
    }

    // Select files if this step is a select operation.
    if (! empty($this->build['select'])) {
      $this->files_select();
    }

    // Non select operations
    else {
      if (isset($file)) {
        // Lock the file before running
        // @TODO there is a problem with files coming out of the queue because
        //       these get a locked status before this function gets run.
        // @NOTE second thoughts: files should never be passed into this function
        //       that have $file->status == locked.
        if ($file->lock()) {
          $this->file_process($file);
          // Update the current file->uri and step data
          $file->update_uri($file->uri, $this);
          // Now unlock and save the file.
          $file->unlock();
        }
      }
    }
    // Stop the step
    $this->stop();
    return TRUE;
  }


  /**
   * Set a temporary parameter that can be used
   *
   * @param $name
   * @param $value
   */
  function parameter_set($name, $value) {
    $this->parameters[$name] = $value;
  }


  /**
   * Get a parameter
   *
   * @param $name
   */
  function parameter_get($name) {
    if (isset($this->parameters[$name])) {
      return $this->parameters[$name];
    }
    return FALSE;
  }


  /**
   * Select files
   */
  private function files_select() {
    // Get the function to run
    $function = $this->build['callback'];

    // Load any additional functions needed for this step from include files
    if (! empty($this->build['file'])) {
      module_load_include('inc', $this->build['module'], $this->build['file']);
    }

    if (function_exists($function)) {
      if ($files = $function($this)) {
        foreach ($files as $id => $selected_file) {
          // Create the media mover file
          $mmfile = new media_mover_file($this);
          $mmfile->fid = empty($selected_file['fid']) ? FALSE : $selected_file['fid'];
          $mmfile->update_uri($selected_file['uri'], $this);

          // Pass Entity data from the harvest/select step to the subsequent steps
          if (isset($selected_file['entity_type']) && isset($selected_file['entity_id'])) {
            $mmfile->entity_type = $selected_file['entity_type'];
            $mmfile->entity_id = $selected_file['entity_id'];
//@todo: add uid here as well...
          }

          // Generalized way to pass arbitrary data from the harvest/select step to the subsequent steps
          if (isset($selected_file['meta'])) {
            $mmfile->data_set('meta', $selected_file['meta']);
          }
          $mmfile->save(TRUE);
          $files[$id] = $mmfile;
        }
      }
    }
    return $files ? $files : array();
  }


  /**
   * Executes the callback function on a specific step.
   *
   * @param $file
   *   Object, media mover file object
   */
  private function file_process($file) {
    // Allow altering of the file pre process
    // @NOTE hook_preprocess is already defined.
    drupal_alter('media_mover_file_process_pre', $file, $this);
    // Get the function to run.
    $function = $this->build['callback'];
    // Load any additional functions needed for this step from include files.
    if (!empty($this->build['file'])) {
      module_load_include('inc', $this->build['module'], $this->build['file']);
    }

    // Process the file
    // Return value is one of: URI, TRUE, FALSE).  $file->status may also have
    // been updated. There are only two conditions where the $file can advance:
    // $uri == TRUE or $uri is a uri.
    if ($uri = $function($this, $file)) {
      $advance = TRUE;
      $file->update_uri($uri, $this);
    }
    else {
      $advance = FALSE;
    }

    // Save the updated file
    $file->save($advance);
  }


  /**
   * Starts the run of a specified step. Will lock the status
   * and set the start/stop times
   *
   * @return boolean
   *   TRUE if the configuration started, FALSE if not
   */
  private function start() {
    // Open a transaction to keep this thread safe.
    $transaction = db_transaction();

    try {
      $result = db_select('media_mover_steps', 'mms')
        ->condition('mms.cid', $this->cid, '=')
        ->condition('mms.status', MMA_STEP_STATUS_READY, '=')
        ->fields('mms', array('status'))
        ->execute()
        ->fetchAssoc();
      if ($result) {
        // Update the status on our step
        $this->status = MMA_STEP_STATUS_RUNNING;
        // Set the stop time to the last time this started
        $this->stop_time = $this->start_time;
        $this->start_time = time();
        // Save the status
        drupal_write_record('media_mover_steps', $this, array('sid'));
        return TRUE;
      }
      return FALSE;
    }
    catch (Exception $error) {
      $transaction->rollback();
      return FALSE;
    }
  }


  /**
   * Stops a running step.
   *
   * @return boolean
   *   TRUE if the step was stopped, FALSE if not
   */
  private function stop() {
    // If this is a passthrough, ignore the lock
    if (! empty($this->passthrough)) {
      return TRUE;
    }

    // Open a transaction to keep this thread safe.
    $transaction = db_transaction();

    try {
      $query = db_select('media_mover_steps', 'mms')
        ->condition('mms.cid', $this->cid)
        ->condition('mms.status', MMA_STEP_STATUS_RUNNING, '=')
        ->fields('mms');
      $result = $query->execute()->fetchAssoc();
      if ($result) {
        // Update the status on our step
        $this->status = MMA_STEP_STATUS_READY;
        // Set the stop time to when this run started- this allows for
        // any activity that occured while this was running to get picked
        // up next time around
        $this->stop_time = $this->start_time;
        drupal_write_record('media_mover_steps', $this, array('sid'));
        return TRUE;
      }
      return FALSE;
    }
    catch (Exception $error) {
      $transaction->rollback();
      return FALSE;
    }
  }


  /**
   * Get the configuration form for this step.
   */
  public function configuration_form() {
    // Make sure that we load any include files
    if (! empty($this->build['file'])) {
      module_load_include('inc', $this->build['module'], $this->build['file']);
    }

    // Get the step configuration form function
    $function = $this->build['configuration'];

    if (function_exists($function)) {
      return $function($this);
    }
  }

}

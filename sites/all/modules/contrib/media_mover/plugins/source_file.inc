<?php

/**
 * @file
 * Provides the ability to use the source file for the next step in a configuration.
 */


/**
 * Media Mover API - Step callback - Restart from Source file.
 *
 * A helper step that allows to chain multiple conversions from the same source
 * file.
 *
 * @param object $step
 *   Media mover step object
 * @param array $file
 *   Media mover file object
 *
 * @return string
 *   New file uri
 */
function media_mover_api_step_source_file($step, $file) {
  $file->uri = $file->source_uri;
  $file->fid = empty($file->source_fid) ? FALSE : $file->source_fid;
  // Return the file URI.
  return $file->uri;
}

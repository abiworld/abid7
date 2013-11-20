<?php

/**
 * @file
 * Reimplements the Archiver classes to support file paths
 */

/**
 * Archiver for .tar files.
 */
class ArchiverTarMediaMover extends ArchiverTar {

  public function __construct($file_path) {
    // @TODO check and see if we should compress?
    $this->tar = new Archive_Tar($file_path, true);
  }

  /**
   * Implement the add function with the ability to not use the full file_path
   *
   * In this case $basename can only be true or false. If it is set the full
   * file_path is stripped.
   *
   * @param type $file_path
   * @param type $basename
   * @return \ArchiverZipMediaMover
   */
  public function add($file_path, $basename = FALSE) {
    if ($basename) {
      $basename = basename($file_path);
      $remove_path = str_replace($basename, '', $file_path);
      $this->tar->addModify($file_path, '', $remove_path);
    }

    // Normal behavior
    else {
      $this->tar->add($file_path);
    }
    return $this;
  }
}

/**
 * Overrides default Drupal class to handle paths better
 */
class ArchiverZipMediaMover extends ArchiverZip {

  /**
   * Implement the add function with the ability to not use the full file_path
   *
   * @param type $file_path
   * @param type $basename
   * @return \ArchiverZipMediaMover
   */
  public function add($file_path, $basename = FALSE) {
    if ($basename) {
      if (! is_string($basename)) {
        $basename = basename($file_path);
      }
      $this->zip->addFile($file_path, $basename);
    }
    else {
      $this->zip->addFile($file_path);
    }
    return $this;
  }
}
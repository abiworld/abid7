<?php
/**
 * @file
 * Implementation of DrupalUserMigration for Drupal 6 sources.
 */

/**
 * Handling specific to a Drupal 6 source for users.
 */
class BMJMigrateUsers extends DrupalUser6Migration {
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);
  }

  /**
   * Implementation of Migration::prepareRow().
   *
   * @param $row
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $d6_rids = array(7, 4, 3, 12, 10);

    //We need to filter out any AC users
     $result = Database::getConnection('default', $this->sourceConnection)
                ->select('users_roles', 'ur')
                ->fields('ur', array('uid', 'rid'))
                ->condition('ur.rid', $d6_rids, 'IN')
                ->condition('ur.uid', $row->uid)
                ->execute();

    if(!$result->rowCount()){
      return FALSE;
    }
  }

}

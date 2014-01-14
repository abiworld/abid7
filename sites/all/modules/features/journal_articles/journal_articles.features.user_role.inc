<?php
/**
 * @file
 * journal_articles.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function journal_articles_user_default_roles() {
  $roles = array();

  // Exported role: abi_staff.
  $roles['abi_staff'] = array(
    'name' => 'abi_staff',
    'weight' => 3,
  );

  // Exported role: editor.
  $roles['editor'] = array(
    'name' => 'editor',
    'weight' => 4,
  );

  // Exported role: member.
  $roles['member'] = array(
    'name' => 'member',
    'weight' => 2,
  );

  // Exported role: speaker.
  $roles['speaker'] = array(
    'name' => 'speaker',
    'weight' => 5,
  );

  return $roles;
}

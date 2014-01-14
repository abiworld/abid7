<?php
/**
 * @file
 * journal_articles.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function journal_articles_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'access private fields'.
  $permissions['access private fields'] = array(
    'name' => 'access private fields',
    'roles' => array(),
    'module' => 'field_permissions',
  );

  // Exported permission: 'administer field collections'.
  $permissions['administer field collections'] = array(
    'name' => 'administer field collections',
    'roles' => array(),
    'module' => 'field_collection',
  );

  // Exported permission: 'administer field permissions'.
  $permissions['administer field permissions'] = array(
    'name' => 'administer field permissions',
    'roles' => array(),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create field_article_rank'.
  $permissions['create field_article_rank'] = array(
    'name' => 'create field_article_rank',
    'roles' => array(
      'abi_staff' => 'abi_staff',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create field_pdf_file'.
  $permissions['create field_pdf_file'] = array(
    'name' => 'create field_pdf_file',
    'roles' => array(
      'abi_staff' => 'abi_staff',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_article_rank'.
  $permissions['edit field_article_rank'] = array(
    'name' => 'edit field_article_rank',
    'roles' => array(
      'abi_staff' => 'abi_staff',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_pdf_file'.
  $permissions['edit field_pdf_file'] = array(
    'name' => 'edit field_pdf_file',
    'roles' => array(
      'abi_staff' => 'abi_staff',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_article_rank'.
  $permissions['edit own field_article_rank'] = array(
    'name' => 'edit own field_article_rank',
    'roles' => array(
      'abi_staff' => 'abi_staff',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_pdf_file'.
  $permissions['edit own field_pdf_file'] = array(
    'name' => 'edit own field_pdf_file',
    'roles' => array(
      'abi_staff' => 'abi_staff',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'use text format filtered_html'.
  $permissions['use text format filtered_html'] = array(
    'name' => 'use text format filtered_html',
    'roles' => array(
      'abi_staff' => 'abi_staff',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'view field_article_rank'.
  $permissions['view field_article_rank'] = array(
    'name' => 'view field_article_rank',
    'roles' => array(
      'abi_staff' => 'abi_staff',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_pdf_file'.
  $permissions['view field_pdf_file'] = array(
    'name' => 'view field_pdf_file',
    'roles' => array(
      'abi_staff' => 'abi_staff',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_article_rank'.
  $permissions['view own field_article_rank'] = array(
    'name' => 'view own field_article_rank',
    'roles' => array(
      'abi_staff' => 'abi_staff',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_pdf_file'.
  $permissions['view own field_pdf_file'] = array(
    'name' => 'view own field_pdf_file',
    'roles' => array(
      'abi_staff' => 'abi_staff',
      'member' => 'member',
    ),
    'module' => 'field_permissions',
  );

  return $permissions;
}

<?php
/**
 * @file
 *
 * Resources - book navigation panel.
 */

/**
 * Description of the plugin.
 */
$plugin = array(
  'title' => t('Resources book navigation'),
  'description' => t('Shows a menu of the whole resources book.'),
  'category'         => array(t('BMJ Resources'), 0),
  'content_types'    => 'bmj_resources_book_navigation',
  'single'           => TRUE,
  'render callback'  => 'bmj_resources_book_navigation_render',
  'edit form'        => 'bmj_resources_book_navigation_edit_form',
  'admin title'      => 'bmj_resources_book_navigation_admin_title',
  'admin info'       => 'bmj_resources_book_navigation_admin_info',
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

/**
 * Run-time rendering of the pane.
 *
 * @param $subtype
 * @param $conf
 *   Configuration as done at admin time
 * @param $args
 * @param $context
 *   Context - in this case we don't have any
 *
 * @return
 *   An object with at least title and content members
 */
function bmj_resources_book_navigation_render($subtype, $conf, $panel_args, $context) {
  $node = $context->data;
  $block = new stdClass();

  $block->title   = 'Resources book navigation';

  $tree = menu_tree_all_data($node->book['menu_name'], $node->book);
  //dsm($tree);
  $data = array_shift($tree);
  $book_parent = node_load($node->book['bid']);
  $block->content = '<h4 class="resource-menu-header">'.l($book_parent->title, 'node/'.$book_parent->nid).'</h4>';
  $block->content .= ($data['below']) ? render(menu_tree_output($data['below'])) : '';

  return $block ;
}


/**
 * 'Edit form' callback for the content type.
 */
function bmj_resources_book_navigation_edit_form($form, &$form_state) {

  return $form;
}

/**
 * Callback to provide title of pane.
 *
 * @param unknown_type $subtype
 * @param unknown_type $conf
 * @param unknown_type $context
 */
function bmj_resources_book_navigation_admin_title($subtype, $conf, $context = NULL) {
  return t('Resources navigation');
}

/**
 * Callback to provide administrative info (the preview in panels when building
 * a panel).
 */
function bmj_resources_book_navigation_admin_info($subtype, $conf, $context = NULL) {
  $block = new stdClass();
  $block->title = t('Displays resources book navigation menu.');
  $block->content = '';
  return $block;
}
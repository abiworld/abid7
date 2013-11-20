<?php
/**
 * @file
 * Bmj Podcast Archives
 *
 * Copyright (c) 2010-2011 Board of Trustees, Leland Stanford Jr. University
 * This software is open-source licensed under the GNU Public License Version 2 or later
 * The full license is available in the LICENSE.TXT file at the root of this repository
 */

$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('Bmj Rapid responses archive'),
  'description' => t('Renders the rapid response archive'),
  'category' => t('BMJ Archive'),
);

function jnl_bmj_bmj_archive_rapid_response_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();

  $pager_limit = $conf['pager_limit'];

  $sort_by = isset($_GET['sort_by']) ? $_GET['sort_by'] : 'field_highwire_a_epubdate_value';
  $sort_order = isset($_GET['sort_order']) ? $_GET['sort_order'] : 'DESC';
  $items_per_page = isset($_GET['items_per_page']) ? $_GET['items_per_page'] : 10;


  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->entityCondition('bundle', 'highwire_comment');
  $query->propertyCondition('status', 1);
  // $query->propertyOrderBy($sort_by, $sort_order);
  $query->pager($items_per_page);
  $results = $query->execute();

  if (count($results)) {
    $nids = array();
    foreach ($results['node'] as $nds) {
      $responses[] = node_load($nds->nid);
    }
  }

  foreach ($responses as $response) {
    $response_archive[] = theme('bmj_response_archive', array('node' => $response));
  }

  $block->content = theme('item_list', array('items' => $response_archive)) . theme('pager');
  return $block;
}

function jnl_bmj_bmj_archive_rapid_response_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['pager_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Pager Limit'),
    '#description' => t('Number of comments to display on a page'),
    '#default_value' => isset($conf['pager_limit']) ? $conf['pager_limit'] : 10,
  );

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function jnl_bmj_bmj_archive_rapid_response_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

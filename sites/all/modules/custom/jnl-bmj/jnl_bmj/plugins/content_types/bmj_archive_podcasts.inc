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
  'title' => t('Bmj Podcast Archives'),
  'description' => t('Fetches the podcasts based on the year.'),
  'category' => t('BMJ Archive'),
  'required context' => new ctools_context_required(t('Year'), 'string'),
);

function jnl_bmj_bmj_archive_podcasts_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();

  $year = $context->data;

  $year_start = strtotime($year . '-01-01');
  $year_end = strtotime($year . '-12-31');

  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'node')
              ->entityCondition('bundle', 'bmj_podcast')
              ->propertyCondition('created', array($year_start, $year_end) , 'BETWEEN')
              ->execute();

  if (count($results)) {
    $nids = array();
    foreach ($results['node'] as $nds) {
      $podcasts[] = node_load($nds->nid);
    }
  }

  foreach ($podcasts as $podcast) {
    $title = l($podcast->title, 'node/' . $podcast->nid, array('attributes' => array('class' => array('podcast-title'))));
    $post_date = '<div class="podcast-post-date">' . format_date($node->created, 'hw_datetime_short') . '</div>';
    $podcast_markup[] = '<div class = "podcast-details">' . $title . $post_date . '</div>';
  }

  $block->content = theme('item_list', array('items' => $podcast_markup, 'title' => $year));
  return $block;
}

function jnl_bmj_bmj_archive_podcasts_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function jnl_bmj_bmj_archive_podcasts_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

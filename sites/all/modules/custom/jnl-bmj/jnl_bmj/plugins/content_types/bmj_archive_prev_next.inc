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
  'title' => t('Bmj Podcast Archives Prev Next year links'),
  'description' => t('Generates the previous and next year links based on current year.'),
  'category' => t('BMJ Archive'),
  'required context' => new ctools_context_required(t('Year'), 'string'),
);

function jnl_bmj_bmj_archive_prev_next_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $year = $context->data;
  $widget = $conf['widget'];

  if($widget == 'issue_covers' || $widget == 'issue_years') {
    $block->content = theme('bmj_prev_next_year_covers', array('year' => $year, 'type' => $panel_args[0]));
  }
  elseif ($widget == 'podcasts') {
    $block->content = theme('bmj_prev_next_year_podcast', array('year' => $year));
  }

  return $block;
}

function jnl_bmj_bmj_archive_prev_next_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['widget'] = array(
    '#type' => 'select',
    '#title' => t('Select the widget for the prev/next links'),
    '#options' => array(
      'issue_covers' => 'covers',
      'issue_years' => 'years',
      'podcasts' => 'podcasts'
    ),
    '#default_value' => isset($conf['widget']) ? $conf['widget'] : '',
  );

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function jnl_bmj_bmj_archive_prev_next_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

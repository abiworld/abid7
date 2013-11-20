<?php
/**
 * @file
 * Bmj Issue Monthly Pager
 *
 * Copyright (c) 2010-2011 Board of Trustees, Leland Stanford Jr. University
 * This software is open-source licensed under the GNU Public License Version 2 or later
 * The full license is available in the LICENSE.TXT file at the root of this repository
 */

$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('Bmj Issue Monthly Pager'),
  'description' => t('Renders the previous and next month links'),
  'category' => t('BMJ Archive'),
  'required context' => array(new ctools_context_required(t('Year'), 'string'),new ctools_context_required(t('Period'), 'string'))
);

function jnl_bmj_bmj_issue_monthly_pager_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  foreach ($context as $context_data) {
    if ($context_data->keyword == "period") {
      $period = $context_data->data;
    }
    elseif ($context_data->keyword == "year") {
      $year = $context_data->data;
    }
  }

  $period_array = explode('-', $period);
  $month = (int)$period_array[0];
  $date = (int)$period_array[1];

  $prev = $month - 1;
  $next = $month + 1;

  if ($month == 12) {
    $next = 01;
    $year = $year + 1;
  }
  if ($month == 01) {
    $prev = 12;
    $year = $year -1;
  }
  $blue_arrow_right = '<i class="arrow-right-sm-blue"></i>';
  $blue_arrow_left = '<i class="arrow-left-sm-blue"></i>';
  $links[] = l($blue_arrow_left . t('previous'), 'archive/online/' . $year . '/' . $prev . '-' . $date, array('attributes' => array('class' => array('btn')), 'html' => TRUE));
  $links[] = l(t('next') . $blue_arrow_right, 'archive/online/' . $year . '/' . $next . '-' . $date, array('attributes' => array('class' => array('btn')), 'html' => TRUE));

  $block->content = theme('item_list', array('items' => $links));
  return $block;
}

function jnl_bmj_bmj_issue_monthly_pager_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function jnl_bmj_bmj_issue_monthly_pager_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

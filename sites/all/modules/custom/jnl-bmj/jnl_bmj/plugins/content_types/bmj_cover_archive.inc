<?php
/**
 * @file
 * Bmj Cover Archives
 *
 * Copyright (c) 2010-2011 Board of Trustees, Leland Stanford Jr. University
 * This software is open-source licensed under the GNU Public License Version 2 or later
 * The full license is available in the LICENSE.TXT file at the root of this repository
 */

$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('Bmj Issue Cover Archives'),
  'description' => t('Fetches the Issue Archives based for a year.'),
  'category' => t('BMJ Archive'),
  'required context' => new ctools_context_required(t('Year'), 'string'),
);



function jnl_bmj_bmj_cover_archive_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $year = $context->data;

  // Today's date.
  $today = new DateTime();
  $current_year = $today->format('Y');
  $current_month = '';
  if ($year == $current_year) {
    $current_month = $today->format('m');
  }

  // get the number of months to list
  $months = _jnl_bmj_cover_archive($current_month);

  $image_style = $conf['cover_image_style'];
  $cover_arg['cover_size'] = $conf['cover_image_style'];
  $cover_arg['variant_link'] = 'default';

  foreach ($months as $index => $month_name) {
    $issue_nids = highwire_issues_from_year($year, $index, 'print');
    $issue_covers = array();
    foreach($issue_nids as $nid) {
      $issue_node = node_load($nid);

      $variables = array();
      // cover images with link
      $variables['issue_cover'] = _bmj_issue_archive_issues_issue_cover($issue_node, $cover_arg);

      // publication date
      $issue_ppubdate = date('F d, Y', strtotime($issue_node->field_highwire_a_ppubdate[X][0]['value']));
      $variables['date'] = l(date('F d, Y', strtotime($issue_node->field_highwire_a_ppubdate[X][0]['value'])), 'node/' . $issue_node->nid);

      // metadata
      $metadata = t('Vol. @vol Num. @num', array('@vol' => $issue_node->field_highwire_a_vol_num[X][0]['value'], '@num' => $issue_node->field_highwire_a_issue_num[X][0]['value'] ));

      $variables['metadata'] = $metadata;

      $issue_covers[] = theme('bmj_issue_cover', $variables);
    }

    $issue_cover_months[] = theme('item_list', array('items' => $issue_covers, 'title' => $month_name, 'attributes' => array('class' => array('issue-cover-month'))));
  }

  $block->content = theme('item_list', array('items' => $issue_cover_months, 'attributes' => array('class' => array('issue-cover-archive'))));
  return $block;
}

function jnl_bmj_bmj_cover_archive_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['cover_image_style'] = array(
    '#title' => t('Issue Cover image size'),
    '#type' => 'select',
    '#description' => t('Please select the image preset of Issue Cover Image be displayed'),
    '#default_value' => !empty($conf['cover_image_style']) ? $conf['cover_image_style'] : '',
    '#options' => cover_image_presets(),
  );

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function jnl_bmj_bmj_cover_archive_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Helper function to get the various image presets available
 */
function cover_image_presets() {
 foreach (image_styles() as $key => $val) {
    $image_size[$key] = $key;
  }

  return $image_size;
}

/**
 * Return the months array
 */
function _jnl_bmj_cover_archive($till_mnth) {
  $months = array(
    '01' => 'January',
    '02' => 'February',
    '03' => 'March',
    '04' => 'April',
    '05' => 'May',
    '06' => 'June',
    '07' => 'July',
    '08' => 'August',
    '09' => 'September',
    '10' => 'October',
    '11' => 'November',
    '12' => 'December'
  );

  // if till_mnth is not empty &
  // is not till_mnth is not Dec
  // then return all mnths
  if (!empty($till_mnth) && $till_mnth != '12') {
    foreach ($months as $month => $name) {
      if ($month <= $till_mnth) {
        continue;
      }
      unset($months[$month]);
    }
  }

  return $months;
}


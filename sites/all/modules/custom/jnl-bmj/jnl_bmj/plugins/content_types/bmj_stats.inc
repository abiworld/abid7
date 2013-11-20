<?php
/**
 * @file
 * BMJ Stats
 *
 * Copyright (c) 2010-2011 Board of Trustees, Leland Stanford Jr. University
 * This software is open-source licensed under the GNU Public License Version 2 or later
 * The full license is available in the LICENSE.TXT file at the root of this repository
 */
$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('BMJ Artice: Usage Statistics / Metrics'),
  'description' => t('Provides usage statistics for articles'),
  'category' => t('BMJ Article'),
  'required context' => new ctools_context_required(t('Article'), 'node'),
  'defaults' => array('logging' => 1),
);

function jnl_bmj_bmj_stats_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $block->module = 'jnl_bmj';

  //@@TODO Make date form configurable
  $date_format = 'F Y';
  $date_format_day = 'F Y d';

  $apath = $context->data->field_highwire_a_apath[X][0]['value'];

  $hwstats = new HWStats();
  $raw_stats = $hwstats->execute($apath);

  if ($raw_stats) {

    // Calculate totals
    $totals = array(
      'abstract' => 0,
      'full' => 0,
      'pdf' => 0,
    );
    foreach ($raw_stats as $stat) {
      $totals['abstract'] += $stat['abstract'];
      $totals['full'] += $stat['full'];
      $totals['pdf'] += $stat['pdf'];
    }

    // Build the headers
    $header = array('');
    if ($totals['abstract']) {
      $header[] = 'Abstract';
    }
    if ($totals['full']) {
      $header[] = 'Full';
    }
    if ($totals['pdf']) {
      $header[] = 'PDF';
    }

    // Build the rows
    $rows = array();
    foreach ($raw_stats as $stat) {
      $row = array();
      if (!empty($stat['month'])) {
        $row[] = date_translate($stat['month'], $date_format);
      }
      if (!empty($stat['day'])) {
        $row[] = date_translate($stat['day'], $date_format_day);
      }
      if ($totals['abstract']) {
        $row[] = $stat['abstract'];
      }
      if ($totals['full']) {
       $row[] = $stat['full'];
      }
      if ($totals['pdf']) {
        $row[] = $stat['pdf'];
      }
      $rows[] = $row;
    }

    if (!empty($rows)) {
      $rows = array_reverse($rows);
    }

    // Add the total row
    if (isset($conf['total_row']) && $conf['total_row']) {
      $row = array('data' => array('Total'), 'class' => array('highwire-stats-total'));
      if ($totals['abstract']) {
        $row['data'][] = $totals['abstract'];
      }
      if ($totals['full']) {
       $row['data'][] = $totals['full'];
      }
      if ($totals['pdf']) {
        $row['data'][] = $totals['pdf'];
      }
      $rows[] = $row;
    }

    $block->content = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('highwire-stats'))));

    if($conf['logging'] && function_exists('highwire_log')) {
      highwire_log('aus');
    }


    return $block;
  }
}

function jnl_bmj_bmj_stats_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['logging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Feature Logging'),
    '#default_value' => $conf['logging'],
  );

  $form['total_row'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show total row in usage'),
    '#default_value' => isset($conf['total_row']) ? $conf['total_row'] : 0,
    '#description' => "If set shows row of total count",
  );
  return $form;
}

/**
 * Validate the configuration.
 */
function jnl_bmj_bmj_stats_content_type_edit_form_validate(&$form, &$form_state) {
  $form_state['conf']['total_row'] = $form_state['values']['total_row'];
  $form_state['conf']['logging'] = $form_state['values']['logging'];
}


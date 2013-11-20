<?php
/**
 * @file
 * Bmj Rapid response form
 *
 * Copyright (c) 2010-2011 Board of Trustees, Leland Stanford Jr. University
 * This software is open-source licensed under the GNU Public License Version 2 or later
 * The full license is available in the LICENSE.TXT file at the root of this repository
 */

$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('Bmj Rapid responses Form'),
  'description' => t('Renders the rapid response form'),
  'category' => t('BMJ Archive'),
  'edit form' => 'bmj_rapid_response_content_type_edit_form',
  'required context' => new ctools_context_required(t('Node'), 'node')
);

function jnl_bmj_bmj_rapid_response_form_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $node = $context->data;
  module_load_include('inc', 'node', 'node.pages');
  $form = node_add('highwire_comment');
  $form['field_highwire_c_competing_int'][X]['#prefix'] = check_markup($conf['statement_cit_markup']['value'], $conf['statement_cit_markup']['format']);
  $block->content = drupal_render($form);

  return $block;
}

function bmj_rapid_response_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['statement_cit_markup'] = array(
    '#type' => 'text_format',
    '#title' => t('Markup for Statement block'),
    '#description' => t('Enter the body text for the block'),
    '#default_value' => isset($conf['statement_cit_markup']['value']) ? $conf['statement_cit_markup']['value'] : '',
    '#format' => isset($conf['statement_cit_markup']['format']) ? $conf['statement_cit_markup']['format'] : '',
  );

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function bmj_rapid_response_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

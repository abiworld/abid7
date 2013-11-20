<?php
/**
 * @file
 * BMJ Featured Hero
 *
 * Copyright (c) 2010-2011 Board of Trustees, Leland Stanford Jr. University
 * This software is open-source licensed Xer the GNU Public License Version 2 or later
 * The full license is available in the LICENSE.TXT file at the root of this repository
 */
$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('BMJ Read More Link'),
  'description' => t('Display a link to the provided path.'),
  'category' => t('All Pages'),
  //'required context' => new ctools_context_required(t('Article Node'), 'node'),
  'render callback' => 'bmj_bmj_read_more_link_content_type_render',
  'edit form' => 'bmj_bmj_read_more_link_content_type_edit_form',
);

function bmj_bmj_read_more_link_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();

  $block->content = '<div class="btn">';
  $block->content .= l($conf['text'], $conf['path'], array('html' => TRUE));
  $block->content .= '</div>';
  return $block;
}

/*
 * Ctool configuration form
 */
function bmj_bmj_read_more_link_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

    $form['text'] = array(
      '#type' => 'textfield', 
      '#title' => t('Text'), 
      '#description' => 'E.g. see all News & Views',
      '#default_value' => ($conf['text'])? $conf['text'] : NULL, 
      '#size' => 60,  
      '#required' => TRUE,
    );

    $form['path'] = array(
      '#type' => 'textfield', 
      '#title' => t('Path'), 
      '#default_value' => ($conf['path'])? $conf['path'] : NULL, 
      '#size' => 60,  
      '#required' => TRUE,
    );

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function bmj_bmj_read_more_link_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/*
 * Validation function
 */
function bmj_bmj_read_more_link_content_type_edit_form_validate($form, &$form_state) {
  
}

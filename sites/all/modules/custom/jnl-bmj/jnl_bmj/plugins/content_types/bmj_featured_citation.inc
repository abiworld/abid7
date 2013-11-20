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
  'title' => t('BMJ Featured Citation'),
  'description' => t('Display citations featured/scheduled in nodequeues.'),
  'category' => t('Highwire'),
  'required context' => new ctools_context_required(t('Article Node'), 'node'),
  'render callback' => 'bmj_bmj_featured_citation_content_type_render',
  'edit form' => 'bmj_bmj_featured_citation_content_type_edit_form',
);

function bmj_bmj_featured_citation_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();

  $block->content = theme('bmj_featured_citation', array(
    'node' => $context->data,
    'mode' => $conf['mode'],
    )
  );
  return $block;
}

/*
 * Ctool configuration form
 */
function bmj_bmj_featured_citation_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

    $form['mode'] = array(
      '#type' => 'select',
       '#title' => t('Display Mode'),
       '#options' => array(
          'hero' => t('Hero'),
          'photo_featured' => t('Photo Featured'),
          'editors_choice' => t('Editors Choice'),
          'homepage_slideshow' => t('Homepage Slideshow'),
       ),
       '#default_value' => $conf['mode'],
       '#description' => t('Select which output you would like to display.'),
    );

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function bmj_bmj_featured_citation_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/*
 * Validation function
 */
function bmj_bmj_featured_citation_content_type_edit_form_validate($form, &$form_state) {
  
}

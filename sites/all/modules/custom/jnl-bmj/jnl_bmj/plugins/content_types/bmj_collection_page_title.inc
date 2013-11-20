<?php
/**
 * @file
 * Bmj Collection Subterms
 *
 * Copyright (c) 2010-2011 Board of Trustees, Leland Stanford Jr. University
 * This software is open-source licensed under the GNU Public License Version 2 or later
 * The full license is available in the LICENSE.TXT file at the root of this repository
 */

$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('Bmj Collection Page title'),
  'description' => t('Renders the page-title on collection term pages with color class attached.'),
  'category' => t('BMJ Collections'),
  'required context' => new ctools_context_required(t('Parent Terms'), 'taxonomy_term'),
);

function jnl_bmj_bmj_collection_page_title_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();

  if ($context->data) {
    $term = $context->data;
  }
  else
    return NULL;

  $page_title = '<h1 class="collection-title-w-border ' . jnl_bmj_topic_colour_class($term->name) . '" id="page-title">' . $term->name . '</h1>';
  drupal_set_title($term->name);
  $block->content = $page_title;

  return $block;
}

function jnl_bmj_bmj_collection_page_title_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function jnl_bmj_bmj_collection_page_title_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

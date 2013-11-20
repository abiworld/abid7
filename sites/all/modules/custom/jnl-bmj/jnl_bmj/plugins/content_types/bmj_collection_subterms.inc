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
  'title' => t('Bmj Collection Children Terms'),
  'description' => t('Renders the children terms for a collection'),
  'category' => t('BMJ Collections'),
  'required context' => new ctools_context_required(t('Parent Terms'), 'taxonomy_term'),
);

function jnl_bmj_bmj_collection_subterms_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();

  if ($context->data) {
    $parent = $context->data;
    $children_terms = taxonomy_get_children($parent->tid);
  }
  else
    return NULL;

  foreach ($children_terms as $tid => $child_term) {
    $link_class = jnl_bmj_topic_colour_class($child_term->name);
    $child_terms[] = l($child_term->name, 'taxonomy/term/' . $child_term->tid, array('attributes' => array('class' => array($link_class))));
  }

  $title = $conf['override_title'] ? $conf['override_title_text'] : '';
  $block->title = $title;

  $output = '';
  if (count($child_terms)) {
    $output = theme('item_list', array('items' => $child_terms));
  }
  $block->content = $output;
  return $block;
}

function jnl_bmj_bmj_collection_subterms_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function jnl_bmj_bmj_collection_subterms_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

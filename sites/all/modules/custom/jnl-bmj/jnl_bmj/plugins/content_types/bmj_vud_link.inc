<?php
/**
 * @file
 * Bmj Podcast Links
 *
 * Copyright (c) 2010-2011 Board of Trustees, Leland Stanford Jr. University
 * This software is open-source licensed under the GNU Public License Version 2 or later
 * The full license is available in the LICENSE.TXT file at the root of this repository
 */

$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('Bmj Vud Link'),
  'description' => t('Renders the Vote up/down widget for the node.'),
  'category' => t('BMJ'),
  'required context' => new ctools_context_required(t('Article node'), 'node'),
);

function jnl_bmj_bmj_vud_link_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $node = $context->data;
  $node_view = node_view($node);
  $block->content = $node_view['vud_node_widget_display']['#value'];

  return $block;
}

function jnl_bmj_bmj_vud_link_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function jnl_bmj_bmj_vud_link_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

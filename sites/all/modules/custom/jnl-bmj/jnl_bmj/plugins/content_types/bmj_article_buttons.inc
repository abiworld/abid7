<?php
/**
 * @file
 * Bmj Article Buttons
 *
 * Theme functions can be found in jnl_bmj.module
 * Copyright (c) 2010-2011 Board of Trustees, Leland Stanford Jr. University
 * This software is open-source licensed under the GNU Public License Version 2 or later
 * The full license is available in the LICENSE.TXT file at the root of this repository
 */

$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('Bmj Article buttons'),
  'description' => t('Buttons for pdf, share links and respond to article'),
  'category' => t('BMJ Article'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'edit form' => 'jnl_bmj_bmj_article_buttons_content_type_edit_form',
);

function jnl_bmj_bmj_article_buttons_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $article_node = $context->data;

  $block->content = theme('bmj_article_buttons', array('node' => $article_node, 'conf' => $conf, 'mode' => $conf['mode']));
  return $block;
}

function jnl_bmj_bmj_article_buttons_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['container_id_share'] = array(
    '#type' => 'textfield',
    '#title' => t('Container Id for the share links'),
    '#description' => t('Using this container Id, we will display the share links in a popup'),
    '#default_value' => isset($conf['container_id_share']) ? $conf['container_id_share'] : ''
  );

  $form['container_id_response'] = array(
    '#type' => 'textfield',
    '#title' => t('Container Id for the respond to article content'),
    '#description' => t('Using this container Id, we will display the respond to this article in a popup'),
    '#default_value' => isset($conf['container_id_response']) ? $conf['container_id_response'] : ''
  );

  $form['mode'] = array(
    '#type' => 'select',
    '#title' => t('Display Mode'),
    '#options' => array(
      'article' => t('Article'), // PDF, Respond, Share
      'hero' => t('Hero'), // +, Respond, Read
      'citation' => t('Citation List'), // Respond
    ),
    '#default_value' => $conf['mode'],
    '#description' => t('Set this to <em>Yes</em> if you would like this category to be selected by default.'),
   );

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function jnl_bmj_bmj_article_buttons_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

<?php
/**
 * @file
 * Bmj Article response
 *
 * Copyright (c) 2010-2011 Board of Trustees, Leland Stanford Jr. University
 * This software is open-source licensed under the GNU Public License Version 2 or later
 * The full license is available in the LICENSE.TXT file at the root of this repository
 */

$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('Bmj Respond to this Article'),
  'description' => t('Comments for Article, It requires the node id of which comments are to be dispayed'),
  'category' => t('BMJ Article'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

function jnl_bmj_bmj_article_response_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $comment_ids = array();
  $article_node = $context->data;
  $comment_ids = highwire_get_comments($article_node->nid);
  $node = $context->data;
  $comments = '';
  $content = '';
  $article_response = array();
  $destination = drupal_get_destination();

  if($_GET['tab'] == 'responses') {
    drupal_add_js(drupal_get_path('module', 'jnl_bmj') . '/plugins/content_types/js/bmj_open_reponse_popover.js');
  }

  if($_GET['tab'] == 'response-form') {
    drupal_add_js(drupal_get_path('module', 'jnl_bmj') . '/plugins/content_types/js/bmj_open_reponse_form.js');
  }

  if($_GET['tab'] = 'rapid-responses') {
    drupal_add_js(drupal_get_path('module', 'jnl_bmj') . '/plugins/content_types/js/bmj_open_reponses_modal.js');
  }
  // if (!empty($conf['link_icon'])) {
    // $icon_class = '<i class="bmj-icon '.$conf['link_icon'].'"></i>';
  // }
  $icon_class = '<i class="bmj-icon icon-comment"></i>';
  if(isset($conf['response_link_only']) && ($conf['response_link_only'])) {
    if(isset($conf['redirect_to_plugin']) && ($conf['redirect_to_plugin'])) {
      $block->content = l($icon_class . t($conf['response_link_text']), 'node/add/highwire-comment/' . $node->nid, array('query' => array($destination), 'attributes' => array('class' => 'submit-a-e-letter', 'btn', 'btn-primary'), 'html' => TRUE));
    }
    else {
      $block->content = '<button class="submit-a-e-letter btn btn-primary" data-toggle="modal" data-target="#rapid-response-form">' . $icon_class . 'Respond to this Article</button>';
    }
  }
  else {
    if (isset($conf['response_link']) && ($conf['response_link'])) {
      $content = '<button class="submit-a-e-letter btn btn-primary" data-toggle="modal" data-target="#rapid-response-form">' . $icon_class . 'Respond to this Article</button>';
    }
    elseif(isset($conf['response_link']) && ($conf['response_link']) && ($conf['redirect_to_plugin'])) {
      $content = l($icon_class . t($conf['response_link_text']), 'node/add/highwire-comment/' . $node->nid, array('query' => array($destination), 'attributes' => array('class' => array('submit-a-e-letter', 'btn', 'btn-primary')), 'html' => TRUE));
    }

    if (is_array($comment_ids)) {
      foreach ($comment_ids as $key => $value) {
        $cnode = node_load($value);
        $article_response[] = theme('bmj_comments_list', array('node' => $cnode, 'conf' => $conf));
      }

      $article_response_list = theme('item_list', array('items' => $article_response, 'title' => '', 'attributes' => array('class' => array('bmj-comment'))));
      $content .= $article_response_list . $comments;
    }

    $block->content = $content;
  }

  return $block;
}

/**
 * Configuration form for plugin.
 */
function jnl_bmj_bmj_article_response_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['link_title'] = array(
    '#title' => t('Display article response title as link.'),
    '#type' => 'checkbox',
    '#description' => t('Select this box if you want to display article response title as link'),
    '#default_value' => !empty($conf['link_title']) ? $conf['link_title'] : '',
  );

  $form['response_link'] = array(
    '#title' => t('Display Submit a response link'),
    '#type' => 'checkbox',
    '#description' => t('Select this box if you want to display submit a e-letter link'),
    '#default_value' => !empty($conf['response_link']) ? $conf['response_link'] : '',
  );

  $form['response_link_text'] = array(
    '#title' => t('Text for response link'),
    '#type' => 'textfield',
    '#default_value' => !empty($conf['response_link_text']) ? $conf['response_link_text'] : 'Submit a Response to This E-letter',
  );

  $form['redirect_to_plugin'] = array(
    '#title' => t('Redirect to Caller page after submit'),
    '#type' => 'checkbox',
    '#description' => t('Redirect to the All comments associated to article page (Caller page)'),
    '#default_value' => !empty($conf['redirect_to_plugin']) ? $conf['redirect_to_plugin'] : '',
  );

  $form['response_link_only'] = array(
    '#title' => t('Display Only Submit a response link(Skip the comments posted)'),
    '#type' => 'checkbox',
    '#description' => t('Select this box if you want to display submit a e-letter link only'),
    '#default_value' => !empty($conf['response_link_only']) ? $conf['response_link_only'] : '',
  );

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function jnl_bmj_bmj_article_response_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

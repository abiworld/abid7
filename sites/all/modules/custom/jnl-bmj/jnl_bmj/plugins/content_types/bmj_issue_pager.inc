<?php
/**
 * @file
 * BMJ Issue Pager
 *
 * Copyright (c) 2010-2011 Board of Trustees, Leland Stanford Jr. University
 * This software is open-source licensed under the GNU Public License Version 2 or later
 * The full license is available in the LICENSE.TXT file at the root of this repository
 */

/**
 * Announcing metadata for the ctools content type  'Highwire node Pager'
 */
$plugin = array(
  'title'            => t('BMJ Issue Pager'),
  'description'      => t('Node Pager (Next/Previous) Links'),
  'single'           => TRUE,
  'category'         => t('BMJ Archive'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

/**
 * Rendering function for the 'highwire_node_pager' content type.
 * This will return the list of versions that nodes has
 */
function jnl_bmj_bmj_issue_pager_content_type_render($subtype, $conf, $panel_args, &$context) {
  $node = $context->data;
  $nid = $node->nid;
  $prev_next_nodes = highwire_prev_next_from_nid($nid);
  $pager_vars = array('attributes' => array('class' => array('highwire-node-pager')));
  $pager_vars['attributes']['class'][] = drupal_html_class($node->type) . '-pager';

  if ($prev_next_nodes['prev']) {
    $prev_node = node_load($prev_next_nodes['prev']);
    $pager_vars['prev'] = array(
      'title' => $conf['previous_link_title'],
      'href' => 'node/' . $prev_node->nid,
      'attributes' => array(
        'title' => $prev_node->title,
        'data-font-icon' => $conf['previous_link_icon'],
        'rel' => 'prev',
      ),
    );
  }
  if ($prev_next_nodes['next']) {
    $next_node = node_load($prev_next_nodes['next']);
    $pager_vars['next'] = array(
      'title' => $conf['next_link_title'],
      'href' => 'node/' . $next_node->nid,
      'attributes' => array(
        'title' => $next_node->title,
        'data-font-icon' => $conf['next_link_icon'],
        'rel' => 'next',
      ),
    );
  }

  $block = new stdClass();
  $block->title = '';

  $block->content = theme('bmj_issue_pager', $pager_vars);
  return $block;
}

/**
 * The form to add or edit an node pager as content.
 */
function jnl_bmj_bmj_issue_pager_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['previous_link'] = array(
    '#type' => 'fieldset',
    '#title' => t('Previous Link'),
    '#description' => t('Set the previous link text & icon.'),
  );
  $form['previous_link']['previous_link_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Text'),
    '#default_value' => isset($conf['previous_link_title']) ? $conf['previous_link_title'] : t('Previous'),
  );
  $form['previous_link']['previous_link_icon'] = array(
    '#type' => 'textfield',
    '#title' => t('Icon'),
    '#description' => t('Include the entire icon class (i.e.: "icon-angle-right"). A full list of icons can be found <a href="http://fortawesome.github.com/Font-Awesome/#icons-new" target="_blank">here</a>.'),
    '#default_value' => isset($conf['previous_link_icon']) ? $conf['previous_link_icon'] : 'icon-circle-arrow-left',
  );

  $form['next_link'] = array(
    '#type' => 'fieldset',
    '#title' => t('Next Link'),
    '#description' => t('Set the next link text & icon.'),
  );
  $form['next_link']['next_link_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Text'),
    '#default_value' => isset($conf['next_link_title']) ? $conf['next_link_title'] : t('Next'),
  );
  $form['next_link']['next_link_icon'] = array(
    '#type' => 'textfield',
    '#title' => t('Icon'),
    '#description' => t('Include the entire icon class (i.e.: "icon-angle-right"). A full list of icons can be found <a href="http://fortawesome.github.com/Font-Awesome/#icons-new" target="_blank">here</a>.'),
    '#default_value' => isset($conf['next_link_icon']) ? $conf['next_link_icon'] : 'icon-circle-arrow-right',
  );

  return $form;
}

/**
 * Store the node pager configuration.
 */
function jnl_bmj_bmj_issue_pager_content_type_edit_form_submit($form, &$form_state) {
  $fields = array('previous_link_title', 'previous_link_icon', 'next_link_title', 'next_link_icon');
  foreach ($fields as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

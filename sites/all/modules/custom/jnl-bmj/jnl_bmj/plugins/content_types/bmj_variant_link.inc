<?php
/**
 * @file
 * Highwire variant_link
 *
 * Copyright (c) 2010-2011 Board of Trustees, Leland Stanford Jr. University
 * This software is open-source licensed under the GNU Public License Version 2 or later
 * The full license is available in the LICENSE.TXT file at the root of this repository
 */

$plugin = array (
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('BMJ: Variant Link'),
  'description' => t('Provides a simple link to a single variant'),
  'category' => t('BMJ Article'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'render callback'  => 'jnl_bmj_bmj_variant_link_render',
  'edit form'        => 'jnl_bmj_bmj_variant_link_edit_form'
);

function jnl_bmj_bmj_variant_link_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $block->title = '';
  $node = $context->data;

  $valid_variants = highwire_markup_get_node_variants($node);

  if (in_array($conf['variant'], $valid_variants)) {
    $content = '';

    $variant = highwire_markup_get_variants($conf['variant']);
    $variant_title = $variant['title'];

    $token_conf = array('node' => $node);

    if(!empty($conf['prelabel'])){
      $content .= '<span class="highwire-variant-link-pre-label label">' . token_replace($conf['prelabel'], $token_conf) . '</span>';
    }

    $options = array(
      'html' => TRUE,
    );

    if (!empty($conf['link_icon'])) {
      $options['attributes']['data-font-icon'] = $conf['link_icon'];
      $icon_class = '<i class="bmj-icon '.$conf['link_icon'].'"></i>';
    }

    if ($conf['target']) {
      $options['attributes']['target'] = $conf['target'];
    }

    $content .= $icon_class . l($conf['label'] ? token_replace($conf['label'], $token_conf) : $variant_title, drupal_get_path_alias('node/' . $node->nid) . '.' . $conf['variant'], $options);

    $block->content = $content;
    return $block;
  }
}

function jnl_bmj_bmj_variant_link_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['variant'] = array(
    '#type' => 'select',
    '#title' => t('Variant'),
    '#default_value' => @$conf['variant'],
    '#options' => highwire_markup_get_variant_options(),
    '#required' => TRUE,
  );

  $form['prelabel'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#description' => 'Label will be placed just before the link text',
    '#default_value' => !empty($conf['prelabel']) ? $conf['prelabel'] : '',
  );

  $form['label'] = array(
    '#title' => t('Link Text'),
    '#type' => 'textfield',
    '#description' => 'Please enter the link text. Leave empty to display the title of the variant',
    '#default_value' => !empty($conf['label']) ? $conf['label'] : '',
  );

  $form['link_icon'] = array(
    '#type' => 'textfield',
    '#title' => t('Link icon'),
    '#description' => t('Include the entire icon class (i.e.: "icon-angle-right"). A full list of icons can be found <a href="http://fortawesome.github.com/Font-Awesome/#icons-new" target="_blank">here</a>.'),
    '#default_value' => isset($conf['link_icon']) ? $conf['link_icon'] : 'icon-link',
  );

  $form['target'] = array(
    '#title' => t('Link Target'),
    '#type' => 'textfield',
    '#description' => 'Please enter the link target, leave blank to open in current window.',
    '#default_value' => !empty($conf['target']) ? $conf['target'] : '',
  );

  return $form;
}

function jnl_bmj_bmj_variant_link_edit_form_submit(&$form, &$form_state) {
  foreach (array('variant', 'prelabel', 'label', 'link_icon', 'target') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

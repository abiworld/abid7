<?php
/**
 * @file
 * Highwire Issue Cover Image
 *
 * Copyright (c) 2010-2011 Board of Trustees, Leland Stanford Jr. University
 * This software is open-source licensed under the GNU Public License Version 2 or later
 * The full license is available in the LICENSE.TXT file at the root of this repository
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
    'single' => TRUE,
    'icon' => 'icon_node_form.png',
    'title' => t('BMJ Issue cover'),
    'description' => t('Provides issue cover image'),
    'category' => t('BMJ'),
    'required context' => new ctools_context_required(t('Issue Node'), 'node'),
  );


/**
 * Output function for the 'issue_cover' content type.
 */
function jnl_bmj_bmj_issue_cover_content_type_render($subtype, $conf, $panel_args, &$context) {
  ctools_include('plugins');
  if (empty($context->data)) {
    return "";
  }

  $node = $context->data;

  // Get the image for the current issue
  // get the issue cover image details
  $issue_cover_details = $node->field_highwire_issue_cover[X][0];
  $variables = $arributes = array();

  // get the image size
  $image_size = isset($conf['cover_size'])? $conf['cover_size'] : 'thumbnail';

  // the details about image
  foreach (array('alt', 'title') as $key) {
    $variables[$key] = $issue_cover_details[$key];
  }
  $variables['style_name'] = $image_size;

  $variants = highwire_markup_get_node_variants($node);
  if ($conf['variant_link'] != 'none' && $conf['variant_link'] != 'default' && in_array($conf['variant_link'], $variants)) {
    $link_url = drupal_get_path_alias('node/' . $node->nid) . '.' . $conf['variant_link'];
  }
  else if ($conf['variant_link'] == 'default') {
    $link_url = drupal_get_path_alias('node/' . $node->nid);
  }
  else {
    $link_url = FALSE;
  }

  if (file_exists($issue_cover_details['uri'])) {
    $variables['path'] = $issue_cover_details['uri'];
    $theme_func = 'image_style';
  }
  else {
    $variables['path'] = drupal_get_path('module', 'highwire') . '/images/cover_default_m.png';
    $theme_func = 'image';
  }

  if ($link_url) {
    $issue_cover = l(theme($theme_func, $variables), $link_url,  $options = array('html' => TRUE));
  }
  else {
    $issue_cover = theme($theme_func, $variables);
  }

  $block = new stdClass();
  $block->content = '<div id="issue-cover-wrapper">';
  $block->content .= theme('bmj_content_type_issue_cover', array('issue_cover' => $issue_cover));
  if ($conf['show_issue_links']) {
    $block->content .= '<a href="#issue-links-popover" role="button" id="issue-cover-plus" class="btn issue-cover-plus-processed" data-toggle="popover" data-original-title="" title=""><span class="icon"><i class="bmj-icon blue-icon icon-plus"></i></span></a>';
    drupal_add_js(drupal_get_path('module', 'jnl_bmj') . '/plugins/content_types/js/bmj_issue_links_popover.js');
  }
  $block->content .= "</div>";
  return $block;
}

/**
 * Returns HTML for issue cover image with link to issue
 * cover-expansion (if present) or to the issue toc.
 * @params $variable
 * An associative array containing
 * -issue_title: Title of the issue (Optional)
 * -issue_cover: Image cover if present or default cover image (Required)
 */
function theme_bmj_content_type_issue_cover($variables) {
  $output  = '<div class = "cover-issue">';
  $output .= '<div class = "cover-issue-image">' . $variables['issue_cover'] . '</div>';
  $output .= '</div>';
  return $output;
}

/**
 * The form to add or edit a issue_cover as content.
 */
function jnl_bmj_bmj_issue_cover_content_type_edit_form($form, &$form_state) {
  ctools_include('plugins');
  $conf = $form_state['conf'];

  // Get a list of issue variants
  $variant_options = array(
    'none' => 'No Link',
    'default' => 'Default Variant',
  );

  foreach (highwire_markup_get_variants() as $variant) {
    if (in_array('issue', $variant['atom-types'])) {
      $variant_options[$variant['variant']] = $variant['title'];
    }
  }

  $form['cover_size'] = array(
    '#title' => t('Image size'),
    '#type' => 'select',
    '#description' => t('Please select the image size to be display'),
    '#default_value' => !empty($conf['cover_size']) ? $conf['cover_size'] : '',
    '#options' => _cover_image_size(),
  );

  $form['variant_link'] = array(
    '#type' => 'select',
    '#options' => $variant_options,
    '#title' => t('Link to Issue Variant'),
    '#default_value' => $conf['variant_link'] ? $conf['variant_link'] : 'none',
    '#description' => t("Link to this issue variant when the image is clicked"),
  );

  $form['show_issue_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Issue links below'),
    '#default_value' => $conf['show_issue_links'] ? $conf['show_issue_links'] : 0,
    '#description' => t("Enable to show issue links below the issue cover"),
  );
  //@@TODO: No cover behavior

  return $form;
}

/**
 * Store the issue_cover selection.
 */
function jnl_bmj_bmj_issue_cover_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['cover_size'] = $form_state['values']['cover_size'];
  $form_state['conf']['variant_link'] = $form_state['values']['variant_link'];
  $form_state['conf']['show_issue_links'] = $form_state['values']['show_issue_links'];
}

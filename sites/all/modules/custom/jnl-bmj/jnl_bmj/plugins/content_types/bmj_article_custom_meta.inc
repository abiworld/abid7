<?php
/**
 * @file
 *
 * Article - This article Custom-Meta pane.
 */

/**
 * Description of the plugin.
 */
$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => ('BMJ Custom Meta'),
  'description' => t('Supplies linked icons for the BMJ custom meta tags associated with an article (Fast-Track, CME, endgames etc)'),
  'category' => array(t('BMJ Article'), 0),
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

/**
 * Callback to provide administrative title of pane.
 */
function jnl_bmj_bmj_article_custom_meta_content_type_admin_title($subtype, $conf, $context = NULL) {
  return t('Custom Meta Icons and Links');
}

/**
 * Callback to provide pane body in panel building display.
 */
function jnl_bmj_bmj_article_custom_meta_content_type_admin_info($subtype, $conf, $context = NULL) {
  $pane = new stdClass();
  $pane->title = t('Displays CME, fast-track, endgames &c.');
  $pane->content ='';
  return $pane;
}

/**
 * Render callback for the content type.
 */
function jnl_bmj_bmj_article_custom_meta_content_type_render($subtype, $conf, $args, $context) {
  $block = new stdclass();
  $block->title = '';
  $block->content = '';

  $custom_meta = bmj_article_get_custom_meta($context->data);

  if (!empty($custom_meta['imagename'])) {
    $image = array();
    $image['path'] = drupal_get_path("module", "jnl_bmj") . "/plugins/content_types/bmj_article_custom_meta_images/" . $custom_meta['imagename'];
    $image['attributes'] = array('class' => $custom_meta['meta']);

    $block->content = l(theme_image($image), $custom_meta['urlname'], array('html' => TRUE,));
  }
  return $block;
}

/**
 * 'Edit form' callback for the content type.
 */
function jnl_bmj_bmj_article_cutom_meta_content_type_edit_form($form, &$form_state) {
  return $form;
}


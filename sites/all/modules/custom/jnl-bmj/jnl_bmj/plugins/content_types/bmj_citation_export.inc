<?php
/**
 * @file
 * BMJ Citation Export Plugin
 *
 */

/**
 * Announcing metadata for the ctools content type  'BMJ Citation Export'
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('BMJ Citation Export'),
  'description' => t('Output the item in a form suitable for various external citation managers.'),
  'admin info' => 'jnl_bmj_bmj_citation_export_content_type_admin_info',
  'content_types' => 'highwire_citation_export',
  'required context' => new ctools_context_required(t('Article'), 'node'),
  'category' => t('BMJ Article'),
  'defaults' => array('link_text' => t('Download Citation')),
);

/**
 * Output function for the 'bmj_citation_export' content type.
 *
 * @param type $subtype
 * @param type $conf
 * @param type $panel_args
 * @param type $context
 */
function jnl_bmj_bmj_citation_export_content_type_render($subtype, $conf, $panel_args, $context) {

  $node = $context->data;

  drupal_add_js(drupal_get_path('module', 'highwire_citation') . '/plugins/content_types/js/highwire_citation_export.js', 'file');

  $var = array();
  $var['node'] = $node;
  $var['conf'] = $conf;
  $var['inline'] = 'block';
  $output = theme('highwire_citation_export', $var);

  $block = new stdClass();
  $block->title = '';
  $block->content = $output;
  return $block;
}

/**
 * Return an edit form for the content type.
 * @param array $form
 * @param array $form_state
 */
function jnl_bmj_bmj_citation_export_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form = array_merge($form,  highwire_article_citation_options_form($conf));

  return $form;
}

function jnl_bmj_bmj_citation_export_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Return title for display in the admin panel.
 * @param type $subtype
 * @param array $conf
 * @param array $context
 * @return string
 */
function jnl_bmj_bmj_citation_export_content_type_admin_title($subtype, $conf, $context) {
  return t('Citation Export');
}




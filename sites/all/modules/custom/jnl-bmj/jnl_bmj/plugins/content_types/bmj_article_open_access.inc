<?php
/**
 * @file
 * Article - Displays Open Access logo for article page
 */

/**
 * Description of the plugin.
 */
$plugin = array(
  'title'           => t('BMJ Open Access'),
  'description'     => t('Displays Open Access logo.'),
  'category'        => t('BMJ Article'),
  'single'          => TRUE,
  'required context' => new ctools_context_required(t('Article Node'), 'node'),
);

/**
 * Callback to provide administrative title of pane.
 */
function jnl_bmj_bmj_article_open_access_content_type_admin_title($subtype, $conf, $context = NULL) {
  return t('Open Access');
}

/**
 * Callback to provide pane body in panel building display.
 */
function jnl_bmj_bmj_article_open_access_content_type_admin_info($subtype, $conf, $context = NULL) {
  $pane = new stdClass();
  $pane->title = t('Displays Open Access logo.');
  $pane->content ='';
  return $pane;
}

function jnl_bmj_bmj_article_open_access_content_type_render($subtype, $conf, $args, $context) {
  // Get node from context. Escape if not an article.
  if (empty($context) || empty($context->data)) {
    return;
  }
  $node = $context->data;
  if ($node->type != 'highwire_article') {
    return;
  }

  $oa_details = _bmj_get_open_access_details($node);
  if (!empty($oa_details)) {
    $open_access_html = '<div class="creative-commons"><span class="creative-commons-article">' . $oa_details['open_access'] . ' <span>Open access</span></span></div>';
  }
  else {
    return '';
  }

  $block = new stdClass();
  $block->title = '';
  $block->content = $open_access_html;
  return $block;
}



/**
 * 'Edit form' callback for the content type.
 */
function jnl_bmj_bmj_article_open_access_content_type_edit_form($form, &$form_state) {
  return $form;
}

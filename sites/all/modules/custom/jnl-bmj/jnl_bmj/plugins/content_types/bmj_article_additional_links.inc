<?php
/**
 * @file
 * BMJ Article links - Additional Data
 *
 */
$plugin = array(
  'title'            => t('BMJ Article links - Additional Data'),
  'description'      => t('BMJ Article links'),
  'single'           => TRUE,
  'category'         => t('BMJ Article'),
  'required context' => new ctools_context_required(t('Article node'), 'node'),
);

/**
 * Callback to provide pane title in panel building display.
 */
function jnl_bmj_bmj_article_additional_links_content_type_admin_title($subtype, $conf, $context = NULL) {
  return t('BMJ Article links - Additional Data');
}

/**
 * Output for Article links
 */
function jnl_bmj_bmj_article_additional_links_content_type_render($subtype, $conf, $panel_args, &$context) {
  $article_node = $context->data;
  if (empty($article_node->field_bmj_additional_article_inf) && empty($article_node->field_bmj_additional_article_inf[X][0]['target_id'])) {
    return '';
  }

  $article_data_nid = $article_node->field_bmj_additional_article_inf[X][0]['target_id'];
  $article_data_node = node_load($article_data_nid);

  $bmj_links_collection = $article_data_node->field_bmj_links_collection;
  $article_links = '';
  if (count($bmj_links_collection)) {
    $item_ids = array();
    foreach ($bmj_links_collection[X] as $key => $value) {
      $item_ids[$value['value']] = $value['value'];
    }
    $bmj_links = entity_load('field_collection_item', $item_ids);

    $links = array();
    foreach ($bmj_links as $key => $value) {
      $links[] = array(
        'title' => $value->field_bmj_article_links[X][0]['title'],
        'href' => $value->field_bmj_article_links[X][0]['url'],
        'attributes' => array('target' => '_blank'),
      );
    }

    $article_links = '<div class="bmj-article-links-collection">';
    $article_links .= theme('links', array('links' => $links, 'attributes' => array('class' => array('links'))));
    $article_links .= '</div>';
  }

  $block = new stdClass();
  $title = ($conf['override_title'] && !empty($conf['override_title_text'])) ? $conf['override_title_text'] : '';
  $block->title = $title;
  $block->content = $article_links;

  return $block;
}

/**
 * The form to add or edit a article links as content.
 */
function jnl_bmj_bmj_article_additional_links_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  return $form;
}


<?php
/**
 * @file
 *
 * Related articles
 */

/**
 * Description of the plugin.
 */
$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('Related articles'),
  'description' => t('Displays links and references to content related to the current article.'),
  'category' => array(t('BMJ Article'), 0),
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

/**
 * Callback to provide pane title in panel building display.
 */
function jnl_bmj_bmj_related_articles_content_type_admin_title($subtype, $conf, $context = NULL) {
  return t('Related content');
}

/**
 * Callback to provide pane info in panel building display.
 */
function jnl_bmj_bmj_related_articles_content_type_admin_info($subtype, $conf, $context = NULL) {
  $pane = new stdClass();
  $pane->title = t('Displays links and references to content related to the current article.');
  $pane->content = '';
  return $pane;
}

/**
 * Run-time rendering of the pane.
 */
function jnl_bmj_bmj_related_articles_content_type_render($subtype, $conf, $args, $context) {

  // No context? Not a node.
  if (empty($context) || empty($context->data)) {
    return;
  }

  // Get the node.
  $node = $context->data;

  // Not an article?
  if ($node->type != 'highwire_article') {
    return;
  }

  $display_list = isset($conf['display_list']) ? $conf['display_list'] : 0;
  $no_results_text = !empty($conf['no_results_text']) ? t('@title', array('@title' => $conf['no_results_text'])) : '';
  if ($display_list) {
    // get the related articles
    $related = highwire_get_related($node->nid);

    $articles = array();
    foreach ($related as $item) {
      $articles[] = $item['entity_id'];
    }

    if (!empty($articles)) {
      $nodes = node_load_multiple($articles, array('type' => 'highwire_article'));

      if (!empty($nodes)) {
        foreach ($nodes as $nid => $article) {
          $items[] = theme('highwire_article_citation', array('node' => $article, 'conf' => $conf));
        }
      }
    } elseif ($no_results_text) {
      $items[] = '<div class="no-results">' . $no_results_text . '</div>';
    }
  }

  $title = (isset($conf['title']) && (!empty($conf['title']))) ? $conf['title'] : t('Related Articles');
  $block->title = $title;
  if (!empty($items) || !empty($footer)) {
    $block->content = theme('article_citation_list', array('items' => $items, 'class' => 'bmj-related-articles',  'attributes' => array('class' => array('bmj-related-articles-list')), 'nowrap' => TRUE));
    return $block;
  }
  else {
    return FALSE;
  }

}

/*
 * Ctool configuration form
 */
function jnl_bmj_bmj_related_articles_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $citation_options = highwire_article_citation_options_form($conf);
  foreach($citation_options as &$form_item) {
    $form_item['#states'] = array(
      'invisible' => array(
        ':input[name="display_list"]' => array('checked' => FALSE),
      ),
    );
  }

  $form = array_merge($form, $citation_options);

  $form['display_list'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Citing Articles list'),
    '#description' => t('Disable this option if you only want to show the external links.'),
    '#default_value' => isset($conf['display_list']) ? $conf['display_list'] : 1,
    '#weight' => 0,
  );

  $form_items = array('context', 'aligner_start', 'override_title', 'override_title_text', 'aligner_stop', 'override_title_markup');
  $weight = 0 - count($form_items);

  foreach ($form_items as $key) {
    $form[$key]['#weight'] = $weight;
    $weight++;
  }

  $form['no_results_text'] = array(
    '#type' => 'textfield',
    '#title' => t('No Results Message'),
    '#description' => t('Text to be displayed if there are no related articles found. Leave empty to hide this message.'),
    '#default_value' => isset($conf['no_results_text']) ? $conf['no_results_text'] : 'No related articles found.',
    '#states' => array(
      'invisible' => array(
        ':input[name="display_list"]' => array('checked' => FALSE),
      ),
    ),
  );

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function jnl_bmj_bmj_related_articles_content_type_edit_form_submit(&$form, &$form_state) {
  // Exclude unnecessary elements.
  form_state_values_clean($form_state);
  foreach ($form_state['values'] as $key => $value) {
    $form_state['conf'][$key] = $value;
  }
}

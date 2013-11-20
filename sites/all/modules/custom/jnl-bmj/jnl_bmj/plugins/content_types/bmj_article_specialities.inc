<?php

/**
 * @file
 *
 * Pane content type that shows the collection topics of an article.
 */

/**
 * Description of the plugin.
 */
$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title'           => t('Specialities topics for article'),
  'description'     => t('Displays the Specialities topics with which the article is associated.'),
  'category' => t('BMJ Article'),
  'required context' => new ctools_context_required(t('Node'), 'node')
);

/**
 * Run-time rendering of the pane.
 *
 * @param $subtype
 * @param $conf
 *   Configuration as done at admin time
 * @param $args
 * @param $context
 *   Context - in this case we don't have any
 *
 * @return
 *   An object with at least title and content members
 */
function jnl_bmj_bmj_article_specialities_content_type_render($subtype, $conf, $panel_args, &$context) {
  // Get node from context. Escape if not an article.
  if (empty($context) || empty($context->data)) {
    return;
  }

  $node = $context->data;
  if ($node->type != 'highwire_article') {
    return;
  }

  if (isset($conf['excluding_terms'])) {
    $excluding_terms = explode(',', $conf['excluding_terms']);
  }
  else {
    $excluding_terms = array('uk', 'us', 'india', 'local', 'clinical', 'non-clinical', 'special collection', 'bmj series', 'bmj', 'topic', 'target region');
  }

  $article_collections = $node->field_highwire_a_collections[X];

  $vocabularies = taxonomy_get_vocabularies();
  foreach ($vocabularies as $key => $vocab) {
    if ($vocab->machine_name == 'highwire_collection') {
      $collections_vid = $key;
      break;
    }
  }

  foreach ($article_collections as $term) {
    $article_specialities_term = taxonomy_term_load($term['tid']);
    if (($article_specialities_term->vid == $collections_vid) && (array_search(strtolower($article_specialities_term->name), $excluding_terms) === FALSE) && ($article_specialities_term->field_highwire_collection_jid[X][0]['value'])) {
      $link_class = jnl_bmj_topic_colour_class($article_specialities_term->name);
      $article_specialities[] = l($article_specialities_term->name, 'taxonomy/term/' . $article_specialities_term->tid, array('attributes' => array('class' => array('article-speciality', $link_class))));
    }
  }

  $title = $output = '';
  if (count($article_specialities)) {
    $title = t('Specialities');
    $variables = array();
    $variables['items'] = $article_specialities;
    $variables['limit'] = $conf['limit'];
    $variables['more-link'] = $conf['more-link'];
    $variables['less-link'] = $conf['less-link'];
    $variables['attributes'] = array('class' => 'item-list-speciality');
    $output = theme('bmj_item_list_more_less', $variables);
  }

  $block = new stdClass();
  if ($conf['override_title'] && !empty($conf['override_title_text'])) {
    $title = $conf['override_title_text'];
  }

  $block->title = $title;
  $block->content = $output;

  return $block;
}


/**
 * 'Edit form' callback for the content type.
 */
function jnl_bmj_bmj_article_specialities_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['excluding_terms'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter the terms to be excluded while filtering out specialities'),
    '#description' => t('Enter a comma seperated list of terms to be excluded'),
    '#default_value' => isset($conf['excluding_terms']) ? $conf['excluding_terms'] : '',
  );

  $form['limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of terms to display'),
    '#default_value' => $conf['limit'],
    '#description' => t('Number of terms/items to display.'),
  );

  $form['show_hide_link']['more_link_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Text'),
    '#prefix' => '<h2>' . t('"Show More" Link') . '</h2>',
    '#default_value' => isset($conf['more-link']['title']) ? $conf['more-link']['title'] : 'Show More',
    '#description' => t('Set the text for the "Show More" link.'),
  );
  $form['show_hide_link']['more_link_icon'] = array(
    '#type' => 'textfield',
    '#title' => t('Icon'),
    '#default_value' => isset($conf['more-link']['icon']) ? $conf['more-link']['icon'] : 'icon-arrow-down',
     '#description' => t('Include the entire icon class (i.e.: "icon-arrow-down"). A full list of icons can be found <a href="http://fortawesome.github.com/Font-Awesome/#icons-new" target="_blank">here</a>.'),
  );
  $form['show_hide_link']['less_link_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Text'),
    '#prefix' => '<h2>' . t('"Show Less" Link') . '</h2>',
    '#default_value' => isset($conf['more-link']['title']) ? $conf['less-link']['title'] : 'Show Less',
    '#description' => t('Set the text for the "Show Less" link.'),
  );
  $form['show_hide_link']['less_link_icon'] = array(
    '#type' => 'textfield',
    '#title' => t('Icon'),
    '#default_value' => isset($conf['more-link']['icon']) ? $conf['less-link']['icon'] : 'icon-arrow-up',
     '#description' => t('Include the entire icon class (i.e.: "icon-arrow-up"). A full list of icons can be found <a href="http://fortawesome.github.com/Font-Awesome/#icons-new" target="_blank">here</a>.'),
  );

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function jnl_bmj_bmj_article_specialities_content_type_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['excluding_terms'] = $form_state['values']['excluding_terms'];
  $form_state['conf']['limit'] = $form_state['values']['limit'];

  $form_state['conf']['more-link']['title'] = $form_state['values']['more_link_title'];
  $form_state['conf']['more-link']['icon'] = $form_state['values']['more_link_icon'];

  $form_state['conf']['less-link']['title'] = $form_state['values']['less_link_title'];
  $form_state['conf']['less-link']['icon'] = $form_state['values']['less_link_icon'];
}

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
  'title'           => t('BMJ collection children'),
  'description'     => t('Displays the children terms for the selected parent.'),
  'category' => t('BMJ Archive'),
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
function jnl_bmj_bmj_collection_children_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $parent_tid = $conf['parent_term'];

  $children_terms = taxonomy_get_children($parent_tid);

  $block->content = theme('bmj_specialities_two_col', array('children' => $children_terms, 'count' => count($children_terms)));
  return $block;
}


/**
 * 'Edit form' callback for the content type.
 */
function jnl_bmj_bmj_collection_children_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $options = jnl_bmj_topics();
  $form['parent_term'] = array(
    '#type' => 'select',
    '#title' => t('Select the parent term:'),
    '#description' => t('Children terms for this parent terms will be rendered'),
    '#options' => $options,
    '#default_value' => isset($conf['parent_term']) ? $conf['parent_term'] : ''
  );

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function jnl_bmj_bmj_collection_children_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}


function jnl_bmj_topics() {
  $options = array();
  $query = new EntityFieldQuery();
  $vocab = taxonomy_vocabulary_machine_name_load('highwire_collection');

  $results = $query->entityCondition('entity_type', 'taxonomy_term')
    ->propertyCondition('vid', $vocab->vid)
    ->fieldCondition('field_highwire_collection_ttype', 'value', 'branch')
    ->execute();

  if(count($results)) {
    $taxonomy_terms = array_shift($results);
    foreach($taxonomy_terms as $term) {
      $term_option = taxonomy_term_load($term->tid);
      $options[$term->tid] = $term_option->name;
    }
  }

  return $options;
}

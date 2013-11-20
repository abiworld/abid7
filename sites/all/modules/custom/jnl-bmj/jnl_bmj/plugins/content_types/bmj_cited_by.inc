<?php
/**
 * @file
 * Highwire Cited By
 *
 * Copyright (c) 2010-2011 Board of Trustees, Leland Stanford Jr. University
 * This software is open-source licensed under the GNU Public License Version 2 or later
 * The full license is available in the LICENSE.TXT file at the root of this repository
 */

$plugin = array(
  'title'            => t('Cited By'),
  'description'      => t('Article Cited By'),
  'single'           => TRUE,
  'category'         => t('BMJ Article'),
  'required context' => new ctools_context_required(t('Article'), 'node'),
);

/**
 * Output for Cited By
 */
function jnl_bmj_bmj_cited_by_content_type_render($subtype, $conf, $panel_args, &$context) {
  $node = $context->data;
  $data = FALSE;
  $vars = array();
  $links = array();
  $no_results_text = !empty($conf['no_results_text']) ? t('@title', array('@title' => $conf['no_results_text'])) : '';

  $title_with_link = isset($conf['title_with_link']) ? $conf['title_with_link'] : 0;
  $show_variant_links = isset($conf['show_variant_links']) ? $conf['show_variant_links'] : 0;
  $display_list = isset($conf['display_list']) ? $conf['display_list'] : 0;

  // Add highwire class for consistent theming
  $vars['attributes']['class'][] = 'highwire-article-citation-list';

  //Highwire Reference Links
  $vars['items'] = array();
  if ($display_list) {
    // Query the service
    $cited_by = new HWCitedBy($node->field_highwire_a_pisa_master[X][0]['value']);
    $cited_by_articles = $cited_by->execute();

    if (!empty($cited_by_articles)) {
      $data = TRUE;
      foreach ($cited_by_articles as $article) {
        $variant_links = array();
        $title = '<div class="highwire-cite-title">' . t('@title', array('@title' => $article['title'])) . '</div>';

        if ($title_with_link) {
          $title = l($title, $article['url'], array('html' => TRUE));
        }

        if ($show_variant_links) {
          foreach ($article['variants'] as $type => $variant) {
            $variant_links['variant-' . $type] = array(
              'title' => t('@title', array('@title' => $variant['title'])),
              'href'  => $variant['url'],
            );
          }
        }

        $item_list['data'] = $title;
        $item_list['data'] .= '<div class="variant-links">';
        $item_list['data'] .= theme('links', array('links' => $variant_links, 'attributes' => array('class' => array('links', 'inline'))));
        $item_list['data'] .= '</div>';

        $vars['items'][] = $item_list;
      }
    } elseif (!empty($no_results_text)) {
      $vars['items'][] = '<div class="no-results">' . $no_results_text . '</div>';
    }
  }

  $block = new stdClass();
  $title = $conf['override_title'] ? $conf['override_title_text'] : t('Cited By...');
  $block->title = $title;

  if ($data) {
    $block->content = theme('highwire_list', $vars);
  }

  return $block;
}

/**
 * The form to add or edit a Cited By as content.
 */
function jnl_bmj_bmj_cited_by_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['display_list'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Citing Articles list'),
    '#description' => t('Disable this option if you only want to show the external links.'),
    '#default_value' => isset($conf['display_list']) ? $conf['display_list'] : 1,
  );

  $form['title_with_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Title as link'),
    '#default_value' => isset($conf['title_with_link']) ? $conf['title_with_link'] : 0,
    '#description' => "If selected the title would have link which navigated to abstract of the article",
    '#states' => array(
      'invisible' => array(
        ':input[name="display_list"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['show_variant_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show variant link'),
    '#default_value' => isset($conf['show_variant_links']) ? $conf['show_variant_links'] : 0,
    '#description' => "If selected the variant links would be listed with title",
    '#states' => array(
      'invisible' => array(
        ':input[name="display_list"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['no_results_text'] = array(
    '#type' => 'textfield',
    '#title' => t('No results text'),
    '#description' => t('Text to display when no results are found. Leave empty to hide this message.'),
    '#default_value' => isset($conf['no_results_text']) ? $conf['no_results_text'] : 'No citing articles found.',
    '#states' => array(
      'invisible' => array(
        ':input[name="display_list"]' => array('checked' => FALSE),
      ),
    ),
  );

  return $form;
}

/**
 * Store the Cited By configuration.
 */
function jnl_bmj_bmj_cited_by_content_type_edit_form_submit($form, &$form_state) {
  // Exclude unnecessary elements.
  form_state_values_clean($form_state);
  foreach ($form_state['values'] as $key => $value) {
    $form_state['conf'][$key] = $value;
  }
}

/**
 * Callback to provide pane title in panel building display.
 */
function jnl_bmj_bmj_cited_by_content_type_admin_title($subtype, $conf, $context = NULL) {
  return t('Cited By');
}

/**
 * Callback to provide pane info in panel building display.
 */
function jnl_bmj_bmj_cited_by_content_type_admin_info($subtype, $conf, $context = NULL) {
  $pane = new stdClass();
  $pane->title = t('Displays Cited By article.');
  $pane->content = '';
  return $pane;
}


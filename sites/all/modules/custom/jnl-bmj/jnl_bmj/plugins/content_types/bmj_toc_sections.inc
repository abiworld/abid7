<?php

/**
 * @file
 * Highwire TOC Sections
 *
 * Copyright (c) 2010-2011 Board of Trustees, Leland Stanford Jr. University
 * This software is open-source licensed under the GNU Public License Version 2 or later
 * The full license is available in the LICENSE.TXT file at the root of this repository
 */
$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('BMJ Toc Sections'),
  'description' => t('Show Rendered Unique Toc Sections for BMJ'),
  'category' => t('BMJ Issues'),
  'required context' => new ctools_context_required(t('Issue Node'), 'node'),
);

function jnl_bmj_bmj_toc_sections_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
  $flag = FALSE;
  $node = $context->data;

  if ($node->field_highwire_issue_toc_data[X][0]['value']) {
    $toc = unserialize($node->field_highwire_issue_toc_data[X][0]['value']);
    $sections = array();
    foreach ($toc as $row_key => $row_value) {
      if ($row_value['heading']) {

        // this is something will help to genrate drupal_html_id
        $fragment = $row_value['heading'];
        $fragment = strtr(drupal_strtolower($fragment), array(' ' => '-', '_' => '-', '[' => '-', ']' => ''));
        $fragment = preg_replace('/[^A-Za-z0-9\-_]/', '', $fragment);
        $fragment = preg_replace('/\-+/', '-', $fragment);

        $href =  ($flag) ? 'content/current' : 'node/' . $node->nid;
        $sections [] = array(
          'title' => $row_value['heading'],
          'href' => $href,
          'fragment' => $fragment,
        );
      }
    }
    $block->title = '';
    $fragments = array();
    $unique_sections = array();
    foreach ($sections as $section) {
      if (in_array($section['fragment'], $fragments)) {
        continue;
      }
      else {
        $fragments[] = $section['fragment'];
        $unique_sections[] = $section;
      }
    }

    $block->content = theme('links', array('links' => $unique_sections));
  }
  return $block;
}
function jnl_bmj_bmj_toc_sections_content_type_edit_form($form, &$form_state) {
  return $form;
}

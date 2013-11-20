<?php
/**
 * @file
 * jnl_bmj.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function jnl_bmj_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__bmj_additional_article_data';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
        'path' => array(
          'weight' => '1',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_node__bmj_additional_article_data'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'highwire_metatags';
  $strongarm->value = 'enabled_description';
  $export['highwire_metatags'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_bmj_additional_article_data';
  $strongarm->value = array();
  $export['menu_options_bmj_additional_article_data'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_bmj_additional_article_data';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_bmj_additional_article_data'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_bmj_additional_article_data';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_bmj_additional_article_data'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_bmj_additional_article_data';
  $strongarm->value = '0';
  $export['node_preview_bmj_additional_article_data'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_bmj_additional_article_data';
  $strongarm->value = 0;
  $export['node_submitted_bmj_additional_article_data'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_highwire_adjunct_pattern';
  $strongarm->value = '[node:field_highwire_a_cpath]';
  $export['pathauto_node_highwire_adjunct_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_highwire_article_pattern';
  $strongarm->value = '[node:field_highwire_a_cpath]';
  $export['pathauto_node_highwire_article_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_highwire_fragment_pattern';
  $strongarm->value = '[node:field_highwire_a_cpath]';
  $export['pathauto_node_highwire_fragment_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_highwire_issue_pattern';
  $strongarm->value = '[node:field_highwire_a_cpath]';
  $export['pathauto_node_highwire_issue_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_highwire_journal_pattern';
  $strongarm->value = '[node:field_highwire_a_cpath]';
  $export['pathauto_node_highwire_journal_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_node_highwire_volume_pattern';
  $strongarm->value = '[node:field_highwire_a_cpath]';
  $export['pathauto_node_highwire_volume_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_highwire_article_categories_pattern';
  $strongarm->value = 'category/[term:name]';
  $export['pathauto_taxonomy_term_highwire_article_categories_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_highwire_article_keywords_pattern';
  $strongarm->value = 'keyword/[term:name]';
  $export['pathauto_taxonomy_term_highwire_article_keywords_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_highwire_collection_pattern';
  $strongarm->value = 'collection/[term:name]';
  $export['pathauto_taxonomy_term_highwire_collection_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_highwire_import_category_pattern';
  $strongarm->value = 'highwire/import/[term:name]';
  $export['pathauto_taxonomy_term_highwire_import_category_pattern'] = $strongarm;

  return $export;
}

<?php
/**
 * @file
 * jnl_bmj.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function jnl_bmj_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_bmj_override_title|node|bmj_additional_article_data|form';
  $field_group->group_name = 'group_bmj_override_title';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'bmj_additional_article_data';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Override Title',
    'weight' => '2',
    'children' => array(
      0 => 'field_bmj_override_title',
      1 => 'field_bmj_display_title',
      2 => 'field_bmj_us_title',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Override Title',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_bmj_override_title|node|bmj_additional_article_data|form'] = $field_group;

  return $export;
}

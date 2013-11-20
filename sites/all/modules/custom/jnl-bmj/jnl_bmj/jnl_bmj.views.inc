<?php
/**
 * Implements hook_views_data_alter
 */
function jnl_bmj_views_data_alter(&$data) {
  $data['bmj']['table']['group']  = t('Bmj');

  $data['field_data_field_highwire_c_subject']['bmj_rr_parent_filter'] = array(
    'title' => t('bmj_filter_not_null_parents'),
    'group' => 'Bmj',
    'help' => t('Filter comments with parent that are not null'),
    'real field' => 'field_highwire_c_subject_nid',
    'filter' => array(
      'handler' => 'jnl_bmj_handler_filter_not_null_parent',
    ),
  );

  $data['field_data_field_highwire_c_subject']['jnl_bmj_handler_argument_rr_on_parent'] = array(
    'title' => t('Parent Arcticle nid'),
    'group' => 'Bmj',
    'help' => t('Filter comments with the selected parent article '),
    'real field' => 'field_highwire_c_subject_nid',
    'argument' => array(
      'handler' => 'jnl_bmj_handler_argument_rr_on_parent',
    ),
  );
}

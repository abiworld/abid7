<?php
/**
 * @file
 * Highwire PPV Purchase Link
 *
 * Copyright (c) 2010-2011 Board of Trustees, Leland Stanford Jr. University
 * This software is open-source licensed under the GNU Public License Version 2 or later
 * The full license is available in the LICENSE.TXT file at the root of this repository
 */

$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('BMJ PPV Purchase Links'),
  'description' => t('Link to PPV subscription option'),
  'category' => t('BMJ'),
  'required context' => new ctools_context_optional(t('Node'), 'node'),
  'render callback' => 'bmj_purchase_link_content_type_render',  
  'edit form' => 'bmj_purchase_link_content_type_edit_form',  
);

function bmj_purchase_link_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();
	$node = $context->data;
	$nid = '';
	if(empty($context->data)){
		$destination = parse_url($_GET['destination']);
		$cpath_info = array_shift(highwire_get_cpath_index($destination['path']));
		$nid = $cpath_info->entity_id;
	} else {
		$nid = $node->nid;
	}
	
	//$nid = $node->nid;
	if(!empty($nid)){
	  
		$block->content = str_replace('%payment_link' , 'highwire/payment/ppv/'.$nid , $conf['article_link']);
	  
	} 
	
	return $block;
}

function bmj_purchase_link_content_type_edit_form($form, &$form_state) {
  // Nothing
	$conf = $form_state['conf'];
	
	$form['article_link'] = array(
			'#type' => 'textarea',
			'#title' => t('Define pay per article link'),
			'#description' => t('Build your html here, place %payment_link where you would like your link'),
			'#default_value' => isset($conf['article_link']) ? $conf['article_link'] : '<p><a href="'.base_path().'%payment_link">Pay Per Article</a> - You may access this article (from the computer you are currently using) for 1 day for US$20.00</p>',			
	);


	return $form;
}

/**
 * Validate the configuration.
 */
function bmj_purchase_link_content_type_edit_form_validate(&$form, &$form_state) {
  // Nothing
	$form_state['conf']['article_link'] = $form_state['values']['article_link'];
}


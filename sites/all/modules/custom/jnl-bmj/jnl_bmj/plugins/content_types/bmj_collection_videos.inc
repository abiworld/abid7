<?php
/**
 * @file
 * Bmj Podcast Archives
 *
 * Copyright (c) 2010-2011 Board of Trustees, Leland Stanford Jr. University
 * This software is open-source licensed under the GNU Public License Version 2 or later
 * The full license is available in the LICENSE.TXT file at the root of this repository
 */
define('BMJ_VIDEO_LOCATION', "rtmp://fms.1efd.edgecastcdn.net/001EFD/video/");

$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('Bmj Video Collection Archives'),
  'description' => t('Fetches the videos based on the collection term.'),
  'category' => t('BMJ Archive'),
  'required context' => new ctools_context_optional(t('Collection Id'), 'string'),
);

function jnl_bmj_bmj_collection_videos_content_type_render($subtype, $conf, $panel_args, &$context) {
  $block = new stdClass();

  $video_collection_id = $context->data;
  $style_name = $conf['video_image_style'];
  $theme_path = drupal_get_path('theme', 'bmj');

  $block->title = t('Video Abstracts');

  $query = new EntityFieldQuery();
  $results = $query->entityCondition('entity_type', 'node')
              ->entityCondition('bundle', 'bmj_video')
              ->fieldCondition('field_bmj_video_category', 'tid', $video_collection_id)
              ->execute();

  if (count($results)) {
    $nids = array();
    foreach ($results['node'] as $nds) {
      $videos[] = node_load($nds->nid);
    }
    _bmj_video_js_settings();
    foreach ($videos as $video) {
      // $image = theme('image_style', array('style_name' => $image_style, 'path' => file_load($video->field_bmj_video_image[X][0]['fid'])->uri, 'alt' => '', 'title' => '', 'attributes' => array('class' => array('bmj-video-' . $image_style))));
      $image_style_path = image_style_url($image_style, file_load($video->field_bmj_video_image[X][0]['fid'])->uri);

      $video_link .=  '<a class="videoplayer avideoplayer" href="' . BMJ_VIDEO_LOCATION . $node->field_bmj_video_file_url[X][0]['value'] .
                      '"style="display:block;float:left;text-align:center;background-image:url(' . $image_style_path .
                      ')"><img src="/'.$theme_path.'/images/pixel.gif" class="play"  alt="BMJ video"/></a>';

      $video_title = l($node->title, 'node/' . $video->nid, array('attributes' => array('class' => array('video-node-link'))));
      $video_description = $node->body[X][0]['value'];

      $video_markup = theme('bmj_archive_video_abstracts', array('video_link' => $video_link, 'video_title' => $video_title, 'video_desc' => $video_description));
    }
    $block->content = theme('item_list', array('items' => $video_markup, 'title' => 'Video Abstracts'));
  }
  else {
    _bmj_video_js_settings();
    $block->content = '';
  }
  return $block;
}

function jnl_bmj_bmj_collection_videos_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['video_image_style'] = array(
    '#title' => t('Video image size'),
    '#type' => 'select',
    '#description' => t('Please select the image preset of Video Image be display'),
    '#default_value' => !empty($conf['video_image_style']) ? $conf['video_image_style'] : '',
    '#options' => video_image_presets(),
  );

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function jnl_bmj_bmj_collection_videos_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (element_children($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Helper function to get the various image presets available
 */
function video_image_presets() {
 foreach (image_styles() as $key => $val) {
    $image_size[$key] = $key;
  }

  return $image_size;
}

/*
 * Helper function for jQuery BMJ podcast player
 */
function _bmj_video_js_settings($node = NULL) {
  drupal_add_js(drupal_get_path('module', 'jnl_bmj') . '/plugins/content_types/js/flowplayer-3.2.12.min.js');
  drupal_add_js(drupal_get_path('module', 'jnl_bmj') . '/plugins/content_types/js/flowplayer.playlist-3.0.8.min.js');
  drupal_add_js(drupal_get_path('module', 'jnl_bmj') . '/plugins/content_types/js/bmj_video.player.js');
  drupal_add_css(drupal_get_path('module', 'jnl_bmj') .'/plugins/content_types/css/bmj_video.player.css');

  if ($node) {
    $video_image = file_load($node->field_bmj_video_image[X][0]['fid']);
    $vsettings = array($node->field_bmj_video_file_url[X][0]['value'], $video_image->uri, $node->title);
    $video_settings = array(
      'video_location' => BMJ_VIDEO_LOCATION,
      'video_title' => $node->title,
      'video_image' => $video_image->uri,
      'video_filename' => $node->field_bmj_video_file_url[X][0]['value'],
    );
    drupal_add_js(array('bmj_video' => $video_settings), 'setting');
  }
}

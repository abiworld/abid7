<?php

/**
 * @file
 * Up Down Widget.
 */

/**
 * Implementation of hook_vud_widgets(). (Specialized)
 */
function jnl_bmj_bmj_updown_vud_widgets() {
  return array(
    'title' => t('BMJ Updown'),
    'widget template' => 'widget',
  );
}

<?php
/**
 * @file
 * jnl_bmj.panels_default.inc
 */

/**
 * Implements hook_default_panels_mini().
 */
function jnl_bmj_default_panels_mini() {
  $export = array();

  $mini = new stdClass();
  $mini->disabled = FALSE; /* Edit this to true to make a default mini disabled initially */
  $mini->api_version = 1;
  $mini->name = 'jnl_bmj_search_box';
  $mini->category = 'All pages';
  $mini->admin_title = 'Search Box';
  $mini->admin_description = '';
  $mini->requiredcontexts = array();
  $mini->contexts = array();
  $mini->relationships = array();
  $display = new panels_display();
  $display->layout = 'onecol';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'middle' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-1';
    $pane->panel = 'middle';
    $pane->type = 'highwire_seach_quicksearch';
    $pane->subtype = 'highwire_seach_quicksearch';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'buttons' => NULL,
      'context' => 'empty',
      'aligner_start' => NULL,
      'override_title' => 0,
      'override_title_text' => '',
      'aligner_stop' => NULL,
      'override_title_markup' => NULL,
      'submit_button_type' => 'mini',
      'placeholder' => 'Search BMJ Journals',
      'title_search' => '',
      'target' => 'search/site',
      'form_build_id' => 'form-BRCKeiNENBPYwtQk6sDfq-FeFOiOsmJy9Pz-LAc3S4o',
      'form_token' => '49nbGWxbhRRsHfkXkh6dddfkaKGCELpMxi_yXJR0hXs',
      'form_id' => 'highwire_search_quicksearch_form_edit_form',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $display->content['new-1'] = $pane;
    $display->panels['middle'][0] = 'new-1';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $mini->display = $display;
  $export['jnl_bmj_search_box'] = $mini;

  return $export;
}

<?php
// bmj_minipanel_model_link
/**
 * @file
 * Contains the content type plugin for a mini panel. While this does not
 * need to be broken out into a .inc file, it's convenient that we do so
 * that we don't load code unneccessarily. Plus it demonstrates plugins
 * in modules other than Panels itself.
 *
 */

/**
 * Specially named hook. for .inc file. This looks a little silly due to the
 * redundancy, but that's really just because the content type shares a
 * name with the module.
 */
$plugin = array(
  'title' => t('BMJ Mini panels dialog'),
  'content type' => 'jnl_bmj_bmj_minipanel_model_link_content_type_content_type',
);

function _jnl_bmj_bmj_minipanel_model_link_content_type($mini) {
  if (empty($mini)) {
    // The mini panel is deleted or missing.
    return;
  }

  if (!empty($mini->disabled)) {
    return;
  }

  $title = filter_xss_admin($mini->admin_title);
  $type = array(
    'title' => $title,
    // For now mini panels will just use the contrib block icon.
    'icon' => 'icon_mini_panel.png',
    'description' => $title,
    'category' => 'BMJ Minipanel model link',
  );
  if (!empty($mini->requiredcontexts)) {
    $type['required context'] = array();
    foreach ($mini->requiredcontexts as $context) {
      $info = ctools_get_context($context['name']);
      // TODO: allow an optional setting
      $type['required context'][] = new ctools_context_required($context['identifier'], $info['context name']);
    }
  }
  return $type;
}

function jnl_bmj_bmj_minipanel_model_link_content_type_content_type($subtype_id, $plugin) {
  $mini = panels_mini_load($subtype_id);
  return _jnl_bmj_bmj_minipanel_model_link_content_type($mini);
}

/**
 * Return each available mini panel available as a subtype.
 */
function jnl_bmj_bmj_minipanel_model_link_content_type_content_types($plugin) {
  $types = array();
  foreach (panels_mini_load_all() as $mini) {
    $type = _jnl_bmj_bmj_minipanel_model_link_content_type($mini);
    if ($type) {
      $types[$mini->name] = $type;
    }
  }
  return $types;
}


/**
 * Render a mini panel called from a panels display.
 */
function jnl_bmj_bmj_minipanel_model_link_content_type_render($subtype, $conf, $panel_args, &$contexts) {
  $block = new stdClass();
  $mini = panels_mini_load($subtype);
  if (!$mini) {
    return FALSE;
  }
  if (empty($mini) || !empty($mini->disabled)) {
    return;
  }

  // Render the mini panel
  $context = ctools_context_match_required_contexts($mini->requiredcontexts, $contexts);
  $mini->context = $mini->display->context = ctools_context_load_contexts($mini, FALSE, $context);
  $mini->display->args = $panel_args;
  $mini->display->css_id = panels_mini_get_id($subtype);
  $mini->display->owner = $mini;
  $mini->display->owner->id = $mini->name;

  $mini_out = panels_render_display($mini->display);

  // Render the link
  $link = array();
  if ($conf['label']) {
    $link['#title'] = $conf['label'];
    $link['#label_display'] = 'inline';
  } else {
    $link['#label_display'] = 'hidden';
  }
  $link_title = !empty($conf['link_title']) ? $conf['link_title'] : $mini->display->get_title();
  $link_path = '';
  $options = array('html' => TRUE);
  $options['attributes']['data-toggle'] = 'modal';
  $options['attributes']['role'] = 'button';

  $mini_fragment_id = drupal_html_id(panels_mini_get_id($subtype));
  $options['fragment'] = $mini_fragment_id;

  if (!empty($conf['link_icon'])) {
    $options['attributes']['data-font-icon'] = $conf['link_icon'];
    $icon_class = '<i class="bmj-icon '.$conf['link_icon'].'"></i>';
  }

  $link[0]['#markup'] = l($link_title, $link_path, $options);

  $link_out = drupal_render($link);

  $block = new stdClass();
  $block->module  = 'panels_mini';
  $block->delta   = $subtype;
  $block->title   = '';

  $block->content  = "<div class='minipanel-model-wrapper'>";
  $block->content .= "<div class='minipanel-model-link-link'>" . $icon_class . $link_out . "</div>";
  $block->content .= '<!-- Modal -->';
  $block->content .= '<div id="' . $mini_fragment_id .  '" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">';

  if (!empty($conf['label'])) {
    $block->content .= '<div class="modal-header">';
    $block->content .= '<button type="button" class="close" data-dismiss="modal" aria-hidden="true">×</button>';
    $block->content .= '<h4 id="model-label">' . $conf['label']. '</h4>';
    $block->content .= '</div>';
  }

  $block->content .= '<div class="modal-body">'. $mini_out .'</div></div></div>';

  return $block;
}

/**
 * Edit form for the mini panel content type.
 */
function jnl_bmj_bmj_minipanel_model_link_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#description' => 'Label will be placed just before the link text. Leave empty to hide the label.',
    '#default_value' => !empty($conf['label']) ? $conf['label'] : '',
  );

  $form['link_title'] = array(
    '#title' => t('Link Text'),
    '#type' => 'textfield',
    '#description' => 'Please enter the link text. Leave empty to display the mini-panel title',
    '#default_value' => !empty($conf['link_title']) ? $conf['link_title'] : '',
  );

  $form['link_icon'] = array(
    '#type' => 'textfield',
    '#title' => t('Link icon'),
    '#description' => t('Include the entire icon class (i.e.: "icon-angle-right"). A full list of icons can be found <a href="http://fortawesome.github.com/Font-Awesome/#icons-new" target="_blank">here</a>.'),
    '#default_value' => isset($conf['link_icon']) ? $conf['link_icon'] : '',
  );

  return $form;
}

function jnl_bmj_bmj_minipanel_model_link_content_type_edit_form_submit($form, &$form_state) {
  foreach (array('label', 'link_title', 'link_icon') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Provide the administrative title of a mini panel.
 */
function jnl_bmj_bmj_minipanel_model_link_content_type_admin_title($subtype, $conf) {
  $mini = panels_mini_load($subtype);
  if (!$mini) {
    return t('Deleted/missing mini panel @name', array('@name' => $subtype));
  }

  $title = filter_xss_admin($mini->admin_title);
  if (empty($title)) {
    $title = t('Untitled mini panel');
  }
  return 'mini-panel model: ' . $title;
}


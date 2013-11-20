<?php

/**
 * @file
 * Theme functions for the Media Mover UI
 */

/**
 * Display the add configuration action selector.
 *
 * @param array $variables
 *   Array containing: 'element' - Rendered form element array
 * @return string
 */
function theme_media_mover_ui_configuration_action_select($variables) {
  $element = $variables['element'];
  foreach (element_children($element) as $item) {
    if (! element_property($element[$item])) {
      $html[] = '<div style="float: left;">';
      $html[] = drupal_render_children($element[$item]);
      $html[] = '</div>';
    }
  }
  $html[] = '<div style="clear: left;"></div>';
  return implode("\n", $html);
}


/**
 * Display the header for a media mover configuration.
 *
 * @param array $variables
 *   Array containing:
 *   'configuration' - media mover configuration object
 * @return string
 */
function theme_media_mover_ui_configuration_view_header($variables) {
  if ($configuration = $variables['configuration']) {
    // Build the operations that can be done on each step
    $ops = array();
    $ops[] = l(t('View'), MMA_MENU_PATH . '/configuration/' . $configuration->cid . '/view');
    switch ($configuration->status) {
      case MMA_CONFIG_STATUS_ENABLED:
        $ops[] = l(t('Edit'), MMA_MENU_PATH . '/configuration/' . $configuration->cid . '/edit');
        $ops[] = l(t('Disable'), MMA_MENU_PATH . '/configuration/' . $configuration->cid . '/status/' . MMA_CONFIG_STATUS_DISABLED);
        $ops[] = l(t('Run'), MMA_MENU_PATH . '/configuration/' . $configuration->cid . '/run');
        break;
      case MMA_CONFIG_STATUS_OVERRIDDEN:
        $ops[] = l(t('Edit'), MMA_MENU_PATH . '/configuration/' . $configuration->cid . '/edit');
        $ops[] = l(t('Revert'), MMA_MENU_PATH . '/configuration/' . $configuration->cid . '/revert');
        $ops[] = l(t('Disable'), MMA_MENU_PATH . '/configuration/' . $configuration->cid . '/status/' . MMA_CONFIG_STATUS_DISABLED);
        $ops[] = l(t('Run'), MMA_MENU_PATH . '/configuration/' . $configuration->cid . '/run');
        break;
      case MMA_CONFIG_STATUS_DEFAULT:
        $ops[] = l(t('Override'), MMA_MENU_PATH . '/configuration/' . $configuration->cid . '/edit');
        $ops[] = l(t('Disable'), MMA_MENU_PATH . '/configuration/' . $configuration->cid . '/status/' . MMA_CONFIG_STATUS_DISABLED);
        $ops[] = l(t('Run'), MMA_MENU_PATH . '/configuration/' . $configuration->cid . '/run');
        break;
      case MMA_CONFIG_STATUS_DISABLED:
        $ops[] = l(t('Enable'), MMA_MENU_PATH . '/configuration/' . $configuration->cid . '/status/' . MMA_CONFIG_STATUS_ENABLED);
        break;
    }
    $ops[] = l(t('Files'), MMA_MENU_PATH . '/files/configuration/' . $configuration->cid);
    $ops[] = l(t('Delete'), MMA_MENU_PATH . '/configuration/' . $configuration->cid . '/delete');
    return implode(' | ', $ops);
  }
}
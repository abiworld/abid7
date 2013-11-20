<?php
/**
 * @file
 * BMJ Article Permission Link
 *
 */
$plugin = array(
  'single' => TRUE,
  'icon' => 'icon_node_form.png',
  'title' => t('Permission Link'),
  'description' => t('Show get permission link.'),
  'category' => t('BMJ Article'),
  'required context' => new ctools_context_required(t('Article Node'), 'node'),
);

function jnl_bmj_bmj_permission_link_content_type_render($subtype, $conf, $panel_args, &$context) {
  ctools_include('plugins');
  $node = $context->data;
  $publication_date = date('m/d/Y', strtotime($node->field_highwire_a_epubdate[X][0]['value']));
  $apath = $node->field_highwire_a_apath[X][0]['value'];
  foreach ($node->field_highwire_article_authors[X] as $author) {
    $authors[] = $author['value'];
  }
  $author = implode(", ", $authors);
  $doi = $node->field_highwire_article_doi[X][0]['value'];
  $vol_num = $node->field_highwire_a_vol_num[X][0]['value'];
  $copyright = "British Medical Journal Publishing Group";

  $oa_details = _bmj_get_open_access_details($node);
  $open_access = 'n';
  if (!empty($oa_details)) {
    $open_access = $oa_details['open_access'];
  }

  $options = array(
    'html' => TRUE,
    'attributes' => array(
      'rel' => 'external',
      'class' => array('bmj-permission-link', 'request-permissions'),
      'target' => '_blank',
    ),
    'query' => array (
      'publisherName' => 'BMJ',
      'publication' => 'bmj',
      'title'=> $node->title,
      'publicationDate' => $publication_date,
      'author' => $author,
      'contentID'=> $doi,
      'volumeNum' => $vol_num,
      'issueNum' => '',
      'startPage' => '',
      'endPage' => '',
      'copyright'=> $copyright,
      'oa' => $open_access,
      'orderBeanReset' => TRUE,
      'link_type' => 'PERMISSIONDIRECT',
      'atom' => $apath,
    )
  );
  if (!empty($conf['link_icon'])) {
    $options['attributes']['data-font-icon'] = !empty($conf['link_icon']) ? $conf['link_icon'] : '';
    $icon_class = '<i class="bmj-icon '.$conf['link_icon'].'"></i>';
  }

  $block = new stdClass();
  if(isset($conf['permission_link_text']) && (!empty($conf['permission_link_text']))) {
    $link_text = $conf['permission_link_text'];
  }
  else {
    $link_text = "© Get Permissions";
  }
  $block->content = $icon_class . l($link_text, 'https://s100.copyright.com/AppDispatchServlet', $options);
  return $block;
}

/**
 * Ctool configuration form
 */
function jnl_bmj_bmj_permission_link_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['permission_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Permission link Text'),
    '#description' => t('Text of the get permission link'),
    '#default_value' => $conf['permission_link_text'] ? $conf['permission_link_text'] : "",
  );

  $form['link_icon'] = array(
    '#type' => 'textfield',
    '#title' => t('Permission link icon'),
    '#description' => t('Include the entire icon class (i.e.: "icon-angle-right"). A full list of icons can be found <a href="http://fortawesome.github.com/Font-Awesome/#icons-new" target="_blank">here</a>.'),
    '#default_value' => isset($conf['link_icon']) ? $conf['link_icon'] : 'icon-copyright',
  );

  return $form;
}

/**
 * The submit form handler stores the data in $conf.
 */
function jnl_bmj_bmj_permission_link_content_type_edit_form_submit(&$form, &$form_state) {
  // Exclude unnecessary elements.
  form_state_values_clean($form_state);
  foreach ($form_state['values'] as $key => $value) {
    $form_state['conf'][$key] = $value;
  }
}
/**
 * Callback to provide pane title in panel building display.
 */
function jnl_bmj_bmj_permission_link_content_type_admin_title($subtype, $conf, $context = NULL) {
  return t('Permission Link');
}

/**
 * Callback to provide pane info in panel building display.
 */
function jnl_bmj_bmj_permission_link_content_type_admin_info($subtype, $conf, $context = NULL) {
  $pane = new stdClass();
  $pane->title = t('Displays Permission Link for article.');
  $pane->content = '';
  return $pane;
}


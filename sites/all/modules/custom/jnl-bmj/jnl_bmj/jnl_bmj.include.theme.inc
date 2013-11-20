<?php
/**
 * @file
 * BMJ themes definations
 */


/**
 * Theme implementation of bmj_bootstrap_buttons
 */
function theme_bmj_bootstrap_button($variables) {
  return '<button class="btn">' . $variables['button_content'] . '</button>';
}


/* @file
 * Highwire Comment ariticle citation - basic theme function(s).
 */

function theme_bmj_comments_list(&$variables) {
  $node = $variables['node'];
  $conf = $variables['conf'];
  if ($conf['link_title']) {
    $title = l($node->title, 'node/' . $node->nid, array('attributes' => array('class' => array('article-response-title-link'))));
  }
  else {
    $title = '<div class="article-response-title">' . $node->title . '</div>';
  }

  $article_response_body_summary = '<div class="article-response-body-summary">' . text_summary(strip_tags($node->field_highwire_c_response_body[X][0]['value']), NULL, 200) . '</div>';

   $blue_arrow_right = '<i class="arrow-right-sm-blue"></i>';

//  $more_link = l(t('more ').$blue_arrow_right, '', array('attributes' => array('class' => array('article-response-more-link', 'btn', 'btn-primary', 'btn-lg'), 'data-toggle' => 'modal', 'data-target' => '#rapid-response-related')), 'fragment' => 'rapid-response-article', 'html' => TRUE));
  $more_link = '<button type="button" class="btn more-btn pull-right" data-toggle="modal" data-target="#rapid-response-related">more <i class="arrow-right-sm-blue"></i></button>';

  $output = $title . $article_response_body_summary . $more_link;

  return $output;
}

/**
 * Returns HTML for a list or nested list of items with Show More/Show Less links.
 *
 * @param $variables
 *   An associative array containing:
 *   - items: An array of items to be displayed in the list. If an item is a
 *     string, then it is used as is. If an item is an array, then the "data"
 *     element of the array is used as the contents of the list item. If an item
 *     is an array with a "children" element, those children are displayed in a
 *     nested list. All other elements are treated as attributes of the list
 *     item element.
 *   - title: The title of the list.
 *   - type: The type of list to return (e.g. "ul", "ol").
 *   - attributes: The attributes applied to the list element.
 *   - limit: number of items to show, n reset will be hidden
 *   - more-link: An associative array containing
 *               --title: Link title for show more defaulted to "Show More"
 *               --icon: link icon
 *   - less-link: An associative array containing
 *               --title: Link title for show more defaulted to "Show More"
 *               --icon: link icon
 */
function theme_bmj_item_list_more_less($variables) {

  //Inlcude css/js for exapansion list
  drupal_add_css(drupal_get_path('module', 'jnl_bmj') . '/css/jnl_bmj_item_list_show_less_more.css');
  drupal_add_js(drupal_get_path('module', 'jnl_bmj') . '/js/jnl_bmj_item_list_show_less_more.js');

  $items = $variables['items'];
  $title = $variables['title'];
  $type = $variables['type'];
  $attributes = $variables['attributes'];
  $limit = $variables['limit'];
  $more_link = $variables['more-link'];
  $less_link = $variables['less-link'];

  // set the context wrapper for nested link
  $context_class = drupal_html_id('bmj-item-list');

  // Only output the list container and title, if there are any list items.
  // Check to see whether the block title exists before adding a header.
  // Empty headers are not semantic and present accessibility challenges.
  $output = '<div class="bmj-item-more-less-list ' . $context_class . '">';
  if (isset($title) && $title !== '') {
    $output .= '<h3>' . $title . '</h3>';
  }

  if (!empty($items)) {
    $output .= "<$type" . drupal_attributes($attributes) . '>';
    $num_items = count($items);
    $i = 0;
    $show_hide_flag = FALSE;
    $test = FALSE;
    foreach ($items as $item) {
      $attributes = array();
      $children = array();
      $data = '';
      $i++;
      if (is_array($item)) {
        foreach ($item as $key => $value) {
          if ($key == 'data') {
            $data = $value;
          }
          elseif ($key == 'children') {
            $children = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $data = $item;
      }
      if (count($children) > 0) {
        // Render nested list.
        // @TODO Need to add nested more-less links
        $data .= theme_item_list_more_less(array('items' => $children, 'title' => NULL, 'type' => $type, 'more-link' => $more_link, 'less-link' => $less_link, 'attributes' => $attributes));
      }
      // compute the class to show/hide
      if ($limit > 0 && $limit < $i) {
        $attributes['class'][] = 'bmj-list-item-hide';
        $attributes['class'][] = 'bmj-hide-list-item';
        $show_hide_flag = TRUE;
      }

      if ($i == 1) {
        $attributes['class'][] = 'first';
      }
      if ($i == $num_items) {
        $attributes['class'][] = 'last';
      }
      $output .= '<li' . drupal_attributes($attributes) . '>' . $data . "</li>\n";
    }
    $output .= "</$type>";
  }

  if ($show_hide_flag) {
    $show_hide_links['show-more'] = array(
      'title' => !empty($more_link['title']) ? $more_link['title'] : t('Show More'),
      'href' => '',
      'external' => TRUE,
      'fragment' => ' ',
      'attributes' => array(
        'class' => array('bmj-more-less-link', 'more-link', 'btn'),
        'data-font-icon' => !empty($more_link['icon']) ? $more_link['icon'] : '',
        'data-context' => $context_class,
      ),
    );

    $show_hide_links['show-less'] = array(
      'title' => !empty($less_link['title']) ? $less_link['title'] : t('Show Less'),
      'href' => '',
      'external' => TRUE,
      'fragment' => ' ',
      'attributes' => array(
        'class' => array('bmj-more-less-link', 'less-link', 'btn'),
        'data-font-icon' => !empty($less_link['icon']) ? $less_link['icon'] : '',
        'data-context' => $context_class,
      ),
    );
    $vars = array();
    $vars['links'] = $show_hide_links;
    $vars['attributes'] = array('class' => array('links', 'inline'));
    $output .= '<div class = "bmj-item-list-show-more-less">';
    $output .= theme('links', $vars);
    $output .= '</div>';
  }
  $output .= '</div>';
  return $output;
}


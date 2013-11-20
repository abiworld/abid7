<?php

/**
 * @file
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->wrapper_prefix: A complete wrapper containing the inline_html to use.
 *   - $field->wrapper_suffix: The closing tag for the wrapper.
 *   - $field->separator: an optional separator that may appear before a field.
 *   - $field->label: The wrap label text to use.
 *   - $field->label_html: The full HTML of the label to use including
 *     configured element type.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */
?>
<?php
  if (isset($fields['nid'])) {
    $response_node = node_load($fields['nid']->raw);

    $parent = node_load($response_node->field_highwire_c_subject[X][0]['nid']);
    $show_parent_info = FALSE;
    if ($parent) {
      $parent_authors = array();
      $i = 0;

      if (isset($parent->field_highwire_article_authors)) {
        foreach ($parent->field_highwire_article_authors[X] as $key => $author) {
          if ($i > 4) {
            $parent_authors[] = 'et al';
            break;
          }
          $parent_authors[] = $author['value'];
          $i++;
        }
      }
      $parent_authors     = (count($parent_authors)) ? check_plain(implode(', ', $parent_authors).'.') : '';
      $parent_volume      = check_plain($parent->field_highwire_a_vol_num[X][0]['value']);
      $parent_doi         = isset($parent->field_highwire_a_doi) ? check_plain($parent->field_highwire_a_doi[X][0]['value']) : '';
      $parent_unpub_class = '';
      $parent_unpub_text = '';
      if ($parent->status == 0) {
        $parent_unpub_class = ' node-unpublished ';
        $parent_unpub_text  = ' (Unpublished) ';
      }
      $parent_link = l($parent->title, 'node/' . $parent->nid);

      $date = $response_node->field_highwire_a_epubdate[X][0]['value'];
      $formated_date = date('j F Y', strtotime($date));
      if (isset($response_node->field_highwire_c_response_body[X])) {
        $text = check_markup($response_node->field_highwire_c_response_body[X][0]['value'], $response_node->field_highwire_c_response_body[X][0]['format'], FALSE);
      }
      $out = array();

      foreach ($response_node->field_highwire_a_contributors[X] as $key => $author) {
        $out[] = '<strong>' . $author['name_given'] . ' ' . $author['name_sur'] . '</strong>' . $author['aff'];
      }

      $authors = implode(', ', $out);
      $authors .= $response_node->field_bmj_rr_other_authors[X][0]['value'];

      $state = strtolower(preg_replace('/[^a-zA-Z0-9]/', '', $response_node->field_highwire_a_state[X][0]['value']));
      $title = l($response_node->title, 'node/'.$response_node->nid, array('html' => TRUE));
?>

<div class="node node-rapid-response clear-block state-<?php echo $state;?>">
  <div class="clear-block">
    <div class="left-block">
      <div class="grid_6 first">
        <h2><?php print $title?></h2>
      </div>
      <div class="grid_2 last">
        <h4><?php print $formated_date ?></h4>
      </div>
      <div class="author">
        <p><?php print $authors; ?></p>
      </div>
    </div>
    <div class="right-block">
      <div class="vote-widget">
        <p><strong>Click to like:</strong></p>
        <?php
          $node_view = node_view($response_node);
          print $node_view['vud_node_widget_display']['#value']; ?>
      </div>
    </div>
  </div>

  <div class="responses-title clear-block">

    <div class="grid_6 first">

    <?php if ($show_parent_info): ?>
      <p class="responses-article-title">Re: <span class="<?php print $parent_unpub_class; ?>article-title"><?php print $parent_unpub_text; ?><?php print $parent_link;?></span>.
      <span class="author-names"><?php print $parent_authors; ?></span>
      <span class="doi"><?php print $parent_volume; ?>:doi:</span><span class="doi-details"><?php print $parent_doi; ?></span></p>
    <?php endif; ?>


    <?php
      if (isset($text)) {
        print $text;
      }
    ?>
    </div>

    <div class="grid_2 last">
      <?php if ($show_parent_info) { ?>
      <div class="response-links">
        <ul>
          <li><?php print l('Respond to this article', 'node/add/highwire-comment'); ?></li>
          <li class="last"><?php print l('Read all responses for this article', 'node/'.$parent->nid, array('query' => array('tab' => 'responses'))); ?></li>
        </ul>
      </div>
    <?php } ?>
    </div>
  </div>
</div>
<?php }}?>

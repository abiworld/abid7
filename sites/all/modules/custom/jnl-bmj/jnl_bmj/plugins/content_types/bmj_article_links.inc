<?php
/**
 * @file
 * BMJ Article links
 *
 */
$plugin = array(
  'title'            => t('Article external links'),
  'description'      => t('Article external links'),
  'single'           => TRUE,
  'category'         => t('BMJ Article'),
  'required context' => new ctools_context_required(t('Article node'), 'node'),
);

/**
 * Callback to provide pane title in panel building display.
 */
function jnl_bmj_bmj_article_links_content_type_admin_title($subtype, $conf, $context = NULL) {
  return t('Article links');
}

/**
 * Callback to provide pane info in panel building display.
 */
function jnl_bmj_bmj_article_links_content_type_admin_info($subtype, $conf, $context = NULL) {
  $pane = new stdClass();
  $pane->title = t('Additionally display these external links for citing articles.');
  $links = array(
    'pubmed' => 'Find similar articles in PubMed',
    'author' => 'Articles by <author names>',
    'isi' => 'Citing articles via Web of Science (timescited)',
    'scopus' => 'Citing articles via Scopus (timescited)',
    'google' => 'Google Scholar',
    'portfolio' => 'Add article to BMJ portfolio',
  );

  $list_item = array();
  foreach ($conf['external_links'] as $key => $value) {
    if ($value) {
      $list_item[] = $links[$key];
    }
  }
  $pane->content = theme('item_list', array('title' => '', 'items' => $list_item, 'attributes' => array('class' => 'links-list')));
  return $pane;
}

/**
 * Output for Article links
 */
function jnl_bmj_bmj_article_links_content_type_render($subtype, $conf, $panel_args, &$context) {
  $links = array();

  if ($conf['external_links']['pubmed'] && @$context->data->field_highwire_article_pmid[X][0]['value']) {
    $links['related-pubmed'] = array(
      'title' => t('Find similar articles in PubMed'),
      'href' => 'lookup/external-ref',
      'query' => array(
        'link_type' => 'MED_NBRS',
        'access_num' => $context->data->field_highwire_article_pmid[X][0]['value'],
      ),
      'attributes' => array('target' => '_blank'),
    );
  }
  if ($conf['external_links']['author'] && @$context->data->field_highwire_article_authors[X][0]['value']) {
    foreach ($context->data->field_highwire_article_authors[X] as $id => $author) {
      // To do PubMed search Surname must be first
      $author_name = explode(" ", $author['value']);
      $last_name = array_pop($author_name);
      $author_name = implode(" ", $author_name);
      $links[] = array(
        'title' => t('Articles by @name', array('@name' => $author['value'])),
        'href' => 'http://www.ncbi.nlm.nih.gov/sites/entrez',
        'external' => TRUE,
        'query' => array(
          'cmd' => 'search',
          'db' => 'pubmed',
          'term' => $last_name . ' ' . $author_name . '[au]',
          'dispmax'  => '50',
        ),
        'attributes' => array('target' => '_blank'),
      );
    }
  }

  if ($conf['external_links']['isi']) {
    $links_service = new HWLinks('isi');
    $isidata = $links_service->execute(highwire_strip_atom_path($context->data->field_highwire_a_apath[X][0]['value']));
    $type = $conf['scopus_type'];
    if (@$isidata['links']) {
      foreach ($isidata['links'] as $link) {
        if ($link['type'] == $type) {
          $isi_url = $link['url'];
          break;
        }
      }
      $times_cited = !empty($isidata['timescited']) ? '(' . $isidata['timescited'] . ')' : '';
      if (!empty($isi_url)) {
        $links['related-isi'] = array(
          'title' => t('Citing articles via Web of Science !timescited', array('!timescited' => $times_cited)),
          'href' => 'lookup/external-ref',
          'query' => array(
            'access_num' => $context->data->field_highwire_a_pisa_id[X][0]['value'],
            'link_type' => 'ISI_CITING',
            'external_url' => $isi_url
          ),
          'attributes' => array('target' => '_blank'),
        );
      }
    }
  }

  if ($conf['external_links']['scopus']) {
    $links_service = new HWLinks('scopus');
    $scopusdata = $links_service->execute(highwire_strip_atom_path($context->data->field_highwire_a_apath[X][0]['value']));
    $type = $conf['scopus_type'];
    $times_cited = !empty($scopusdata['timescited']) ? '(' . $scopusdata['timescited'] . ')' : '';
    if (@$scopusdata['links']) {
      foreach ($scopusdata['links'] as $link) {
        if ($link['type'] == $type) {
          $scopus_url = $link['url'];
          break;
        }
      }
      if (!empty($scopus_url)) {
        $links['related-scopus'] = array(
          'title' => t('Citing articles via Scopus !timescited', array('!timescited' => $times_cited)),
          'href' => 'lookup/external-ref',
          'query' => array('access_num' => $context->data->field_highwire_a_pisa_id[X][0]['value'], 'link_type' => 'ISI_CITING', 'external_url' => $scopus_url),
          'attributes' => array('target' => '_blank'),
        );
      }
    }
  }

  if ($conf['external_links']['google']) {
    $links['related-google-scholar'] = array(
      'title' => t('Google Scholar'),
      'href' => 'lookup/external-ref',
      'query' => array(
        'link_type' => 'GOOGLESCHOLARRELATED',
        'access_num' => highwire_article_external_url($context->data),
      ),
      'attributes' => array('target' => '_blank'),
    );
  }

  if ($conf['external_links']['portfolio']) {
    $links['portfolio'] = array(
      'title' => t('Add article to BMJ portfolio'),
      'href' => 'http://portfolio.bmj.com/portfolio/add-to-portfolio.html',
      'query' => array(
        't' => drupal_get_title(),
        'u' => highwire_article_external_url($context->data)
      ),
      'attributes' => array('target' => '_blank', 'id' => 'add-to-bmj-portfolio', 'rel' => 'nofollow',  'target' => '_blank'),
    );
  }

  $article_links = '';
  if (count($links)) {
    $article_links_classes = array('links');
    if ($display_list) {
      $article_links_classes[] = 'inline';
    }

    $article_links = '<div class="bmj-article-links">';
    $article_links .= theme('links', array('links' => $links, 'attributes' => array('class' => $article_links_classes)));
    $article_links .= '</div>';
  }
  $block = new stdClass();
  $title = (isset($conf['title']) && (!empty($conf['title']))) ? $conf['title'] : '';
  $block->title = $title;
  $block->content = $article_links;

  return $block;
}

/**
 * The form to add or edit a article links as content.
 */
function jnl_bmj_bmj_article_links_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['external_links'] = array(
    '#type' => 'checkboxes',
    '#title' => t('External Links'),
    '#descripton' => t('Additionally display these external links for citing articles'),
    '#default_value' => isset($conf['external_links']) ? $conf['external_links'] : array('isi', 'pubmed', 'scopus', 'author'),
    '#options' => array(
      'pubmed' => 'PubMed',
      'author' => 'Articles by <author>',
      'isi' => 'Web of Science',
      'scopus' => 'Scopus',
      'google' => 'Google Scholar',
      'portfolio' => 'Add to BMJ portfolio',
    ),
  );

  $form['isi_type_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select type for Web of Science'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="external_links[isi]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['isi_type_fieldset']['isi_type'] = array(
    '#type' => 'radios',
    '#title' => t('Select type for Web of Science'),
    '#default_value' => isset($conf['isi_type']) ? $conf['isi_type'] : '',
    '#options' => array(
      'citing' => 'Citing',
      'related' => 'Related',
    ),
  );

  $form['scopus_type_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select type for Scopus'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="external_links[scopus]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['scopus_type_fieldset']['scopus_type'] = array(
    '#type' => 'radios',
    '#default_value' => isset($conf['scopus_type']) ? $conf['scopus_type'] : '',
    '#options' => array(
      'citing' => 'Citing',
      'related' => 'Related',
    ),
  );

  return $form;
}

/**
 * Store the article links configuration.
 */
function jnl_bmj_bmj_article_links_content_type_edit_form_submit($form, &$form_state) {
  // Exclude unnecessary elements.
  form_state_values_clean($form_state);
  foreach ($form_state['values'] as $key => $value) {
    $form_state['conf'][$key] = $value;
  }
}


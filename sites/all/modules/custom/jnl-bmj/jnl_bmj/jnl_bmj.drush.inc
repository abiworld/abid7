<?php

define(JNL_BMJ_D6_ROOT, '/opt/sites/jnl-bmj-dev/drupal-webroot');

/**
 * Implementation of hook_drush_command().
 */
function jnl_bmj_drush_command() {
  $items = array();

  $items['bmj-load-article-data'] = array(
    //'callback' => 'highwire_atomload_callback',
    'description' => "Loads old county data into drupal.",
    'drupal dependencies' => array('highwire'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  $items['bmj-load-book-pages'] = array(
    //'callback' => 'highwire_atomload_callback',
    'description' => "Load book pages into drupal",
    'drupal dependencies' => array('highwire'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  $items['bmj-load-eletters'] = array(
    //'callback' => 'highwire_atomload_callback',
    'description' => "Load eletters into drupal",
    'drupal dependencies' => array('highwire'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'options' => array(
       'nids' => array(
         'description' => 'comma seperated list of d6 nids to load',
       ),
     ),
  );

    $items['bmj-load-press-release'] = array(
    //'callback' => 'highwire_atomload_callback',
    'description' => "Load press releases into drupal",
    'drupal dependencies' => array('highwire'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  return $items;
}

function drush_jnl_bmj_bmj_load_article_data(){

  highwire_system_message("Loading additional article data from bmj d6");

  $country_terms_names = array('india', 'us', 'uk');
  $vocab = 'bmj_countries';

  $country_terms = array();
  foreach ($country_terms_names as $name) {
    $term = array_shift(taxonomy_get_term_by_name($name, $vocab));
    $country_terms[strtolower($term->name)] = $term;
  }

  //Going to build a data array for loading class
  $data = array();

  db_set_active('bmj_d6');


  highwire_system_message('Getting country data');

  $results = db_query("SELECT * FROM bmj_countries_node");

  foreach ($results as $row) {
    $apath = db_query("SELECT * FROM content_field_highwire_a_apath WHERE nid = " .$row->nid)->fetchObject();

    //Looks like bmj made an entry in the table for every node in drupal, we only care about ones with apaths
    if(empty($apath->field_highwire_a_apath_value)) {
      continue;
    }

    foreach ($country_terms_names as $name) {
      if($row->{$name}) {

        $data[$apath->field_highwire_a_apath_value]['countries'][] = $country_terms[$name]->tid;
        $data[$apath->field_highwire_a_apath_value]['apath'] = $apath->field_highwire_a_apath_value;
        $data[$apath->field_highwire_a_apath_value]['resource_id'] = $apath->field_highwire_a_apath_value . '_additional_info';

      }
    }

   if(!empty($row->local)) {
      $data[$apath->field_highwire_a_apath_value]['local'] = 1;
      $data[$apath->field_highwire_a_apath_value]['apath'] = $apath->field_highwire_a_apath_value;
      $data[$apath->field_highwire_a_apath_value]['resource_id'] = $apath->field_highwire_a_apath_value . '_additional_info';
   }

    if($data[$apath->field_highwire_a_apath_value]['countries']) {
      $data[$apath->field_highwire_a_apath_value]['countries'] = array_unique($data[$apath->field_highwire_a_apath_value]['countries']);
    }

  }


  highwire_system_message("Found " . count($data) . " affected country nodes");





  //Next we need to go to the d6 db and get any override
  highwire_system_message("Getting additional article links");
  //Get the additional bmj article links
  $results = db_query("SELECT * FROM content_field_bmj_article_links, content_field_highwire_a_apath, node
      WHERE node.nid = content_field_bmj_article_links.nid AND
            node.nid = content_field_highwire_a_apath.nid AND
            (content_field_bmj_article_links.field_bmj_article_links_url IS NOT NULL AND content_field_bmj_article_links.field_bmj_article_links_title IS NOT NULL)");

  foreach ($results as $row) {
    if(empty($row->field_highwire_a_apath_value)) {
      continue;
    }

    $attributes = unserialize_rec($row->field_bmj_article_links_attributes);

    if(!is_array($attributes)) {
      $attributes = array();
    }

    $data[$row->field_highwire_a_apath_value]['article_links'][] = array(
          'url' => $row->field_bmj_article_links_url,
          'title' => $row->field_bmj_article_links_title,
          'short_title' => $attributes['short_title'],
        );

    $data[$row->field_highwire_a_apath_value]['apath'] = $row->field_highwire_a_apath_value;
    $data[$row->field_highwire_a_apath_value]['resource_id'] = $row->field_highwire_a_apath_value . '_additional_info';

  }


 //Next we need to go to the d6 db and get any override
  highwire_system_message("Getting additional bmjgroup links");
  //Get the additional bmj article links
  $results = db_query("SELECT * FROM content_field_bmj_bmjgroup_article_links, content_field_highwire_a_apath, node
      WHERE node.nid = content_field_bmj_bmjgroup_article_links.nid AND
            node.nid = content_field_highwire_a_apath.nid AND
            (content_field_bmj_bmjgroup_article_links.field_bmj_bmjgroup_article_links_url IS NOT NULL AND content_field_bmj_bmjgroup_article_links.field_bmj_bmjgroup_article_links_title IS NOT NULL)");

  foreach ($results as $row) {
    if(empty($row->field_highwire_a_apath_value)) {
      continue;
    }

    $attributes = unserialize_rec($row->field_bmj_bmjgroup_article_links_attributes);

    if(!is_array($attributes)) {
      $attributes = array();
    }

    $data[$row->field_highwire_a_apath_value]['bmjgroup_links'][] = array(
          'url' => $row->field_bmj_bmjgroup_article_links_url,
          'title' => $row->field_bmj_bmjgroup_article_links_title,
          'short_title' => $attributes['short_title'],
        );

    $data[$row->field_highwire_a_apath_value]['apath'] = $row->field_highwire_a_apath_value;
    $data[$row->field_highwire_a_apath_value]['resource_id'] = $row->field_highwire_a_apath_value . '_additional_info';

  }




  highwire_system_message("Getting additional data like override title, us title etc...");

  $results = db_query("SELECT * FROM {content_type_highwire_article}, {content_field_highwire_a_apath}, {node}, {content_field_bmj_article_override_title}
                      WHERE node.nid = content_type_highwire_article.nid AND
                            node.nid = content_field_highwire_a_apath.nid  AND
                            node.nid = content_field_bmj_article_override_title.nid AND
                            (content_type_highwire_article.field_bmj_article_intro_value IS NOT NULL OR content_type_highwire_article.field_bmj_article_us_title_value)");

  foreach ($results as $row) {

    if(empty($row->field_highwire_a_apath_value)) {
      continue;
    }
    $data[$row->field_highwire_a_apath_value]['intro'] = $row->field_bmj_article_intro_value;
    $data[$row->field_highwire_a_apath_value]['override_title'] = $row->field_bmj_article_override_title_value == 'Yes' ? 1 : 0;
    $data[$row->field_highwire_a_apath_value]['us_title'] = $row->field_bmj_article_us_title_value;
    $data[$row->field_highwire_a_apath_value]['display_title'] = $row->field_bmj_snippet_display_title_value;
    $data[$row->field_highwire_a_apath_value]['apath'] = $row->field_highwire_a_apath_value;
    $data[$row->field_highwire_a_apath_value]['resource_id'] = $row->field_highwire_a_apath_value . '_additional_info';
  }


  //Need to line up press releases
  $results = db_query("SELECT * FROM {content_type_highwire_article}, {content_field_highwire_a_apath}, {node}, {content_field_bmj_pressrelease_ref}
                      WHERE node.nid = content_type_highwire_article.nid AND
                            node.nid = content_field_highwire_a_apath.nid  AND
                            node.nid = content_field_bmj_pressrelease_ref.nid AND
                            (content_field_bmj_pressrelease_ref.field_bmj_pressrelease_ref_nid IS NOT NULL)");

  foreach ($results as $row) {

    if(empty($row->field_highwire_a_apath_value)) {
      continue;
    }

    db_set_active();

    $resource = highwire_resource_from_resource_id("d6_press_release_" . $row->field_bmj_pressrelease_ref_nid);

    db_set_active('bmj_d6');

    if($resource) {
    	$data[$row->field_highwire_a_apath_value]['apath'] = $row->field_highwire_a_apath_value;
    	$data[$row->field_highwire_a_apath_value]['resource_id'] = $row->field_highwire_a_apath_value . '_additional_info';
      $data[$row->field_highwire_a_apath_value]['press_release'] = (int)$resource['id'];
    }

  }

  db_set_active();


  //Now lets load this data
  highwire_system_message("Loading " . count($data) . " addtional data items");

  $total = count($data);
  $count = 0;
  $time = time();

  foreach ($data as $key => $d) {
    if($count % 50 == 0) {
      $percent = (int)(($count/$total) * 100);
      $total_time = time() - $time;
      highwire_system_message("Loaded $count of $total - $percent% - Time Lapsed: $total_time seconds");
    }

    $loader = new D6DataLoader();
    $loader->setHashReset();
    $item = $loader->load($d);
    $count++;
  }

}


function drush_jnl_bmj_bmj_load_book_pages() {
  $php_code = ' $results = db_query("SELECT * FROM {node} where type=\"book\"");
  $nodes = array();
  while ($row = db_fetch_object($results)) {
    $nodes[] = node_load($row->nid);
  }

  echo json_encode($nodes);';

  $output = '';
  exec("drush -r ".JNL_BMJ_D6_ROOT." php-eval '$php_code'", $output);

  $nodes = json_decode($output[0]);

  $data = array();
  $aditional_data = array();


  //Need to make sure top level books are processed first
  foreach ($nodes as $key => $node) {
    if($node->nid == $node->book->bid) {
      $array = $node;
      unset($nodes[$key]);
      array_unshift($nodes, $array);
    }
  }

  foreach ($nodes as $node) {
    $parents = array();

    for($i=1; $i < 10; $i++) {
      $parents[] = $node->book->{'p'.$i};
    }



    $data[$node->nid] = array(
      'title' => $node->title,
      'body' => $node->body,
      'teaser' => $node->teaser,
      'parents' => array_filter($parents),
      'weight' => $node->book->weight,
      'files' => !empty($node->field_bmj_attachments) ? $node->field_bmj_attachments : FALSE,
      'resource_id' => 'db_book_'.$node->nid,
      'book_id' =>  'db_book_'.$node->book->bid,
      'top_level' => $node->book->bid == $node->nid ? true : false,
      'menu_name' => $node->book->menu_name,
      'path' => $node->path,
      'book_nid' => $node->book->bid,
      'menu_id' => $node->book->mlid,
      'weight' => $node->book->weight,
      'depth' => $node->book->depth,
      'has_children' => $node->book->has_children,
      'plid' => $node->book->plid,
      'old_nid' => $node->nid,
      'hidden' => $node->book->hidden,

    );

    //print_r($data);

  }

  $items = array();
  foreach ($data as $d) {
    $bookLoader = new LoadBookPages();
    $bookLoader->setHashReset();
    $items[] = $bookLoader->load($d, NULL, TRUE, array('hidden' => $d['hidden'], 'data' => $d, 'book_id' => $d['book_id'] ,'parent' => $d['parents'], 'legacy_mlid' => $d['menu_id'], 'plid' => $d['plid']));
  }

  //Figure out bid
  foreach ($items as $item_key => $item) {
    foreach ($items as $second_key => $second_item) {
      if($item['extra_data']['data']['book_nid'] == $second_item['extra_data']['data']['old_nid']) {
        $items[$item_key]['bid'] = $second_item['nid'];
      }
    }
  }

  //figure out plid
  foreach ($items as $item_key => $item) {
    foreach ($items as $second_key => $second_item) {
      if($item['extra_data']['plid'] == $second_item['extra_data']['legacy_mlid']) {
        $node = node_load($second_item['nid'], NULL, TRUE);
        $items[$item_key]['plid'] = $node->book['mlid'];
      }
    }
  }

  $bookLoader = new LoadBookPages();
  foreach ($items as $item) {
    $errors = array();

    $node = $bookLoader->postprocess($item, FALSE, $errors);
    if(!empty($node)) {
      node_save($node);
    }

  }

}


function drush_jnl_bmj_bmj_load_eletters() {
  if($nids = drush_get_option('nids')) {
    $nids = explode(",", $nids);
    foreach ($nids as $key => $nid) {
      $nids[$key] = trim($nid);
    }
    $total_rrs = count($nids);
  } else {

    //First get total count, we have to looop other wise we run out of memory
    $php_code = ' $results = db_query("SELECT count(*) as total FROM {node} where type=\"bmj_rapid_response\"");
    $nodes = array();
    while ($row = db_fetch_object($results)) {
      $total = $row->total;
    }

    echo json_encode(array("total" => $total));';


    $output = '';
    exec("drush -r ".JNL_BMJ_D6_ROOT." php-eval '$php_code'", $output);

    $total_rrs = json_decode($output[0]);
    $total_rrs = $total_rrs->total;

  }

  $limit_start = 0;
  $total_select = 1000;

  highwire_system_message("Loading " . $total_rrs . " eletters.");

  //Lets do a 100 at time
  while($total_rrs >= 0) {

    if($nids) {
      $php_code = ' $results = db_query("SELECT * FROM {node} where type=\"bmj_rapid_response\" and nid in ('.implode(',', $nids).') order by nid desc LIMIT '.$limit_start.', '.$total_select.' ");
      $nodes = array();

      while ($row = db_fetch_object($results)) {
          $nodes[] = node_load($row->nid);
      }
      echo json_encode($nodes);';
    } else {
      $php_code = ' $results = db_query("SELECT * FROM {node} where type=\"bmj_rapid_response\" order by nid desc LIMIT '.$limit_start.', '.$total_select.' ");
      $nodes = array();
      while ($row = db_fetch_object($results)) {
        $nodes[] = node_load($row->nid);
      }

      echo json_encode($nodes);';
    }

    $output = '';
    exec("drush -r ".JNL_BMJ_D6_ROOT." php-eval '$php_code'", $output);

    $nodes = json_decode($output[0]);
    highwire_system_message("Loading " . count($nodes));
    if(count($nodes) <= 0) {
      highwire_system_message("Didn't find any nodes, breaking the loop");
      break;
    }

    foreach ($nodes as $node) {
      $node = (array)$node;
      $node['resource_id'] = 'd6_eletters_' .$node['nid'];
      $eletterLoader = new LoadEletters();
      $eletterLoader->setHashReset();
      $eletterLoader->load($node);
    }

    $total_rrs = $total_rrs - $total_select;
    $limit_start = $limit_start + $total_select;
    unset($nodes);
    highwire_system_message("Loaded " . $limit_start . " out of " . ($total_rrs + $limit_start) . " " . (int)($limit_start / ($total_rrs + $limit_start) * 100) . "% complete");
  }
}

function drush_jnl_bmj_bmj_load_press_release() {
  $php_code = ' $results = db_query("SELECT * FROM {node} where type=\"press_release\"");
  $nodes = array();
  while ($row = db_fetch_object($results)) {
    $nodes[] = node_load($row->nid);
  }

  echo json_encode($nodes);';

  $output = '';
  exec("drush -r ".JNL_BMJ_D6_ROOT." php-eval '$php_code'", $output);

  $nodes = json_decode($output[0]);

  foreach ($nodes as $node) {
    $node = (array)$node;
    $node['resource_id'] = 'd6_press_release_' .$node['nid'];
    $pressReleaseLoader = new LoadPressRelease();
    $pressReleaseLoader->setHashReset();
    $pressReleaseLoader->load($node);
  }
}


//The links field in d6 has bug where things have been serialezd multiple times
function unserialize_rec($str, $count = 0) {

  if(is_array($str)) {
    return $str;
  }

  //Safety messure, lets only do this 30 times max


  $data = unserialize($str);

  $count++;

  if(is_array($data)) {
    return $data;
  }

  return unserialize_rec($data, $count);

}




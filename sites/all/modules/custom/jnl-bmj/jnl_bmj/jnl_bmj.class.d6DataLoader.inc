<?php

class D6DataLoader extends HWLoad {


  public $resource_id = 'resource_is';
  public $node_type = 'bmj_additional_article_data';


  function getExistingNid() {
    if($nid = highwire_nid_from_apath($this->data['apath'])) {
      $result = db_query("SELECT field_bmj_additional_article_inf_target_id FROM {field_data_field_bmj_additional_article_inf} WHERE entity_id = " . $nid)->fetchObject();
      if($result->field_bmj_additional_article_inf_target_id) {
        return $result->field_bmj_additional_article_inf_target_id;
      } else {
        return false;
      }
    } else {
      return false;
    }
  }

  function assignValues() {
    $node = $this->node;
    $data = $this->data;

    $article_nid = highwire_nid_from_apath($data['apath']);

    $article_node = node_load($article_nid, NULL, TRUE);

    $node->title = $this->cleanTitle('Additional Article Data - ' . $article_node->title);

    $node->field_bmj_override_title = array();

    if(!empty($data['override_title'])) {
      $node->field_bmj_override_title[X][0]['value'] = 1;
    }

    $node->field_bmj_article_intro = array();

    if(!empty($data['intro'])) {
      $node->field_bmj_article_intro[X][0]['value'] = 1;
    }


    if($data['press_release']) {
      $node->field_bmj_press_release[X][0]['nid'] = $data['press_release'];
    }

    $node->field_bmj_display_title = array();

    if(!empty($data['display_title'])) {
      $node->field_bmj_display_title[X][0]['value'] = $data['display_title'];
    } else {
      $node->field_bmj_display_title[X][0]['value'] = $article_node->title;
    }

    $node->field_bmj_us_title = array();

    if(!empty($data['us_title'])) {
      $node->field_bmj_us_title[X][0]['value'] = $data['us_title'];
    }

    $node->field_bmy_countries_local = array();
    if(!empty($data['local'])) {
      $node->field_bmy_countries_local[X][0]['value'] = 1;
    }

    $node->field_bmj_countries = array();
    if(!empty($data['countries'])) {
      foreach ($data['countries'] as $country) {
        $node->field_bmj_countries[X][]['tid'] = $country;
      }
    }

    if(!empty($data['article_links'])) {
      if(is_array($node->field_bmj_links_collection)) {
        foreach ($node->field_bmj_links_collection[X] as $link_col_id) {
          entity_delete('field_collection_item', $link_col_id['value']);
        }
      }

      $node->field_bmj_links_collection = array();

      foreach ($data['article_links'] as $article_link) {
        $collection = entity_create('field_collection_item', array('field_name' => 'field_bmj_links_collection'));
        $collection->setHostEntity('node', $node);
        $collection->field_bmj_article_links[X][0] = array(
          'url' => $article_link['url'],
          'title' => $article_link['title'],
        );
        $collection->field_bmj_short_link_title[X][0]['value'] = $article_link['short_title'];
        $collection->save();
      }
    }


    if(!empty($data['bmjgroup_links'])) {
      if(is_array($node->field_bmjgroup_article_links_col)) {
        foreach ($node->field_bmjgroup_article_links_col[X] as $link_col_id) {
          entity_delete('field_collection_item', $link_col_id['value']);
        }
      }

      $node->field_bmjgroup_article_links_col = array();

      //If we have links at this point, we need to add them
      foreach ($data['bmjgroup_links'] as $article_link) {
        $collection = entity_create('field_collection_item', array('field_name' => 'field_bmjgroup_article_links_col'));
        $collection->setHostEntity('node', $node);
        $collection->field_bmj_group_article_links[X][0] = array(
          'url' => $article_link['url'],
          'title' => $article_link['title'],
        );
        $collection->field_bmj_short_link_title[X][0]['value'] = $article_link['short_title'];
        $collection->save();
      }

    }

    //Need to save the addition info node to the article node
    $article_node->field_bmj_additional_article_inf[X][0]['target_id'] = $node->nid;
    node_save($article_node);

    $this->node = $node;

  }
}
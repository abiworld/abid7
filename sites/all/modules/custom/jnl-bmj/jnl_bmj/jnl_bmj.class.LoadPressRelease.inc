<?php

class LoadPressRelease extends HWLoad {


  public $resource_id = 'resource_id';
  public $node_type = 'bmj_press_release';
  public $primary_key = 'resource_id';



  function getExistingNid() {
    if($resource = highwire_resource_from_resource_id($this->data['resource_id'])) {
      return $resource['id'];
    }
    return false;
  }

  function assignValues() {
    $node = $this->node;
    $data = $this->data;

    $node->title = $data['title'];
    $node->status = $data['status'];

    if($path = path_load('node/'.$node->nid)) {
      path_delete(array('source' => 'node/'.$node->nid));
      $path = array('language' => X, 'source' => 'node/' .$node->nid, 'alias' => $data['path']);
      path_save($path);
    } else {
      $path = array('language' => X, 'source' => 'node/' .$node->nid, 'alias' => $data['path']);
      path_save($path);
    }

    $node->path['pathauto'] = FALSE;

    $node->field_bmj_press_release_body[X][0]['value'] = $data['field_press_release_body'][0]->value;
    $node->field_bmj_press_release_body[X][0]['format'] = 'highwire_full_html';
    $node->field_bmj_press_release_body[X][0]['safe_value'] = $data['field_press_release_body'][0]->value;


  }

  function postprocessItem($item, &$node, $test = FALSE, &$errors) {
    $node->path['pathauto'] = FALSE;


    return TRUE;
  }
}
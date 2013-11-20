<?php

class LoadEletters extends HWLoad {


  public $resource_id = 'resource_id';
  public $node_type = 'highwire_comment';
  public $primary_key = 'resource_id';



  function getExistingNid() {
    if(!empty($this->data['field_highwire_a_apath'][0]->value)) {
      if($nid = highwire_nid_from_apath($this->data['field_highwire_a_apath'][0]->value)) {
        //Check to see if the reasource exists and change the resource_id so we don't get a pdo execeptions
        if($resource = highwire_resource_from_resource_id($this->data['field_highwire_a_apath'][0]->value)) {
          $this->data['resource_id'] = $this->data['field_highwire_a_apath'][0]->value;
        }
        return $nid;
      }
    }

    if($resource = highwire_resource_from_resource_id($this->data['resource_id'])) {
      //Check to see if the node still exists
      if($node = node_load($resource['id'])) {
        return $resource['id'];
      }
    }
    return false;
  }

  function assignValues() {
    $node = $this->node;
    $data = $this->data;

    $node->title = $data['title'];
    $node->field_highwire_a_full_title[X][0]['value'] = $data['title'];

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

    if(!empty($data['field_bmj_rr_address'][0]->value)) {
      $node->field_bmj_rr_address[X][0]['value'] = $data['field_bmj_rr_address'][0]->value;
    }

    if(!empty($data['field_bmj_rr_email'][0]->email)) {
      $node->field_bmj_rr_email[X][0]['email'] = $data['field_bmj_rr_email'][0]->email;
    }

    if(!empty($data['field_bmj_rr_first_name'][0]->value)) {
      $node->field_bmj_rr_first_name[X][0]['value'] = $data['field_bmj_rr_first_name'][0]->value;
    }

    if(!empty($data['field_bmj_rr_last_name'][0]->value)) {
      $node->field_bmj_rr_last_name[X][0]['value'] = $data['field_bmj_rr_last_name'][0]->value;
    }

    if(!empty($data['field_bmj_rr_affiliation'][0]->value)) {
      $node->field_bmj_rr_affiliation[X][0]['value'] = $data['field_bmj_rr_affiliation'][0]->value;
    }

    if(isset($data['field_bmj_rr_agreement'][0]->value)) {
      $node->field_highwire_c_agreement[X][0]['value'] = $data['field_bmj_rr_agreement'][0]->value;
    }

    if(isset($data['field_bmj_rr_no_comp_interests'][0]->value)) {
      $node->field_highwire_c_competing_int[X][0]['value'] = $data['field_bmj_rr_no_comp_interests'][0]->value;
    }

    if(isset($data['field_bmj_rr_competing_interests'][0]->value)) {
      $node->field_highwire_c_competing[X][0]['value'] = $data['field_bmj_rr_competing_interests'][0]->value;
      $node->field_highwire_c_competing[X][0]['safe_value'] = $data['field_bmj_rr_competing_interests'][0]->value;
    }

    if(!empty($data['field_bmj_rr_occupation'][0]->value)) {
      $node->field_bmj_rr_occupation[X][0]['value'] = $data['field_bmj_rr_occupation'][0]->value;
    }

    if(isset($data['field_bmj_rr_other_authors'][0]->value)) {
      $node->field_bmj_rr_other_authors[X][0]['value'] = $data['field_bmj_rr_other_authors'][0]->value;
    }

    if(!empty($data['field_bmj_rr_parent_node'][0]->nid)) {
      //get apaths of parent node
      db_set_active('bmj_d6');
      $result = db_query("SELECT * from {node} where node.nid = :nid", array(':nid' => $data['field_bmj_rr_parent_node'][0]->nid));
      foreach ($result as $row) {
        $parent_type = $row->type;
        $parent_title = $row->title;
        if($parent_type == 'highwire_article') {
          $ap_results = db_query("SELECT * from {content_field_highwire_a_apath}, {node} where node.nid = content_field_highwire_a_apath.nid AND node.nid = :nid", array(':nid' => $data['field_bmj_rr_parent_node'][0]->nid));
          foreach ($ap_results as $ap_row) {
            $ap_row = $row->field_highwire_a_apath_value;
          }
        }
        $parent_nid = $row->nid;
        break;
      }
      db_set_active();
      if($parent_type == 'highwire_article') {
        if(!$apath) {
          highwire_system_message("Missing apath for d6 article " . $data['field_bmj_rr_parent_node'][0]->nid, 'warning');
        } else {
          if($d7_article_nid = highwire_nid_from_apath($apath)){
            $node->field_highwire_c_subject[X][0]['nid'] = $d7_article_nid;
          } else {
            //Let try and load the article if it's missing
            highwire_system_message("Could not find parent article, trying to load it");
            highwire_atomload(array($apath));
            if($d7_article_nid = highwire_nid_from_apath($apath)) {
              $node->field_highwire_c_subject[X][0]['nid'] = $d7_article_nid;
            } else {
              highwire_system_message("Tried to load article, but failed, parent node assigned on comment node " . $node->nid);
            }
          }
        }
      } else if($parent_type == 'portal_page') {
        highwire_system_message("Parent is a portal_page");

        $result = db_query("SELECT * FROM {node} where title = '$parent_title' and type ='bmj_compaign_page'");

        foreach ($result as $row) {
          $campain_parent = $row->nid;
          break;
        }

        if($campain_parent) {
          $node->field_highwire_c_subject[X][0]['nid'] = $campain_parent;
        } else {
          highwire_system_message("Could not find campain parent " .$data['nid'] , 'error');
        }

      }
    } else {
      highwire_system_message("Missing parent nid for d6 nid " .$data['nid'], 'warning');
    }

    if(!empty($data['field_bmj_rr_response_body'][0]->value)) {
      $node->field_highwire_c_response_body[X][0]['value'] = $data['field_bmj_rr_response_body'][0]->value;
      $node->field_highwire_c_response_body[X][0]['safe_value'] = $data['field_bmj_rr_response_body'][0]->value;
    }

    $state_map = array(3 => 'R', 2 => 'E', 0 => 'N');

    if(isset($data['field_bmj_workflow_state'][0]->value)) {
      $node->field_highwire_a_state[X][0]['value'] = $state_map[$data['field_bmj_workflow_state'][0]->value];
    }

    //Didn't see any attachments on any rr's
    if(!empty($data['field_bmj_rr_attachments]'][0])) {
      highwire_system_message("Found attachement on d6 nid " .$data['nid']);
    }

    if(!empty($data['field_highwire_a_epubdate'][0])) {
      $node->field_highwire_a_epubdate[X][0]['value'] = $data['field_highwire_a_epubdate'][0]->value;
      $node->field_highwire_a_epubdate[X][0]['value2'] = $data['field_highwire_a_epubdate'][0]->value2;
    }

    if(!empty($data['field_highwire_a_apath'][0]->value)) {
      $node->field_highwire_a_apath[X][0]['value'] = $data['field_highwire_a_apath'][0]->value;
    }

//     //Contributors
//     $node->field_highwire_a_contributors = array();

//     $contrib_xml = '<atom:author xmlns:atom="http://www.w3.org/2005/Atom" xmlns:nlm="http://schema.highwire.org/NLM/Journal" nlm:contrib-type="publisher"><atom:name>British Medical Journal Publishing Group</atom:name>';

//     if(!empty($data['field_bmj_rr_address']) || !empty($data['field_bmj_rr_email'])) {
//       $contrib_xml .= '<nlm:address>';

//       if(!empty($data['field_bmj_rr_address'])) {
//         $contrib_xml .= '<nlm:addr-line>' . $data['field_bmj_rr_address'][0]->value . '</nlm:addr-line>';
//       }
//       if(!empty($data['field_bmj_rr_email'])) {
//         $contrib_xml .= '<nlm:email>'.$data['field_bmj_rr_email'][0]->value.'</nlm:email>';
//       }
//       $contrib_xml .= '</nlm:address>';
//     }

//     if(!empty($data['field_bmj_rr_first_name']) || !empty($data['field_bmj_rr_last_name'])) {
//       $contrib_xml .= '<nlm:name name-style="western">';

//       if(!empty($data['field_bmj_rr_first_name'])) {
//         $contrib_xml .= '<nlm:given-names>'.$data['field_bmj_rr_first_name'][0]->value.'</nlm:given-names>';
//       }


//       $contrib_xml .= '</nlm:name>';
//     }

//     $contrib_xml .= '</atom:author>';

    //if(!empty())

  }

  function postprocessItem($item, &$node, $test = FALSE, &$errors) {
    $node->path['pathauto'] = FALSE;


    return TRUE;
  }
}
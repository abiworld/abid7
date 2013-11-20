<?php

class LoadBookPages extends HWLoad {


  public $resource_id = 'resource_id';
  public $node_type = 'book';
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

    $node->body[X][0]['value'] = $data['body'];
    $node->body[X][0]['format'] = 'highwire_full_html';
    $node->body[X][0]['safe_value'] = $data['body'];
    
    //Nasty code to grab all images in body and download them
    $images = array();
    preg_match_all('/<img[^>]+>/i', $data['body'], $images);
    
    foreach($images[0] as $img){
    	$srcs = '';
    	preg_match('/src=[\"\'](.+?)[\"\']./', $img, $srcs);
    	foreach($srcs  as $src){
    		
    		$src = str_replace('src=', '', $src);
    		$src = trim(str_replace('"', '', $src));
    		
    		//need ot check path if it's relative
    		$src_info = parse_url($src);
    		
    		//We only care about images from bmj.com
    		if($src_info['host'] == 'www.bmj.com' || empty($src_info['host'])){

    			$uri  =  'public://' . str_replace('/sites/default/files/', '', $src_info['path']);
    			if($src_info['path'][0] != '/') {
    				$path = '/' . $src_info['path'];
    			} else {
    				$path = $src_info['path'];
    			}
    			$path = 'http://www.bmj.com' . $path;
    			
    			//Try and cleanup wierdness
    			if(strpos($src, '../') !== false) {
    				continue;
    			}
    			
    			//If it's not in the files directory, i do not know what to do with it
    			if(strpos($src, 'sites/default') === FALSE) {
    				continue;
    			}
    			
    			highwire_download_external_file($path, $uri, FILE_EXISTS_REPLACE, FALSE);
    		}
    	}
    }
    
    

    if($path = path_load('node/'.$node->nid)) {
      path_delete(array('source' => 'node/'.$node->nid));
      $path = array('language' => X, 'source' => 'node/' .$node->nid, 'alias' => $data['path']);
      path_save($path);
    } else {
      $path = array('language' => X, 'source' => 'node/' .$node->nid, 'alias' => $data['path']);
      path_save($path);
    }

    $node->path['pathauto'] = FALSE;

    $node->field_bmj_attachments = array();

    if($data['top_level']) {
      $node->book['bid'] = $node->nid;
      $node->book['nid'] = $node->nid;
      $node->book['menu_name'] = $data['menu_name'];
      $node->book['plid'] = 0;
    }
   

    if(!empty($data['files'][0])) {
      foreach ($data['files'] as $file) {

        $uri  =  'public://' . str_replace('sites/default/files/', '', $file->filepath);

        $new_file = highwire_download_external_file('http://www.bmj.com/'.$file->filepath, $uri, FILE_EXISTS_REPLACE);

        if($new_file) {
          //Need to set timestamp in the past, stop filefield paths
          db_query("UPDATE file_managed SET timestamp=" . (time()-10000) . ' WHERE fid='.$new_file['fid']);
          $node->field_bmj_attachments[X][] = $new_file;
        } else {
          highwire_system_message("Download failed on " . 'http://www.bmj.com/'.$file->filepath, 'error');
        }

      }
    }

    $this->node = $node;
  }

  function postprocessItem($item, &$node, $test = FALSE, &$errors) {
    $node = node_load($item['nid'], NULL, TRUE);
    $node->path['pathauto'] = FALSE;

    if($item['plid']) {
      $node->book['plid'] = $item['plid'];
    }

    $parents = array_filter($item['extra_data']['parent']);
    if($node->nid != $node->book['bid']) {
      if($item['bid']) {
          $node->book['bid'] = $item['bid'];
          $node->book['nid'] = $item['nid'];
          $node->book['hidden'] = $item['extra_data']['hidden'];
          if($item['extra_data']['data']['weight']) $node->book['weight'] = $item['extra_data']['data']['weight'];
          $node->book['menu_name'] = $item['extra_data']['data']['menu_name'];
          _book_update_outline($node);
      } else {
        highwire_system_message("missing parent " .$item['id']);
        return FALSE;
      }

    } else {
      highwire_system_message("Bid == nid wtf " .$item['id']);
      return FALSE;
    }

    return TRUE;
  }
}
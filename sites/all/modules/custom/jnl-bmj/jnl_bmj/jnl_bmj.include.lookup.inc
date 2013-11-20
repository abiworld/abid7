<?php
/**
 * @file
 * BMJ Include Lookup
 */

/**
 * Auxiliary function to get the custom meta of an article
 */
function bmj_article_get_custom_meta($article){
  if (is_numeric($article))
    $article = node_load($article);
  if ($article->type != 'highwire_article')
    return FALSE;

  $image_lookup = array(
    "fast-track" => "fast-track.jpg",
    "cccme" => "cccme.jpg",
    "making-a-difference" => "making-a-difference.gif",
  );

  $url_lookup = array(
    "fast-track" => "http://resources.bmj.com/bmj/authors/fast-track-publication",
    "cccme" => "http://learning.bmj.com/learning/certprovider.html?action=redirectByDoi&doi=" . $article->field_highwire_article_doi[X][0]['value'],
    "making-a-difference" => "http://makingadifference.bmj.com",
  );

  $custom_meta_array = highwire_extract_custom_meta($article);


  if (isset($custom_meta_array['special-property']) && isset($image_lookup[$custom_meta_array['special-property']])) {
    $custom_meta = $custom_meta_array['special-property'];
    return array(
      'imagename' => $image_lookup[$custom_meta],
      'urlname' => $url_lookup[$custom_meta],
      'meta' => $custom_meta,
    );
  }
  return array();
}

/**
 *  Extract custom-meta information from custom-meta fields
 */
function highwire_extract_custom_meta($article) {
  $results = array();
  foreach ($article->field_highwire_a_cm[X] as $key => $value) {
    $results[$value['first']] = $value['second'];
  }
  return $results;
}

/**
 * Helper function which sends out the details about openaccess
 */
function _bmj_get_open_access_details($node) {
  $open_access_xml = $node->field_highwire_article_perm[X][0]['value'];

  $dom =  new BetterDOMDocument($open_access_xml);

  $open_access_array = $dom->getArray();
  $license_href = !empty($open_access_array['nlm:permissions'][0]['nlm:license']) ? $open_access_array['nlm:permissions'][0]['nlm:license'][0]['@xlink:href'] : '';

  $oa_details = array();
  if (($license_href == "http://creativecommons.org/licenses/") || ($license_href == "http://creativecommons.org/licenses/by-nc/3.0/")) {
    $oa_details['open_access_message'] = 'Free via Creative Commons: Non-commercial use only';
    $oa_details['open_access'] = '<span class="icon-cc">CC</span><span class="icon-by">BY</span><span class="icon-nc">NC</span>';
  }
  elseif ($license_href == "http://creativecommons.org/licenses/by/3.0/") {
    $oa_details['open_access_message'] = 'Free via Creative Commons: Commercial use allowed';
    $oa_details['open_access'] = '<span class="icon-cc">CC</span><span class="icon-by">BY</span>';
  }
  return $oa_details;
}


<?php
class jnl_bmj_handler_filter_not_null_parent extends views_handler_filter {
  function query() {
    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], 'field_highwire_c_subject_nid', NULL, "IS NOT NULL");
  }
}

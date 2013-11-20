<?php
/**
 * Created by PhpStorm.
 * User: piyuesh
 * Date: 05/08/13
 * Time: 3:40 PM
 */
class jnl_bmj_handler_argument_rr_on_parent extends views_handler_argument {
  function query() {
    $this->ensure_my_table();
    $this->query->add_where('bmj', 'field_highwire_c_subject_nid', $this->argument, "=");
  }
}

<?php

/*
 * Highwire citation template for Highwire A style
 *
 * <b>Title of Article<b>
 * Authors
 * journal, Date of publication  vol no issu no p.page numbers
 * link to abstract
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("JCore Title Only"),
  'description' => t('Title of Article'),
  'theme' => 'highwire_citation_style_jcore_title_only',
  'template' => 'plugins/highwire_citation/jcore_title_only',
  'css' => 'plugins/highwire_citation/jcore_title_only.css',
);

<div id="page" class="container <?php print $classes; ?>"<?php print $attributes; ?>>
  <!-- ______________________ HEADER _______________________ -->
  <header>
    <div id="header-region">
      <div class="row-fluid">
        <?php if ($logo): ?>
          <div class="brand">
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
              <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>"/>
            </a>
          </div>
        <?php endif; ?>

        <div class="site-header-togglers">   
          <button data-toggle="collapse" data-target=".nav-collapse2" type="button" class="btn search"><i class="icon-search icon-white"></i></button>        
        </div>
        <div class="searchbar">     
          <div class="collapse nav-collapse2">
            <div class="input-append">
              <?php print $search_block; ?>
                <!-- <input id="appendedInputButton" placeholder="Search BMJ Group" type="text">
                <button class="btn" type="button"><i class="icon-search"></i></button> -->
            </div>
          </div><!--/.nav-collapse2 --> 
        </div> <!--/.searchbar -->
      </div>

      <?php if ($site_name || $site_slogan): ?>
        <div id="name-and-slogan">
          <?php if ($site_name): ?>
            <?php if ($title): ?>
              <div id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
              </div>
            <?php else: /* Use h1 when the content title is empty */ ?>
              <h1 id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
              </h1>
            <?php endif; ?>
          <?php endif; ?>
          <?php if ($site_slogan): ?>
            <div id="site-slogan"><?php print $site_slogan; ?></div>
          <?php endif; ?>
        </div>
      <?php endif; ?>
    </div>
  </header> <!-- /header -->
  
  <section>
    <div id="content">
      <div id="content-inner" class="inner column center">
        <?php if ($page['content_top']): ?>
          <div id="content_top"><?php print render($page['content_top']) ?></div>
        <?php endif; ?>
        <?php if ($breadcrumb || $title || $messages || $tabs || $action_links): ?>
          <div id="content-header">
            <?php print $breadcrumb; ?>
            <!-- Remove heading -->
            <?php print $messages; ?>
            <?php print render($page['help']); ?>
            <?php if ($tabs): ?>
              <div class="tabs"><?php print render($tabs); ?></div>
            <?php endif; ?>
            <?php if ($action_links): ?>
              <ul class="action-links">
                <?php print render($action_links); ?>
              </ul>
            <?php endif; ?>
            <?php if ($page['highlight']): ?>
              <div id="highlight"><?php print render($page['highlight']) ?></div>
            <?php endif; ?>
          </div> <!-- /#content-header -->
        <?php endif; ?>
        <div id="content-area">
          <?php print render($page['content']) ?>
        </div>
        <?php print $feed_icons; ?>
        <?php if ($page['content_bottom']): ?>
          <div id="content_bottom"><?php print render($page['content_bottom']) ?></div>
        <?php endif; ?>
      </div> <!-- /content-inner -->
    </div> <!-- /content -->
  </section>

  
  <?php if(!drupal_is_front_page()):?>
    <nav>
      <div class="page-nave row-fluid">
        <a href="<?php print $front_page; ?>" title="Home" alt="Home" class="btn btn-primary pull-right">
          Home <i class="icon-arrow-right icon-white"></i>
        </a>
      </div>
    </nav>
  <?php endif;?>

  <!-- ______________________ FOOTER _______________________ -->
  <?php if (isset($page['footer'])): ?>
    <footer id="footer">
      <?php print render($page['footer']); ?>
    </footer> <!-- /footer -->
  <?php endif; ?>

</div> <!-- /page -->
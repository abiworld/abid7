(function($) {

Drupal.behaviors.actionEducation = {
  attach: function (context) {
  
    $(window).load(function() {
      window.setTimeout(function() {
        
        Drupal.Education.equalHeight($('#panel-forth-wrapper .grid-inner'))
        
        Drupal.Education.equalHeight2($('#main-content > .grid-inner'), $('#sidebar-home-wrapper > .grid-inner'));
        Drupal.Education.equalHeight($('#main-content > .grid-inner, #sidebar-featured-wrapper > .grid-inner'));
        
        Drupal.Education.equalHeight2(
          $('#panel-second-wrapper .panel-second-1 > .grid-inner'),
          $('#panel-second-wrapper .region-panel-second-2')
        );
        
        Drupal.Education.equalHeight($('#main-content > .grid-inner, #sidebar-second-wrapper > .grid-inner'));
      }, 100);
      
      if ($(".galleryformatter .gallery-slides").length) {
        if($(".galleryformatter .gallery-slides").height() == 0) {
          $(".galleryformatter .gallery-slides").css({height: $(".galleryformatter .gallery-slides img").height() + "px"});
        }
      }
    });
    
    Drupal.Education.centerSlideshowControls();
      
    $(window).resize(function() {
      Drupal.Education.centerSlideshowControls();
      /*
      var jcarousel = $('#panel-first-wrapper .jcarousel').parent();
      jcarousel.css('width', $("#panel-first-wrapper .container .block-views").width() + "px");
      jcarousel.parent().css('width', $("#panel-first-wrapper .container .block-views").width() + "px");
      jcarousel.find('.jcarousel').jcarousel('reload');
      
      var jcarousel = $('#panel-third-wrapper .jcarousel').parent();
      jcarousel.css('width', $("#panel-third-wrapper .container .block-views").width() + "px");
      jcarousel.parent().css('width', $("#panel-third-wrapper .container .block-views").width() + "px");
      jcarousel.find('.jcarousel').jcarousel('reload');
      */
    });
    
    $('.btn-btt').click(function(){
      $('body,html').animate({
        scrollTop: 0
      }, 800);
    });
  }
};

Drupal.Education = Drupal.Education || {};

Drupal.Education.centerSlideshowControls = function() {
  var w = $('#block-views-slideshow-block').width() - $('#widget_pager_bottom_slideshow-block').width();
  $('#widget_pager_bottom_slideshow-block').css('left', w/2 + 'px');
}

Drupal.Education.equalHeight = function(elements) {
  highest = 0;
  elements.each(function() {
    if($(this).innerHeight() > highest) {
      highest = $(this).outerHeight();
    }
  });
  return elements.each(function() {
    extra = $(this).outerHeight() - $(this).height();
    if(($.browser.msie && $.browser.version == 6.0)) {
      $(this).css({'height': highest - extra, 'overflow': 'hidden'});
    }
    else {
      $(this).css({'min-height': highest - extra});
    }
  });
}

Drupal.Education.equalHeight2 = function(elements, exceptedElements) {
  highest = 0;
  elements.each(function() {
    if($(this).innerHeight() > highest) {
      highest = $(this).outerHeight();
    }
  });
  exceptedElements.each(function() {
    if($(this).innerHeight() > highest) {
      highest = $(this).outerHeight();
    }
  });
  return elements.each(function() {
    extra = $(this).outerHeight(true) - $(this).height()  
      //+ parseInt($(this).css('margin-top')) + parseInt($(this).css('margin-bottom'))
    ;
    
    if(($.browser.msie && $.browser.version == 6.0)) {
      $(this).css({'height': highest - extra, 'overflow': 'hidden'});
    }
    else {
      $(this).css({'min-height': highest - extra});
    }
  });
}

})(jQuery);
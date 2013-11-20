(function ($) {
  Drupal.behaviors.openPopover = {
    attach: function(context, settings) {
      $(document).ready(function(){
        $('#article-response').trigger('click');
        $('html,body').animate({scrollTop: $("#article-response").offset().top},'slow');
      });
    }
  };
})(jQuery);

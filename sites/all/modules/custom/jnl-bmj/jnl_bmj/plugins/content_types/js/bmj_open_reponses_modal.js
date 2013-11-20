(function ($) {
  Drupal.behaviors.openResponsePopover = {
    attach: function(context, settings) {
      $(document).ready(function(){
        $('#rapid-response-related').modal();
      });
    }
  };
})(jQuery);

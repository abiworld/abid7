(function ($) {
  Drupal.behaviors.popoverBootstrap = {
    attach: function(context, settings) {
      $('#mini-panel-bmj_rapid_responses_form').children('div.panel-panel').children('div').addClass('modal-body');
      $('#mini-panel-bmj_related_rapid_responses').children('div.panel-panel').children('div').addClass('modal-body');

      $('#article-response', context).once('article-response', function() {
        $(this).popover({
          trigger: 'manual',
          placement: 'bottom',
          html: true,
          content: function() {
            return $('#' + Drupal.settings.jnl_bmj.container_id_response).html();
          }
        }).click(function(event) {
          event.preventDefault();
          event.stopPropagation();
          $("#article-response").popover('toggle');
          $('#article-share').popover('hide');
        });
      });


      $('#article-share', context).once('article-share', function() {
        $(this).popover({
          trigger: 'manual',
          placement: 'bottom',
          html: true,
          content: function() {
            return $('#' + Drupal.settings.jnl_bmj.container_id_share).html();
          }
        }).click(function(event) {
          event.preventDefault();
          event.stopPropagation();
          $("#article-share").popover('toggle');
          $('#article-response').popover('hide');
        });
      });

      $('a.submit-a-e-letter', context).once('submit-a-e-letter', function() {
        $(this).click(function(){
          $('#rapid-response-form').modal('show');
        });
      });

      // $('body').click(function(event) {
      //   $("#article-response").popover('hide');
      //   $('#article-share').popover('hide');
      // });
    }
  }
})(jQuery);
